/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// Include files
// local
#include "GenFSRLog.h"

// from Event
#include "Event/GenFSR.h"
#include "Event/GenCountersFSR.h"
#include "Event/CrossSectionsFSR.h"

// to write a file
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

//-----------------------------------------------------------------------------
// Implementation file for class : GenFSRLog
//
// 2015-06-23 : Davide Fazzini
//-----------------------------------------------------------------------------

namespace
{
  //=============================================================================
  //  Add generator level counters in the xml file
  //=============================================================================

  void addGenCounters(LHCb::GenFSR& genFSR, std::ostream& xmlOutput)
  {
    LHCb::GenCountersFSR counterFSR;    
    std::map<std::string, std::string> mapCounter = counterFSR.getFullNames();
    
    for(auto const& counter : genFSR.genCounters())
    {
      auto key = counter.first;
      
      if(key > LHCb::GenCountersFSR::CounterKey::bAndcAcc) continue;
      if(key == LHCb::GenCountersFSR::CounterKey::BeforeFullEvt ||
         key == LHCb::GenCountersFSR::CounterKey::AfterFullEvt) continue;
      
      std::string name = LHCb::GenCountersFSR::CounterKeyToString(key);
      std::string fullName = mapCounter[name];      
      
      longlong value = counter.second.second;

      xmlOutput << "  <counter name = \"" << fullName << "\">" << std::endl
                << "    <value> " << value << " </value>" << std::endl
                << "  </counter>" << std::endl; 
    }    
  }

  //=============================================================================
  //  Add generator level cut efficiencies in the xml file
  //=============================================================================

  void addCutEfficiencies(LHCb::GenFSR& genFSR, std::ofstream& xmlOutput)
  {
    LHCb::GenCountersFSR crossFSR;    
    std::map<std::string,std::string> mapCross = crossFSR.getFullNames();

    for(auto const& counter : genFSR.genCounters())
    {
      auto key = counter.first;

      if(key == LHCb::GenCountersFSR::CounterKey::EvtInverted)
      {
        std::string name = LHCb::GenCountersFSR::CounterKeyToString(key);
        std::string fullName = mapCross[name];
        longlong value = counter.second.second;

        xmlOutput << "  <counter name = \"" << fullName << "\">" << std::endl
                  << "    <value> " << value << " </value>" << std::endl
                  << "  </counter>" << std::endl;        
      }

      if(key != LHCb::GenCountersFSR::CounterKey::AfterFullEvt &&
         key != LHCb::GenCountersFSR::CounterKey::AfterLevelCut &&
         key != LHCb::GenCountersFSR::CounterKey::AfterPCut &&
         key != LHCb::GenCountersFSR::CounterKey::AfterantiPCut) continue;

      std::string name = LHCb::GenCountersFSR::CounterKeyToString(key);      
      std::string fullName = mapCross[name];

      longlong before = genFSR.getDenominator(key);
      if(before == 0) continue;

      longlong after = counter.second.second;
      double fraction = genFSR.getEfficiency(after, before);
      double error = genFSR.getEfficiencyError(after, before);

      xmlOutput << "  <efficiency name = \"" << fullName << "\">" << std::endl
                << "    <after> " << after << " </after>" << std::endl
                << "    <before> " << before << " </before>" << std::endl
                << "    <value> " << fraction << " </value>" << std::endl
                << "    <error> " << error << " </error>" << std::endl
                << "  </efficiency>" << std::endl; 
    }    
  }

  //=============================================================================
  //  Add generator level fractions in the xml file
  //=============================================================================

