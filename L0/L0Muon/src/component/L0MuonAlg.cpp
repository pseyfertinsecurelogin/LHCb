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
#include <algorithm>
#include <math.h>
#include <set>

#include "L0MuonAlg.h"
#include "SubstituteEnvVarInPath.h"

// Gaudi interfaces
#include "GaudiKernel/IChronoStatSvc.h"

// from Event
#include "Event/L0MuonData.h"
#include "Event/MuonDigit.h"
#include "Event/ODIN.h"

// Registers (to fill optical links)
#include "ProcessorKernel/RegisterFactory.h"
#include "ProcessorKernel/TileRegister.h"

// Emulator
#include "L0MuonKernel/L0MuonKernelFromXML.h"
#include "L0MuonKernel/MuonTriggerUnit.h"
#include "L0MuonKernel/UnitFactory.h"
namespace {
  ///< Set the layouts to be used in fillOLsfromCoords
  static const auto m_layout = ///< pad layout for the whole MuonSystem
      MuonSystemLayout( MuonStationLayout( MuonLayout( 24, 8 ) ), MuonStationLayout( MuonLayout( 48, 8 ) ),
                        MuonStationLayout( MuonLayout( 48, 8 ) ), MuonStationLayout( MuonLayout( 12, 8 ) ),
                        MuonStationLayout( MuonLayout( 12, 8 ) ) );

  static const auto s_ollayout = ///< optical link layout for the whole MuonSystem
      MuonSystemLayout(
          MuonStationLayout( MuonLayout( 2, 4 ) ),
          MuonStationLayout( MuonLayout( 4, 1 ), MuonLayout( 4, 2 ), MuonLayout( 2, 2 ), MuonLayout( 2, 2 ) ),
          MuonStationLayout( MuonLayout( 4, 1 ), MuonLayout( 4, 2 ), MuonLayout( 2, 2 ), MuonLayout( 2, 2 ) ),
          MuonStationLayout( MuonLayout( 2, 2 ) ), MuonStationLayout( MuonLayout( 2, 2 ) ) );

  static const auto s_lulayout = ///< logical unit layout for the whole MuonSystem
      MuonSystemLayout(
          MuonStationLayout( MuonLayout( 0, 0 ) ),
          MuonStationLayout( MuonLayout( 8, 1 ), MuonLayout( 4, 2 ), MuonLayout( 2, 2 ), MuonLayout( 2, 2 ) ),
          MuonStationLayout( MuonLayout( 8, 1 ), MuonLayout( 4, 2 ), MuonLayout( 2, 2 ), MuonLayout( 2, 2 ) ),
          MuonStationLayout( MuonLayout( 0, 0 ), MuonLayout( 4, 2 ), MuonLayout( 2, 2 ), MuonLayout( 2, 2 ) ),
          MuonStationLayout( MuonLayout( 0, 0 ), MuonLayout( 4, 2 ), MuonLayout( 2, 2 ), MuonLayout( 2, 2 ) ) );

  static const auto s_stripH = ///< vertical strip layout for the whole MuonSystem
      MuonSystemLayout(
          MuonStationLayout( MuonLayout( 0, 0 ) ),
          MuonStationLayout( MuonLayout( 8, 8 ), MuonLayout( 4, 8 ), MuonLayout( 2, 8 ), MuonLayout( 2, 8 ) ),
          MuonStationLayout( MuonLayout( 8, 8 ), MuonLayout( 4, 8 ), MuonLayout( 2, 8 ), MuonLayout( 2, 8 ) ),
          MuonStationLayout( MuonLayout( 0, 0 ), MuonLayout( 4, 8 ), MuonLayout( 2, 8 ), MuonLayout( 2, 8 ) ),
          MuonStationLayout( MuonLayout( 0, 0 ), MuonLayout( 4, 8 ), MuonLayout( 2, 8 ), MuonLayout( 2, 8 ) ) );

