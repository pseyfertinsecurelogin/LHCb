// Include file

// local
#include "GenFSRJson.h"

// from Event
#include "Event/GenFSR.h"
#include "Event/GenCountersFSR.h"
#include "Event/CrossSectionsFSR.h"

// other libraries
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <stdio.h>
#include "TSystem.h"

#define script_version "20180626"

//-----------------------------------------------------------------------------
// Implementation file for class : GenFSRJson
//
// 2018-06-26 : Davide Fazzini
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( GenFSRJson )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
GenFSRJson::GenFSRJson( const std::string& name,
                        ISvcLocator* pSvcLocator)
: GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty("FileRecordLocation", m_fileRecordName = "/FileRecords");
  declareProperty("FSRName"           , m_FSRName = "/GenFSR");
  declareProperty("JsonFileLocation"  , m_jsonFileLocation = "");
  declareProperty("JsonFileName"      , m_jsonFileName = "GenerationFSRnew.json");
  declareProperty("prodID"            , m_prodID = "");
  declareProperty("appConfigVersion"  , m_appConfigVersion = "");
  declareProperty("appConfigFile"     , m_appConfigFile = "");
  declareProperty("gaussVersion"      , m_gaussVersion = "");
  declareProperty("simCond"           , m_simCond = "");
  declareProperty("dddb"              , m_dddb = "");
}
//=============================================================================
// Destructor
//=============================================================================
GenFSRJson::~GenFSRJson() {} 

