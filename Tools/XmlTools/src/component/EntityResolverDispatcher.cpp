/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "GaudiKernel/ParsersFactory.h"

#include "GaudiKernel/AlgTool.h"
#include "Kernel/ICondDBInfo.h"
#include "XmlTools/IXmlEntityResolver.h"

#include <regex>

#include <xercesc/util/XMLString.hpp>

namespace {
  struct XMLStrDeleter {
    void operator()( char* c ) { xercesc::XMLString::release( &c ); }
    void operator()( XMLCh* c ) { xercesc::XMLString::release( &c ); }
  };
} // namespace

/// Simple xercesc::EntityResolver that can delegate to other resolvers
class EntityResolverDispatcher : public extends<AlgTool, IXmlEntityResolver, ICondDBInfo>,
                                 virtual public xercesc::EntityResolver {

  StringArrayProperty                                              m_resolversNames{"EntityResolvers", {}};
  SimpleProperty<std::vector<std::pair<std::string, std::string>>> m_mappingsDesc{"Mappings", {}};

  std::vector<SmartIF<IXmlEntityResolver>>               m_resolvers;
  std::vector<std::pair<std::regex, const std::string&>> m_mappings;

public:
  EntityResolverDispatcher( const std::string& type, const std::string& name, const IInterface* parent )
      : base_class( type, name, parent ) {
    declareProperty( m_resolversNames.name(), m_resolversNames );
    m_resolversNames.declareUpdateHandler( [this]( Property& ) {
      m_resolvers.clear();
      for ( const auto& name : m_resolversNames.value() ) {
        IXmlEntityResolver* tmp = nullptr;
        if ( !toolSvc()->retrieveTool( name, tmp ) )
          throw GaudiException( "failed to retrieve tool " + name, this->name(), StatusCode::FAILURE );
        m_resolvers.emplace_back( tmp );
      }
    } );

    declareProperty( m_mappingsDesc.name(), m_mappingsDesc,
                     "list of pairs of regex and substitution patterns to redirect requests" );
    m_mappingsDesc.declareUpdateHandler( [this]( Property& ) {
      m_mappings.clear();
      for ( const auto& mapping : m_mappingsDesc.value() ) {
        if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
          debug() << "defined mapping \"" << mapping.first << "\" -> \"" << mapping.second << "\"" << endmsg;
        m_mappings.emplace_back( std::regex{mapping.first}, mapping.second );
      }
    } );
  }

  StatusCode finalize() override {
    m_resolvers.clear();
    return base_class::finalize();
  }

  /// Return a pointer to the actual implementation of a xercesc::EntityResolver.
  xercesc::EntityResolver* resolver() override { return this; }

  /// Create a Xerces-C input source based on the given systemId (publicId is ignored).
  /// If the systemId does not begin with "conddb:", it returns NULL, so the parser go on with the default action.
  xercesc::InputSource* resolveEntity( const XMLCh* const publicId, const XMLCh* const systemId ) override {
    std::unique_ptr<XMLCh, XMLStrDeleter> tmpSysId;
    if ( !m_mappings.empty() ) {
      std::unique_ptr<char, XMLStrDeleter> tmp1{xercesc::XMLString::transcode( systemId )};
      std::string                          tmp2{tmp1.get()};
      for ( const auto& mapping : m_mappings ) { tmp2 = std::regex_replace( tmp2, mapping.first, mapping.second ); }
      tmpSysId.reset( xercesc::XMLString::transcode( tmp2.c_str() ) );
      *const_cast<XMLCh**>( &systemId ) = tmpSysId.get();
      if ( UNLIKELY( msgLevel( MSG::DEBUG ) && tmp2.compare( tmp1.get() ) ) )
        debug() << "mapped " << tmp1.get() << " to " << tmp2 << endmsg;
    }
    xercesc::InputSource* out = nullptr;
    for ( auto& resolver : m_resolvers ) {
      out = resolver->resolver()->resolveEntity( publicId, systemId );
      if ( out ) break;
    }
    return out;
  }

  void defaultTags( std::vector<LHCb::CondDBNameTagPair>& tags ) const override {
    for ( auto& res : m_resolvers ) {
      SmartIF<ICondDBInfo> cdbInfo{res};
      if ( cdbInfo ) {
        cdbInfo->defaultTags( tags );
      } else {
        auto        named         = res.as<INamedInterface>();
        const auto& resolver_name = named ? named->name() : std::string{"unknown"};
        warning() << "cannot get CondDB tags from " << resolver_name << endmsg;
      }
    }
  }
};

DECLARE_COMPONENT( EntityResolverDispatcher )
