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
#include "Event/CrossSectionsFSR.h"
#include "Event/GenCountersFSR.h"
#include "Event/GenFSR.h"
#include "FSRAlgs/IFSRNavigator.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/Time.h"
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
namespace fs = std::filesystem;

#define script_version "20181113"

//-----------------------------------------------------------------------------
// Implementation file for class : GenFSRStat
//
// 2015-08-03 : Davide Fazzini
//-----------------------------------------------------------------------------

/** @class GenFSRStat GenFSRStat.h
 *
 *
 *  @author Davide Fazzini
 *  @date   2015-07-29
 */

class GenFSRStat : public GaudiAlgorithm {
public:
  /// Standard constructor
  GenFSRStat( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

  void printFSR(); // Print the GenFSR in a file .html

private:
  Gaudi::Property<std::string> m_fileRecordName{this, "FileRecordLocation", "/FileRecords",
                                                "TES location where FSRs are persisted"};
  Gaudi::Property<std::string> m_FSRName{this, "FSRName", "/GenFSR", "Name of the genFSR tree"};
  Gaudi::Property<std::string> m_prodID{this, "prodID", "", "Production ID used in the generation"};
  Gaudi::Property<std::string> m_appConfigVersion{this, "appConfigVersion", "",
                                                  "Version of AppConfig used in the simulation"};
  Gaudi::Property<std::string> m_appConfigFile{this, "appConfigFile", "", "Name of the AppConfig file"};
  Gaudi::Property<std::string> m_gaussVersion{this, "gaussVersion", "", "Gauss version used in the simulation"};
  Gaudi::Property<std::string> m_simCond{this, "simCond", "", "Tag for the SimCond database"};
  Gaudi::Property<std::string> m_dddb{this, "dddb", "", "Tag for the DDDB database"};
  Gaudi::Property<std::string> m_htmlOutputLocation{this, "htmlOutputLocation", "",
                                                    "Path where to save the .html output"};
  Gaudi::Property<std::string> m_htmlOutputName{this, "htmlOutputName", "GenerationFSR_" + m_appConfigFile + ".html",
                                                "Name of the .html output"};

  SmartIF<IDataProviderSvc> m_fileRecordSvc;
  IFSRNavigator*            m_navigatorTool = nullptr; // tool to navigate FSR
};

namespace {
  //=============================================================================
  //  Get current date/time, format is YYYY-MM-DD.HH:mm:ss
  //=============================================================================

  // naive function to get the current time
  std::string getCurrentTime() { return Gaudi::Time::current().format( true ); }

  //=============================================================================
  //  Get correct event type description
  //=============================================================================

  const std::string getEvtTypeDesc( int evtType ) {
    std::string path_decFile = System::getEnv( "DECFILESROOT" );
    std::string description;

    if ( !path_decFile.empty() ) {
      std::string  evtType_str = std::to_string( evtType );
      std::string  path_desc   = path_decFile + "/options/" + evtType_str + ".py";
      std::fstream file_desc( path_desc, std::fstream::in );
      std::string  line_ref  = "$DECFILESROOT/dkfiles/";
      std::string  line_stop = ".dec\"";
      std::string  line;

      while ( getline( file_desc, line ) ) {
        std::size_t pos_ref = line.find( line_ref );

        if ( pos_ref != std::string::npos ) {
          std::size_t pos_end = line.find( line_stop );
          pos_ref             = pos_ref + line_ref.length();
          int count           = pos_end - pos_ref;
          description         = line.substr( pos_ref, count );
          break;
        }
      }

      file_desc.close();
    }

    return description;
  }

  //=============================================================================
  //  Add global statistics in the html file
  //=============================================================================

  void writeGlobalStat( LHCb::GenFSR& genFSR, std::ostream& htmlOutput ) {
    longlong countEvtGen = 0, countEvtAcc = 0, countIntGen = 0, countIntAcc = 0;

    for ( const auto& counter : genFSR.genCounters() ) {
      auto key = counter.first;

      if ( key > LHCb::GenCountersFSR::CounterKey::IntAccepted ) continue;

      switch ( key ) {
      case LHCb::GenCountersFSR::CounterKey::EvtGenerated:
        countEvtGen = counter.second.second;
        break;
      case LHCb::GenCountersFSR::CounterKey::IntGenerated:
        countIntGen = counter.second.second;
        break;
      case LHCb::GenCountersFSR::CounterKey::EvtAccepted:
        countEvtAcc = counter.second.second;
        break;
      case LHCb::GenCountersFSR::CounterKey::IntAccepted:
        countIntAcc = counter.second.second;
        break;
      }
    }

    double fraction_events       = genFSR.getEfficiency( countEvtAcc, countEvtGen );
    double fraction_interactions = genFSR.getEfficiency( countIntAcc, countIntGen );

    htmlOutput << "<p> Number of accepted events/generated events: " << countEvtAcc << "/" << countEvtGen;
    htmlOutput << " = " << fraction_events << "</p>\n";
    htmlOutput << "<p> Number of interactions in accepted events/generated interactions: " << countIntAcc;
    htmlOutput << "/" << countIntGen << " = " << fraction_interactions << "</p>\n";
  }