  void addGenFractions(LHCb::GenFSR& genFSR, std::ostream& xmlOutput)
  {
    LHCb::GenCountersFSR crossFSR;
    std::map<std::string,std::string> mapCross = crossFSR.getFullNames();

    for(auto const& counter : genFSR.genCounters())
    {
      auto key = counter.first;
      if(key < LHCb::GenCountersFSR::CounterKey::B0Gen || 
         key >= LHCb::GenCountersFSR::CounterKey::BeforePCut) continue;

      std::string name = LHCb::GenCountersFSR::CounterKeyToString(key);      
      std::string fullName = mapCross[name];

      longlong before = genFSR.getDenominator(key);
      if(before == 0) continue;

      longlong after = counter.second.second;
      double fraction = genFSR.getEfficiency(after, before); 
      double error = genFSR.getEfficiencyError(after, before);

      xmlOutput << "  <fraction name = \"" << fullName << "\">" << std::endl
                << "    <number> " << after << " </number>" << std::endl
                << "    <value> " << fraction << " </value>" << std::endl
                << "    <error> " << error << " </error>" << std::endl
                << "  </fraction>" << std::endl;      
    }
  }
  
  //=============================================================================
  //  Add generator level cross-sections in the xml file
  //=============================================================================

  void addGenCrossSections(LHCb::GenFSR& genFSR, std::ostream& xmlOutput)
  {
    LHCb::GenFSR::GeneratorCounter genCounters = genFSR.genCounters();
    
    LHCb::GenCountersFSR counterFSR;    
    std::map<std::string,std::string> mapCounter = counterFSR.getFullNames();

    for(auto const& counter : genFSR.genCounters())
    {
      auto key = counter.first;

      if(key > LHCb::GenCountersFSR::CounterKey::bAndcAcc) continue;
      if(key ==  LHCb::GenCountersFSR::CounterKey::BeforeFullEvt || 
         key == LHCb::GenCountersFSR::CounterKey::AfterFullEvt) continue;

      std::string name = LHCb::GenCountersFSR::CounterKeyToString(key);      
      std::string fullName = mapCounter[name];

      longlong before = genFSR.getDenominator(key);
      if(before == 0) continue;
      
      double C = key >=  LHCb::GenCountersFSR::CounterKey::OnebGen ? 
        genFSR.getCrossSectionInfo(LHCb::CrossSectionsFSR::CrossSectionKeyToType("MBCrossSection")).second : 1;
      
      longlong after = key == 2 ? genFSR.getGenCounterInfo(key+1).second :  counter.second.second;
      if(after == 0) continue;      
      
      double fraction = genFSR.getEfficiency(after, before, C);

      bool flag = (key != LHCb::CrossSectionsFSR::CrossSectionKey::MeanNoZeroPUInt &&
                   key != LHCb::CrossSectionsFSR::CrossSectionKey::MeanPUInt &&
                   key != LHCb::CrossSectionsFSR::CrossSectionKey::MeanPUIntAcc);
      
      double error = genFSR.getEfficiencyError(after, before, C, flag);

      xmlOutput << "  <crosssection name = \"" << fullName << "\">" << std::endl
                << "    <generated> " <<  after  << " </generated>" << std::endl
                << "    <value> " << fraction << " </value>" << std::endl
                << "    <error> " << error << " </error>" << std::endl
                << "  </crosssection>" << std::endl; 
    }    
  }

  //=============================================================================
  //  Add pythia cross-sections in the xml file
  //=============================================================================

  void addGeneratorCrossSections(LHCb::GenFSR& genFSR, std::ostream& xmlOutput)
  { 
    for(auto const& cross : genFSR.crossSections())
    {
      auto key = cross.first;
      std::string name = cross.second.first;
      name.erase(name.size() - 2);
      
      longlong number = genFSR.getGenCounterInfo(key+100).second;      
      double value = cross.second.second;
      
      xmlOutput << "  <crosssection id = \"" << key << "\">" << std::endl
                << "    <description> \"" << name << "\" </description>" << std::endl
                << "    <generated> " <<  number  << " </generated>" << std::endl
                << "    <value> " <<  value << " </value>" << std::endl
                << "  </crosssection>" << std::endl;
    }
  }
}
      
