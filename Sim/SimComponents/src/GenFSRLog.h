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
#ifndef GENFSRLOG_H
#define GENFSRLOG_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IDataProviderSvc.h"

// from Event
#include "Event/GenFSR.h"

// from local
#include "FSRAlgs/IFSRNavigator.h"

/** @class GenFSRLog GenFSRLog.h
 *
 *
 *  @author Davide Fazzini
 *  @date   2015-06-23
 */

class GenFSRLog : public GaudiAlgorithm {
public:
  // Standard constructor
  GenFSRLog( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; // Algorithm initialization
  StatusCode execute() override;    // Algorithm execution
  StatusCode finalize() override;   // Algorithm finalization

  void printFSR(); // Print the GenFSR in a file .xml

private:
  Gaudi::Property<std::string> m_fileRecordName{this, "FileRecordLocation", "/FileRecords",
                                                "TES location where FSRs are persisted"};
  Gaudi::Property<std::string> m_FSRName{this, "FSRName", "/GenFSR", "Name of the genFSR tree"};
  Gaudi::Property<std::string> m_xmlOutputLocation{this, "XmlOutputLocation", "", "Path where to save the .xml output"};
  Gaudi::Property<std::string> m_xmlOutputName{this, "xmlOutputName", "GeneratorLogFSR.xml", "Name of the .xml output"};

  SmartIF<IDataProviderSvc> m_fileRecordSvc;
  IFSRNavigator*            m_navigatorTool = nullptr; // tool to navigate FSR
};
#endif // GENFSRLOG_H
