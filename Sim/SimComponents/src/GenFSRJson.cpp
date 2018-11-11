// Include file

// local
#include "GenFSRJson.h"

// from Event
#include "Event/GenFSR.h"
#include "Event/GenCountersFSR.h"
#include "Event/CrossSectionsFSR.h"

// From GaudiKernel
#include "GaudiKernel/System.h"
#include "GaudiKernel/Time.h"
#include "GaudiKernel/IDataProviderSvc.h"

// other libraries
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include "TSystem.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

using boost::property_tree::ptree;

#define script_version "20181108"

//-----------------------------------------------------------------------------
// Implementation file for class : GenFSRJson
//
// 2018-06-26 : Davide Fazzini
//
//
// N.B: The genCounters are identified by means of a key number. 
//      The complete list of all the counters is here: /Event/GenEvent/xml/GenCountersFSR.xml
//      Since counters belonging to the same class have consecutive keys, the methods used by 
//      this service cut on the key variable in order to select the correct counter class.
//-----------------------------------------------------------------------------

namespace 
{
  std::string getCurrentTime() { 
    return Gaudi::Time::current().format(true);
  }  
  
  const std::string getEvtTypeDesc(int evtType) {
    std::string description;
    std::string path_decFile = System::getEnv("DECFILESROOT");

    if(path_decFile != "")
    {
      std::string evtType_str = std::to_string(evtType);
      std::string path_desc = path_decFile + "/options/" + evtType_str + ".py";
      std::fstream file_desc(path_desc, std::fstream::in);
      std::string line_ref = "$DECFILESROOT/dkfiles/";      
      std::string line_stop = ".dec\"";
      std::string line;

      while(getline(file_desc,line))
      {
        std::size_t pos_ref = line.find(line_ref);

        if(pos_ref != std::string::npos)
        {
          std::size_t pos_end = line.find(line_stop);
          pos_ref = pos_ref + line_ref.length();
          int count = pos_end - pos_ref;
          description = line.substr(pos_ref, count);
          break; 
        } 
      }
      file_desc.close(); 
    }
    return description; 
  }
  
  
  //=============================================================================   
  //  Add generator level counters in the json file                                 
  //=============================================================================

  ptree writeGeneratorCounters(const LHCb::GenFSR& genFSR)
  { 
    LHCb::CrossSectionsFSR crossFSR;
    std::map<std::string,std::string> mapCross = crossFSR.getFullNames();
    
    ptree array_tree;
    
    for(auto const& counter : genFSR.genCounters())
    {
      auto key = counter.first;
      
      if(key > LHCb::GenCountersFSR::CounterKey::bAndcAcc) continue;
      
      if(key == LHCb::GenCountersFSR::CounterKey::BeforeFullEvt || 
         key == LHCb::GenCountersFSR::CounterKey::AfterFullEvt) continue;
      
      longlong before = genFSR.getDenominator(key);
      if(before == 0) continue;
      
      double C = (key >= LHCb::GenCountersFSR::CounterKey::OnebGen ? 
                  genFSR.getCrossSectionInfo(LHCb::CrossSectionsFSR::CrossSectionKey::MBCrossSection).second 
                  : 1);
      
      longlong after = (key == LHCb::GenCountersFSR::CounterKey::EvtGenerated ? 
                        genFSR.getGenCounterInfo(key+1).second : counter.second.second);
      if(after == 0) continue;
      
      bool flag = (key != LHCb::CrossSectionsFSR::CrossSectionKey::MeanNoZeroPUInt &&
                   key != LHCb::CrossSectionsFSR::CrossSectionKey::MeanPUInt &&
                   key != LHCb::CrossSectionsFSR::CrossSectionKey::MeanPUIntAcc);
      
      double fraction = genFSR.getEfficiency(after, before, C);
      double error = genFSR.getEfficiencyError(after, before, C, flag);
      std::string fullName = mapCross[LHCb::CrossSectionsFSR::CrossSectionKeyToString(key)];    
    
      ptree counter_tree;
      counter_tree.put("descr", fullName);
      counter_tree.put("type", "counter");
      counter_tree.put("value", fraction);      
      counter_tree.put("error", error);

      array_tree.push_back(std::make_pair("", counter_tree));
    }
    
    return array_tree;
  }
  
  //=============================================================================    
  //  Add generator level cut efficiencies in the xml file                           
  //=============================================================================    