  //=============================================================================
  //  Add generator level counters in the html file
  //=============================================================================

  void writeGeneratorCounters( LHCb::GenFSR& genFSR, int count, std::ostream& htmlOutput ) {
    const auto& mapCross = LHCb::CrossSectionsFSR::getFullNames();

    // write the table options
    htmlOutput << "<tr>\n<td align=center>\n<table border=2>\n<th colspan=" << count;
    htmlOutput << "><div class=firstcell>Counters Interaction</div></th>\n" << std::endl;
    htmlOutput << "<tr>\n";

    // write the list of cross-section names
    for ( auto const& counter : genFSR.genCounters() ) {
      auto key = counter.first;

      if ( key > LHCb::GenCountersFSR::CounterKey::bAndcAcc ) continue;
      if ( key == LHCb::GenCountersFSR::CounterKey::BeforeFullEvt ||
           key == LHCb::GenCountersFSR::CounterKey::AfterFullEvt )
        continue;

      std::string name     = LHCb::CrossSectionsFSR::CrossSectionKeyToString( key );
      const auto& fullName = mapCross.at( name );

      longlong before = genFSR.getDenominator( key );
      longlong after  = genFSR.getGenCounterInfo( key ).second;

      if ( before == 0 || after == 0 ) continue;

      if ( key >= LHCb::GenCountersFSR::CounterKey::OnebGen )
        htmlOutput << "<td><font size=2>" << fullName << " (mb)</font></td>\n";
      else
        htmlOutput << "<td><font size=2>" << fullName << "</font></td>\n";
    }

    htmlOutput << "</tr>\n\n<tr>\n";

    // write the value of each fraction
    for ( auto const& counter : genFSR.genCounters() ) {
      auto key = counter.first;

      if ( key > LHCb::GenCountersFSR::CounterKey::bAndcAcc ) continue;
      if ( key == LHCb::GenCountersFSR::CounterKey::BeforeFullEvt ||
           key == LHCb::GenCountersFSR::CounterKey::AfterFullEvt )
        continue;

      longlong before = genFSR.getDenominator( key );
      if ( before == 0 ) continue;

      double C = ( key >= LHCb::GenCountersFSR::CounterKey::OnebGen
                       ? genFSR.getCrossSectionInfo( LHCb::CrossSectionsFSR::CrossSectionKey::MBCrossSection ).second
                       : 1 );

      longlong after =
          ( key == LHCb::GenCountersFSR::CounterKey::EvtGenerated ? genFSR.getGenCounterInfo( key + 1 ).second
                                                                  : counter.second.second );

      if ( after == 0 ) continue;

      double fraction = genFSR.getEfficiency( after, before, C );
      htmlOutput << "<td>" << fraction << "</td>\n";
    }
    htmlOutput << "</tr>\n\n<tr>\n";

    // write the related error of each fraction
    for ( auto const& counter : genFSR.genCounters() ) {
      auto key = counter.first;

      if ( key > LHCb::GenCountersFSR::CounterKey::bAndcAcc ) continue;
      if ( key == LHCb::GenCountersFSR::CounterKey::BeforeFullEvt ||
           key == LHCb::GenCountersFSR::CounterKey::AfterFullEvt )
        continue;

      longlong before = genFSR.getDenominator( key );
      if ( before == 0 ) continue;

      double C = ( key >= LHCb::GenCountersFSR::CounterKey::OnebGen
                       ? genFSR.getCrossSectionInfo( LHCb::CrossSectionsFSR::CrossSectionKey::MBCrossSection ).second
                       : 1 );

      longlong after =
          ( key == LHCb::GenCountersFSR::CounterKey::EvtGenerated ? genFSR.getGenCounterInfo( key + 1 ).second
                                                                  : counter.second.second );

      if ( after == 0 ) continue;

      bool flag = ( key != LHCb::CrossSectionsFSR::CrossSectionKey::MeanNoZeroPUInt &&
                    key != LHCb::CrossSectionsFSR::CrossSectionKey::MeanPUInt &&
                    key != LHCb::CrossSectionsFSR::CrossSectionKey::MeanPUIntAcc );

      double error = genFSR.getEfficiencyError( after, before, C, flag );
      htmlOutput << "<td>&plusmn; " << error << "</td>\n";
    }

    htmlOutput << "</tr>\n</table>\n</td>\n</tr>" << std::endl;
  }

