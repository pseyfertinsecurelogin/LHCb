// Include files
#include "DetDesc/Services.h"
#include <mutex>
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IUpdateManagerSvc.h"


namespace {
   // allow make_shared to actually construct Services
   // even though it has a protected constructor...
   struct ServicesMaker : DetDesc::Services { };

   static std::weak_ptr<DetDesc::Services> s_services;
   //TODO: do we have to add padding to avoid false sharing of mutexes??
   static std::mutex s_mutex;
   static std::mutex s_svcLoc_mutex;
   static std::mutex s_msgsvc_mutex;
   static std::mutex s_updsvc_mutex;
   static std::mutex s_detsvc_mutex;
}

/**
 * the accessor to Service Locator
 * @exception GaudiException the service could not be located
 * @return pointer to message service
 */
SmartIF<ISvcLocator>& DetDesc::Services::svcLocator() const {
  /// Get the service if necessary
  if (UNLIKELY(!m_svcLocator)){
    std::lock_guard lock(s_svcLoc_mutex);
    if (!m_svcLocator) m_svcLocator = Gaudi::svcLocator();
    if (!m_svcLocator) {
      throw GaudiException("DetDesc::ISvcLocator* points to NULL!",
                           "*DetDescException*" ,
                           StatusCode::FAILURE);
    }
  }
  return m_svcLocator;
}

/**
 * the accessor to Detector data provider
 * @exception GaudiException the service could not be located
 * @return pointer to detector data provider
 */
IDataProviderSvc* DetDesc::Services::detSvc() const {
  /// locate the service if necessary
  if (UNLIKELY(!m_detSvc)){
    std::lock_guard lock(s_detsvc_mutex);
    if (!m_detSvc) m_detSvc = svcLocator()->service ("DetectorDataSvc");
    if (!m_detSvc) {
      throw GaudiException
        ("DetDesc::Could not locate IDataProviderSvc='DetectorDataSvc'",
         "*DetDescException*" , StatusCode::FAILURE);
    }
  }
  // If already there, return the cached service
  return m_detSvc ;
}

/**
 * the accessor to Message Service
 * @exception GaudiException the service could not be located
 * @return pointer to message service
 */
IMessageSvc* DetDesc::Services::msgSvc() const {
  // locate the service if necessary
  if (UNLIKELY(!m_msgSvc)) {
    std::lock_guard lock(s_msgsvc_mutex);
    if (!m_msgSvc) m_msgSvc = svcLocator()->service("MessageSvc");
    if (!m_msgSvc) {
      throw GaudiException
        ("DetDesc::Could not locate IMessageSvc='MessageSvc'",
         "*DetDescException*" , StatusCode::FAILURE);
    }
  }
  return m_msgSvc;
}

/**
 * the accessor to Update Manager Service
 * @exception GaudiException the service could not be located
 * @return pointer to UpdateManagerSvc instance
 */
IUpdateManagerSvc* DetDesc::Services::updMgrSvc(bool create) const {
  // locate the service if necessary
  if (UNLIKELY(!m_updMgrSvc)) {
    std::lock_guard lock(s_updsvc_mutex);
    if (!m_updMgrSvc) m_updMgrSvc = svcLocator()->service("UpdateManagerSvc", create);
    if (!m_updMgrSvc)
      throw GaudiException
        ("DetDesc::Could not locate IUpdateManagerSvc='UpdateManagerSvc'",
         "*DetDescException*" , StatusCode::FAILURE);
  }
  return m_updMgrSvc;
}

/**
 * Gets a shared_ptr to the single instance of Services
 */


DetDesc::ServicesPtr DetDesc::Services::services() {
  auto sp = s_services.lock();
  if (UNLIKELY(!sp)) {
    std::lock_guard lock(s_mutex);
    sp = s_services.lock();
    if (!sp) {
      sp = std::make_shared<ServicesMaker>();
      s_services = sp;
    }
  }
  return sp;
}
