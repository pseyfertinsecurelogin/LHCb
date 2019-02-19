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

#ifndef GENFSRJSON_H
#define GENFSRJSON_H 1

// Include files
// From Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IDataProviderSvc.h"

// from Event
#include "Event/GenFSR.h"

// from Kernel
#include "FSRAlgs/IFSRNavigator.h"

/** @class GenFSRJson GenFSRJson.h
 *
 *
 *  @author Davide Fazzini
 *  @date   2018-06-26
 */

class GenFSRJson : public GaudiAlgorithm {
public:
  /// Standard constructor
  GenFSRJson( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    // Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

  void printFSR(); // Print the GenFSR in a file .json

private:
  Gaudi::Property<std::string> m_fileRecordName{this, "FileRecordLocation", "/FileRecords",
                                                "TES location where FSRs are persisted"};
  Gaudi::Property<std::string> m_FSRName{this, "FSRName", "/GenFSR", "Name of the genFSR tree"};
  Gaudi::Property<std::string> m_prodID{this, "prodID", "", "Production ID used in the generation"};
  Gaudi::Property<std::string> m_appConfigVersion{this, "appConfigVersion", "",
                                                  "Version of AppConfig used in the simulation"};
  Gaudi::Property<std::string> m_appConfigFile{this, "appConfigFile", "", "Name of the AppConfig file"};
  Gaudi::Property<std::string> m_gaussVersion{this, "gaussVersion", "", "Gauss version used in the simulation"};
  Gaudi::Property<std::string> m_simCond{this, "simCond", "", "Tag for the SimCond database"};
  Gaudi::Property<std::string> m_dddb{this, "dddb", "", "Tag for the DDDB database"};
  Gaudi::Property<std::string> m_jsonOutputLocation{this, "jsonOutputLocation", "",
                                                    "Path where to save the .json output"};
  Gaudi::Property<std::string> m_jsonOutputName{this, "jsonOutputName", "GenerationFSR_" + m_appConfigFile + ".json",
                                                "Name of the .json output"};

  SmartIF<IDataProviderSvc> m_fileRecordSvc;
  IFSRNavigator*            m_navigatorTool = nullptr; // tool to navigate FSR
};
#endif // GENFSRJSON_H
