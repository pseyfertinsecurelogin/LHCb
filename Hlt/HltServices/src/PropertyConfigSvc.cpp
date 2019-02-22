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
// Include files

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iterator>
#include <list>
#include <regex>
#include <sstream>

// local
#include "PropertyConfigSvc.h"

// from Gaudi
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/IAlgManager.h"
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/IAuditor.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/System.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PropertyConfigSvc
//
// 2007-10-24 : Gerhard Raven
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( PropertyConfigSvc )

namespace {
  template <typename T>
  auto equals( T&& t ) {
    return [arg0 = std::forward<T>( t )]( const auto& arg ) { return arg0 == arg; };
  }

  // model this as output iterator
  class property2jos : public std::iterator<std::output_iterator_tag, const PropertyConfig::Prop> {
  public:
    property2jos& operator++() { return *this; }
    property2jos& operator*() { return *this; }
    property2jos( IJobOptionsSvc* jos, const std::string& name, std::ostream* os = nullptr )
        : m_jos( jos ), m_name( name ), m_properties( jos->getProperties( name ) ), m_out( os ) {
      assert( m_jos );
    }
    property2jos& operator=( const PropertyConfig::Prop& prop ) {
      if ( m_out ) { *m_out << m_name << '.' << prop.first << '=' << prop.second << ";\n"; }
      const Property* p = ( m_properties != nullptr ? find( prop.first ) : nullptr );
      if ( p == nullptr || p->toString() != prop.second ) { // only update if non-existant or not up-to-date
                                                            // resolve references
        // this is done in order to support the online use case, where we have options
        // of the form foo.something = @OnlineEnv.somethingElse
        // WARNING: values are substituted when seen, so there is a possibility that they are
        //          substituted just prior to the reference changing -- this is not the case
        //          in the online scenario, as the reference come from 'static' items, _not_
        //          configured through this service!!!
        // NOTE: if the reference is not found, optionally, a default, can be specified.
        //       eg.  foo.something = @OnlineEnv.somethingelse@somedefault
        if ( !prop.second.empty() && prop.second[0] == '@' ) {
          static std::regex pattern( "^@([^\\.]+)\\.([^@]+)(@.+)?$" );
          std::smatch       what;
          if ( !std::regex_match( prop.second, what, pattern ) ) {
            throw GaudiException( prop.second, "badly formatted reference property ", StatusCode::FAILURE );
          }
          std::string     value;
          const Property* refProp =
              Gaudi::Utils::getProperty( m_jos->getProperties( std::string( what[1] ) ), std::string( what[2] ) );
          if ( refProp ) {
            value = refProp->toString();
          } else if ( what[3].first != what[3].second ) {
            value = std::string( what[3] ).substr( 1 );
          } else {
            throw GaudiException( prop.second, "failed to find reference property ", StatusCode::FAILURE );
          }
          m_jos->addPropertyToCatalogue( m_name, StringProperty( prop.first, value ) );
          // Q: we do not chase references (to references, to ... ) -- should we?
          // A: no, as the JobOptionsSvc parser substitutes values, so what we get back from
          //    the jos NEVER has references in it...
        } else {
          m_jos->addPropertyToCatalogue( m_name, StringProperty( prop.first, prop.second ) );
        }
      }
      return *this;
    }

  private:
    const Property* find( std::string_view name ) {
      auto i = find_if( begin( *m_properties ), end( *m_properties ),
                        [name]( const Property* p ) { return p->name() == name; } );
      return i != end( *m_properties ) ? *i : nullptr;
    }
    IJobOptionsSvc*                     m_jos;
    std::string                         m_name;
    const std::vector<const Property*>* m_properties;
    std::ostream*                       m_out;
  };
} // namespace