  static const auto s_stripV = ///< horizontal strip layout for the whole MuonSystem
      MuonSystemLayout(
          MuonStationLayout( MuonLayout( 0, 0 ) ),
          MuonStationLayout( MuonLayout( 48, 1 ), MuonLayout( 48, 2 ), MuonLayout( 48, 2 ), MuonLayout( 48, 2 ) ),
          MuonStationLayout( MuonLayout( 48, 1 ), MuonLayout( 48, 2 ), MuonLayout( 48, 2 ), MuonLayout( 48, 2 ) ),
          MuonStationLayout( MuonLayout( 0, 0 ), MuonLayout( 12, 2 ), MuonLayout( 12, 2 ), MuonLayout( 12, 2 ) ),
          MuonStationLayout( MuonLayout( 0, 0 ), MuonLayout( 12, 2 ), MuonLayout( 12, 2 ), MuonLayout( 12, 2 ) ) );
} // namespace

DECLARE_COMPONENT( L0MuonAlg )

L0MuonAlg::L0MuonAlg( const std::string& name, ISvcLocator* pSvcLocator ) : L0AlgBase( name, pSvcLocator ) {

  declareProperty( "EnableTAE", m_enableTAE = false );
  // M1 M2 M3 M4 M5
  m_foiXSize = {4, 5, 0, 2, 4};
  m_foiYSize = {0, 0, 0, 1, 1};

  declareProperty( "TCK", m_tck = "" );
  declareProperty( "UseTCKFromData", m_useTCKFromData = false );
  declareProperty( "L0DUConfigProviderName", m_configName = "L0DUConfig" );
  declareProperty( "L0DUConfigProviderType", m_configType = "L0DUMultiConfigProvider" );

  declareProperty( "IgnoreCondDB", m_ignoreCondDB = false );
  declareProperty( "ConditionNameCB", m_conditionNameFOI = "Conditions/Online/L0MUON/Q1/SoftFOI" );
  declareProperty( "ConditionNamePB", m_conditionNameVersion = "Conditions/Online/L0MUON/Q1/Versions" );
  declareProperty( "ParameterNameFOIx", m_parameterNameFOIx = "SoftFOIX" );
  declareProperty( "ParameterNameFOIy", m_parameterNameFOIy = "SoftFOIY" );
  declareProperty( "ParameterNameVersion", m_parameterNameVersion = "EMUL" );

  declareProperty( "Version", m_version = 3 );

  declareProperty( "ConfigFile", m_configfile = "$PARAMFILESROOT/data/L0MuonKernel.xml" );
  declareProperty( "IgnoreM1", m_ignoreM1 = false );
  declareProperty( "ForceM3", m_forceM3 = false );

  declareProperty( "FoiXSize", m_foiXSize ); /// Obsolete (kept for backward compatibility)
  declareProperty( "FoiYSize", m_foiYSize ); /// Obsolete (kept for backward compatibility)

  declareProperty( "DebugMode", m_debug = false );

  // Default for Boole :
  declareProperty( "InputSource", m_inputSource = 0 );
  declareProperty( "DAQMode", m_mode = 1 );
  declareProperty( "Compression", m_compression = true );

  declareProperty( "LUTPath", m_lut_path = "$PARAMFILESROOT/data" );
  declareProperty( "LUTBaseName", m_lut_basename = "L0MuonM1M2LUT_" );
  declareProperty( "LUTVersion", m_lut_version = "V3" );

  declareProperty( "ModifyDigits", m_modify_digits = false );
  declareProperty( "ModifyDigitsToolType", m_modifyInputToolName = "L0MuonModifyInputTool" );
  declareProperty( "ModifyDigitsToolName", m_modifyInputToolType = "L0MuonModifyInputTool" );
}