  //=============================================================================
  //  Add generator level cut efficiencies in the xml file
  //=============================================================================
  void writeCutEfficiencies( LHCb::GenFSR& genFSR, int count, std::ostream& htmlOutput ) {

    const auto& mapCross = LHCb::CrossSectionsFSR::getFullNames();

    htmlOutput << "<tr>\n<td align=center>\n<table border=2>\n<th colspan=" << count;
    htmlOutput << "><div class=firstcell>Generator Efficiencies</div></th>\n";
    htmlOutput << "<tr>\n";

    for ( auto const& counter : genFSR.genCounters() ) {
      auto key = counter.first;

      if ( key != LHCb::GenCountersFSR::CounterKey::AfterFullEvt &&
           key != LHCb::GenCountersFSR::CounterKey::AfterLevelCut &&
           key != LHCb::GenCountersFSR::CounterKey::EvtSignal &&
           key != LHCb::GenCountersFSR::CounterKey::EvtantiSignal &&
           key != LHCb::GenCountersFSR::CounterKey::AfterPCut &&
           key != LHCb::GenCountersFSR::CounterKey::AfterantiPCut )
        continue;

      std::string name     = LHCb::CrossSectionsFSR::CrossSectionKeyToString( key );
      const auto& fullName = mapCross.at( name );

      longlong before = genFSR.getDenominator( key );
      if ( before == 0 ) continue;

      longlong after = counter.second.second;
      if ( after == 0 ) continue;

      htmlOutput << "<td><font size=2>" << fullName << "</font></td>\n";
    }
    htmlOutput << "</tr>\n\n<tr>\n";

    for ( auto const& counter : genFSR.genCounters() ) {
      auto key = counter.first;

      if ( key != LHCb::GenCountersFSR::CounterKey::AfterFullEvt &&
           key != LHCb::GenCountersFSR::CounterKey::AfterLevelCut &&
           key != LHCb::GenCountersFSR::CounterKey::EvtSignal &&
           key != LHCb::GenCountersFSR::CounterKey::EvtantiSignal &&
           key != LHCb::GenCountersFSR::CounterKey::AfterPCut &&
           key != LHCb::GenCountersFSR::CounterKey::AfterantiPCut )
        continue;

      longlong before = genFSR.getDenominator( key );
      if ( before == 0 ) continue;

      longlong after = counter.second.second;
      if ( after == 0 ) continue;

      double fraction = genFSR.getEfficiency( after, before );
      htmlOutput << "<td>" << fraction << "</td>\n";
    }

    htmlOutput << "</tr>\n\n<tr>\n";

    for ( auto const& counter : genFSR.genCounters() ) {
      auto key = counter.first;

      if ( key != LHCb::GenCountersFSR::CounterKey::AfterFullEvt &&
           key != LHCb::GenCountersFSR::CounterKey::AfterLevelCut &&
           key != LHCb::GenCountersFSR::CounterKey::EvtSignal &&
           key != LHCb::GenCountersFSR::CounterKey::EvtantiSignal &&
           key != LHCb::GenCountersFSR::CounterKey::AfterPCut &&
           key != LHCb::GenCountersFSR::CounterKey::AfterantiPCut )
        continue;

      longlong before = genFSR.getDenominator( key );
      if ( before == 0 ) continue;

      longlong after = counter.second.second;
      if ( after == 0 ) continue;

      double error = genFSR.getEfficiencyError( after, before );
      htmlOutput << "<td>&plusmn; " << error << "</td>\n";
    }
    htmlOutput << "</tr>\n</table>\n</td>\n</tr>" << std::endl;
  }

  //=============================================================================
  //  Add generated hadron counters in the html file
  //=============================================================================

