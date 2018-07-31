#ifndef GENFSRJSON_H 
#define GENFSRJSON_H 1

// Include files 
// From Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
// From GaudiKernel
#include "GaudiKernel/Service.h"
// from Event
#include "Event/GenFSR.h"
// from Kernel 
#include "FSRAlgs/IFSRNavigator.h"
#include "FSRAlgs/IGenFSROutSvc.h"

/** @class GenFSRJson GenFSRJson.h
 *  
 *
 *  @author Davide Fazzini
 *  @date   2018-06-26
 */

//class GenFSRJson : public GaudiAlgorithm 
class GenFSRJson : public extends<Service, IGenFSROutSvc>
{
public: 
  /// Standard constructor
  GenFSRJson( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  //StatusCode execute   () override;    ///< Algorithm execution 
  StatusCode finalize  () override;    ///< Algorithm finalization

  void printFSR() override; // Print the GenFSR in a file .json 

private:

  Gaudi::Property<std::string> m_fileRecordName{this, "FileRecordLocation", "/FileRecords"};
  Gaudi::Property<std::string> m_FSRName{this, "FSRName", "/GenFSR"};
  Gaudi::Property<std::string> m_jsonFileLocation{this, "JsonFileLocation", ""}; 
  Gaudi::Property<std::string> m_jsonFileName{this, "JsonFileName", "GenerationFSRnew.json"}; 
  Gaudi::Property<std::string> m_prodID{this, "prodID", ""}; 
  Gaudi::Property<std::string> m_appConfigVersion{this, "appConfigVersion", ""}; 
  Gaudi::Property<std::string> m_appConfigFile{this, "appConfigFile", ""}; 
  Gaudi::Property<std::string> m_gaussVersion{this, "gaussVersion", ""}; 
  Gaudi::Property<std::string> m_simCond{this, "simCond", ""}; 
  Gaudi::Property<std::string> m_dddb{this, "dddb", ""};

  void writeGeneratorCounters(const LHCb::GenFSR& genFRS, std::ofstream& jsonFile);
  void writeCutEfficiencies(const LHCb::GenFSR& genFRS, std::ofstream& jsonFile);
  void writeGenHadronCounters(const LHCb::GenFSR& genFRS, std::ofstream& jsonFile);
  void writeAccHadronCounters(const LHCb::GenFSR& genFRS, std::ofstream& jsonFile);
  void writeGeneratorCrossSections(const LHCb::GenFSR& genFRS, std::ofstream& jsonFile);  
  void writeGlobalStat(const LHCb::GenFSR& genFSR, std::ofstream& jsonFile);
  
  SmartIF<IDataProviderSvc> m_fileRecordSvc;
  ToolHandle<IFSRNavigator> m_navigatorTool{this, "FSRNavigator", "FSRNavigator"};

};
#endif // GENFSRJSON_H