//=============================================================================
// Initialization
//=============================================================================
StatusCode GenFSRJson::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;

  // get the File Records service
  m_fileRecordSvc = service("FileRecordDataSvc", true);

  // prepare navigator tool 
  m_navigatorTool = tool<IFSRNavigator>("FSRNavigator", "FSRNavigator");

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode GenFSRJson::execute() {

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  setFilterPassed(true);  // Mandatory. Set to true if event is accepted.
  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode GenFSRJson::finalize() {

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Finalize" << endmsg;

  GenFSRJson::printJsonFSR();

  return GaudiAlgorithm::finalize();  // must be called after all other actions
}

//=============================================================================

//=============================================================================  
//  Print the GenFSR in a file json
//============================================================================= 

void GenFSRJson::printJsonFSR()
{  
  if (msgLevel(MSG::DEBUG)) debug() << "write to file: " << m_jsonFileName << endmsg;

  // root of store  
  std::string fileRecordRoot = m_fileRecordName;  
  int nFSRs = 0;

  //GenFSRJson::getEvtTypeDesc(100);

  // make an inventory of the FileRecord store  
  std::vector< std::string > addresses = m_navigatorTool->navigate(fileRecordRoot, m_FSRName);

  for(std::vector< std::string >::iterator iAddr=addresses.begin(); iAddr!=addresses.end(); iAddr++)
    nFSRs += 1;

  if(nFSRs != 0)
    for(std::vector< std::string >::iterator iAddr=addresses.begin(); iAddr!=addresses.end(); iAddr++)
    {
      std::string genRecordAddress = *iAddr;
      std::string line = "";

      // read GenFSR
      LHCb::GenFSR* genFSR = getIfExists<LHCb::GenFSR>(m_fileRecordSvc.get(), genRecordAddress);
      if (genFSR==NULL)
      {
        Warning("A genFSR record was not found").ignore();
        if (msgLevel(MSG::DEBUG)) debug() << genRecordAddress << " not found" << endmsg; 
      }
      else
      {
        int evtType = genFSR->getSimulationInfo("evtType", 0);
        int njobs = genFSR->getSimulationInfo("nJobs", 0);

        std::string genMethod = genFSR->getSimulationInfo("generationMethod", "");
        std::string genName = genFSR->getSimulationInfo("hardGenerator", "");
        std::string decFiles = genFSR->getSimulationInfo("decFiles", "");

        std::string evtType_str =  dynamic_cast<std::ostringstream*>(&(std::ostringstream() << evtType))->str();
        std::string njobs_str =  dynamic_cast<std::ostringstream*>(&(std::ostringstream() << njobs))->str();
;
        std::string time = "";

        time = getCurrentTime();

        LHCb::GenFSR::GeneratorCounter::iterator icounter;
        LHCb::GenFSR::GeneratorCounter genCounters = genFSR->getGenCounters();
        LHCb::GenFSR::CrossSection::iterator icross;
        LHCb::GenFSR::CrossSection crossSections = genFSR->getCrossSections();
                
        // read the already existing file     
        std::fstream readFile;
        std::string readFileName = m_jsonFileLocation + "GenerationFSR_" + m_appConfigFile + ".json";

        readFile.open(readFileName.c_str(), std::fstream::in);        
        // open the new file    
        m_jsonFile.open((m_jsonFileLocation + m_jsonFileName).c_str(), std::fstream::out);        

        if (m_jsonFile.is_open())
        {
          if (msgLevel(MSG::DEBUG)) 
            debug() << "Json file: " << m_jsonFileLocation.c_str() << m_jsonFileName.c_str() << " created." << endmsg;

          if (readFile.is_open())
          {
            if (msgLevel(MSG::DEBUG)) 
              debug() << "Read file " << readFileName << " opened" << endmsg;
          }
          // the file does not exist, create it
          else
          {
            if (msgLevel(MSG::DEBUG)) 
              debug() << readFileName << " does not exist, creating it now." << endmsg;

            std::string evtDesc = getEvtTypeDesc(evtType);

            m_jsonFile << "{\n   \"APPCONFIG_file\": \"" << m_appConfigFile << "\",\n";
            m_jsonFile << "   \"APPCONFIG_version\": \"" << m_appConfigVersion << "\",\n";
            m_jsonFile << "   \"evtType\": \"" << evtType_str << "\",\n";
            m_jsonFile << "   \"evtTypeDesc\": \"" << evtDesc << "\",\n";
            m_jsonFile << "   \"DecFiles_version\": \"" << decFiles << "\",\n";
            m_jsonFile << "   \"Hard Generator\": \"" << genName << "\",\n";
            m_jsonFile << "   \"Gauss_version\": \"" << m_gaussVersion << "\",\n";
            m_jsonFile << "   \"DDDB\": \"" << m_dddb << "\",\n";
            m_jsonFile << "   \"SIMCOND\": \"" << m_simCond << "\",\n";
            m_jsonFile << "   \"genTimeStamp\": \"" << time << "\",\n";
            
            writeGeneratorCounters(genFSR);
            writeGenHadronCounters(genFSR);
            writeAccHadronCounters(genFSR);
            writeGeneratorCrossSections(genFSR);            
            writeCutEfficiencies(genFSR);
            writeGlobalStat(genFSR);

            m_jsonFile << "   \"prodID\": \"" << m_prodID << "\",\n";
            m_jsonFile << "   \"nb_jobs\": \"" << njobs_str << "\",\n";
            m_jsonFile << "   \"script_version\": \"" << script_version << "\"\n}";            
          }
          
          m_jsonFile.close(); 
          
          // remove the old file
          std::string originalFile = m_jsonFileLocation + "GenerationFSR_" + m_appConfigFile + ".json";
          remove(originalFile.c_str());          
          std::string newFile = m_jsonFileLocation + m_jsonFileName;
          // create the new .json file
          rename(newFile.c_str(),originalFile.c_str());
        }
      }
    }
}

//=============================================================================   
//  Add generator level counters in the json file                                 
//=============================================================================   

void GenFSRJson::writeGeneratorCounters(LHCb::GenFSR* genFSR)
{
  LHCb::GenFSR::GeneratorCounter::iterator icounter;
  LHCb::GenFSR::GeneratorCounter genCounters = genFSR->getGenCounters();
  int key = 0;
  std::string name = "", fullName = "";
  longlong after = 0, before = 0;
  double fraction = 0, error = 0, C = 1;  
  bool flag = true;

  LHCb::CrossSectionsFSR crossFSR;
  std::map<std::string,std::string> mapCross = crossFSR.getFullNames();
  LHCb::GenCountersFSR counterFSR;  
  std::map<std::string,std::string> mapCounter = counterFSR.getFullNames();

  m_jsonFile << "   \"InteractionCounters\": [\n";
  
  for(icounter = genCounters.begin(); icounter != genCounters.end(); icounter++)
  {
    key = icounter->first;
    if(key > 24) continue;

    name =  LHCb::GenCountersFSR::CounterKeyToString(key);
    if(name == "BeforeFullEvt" || name == "AfterFullEvt") continue;
    
    before = genFSR->getDenominator(key);
    after = genFSR->getGenCounterInfo(key).second;

    if(before == 0) continue;
    if(key >= 10)
    {
      int keycs = LHCb::CrossSectionsFSR::CrossSectionKeyToType("MBCrossSection");
      C = genFSR->getCrossSectionInfo(keycs).second; 
    }
    else
      C = 1;

    if(name == "EvtGenerated") after = genFSR->getGenCounterInfo(key+1).second;
    else after = icounter->second.second;
    if(after == 0) continue;

    name =  LHCb::CrossSectionsFSR::CrossSectionKeyToString(key);
    if(name == "MeanNoZeroPUInt" || name == "MeanPUInt" || name == "MeanPUIntAcc") flag = false;
    else flag = true;

    fraction = genFSR->getEfficiency(after, before, C);
    error = genFSR->getEfficiencyError(after, before, C, flag);
    fullName = mapCross[name];    

    m_jsonFile << "      {\"descr\": \"" << fullName << "\",\n";
    m_jsonFile << "       \"type\": \"" << "counter" << "\",\n";
    m_jsonFile << "       \"value\": " << fraction << ",\n";
    m_jsonFile << "       \"error\": " << error << ",\n";
    m_jsonFile << "      },\n";
  }

  m_jsonFile << "   ],\n";
}

//=============================================================================    
//  Add generator level cut efficiencies in the xml file                           
//=============================================================================    
void GenFSRJson::writeCutEfficiencies(LHCb::GenFSR* genFSR)
{
  LHCb::GenFSR::GeneratorCounter::iterator icounter;
  LHCb::GenFSR::GeneratorCounter genCounters = genFSR->getGenCounters();
  int key = 0;
  std::string name = "", fullName = "";
  longlong after = 0, before = 0;  
  double fraction = 0, error = 0;

  LHCb::GenCountersFSR counterFSR;
  std::map<std::string,std::string> mapCounter = counterFSR.getFullNames();
  LHCb::CrossSectionsFSR crossFSR;
  std::map<std::string,std::string> mapCross = crossFSR.getFullNames();

  m_jsonFile << "   \"SignalCounters\": [\n";

  for(icounter = genCounters.begin(); icounter != genCounters.end(); icounter++)
  {
    key = icounter->first;
    name =  LHCb::GenCountersFSR::CounterKeyToString(key);
    
    if(name != "AfterFullEvt" && name != "AfterLevelCut" && 
       name != "EvtSignal" && name != "EvtantiSignal" &&
       name != "AfterPCut"  && name != "AfterantiPCut") continue;

    before = genFSR->getDenominator(key);
    if(before == 0) continue;
    after    = icounter->second.second;    
    if(after == 0) continue;

    fraction = genFSR->getEfficiency(after, before);
    error = genFSR->getEfficiencyError(after, before);
    
    name =  LHCb::CrossSectionsFSR::CrossSectionKeyToString(key);
    fullName = mapCross[name];

    m_jsonFile << "      {\"descr\": \"" << fullName << "\",\n";
    m_jsonFile << "       \"type\": \"" << "counter" << "\",\n";
    m_jsonFile << "       \"value\": " << fraction << ",\n";
    m_jsonFile << "       \"error\": " << error << ",\n";
    m_jsonFile << "      },\n";
  }

  m_jsonFile << "   ],\n";
}

//=============================================================================    
//  Add generated hadron counters in the json file                                 
//=============================================================================    

void GenFSRJson::writeGenHadronCounters(LHCb::GenFSR* genFSR)
{
  LHCb::GenFSR::GeneratorCounter::iterator icounter;
  LHCb::GenFSR::GeneratorCounter genCounters = genFSR->getGenCounters();
  int key = 0;
  std::string name = "", fullName = "";
  longlong after = 0, before = 0;
  double fraction = 0, error = 0;
  
  LHCb::CrossSectionsFSR crossFSR;
  std::map<std::string,std::string> mapCross = crossFSR.getFullNames();

  m_jsonFile << "   \"HadronCounters\": [\n";

  for(icounter = genCounters.begin(); icounter != genCounters.end(); icounter++)
  {
    key = icounter->first;
    
    // Generated Hadron Fractions   
    if((key>=30 && key<=40) || (key>=55 && key<=63) || (key>=75 && key<=77) || (key>=85 && key<=87))
    {
      before = genFSR->getDenominator(key);
      if(before == 0) continue;
      // CHECK!!!!!!!!
      after = genFSR->getGenCounterInfo(key).second;
      //after = icounter->second.second;
      if(after == 0) continue;
      
      fraction = genFSR->getEfficiency(after, before);
      error = genFSR->getEfficiencyError(after, before);

      name = LHCb::CrossSectionsFSR::CrossSectionKeyToString(key);
      fullName = mapCross[name];

      m_jsonFile << "      {\"descr\": \"" << fullName << "\",\n";
      m_jsonFile << "       \"type\": \"" << "counter" << "\",\n";
      m_jsonFile << "       \"value\": " << fraction << ",\n";
      m_jsonFile << "       \"error\": " << error << ",\n";
      m_jsonFile << "      },\n";
    }
  }
}
      
//=============================================================================    
//  Add accepted hadron counters in the json file                                  
//=============================================================================    

void GenFSRJson::writeAccHadronCounters(LHCb::GenFSR* genFSR)
{
  LHCb::GenFSR::GeneratorCounter::iterator icounter;
  LHCb::GenFSR::GeneratorCounter genCounters = genFSR->getGenCounters();
  int key = 0;
  longlong after = 0, before = 0;
  double fraction = 0, error = 0;
  std::string name = "", fullName = "";
  
  LHCb::CrossSectionsFSR crossFSR;  
  std::map<std::string,std::string> mapCross = crossFSR.getFullNames();

  for(icounter = genCounters.begin(); icounter != genCounters.end(); icounter++)
  {
    key = icounter->first;
    // Accepted Hadron Fractions                                                   
    if ((key>=41 && key<=51) || (key>=64 && key<=72) || (key>=78 && key<=80) || (key>=88 && key<=90))
    {
      before = genFSR->getDenominator(key);
      if(before == 0) continue;
      after = genFSR->getGenCounterInfo(key).second;
      if(after == 0) continue;

      fraction = genFSR->getEfficiency(after, before);
      error    = genFSR->getEfficiencyError(after, before);

      name = LHCb::CrossSectionsFSR::CrossSectionKeyToString(key);
      fullName = mapCross[name];
      
      m_jsonFile << "      {\"descr\": \"" << fullName << "\",\n";
      m_jsonFile << "       \"type\": \"" << "counter" << "\",\n";
      m_jsonFile << "       \"value\": " << fraction << ",\n";
      m_jsonFile << "       \"error\": " << error << ",\n";
      m_jsonFile << "      },\n";
    }
  }
  
  m_jsonFile << "   ],\n";
}

//=============================================================================    
//  Add generator cross-sections in the json file                                  
//=============================================================================    

void GenFSRJson::writeGeneratorCrossSections(LHCb::GenFSR* genFSR)
{
  LHCb::GenFSR::CrossSection::iterator icross;
  LHCb::GenFSR::CrossSection crossSections = genFSR->getCrossSections();
  std::string name = "";  
  double value = 0;

  m_jsonFile << "   \"HardGeneratorCrossSection\": [\n";
  
  for(icross = crossSections.begin(); icross != crossSections.end(); icross++)
  {
    name = icross->second.first;
    value = icross->second.second;
    if(value == 0 || std::isnan(value) != 0) continue;

    m_jsonFile << "      {\"descr\": \"" << name.c_str() << "\",\n";
    m_jsonFile << "       \"type\": \"" << "cross-section" << "\",\n";
    m_jsonFile << "       \"value\": " << value << ",\n";
    m_jsonFile << "      },\n";
  }
  
  m_jsonFile << "   ],\n";
}

//=============================================================================  
//  Add global statistics in the json file
//=============================================================================

void GenFSRJson::writeGlobalStat(LHCb::GenFSR* genFSR)
{
  LHCb::GenFSR::GeneratorCounter::iterator icounter;
  LHCb::GenFSR::GeneratorCounter genCounters = genFSR->getGenCounters();
  int key = 0;
  std::string name = "";
  longlong countEvtGen = 0, countEvtAcc = 0, countIntGen = 0, countIntAcc = 0;
  
  LHCb::GenCountersFSR counterFSR;  
  std::map<std::string,std::string> mapCounter = counterFSR.getFullNames();

  m_jsonFile << "   \"globStat\": [\n";
  
  for(icounter = genCounters.begin(); icounter != genCounters.end(); icounter++)
  {
    key = icounter->first;    
    if(key > 5) continue;

    name =  LHCb::GenCountersFSR::CounterKeyToString(key);

    if(name == "EvtGenerated") countEvtGen = icounter->second.second;
    if(name == "IntGenerated") countIntGen = icounter->second.second;
    if(name == "EvtAccepted") countEvtAcc = icounter->second.second;
    if(name == "IntAccepted") countIntAcc = icounter->second.second;
  }
  
  m_jsonFile << "      {\"descr\": \"Number of accepted events/generated events\",\n";
  m_jsonFile << "       \"type\": \"" << "fraction" << "\",\n";
  m_jsonFile << "       \"numer\": " << countEvtAcc << ",\n";  
  m_jsonFile << "       \"denom\": " << countEvtGen << ",\n";
  m_jsonFile << "      },\n";

  m_jsonFile << "      {\"descr\": \"Number of interactions in accepted events/generated interactions\",\n";
  m_jsonFile << "       \"type\": \"" << "fraction" << "\",\n";
  m_jsonFile << "       \"numer\": " << countIntAcc << ",\n";
  m_jsonFile << "       \"denom\": " << countIntGen << ",\n";
  m_jsonFile << "      },\n   ],\n";
}


//=============================================================================       
//  Get current date/time, format is YYYY-MM-DD.HH:mm:ss
//=============================================================================

// naive function to get the current time
const std::string GenFSRJson::getCurrentTime()
{
  time_t now = time(0);
  struct tm  tstruct;
  char time[80];
  tstruct = *localtime(&now);  
  strftime(time, sizeof(time), "%c", &tstruct);

  return time; 
}

//=============================================================================
//  Get correct event type description
//=============================================================================

const std::string GenFSRJson::getEvtTypeDesc(int evtType)
{
  std::string path_decFile = gSystem->Getenv("DECFILESROOT");
  std::string path_desc = "", line_ref = "", line_stop = "";
  std::string line = "", description = "";
  std::fstream file_desc;
  std::string evtType_str = dynamic_cast<std::ostringstream*>(&(std::ostringstream() << evtType))->str();
  std::size_t pos_ref, pos_end;
  int count = 0;

  if(path_decFile != NULL)
  {
    path_desc = path_decFile + "/options/" + evtType_str + ".py";
    file_desc.open(path_desc, std::fstream::in);
    line_ref = "$DECFILESROOT/dkfiles/";    
    line_stop = ".dec\"";

    while(getline(file_desc,line))
    {      
      pos_ref = line.find(line_ref);

      if(pos_ref != std::string::npos)
      {
        pos_end = line.find(line_stop);
        pos_ref = pos_ref + line_ref.length();
        count = pos_end - pos_ref;
        description = line.substr(pos_ref, count);
        break; 
      } 
    }
    file_desc.close();    
  }

  return description; 
}
