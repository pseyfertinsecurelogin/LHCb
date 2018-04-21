#ifndef UPDATEMANAGERSVC_H
#define UPDATEMANAGERSVC_H 1

// Include files
#include "GaudiKernel/Service.h"
#include "GaudiKernel/Time.h"
#include "GaudiKernel/ClassID.h"
#include "GaudiKernel/IOpaqueAddress.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/MsgStream.h"

#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/UpdateManagerException.h"

#include "GaudiKernel/Map.h"

#include "GaudiKernel/HashMap.h"

#include <string>
#include <map>
#include <exception>
#include <algorithm>
#include <shared_mutex>

#include "DetDesc/ICondIOVResource.h"

#ifndef WIN32
#include <pthread.h>
#endif

// Forward declarations
template <class TYPE> class SvcFactory;
class IIncidentSvc;
class IEventProcessor;
struct Condition;

/** @class UpdateManagerSvc UpdateManagerSvc.h
 *
 *
 *  @author Marco Clemencic
 *  @date   2005-03-30
 */
class UpdateManagerSvc: public extends<Service, IUpdateManagerSvc, IIncidentListener, ICondIOVResource> {
public:
  /// Standard constructor
  using base_class::base_class;

  /// Initialize Service
  StatusCode initialize() override;

  /// Start Service.
  StatusCode start() override;

  /// Stop Service.
  /// Dump the status of the network of dependencies.
  StatusCode stop() override;

  /// Finalize Service
  StatusCode finalize() override;

  /// Return the pointer to the data provider service, used to retrieve objects.
  IDataProviderSvc *dataProvider() const override;
  /// Return the pointer to the detector data service, used to obtain the event time..
  IDetDataSvc *detDataSvc() const override;

  /// Start a the update loop getting the time to use from the detector data service.
  StatusCode newEvent() override;
  /// Start a the update loop using the provided time to decide if an item is valid or not.
  /// \warning{The time used to retrieve an object from the condition database is the one obtained from
  /// the detector data service.}
  StatusCode newEvent(const Gaudi::Time &evtTime) override;

  //virtual StatusCode runAll() const;

  bool getValidity(const std::string path, Gaudi::Time& since, Gaudi::Time &until, bool path_to_db = false) override;
  void setValidity(const std::string path, const Gaudi::Time& since, const Gaudi::Time &until, bool path_to_db = false) override;

  /// Debug method: it dumps the dependency network through the message service (not very readable, for experts only).
  void dump() override;

  /// Force the update manager service to wait before entering the newEvent loop.
  void acquireLock() override;
  /// Let the update manager service enter the newEvent loop.
  void releaseLock() override;

  /// Remove all the items referring to objects present in the transient store.
  /// This is needed when the Detector Transient Store is purged, otherwise we
  /// will keep pointers to not existing objects.
  void purge() override;

  // ---- Implement IIncidentListener interface ----
  /// Handle BeginEvent incident.
  void handle(const Incident &inc) override;

  ICondIOVResource::IOVLock reserve( const Gaudi::Time& eventTime ) const override;

protected:

  /// Register a condition for an object
  //  virtual StatusCode i_registerCondition(const std::string &condition, BaseObjectMemberFunction *mf);

  /// Register a condition for an object together with the destination for the pointer to the condition object.
  void i_registerCondition(const std::string &condition, BaseObjectMemberFunction *mf,
                                 BasePtrSetter *ptr_dest = nullptr) override;

  /// Register a condition for an object
  void i_registerCondition(void *obj, BaseObjectMemberFunction *mf) override;

  /// Used to force an update of the given instance (e.g. when the object is created during an event).
  StatusCode i_update(void *instance) override;

  /// Used to remove an object from the dependency network.
  /// \warning{Removing an object is dangerous}
  void i_unregister(void *instance) override;

  /// Force an update of all the object depending on the given one for the next event.
  void i_invalidate(void *instance) override;

private:

#include "UpdateManagerSvc_Item.icpp"

  /// Hashmap for fast string access
  GaudiUtils::HashMap<std::string, Item*> m_pathMap;

  void insertInMap( Item* it ) {
    const std::pair<const std::string,Item*> tempS( it->path, it );
    m_pathMap.insert( tempS );
  }

  /// Connects two items in a parent-child relationship through the give member function.
  inline void link(Item* parent, BaseObjectMemberFunction *mf, Item *child);

  // Disconnect a parent from the child.
  void unlink(Item *parent, Item *child);

  /// Finds the item matching the given path.
  inline Item *findItem(const std::string &path, bool is_path_to_db = false) const;

  /// Finds the item matching the given pointer.
  inline Item *findItem(void *p) const;

  /// Finds the item containing the given member function.
  inline Item *findItem(BaseObjectMemberFunction *mf) const;

  /// Removes an item from the list of head items.
  inline void removeFromHead(Item *item);

  // Properties
  Gaudi::Property<std::string> m_dataProviderName
    {this, "DataProviderSvc", "DetectorDataSvc", "Name of the Data Provider"};
  Gaudi::Property<std::string> m_detDataSvcName
    {this, "DetDataSvc", "", "Name of the DetDataSvc, empty means _the same as data provider_"};
  Gaudi::Property<bool> m_withoutBeginEvent
    {this, "WithoutBeginEvent", false,
     "Whether beginEvent is working or not. E.g. it is not in Hive"};
  /// The syntax to define a condition is:<BR>
  /// path := type1 name1 = value1; type2 name2 = value2; ...
  Gaudi::Property<std::vector<std::string>> m_conditionsOveridesDesc
    {this, "ConditionsOverride", {}, "List of condition definitions to override the ones in the transient store"};
  Gaudi::Property<std::string> m_dotDumpFile
    {this, "DotDumpFile", "", "Name of the dot (graphviz) file into which write the dump"};

  // ---------- data members ----------
  /// Handle to the Data Provider (where to find conditions).
  SmartIF<IDataProviderSvc> m_dataProvider;

  /// Name of the root node of the Transient Store.
  std::string       m_dataProviderRootName;

  /// Handle to the IDetDataSvc interface (used to get the event time).
  /// If the service is not found it is not fatal, but you cannot use the method newEvent()
  /// without the event time parameter (will always fail).
  SmartIF<IDetDataSvc> m_detDataSvc;

  /// Pointer to the incident service;
  SmartIF<IIncidentSvc> m_incidentSvc;

  /// Pointer to the event processor in order to be able to top the run if something goes wrpong during an update.
  SmartIF<IEventProcessor> m_evtProc;

  /// List used to keep track of all the registered items.
  std::vector<std::unique_ptr<Item>>                                        m_all_items;
  /// List used to record all the objects without parents. (for fast access)
  Item::ItemList                                                            m_head_items;
  /// Lower bound of intersection of head IOVs.
  Gaudi::Time                                                               m_head_since = 1;
  /// Higher bound of intersection of head IOVs.
  Gaudi::Time                                                               m_head_until = 0;

  /// Map containing the list of parsed condition definitions
  std::map<std::string,std::unique_ptr<Condition>> m_conditionsOverides;

#ifndef WIN32
  /// mutex lock used to avoid dependencies corruptions in a multi-thread environment.
  pthread_mutex_t m_busy = PTHREAD_MUTEX_INITIALIZER;
#endif

  mutable std::shared_timed_mutex m_IOVresource;
  mutable std::mutex m_IOVreserve_mutex;
};

#include "UpdateManagerSvc.icpp"

#endif // UPDATEMANAGERSVC_H
