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
#ifndef MCSTATTOOLS_GENFSRSTAT_H
#define MCSTATTOOLS_GENFSRSTAT_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

// from Event
#include "Event/GenFSR.h"

// from Kernel
#include "FSRAlgs/IFSRNavigator.h"


/** @class GenFSRStat GenFSRStat.h MCStatTools/GenFSRStat.h
 *
 *
 *  @author Davide Fazzini
 *  @date   2015-07-29
 */
class GenFSRStat : public GaudiAlgorithm {
public:
  /// Standard constructor
  GenFSRStat( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

private:

  void printHtmlFSR();  // Print the GenFSR in a file .xml

  void writeGeneratorCounters(LHCb::GenFSR* genFRS, int count);
  void writeCutEfficiencies(LHCb::GenFSR* genFRS, int count);
  void writeGenHadronCounters(LHCb::GenFSR* genFRS, int countGen);
  void writeAccHadronCounters(LHCb::GenFSR* genFRS, int countAcc);
  void writeGeneratorCrossSections(LHCb::GenFSR* genFRS, int count);
  void writeFinalComments(LHCb::GenFSR* genFSR);

  const std::string getCurrentTime();
  const std::string getEvtTypeDesc(int evtType);

  SmartIF<IDataProviderSvc> m_fileRecordSvc;

  std::string m_fileRecordName;     // location of FileRecords
  std::string m_FSRName;            // specific tag of summary data in FSR
  std::string m_htmlFileName;       // html file name
  std::string m_htmlFileLocation;   // html file location
  std::ofstream m_htmlFile;         // file html

  IFSRNavigator* m_navigatorTool = nullptr;   // tool to navigate FSR

  // Informations not stored in the FSR
  std::string m_dddb;
  std::string m_simCond;
  std::string m_gaussVersion;
  std::string m_appConfigFile;
  std::string m_appConfigVersion;
  std::string m_prodID;

};
#endif // MCSTATTOOLS_GENFSRSTAT_H
