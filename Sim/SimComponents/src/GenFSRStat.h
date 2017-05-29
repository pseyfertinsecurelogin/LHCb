#ifndef MCSTATTOOLS_GENFSRSTAT_H
#define MCSTATTOOLS_GENFSRSTAT_H 1

#include <iosfwd>
#include <string>

// from Event
#include "Event/GenFSR.h"
// from Kernel
#include "FSRAlgs/IFSRNavigator.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StatusCode.h"

class IFSRNavigator;
class ISvcLocator;
namespace LHCb {
class GenFSR;
}  // namespace LHCb


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