//=============================================================================
// Initialization
//=============================================================================
StatusCode PropertyConfigSvc::initialize() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "Initialize" << endmsg;
  StatusCode status = Service::initialize();
  if ( !status.isSuccess() ) return status;
  m_accessSvc = service( s_accessSvc );
  if ( !m_accessSvc ) return StatusCode::FAILURE;
  m_algMgr = service( "ApplicationMgr" );
  if ( !m_algMgr ) return StatusCode::FAILURE;
  m_appMgrUI = m_algMgr;
  if ( !m_appMgrUI ) return StatusCode::FAILURE;
  m_joboptionsSvc = service( "JobOptionsSvc" );
  if ( !m_joboptionsSvc ) return StatusCode::FAILURE;
  m_toolSvc = service( "ToolSvc" );
  if ( !m_toolSvc ) return StatusCode::FAILURE;
  m_toolSvc->registerObserver( this );

  if ( !m_ofname.empty() ) m_os = std::make_unique<std::ofstream>( m_ofname.value() );

  // read table of pre-assigned, possible configurations for this job...
  // i.e. avoid reading _everything_ when we really need to be quick
  for ( const auto& i : m_prefetch ) {
    auto digest = ConfigTreeNode::digest_type::createFromStringRep( i );
    assert( digest.str() == i );
    if ( loadConfig( digest ).isFailure() ) {
      error() << " failed to load " << digest << endmsg;
      return StatusCode::FAILURE;
    }
    if ( m_createGraphVizFile ) createGraphVizFile( digest, digest.str() );
  }
  return status;
}

void PropertyConfigSvc::onCreate( const IAlgTool* tool ) {
  assert( tool != nullptr );
  auto name = tool->name();
  if ( msgLevel( MSG::DEBUG ) ) debug() << "adding " << name << " to toolmap " << endmsg;
  m_toolmap.emplace( name, const_cast<IAlgTool*>( tool ) );
}

//=============================================================================
// Finalization
//=============================================================================
StatusCode PropertyConfigSvc::finalize() {
  StatusCode status = Service::finalize();
  m_joboptionsSvc.reset();
  m_algMgr.reset();
  m_accessSvc.reset();
  m_os.reset();
  return status;
}

template <>
IService* PropertyConfigSvc::resolve( const std::string& name ) const {
  IService* i{nullptr};
  return serviceLocator()->getService( name, i ).isSuccess() ? i : nullptr;
}
template <>
IAlgorithm* PropertyConfigSvc::resolve( const std::string& name ) const {
  IAlgorithm* i{nullptr};
  return m_algMgr->getAlgorithm( name, i ).isSuccess() ? i : nullptr;
}

template <>
IAlgTool* PropertyConfigSvc::resolve( const std::string& name ) const {
  auto i = m_toolmap.find( name );
  return i != m_toolmap.end() ? i->second : nullptr;
}

template <>
Algorithm* PropertyConfigSvc::resolve( const std::string& name ) const {
  return dynamic_cast<Algorithm*>( resolve<IAlgorithm>( name ) );
}
template <>
Service* PropertyConfigSvc::resolve( const std::string& name ) const {
  return dynamic_cast<Service*>( resolve<IService>( name ) );
}
template <>
AlgTool* PropertyConfigSvc::resolve( const std::string& name ) const {
  return dynamic_cast<AlgTool*>( resolve<IAlgTool>( name ) );
}

StatusCode PropertyConfigSvc::invokeSetProperties( const PropertyConfig& conf ) const {
  info() << " calling SetProperties for " << conf.name() << endmsg;
  if ( conf.kind() == "IAlgorithm" ) {
    Algorithm* alg = resolve<Algorithm>( conf.name() );
    return alg ? alg->setProperties() : StatusCode::FAILURE;
  }
  if ( conf.kind() == "IService" ) {
    Service* svc = resolve<Service>( conf.name() );
    return svc ? svc->setProperties() : StatusCode::FAILURE;
  }
  if ( conf.kind() == "IAlgTool" ) {
    AlgTool* tool = resolve<AlgTool>( conf.name() );
    return tool ? tool->setProperties() : StatusCode::FAILURE;
  }
  return StatusCode::FAILURE;
}