StatusCode L0MuonAlg::initialize() {
  StatusCode sc = L0AlgBase::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;         // error printed already by GaudiAlgorithm

  m_lut = std::make_unique<L0MPtLUT>();

  // Instanciate the MuonTrigger Units and Registers
  L0Muon::RegisterFactory::selectInstance( 0 );
  std::string xmlFileName = L0MuonUtils::SubstituteEnvVarInPath( m_configfile );
  info() << "XML file = " << xmlFileName << endmsg;
  L0Muon::L0MuonKernelFromXML( xmlFileName );
  L0Muon::UnitFactory* ufactory = L0Muon::UnitFactory::instance();
  m_muontriggerunit             = dynamic_cast<L0Muon::L0MUnit*>( ufactory->topUnit() );
  if ( msgLevel( MSG::DEBUG ) ) debug() << "MuonTrigger build from xml " << endmsg;

  // Set debug mode
  if ( m_debug ) {
    m_muontriggerunit->setDebugMode();
    info() << "MuonTrigger debug mode " << endmsg;
  }

  //
  // Set LUT
  //
  std::string lutFileName = L0MuonUtils::SubstituteEnvVarInPath( m_lut_path + '/' + m_lut_basename + m_lut_version );
  bool        lut_ok      = m_lut->read( lutFileName );
  if ( lut_ok ) {
    m_muontriggerunit->setLUTPointer( m_lut.get() );
    info() << "MuonTrigger will use " << lutFileName << " to get pt of candidates" << endmsg;
  } else {
    info() << "Can not read LUT " << lutFileName << " MuonTrigger will compute pt according to formulae" << endmsg;
  }

  // Set properties
  // -- FOI : taken from the trigger configuration specified by the L0LTCK
  // -- 1) get L0DU config provider tool
  m_confTool = tool<IL0DUConfigProvider>( m_configType, m_configName );
  // -- 2) TCK specified in option
  if ( !m_useTCKFromData ) {
    // Check validity
    if ( m_tck == "" ) {
      error() << "The Trigger Configuration Key is undefined" << endmsg;
      return StatusCode::FAILURE;
    }
    if ( "0x" != m_tck.substr( 0, 2 ) ) {
      error() << "The requested TCK value " << m_tck << " MUST be in hexadecimal format '0x" << m_tck << "'" << endmsg;
      return StatusCode::FAILURE;
    }
    //--------------------------------
    std::istringstream is( m_tck.c_str() );
    is >> std::hex >> m_itck;
    // Read config and get FOI
    sc = updateL0TCKFOI();
    if ( sc.isFailure() ) return Error( "Unable to get the FOI from the TCK", StatusCode::FAILURE, 50 );
  } else {
    m_itck = -1;
    info() << "default FOI will be used until first event when the event TCK will be read" << endmsg;
  }

  // -- Emulator version is taken from condDB - register the conditions
  if ( !m_ignoreCondDB ) {
    if ( this->exist<Condition>( detSvc(), m_conditionNameVersion, false ) ) {
      debug() << "CondDB: accessing " << m_conditionNameVersion << endmsg;
      registerCondition( m_conditionNameVersion, m_l0CondCtrl, &L0MuonAlg::updateL0CondVersion );
    } else {
      warning() << "CondDB: cannot access " << m_conditionNameVersion << endmsg;
      warning() << "Emulation will run with the processor version defined in options." << endmsg;
    }
    sc = runUpdate();
    if ( sc.isFailure() ) {
      Warning( "Unable to register the L0Muon conditions ", StatusCode::FAILURE, 50 ).ignore();
      m_muontriggerunit->setProperties( l0MuonProperties() );
    }
  } else {
    m_muontriggerunit->setProperties( l0MuonProperties() ); // called in the updateL0CondVersion if m_ignoreCondDB is
                                                            // false
  }

  //   IChronoStatSvc * svc = chronoSvc();
  //   svc->chronoStart("L0MuonTrigger Initialize");

  // TAE slots names
  if ( m_enableTAE ) {
    m_tae_items[-7] = "Prev7/";
    m_tae_items[-6] = "Prev6/";
    m_tae_items[-5] = "Prev5/";
    m_tae_items[-4] = "Prev4/";
    m_tae_items[-3] = "Prev3/";
    m_tae_items[-2] = "Prev2/";
    m_tae_items[-1] = "Prev1/";
    m_tae_items[0]  = "";
    m_tae_items[1]  = "Next1/";
    m_tae_items[2]  = "Next2/";
    m_tae_items[3]  = "Next3/";
    m_tae_items[4]  = "Next4/";
    m_tae_items[5]  = "Next5/";
    m_tae_items[6]  = "Next6/";
    m_tae_items[7]  = "Next7/";
  }

  // Initialize
  m_muontriggerunit->initialize();

  //   svc->chronoStop("L0MuonTrigger Initialize");
  //   svc->chronoDelta("L0MuonTrigger Initialize",IChronoStatSvc::KERNEL);
  //   svc->chronoPrint("L0MuonTrigger Initialize");

  // L0MuonOutputs tool
  m_outputTool = tool<L0MuonOutputs>( "L0MuonOutputs", "OutputTool", this );

  //
  if ( m_modify_digits )
    m_modifyInputTool = tool<IL0MuonModifyInputTool>( m_modifyInputToolName, m_modifyInputToolType, this );

  m_totEvent = 0;
  m_totBx    = 0;

  return StatusCode::SUCCESS;
}

