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
#ifndef GENFSRREAD_H
#define GENFSRREAD_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IDataProviderSvc.h"

// from Event
#include "Event/GenFSR.h"

// from Kernel
#include "FSRAlgs/IFSRNavigator.h"

/** @class GenFSRRead GenFSRRead.h
 *
 *
 *  @author Davide Fazzini
 *  @date   2015-06-23
 */

class GenFSRRead : public GaudiAlgorithm {
public:
  // Standard constructor
  GenFSRRead( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; // Algorithm initialization
  StatusCode execute() override;    // Algorithm execution
  StatusCode finalize() override;   // Algorithm finalization

  void printFSR(); // Print the GenFSR

private:
  Gaudi::Property<std::string> m_fileRecordName{this, "FileRecordLocation", "/FileRecords",
                                                "TES location where FSRs are persisted"};
  Gaudi::Property<std::string> m_FSRName{this, "FSRName", "/GenFSR", "Name of the genFSR tree"};

  SmartIF<IDataProviderSvc> m_fileRecordSvc;
  IFSRNavigator*            m_navigatorTool = nullptr; // tool to navigate FSR
};
#endif // GENFSRREAD_H