//=============================================================================
// implemenation of the IPropertyConfigSvc interface:
//=============================================================================
PropertyConfig PropertyConfigSvc::currentConfiguration( const INamedInterface& obj ) const {
  // TODO: move the next blob into PropertyConfig c'tor itself???
  SmartIF<INamedInterface> ini = const_cast<INamedInterface*>( &obj ); // we do treat obj logically const,
                                                                       // even if we call code which seems
                                                                       // wants non-const version of obj

  // figure out whether we have a Service, Tool, Algorithm or Auditor...
  auto kind = ( ini.as<IAlgorithm>()
                    ? PropertyConfig::kind_t::IAlgorithm
                    : ini.as<IService>() ? PropertyConfig::kind_t::IService
                                         : ini.as<IAlgTool>() ? PropertyConfig::kind_t::IAlgTool
                                                              : ini.as<IAuditor>() ? PropertyConfig::kind_t::IAuditor
                                                                                   : PropertyConfig::kind_t::Invalid );

  return PropertyConfig( obj.name(), *ini.as<IProperty>(), kind );
}

PropertyConfig::digest_type PropertyConfigSvc::findInTree( const ConfigTreeNode::digest_type& configTree,
                                                           const std::string&                 name ) const {
  for ( const auto& i : collectLeafRefs( configTree ) ) {
    const PropertyConfig* pc = resolvePropertyConfig( i );
    assert( pc != nullptr );
    if ( pc->name() == name ) return i;
  }
  return PropertyConfig::digest_type::createInvalid();
}

//=============================================================================
// Walk configuration trees (& cache the result)
//=============================================================================

ConfigTreeNode::digest_type PropertyConfigSvc::resolveAlias( const ConfigTreeNodeAlias::alias_type& alias ) const {
  auto digest = m_aliases.with_lock( [&]( const ConfigTreeNodeAliasMap_t& aliases ) {
    auto i = aliases.find( alias );
    return i != aliases.end() ? i->second : ConfigTreeNode::digest_type::createInvalid();
  } );
  if ( digest.valid() ) return digest;

  auto node = m_accessSvc->readConfigTreeNodeAlias( alias );
  if ( !node ) {
    error() << " could not obtain alias " << alias << endmsg;
    return ConfigTreeNode::digest_type::createInvalid();
  }
  // add it into alias cache
  auto ref = node->digest();
  m_aliases.with_lock( [&]( ConfigTreeNodeAliasMap_t& aliases ) { aliases.emplace( alias, ref ); } );
  m_nodes.with_lock( [&]( ConfigTreeNodeMap_t& nodes ) {
    // add to ConfigTreeNode cache now that we got it anyway...
    if ( nodes.find( ref ) == nodes.end() ) nodes.emplace( ref, *node );
  } );
  info() << " resolved " << alias << " to " << ref << endmsg;
  return ref;
}

const std::list<ConfigTreeNode::digest_type>&
PropertyConfigSvc::collectNodeRefs( const ConfigTreeNodeAlias::alias_type& alias ) const {
  return collectNodeRefs( resolveAlias( alias ) );
}