  void writeGenHadronCounters( LHCb::GenFSR& genFSR, int countGen, std::ostream& htmlOutput ) {

    const auto& mapCross = LHCb::CrossSectionsFSR ::getFullNames();

    htmlOutput << "<tr>\n<td align=center>\n<table border=2>\n<th colspan=" << countGen;
    htmlOutput << "><div class=firstcell> Generated Hadron Counters</div></th>\n" << std::endl;
    htmlOutput << "<tr>\n";

    for ( auto const& counter : genFSR.genCounters() ) {
      auto key = counter.first;

      // Generated Hadron Fractions
      if ( ( key >= LHCb::GenCountersFSR::CounterKey::B0Gen && key <= LHCb::GenCountersFSR::CounterKey::bbGen ) ||
           ( key >= LHCb::GenCountersFSR::CounterKey::D0Gen && key <= LHCb::GenCountersFSR::CounterKey::ccGen ) ||
           ( key >= LHCb::GenCountersFSR::CounterKey::BGen && key <= LHCb::GenCountersFSR::CounterKey::B2starGen ) ||
           ( key >= LHCb::GenCountersFSR::CounterKey::DGen && key <= LHCb::GenCountersFSR::CounterKey::D2starGen ) ) {
        std::string name     = LHCb::CrossSectionsFSR::CrossSectionKeyToString( key );
        const auto& fullName = mapCross.at( name );

        longlong before = genFSR.getDenominator( key );
        if ( before == 0 ) continue;

        longlong after = genFSR.getGenCounterInfo( key ).second;
        if ( after == 0 ) continue;

        htmlOutput << "<td><font size=2>" << fullName << "</font></td>\n";
      }
    }

    htmlOutput << "</tr>\n\n<tr>\n";

    for ( auto const& counter : genFSR.genCounters() ) {
      auto key = counter.first;

      // Generated Hadron Fractions
      if ( ( key >= LHCb::GenCountersFSR::CounterKey::B0Gen && key <= LHCb::GenCountersFSR::CounterKey::bbGen ) ||
           ( key >= LHCb::GenCountersFSR::CounterKey::D0Gen && key <= LHCb::GenCountersFSR::CounterKey::ccGen ) ||
           ( key >= LHCb::GenCountersFSR::CounterKey::BGen && key <= LHCb::GenCountersFSR::CounterKey::B2starGen ) ||
           ( key >= LHCb::GenCountersFSR::CounterKey::DGen && key <= LHCb::GenCountersFSR::CounterKey::D2starGen ) ) {
        longlong before = genFSR.getDenominator( key );
        if ( before == 0 ) continue;

        longlong after = counter.second.second;
        if ( after == 0 ) continue;

        double fraction = genFSR.getEfficiency( after, before );
        htmlOutput << "<td>" << fraction << "</td>\n";
      }
    }
    htmlOutput << "</tr>\n\n<tr>\n";

    for ( auto const& counter : genFSR.genCounters() ) {
      auto key = counter.first;

      // Generated Hadron Fractions
      if ( ( key >= LHCb::GenCountersFSR::CounterKey::B0Gen && key <= LHCb::GenCountersFSR::CounterKey::bbGen ) ||
           ( key >= LHCb::GenCountersFSR::CounterKey::D0Gen && key <= LHCb::GenCountersFSR::CounterKey::ccGen ) ||
           ( key >= LHCb::GenCountersFSR::CounterKey::BGen && key <= LHCb::GenCountersFSR::CounterKey::B2starGen ) ||
           ( key >= LHCb::GenCountersFSR::CounterKey::DGen && key <= LHCb::GenCountersFSR::CounterKey::D2starGen ) ) {
        longlong before = genFSR.getDenominator( key );
        if ( before == 0 ) continue;
        longlong after = counter.second.second;
        if ( after == 0 ) continue;

        double error = genFSR.getEfficiencyError( after, before );
        htmlOutput << "<td>&plusmn; " << error << "</td>\n";
      }
    }
    htmlOutput << "</tr>\n</table>\n</td>\n</tr>" << std::endl;
  }

  //=============================================================================
  //  Add accepted hadron counters in the html file
  //=============================================================================

  void writeAccHadronCounters( LHCb::GenFSR& genFSR, int countAcc, std::ostream& htmlOutput ) {

    const auto& mapCross = LHCb::CrossSectionsFSR ::getFullNames();

    htmlOutput << "<tr>\n<td align=center>\n<table border=2>\n<th colspan=" << countAcc;
    htmlOutput << "><div class=firstcell> Accepted Hadron Counters</div></th>\n" << std::endl;
    htmlOutput << "<tr>\n";

    for ( auto const& counter : genFSR.genCounters() ) {
      auto key = counter.first;

      // Accepted Hadron Fractions
      if ( ( key >= LHCb::GenCountersFSR::CounterKey::B0Acc && key <= LHCb::GenCountersFSR::CounterKey::bbAcc ) ||
           ( key >= LHCb::GenCountersFSR::CounterKey::D0Acc && key <= LHCb::GenCountersFSR::CounterKey::ccAcc ) ||
           ( key >= LHCb::GenCountersFSR::CounterKey::BAcc && key <= LHCb::GenCountersFSR::CounterKey::B2starAcc ) ||
           ( key >= LHCb::GenCountersFSR::CounterKey::DAcc && key <= LHCb::GenCountersFSR::CounterKey::D2starAcc ) ) {

        std::string name     = LHCb::CrossSectionsFSR::CrossSectionKeyToString( key );
        const auto& fullName = mapCross.at( name );

        longlong before = genFSR.getDenominator( key );
        if ( before == 0 ) continue;

        longlong after = genFSR.getGenCounterInfo( key ).second;
        if ( after == 0 ) continue;

        htmlOutput << "<td><font size=2>" << fullName << "</font></td>\n";
      }
    }

    htmlOutput << "</tr>\n</table>\n</td>\n</tr>" << std::endl;
  }

