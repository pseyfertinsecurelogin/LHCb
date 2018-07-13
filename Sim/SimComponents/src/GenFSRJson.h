#ifndef GENFSRJSON_H 
#define GENFSRJSON_H 1

// Include files 
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

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

class GenFSRJson : public GaudiAlgorithm 
{
public: 
  /// Standard constructor
  GenFSRJson( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~GenFSRJson( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

private:

  void printJsonFSR(); // Print the GenFSR in a file .json

  void writeGeneratorCounters(LHCb::GenFSR* genFRS);
  void writeCutEfficiencies(LHCb::GenFSR* genFRS);
  void writeGenHadronCounters(LHCb::GenFSR* genFRS);
  void writeAccHadronCounters(LHCb::GenFSR* genFRS);
  void writeGeneratorCrossSections(LHCb::GenFSR* genFRS);  
  void writeGlobalStat(LHCb::GenFSR* genFSR);

  const std::string getCurrentTime();  
  const std::string getEvtTypeDesc(int evtType);

  SmartIF<IDataProviderSvc> m_fileRecordSvc;  

  std::string m_fileRecordName;   // location of FileRecords    
  std::string m_FSRName;   // specific tag of summary data in FSR   
  std::string m_jsonFileName;   // json file name   
  std::string m_jsonFileLocation;   // json file location  
  std::ofstream m_jsonFile;  // file json 

  IFSRNavigator* m_navigatorTool = nullptr; // tool to navigate FSR 

  // Informations not stored in the FSR  
  std::string m_dddb;
  std::string m_simCond;  
  std::string m_gaussVersion;
  std::string m_appConfigFile;  
  std::string m_appConfigVersion;  
  std::string m_prodID;  

};
#endif // GENFSRJSON_H