StatusCode L0MuonAlg::execute() {

  L0Muon::RegisterFactory::selectInstance( 0 );

  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << "-----------------------------------------------------------------" << endmsg;
    debug() << "-- Start execution:" << endmsg;
  }

  StatusCode sc;

  if ( m_useTCKFromData ) {
    LHCb::ODIN* odin = getIfExists<LHCb::ODIN>( LHCb::ODINLocation::Default, false );
    if ( NULL != odin ) {
      // read tck from odin
      unsigned int odintck = odin->triggerConfigurationKey();
      int          itck    = int( odintck & 0xFFFF );
      if ( itck != m_itck ) { // if it is a new tck
        int previous_tck = m_itck;
        m_itck           = itck;
        // update foi from this tck config
        sc = updateL0TCKFOI();
        if ( sc.isFailure() ) {
          Error( "Unable to get the FOI from the TCK, will keep current FOI", StatusCode::FAILURE, 50 ).ignore();
          m_itck = previous_tck;
        } else {
          // Set the properties of the MuonTriggerUnit
          m_muontriggerunit->setProperties( l0MuonProperties() );
        }
      }
    } else {
      Error( "ODIN not found at " + LHCb::ODINLocation::Default + ", unable to get TCK for this event",
             StatusCode::FAILURE, 50 )
          .ignore();
    }
  }

  // IChronoStatSvc * svc = chronoSvc();
  // svc->chronoStart("L0MuonTrigger Execute");

  int tae_size = 0;
  if ( m_enableTAE ) {
    LHCb::ODIN* odin = getIfExists<LHCb::ODIN>( LHCb::ODINLocation::Default, false );
    if ( NULL != odin ) {
      // TAE size from odin
      tae_size = int( odin->timeAlignmentEventWindow() );
    } else {
      Warning( "ODIN not found at " + LHCb::ODINLocation::Default + ", TAE mode requested but not used",
               StatusCode::FAILURE, 50 )
          .ignore();
    }
  }

  int ntae = 0;
  for ( int itae = -1 * tae_size; itae <= tae_size; ++itae ) {
    std::string rootInTes = m_tae_items[itae];

    sc = setProperty( "RootInTES", rootInTes );
    if ( sc.isFailure() ) return Error( "Unable to set RootInTES property of L0MuonAlg", StatusCode::SUCCESS, 50 );

    // Set rootInTES to point to the current TimeSlot
    sc = m_outputTool->setProperty( "RootInTES", rootInTES() );
    if ( sc.isFailure() ) continue; // error printed already by GaudiAlgorithm

    // Get list of hits used as input for the emulator
    if ( m_inputSource > 0 ) {
      // Use hits from L0MuonDatas
      sc = getDigitsFromL0Muon();
      if ( sc.isFailure() ) return Error( "Failed to get Hits from L0Muon ... abort", StatusCode::SUCCESS, 50 );
    } else if ( m_inputSource == 0 ) {
      // Use Muon Digits or Muon ZS raw data
      sc = getDigitsFromMuon();
      if ( sc.isFailure() ) return Error( "Failed to get Hits from Muon ... abort", StatusCode::SUCCESS, 50 );
    } else if ( m_inputSource < 0 ) {
      // Use NZS muon raw data
      sc = getDigitsFromMuonNZS();
      if ( sc.isFailure() ) return Error( "Failed to get Hits from Muon NZS bank ... abort", StatusCode::SUCCESS, 50 );
    }

    if ( m_modify_digits ) m_modifyInputTool->modifyInput( m_digits );

    // Fill the OL register with the input hits
    sc = fillOLsfromDigits();
    if ( sc.isFailure() ) { return Error( "Failed to load OLs ... abort", StatusCode::SUCCESS, 50 ); }

    // Preexecution phase: data exchange between PUs
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Preexecute of MuonKernel units ..." << endmsg;
    m_muontriggerunit->preexecute();

    // Execution phase: search for candidates and fill output registers
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Execution of MuonKernel units ..." << endmsg;
    m_muontriggerunit->execute();

    // Specify the version parameters to the output tool
    m_outputTool->setVersion( m_version, m_mode, m_compression );

    // Fill the Raw Event container
    if ( writeBanks() ) {
      if ( msgLevel( MSG::DEBUG ) ) debug() << "Fill RawEvent ..." << endmsg;
      sc = m_outputTool->writeRawBanks();
      if ( sc.isFailure() ) return sc;
    }

    // Write on TES
    if ( writeOnTES() ) {
      if ( msgLevel( MSG::DEBUG ) ) debug() << "Write on TES ..." << endmsg;
      sc = m_outputTool->writeOnTES( l0context() );
      if ( sc.isFailure() ) return sc;
    }

    // Fill the container for the L0DU (L0ProcessorData)
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Fill L0ProcessorData ..." << endmsg;
    sc = m_outputTool->writeL0ProcessorData();
    if ( sc.isFailure() ) return sc;

    // Postexecution phase: reset registers
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Postexecution of MuonKernel units ..." << endmsg;
    m_muontriggerunit->postexecute();

    ++m_totBx;
    ++ntae;
  } // End of loop over time slots
  if ( ntae == 0 ) return Error( "No valid time slice found", StatusCode::SUCCESS, 50 );

  // svc->chronoStop("L0MuonTrigger Execute");
  // svc->chronoDelta("L0MuonTrigger Execute", IChronoStatSvc::KERNEL);
  // if( MSG::DEBUG >= log.level() ) svc->chronoPrint("L0MuonTrigger Execute");

  ++m_totEvent;

  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << "-- Execution done." << endmsg;
    debug() << "-----------------------------------------------------------------" << endmsg;
  }

  return StatusCode::SUCCESS;
}

