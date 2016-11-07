// Include files

#include "GaudiKernel/IDetDataSvc.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/IEventProcessor.h"
#include "GaudiKernel/System.h"

#include "DetDesc/ValidDataObject.h"
#include "DetDesc/Condition.h"

#include <set>
#include <sstream>
#include <fstream>
#include <memory>

// local
#include "UpdateManagerSvc.h"

#include "ConditionParser.h"

DECLARE_SERVICE_FACTORY( UpdateManagerSvc )

//-----------------------------------------------------------------------------
// Implementation file for class : UpdateManagerSvc
//
// 2005-03-30 : Marco Clemencic
//-----------------------------------------------------------------------------

//=============================================================================
// IService implementation
//=============================================================================
StatusCode UpdateManagerSvc::initialize(){
  // base class initialization
  StatusCode sc = base_class::initialize();
  if (!sc.isSuccess()) return sc;
  // local initialization
  if( msgLevel(MSG::DEBUG) )
    debug() << "--- initialize ---" << endmsg;

  // find the data provider
  m_dataProvider = serviceLocator()->service(m_dataProviderName,true);
  if (!m_dataProvider) {
    error() << "Unable to get a handle to the data provider" << endmsg;
    return StatusCode::FAILURE;
  }
  if( msgLevel(MSG::DEBUG) )
    debug() << "Got pointer to IDataProviderSvc \"" << m_dataProviderName << '"' << endmsg;
  auto dMgr = m_dataProvider.as<IDataManagerSvc>();
  if ( !dMgr ) {
    warning() << "Cannot access IDataManagerSvc interface of \"" << m_dataProviderName
        << "\": using empty RootName" << endmsg;
    m_dataProviderRootName = "";
  } else {
    m_dataProviderRootName = dMgr->rootName() + "/";
  }

  // find the detector data service
  if (m_detDataSvcName.empty()) m_detDataSvcName = m_dataProviderName;
  m_detDataSvc = serviceLocator()->service(m_detDataSvcName,true);
  if (!m_detDataSvc) {
    warning() << "Unable to get a handle to the detector data service interface:"
      " all the calls to newEvent(void) will fail!" << endmsg;
  } else {
    if( msgLevel(MSG::DEBUG) )
      debug() << "Got pointer to IDetDataSvc \"" << m_detDataSvcName << '"' << endmsg;
  }

  {
    // before registering to the incident service I have to be sure that the EventClockSvc is ready
    auto evtClockSvc = service("EventClockSvc",  true);
    if (!evtClockSvc) {
      warning() << "Unable find EventClockSvc, probably I'll not work." << endmsg;
    } else if ( msgLevel(MSG::DEBUG) ) {
      debug() << "Good: EventClockSvc found" << endmsg;
    }
  }

  // register to the incident service for BeginEvent incidents
  m_incidentSvc = service("IncidentSvc", false);
  if ( !m_incidentSvc ) {
    error() << "Unable to register to the incident service." << endmsg;
    return StatusCode::FAILURE;
  }
  if( msgLevel(MSG::DEBUG) )
      debug() << "Got pointer to IncidentSvc" << endmsg;
  m_incidentSvc->addListener(this,IncidentType::BeginEvent);

  m_evtProc = serviceLocator()->service("ApplicationMgr");
  if ( !sc.isSuccess() ) {
    error() << "Cannot find an event processor." << endmsg;
    return sc;
  }

  // Loop over overridden conditions
  for ( const auto& co : m_conditionsOveridesDesc ) {
    std::string name;
    auto cond = std::make_unique<Condition>();
    if (!ConditionParser(co,name,*cond)) {
      // something went wrong while parsing
      error() << "Cannot understand condition:" << endmsg;
      error() << co << endmsg;
      return StatusCode::FAILURE;
    }

    // Special update mode
    cond->setUpdateMode(ValidDataObject::OVERRIDE);

    // Remove TS root name from the path
    if ( name[0] == '/'
         && name.compare(0,m_dataProviderRootName.size(),m_dataProviderRootName) == 0 ){
      name.erase(0,m_dataProviderRootName.size());
    }

    // If a condition override with that name already exists, delete it
#if  __cplusplus > 201402L
    auto ret = m_conditionsOverides.insert_or_assign( name, std::move(cond) );
    if (!ret.second) {
      warning() << "Override condition for path '" << name
          << "' is defined more than once (I use the last one)." << endmsg;
    }
    auto dest = ret.first;
#else
    auto dest = m_conditionsOverides.find(name);
    if ( dest!=m_conditionsOverides.end() ) {
      warning() << "Override condition for path '" << name
          << "' is defined more than once (I use the last one)." << endmsg;
      dest->second = std::move(cond);
    } else {
      auto ret = m_conditionsOverides.emplace( name, std::move(cond) );
      assert( ret.second ); // we first did a 'find' and there was nothing, so this better be true...
      dest = ret.first;
    }
#endif
    // Add the condition to internal list
    if( msgLevel(MSG::DEBUG) )
      debug() << "Added condition: " << name << "\n" << dest->second->printParams() << endmsg;

  }
  return StatusCode::SUCCESS;
}