const PropertyConfigSvc::Tree2NodeMap_t::mapped_type&
PropertyConfigSvc::collectNodeRefs( const ConfigTreeNode::digest_type& nodeRef ) const {
  // first check cache...
  auto ptr = m_nodesInTree.with_lock( [&]( const Tree2NodeMap_t& nodesInTree ) {
    auto j = nodesInTree.find( nodeRef );
    return j != nodesInTree.end() ? &j->second : nullptr;
  } );
  if ( ptr ) return *ptr;

  Tree2NodeMap_t::mapped_type nrefs; // note: we use list, as appending to a list
                                     // does not invalidate iterators!!!!
  nrefs.push_back( nodeRef );
  for ( auto i = begin( nrefs ); i != end( nrefs ); ++i ) { // WARNING: end(nrefs) updates in the body of the loop!!!
    const ConfigTreeNode* node = resolveConfigTreeNode( *i );
    if ( !node ) {
      throw GaudiException( "failed to resolve node ", this->name() + "::collectNodeRefs", StatusCode::FAILURE );
    }
    // FIXME: this is not going to be very fast, as it going to
    // make this operation quadratic...  should keep list sorted..
    // but if we do, we need to 'restart' at the insertion point
    // to insure the dependencies of the inserted elements are
    // taken into account... On top of that, the current linear
    // structure is sorted by dependency, which is kind of nice ;-)
    std::copy_if( begin( node->nodes() ), end( node->nodes() ), std::back_inserter( nrefs ),
                  [&nrefs]( Tree2NodeMap_t::mapped_type::const_reference k ) {
                    return std::none_of( begin( nrefs ), end( nrefs ), equals( k ) );
                  } );
  }
  // insert in cache
  // we may be beaten to this point, and nodeRef may already be there,
  // but that is OK / irrelevant ;-)
  return m_nodesInTree.with_lock( [&]( Tree2NodeMap_t& nodesInTree ) -> decltype( auto ) {
    auto r = nodesInTree.emplace( nodeRef, std::move( nrefs ) );
    return ( r.first->second ); // note: the () matter!
  } );
}

const std::vector<PropertyConfig::digest_type>&
PropertyConfigSvc::collectLeafRefs( const ConfigTreeNodeAlias::alias_type& alias ) const {
  return collectLeafRefs( resolveAlias( alias ) );
}

const std::vector<PropertyConfig::digest_type>&
PropertyConfigSvc::collectLeafRefs( const ConfigTreeNode::digest_type& nodeRef ) const {
  auto ptr = m_leavesInTree.with_lock( [&]( const Tree2LeafMap_t& leavesInTree ) {
    auto i = leavesInTree.find( nodeRef );
    return i != leavesInTree.end() ? &i->second : nullptr;
  } );
  if ( ptr ) return *ptr;
  std::vector<PropertyConfig::digest_type> leafRefs;
  for ( auto& i : collectNodeRefs( nodeRef ) ) {
    const ConfigTreeNode*       node    = resolveConfigTreeNode( i );
    PropertyConfig::digest_type leafRef = node->leaf();
    if ( leafRef.valid() ) leafRefs.push_back( leafRef );
  }
  return m_leavesInTree.with_lock( [&]( Tree2LeafMap_t& leavesInTree ) -> decltype( auto ) {
    auto rv = leavesInTree.emplace( nodeRef, std::move( leafRefs ) );
    return ( rv.first->second ); // note: the () matter!
  } );
}

StatusCode PropertyConfigSvc::loadConfig( const ConfigTreeNode::digest_type& nodeRef ) {
  try {
    if ( !nodeRef.valid() ) return StatusCode::FAILURE;
    info() << "loading config " << nodeRef.str() << endmsg;
    for ( auto& i : collectLeafRefs( nodeRef ) ) resolvePropertyConfig( i );
    return validateConfig( nodeRef );
  } catch ( const std::exception& x ) {
    error() << x.what() << endmsg;
    throw x;
  }
  return StatusCode::FAILURE;
}

//=============================================================================
// Configure
//=============================================================================
StatusCode PropertyConfigSvc::configure( const ConfigTreeNodeAlias::alias_type& alias, bool callSetProperties ) const {
  info() << " configuring using " << alias << endmsg;
  return configure( resolveAlias( alias ), callSetProperties );
}