  ptree writeCutEfficiencies(const LHCb::GenFSR& genFSR)
  {
    LHCb::CrossSectionsFSR crossFSR;
    std::map<std::string,std::string> mapCross = crossFSR.getFullNames();
    
    ptree array_tree;
    
    for(const auto& counter : genFSR.genCounters())
    {
      auto key = counter.first;
      
      if(key != LHCb::GenCountersFSR::CounterKey::AfterFullEvt &&
         key != LHCb::GenCountersFSR::CounterKey::AfterLevelCut &&
         key != LHCb::GenCountersFSR::CounterKey::EvtSignal &&
         key != LHCb::GenCountersFSR::CounterKey::EvtantiSignal &&
         key != LHCb::GenCountersFSR::CounterKey::AfterPCut &&
         key != LHCb::GenCountersFSR::CounterKey::AfterantiPCut) continue;
      
      
      longlong before = genFSR.getDenominator(key);
      if(before == 0) continue;
      longlong after    = counter.second.second;    
      if(after == 0) continue;
      
      double fraction = genFSR.getEfficiency(after, before);
      double error = genFSR.getEfficiencyError(after, before);
      
      std::string fullName = mapCross[LHCb::CrossSectionsFSR::CrossSectionKeyToString(key)];
     
      ptree efficiency_tree;
      
      efficiency_tree.put("descr", fullName);
      efficiency_tree.put("type", "counter");
      efficiency_tree.put("value", fraction);    
      efficiency_tree.put("error", error);
      
      array_tree.push_back(std::make_pair("", efficiency_tree));
    }
    
    return array_tree;
  }
  
  //=============================================================================    
  //  Add generated and accepted hadron counters in the json file                                  
  //=============================================================================
  
  ptree writeHadronCounters(const LHCb::GenFSR& genFSR)
  {
    ptree array_tree;

    LHCb::CrossSectionsFSR crossFSR;  
    std::map<std::string,std::string> mapCross = crossFSR.getFullNames();
    
    for(const auto& counter : genFSR.genCounters())
    {
      auto key = counter.first;

      if((key >= LHCb::GenCountersFSR::CounterKey::B0Gen &&
          key <= LHCb::GenCountersFSR::CounterKey::D2starAcc))
      {
        longlong before = genFSR.getDenominator(key);
        if(before == 0) continue;
        longlong after = counter.second.second;
        if(after == 0) continue;
        
        double fraction = genFSR.getEfficiency(after, before);
        double error    = genFSR.getEfficiencyError(after, before);
        
        std::string fullName = mapCross[LHCb::CrossSectionsFSR::CrossSectionKeyToString(key)];

        ptree counter_tree;        
        counter_tree.put("descr", fullName);
        counter_tree.put("type", "counter");
        counter_tree.put("value", fraction);      
        counter_tree.put("error", error);
        
        array_tree.push_back(std::make_pair("", counter_tree));
      }
    }

    return array_tree;
  }
  
  //=============================================================================    
  //  Add generator cross-sections in the json file                                  
  //=============================================================================
  
  ptree writeGeneratorCrossSections(const LHCb::GenFSR& genFSR)
  {
    ptree array_tree;
    
    for(const auto& cross : genFSR.crossSections())
    {
      std::string name = cross.second.first;
      double value = cross.second.second;
      
      if(value == 0 || std::isnan(value) != 0) continue;
      
      ptree cross_tree;
      cross_tree.put("descr", name);
      cross_tree.put("type", "cross-section");
      cross_tree.put("value", value);
      
      array_tree.push_back(std::make_pair("", cross_tree));
    }

    return array_tree;
  }

  //=============================================================================  
  //  Add global statistics in the json file
  //=============================================================================
  
  ptree writeGlobalStat(const LHCb::GenFSR& genFSR)
  {
    longlong countEvtGen = 0, countEvtAcc = 0, countIntGen = 0, countIntAcc = 0;
         
    ptree array_tree;
    
    for(const auto& counter : genFSR.genCounters())
    {
      auto key = counter.first;    
      if(key >  LHCb::GenCountersFSR::CounterKey::IntAccepted) continue;
      
      switch(key) 
      {
      case LHCb::GenCountersFSR::CounterKey::EvtGenerated: countEvtGen = counter.second.second; break;
      case LHCb::GenCountersFSR::CounterKey::IntGenerated: countIntGen = counter.second.second; break;
      case LHCb::GenCountersFSR::CounterKey::EvtAccepted:  countEvtAcc = counter.second.second; break;
      case LHCb::GenCountersFSR::CounterKey::IntAccepted:  countIntAcc = counter.second.second; break;
      }

    }

    ptree event_tree, interaction_tree;
    
    event_tree.put("descr", "Number of accepted events/generated events");
    event_tree.put("type", "fraction");    
    event_tree.put("numer", countEvtAcc);
    event_tree.put("denom", countEvtGen);

    interaction_tree.put("descr", "Number of interactions in accepted events/generated interactions");
    interaction_tree.put("type", "fraction");
    interaction_tree.put("numer", countIntAcc);
    interaction_tree.put("denom", countIntGen);

    array_tree.push_back(std::make_pair("", event_tree));
    array_tree.push_back(std::make_pair("", interaction_tree));
  
    return array_tree;
  }
}

