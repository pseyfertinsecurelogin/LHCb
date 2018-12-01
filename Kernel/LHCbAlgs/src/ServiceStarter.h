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
#ifndef SRC_SERVICESTARTER_H
#define SRC_SERVICESTARTER_H 1
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

namespace LHCbAlgsTest
{

  /** @class ServiceStarter ServiceStarter.h src/ServiceStarter.h
   *
   * Simple test algorithm to trigger the instantiation of a service in
   * a specific phase (initialize, start or execute).
   *
   * @author Marco Clemencic
   * @date 09/01/2012
   */
  class ServiceStarter final : public GaudiAlgorithm
  {

  public:

    /// Standard constructor
    using GaudiAlgorithm::GaudiAlgorithm;

    StatusCode initialize() override;    ///< Algorithm initialization
    StatusCode start     () override;    ///< Algorithm start
    StatusCode execute   () override;    ///< Algorithm execution
    StatusCode finalize  () override;    ///< Algorithm finalization

  private:

    StatusCode i_retrieveService(const std::string &currentPhase);

  private:

    /// Service to get.
    Gaudi::Property<std::string> m_serviceName
    { this, "Service", {}, "Service to retrieve."};

    /// When to get the service;
    Gaudi::Property<std::string> m_phase
    { this, "Phase", {}, "When to retrieve the service (initialize, start, execute)." };

    /// Internal reference to the retrieved service.
    SmartIF<IService> m_service;

  };

}
#endif // SRC_SERVICESTARTER_H