StatusCode
PropertyConfigSvc::outOfSyncConfigs( const ConfigTreeNode::digest_type&                            configID,
                                     std::back_insert_iterator<std::vector<const PropertyConfig*>> newConfigs ) const {
  assert( configID.valid() ); // caller already verified this!
  for ( const auto& i : collectLeafRefs( configID ) ) {
    const PropertyConfig* config = resolvePropertyConfig( i );
    if ( !config ) {
      error() << "Failed to resolve " << i << endmsg;
      return StatusCode::FAILURE;
    }
    if ( m_skip.find( config->name() ) != end( m_skip ) ) {
      warning() << " skipping configuration of " << config->name() << " because it is in the 'skip' list" << endmsg;
      continue;
    }
    auto current = m_configPushed.find( config->name() );

    // WARNING: the following doesn't catch all flow changes
    //          as it fails to recognize that we've dropped some
    //          part of the configuration tree... it only recognizes
    //          additional new branches...
    if ( current == m_configPushed.end() && !m_configPushed.empty() ) { // new, after already having pushed
      warning() << " WARNING WARNING WARNING WARNING WARNING WARNING" << endmsg;
      warning() << " WARNING  new configuration has entry not in old configuration" << endmsg;
      warning() << " WARNING  " << config->name() << endmsg;
      warning() << " WARNING  this transition will most likely fail to do what you want it to do... " << endmsg;
      warning() << " WARNING WARNING WARNING WARNING WARNING WARNING\n" << endmsg;
      if ( !m_allowFlowChanges ) return StatusCode::FAILURE;
    }

    if ( current == m_configPushed.end() || current->second != i ) { // not present, or different; needs to be pushed
      if ( msgLevel( MSG::DEBUG ) )
        debug() << " " << config->name() << " current: "
                << ( current == m_configPushed.end() ? PropertyConfig::digest_type::createInvalid() : current->second )
                << " requested: " << i << endmsg;
      *newConfigs = config;
    }
  }
  // TODO: add check for 'dropped' parts of the configuration tree...
  return StatusCode::SUCCESS;
}

// TODO: add a locking mechanism that, after the first configure, forbids new entries,
//      i.e. only allow pushing changes to existing entries, i.e. m_configPushed must
//      be a valid digest prior to pushing...
//      This would basically forbid changes in flow, as any added 'member' in an existing
//      sequencer would not have its properties pushed yet, but will in the config, so it
//      would be caught when trying to push the properties of the new algorithm.
StatusCode PropertyConfigSvc::configure( const ConfigTreeNode::digest_type& configID, bool callSetProperties ) const {
  // TODO: make thread safe!
  //      move chcek on m_configPushed out of  outOfSyncConfigs
  //      to avoid reading m_configPushed there, and restrict read/write
  //      of m_configPushed to configure only!
  if ( msgLevel( MSG::DEBUG ) ) debug() << " configuring using " << configID << endmsg;
  if ( !configID.valid() ) return StatusCode::FAILURE;
  setTopAlgs( configID ); // do this last instead of first?
  std::vector<const PropertyConfig*> configs;
  if ( auto sc = outOfSyncConfigs( configID, std::back_inserter( configs ) ); sc.isFailure() ) return sc;
  for ( const auto& i : configs ) {
    if ( msgLevel( MSG::DEBUG ) ) debug() << " configuring " << i->name() << " using " << i->digest() << endmsg;
    const PropertyConfig::Properties& map = i->properties();

    // TODO: make sure that online this cannot be done...
    std::string fqname = i->type() + "/" + i->name();
    Transformer transformer( fqname, warning() );
    for ( const auto& trans : m_transform ) {
      if ( std::regex re( trans.first ); std::regex_match( fqname, re ) ) transformer.push_back( &trans.second );
    }
    if ( !transformer.empty() ) {
      std::transform( begin( map ), end( map ), property2jos( m_joboptionsSvc, i->name(), m_os.get() ), transformer );
    } else {
      std::copy( begin( map ), end( map ), property2jos( m_joboptionsSvc, i->name(), m_os.get() ) );
    }
    m_configPushed[i->name()] = i->digest();
  }
  //  _after_ pushing all configurations, invoke 'setProperties'...
  //@TODO: should we do this in reverse order??
  if ( callSetProperties ) {
    for ( const auto& i : configs ) {
      if ( auto sc = invokeSetProperties( *i ); sc.isFailure() ) {
        error() << "failed whilst invoking setProperties for " << i->name() << endmsg;
        return sc;
      }
    }
  }
  return StatusCode::SUCCESS;
}