// Instantiation of a static factory class used by clients to create                    
// instances of this service                                                            
DECLARE_COMPONENT(GenFSRJson)

//=============================================================================
//  Initialization
//=============================================================================  

StatusCode GenFSRJson::initialize() 
{
  StatusCode sc = Service::initialize(); // must be executed first
  
  if ( sc.isFailure() ) return sc;   // error prinsted already by Service
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;
  
  // get the File Records service    
  m_fileRecordSvc = Gaudi::svcLocator()->service("FileRecordDataSvc");

  return sc;
}

//=============================================================================  
//  Finalize 
//=============================================================================     
StatusCode GenFSRJson::finalize() 
{
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Finalize" << endmsg;
  
  GenFSRJson::printFSR();
  
  return Service::finalize();
}

//=============================================================================
//  Print the GenFSR in a file json
//=============================================================================

void GenFSRJson::printFSR()
{
  if (msgLevel(MSG::DEBUG)) debug() << "write to file: " << m_jsonOutputName << endmsg;
  
  // make an inventory of the FileRecord store
  std::vector< std::string > addresses = m_navigatorTool->navigate(m_fileRecordName, m_FSRName);
  
  for(const auto& genRecordAddress: addresses)
  {    
    std::string line;
    DataObject *obj = nullptr;
    StatusCode sc = m_fileRecordSvc->retrieveObject(genRecordAddress, obj);
      
    // If so, retrieve it
    if(!sc.isSuccess())
    {
      error() << "Unable to retrieve object '" << genRecordAddress << "'" << endmsg;
      continue;
    }
    
    LHCb::GenFSR* genFSR = dynamic_cast<LHCb::GenFSR*>(obj);
    
    if (genFSR == nullptr)
    {
      warning() << "genFSR record not found!" << endmsg;
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

      // open the new file                       
      std::ofstream jsonOutput(m_jsonOutputLocation + m_jsonOutputName, 
                               std::fstream::out);
      
      if(jsonOutput.is_open())
      {          
        if (msgLevel(MSG::DEBUG))
          debug() << "Json output: " << m_jsonOutputLocation << m_jsonOutputName << " created." << endmsg;
        
        if (msgLevel(MSG::DEBUG))
          debug() << m_jsonOutputLocation << "GenerationFSR_" << m_appConfigFile << ".json" <<
            " does not exist, creating it now." << endmsg;
        
        ptree main_tree;
        
        std::string evtDesc = ::getEvtTypeDesc(evtType);       

        main_tree.put("\"APPCONFIG_file\"", m_appConfigFile);
        main_tree.put("\"APPCONFIG_version\"", m_appConfigVersion);
        main_tree.put("\"evtType\"", evtType_str);
        main_tree.put("\"evtTypeDesc\"", evtDesc);
        main_tree.put("\"DecFiles_version\"", decFiles);
        main_tree.put("\"Hard Generator\"", genName);
        main_tree.put(" \"Gauss_version\"", m_gaussVersion);
        main_tree.put("\"DDDB\"", m_dddb);
        main_tree.put("\"SIMCOND\"", m_simCond);
        main_tree.put("\"genTimeStamp\"", time);
        
        
        ptree gencounters_array = ::writeGeneratorCounters(*genFSR);
        ptree hadroncounters_array = ::writeHadronCounters(*genFSR);
        ptree cross_array = ::writeGeneratorCrossSections(*genFSR);
        ptree efficiencies_array = ::writeCutEfficiencies(*genFSR);
        ptree stats_array = ::writeGlobalStat(*genFSR);
       
        main_tree.add_child("InteractionCounters", gencounters_array);
        main_tree.add_child("HadronCounters", hadroncounters_array);
        main_tree.add_child("HardGeneratorCrossSection", cross_array);
        main_tree.add_child("SignalCounters", efficiencies_array);
        main_tree.add_child("globStat", stats_array);
  
  
        main_tree.put("prodID", m_prodID);
        main_tree.put("nb_jobs", njobs_str);
        main_tree.put("script_version", script_version);
        
        write_json(jsonOutput, main_tree, false);
        jsonOutput.close(); 
      }
    }
  }
}