StatusCode L0MuonAlg::finalize() {

  L0Muon::UnitFactory* ufactory = L0Muon::UnitFactory::instance();
  delete ( ufactory );

  m_lut.reset();

  info() << "- ------------------------------------------------------------------" << endmsg;
  info() << "- ========> Final summary of the L0Muon trigger (emulator) <========" << endmsg;
  info() << "- Total number of events processed           : " << format( "%8d", m_totEvent ) << endmsg;
  info() << "- Total number of bunch crossings processed  : " << format( "%8d", m_totBx ) << endmsg;
  m_outputTool->statTot( info() );
  info() << "- ------------------------------------------------------------------" << endmsg;

  return L0AlgBase::finalize(); // must be called after all other actions
}

std::map<std::string, L0Muon::Property> L0MuonAlg::l0MuonProperties() {

  // Translate the gaudi Algorithms properties into the L0MuonKernel properties:

  std::map<std::string, L0Muon::Property> properties;
  std::string                             prop;
  char                                    buf[8];

  // xFOI
  prop = "";
  for ( auto ifoi = m_foiXSize.begin(); ifoi != m_foiXSize.end(); ifoi++ ) {
    sprintf( buf, "%2d", ( *ifoi ) );
    prop += buf;
    if ( std::next( ifoi ) != m_foiXSize.end() ) prop += ",";
  }
  properties["foiXSize"] = L0Muon::Property( prop );

  // yFOI
  prop = "";
  for ( auto ifoi = m_foiYSize.begin(); ifoi != m_foiYSize.end(); ifoi++ ) {
    sprintf( buf, "%2d", ( *ifoi ) );
    prop += buf;
    if ( std::next( ifoi ) != m_foiYSize.end() ) prop += ",";
  }
  properties["foiYSize"] = L0Muon::Property( prop );

  // ignoreM1
  properties["ignoreM1"] = m_ignoreM1 ? L0Muon::Property( "1" ) : L0Muon::Property( "0" );

  // ProcVersion
  sprintf( buf, "%3d", m_version );
  prop                      = buf;
  properties["procVersion"] = L0Muon::Property( prop );
  ;

  info() << "MuonTriggerUnit properties are:" << endmsg;
  for ( const auto& p : properties ) { info() << " " << p.first << " = " << p.second.value() << endmsg; }
  return properties;
}

