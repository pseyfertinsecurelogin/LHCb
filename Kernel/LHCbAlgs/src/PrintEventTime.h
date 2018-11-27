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
#ifndef SRC_PRINTEVENTTIME_H
#define SRC_PRINTEVENTTIME_H 1
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

namespace LHCbAlgsTest {

  /** @class PrintEventTime PrintEventTime.h src/PrintEventTime.h
   *
   * Simple algorithm that prints the current event time.
   *
   * @author Marco Clemencic
   * @date 03/10/2011
   */
  class PrintEventTime: public GaudiAlgorithm
  {

  public:

    /// Standard constructor
    PrintEventTime(const std::string& name, ISvcLocator* pSvcLocator);
    virtual ~PrintEventTime() = default; ///< Destructor

    StatusCode initialize() override;    ///< Algorithm initialization
    StatusCode execute   () override;    ///< Algorithm execution
    StatusCode finalize  () override;    ///< Algorithm finalization

  private:

    SmartIF<IDetDataSvc> m_detDataSvc;

  };

}
#endif // SRC_PRINTEVENTTIME_H