  //=============================================================================
  //  Add generator cross-sections in the html file
  //=============================================================================

  void writeGeneratorCrossSections( LHCb::GenFSR& genFSR, int count, std::ostream& htmlOutput ) {
    htmlOutput << "<tr>\n<td align=center>\n<table border=2>\n<th colspan=" << 10;
    htmlOutput << "><div class=firstcell>Hard Generator Cross-Sections</div></th>\n" << std::endl;
    htmlOutput << "<tr>\n";

    std::vector<std::string> temp_names;

    for ( int irow = 0; irow < int( count / 10 ) + 1; irow++ ) {
      int nColumns = 0;

      // Generator Cross-Sections
      for ( auto const& cross : genFSR.crossSections() ) {
        std::string name = cross.second.first;
        name.erase( name.size() - 2 );

        for ( unsigned int icross = 0; icross < temp_names.size(); icross++ )
          if ( name == temp_names.at( icross ) ) continue;

        temp_names.push_back( name );

        double value = cross.second.second;

        if ( value == 0 || std::isnan( value ) != 0 ) continue;

        htmlOutput << "<td><font size=2>" << name << " (mb)</font></td>\n";

        nColumns++;
        if ( nColumns == 10 ) break;
      }

      htmlOutput << "</tr>\n\n<tr>\n";
      nColumns = 0;

      // Generator Cross-Sections
      for ( auto const& cross : genFSR.crossSections() ) {
        std::string name = cross.second.first;
        name.erase( name.size() - 2 );
        double value = cross.second.second;

        for ( unsigned int icross = 0; icross < temp_names.size(); icross++ )
          if ( name == temp_names.at( icross ) ) continue;

        if ( value == 0 || std::isnan( value ) != 0 ) continue;

        htmlOutput << "<td>" << value << "</td>\n";

        nColumns++;
        if ( nColumns == 10 ) break;
      }

      htmlOutput << "</tr>\n\n<tr>\n";
      htmlOutput << "<tr class=\"blank_row\"><td colspan=\"" << nColumns << "\"></td></tr>\n";
    }

    htmlOutput << "</tr>\n</table>\n</td>\n</tr>" << std::endl;
  }
} // namespace

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( GenFSRStat )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
GenFSRStat::GenFSRStat( const std::string& name, ISvcLocator* pSvcLocator ) : GaudiAlgorithm( name, pSvcLocator ) {}

//=============================================================================
// Initialization
//=============================================================================

StatusCode GenFSRStat::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first

  if ( sc.isFailure() ) return sc; // error printed already by GaudiAlgorithm
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

  // get the File Records service
  m_fileRecordSvc = Gaudi::svcLocator()->service( "FileRecordDataSvc" );

  m_navigatorTool = tool<IFSRNavigator>( "FSRNavigator", "FSRNavigator" );

  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode GenFSRStat::execute() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode GenFSRStat::finalize() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Finalize" << endmsg;

  GenFSRStat::printFSR();

  return GaudiAlgorithm::finalize(); // must be called after all other actions
}

//=============================================================================

//=============================================================================
//  Print the GenFSR in a file xml
//=============================================================================