//=============================================================================
// Reconfigure & Reinitialize
//=============================================================================
StatusCode
PropertyConfigSvc::findTopKind( const ConfigTreeNode::digest_type& configID, std::string_view kind,
                                std::back_insert_iterator<std::vector<const PropertyConfig*>> configs ) const {
  const ConfigTreeNode*       node = resolveConfigTreeNode( configID );
  PropertyConfig::digest_type id   = node->leaf();
  if ( id.invalid() ) {
    // only follow dependencies if no data at current node....
    StatusCode sc( StatusCode::SUCCESS );
    for ( const auto& i : node->nodes() ) {
      sc = findTopKind( i, kind, configs );
      if ( !sc.isSuccess() ) break;
    }
    return sc;
  }
  // we (should) have a leaf ! get it and use it!!!
  const PropertyConfig* config = resolvePropertyConfig( id );
  if ( !config ) {
    if ( msgLevel( MSG::DEBUG ) ) debug() << " could not find " << id << endmsg;
    error() << " could not find a configuration ID" << endmsg;
    return StatusCode::FAILURE;
  }
  if ( config->kind() == kind ) *configs = config;
  return StatusCode::SUCCESS;
}

StatusCode PropertyConfigSvc::setTopAlgs( const ConfigTreeNode::digest_type& id ) const {

  // Obtain the IProperty of the ApplicationMgr
  SmartIF<IProperty>  appProps( serviceLocator() );
  StringArrayProperty topAlgs( "TopAlg", std::vector<std::string>() );
  if ( appProps->getProperty( &topAlgs ).isFailure() ) {
    error() << " problem getting StringArrayProperty \"TopAlg\"" << endmsg;
  }
  if ( msgLevel( MSG::DEBUG ) ) debug() << " current TopAlgs: " << topAlgs.toString() << endmsg;

  if ( !m_initialTopAlgs ) m_initialTopAlgs = topAlgs.value();

  std::vector<const PropertyConfig*> ids;
  StatusCode                         sc = findTopKind( id, "IAlgorithm", std::back_inserter( ids ) );
  if ( sc.isFailure() ) return sc;
  std::list<std::string> request;
  std::transform( begin( ids ), end( ids ), std::back_inserter( request ),
                  []( const PropertyConfig* i ) { return i->fullyQualifiedName(); } );

  // merge the current TopAlg, and requested TopAlg list, conserving
  // the order of BOTH lists.
  // We first loop over the to-be-inserted algos. As soon as one of them
  // is already present, we push everyone 'up to' this algo just in front
  // of it. Next we repeat until done, checking that the next requested algo is not
  // already in the current list prior to the point we're at...
  //@TODO: this doesn't work if topAlgs change on a TCK change: the old ones not
  //       present in the new one will stay..
  //@TODO: find topAlgs before we touched them -- and use those to merge with...

  std::list<std::string> merge( begin( *m_initialTopAlgs ), end( *m_initialTopAlgs ) );

  auto ireq = request.begin();
  while ( ireq != request.end() ) {
    auto i = find( merge.begin(), merge.end(), *ireq );
    // TODO: make sure we don't go backward in i...
    // example: topalgs = [ A, B, C ]
    //          merge = [ X,Y,B,Z,A ] should give error on meeting A
    if ( i != merge.end() ) {
      merge.splice( i, request, request.begin(), ireq );
      request.erase( ireq++ );
    } else {
      ++ireq;
    }
  }
  merge.splice( merge.end(), request );
  assert( request.empty() );

  bool hasChanged = ( merge.size() != topAlgs.value().size() );
  if ( !hasChanged ) {
    auto diff  = std::mismatch( topAlgs.value().begin(), topAlgs.value().end(), merge.begin() );
    hasChanged = ( diff.first != topAlgs.value().end() );
  }

  if ( hasChanged ) {
    topAlgs.setValue( std::vector<std::string>( merge.begin(), merge.end() ) );
    if ( appProps->setProperty( topAlgs ).isFailure() ) {
      error() << " failed to set property" << endmsg;
      return StatusCode::FAILURE;
    }
    info() << " updated TopAlgs: " << topAlgs.toString() << endmsg;
  } else {
    if ( msgLevel( MSG::DEBUG ) ) debug() << " TopAlgs remain unchanged: " << topAlgs.toString() << endmsg;
  }
  return StatusCode::SUCCESS;
}