StatusCode L0MuonAlg::getDigitsFromMuon() {

  m_digits.clear();

  // First try the digits on the TES if there (Boole)
  LHCb::MuonDigits* digits = getIfExists<LHCb::MuonDigits>( LHCb::MuonDigitLocation::MuonDigit );
  if ( digits ) {
    if ( msgLevel( MSG::DEBUG ) )
      debug() << "fillOLsfromDigits:  Getting hits from muon digits" << m_muonBuffer << endmsg;
    std::transform( digits->begin(), digits->end(), std::back_inserter( m_digits ),
                    []( const LHCb::MuonDigit* d ) { return d->key(); } );
    return StatusCode::SUCCESS;
  }

  // Otherwise, decode the Muon bank (ZS)
  if ( msgLevel( MSG::DEBUG ) ) debug() << "Getting hits from Muon Raw buffer (ZS)" << endmsg;

  if ( !m_muonBuffer ) {
    // First call: initialize the pointer to the Muon Raw Buffer Interface
    m_muonBuffer = tool<IMuonRawBuffer>( "MuonRawBuffer", "MuonRaw", this );
    if ( !m_muonBuffer ) {
      error() << "No pointer to muon raw buffer interface tool" << m_muonBuffer << endmsg;
      return StatusCode::FAILURE;
    }
  }

  IProperty* prop = dynamic_cast<IProperty*>( m_muonBuffer );
  if ( prop ) {
    StatusCode sc = prop->setProperty( "RootInTES", rootInTES() );
    if ( sc.isFailure() ) return Error( "Unable to set RootInTES property of MuonRawBuffer", sc );
  } else {
    return Error( "Unable to locate IProperty interface of MuonRawBuffer" );
  }
  StatusCode sc = m_muonBuffer->getTile( m_digits );
  if ( sc.isFailure() ) {
    m_digits.clear();
    return Error( "Unable to get the tiles from the MuonRawBuffer", StatusCode::FAILURE, 50 );
  }

  m_muonBuffer->forceReset();

  return StatusCode::SUCCESS;
}

