#ifndef PROPERTYCONFIGSVC_H
#define PROPERTYCONFIGSVC_H 1

// Include files
#include <string>
#include <map>
#include <set>
#include <vector>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <iterator>
#include <optional>
#include <string_view>

class StatusCode;
namespace Gaudi { namespace Parsers {
StatusCode parse(std::map<std::string, std::map<std::string,
                 std::map< std::string, std::string> > >& result,
                 const std::string& input ) ;

StatusCode parse(std::set<std::string>& result,
                 const std::string& input ) ;
} }

// from Gaudi
#include "GaudiKernel/Service.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/IAlgManager.h"
#include "GaudiKernel/IAppMgrUI.h"
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/VectorMap.h"

// from HltInterfaces
#include "Kernel/IConfigAccessSvc.h"
#include "Kernel/IPropertyConfigSvc.h"
#include "Kernel/PropertyConfig.h"

template <typename T, typename Mutex = std::shared_timed_mutex> // C++17: replace with shared_mutex...
class Synced {
    T m_obj;
    mutable Mutex m_mtx;
public:
    template <typename F> auto with_wlock(F&& f) -> decltype(auto)
    { auto lock=std::unique_lock<Mutex>{m_mtx}; return f(m_obj); }
    template <typename F> auto with_rlock(F&& f) const -> decltype(auto)
    { auto lock=std::shared_lock<Mutex>{m_mtx}; return f(m_obj); }
};

/** @class PropertyConfigSvc PropertyConfigSvc.h
 *
 *  functionality:
 *         configure algorithms, services and tools...
 *
 *         TODO: split into two seperate svcs,
 *               one for getting configuration info (using the
 *               access svc for I/O) and navigating through it,
 *               and a second, which uses the first,
 *               to actually _use_ it to configure
 *               an application..
 *
 *  @author Gerhard Raven
 *  @date   2007-10-24
 */
