#include "GaudiKernel/AlgTool.h"
#include "Kernel/ICondDBInfo.h"
#include "XmlTools/IXmlEntityResolver.h"

/// Simple xercesc::EntityResolver that can delegate to other resolvers
class EntityResolverDispatcher : public extends<AlgTool, IXmlEntityResolver, ICondDBInfo>,
                                 virtual public xercesc::EntityResolver
{

  /// implementations of IXmlEntityResolver.
  StringArrayProperty m_resolversNames{"EntityResolvers", {}};
  std::vector<SmartIF<IXmlEntityResolver>> m_resolvers;

public:
  EntityResolverDispatcher( const std::string& type, const std::string& name, const IInterface* parent )
      : base_class( type, name, parent )
  {
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
  }

  StatusCode finalize() override
  {
    m_resolvers.clear();
    return base_class::finalize();
  }

  /// Return a pointer to the actual implementation of a xercesc::EntityResolver.
  xercesc::EntityResolver* resolver() override { return this; }

  /// Create a Xerces-C input source based on the given systemId (publicId is ignored).
  /// If the systemId does not begin with "conddb:", it returns NULL, so the parser go on with the default action.
  xercesc::InputSource* resolveEntity( const XMLCh* const publicId, const XMLCh* const systemId ) override
  {
    xercesc::InputSource* out = nullptr;
    for ( auto& resolver : m_resolvers ) {
      out = resolver->resolver()->resolveEntity( publicId, systemId );
      if ( out ) break;
    }
    return out;
  }

  void defaultTags( std::vector<LHCb::CondDBNameTagPair>& tags ) const
  {
    for ( auto& res : m_resolvers ) {
      SmartIF<ICondDBInfo> cdbInfo{res};
      if ( cdbInfo ) {
        cdbInfo->defaultTags( tags );
      } else {
        auto named                = res.as<INamedInterface>();
        const auto& resolver_name = named ? named->name() : std::string{"unknown"};
        warning() << "cannot get CondDB tags from " << resolver_name << endmsg;
      }
    }
  }
};

DECLARE_COMPONENT( EntityResolverDispatcher )