StatusCode L0MuonAlg::getDigitsFromMuonNZS() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "Getting hits from Muon Raw buffer " << endmsg;

  m_digits.clear();

  if ( !m_muonBuffer ) {
    // First call: initialize the pointer to the Muon Raw Buffer Interface
    m_muonBuffer = tool<IMuonRawBuffer>( "MuonRawBuffer", "MuonRaw", this );
    if ( !m_muonBuffer ) {
      error() << "No pointer to muon raw buffer interface tool" << m_muonBuffer << endmsg;
      return StatusCode::FAILURE;
    }
  }

  IProperty* prop = dynamic_cast<IProperty*>( m_muonBuffer );
  if ( prop ) {
    StatusCode sc = prop->setProperty( "RootInTES", rootInTES() );
    if ( sc.isFailure() ) return Error( "Unable to set RootInTES property of MuonRawBuffer", sc );
  } else {
    return Error( "Unable to locate IProperty interface of MuonRawBuffer", StatusCode::FAILURE );
  }

  std::vector<std::pair<LHCb::MuonTileID, unsigned int>> tileAndTDC;
  StatusCode                                             sc = m_muonBuffer->getNZSupp( tileAndTDC );
  if ( sc.isFailure() ) {
    m_digits.clear();
    return Error( "Unable to get the tiles from the NZS MuonRawBuffer", StatusCode::FAILURE, 50 );
  }

  std::vector<std::pair<LHCb::MuonTileID, unsigned int>>::iterator itileAndTDC;
  for ( itileAndTDC = tileAndTDC.begin(); itileAndTDC < tileAndTDC.end(); ++itileAndTDC ) {
    m_digits.push_back( itileAndTDC->first );
  }

  m_muonBuffer->forceReset();

  return StatusCode::SUCCESS;
}

StatusCode L0MuonAlg::getDigitsFromL0Muon() {
  LHCb::L0MuonDatas* pdatas = getIfExists<LHCb::L0MuonDatas>( LHCb::L0MuonDataLocation::Default );
  if ( !pdatas ) return Error( "L0MuonDatas not found", StatusCode::FAILURE, 10 );

  if ( msgLevel( MSG::DEBUG ) ) debug() << "Getting hits from L0Muon itself" << endmsg;

  m_digits.clear();

  for ( const auto& data : *pdatas ) {
    LHCb::MuonTileID mkey = data->key();
    const auto&      ols  = data->ols();
    if ( msgLevel( MSG::VERBOSE ) ) {
      if ( ols.size() > 0 ) {
        verbose() << "  -  PU: " << mkey.toString() << " => " << ols.size() << " hits found" << endmsg;
      }
    }
    std::copy( ols.begin(), ols.end(), std::back_inserter( m_digits ) );
  }

  return StatusCode::SUCCESS;
}

StatusCode L0MuonAlg::fillOLsfromDigits() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "fillOLsfromDigits:  m_digits.size()= " << m_digits.size() << endmsg;

  if ( msgLevel( MSG::VERBOSE ) ) {
    for ( const auto& mkey : m_digits ) { verbose() << "fillOLsfromDigits:     mkey: " << mkey.toString() << endmsg; }
  }

  // -- Force M3 (add all M3 hits!!)
  if ( m_forceM3 ) {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "fillOLsfromDigits:  FORCING M3 TO 1 " << endmsg;
    auto m3StripV = s_stripV.stationLayout( 2 ).tiles( 2 ); // stripV in M3 Q3
    auto m3StripH = s_stripH.stationLayout( 2 ).tiles( 2 ); // stripH in M3 Q3
    for ( auto itstrip = m3StripV.begin(); itstrip < m3StripV.end(); ++itstrip ) itstrip->setStation( 2 );
    for ( auto itstrip = m3StripH.begin(); itstrip < m3StripH.end(); ++itstrip ) itstrip->setStation( 2 );
    m_digits.insert( m_digits.begin(), m3StripV.begin(), m3StripV.end() );
    m_digits.insert( m_digits.begin(), m3StripH.begin(), m3StripH.end() );
    if ( msgLevel( MSG::DEBUG ) ) debug() << "fillOLsfromDigits:  m_digits.size()= " << m_digits.size() << endmsg;
  } // -- force M3 done

  for ( LHCb::MuonTileID mkey : m_digits ) {

    if ( !mkey.isValid() ) {
      warning() << mkey.toString() << "is not valid ... skip tile" << endmsg;
      continue;
    }

    // Skip M1 tile
    if ( m_ignoreM1 && mkey.station() == 0 ) continue;

    // MuonTileID of the OL containing the hit
    LHCb::MuonTileID olID = s_ollayout.contains( mkey );

    // Name of the OL register
    char bufnm[1024];
    sprintf( bufnm, "(Q%d,R%d,%d,%d)", olID.quarter(), olID.region(), olID.nX(), olID.nY() );
    char buf[4096];
    sprintf( buf, "OL_%d_%s", mkey.station(), bufnm );

    // Get OL register from the register factory
    L0Muon::RegisterFactory::selectInstance( 0 );
    L0Muon::RegisterFactory* rfactory = L0Muon::RegisterFactory::instance();
    L0Muon::TileRegister*    pReg     = rfactory->createTileRegister( buf, 0 );

    // Add the hit to the register
    pReg->setTile( mkey );

    if ( msgLevel( MSG::VERBOSE ) ) {
      verbose() << "fillOLsfromDigits:  mkey: " << mkey.toString() << " olID: " << olID.toString() << " buf: " << buf
                << endmsg;
    }
  }

  if ( msgLevel( MSG::DEBUG ) ) { debug() << "fillOLsfromDigits:     DONE " << endmsg; }

  return StatusCode::SUCCESS;
}

