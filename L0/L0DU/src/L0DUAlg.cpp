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

// from Gaudi
#include "GaudiKernel/SystemOfUnits.h"
//DAQEvent
#include "Event/RawEvent.h"
// local
#include "L0DUAlg.h"

#include "Event/L0DUBase.h"
using namespace Gaudi::Units;

//-----------------------------------------------------------------------------
// Implementation file for class : L0DUAlg
//
// 2007-10-25 : Olivier Deschamps
//
// Run the L0DU Emulator according to a Trigger Configuration Key (TCK)
// and produce L0DUReport on TES AND/OR rawBank
//
//-----------------------------------------------------------------------------
DECLARE_COMPONENT( L0DUAlg )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
L0DUAlg::L0DUAlg( const std::string& name,
                  ISvcLocator* pSvcLocator)
: L0AlgBase ( name , pSvcLocator )
  , m_config(0)
  , m_confTool(0)
  , m_emulator(0)
{
  declareProperty( "ProcessorDataLocations"  , m_dataLocations   );
  m_dataLocations.push_back(LHCb::L0ProcessorDataLocation::Calo);
  m_dataLocations.push_back(LHCb::L0ProcessorDataLocation::Muon);
  m_dataLocations.push_back(LHCb::L0ProcessorDataLocation::PileUp);
  declareProperty( "AddHerschel"             , m_addHC=false ); // add HRC to data location
 // configure DAQ
  declareProperty( "BankVersion"             , m_rawVsn   = 2 );
  declareProperty( "EmulatorTool"            , m_emulatorType="L0DUEmulatorTool");
  //
  declareProperty( "L0DUReportLocation"          , m_reportLocation = LHCb::L0DUReportLocation::Default );
  //
  declareProperty( "TCK"                     , m_tck="");
  declareProperty( "L0DUConfigProviderName"  , m_configName="L0DUConfig");
  declareProperty( "L0DUConfigProviderType"  , m_configType="L0DUMultiConfigProvider");
  declareProperty( "BankSourceID"            , m_rawSrcID = 0);
  m_rawBankType  = LHCb::RawBank::L0DU; // rawBank Type
  //
  if( context() == "Emulation" )m_reportLocation = LHCb::L0DUReportLocation::Emulated;


}

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode L0DUAlg::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize() ;
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;


  //---------------------------
  if(m_tck == ""){
    error() << "The Trigger Configuration Key is undefined" << endmsg;
    return StatusCode::FAILURE;
  }
  if( "0x" != m_tck.substr( 0, 2 ) ){
    error() << "The requested TCK value " << m_tck << " MUST be in hexadecimal format '0x" << m_tck << "'" << endmsg;
    return StatusCode::FAILURE;
  }


  //----------------------------
  // Get the L0DUAlg emulator tool
  //---------------------------
  m_emulator = tool<IL0DUEmulatorTool>(m_emulatorType,m_emulatorType);

  //--------------------------------
  // Get the L0DUAlg configuration tool
  //--------------------------------
  int itck;
  std::istringstream is( m_tck.c_str() );
  is >> std::hex >> itck;
  m_confTool = tool<IL0DUConfigProvider>(m_configType , m_configName );
  if ( msgLevel(MSG::DEBUG) )
    debug() << " loading the configuration for TCK = " << m_tck << " /  " << itck << endmsg;
  m_config   = m_confTool->config( itck );
  if( !m_config ) {
    error() << " Unable to load the configuration for TCK = " << m_tck << " /  " << itck << endmsg;
    return StatusCode::FAILURE;
  }
  info()<< "The L0DUConfig (TCK=" << m_tck << ") have been succesfully loaded" << endmsg;

  // add HRC to data location when requested (warning data must exist !)
  const std::string locHC = LHCb::L0ProcessorDataLocation::HC;
  bool isHC=false;
  for(std::vector<std::string>::iterator iloc=m_dataLocations.begin();m_dataLocations.end()!=iloc;iloc++){
    if( *iloc == locHC ){isHC=true;break;} // HC location already set in the dataLocations list by user (warning : data must really exist !!)
  }
  if(m_addHC && ! isHC ){isHC=true;m_dataLocations.push_back(locHC);}
  if( isHC )info() << "Herschel has been added to L0Processors replacing L0Calo pi0's" << endmsg;

  return sc;
}


//=============================================================================
// Main execution
//=============================================================================
StatusCode L0DUAlg::execute() {


  // process the emulator
  if ( msgLevel(MSG::DEBUG) )
    debug() << "Emulator processing ( Data = " << m_dataLocations << ", TCK = " << m_tck << " )" <<endmsg;
  StatusCode sc = m_emulator->process(m_config, m_dataLocations );
  if(sc.isFailure()){
    Error("Cannot process the emulator").ignore();
    return sc;
  }

  // push Report and Config on TES
  if(writeOnTES()){
    if ( msgLevel(MSG::DEBUG) ) debug() <<"Push L0DUReport on TES" << endmsg;
    LHCb::L0DUReport* report = new LHCb::L0DUReport(m_emulator->emulatedReport());
    std::string loc = dataLocation( m_reportLocation );
    put (report   , loc );
  }

  //push bank in RawBuffer
  if(writeBanks()){
    if ( msgLevel(MSG::DEBUG) )
      debug() << "Insert RawBank in rawEvent" << endmsg;
    const std::vector<unsigned int> block = m_emulator->bank(m_rawVsn);
    LHCb::RawEvent* raw = getOrCreate<LHCb::RawEvent,LHCb::RawEvent>(LHCb::RawEventLocation::Default);
    raw->addBank(m_rawSrcID , m_rawBankType , m_rawVsn , block);
  }

  return StatusCode::SUCCESS ;
}