StatusCode PropertyConfigSvc::reconfigure( const ConfigTreeNode::digest_type& top ) const {
  // push the right properties to the JobOptionsSvc, and call 'setProperties'
  // on the ones we pushed...
  StatusCode sc = configure( top, true );
  if ( !sc.isSuccess() ) return sc;
  assert( m_appMgrUI );
  return m_appMgrUI->restart();
}

//=============================================================================
// Validate & create pictures...
//=============================================================================
StatusCode PropertyConfigSvc::validateConfig( const ConfigTreeNode::digest_type& ref ) const {
  // check that there are no overlaps, i.e. that a given named
  // object doesn't appear in this configuration tree
  // with more than one id...
  std::map<std::string, PropertyConfig::digest_type> inv;

  for ( const auto& i : collectLeafRefs( ref ) ) {
    const PropertyConfig* config = resolvePropertyConfig( i );
    if ( !config ) {
      error() << "validateConfig:: could not resolve " << i << endmsg;
      return StatusCode::FAILURE;
    }
    auto f = inv.find( config->name() );
    if ( f == inv.end() ) { // not yet present
      inv.emplace( config->name(), i );
    } else if ( f->first != config->name() ) { // collision detected: present, and inconsistent
      error() << "mutually exclusive configs detected for " << config->name() << " : " << i << " and " << f->second
              << endmsg;
      return StatusCode::FAILURE;
    } else { // already present, and consistent
             // DO NOTHING
    }
  }

  if ( msgLevel( MSG::DEBUG ) ) {
    for ( const auto& j : inv ) { debug() << j.first << " -> " << j.second << endmsg; }
  }
  return StatusCode::SUCCESS;
}

void PropertyConfigSvc::createGraphVizFile( const ConfigTreeNode::digest_type& ref, const std::string& fname ) const {
  std::ofstream df( fname );
  df << "digraph pvn {\n ordering=out;\n rankdir=LR;\n";
  for ( const auto& i : collectNodeRefs( ref ) ) {
    const ConfigTreeNode* node   = resolveConfigTreeNode( i );
    const PropertyConfig* config = resolvePropertyConfig( node->leaf() );
    df << "\"" << i
       << "\" [ label=\""
       // <<  "id: " << *i << "\\n"
       << "name: " << config->name() << "\\n"
       << "type: " << config->type() << "\\n"
       << "\"];\n";
    for ( const auto& j : node->nodes() ) { df << "\"" << i << "\" -> \"" << j << "\";\n"; }
  }
  df << "}" << std::endl;
}