void GenFSRStat::printFSR() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "write to file: " + m_htmlOutputName << endmsg;

  // make an inventory of the FileRecord store
  std::vector<std::string> addresses = m_navigatorTool->navigate( m_fileRecordName, m_FSRName );

  for ( const auto& genRecordAddress : addresses ) {
    DataObject* obj = nullptr;
    StatusCode  sc  = m_fileRecordSvc->retrieveObject( genRecordAddress, obj );

    if ( !sc.isSuccess() ) {
      error() << "Unable to retrieve object '" << genRecordAddress << "'" << endmsg;
      continue;
    }
    LHCb::GenFSR* genFSR = dynamic_cast<LHCb::GenFSR*>( obj );

    if ( !genFSR ) {
      warning() << "genFSR record was not found" << endmsg;
      if ( msgLevel( MSG::DEBUG ) ) debug() << genRecordAddress << " not found" << endmsg;
    } else {
      int evType = genFSR->getSimulationInfo( "evtType", 0 );
      int njobs  = genFSR->getSimulationInfo( "nJobs", 0 );

      std::string genMethod  = genFSR->getSimulationInfo( "generationMethod", "" );
      std::string genName    = genFSR->getSimulationInfo( "hardGenerator", "" );
      std::string decFiles   = genFSR->getSimulationInfo( "decFiles", "" );
      std::string evType_str = std::to_string( evType );
      std::string njobs_str  = std::to_string( njobs );
      std::string time       = getCurrentTime();

      int count1 = 0, count2 = 0, count3 = 0, count4 = 0, count5 = 0;

      // look in the FSR what kind if counters are stored
      for ( auto const& counter : genFSR->genCounters() ) {
        auto key = counter.first;

        if ( key == LHCb::GenCountersFSR::CounterKey::AfterFullEvt ||
             key == LHCb::GenCountersFSR::CounterKey::AfterLevelCut ||
             key == LHCb::GenCountersFSR::CounterKey::EvtSignal ||
             key == LHCb::GenCountersFSR::CounterKey::EvtantiSignal ||
             key == LHCb::GenCountersFSR::CounterKey::AfterPCut ||
             key == LHCb::GenCountersFSR::CounterKey::AfterantiPCut ) {
          int before = genFSR->getDenominator( key );
          int after  = genFSR->getGenCounterInfo( key ).second;
          if ( before == 0 || after == 0 ) continue;
          count1 += 1;
        } else if ( key <= LHCb::GenCountersFSR::CounterKey::bAndcAcc &&
                    key != LHCb::GenCountersFSR::CounterKey::BeforeFullEvt &&
                    key != LHCb::GenCountersFSR::CounterKey::AfterFullEvt ) {
          int before = genFSR->getDenominator( key );
          int after  = genFSR->getGenCounterInfo( key ).second;
          if ( before == 0 || after == 0 ) continue;
          count2 += 1;
        } else if ( ( key >= LHCb::GenCountersFSR::CounterKey::B0Gen &&
                      key <= LHCb::GenCountersFSR::CounterKey::bbGen ) ||
                    ( key >= LHCb::GenCountersFSR::CounterKey::D0Gen &&
                      key <= LHCb::GenCountersFSR::CounterKey::ccGen ) ||
                    ( key >= LHCb::GenCountersFSR::CounterKey::BGen &&
                      key <= LHCb::GenCountersFSR::CounterKey::B2starGen ) ||
                    ( key >= LHCb::GenCountersFSR::CounterKey::DGen &&
                      key <= LHCb::GenCountersFSR::CounterKey::D2starGen ) ) {
          int before = genFSR->getDenominator( key );
          int after  = genFSR->getGenCounterInfo( key ).second;
          if ( before == 0 || after == 0 ) continue;
          count3 += 1;
        } else if ( ( key >= LHCb::GenCountersFSR::CounterKey::B0Acc &&
                      key <= LHCb::GenCountersFSR::CounterKey::bbAcc ) ||
                    ( key >= LHCb::GenCountersFSR::CounterKey::D0Acc &&
                      key <= LHCb::GenCountersFSR::CounterKey::ccAcc ) ||
                    ( key >= LHCb::GenCountersFSR::CounterKey::BAcc &&
                      key <= LHCb::GenCountersFSR::CounterKey::B2starAcc ) ||
                    ( key >= LHCb::GenCountersFSR::CounterKey::DAcc &&
                      key <= LHCb::GenCountersFSR::CounterKey::D2starAcc ) ) {
          int before = genFSR->getDenominator( key );
          int after  = genFSR->getGenCounterInfo( key ).second;
          if ( before == 0 || after == 0 ) continue;
          count4 += 1;
        }
      }

      // look in the FSR if there are any generator cross-section is stored
      for ( auto const& cross : genFSR->crossSections() ) {
        double value = cross.second.second;
        if ( value == 0 || std::isnan( value ) != 0 ) continue;
        count5 += 1;
      }

      // read the already existing file
      std::fstream readOutput( m_htmlOutputLocation.value() + m_htmlOutputName.value(), std::fstream::in );
      // open the new file
      std::string   htmlTempName = "GenerationFSRTemp.html";
      std::ofstream htmlOutput( m_htmlOutputLocation.value() + htmlTempName, std::fstream::out );

      if ( htmlOutput.is_open() ) {
        if ( msgLevel( MSG::DEBUG ) )
          debug() << "Html output: " + m_htmlOutputLocation.value() + htmlTempName + " created." << endmsg;

        if ( readOutput.is_open() ) {
          if ( msgLevel( MSG::DEBUG ) )
            debug() << "Html input: " + m_htmlOutputLocation.value() + m_htmlOutputName + "opened." << endmsg;

          // hyper-link related to this simulation conditions
          std::string ref_evType =
              "<a href=\"GenerationFSR_" + m_appConfigFile + ".html#" + evType_str + "\">" + evType_str + "</a>";

          // link related to this simulation conditions
          std::string link = "<a name=\"" + evType_str + "\"></a>";

          bool        flag_evType = false;
          std::size_t pos_end, pos_ref;
          std::string line;

          // read the existing file GenerationFSR.html
          while ( getline( readOutput, line ) ) {
            // look for the hyper-link in the current line
            if ( !flag_evType ) pos_ref = line.find( ref_evType );

            if ( pos_ref != std::string::npos && !flag_evType ) flag_evType = true;

            // if there is not already a simulation for this production, here we need to add the hyperlink
            // to the table
            if ( line == "</p>" ) {
              if ( flag_evType )
                htmlOutput << line << std::endl;
              else
                htmlOutput << ref_evType << std::endl << line << std::endl;
            }
            // add the tables if the current line contains the right link or if the end of the file is reached
            else if ( ( line == link && flag_evType == true ) || ( flag_evType == false && line == "</body>" ) ) {
              // There is already a simulation for this production
              // but if we are here it's because we have to overwrite the previous results
              htmlOutput << link << std::endl; // write link

              std::string evtDesc = getEvtTypeDesc( evType );

              // Write the tables
              htmlOutput << "<table>\n<th colspan=\"2\"><font color=\"#0000FF\">" << evType_str << " (" << evtDesc
                         << ")</font> ";
              htmlOutput << "Hard Generator <font color=\"#088A08\">" << genName << "</font> ";
              htmlOutput << "Gauss <font color=\"#088A08\">" + m_gaussVersion + "</font>";
              htmlOutput << "<br> DecFiles <font color=\"#088A08\">" + decFiles + "</font>";
              htmlOutput << "<br> DDDB <font color=\"#FF0000\">" + m_dddb + "</font> - ";
              htmlOutput << "SIMCOND <font color=\"#FF0000\">" + m_simCond + "</font><br>";
              htmlOutput << "APPCONFIG <font color=\"#088A08\">" + m_appConfigVersion + "</font> ";
              htmlOutput << "<font color=\"#FF0000\">" + m_appConfigFile + "</font><br>";
              htmlOutput << time << "</th>\n<tr>\n\n";

              if ( count2 != 0 ) writeGeneratorCounters( *genFSR, count2, htmlOutput );
              if ( count3 != 0 ) writeGenHadronCounters( *genFSR, count3, htmlOutput );
              if ( count4 != 0 ) writeAccHadronCounters( *genFSR, count4, htmlOutput );
              if ( count5 != 0 ) writeGeneratorCrossSections( *genFSR, count5, htmlOutput );
              if ( count1 != 0 ) writeCutEfficiencies( *genFSR, count1, htmlOutput );

              htmlOutput << "</table>\n";

              // Write the warnings
              if ( count2 == 0 )
                htmlOutput << "<p><font color=\"#FF0000\">Warning: No Counters Interaction lines found</font></p>\n";
              if ( count3 == 0 && genMethod == "Inclusive" )
                htmlOutput
                    << "<p><font color=\"#FF0000\">Warning: No Generated Hadron Counters lines found</font></p>\n";
              if ( count4 == 0 )
                htmlOutput
                    << "<p><font color=\"#FF0000\">Warning: No Accepted Hadron Counters lines found</font></p>\n";
              if ( count5 == 0 )
                htmlOutput
                    << "<p><font color=\"#FF0000\">Warning: No Hard Generator Cross-Sections lines found</font></p>\n";
              if ( count1 == 0 )
                htmlOutput << "<p><font color=\"#FF0000\">Warning: No Signal Counters lines found</font></p>\n";

              writeGlobalStat( *genFSR, htmlOutput );

              htmlOutput << "<p> Statistics done (script version " << script_version << ") with " << njobs_str
                         << " jobs from the following ProdID:<font color=\"#0000FF\">" + m_prodID + "</font></p>\n";

              // skip the lines which contain the previous table because they have been just updated
              if ( flag_evType ) getline( readOutput, line );

              pos_end = line.find( "<a name=" );

              while ( pos_end == std::string::npos && line != "</body>" ) {
                getline( readOutput, line );
                pos_end = line.find( "<a name=" );
              }

              htmlOutput << line << std::endl;
            } else
              htmlOutput << line << std::endl;
          }
        }
        // the file does not exist, create it
        else {
          if ( msgLevel( MSG::DEBUG ) )
            debug() << m_htmlOutputLocation.toString() + m_htmlOutputName.toString()
                    << " does not exist, creating it now." << endmsg;

          // write the web-header
          htmlOutput << "<html>\n<head>\n";
          htmlOutput << "<link rel=\"stylesheet\" "
                        "href=\"http://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/css/lhcb.css\""
                     << " type=\"text/css\" media=\"screen\">\n";
          htmlOutput << "<link rel=\"stylesheet\" "
                        "href=\"http://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/gauss/css/css.css\""
                     << " type=\"text/css\" media=\"screen\">\n";
          htmlOutput << "<link rel=\"stylesheet\" href=\"css.css\" type=\"text/css\" media=\"screen\">\n";
          htmlOutput << "<title>Table for Generation</title>\n";
          htmlOutput << "</head>\n";

          htmlOutput << "\n<body>\n";
          htmlOutput << "<p> Event Type:\n<a href=\"GenerationFSR_" + m_appConfigFile + ".html#" << evType_str << "\">"
                     << evType_str << "</a>\n</p>\n";

          // link
          htmlOutput << "<a name=\"" << evType_str << "\"></a>\n" << std::endl;

          std::string evtDesc = getEvtTypeDesc( evType );
          // Write the tables
          htmlOutput << "<table>\n<th colspan=\"2\"><font color=\"#0000FF\">" << evType_str << " (" << evtDesc
                     << ")</font> ";
          htmlOutput << "Hard Generator <font color=\"#088A08\">" << genName << "</font> ";
          htmlOutput << "Gauss <font color=\"#088A08\">" + m_gaussVersion + ": ";
          htmlOutput << "</font>";
          htmlOutput << "<br> DecFiles <font color=\"#088A08\">" << decFiles << "</font>";
          htmlOutput << "<br> DDDB <font color=\"#FF0000\">" + m_dddb + "</font> - ";
          htmlOutput << "SIMCOND <font color=\"#FF0000\">" + m_simCond + "</font><br>";
          htmlOutput << "APPCONFIG <font color=\"#088A08\">" + m_appConfigVersion + "</font>";
          htmlOutput << "<font color=\"#FF0000\">" + m_appConfigFile + "</font><br>";
          htmlOutput << time << "</th>\n<tr>\n\n";

          if ( count2 != 0 ) writeGeneratorCounters( *genFSR, count2, htmlOutput );
          if ( count3 != 0 ) writeGenHadronCounters( *genFSR, count3, htmlOutput );
          if ( count4 != 0 ) writeAccHadronCounters( *genFSR, count4, htmlOutput );
          if ( count5 != 0 ) writeGeneratorCrossSections( *genFSR, count5, htmlOutput );
          if ( count1 != 0 ) writeCutEfficiencies( *genFSR, count1, htmlOutput );

          htmlOutput << "</table>\n";

          // Write the warnings
          if ( count2 == 0 )
            htmlOutput << "<p><font color=\"#FF0000\">Warning: No Counters Interaction lines found</font></p>\n";
          if ( count3 == 0 && genMethod == "Inclusive" )
            htmlOutput << "<p><font color=\"#FF0000\">Warning: No Generated Hadron Counters lines found</font></p>\n";
          if ( count4 == 0 )
            htmlOutput << "<p><font color=\"#FF0000\">Warning: No Accepted Hadron Counters lines found</font></p>\n";
          if ( count5 == 0 )
            htmlOutput
                << "<p><font color=\"#FF0000\">Warning: No Hard Generator Cross-Sections lines found</font></p>\n";
          if ( count1 == 0 )
            htmlOutput << "<p><font color=\"#FF0000\">Warning: No Signal Counters lines found</font></p>\n";

          writeGlobalStat( *genFSR, htmlOutput );

          htmlOutput << "<p> Statistics done (script version " << script_version << ") with " << njobs_str
                     << " jobs from the following ProdID:<font color=\"#0000FF\">" + m_prodID + "</font></p>\n";

          htmlOutput << "\n</body>\n</html>\n";
        }
        htmlOutput.close();

        // remove the old file
        if ( readOutput.is_open() ) {
          readOutput.close();
          fs::remove( m_htmlOutputLocation.value() + m_htmlOutputName.value() );
        }
        // create the new .html file
        fs::rename( m_htmlOutputLocation.value() + htmlTempName,
                    m_htmlOutputLocation.value() + m_htmlOutputName.value() );
      } else if ( msgLevel( MSG::DEBUG ) )
        debug() << "The output file was not opened correctly" << endmsg;
    }
  }
}