StatusCode UpdateManagerSvc::start() {
  if (m_withoutBeginEvent) {
    return newEvent();
  } else {
    return StatusCode::SUCCESS;
  }
}

StatusCode UpdateManagerSvc::stop(){
  if( msgLevel(MSG::DEBUG) ) {
    debug() << "--- stop ---" << endmsg;
  }
  if ( msgLevel(MSG::DEBUG) || ! m_dotDumpFile.empty() ) dump();
  return base_class::stop();
}

StatusCode UpdateManagerSvc::finalize(){
  // local finalization
  if( msgLevel(MSG::DEBUG) )
    debug() << "--- finalize ---" << endmsg;

  // release the interfaces used
  m_dataProvider.reset();
  m_detDataSvc.reset();
  if (m_incidentSvc) {
    // unregister from the incident svc
    m_incidentSvc->removeListener(this,IncidentType::BeginEvent);
  }
  m_incidentSvc.reset();
  m_evtProc.reset();

  // delete unused overridden conditions (the others are deleted together with the T.S.)
  if ( ! m_conditionsOverides.empty() ) {
    warning() << "Overridden conditions not used:" << endmsg;
    for (auto& c : m_conditionsOverides) warning() << c.first << endmsg;
    m_conditionsOverides.clear();
  }

  // base class finalization
  return base_class::finalize();
}
//=============================================================================
// IUpdateManagerSvc implementation
//=============================================================================
IDataProviderSvc *UpdateManagerSvc::dataProvider() const {
  return m_dataProvider.get();
}
IDetDataSvc *UpdateManagerSvc::detDataSvc() const {
  return m_detDataSvc.get();
}
void UpdateManagerSvc::i_registerCondition(const std::string &condition, BaseObjectMemberFunction *mf,
                                           BasePtrSetter *ptr_dest){
  if ( FSMState() < Gaudi::StateMachine::INITIALIZED ){
    throw GaudiException("Service offline","UpdateManagerSvc::registerCondition",StatusCode::FAILURE);
  }

  std::string cond_copy(condition);

  if (!cond_copy.empty()) {
    // remove the root name if present
    if ( cond_copy[0] == '/'
         && cond_copy.compare(0,m_dataProviderRootName.size(),m_dataProviderRootName) == 0 ){
      cond_copy.erase(0,m_dataProviderRootName.size());
    }
    if( msgLevel(MSG::VERBOSE) )
      verbose() << "registering condition \"" << cond_copy
          << "\" for object of type " << System::typeinfoName(mf->type())
          << " at " << std::hex << mf->castToVoid() << endmsg;
  } else {
    if( msgLevel(MSG::VERBOSE) )
      verbose() << "registering object of type " << System::typeinfoName(mf->type())
          << " (without condition)" << endmsg;
  }

  // find the object
  Item* mf_item = findItem(mf);
  if (!mf_item){ // a new OMF
    m_all_items.push_back(std::make_unique<Item>(mf, m_dataProviderRootName));
    mf_item = m_all_items.back().get();
    m_head_items.push_back(mf_item); // since it is new, it has no parents
    insertInMap( mf_item );
 } else {
    if ( ! mf_item->ptr ) { // the item is know but not its pointer (e.g. after a purge)
      mf_item->vdo = mf->castToValidDataObject();
      mf_item->ptr = mf->castToVoid();
    }
  }

  if (!cond_copy.empty()) {
    // find the condition
    Item* cond_item = findItem(cond_copy);
    if (!cond_item){ // a new condition

      std::unique_ptr<Item> cnd_item;
      // Check if the requested condition is in the override list.
      auto cond_ov = m_conditionsOverides.find(cond_copy);
      if ( cond_ov != m_conditionsOverides.end() ) {
        // yes, it is!
        cnd_item = std::make_unique<Item>(cond_copy,Item::UserPtrType(ptr_dest,mf_item->ptr),
                                          std::move(cond_ov->second));
        // I do not need it anymore in the list
        m_conditionsOverides.erase(cond_ov);
      } else {
        // no override
        cnd_item = std::make_unique<Item>(cond_copy,Item::UserPtrType(ptr_dest,mf_item->ptr));
      }

      m_all_items.push_back(std::move(cnd_item));
      cond_item = m_all_items.back().get();
      insertInMap( cond_item );
    } else {
      if (ptr_dest){
        // I already have this condition registered, but a new user wants to set the pointer to it.
        cond_item->user_dest_ptrs.emplace_back(ptr_dest,mf_item->ptr);
        // Let's check if the object is already loaded (the pointers are set by Item only when it loads them)
        if (cond_item->vdo) {
          ptr_dest->set(cond_item->vdo);
          if ( ptr_dest->isNull() ) { // the dynamic cast failed
            throw GaudiException("A condition in memory cannot be casted to the requested type",
                "UpdateManagerSvc::i_registerCondition", StatusCode::FAILURE );
          }
        }
      }
      if (cond_item->isHead()) removeFromHead(cond_item);
    }
    link(mf_item,mf,cond_item);
  } else {
    // this is usually done inside Item::addChild (called by "link")
    auto mfIt = mf_item->find(mf);
    if (mfIt == mf_item->memFuncs.end()) {
      // I do not have the MF registered inside the item
      // so I add it
      mf_item->memFuncs.insert(mfIt,Item::MembFunc(mf));
    } else {
      // the MF is already there
      if (mfIt->mf != mf) // but it has a different pointer
        // so I do not need to keep the copy I have
        delete mf;
    }
    // Since we are not using a condition, the user pointer setter is not needed
    // nor used, so we must delete it.
    if (ptr_dest) delete ptr_dest;
  }
  // a new item means that we need an update
  m_head_since = 1;
  m_head_until = 0;
}
void UpdateManagerSvc::i_registerCondition(void *obj, BaseObjectMemberFunction *mf){
  if ( FSMState() < Gaudi::StateMachine::INITIALIZED ){
    throw GaudiException("Service offline","UpdateManagerSvc::registerCondition",StatusCode::FAILURE);
  }
  if( msgLevel(MSG::VERBOSE) ) {
    verbose() << "registering object at " << std::hex << obj << std::dec
        << " for object of type " << System::typeinfoName(mf->type()) << " at " << std::hex << mf->castToVoid() << endmsg;
  }

  // find the "condition"
  Item* cond_item = findItem(obj);
  if (!cond_item){ // Error!!!
    throw UpdateManagerException("tried to register for an object not in the UpdateManagerSvc");
  } else {
    if (cond_item->isHead()) removeFromHead(cond_item);
  }
  // find the OMF (Object Member Function)
  Item* mf_item = findItem(mf);
  if (!mf_item){ // a new OMF
    m_all_items.push_back(std::make_unique<Item>(mf, m_dataProviderRootName));
    mf_item = m_all_items.back().get();
    m_head_items.push_back(mf_item); // since it is new, it has no parents
    insertInMap( mf_item );
  }
  if ( ! mf_item->ptr ) { // the item is know but not its pointer (e.g. after a purge)
    mf_item->vdo = mf->castToValidDataObject();
    mf_item->ptr = mf->castToVoid();
  }
  link(mf_item,mf,cond_item);
  // a new item means that we need an update
  m_head_since = 1;
  m_head_until = 0;
}
StatusCode UpdateManagerSvc::newEvent(){
  if (detDataSvc()){
    if (detDataSvc()->validEventTime()) {
      return newEvent(detDataSvc()->eventTime());
    } else {
      warning() << "newEvent(): the event time is not defined!" << endmsg;
    }
  }
  return StatusCode::FAILURE;
}
StatusCode UpdateManagerSvc::newEvent(const Gaudi::Time &evtTime){
  if ( FSMState() < Gaudi::StateMachine::INITIALIZED ){
    throw GaudiException("Service offline","UpdateManagerSvc::newEvent",StatusCode::FAILURE);
  }

  StatusCode sc = StatusCode::SUCCESS;

#ifndef WIN32
  if ( msgLevel(MSG::VERBOSE) )
    verbose() << "newEvent(evtTime): acquiring mutex lock" << endmsg;
  acquireLock();
#endif

  // Check head validity
  if ( evtTime >= m_head_since && evtTime < m_head_until ) {
#ifndef WIN32
    if ( msgLevel(MSG::VERBOSE) )
      verbose() << "newEvent(evtTime): releasing mutex lock" << endmsg;
    releaseLock();
#endif
    return sc; // no need to update
  }

#ifndef WIN32
  try {
#endif

  // We are in the initialization phase if we are not yet "STARTED"
  SmartIF<IStateful> globalState(serviceLocator());
  const bool inInit = globalState &&
                      (globalState->FSMState() < Gaudi::StateMachine::INITIALIZED) &&
                      (globalState->targetFSMState() >= Gaudi::StateMachine::INITIALIZED);
  // The head list may change while updating, I'll loop until it's stable (or a problem occurs)
  bool head_has_changed = false;
  do {
    if ( msgLevel(MSG::DEBUG) ) {
      debug() << "newEvent(evtTime): loop over head items" << endmsg;
    }
    // first I make a copy of the current head
    Item::ItemList head_copy(m_head_items);
    // Start from a clean IOV (I cannot use m_head_X because the head is not stable and they may change)
    Gaudi::Time head_copy_since(Gaudi::Time::epoch());
    Gaudi::Time head_copy_until(Gaudi::Time::max());
    MsgStream item_log(msgSvc(), name()+"::Item");
    for (auto it = head_copy.begin(); it != head_copy.end() && sc.isSuccess(); ++it){
      sc = (*it)->update(dataProvider(), evtTime, item_log, inInit);
      if (sc.isSuccess()) {
        if ( head_copy_since < (*it)->since )  head_copy_since = (*it)->since;
        if ( head_copy_until > (*it)->until )  head_copy_until = (*it)->until;
      }
    }
    // now it is safe to set m_head_X
    m_head_since = head_copy_since;
    m_head_until = head_copy_until;

    // check if we need to re-do the loop (success and a change in the head)
    head_has_changed = sc.isSuccess() && (head_copy != m_head_items);
  } while ( head_has_changed );

#ifndef WIN32
  } catch (...) {
    if ( msgLevel(MSG::VERBOSE) )
      verbose() << "newEvent(evtTime): releasing mutex lock (exception occurred)" << endmsg;
    releaseLock();
    throw;
  }

  if ( msgLevel(MSG::VERBOSE) )
    verbose() << "newEvent(evtTime): releasing mutex lock" << endmsg;
  releaseLock();
#endif

  return sc;
}
StatusCode UpdateManagerSvc::i_update(void *instance){
  if ( FSMState() < Gaudi::StateMachine::INITIALIZED ){
    throw GaudiException("Service offline","UpdateManagerSvc::update",StatusCode::FAILURE);
  }

  if ( msgLevel(MSG::VERBOSE) ) {
    verbose() << "Update specific object at " << instance << endmsg;
  }
  if (detDataSvc() != NULL){
    if (detDataSvc()->validEventTime()) {
      Item* item = findItem(instance);
      if (item) {
        StatusCode sc;
        // We are in the initialization phase if we are not yet "STARTED"
        SmartIF<IStateful> globalState(serviceLocator());
        const bool inInit = globalState &&
                            (globalState->FSMState() < Gaudi::StateMachine::INITIALIZED) &&
                            (globalState->targetFSMState() >= Gaudi::StateMachine::INITIALIZED);
        MsgStream item_log(msgSvc(),name()+"::Item");
        sc = item->update(dataProvider(), detDataSvc()->eventTime(), item_log, inInit);
        if (sc.isSuccess()) {
          if ( m_head_since < item->since )  m_head_since = item->since;
          if ( m_head_until > item->until )  m_head_until = item->until;
        }
        return sc;
      } else {
        warning() << "Cannot find object at " << instance << endmsg;
      }
    } else {
      return StatusCode::SUCCESS;
    }
  }
  return StatusCode::FAILURE;
}
void UpdateManagerSvc::i_invalidate(void *instance){
  if ( FSMState() < Gaudi::StateMachine::INITIALIZED ){
    throw GaudiException("Service not initialized","UpdateManagerSvc::invalidate",StatusCode::FAILURE);
  }

  if ( msgLevel(MSG::VERBOSE) ) {
    verbose() << "Invalidate object at " << instance << endmsg;
  }
  Item* item = findItem(instance);
  if (item) {
    item->invalidate();
    m_head_since = 1;
    m_head_until = 0;
  } else {
    warning() << "Cannot find object at " << instance << endmsg;
  }
}