//=============================================================================
// Resolve
//=============================================================================
const PropertyConfig* PropertyConfigSvc::resolvePropertyConfig( const PropertyConfig::digest_type& ref ) const {
  auto pc = m_configs.with_lock( [&]( const PropertyConfigMap_t& configs ) -> const PropertyConfig* {
    auto i = configs.find( ref );
    if ( i == configs.end() ) return nullptr;
    if ( this->msgLevel( MSG::DEBUG ) ) this->debug() << "already have an entry for id " << ref << endmsg;
    return &( i->second );
  } );
  if ( pc ) return pc;
  auto config = m_accessSvc->readPropertyConfig( ref );
  if ( !config ) {
    error() << " could not obtain PropertyConfig ref " << ref << endmsg;
    return nullptr;
  }
  if ( config->digest() != ref ) {
    error() << " got unexpected config: ref " << ref.str() << " points at " << config->digest().str() << endmsg;
    return nullptr;
  }
  return m_configs.with_lock( [&]( PropertyConfigMap_t& configs ) {
    auto rv = configs.emplace( ref, *config );
    return &( rv.first->second );
  } );
}

const ConfigTreeNode* PropertyConfigSvc::resolveConfigTreeNode( const ConfigTreeNodeAlias::alias_type& ref ) const {
  if ( msgLevel( MSG::DEBUG ) ) debug() << " resolving alias " << ref << endmsg;
  return resolveConfigTreeNode( resolveAlias( ref ) );
}

const ConfigTreeNode* PropertyConfigSvc::resolveConfigTreeNode( const ConfigTreeNode::digest_type& ref ) const {
  if ( msgLevel( MSG::DEBUG ) ) debug() << " resolving nodeRef " << ref << endmsg;
  auto cfn = m_nodes.with_lock( [&]( const ConfigTreeNodeMap_t& nodes ) -> const ConfigTreeNode* {
    auto i = nodes.find( ref );
    if ( i == nodes.end() ) return nullptr;
    if ( msgLevel( MSG::DEBUG ) ) debug() << "already have an entry for id " << ref << endmsg;
    return &( i->second );
  } );
  if ( cfn ) return cfn;
  assert( m_accessSvc );
  auto node = m_accessSvc->readConfigTreeNode( ref );
  if ( !node ) {
    error() << " could not obtain ConfigTreeNode ref " << ref << endmsg;
    return nullptr;
  }
  if ( node->digest() != ref ) {
    error() << " got unexpected node: ref " << ref.str() << " points at " << node->digest().str() << endmsg;
    return nullptr;
  }
  return m_nodes.with_lock( [&]( ConfigTreeNodeMap_t& nodes ) {
    auto rv = nodes.emplace( ref, *node );
    return &( rv.first->second );
  } );
}

PropertyConfig::Prop PropertyConfigSvc::Transformer::operator()( const PropertyConfig::Prop& in ) {
  std::string out = in.second;
  for ( const auto& i : m_list ) { // vector of all component maps to apply
    for ( const auto& j : *i ) {   // map to apply
      if ( in.first != j.first ) continue;
      out = std::accumulate( begin( j.second ), end( j.second ), out, []( const std::string& s, const auto& rule ) {
        return std::regex_replace( s, std::regex{rule.first}, rule.second );
      } );
    }
  }
  if ( out == in.second ) return in;
  m_os << "Applying substitution " << m_c << "." << in.first << " : " << in.second << " => " << out << endmsg;
  return PropertyConfig::Prop( in.first, out );
}

// ============================================================================
// BEGIN new parser:
// ============================================================================
// Gaudi
// ============================================================================
#include "GaudiKernel/ParsersFactory.h"
#include "GaudiKernel/StatusCode.h"
// ============================================================================
namespace Gaudi {
  namespace Parsers {
    // 2011-08-26 : alexander.mazurov@gmail.com
    StatusCode parse( std::map<std::string, std::map<std::string, std::map<std::string, std::string>>>& result,
                      const std::string&                                                                input ) {
      return parse_( result, input );
    }

    StatusCode parse( std::set<std::string>& result, const std::string& input ) { return parse_( result, input ); }
    // ============================================================================
    // END of parser
    // ============================================================================
  } // namespace Parsers
} // namespace Gaudi