StatusCode L0MuonAlg::updateL0CondFOI() {
  //
  // OBSOLETE - the FOI should now be taken from the TCK  (24/03/2011)
  //
  if ( !m_l0CondProc->exists( m_parameterNameFOIx ) ) {
    Error( m_parameterNameFOIx + " parameter does not exist in DB" ).ignore();
    Error( "Use default FOIX" ).ignore();
  } else {
    int condFOI = m_l0CondProc->paramAsInt( m_parameterNameFOIx );
    m_foiXSize  = {( condFOI >> 12 ) & 0xF, // M1
                  ( condFOI >> 8 ) & 0xF,  // M2
                  0,                       // M3
                  ( condFOI >> 4 ) & 0xF,  // M4
                  ( condFOI >> 0 ) & 0xF}; // M5
  }

  if ( !m_l0CondProc->exists( m_parameterNameFOIy ) ) {
    Error( m_parameterNameFOIy + " parameter does not exist in DB" ).ignore();
    Error( "Use default FOIY" ).ignore();
  } else {
    int condFOI = m_l0CondProc->paramAsInt( m_parameterNameFOIy );
    m_foiYSize  = {( condFOI >> 12 ) & 0xF, // M1
                  ( condFOI >> 8 ) & 0xF,  // M2
                  0,                       // M3
                  ( condFOI >> 4 ) & 0xF,  // M4
                  ( condFOI >> 0 ) & 0xF}; // M5
  }

  // Set the properties of the MuonTriggerUnit
  m_muontriggerunit->setProperties( l0MuonProperties() );

  return StatusCode::SUCCESS;
}

StatusCode L0MuonAlg::updateL0CondVersion() {
  if ( !m_l0CondCtrl->exists( m_parameterNameVersion ) ) {
    Error( "Q1CB0.versionEMUL parameter does not exist in DB" ).ignore();
    Error( "Use default processor version" ).ignore();
  } else {
    std::string s_version = m_l0CondCtrl->paramAsString( m_parameterNameVersion );
    m_version             = int( atof( s_version.c_str() ) );
  }
  // Set the properties of the MuonTriggerUnit
  info() << "updateL0CondVersion: new version in condDB, updating emulator parameters" << endmsg;
  m_muontriggerunit->setProperties( l0MuonProperties() );

  return StatusCode::SUCCESS;
}

StatusCode L0MuonAlg::updateL0TCKFOI() {
  info() << "Loading configuration for TCK = 0x" << std::hex << m_itck << std::dec << endmsg;
  LHCb::L0DUConfig* l0duconfig = m_confTool->config( m_itck );
  if ( NULL == l0duconfig ) {
    error() << " Unable to load the configuration for TCK = 0x" << std::hex << m_itck << std::dec << endmsg;
    return StatusCode::FAILURE;
  }
  m_foiXSize = l0duconfig->muonFOIx();
  m_foiYSize = l0duconfig->muonFOIy();

  return StatusCode::SUCCESS;
}
