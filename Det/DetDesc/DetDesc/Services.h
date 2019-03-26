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
#ifndef DETDESC_SERVICES_H
#define DETDESC_SERVICES_H 1

#include "GaudiKernel/SmartIF.h"
#include <memory>
// Forward declarations
class ISvcLocator;
class IDataProviderSvc;
class IMessageSvc;
class IUpdateManagerSvc;

namespace DetDesc {
  class Services;
  using ServicesPtr = std::shared_ptr<Services>;

  /** @class Services Services.h DetDesc/Services.h
   *
   * This class is a container for accessors to different usefull services,
   * namely the service locator, message service and the detector data provider.
   * @author Sebastien Ponce
   */
  class Services {

  public:
    /**
     * the accessor to Service Locator
     * @exception GaudiException the service could not be located
     * @return pointer to message service
     */
    SmartIF<ISvcLocator>& svcLocator() const;
    /**
     * the accessor to Detector data provider
     * @exception GaudiException the service could not be located
     * @return pointer to detector data provider
     */
    IDataProviderSvc* detSvc() const;
    /**
     * the accessor to Message Service
     * @exception GaudiException the service could not be located
     * @return pointer to message service
     */
    IMessageSvc* msgSvc() const;

    /**
     * the accessor to Update Manager Service
     * @exception GaudiException the service could not be located
     * @return pointer to UpdateManagerSvc instance
     */
    IUpdateManagerSvc* updMgrSvc( bool create = false ) const;

    // static acessor
    static ServicesPtr services();

  protected:
    /** Default constructor */
    Services() = default;

  private:
    /// The service locator
    mutable SmartIF<ISvcLocator> m_svcLocator;
    /// The Detector data provider
    mutable SmartIF<IDataProviderSvc> m_detSvc;
    /// The Message service
    mutable SmartIF<IMessageSvc> m_msgSvc;
    /// The Update Manager Service
    mutable SmartIF<IUpdateManagerSvc> m_updMgrSvc;
  };
} // namespace DetDesc

#endif ///< DETDESC_DETDESC_H