void UpdateManagerSvc::unlink(Item *parent, Item *child){

  // check if the parent knows about the child
  auto childIt = std::find(parent->children.begin(),
                           parent->children.end(),child);
  if ( parent->children.end() == childIt )
    return; // parent does not know about child

  // remove from child all the user pointers belonging to the parent
  auto pi = child->user_dest_ptrs.begin();
  while ( pi != child->user_dest_ptrs.end() ) {
    if (pi->second != parent) {
      pi = child->user_dest_ptrs.erase(pi);
    } else {
      ++pi;
    }
  }

  // If the child is used by a MF that uses other Items, we need to disconnect
  // them too.
  std::set<Item*> siblings; // list of Items used together with "child"

  // loop over child parent's pairs (mf,parent) to disconnect from them
  auto p = child->parents.begin();
  while ( p != child->parents.end() ) {
    if (p->first != parent) {
      ++p;
      continue; // skip to next one
    }

    // find the MF inside the parent
    auto p_mf = parent->find(p->second);

    // find iterator to child in MF list ...
    Item::ItemList& mfInternalList = p_mf->items;
    auto entry = std::find(mfInternalList.begin(),
                           mfInternalList.end(),child);
    // ... and remove it (if found)
    if ( mfInternalList.end() != entry )
      mfInternalList.erase(entry);

    // append then other Items in the MF (to unlink them too)
    siblings.insert(mfInternalList.begin(),mfInternalList.end());

    // remove the parent pair from child
    p = child->parents.erase(p);
  }

  // unlink the siblings
  for (auto s = siblings.begin(); s != siblings.end(); ++s ) {
    unlink(parent,*s);
  }

  // Check in the parent if there are MF without children: they have to be
  // removed.
  auto p_mf = parent->memFuncs.begin();
  while ( p_mf != parent->memFuncs.end() ) {
    if ( p_mf->items.empty() ) p_mf = parent->memFuncs.erase(p_mf);
    else ++p_mf;
  }

  // Remove child from parent's list of all children.
  // we have to look again for it because the iterator may have been made
  // invalid by the "unlink" of siblings
  childIt = std::find(parent->children.begin(),
                      parent->children.end(),child);
  if (childIt != parent->children.end())
    parent->children.erase(childIt);

  // check if the child should be part of the head now
  if ( child->isHead() ) m_head_items.push_back(child);

  // Note: I do not need to touch the validity because the it can only increase

}

