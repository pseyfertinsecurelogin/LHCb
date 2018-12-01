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
#include <string>
#include <map>

// Define the parser
#include "GaudiKernel/ParsersFactory.h"

namespace {
   using additionalIDs_t = std::map<std::string, std::map<std::string, unsigned int>>;
}

namespace Gaudi {
namespace Parsers {

// Parser grammar
template <typename Iterator, typename Skipper>
struct Grammar_<Iterator, additionalIDs_t, Skipper > {
   // In this case, the type is a mapping type, so it requires the MapGrammar.
   // For other grammars see GaudiKernel/GrammarsV2.h
   typedef MapGrammar<Iterator, ::additionalIDs_t, Skipper> Grammar;
};

// Additional parser for our property
StatusCode parse(additionalIDs_t& result, const std::string& input) {
    return Gaudi::Parsers::parse_(result, input);
}
}
}

// We also need to be able to print an object of our type as a string that both
// Python and our parser can understand,
#include "GaudiKernel/ToStream.h"
namespace std {
  // This is an example valid for any mapping type.
  ostream& operator<<(ostream& s, const additionalIDs_t& m) {
    bool first = true;
    s << '{';
    for(const auto& i: m) {
      if (first) first = false;
      else s << ", ";
      Gaudi::Utils::toStream(i.first, s) << ": ";
      Gaudi::Utils::toStream(i.second, s);
    }
    s << '}';
    return s;
  }
}

#include "GaudiKernel/Service.h"
#include "GaudiKernel/VectorMap.h"

#include "Kernel/IIndexedANNSvc.h"
#include "Kernel/IPropertyConfigSvc.h"
#include "Kernel/TCK.h"


class TCKANNSvc : public extends<Service,IIndexedANNSvc>  {

public:
  using extends::extends;

  StatusCode initialize() override;
  StatusCode finalize() override;

  GaudiUtils::VectorMap<unsigned int, Gaudi::StringKey> i2s(unsigned int index, const Gaudi::StringKey& major) const override;

private:

  // properties
  Gaudi::Property<additionalIDs_t>  m_additionals {this, "AdditionalIDs" };

  mutable SmartIF<IPropertyConfigSvc>  m_propertyConfigSvc;
  Gaudi::Property<std::string>       m_propertyConfigSvcName { this, "IPropertyConfigSvcInstance", "PropertyConfigSvc" };
  Gaudi::Property<std::string>       m_instanceName { this, "InstanceName",  "HltANNSvc" };
  mutable std::map<TCK,const PropertyConfig*> m_cache; // TODO: flush cache if m_instanceName changes
};

DECLARE_COMPONENT( TCKANNSvc )

StatusCode
TCKANNSvc::initialize()
{
  StatusCode sc = Service::initialize();
  if (sc.isFailure()) {
    fatal() << "Service::initialize() failed!!!"<< endmsg;
    return sc;
  }

  if( msgLevel(MSG::VERBOSE) ) verbose() << "==> Initialize" << endmsg;

  m_propertyConfigSvc = service( m_propertyConfigSvcName);
  if (!m_propertyConfigSvc) {
    fatal() << "TCKANNSvc failed to get the IConfigAccessSvc." << endmsg;
    return StatusCode::FAILURE;
  }

  if (!m_additionals.empty()) {
     warning() << "Additional IDs specified for " << m_instanceName << "." << endmsg
               << "This is an expert options, and not for general use." << endmsg;
  }
  return sc;
}

StatusCode
TCKANNSvc::finalize(  )
{
  m_propertyConfigSvc.reset();
  return Service::finalize();
}


GaudiUtils::VectorMap<unsigned int, Gaudi::StringKey>
TCKANNSvc::i2s(unsigned int tck, const Gaudi::StringKey& major) const
{
    TCK _tck(tck); _tck.normalize();
    auto entry = m_cache.find(_tck);
    if (entry == std::end(m_cache) ) {
        // grab properties of child from config database...
        const ConfigTreeNode* tree = m_propertyConfigSvc->resolveConfigTreeNode( ConfigTreeNodeAlias::alias_type{ std::string("TCK/") +  _tck.str()  } );
        if (!tree) {
          //If we could not resolve the (non-zero) TCK we have a problem
          error() << "Requested TCK " << _tck <<  " could not resolved. Returning an empty map... " << endmsg;
          return {};
        }
        PropertyConfig::digest_type child = m_propertyConfigSvc->findInTree(tree->digest(), m_instanceName);
        if (child.invalid()) {
          error() << "Error finding configuration of " << m_instanceName << " for TCK " << _tck
                  <<  " Returning an empty map... " << endmsg;
          return {};
        }
        const PropertyConfig *config = m_propertyConfigSvc->resolvePropertyConfig(child);
        if (!config) {
          error() << "Error reading configuration of " << m_instanceName << " for TCK " << _tck
                  <<  " Returning an empty map... " << endmsg;
          return {};
        }
        auto status = m_cache.insert( { _tck, config } );
        if (!status.second) {
          error() << "Error updating cache for TCK " << _tck <<  " Returning an empty map... " << endmsg;
          return {};
        }
        entry = status.first;
    }
    auto prop = std::find_if( std::begin(entry->second->properties()), std::end( entry->second->properties() ),
                              [&](const std::pair<std::string,std::string>& p ) { return major.str() == p.first ; } );
    if (prop == std::end(entry->second->properties())) {
      error() << "Error finding requested major " << major <<" in  configuration of " << m_instanceName
              << " for TCK " << _tck <<  " Returning an empty map... " << endmsg;
      return {};
    }
    // now use the property parser to do the hard work of converting the string rep into a map...
    std::map<std::string,int> map;
    // or extend the PropertyConfig interface so it takes a property, invokes 'fromString' on it, and returns StatusCode ?
    // or an templated interface, which wraps a PropertyWithValue around it, then invokes the above, and drops the PropertyWithValue?
    // std::optional< map<string,int> > = config->assign< map<string,int> >( major );
    // PropertyWithValue<std::map<std::string,int>>{  major.str(), &map, false }.fromString( prop->second );
    // auto status = prp.fromString( prop->second );
    auto status = Gaudi::Parsers::parse(map , prop->second);
    if (status.isFailure()) {
      error() << "Error interpreting requested major " << major <<" in  configuration of " << m_instanceName
              << " for TCK " << _tck <<  " Returning an empty map... " << endmsg;
      return {};
    }
    GaudiUtils::VectorMap<unsigned int, Gaudi::StringKey> result;
    auto additionals = m_additionals.find(major.str());
    bool ok = true;
    if (additionals != end(m_additionals)) {
       for (const auto& item : additionals->second) {
          ok = ok && result.insert(item.second, item.first).second;
       }
    }
    if (!ok) {
      error() << "Duplicate entries in additional map for  major " << major <<" in  configuration of "
              << m_instanceName << " for TCK "
              << _tck <<  " Returning an empty map... " << endmsg;
      return {};
    }
    for ( const auto& item : map )  {
        ok = ok && result.insert( item.second, item.first ).second;
    }
    if (!ok) {
      error() << "Duplicate entries in map for  major " << major <<" in  configuration of " << m_instanceName
              << " for TCK " << _tck <<  " Returning an empty map... " << endmsg;
      return {};
    }
    return result;
}
