#ifndef GENFSRLOG_H
#define GENFSRLOG_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IDataManagerSvc.h"

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

  StatusCode initialize() override;  // Algorithm initialization
  StatusCode execute   () override;  // Algorithm execution
  StatusCode finalize  () override;  // Algorithm finalization

private:

  void printXmlFSR();       // Print the GenFSR in a file .xml

  void addGenCounters(LHCb::GenFSR* genFSR);
  void addGenFractions(LHCb::GenFSR* genFSR);
  void addGenCrossSections(LHCb::GenFSR* genFSR);
  void addGeneratorCrossSections(LHCb::GenFSR* genFSR);
  void addCutEfficiencies(LHCb::GenFSR* genFSR);

  SmartIF<IDataProviderSvc> m_fileRecordSvc;

  std::string m_fileRecordName;     // location of FileRecords
  std::string m_FSRName;            // specific tag of summary data in FSR
  std::string m_xmlFileName;        // xml file name
  std::ofstream m_xmlFile;          // file xml

private:

  IFSRNavigator* m_navigatorTool = nullptr;   // tool to navigate FSR

};
#endif // GENFSRLOG_H