void UpdateManagerSvc::i_unregister(void *instance){
  if ( FSMState() < Gaudi::StateMachine::INITIALIZED ){
    // un-registration is allowed after service finalize (no-op).
    if ( msgLevel(MSG::VERBOSE) ) {
      verbose() << "Trying to unregister object at " << instance
          << ", with the service OFFLINE"<< endmsg;
    }
    return;
  }

  if ( msgLevel(MSG::VERBOSE) ) {
    verbose() << "Unregister object at " << instance << endmsg;
  }

  Item* item = findItem(instance);
  if (item){

    // unlink from parents
    auto p = item->parents.begin();
    while ( p != item->parents.end() ) {
      unlink(p->first,item);
      p = item->parents.begin();
    }

    // unlink from children
    auto c = item->children.begin();
    while ( c != item->children.end() ) {
      unlink(item,(*c));
      c = item->children.begin();
    }

    // update the lists of Items
    if ( item->isHead() ) removeFromHead(item);
    // The erased item shoud also disappear from the maps, if this is the last for this key, i.e. isHead
    m_pathMap.erase( item->path );
    // finally we can delete the Item
    m_all_items.erase(std::find_if(m_all_items.begin(),m_all_items.end(),
                                   [&](const std::unique_ptr<Item>& i)
                                   { return i.get()==item; }));
  }
}

