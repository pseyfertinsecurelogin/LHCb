/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef GENFSRMERGE_H
#define GENFSRMERGE_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/IGenericTool.h"            // Interface

// event model
#include "Event/GenFSR.h"

// local
#include "FSRNavigator.h"

/** @class GenMergeFSR GenMergeFSR.h
 *
 *
 *  @author Davide Fazzini
 *  @date   2015-06-25
 */

class GenFSRMerge : public GaudiAlgorithm
{
 public:
  // Standard constructor
  GenFSRMerge(const std::string& name, ISvcLocator* pSvcLocator);

  // Algorithm initialization, execution and finalization
  StatusCode initialize() override;
  StatusCode execute() override;
  StatusCode finalize() override;

 protected:
  // Merge the FSRs of one input file
  virtual StatusCode merge();

  // Reference to run records data service
  SmartIF<IDataProviderSvc> m_fileRecordSvc;

  // Location of FileRecords
  std::string m_FileRecordName;
  // Specific tag of summary data in FSR
  std::string m_FSRName;

 private:
  // Tool navigate FSR
  IFSRNavigator *m_navigatorTool = nullptr;

  int m_evtType;
  std::string m_genName;
  std::string m_method;

};

#endif // GENFSRMERGE_H
