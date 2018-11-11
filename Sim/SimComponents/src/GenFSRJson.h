#ifndef GENFSRJSON_H 
#define GENFSRJSON_H 1

// Include files 
// From GaudiKernel
#include "GaudiKernel/Service.h"
#include "GaudiKernel/SmartDataPtr.h"
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

class GenFSRJson : public extends<Service, IGenFSROutSvc>
{
public: 
  /// Standard constructor
  using base_class::base_class;
  
  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode finalize  () override;    ///< Algorithm finalization

  void printFSR() override; // Print the GenFSR in a file .json 

private:

  Gaudi::Property<std::string> m_fileRecordName{this, "FileRecordLocation", 
      "/FileRecords", "TES location where FSRs are persisted"};
  Gaudi::Property<std::string> m_FSRName{this, "FSRName", "/GenFSR", "Name of the genFSR tree"};
  Gaudi::Property<std::string> m_prodID{this, "prodID", "", "Production ID used in the generation"}; 
  Gaudi::Property<std::string> m_appConfigVersion{this, "appConfigVersion", "", 
      "Version of AppConfig used in the simulation"}; 
  Gaudi::Property<std::string> m_appConfigFile{this, "appConfigFile", "", "Name of the AppConfig file"}; 
  Gaudi::Property<std::string> m_gaussVersion{this, "gaussVersion", "", 
      "Gauss version used in the simulation"}; 
  Gaudi::Property<std::string> m_simCond{this, "simCond", "", "Tag for the SimCond database"}; 
  Gaudi::Property<std::string> m_dddb{this, "dddb", "", "Tag for the DDDB database"};
  Gaudi::Property<std::string> m_jsonOutputLocation{this, "JsonOutputLocation", "",
      "Path where to save the .json output"};
  Gaudi::Property<std::string> m_jsonOutputName{this, "JsonOutputName", "GenerationFSR_" + m_appConfigFile + ".json",
      "Name of the .json output"};
  SmartIF<IDataProviderSvc> m_fileRecordSvc;
  ToolHandle<IFSRNavigator> m_navigatorTool{this, "FSRNavigator", "FSRNavigator"};
};
#endif // GENFSRJSON_H