void UpdateManagerSvc::dump(){
  if ( FSMState() < Gaudi::StateMachine::INITIALIZED ){
    throw GaudiException("Service offline","UpdateManagerSvc::dump",StatusCode::FAILURE);
  }

  std::unique_ptr<std::ofstream> dot_file;

  if ( !m_dotDumpFile.empty() ){
    dot_file = std::make_unique<std::ofstream>(m_dotDumpFile.value().c_str());
  }

  if (dot_file) {
    // dot header
    (*dot_file) << "digraph " << name() << " {\n";
  }

  if ( msgLevel(MSG::VERBOSE) ) {
    verbose() << "--- Dump" << endmsg;
    verbose() << "    " << m_all_items.size() << " items registered" << endmsg;
    verbose() << "     of which " << m_head_items.size() << " in the head" << endmsg;
    verbose() << "         head IOV = " << m_head_since << " - " << m_head_until << endmsg;
  }

  size_t cnt = 0, head_cnt = 0;
  for (const auto& i : m_all_items) {
    if ( msgLevel(MSG::VERBOSE) ) {
      verbose() << "--item " << cnt++ << " " << std::hex << i.get() << std::dec;
      if (i->isHead()){
        msgStream() << " (head)";
        ++head_cnt;
      }
      msgStream() << endmsg;
    }

    if (dot_file) {
      // graph node for registered item (first part, label)
      (*dot_file) << "item_" << std::hex << i.get()
    		  << "[label=\""
    		  << "(" << std::dec << cnt-1 << ") "
    		  << std::hex << i.get() << "\\n"
    		  << "(" << i->ptr << ")";
    }

    if ( msgLevel(MSG::VERBOSE) )
      verbose() << "       ptr  = " << std::hex << i->ptr << std::dec << endmsg;
    if ( !i->path.empty() ) {
      if ( msgLevel(MSG::VERBOSE) )
        verbose() << "       path = " << i->path << endmsg;
      if (dot_file) {
        // If we have the path, we can put it in the graph label
        (*dot_file) << "\\n" << i->path;
      }
    }/* else {
      INamedInterface *ni = dynamic_cast<INamedInterface>(i->ptr);
      if (ni) {
    	// It's a component with name, we can put it in the graph label
    	(*dot_file) << "\\n" << ni->name();
      }
    } */

    if (dot_file) {
      // graph node for registered item (end)
      (*dot_file) << "\"];\n";
    }

    if ( msgLevel(MSG::VERBOSE) )
      verbose() << "        IOV = " << i->since << " - " << i->until << endmsg;
    if (i->memFuncs.size()){
      if ( msgLevel(MSG::VERBOSE) )
        verbose() << "       depend on :" << endmsg;
      for (auto mfIt = i->memFuncs.begin(); mfIt != i->memFuncs.end(); ++mfIt){
        if ( msgLevel(MSG::VERBOSE) )
          verbose() << std::hex << "                  ";
        for (auto itemIt = mfIt->items.begin(); itemIt != mfIt->items.end(); ++itemIt){
          if ( msgLevel(MSG::VERBOSE) )
            msgStream() << " " << *itemIt;
          if (dot_file) {
            // Add an arrow to the graph connecting the user Item to the
            // used Item
            (*dot_file) << "item_" << std::hex << i.get() << " -> " << "item_" << std::hex << *itemIt << ";\n";
          }
        }
        if ( msgLevel(MSG::VERBOSE) )
          msgStream() << std::dec << endmsg;
      }
    }
  }

  if (dot_file) {
    // DIA header
    (*dot_file) << "}\n";
    always() << "DOT file '" << m_dotDumpFile << "' written" << endmsg;
  }

  if ( msgLevel(MSG::VERBOSE) ) {
      verbose() << "Found " << head_cnt << " head items: ";
      msgStream() << ( m_head_items.size() == head_cnt ? "OK"
                                                       : "MISMATCH!!!!!" )
                  << endmsg;
  }
}


