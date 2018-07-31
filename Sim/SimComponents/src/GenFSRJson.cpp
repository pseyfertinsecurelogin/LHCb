// Include file

// local
#include "GenFSRJson.h"

// from Event
#include "Event/GenFSR.h"
#include "Event/GenCountersFSR.h"
#include "Event/CrossSectionsFSR.h"

// From GaudiKernel
#include "GaudiKernel/System.h"
#include "GaudiKernel/Time.h""

// Gaudi Functional
#include "GaudiAlg/Consumer.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiAlg/GaudiCommon.h"

// other libraries
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include "TSystem.h"

#define script_version "20180626"

//-----------------------------------------------------------------------------
// Implementation file for class : GenFSRJson
//
// 2018-06-26 : Davide Fazzini
//-----------------------------------------------------------------------------

namespace 
{
  std::string getCurrentTime() { 
    return Gaudi::Time::current().format(true);
  }  
  
  const std::string getEvtTypeDesc(int evtType) {
    std::string path_decFile = System::getEnv("DECFILESROOT");
    std::string path_desc = "", line_ref = "", line_stop = "";
    std::string line = "", description = "";
    std::fstream file_desc;
    std::string evtType_str = std::to_string(evtType);
    std::size_t pos_ref, pos_end;
    
    int count = 0;

    if(path_decFile != nullptr)
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
}

// Instantiation of a static factory class used by clients to create
// instances of this service
DECLARE_COMPONENT( GenFSRJson )

//=============================================================================   
// Standard constructor, initializes variables 
//=============================================================================  
GenFSRJson::GenFSRJson( const std::string& name,
                        ISvcLocator* pSvcLocator)
//: GaudiAlgorithm (name, pSvcLocator){}
: base_class(name, pSvcLocator){}

   

//=============================================================================
// Initialization
//=============================================================================
StatusCode GenFSRJson::initialize() {
  
  StatusCode sc = Service::initialize(); // must be executed first
  //StatusCode sc = GaudiAlgorithm::initialize();
  
  if ( sc.isFailure() ) return sc;  // error printed already by Service

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;

  // get the File Records service
  m_fileRecordSvc = service("FileRecordDataSvc", true);
  
  return StatusCode::SUCCESS;
}

/*
//=============================================================================    
// Main execution    
//=============================================================================  
StatusCode GenFSRJson::execute() 
{
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;
  
  return StatusCode::SUCCESS;  
}
*/

//=============================================================================
//  Finalize
//=============================================================================
StatusCode GenFSRJson::finalize() {

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Finalize" << endmsg;

  GenFSRJson::printFSR();
  
  //return GaudiAlgorithm::finalize();
  return Service::finalize();
  
}

//=============================================================================

//=============================================================================  
//  Print the GenFSR in a file json
//============================================================================= 

void GenFSRJson::printFSR()
{  
  if (msgLevel(MSG::DEBUG)) debug() << "write to file: " << m_jsonFileName << endmsg;

  // root of store  
  std::string fileRecordRoot = m_fileRecordName;

  // make an inventory of the FileRecord store  
  std::vector< std::string > addresses = m_navigatorTool.get()->navigate(fileRecordRoot, m_FSRName);
  int nFSRs = addresses.size();  

  if(nFSRs != 0)
    for(const auto& genRecordAddress: addresses)
    {
      std::string line = "";

      // read GenFSR
      const LHCb::GenFSR* genFSR = getIfExists<LHCb::GenFSR>(m_fileRecordSvc.get(), genRecordAddress);
      //const LHCb::GenFSR* genFSR = get<LHCb::GenFSR>(m_fileRecordSvc.get(), genRecordAddress);
      
      if (genFSR == nullptr)
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
        
        std::string evtType_str = std::to_string(evtType);
        std::string njobs_str = std::to_string(njobs);
        std::string time = ::getCurrentTime();

        LHCb::GenFSR::GeneratorCounter genCounters = genFSR->genCounters();
        LHCb::GenFSR::CrossSection::iterator icross;
        LHCb::GenFSR::CrossSection crossSections = genFSR->crossSections();
                
        // open the new file    
        std::ofstream m_jsonFile;
        m_jsonFile.open(m_jsonFileLocation + m_jsonFileName, std::fstream::out | std::ofstream::app);

        if (m_jsonFile.is_open())
        {
          if (msgLevel(MSG::DEBUG)) 
            debug() << "Json file: " << m_jsonFileLocation << m_jsonFileName << " created." << endmsg;

          if (msgLevel(MSG::DEBUG)) 
            debug() << m_jsonFileLocation << "GenerationFSR_" << m_appConfigFile << ".json" << 
              " does not exist, creating it now." << endmsg;

          std::string evtDesc = ::getEvtTypeDesc(evtType);

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
          
          writeGeneratorCounters(*genFSR, m_jsonFile);
          writeGenHadronCounters(*genFSR, m_jsonFile);
          writeAccHadronCounters(*genFSR, m_jsonFile);
          writeGeneratorCrossSections(*genFSR, m_jsonFile);            
          writeCutEfficiencies(*genFSR, m_jsonFile);
          writeGlobalStat(*genFSR, m_jsonFile);
          
          m_jsonFile << "   \"prodID\": \"" << m_prodID << "\",\n";
          m_jsonFile << "   \"nb_jobs\": \"" << njobs_str << "\",\n";
          m_jsonFile << "   \"script_version\": \"" << script_version << "\"\n}";
          
          m_jsonFile.close();
        }
          
        // remove the old file
        std::string originalFile = m_jsonFileLocation + "GenerationFSR_" + m_appConfigFile + ".json";
        remove(originalFile.c_str());          
        std::string newFile = m_jsonFileLocation + m_jsonFileName;
        // create the new .json file
        rename(newFile.c_str(),originalFile.c_str());
        }
    }
}