// Declaration of the Algorithm Factory
DECLARE_COMPONENT( GenFSRLog )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
GenFSRLog::GenFSRLog( const std::string& name,
                      ISvcLocator* pSvcLocator)
: GaudiAlgorithm ( name , pSvcLocator )
{  
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode GenFSRLog::initialize() 
{
  StatusCode sc = GaudiAlgorithm::initialize();  // must be executed first

  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;

  // get the File Records service
  m_fileRecordSvc = Gaudi::svcLocator()->service("FileRecordDataSvc");

  m_navigatorTool = tool<IFSRNavigator>("FSRNavigator", "FSRNavigator");

  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode GenFSRLog::execute()
{  
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  return StatusCode::SUCCESS;  
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode GenFSRLog::finalize() {

  if (msgLevel(MSG::DEBUG)) debug() << "==> Finalize" << endmsg;
  
  GenFSRLog::printFSR();

  return GaudiAlgorithm::finalize();  // must be called after all other actions
}

//=============================================================================

//=============================================================================
//  Print the GenFSR in a file xml
//=============================================================================

void GenFSRLog::printFSR()
{
  if (msgLevel(MSG::DEBUG)) debug() << "write to file: " + m_xmlOutputName << endmsg;

  // make an inventory of the FileRecord store
  auto addresses = m_navigatorTool->navigate(m_fileRecordName, m_FSRName);

  if( !addresses.empty() ) 
  {
    std::ofstream xmlOutput(m_xmlOutputName, std::fstream::out);

    if (xmlOutput.is_open())
    {
      if (msgLevel(MSG::DEBUG)) debug() << " Xml Output: " + m_xmlOutputName + " - opened" << endmsg;

      xmlOutput << "<?xml version=\"1.0\"?>" << std::endl
                << "<generatorCounters>" << std::endl
                << "  <version>" << "1.1" << "</version>" << std::endl;

      for(const auto& genRecordAddress: addresses)
      {
        DataObject *obj = nullptr;        
        StatusCode sc = m_fileRecordSvc->retrieveObject(genRecordAddress, obj);

        if(!sc.isSuccess())
        {
          error() << "Unable to retrieve object '" << genRecordAddress << "'" << endmsg;  
          continue;
        }

        LHCb::GenFSR* genFSR = dynamic_cast<LHCb::GenFSR*>(obj);

        if (genFSR == nullptr)
        {
          warning() << "genFSR record was not found" << endmsg;
          if (msgLevel(MSG::DEBUG)) debug() << genRecordAddress << " not found" << endmsg;
        }  
        else 
        {
          int evtType = genFSR->getSimulationInfo("evtType", 0);
          std::string genName = genFSR->getSimulationInfo("hardGenerator", "");
          std::string method = genFSR->getSimulationInfo("generationMethod", "");

          xmlOutput << "  <eventType> " << evtType << " </eventType>" << std::endl;

          // Add generator level counters
          ::addGenCounters(*genFSR, xmlOutput);
          // Add generator level cut efficiencies
          ::addCutEfficiencies(*genFSR, xmlOutput);
          // Add generator level fractions
          ::addGenFractions(*genFSR, xmlOutput);
          // Add generator level cross-sections
          ::addGenCrossSections(*genFSR, xmlOutput);

          xmlOutput << "  <method> " << method << " </method>" << std::endl;
          xmlOutput << "  <generator> " << genName << " </generator>" << std::endl;

          // Add pythia cross-sections
          ::addGeneratorCrossSections(*genFSR, xmlOutput);
        }
      }

      xmlOutput << "</generatorCounters>" << std::endl;
      xmlOutput.close();
    } 
    else if (msgLevel(MSG::DEBUG)) 
      debug() << "The file was not opened correctly" << endmsg;
  } 
  else if (msgLevel(MSG::DEBUG)) 
    debug() << "There are not FSRs to write" << endmsg;
}