class PropertyConfigSvc : public extends<Service,IPropertyConfigSvc>
                        , public IToolSvc::Observer {
private:
  typedef std::map<PropertyConfig::digest_type,  PropertyConfig> PropertyConfigMap_t;
  typedef std::map<ConfigTreeNode::digest_type,  ConfigTreeNode> ConfigTreeNodeMap_t;
  typedef std::map<ConfigTreeNodeAlias::alias_type,  ConfigTreeNode::digest_type> ConfigTreeNodeAliasMap_t;

  //TODO: use multimap instead???
  typedef std::map<ConfigTreeNode::digest_type,  std::vector<PropertyConfig::digest_type> > Tree2LeafMap_t;
  typedef std::map<ConfigTreeNode::digest_type,  std::list<ConfigTreeNode::digest_type> > Tree2NodeMap_t;

public:
  using extends::extends;

  StatusCode initialize() override;    ///< Service initialization
  StatusCode finalize() override;    ///< Service initialization

  PropertyConfig currentConfiguration(const INamedInterface& obj) const override;
  PropertyConfig::digest_type findInTree(const ConfigTreeNode::digest_type& configTree, const std::string& name) const override;

  const Tree2NodeMap_t::mapped_type& collectNodeRefs(const ConfigTreeNode::digest_type& nodeRef) const override;
  const Tree2NodeMap_t::mapped_type& collectNodeRefs(const ConfigTreeNodeAlias::alias_type& nodeRef) const override;
  const Tree2LeafMap_t::mapped_type& collectLeafRefs(const ConfigTreeNode::digest_type& nodeRef) const override;
  const Tree2LeafMap_t::mapped_type& collectLeafRefs(const ConfigTreeNodeAlias::alias_type& nodeRef) const override;

  const PropertyConfig* resolvePropertyConfig(const PropertyConfig::digest_type& ref) const override;
  const ConfigTreeNode* resolveConfigTreeNode(const ConfigTreeNode::digest_type& ref) const override;
  const ConfigTreeNode* resolveConfigTreeNode(const ConfigTreeNodeAlias::alias_type& ref) const override;
  // helper functions

  // preload a configuration
  StatusCode loadConfig(const ConfigTreeNode::digest_type& nodeRef);
  // resolve the ID to get a list of  configurations, then use them to configure
  using Service::configure;
  StatusCode configure(const ConfigTreeNode::digest_type& configID, bool callSetProperties) const;
  StatusCode configure(const ConfigTreeNodeAlias::alias_type& alias, bool callSetProperties) const;
  // reconfigure: first configure, then call sysReinitialize on the top algorithm
  StatusCode reconfigure(const ConfigTreeNode::digest_type& top) const;

  //               component             property               regex        replacement
  typedef std::map<std::string, std::map<std::string, std::map< std::string, std::string> > > TransformMap;

protected:
//
//   component name
//             property name
//                       replacement rule
//   { 'GaudiSequencer/.*' : { 'Members' : { "'PatPV3D/HltPVsPV3D'" : "'PatPV3D/HltPVsPV3D', 'HltMoveVerticesForSwimming/HltMovePVs4Swimming'" } } }
//
  class Transformer {
   public:
       Transformer(std::string component,MsgStream& os): m_os(os), m_c(std::move(component)) {}
       void push_back( const std::map<std::string, std::map<std::string,std::string> >* pmap ) { m_list.push_back(pmap); }
       bool empty() const { return m_list.empty(); }
       PropertyConfig::Prop operator()(const PropertyConfig::Prop& in);
   private:
       typedef std::map<std::string,std::string> ReplMap_t;
       typedef std::map<std::string, ReplMap_t > PropMap_t;
       typedef std::vector< const PropMap_t* >  List_t;
       List_t  m_list;
       MsgStream&  m_os;
       std::string m_c;
  };

  // check validity of given config
  StatusCode validateConfig(const PropertyConfig::digest_type& ref) const;

  IConfigAccessSvc* cas() const { return m_accessSvc;}


private:
  Gaudi::Property<TransformMap>        m_transform { this, "ApplyTransformation" };

  Gaudi::Property<std::string>         s_accessSvc{ this, "ConfigAccessSvc", "ConfigCDBAccessSvc"};
  SmartIF<IJobOptionsSvc>              m_joboptionsSvc;
  SmartIF<IToolSvc>                    m_toolSvc;
  SmartIF<IAlgManager>                 m_algMgr;
  SmartIF<IAppMgrUI>                   m_appMgrUI;
  SmartIF<IConfigAccessSvc>            m_accessSvc;

  mutable Synced<PropertyConfigMap_t>    m_configs;  // config ref -> config (leaf)
  mutable Synced<ConfigTreeNodeMap_t>    m_nodes;    // node   ref -> node
  mutable Synced<ConfigTreeNodeAliasMap_t> m_aliases;    // node   ref -> node
  mutable Synced<Tree2LeafMap_t>         m_leavesInTree; // top level node ref -> config refs (leaves)
  mutable Synced<Tree2NodeMap_t>         m_nodesInTree; // top level node ref -> node refs

  mutable std::map<std::string, PropertyConfig::digest_type> m_configPushed;
  std::map<std::string,IAlgTool*>      m_toolmap;
  Gaudi::Property<std::vector<std::string>> m_prefetch{ this, "prefetchConfig"};    ///< configurations to load at initialization
  Gaudi::Property<std::set<std::string>> m_skip{this, "SkipComponent"};        ///< items NOT to configure with this service NOR THEIR DEPENDENTS!
  Gaudi::Property<std::string>         m_ofname { this, "optionsfile" };
  std::unique_ptr<std::ostream>        m_os;
  Gaudi::Property<bool>                m_createGraphVizFile { this, "createGraphVizFile", false };
  Gaudi::Property<bool>                m_allowFlowChanges { this, "AllowFlowChanges",false};
  mutable std::optional<std::vector<std::string> > m_initialTopAlgs;

  void onCreate(const IAlgTool* tool) override;

  template <typename T> T* resolve(const std::string& name) const;
  StatusCode invokeSetProperties(const PropertyConfig& config) const;

  StatusCode setTopAlgs(const ConfigTreeNode::digest_type& id) const;
  StatusCode findTopKind(const ConfigTreeNode::digest_type& configID,
                         std::string_view kind,
                         std::back_insert_iterator<std::vector<const PropertyConfig*> > components) const;
  void createGraphVizFile(const PropertyConfig::digest_type& ref, const std::string& fname) const;
  ConfigTreeNode::digest_type resolveAlias(const ConfigTreeNodeAlias::alias_type& alias) const;
  StatusCode outOfSyncConfigs(const ConfigTreeNode::digest_type& top,
                              std::back_insert_iterator<std::vector<const PropertyConfig*> >  ) const;
};
#endif // PROPERTYCONFIGSVC_H