//=============================================================================   
//  Add generator level counters in the json file                                 
//=============================================================================   

void GenFSRJson::writeGeneratorCounters(const LHCb::GenFSR& genFSR, std::ofstream& m_jsonFile)
{ 
  LHCb::CrossSectionsFSR crossFSR;
  std::map<std::string,std::string> mapCross = crossFSR.getFullNames();
  LHCb::GenCountersFSR counterFSR;  
  std::map<std::string,std::string> mapCounter = counterFSR.getFullNames();

  m_jsonFile << "   \"InteractionCounters\": [\n";

  for(auto const& counter : genFSR.genCounters())
  {
    auto key = counter.first;

    if(key > 24) continue;

    std::string name =  LHCb::GenCountersFSR::CounterKeyToString(key);
    if(name == "BeforeFullEvt" || name == "AfterFullEvt") continue;
    
    longlong before = genFSR.getDenominator(key);

    double C = 1;
    if(before == 0) continue;
    if(key >= 10)
    {
      int keycs = LHCb::CrossSectionsFSR::CrossSectionKeyToType("MBCrossSection");
      C = genFSR.getCrossSectionInfo(keycs).second; 
    }
    else
      C = 1;

    longlong after = (name == "EvtGenerated" ?  genFSR.getGenCounterInfo(key+1).second
                      : counter.second.second);
    if(after == 0) continue;

    name =  LHCb::CrossSectionsFSR::CrossSectionKeyToString(key);
    bool flag = (name != "MeanNoZeroPUInt" && name != "MeanPUInt" && name != "MeanPUIntAcc");

    double fraction = genFSR.getEfficiency(after, before, C);
    double error = genFSR.getEfficiencyError(after, before, C, flag);
    std::string fullName = mapCross[name];    

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
void GenFSRJson::writeCutEfficiencies(const LHCb::GenFSR& genFSR, std::ofstream& m_jsonFile)
{
  LHCb::GenCountersFSR counterFSR;
  std::map<std::string,std::string> mapCounter = counterFSR.getFullNames(); 
  LHCb::CrossSectionsFSR crossFSR;
  std::map<std::string,std::string> mapCross = crossFSR.getFullNames();

  m_jsonFile << "   \"SignalCounters\": [\n";

  for(const auto& counter : genFSR.genCounters())
  {
    auto key = counter.first;
    std::string name =  LHCb::GenCountersFSR::CounterKeyToString(key);
    
    if(name != "AfterFullEvt" && name != "AfterLevelCut" && 
       name != "EvtSignal" && name != "EvtantiSignal" &&
       name != "AfterPCut"  && name != "AfterantiPCut") continue;

    longlong before = genFSR.getDenominator(key);
    if(before == 0) continue;
    longlong after    = counter.second.second;    
    if(after == 0) continue;

    double fraction = genFSR.getEfficiency(after, before);
    double error = genFSR.getEfficiencyError(after, before);
    
    name =  LHCb::CrossSectionsFSR::CrossSectionKeyToString(key);
    std::string fullName = mapCross[name];

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

void GenFSRJson::writeGenHadronCounters(const LHCb::GenFSR& genFSR, std::ofstream& m_jsonFile)
{
  LHCb::CrossSectionsFSR crossFSR;
  std::map<std::string,std::string> mapCross = crossFSR.getFullNames();

  m_jsonFile << "   \"HadronCounters\": [\n";

  for(const auto& counter : genFSR.genCounters())
  {
    auto key = counter.first;
    
    // Generated Hadron Fractions   
    if((key>=30 && key<=40) || (key>=55 && key<=63) || (key>=75 && key<=77) || (key>=85 && key<=87))
    {
      longlong before = genFSR.getDenominator(key);
      if(before == 0) continue;
      longlong after = counter.second.second;
      if(after == 0) continue;
      
      double fraction = genFSR.getEfficiency(after, before);
      double error = genFSR.getEfficiencyError(after, before);

      std::string name = LHCb::CrossSectionsFSR::CrossSectionKeyToString(key);
      std::string fullName = mapCross[name];

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

void GenFSRJson::writeAccHadronCounters(const LHCb::GenFSR& genFSR, std::ofstream& m_jsonFile)
{
  LHCb::CrossSectionsFSR crossFSR;  
  std::map<std::string,std::string> mapCross = crossFSR.getFullNames();

  for(const auto& counter : genFSR.genCounters())
  {
    auto key = counter.first;
    // Accepted Hadron Fractions                                                   
    
    if ((key>=41 && key<=51) || (key>=64 && key<=72) || (key>=78 && key<=80) || (key>=88 && key<=90))
    {
      longlong before = genFSR.getDenominator(key);
      if(before == 0) continue;
      longlong after = counter.second.second;
      if(after == 0) continue;

      double fraction = genFSR.getEfficiency(after, before);
      double error    = genFSR.getEfficiencyError(after, before);

      std::string name = LHCb::CrossSectionsFSR::CrossSectionKeyToString(key);
      std::string fullName = mapCross[name];
      
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

void GenFSRJson::writeGeneratorCrossSections(const LHCb::GenFSR& genFSR, std::ofstream& m_jsonFile)
{
  m_jsonFile << "   \"HardGeneratorCrossSection\": [\n";

  for(const auto& cross : genFSR.crossSections())
  {
    std::string name = cross.second.first;
    double value = cross.second.second;
    
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

void GenFSRJson::writeGlobalStat(const LHCb::GenFSR& genFSR, std::ofstream& m_jsonFile)
{
  longlong countEvtGen = 0, countEvtAcc = 0, countIntGen = 0, countIntAcc = 0;
  LHCb::GenCountersFSR counterFSR;  
  std::map<std::string,std::string> mapCounter = counterFSR.getFullNames();

  m_jsonFile << "   \"globStat\": [\n";
 
  for(const auto& counter : genFSR.genCounters())
  {
    auto key = counter.first;    
    if(key > 5) continue;

    std::string name =  LHCb::GenCountersFSR::CounterKeyToString(key);

    if(name == "EvtGenerated") countEvtGen = counter.second.second;
    if(name == "IntGenerated") countIntGen = counter.second.second;
    if(name == "EvtAccepted") countEvtAcc = counter.second.second;
    if(name == "IntAccepted") countIntAcc = counter.second.second;
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