void UpdateManagerSvc::purge() {
  if ( FSMState() < Gaudi::StateMachine::INITIALIZED ){
    throw GaudiException("Service offline","UpdateManagerSvc::purge",StatusCode::FAILURE);
  }

  info() << "Purging dependencies network" << endmsg;

  // first I make a copy of the list of objects
  //Item::ItemList items_copy(m_all_items);
  //Item::ItemList items_copy(m_head_items);
  // Start from a clean IOV (I cannot use m_head_X because the head is not stable and they may change)
  //Gaudi::Time head_copy_since(Gaudi::Time::epoch());
  //Gaudi::Time head_copy_until(Gaudi::Time::max());

  for (auto& item : m_all_items ) {
    item->purge(&msgStream());
    if ( ! item->path.empty() ) {
      auto& children = item->children;
      // remove connections to children if the object is going to be reloaded
      auto c = children.begin();
      while ( c != children.end() ) {
        unlink(item.get(),*c);
        c = children.begin();
      }
    }
  }

  m_head_since = 1;
  m_head_until = 0;

}

//=========================================================================
//  search the item with the given path and get its validity
//=========================================================================
bool UpdateManagerSvc::getValidity(const std::string path, Gaudi::Time& since, Gaudi::Time &until,
                                   bool path_to_db) {
  if ( FSMState() < Gaudi::StateMachine::INITIALIZED ){
    throw GaudiException("Service offline","UpdateManagerSvc::registerCondition",StatusCode::FAILURE);
  }
  // search
  const Item* item = findItem(path,path_to_db);
  if (item) {
    // copy IOV limits
    since = item->since;
    until = item->until;
    return true;
  }
  return false;
}
//=========================================================================
//  search the item with the given path and change its validity
//=========================================================================
void UpdateManagerSvc::setValidity(const std::string path, const Gaudi::Time& since, const Gaudi::Time &until,
                                   bool path_to_db) {
  if ( FSMState() < Gaudi::StateMachine::INITIALIZED ){
    throw GaudiException("Service offline","UpdateManagerSvc::registerCondition",StatusCode::FAILURE);
  }

  if (!path_to_db) { // the DDS path is unique
    // search
    Item* item = findItem(path,path_to_db);
    if (item) {
      // set the validity and propagate up
      item->changeValidity(since,until);
      // if the object has already been loaded we should also change its validity
      if (item->vdo) item->vdo->setValidity(since,until);
    }
  } else { // a CondDB path can contain many objects
    auto i = m_all_items.begin();
    while ( i !=  m_all_items.end() ) {
      if ( (*i)->match(path,path_to_db) ) {
        // set the validity and propagate up
        (*i)->changeValidity(since,until);
        // if the object has already been loaded we should also change its validity
        if ((*i)->vdo) (*i)->vdo->setValidity(since,until);
      }
      ++i;
    }
  }
  // adjust head validity
  if ( m_head_since < since ) m_head_since = since;
  if ( m_head_until > until ) m_head_until = until;
}

//=========================================================================
//  Handle BeginEvent incident
//=========================================================================
void UpdateManagerSvc::handle(const Incident &inc) {
  if ( inc.type() == IncidentType::BeginEvent ) {
    if( msgLevel(MSG::DEBUG) )
      debug() << "New BeginEvent incident received" << endmsg;
    StatusCode sc = UpdateManagerSvc::newEvent();
    if (!sc.isSuccess()) {
      fatal() << "***** The update failed. I schedule a stop of the run *****" << endmsg;
      m_evtProc->stopRun();
      // The exception is ignored by the IncidentSvc
      // throw UpdateManagerException("Failed to perform the update","*UpdateManagerSvc*",sc);
    }
  }
}

//=========================================================================
//  Locking functionalities
//=========================================================================
void UpdateManagerSvc::acquireLock(){
#ifndef WIN32
  pthread_mutex_lock(&m_busy);
#endif
}
void UpdateManagerSvc::releaseLock(){
#ifndef WIN32
  pthread_mutex_unlock(&m_busy);
#endif
}

//=============================================================================
