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

//=============================================================================
/** @file DeRichSystem.cpp
 *
 * Implementation file for class : DeRichSystem
 *
 * @author Antonis Papanestis a.papanestis@rl.ac.uk
 * @date   2006-01-27
 */
//=============================================================================

// Gaudi
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/SmartDataPtr.h"

// RichUtils
#include "RichUtils/RichPDIdentifier.h"

// LHCbKernel
#include "Kernel/RichSmartID32.h"

// DetDesc
#include "DetDesc/Condition.h"

// local
#include "RichDet/DeRich.h"
#include "RichDet/DeRichSystem.h"

// boost
#include "boost/format.hpp"
#include "boost/lexical_cast.hpp"

//=============================================================================

const CLID CLID_DERichSystem = 12005; // User defined

// Retrieve Pointer to class defininition structure
const CLID& DeRichSystem::classID() { return CLID_DERichSystem; }

//=========================================================================
//  initialize
//=========================================================================
StatusCode DeRichSystem::initialize() {

  setMyName( "DeRichSystem" );

  _ri_debug << "Initialize " << name() << endmsg;

  // get the version number
  if ( exists( "systemVersion" ) ) { m_version = param<int>( "systemVersion" ); }
  _ri_debug << "System version:" << systemVersion() << endmsg;

  // get rich detectors
  std::vector<std::string> deRichLocs = getDeRichLocations();

  // get condition names for detector numbers
  std::vector<std::string> detCondNames;
  const std::string        str_PhotoDetConfig      = "RichPhotoDetectorConfiguration";
  const std::string        str_PhotoDetConfigValue = "DetectorConfiguration";
  if ( hasCondition( str_PhotoDetConfig ) ) {
    const auto deRC = condition( str_PhotoDetConfig );
    if ( deRC->exists( str_PhotoDetConfigValue ) )
      m_photDetConf = (Rich::RichPhDetConfigType)deRC->param<int>( str_PhotoDetConfigValue );
  }
  if ( m_photDetConf == Rich::PMTConfig ) {
    detCondNames.push_back( "Rich1PMTDetectorNumbers" );
    detCondNames.push_back( "Rich2PMTDetectorNumbers" );
  } else if ( m_photDetConf == Rich::HPDConfig ) {
    detCondNames.push_back( "Rich1DetectorNumbers" );
    detCondNames.push_back( "Rich2DetectorNumbers" );
  } else {
    return Error( "Unknown detector configuration. " );
  }

  // check if the numbers match.
  if ( 0 != detCondNames.size() % deRichLocs.size() ) {
    return Error( "Number of rich detector does not match detector number conditions" );
  }

  // for version 1 there are separate conditions for inactive PDs
  std::vector<std::string> inactiveCondNames;
  if ( systemVersion() == 1 ) {
    inactiveCondNames.push_back( "Rich1InactivePDs" );
    inactiveCondNames.push_back( "Rich2InactivePDs" );
  }

  // loop over detectors and conditions to set things up
  for ( unsigned int i = 0; i < deRichLocs.size(); ++i ) {
    m_detNumConds[(Rich::DetectorType)i] = detCondNames[i];

    updMgrSvc()->registerCondition( this, condition( detCondNames[i] ).path(), &DeRichSystem::buildPDMappings );
    _ri_debug << "Registered:" << condition( detCondNames[i] ).path() << endmsg;

    if ( systemVersion() == 1 ) {
      m_inactivePDConds[(Rich::DetectorType)i] = inactiveCondNames[i];
      updMgrSvc()->registerCondition( this, condition( inactiveCondNames[i] ).path(), &DeRichSystem::buildPDMappings );
      _ri_debug << "Registered:" << condition( inactiveCondNames[i] ).path() << endmsg;
    }
  }

  // Load the RICH detectors
  for ( const auto rich : Rich::detectors() ) {
    SmartDataPtr<DeRich> deR( dataSvc(), DeRichLocations::location( rich ) );
    m_deRich[rich] = deR;
  }

  // Run first update
  const auto sc = updMgrSvc()->update( this );
  if ( sc.isFailure() ) error() << "Failed to update mappings" << endmsg;

  _ri_debug << "DeRichSystem initialized " << endmsg;

  return sc;
}

//=========================================================================
//  buildPDMappings
//=========================================================================
StatusCode DeRichSystem::buildPDMappings() {
  _ri_debug << "Update triggered for PD numbering maps" << endmsg;

  // clear maps and containers
  m_soft2hard.clear();
  m_hard2soft.clear();
  m_l0hard2soft.clear();
  m_activePDSmartIDs.clear();
  m_inactivePDSmartIDs.clear();
  m_allPDSmartIDs.clear();
  m_activePDHardIDs.clear();
  m_inactivePDHardIDs.clear();
  m_allPDHardIDs.clear();
  m_smartid2L0.clear();
  m_hardid2L0.clear();
  m_l0ToL1.clear();
  m_smartid2L1.clear();
  m_hardid2L1.clear();
  m_smartid2L1In.clear();
  m_hardid2L1In.clear();
  m_l1ToRich.clear();
  m_l12smartids.clear();
  m_l12hardids.clear();
  m_L1HardIDAndInputToPDHardID.clear();
  m_l1IDs.clear();
  m_smartid2copyNumber.clear();
  m_copyNumber2smartid.clear();
  m_l1H2CopyN.clear();
  m_l1LogToHard = {{}};
  m_l1HardToLog.clear();

  // Fill the maps for each RICH
  for ( const auto rich : Rich::detectors() ) {
    const StatusCode sc = fillMaps( rich );
    if ( !sc ) { return sc; }
  }

  return StatusCode::SUCCESS;
}

//=========================================================================
//  fillMaps
//=========================================================================
StatusCode DeRichSystem::fillMaps( const Rich::DetectorType rich ) {
  _ri_debug << "Building Mappings for " << rich << endmsg;

  const std::string str_PhotoDetConfig      = "RichPhotoDetectorConfiguration";
  const std::string str_PhotoDetConfigValue = "DetectorConfiguration";

  if ( hasCondition( str_PhotoDetConfig ) ) {
    const auto deRC = condition( str_PhotoDetConfig );
    if ( deRC->exists( str_PhotoDetConfigValue ) )
      m_photDetConf = (Rich::RichPhDetConfigType)deRC->param<int>( str_PhotoDetConfigValue );
  }

  std::string str_NumberOfPDs                  = "";
  std::string str_PDSmartIDs                   = "";
  std::string str_PDHardwareIDs                = "";
  std::string str_PDLevel0IDs                  = "";
  std::string str_PDLevel1HardwareIDs          = "";
  std::string str_PDLevel1InputNums            = "";
  std::string str_PDCopyNumbers                = "";
  std::string str_InactivePDListInSmartIDs     = "";
  std::string str_InactivePDs                  = "";
  std::string str_PDLevel1IDs                  = "";
  std::string str_Level1LogicalToHardwareIDMap = "";

  if ( m_photDetConf == Rich::PMTConfig ) {
    str_NumberOfPDs                  = "NumberOfPMTs";
    str_PDSmartIDs                   = "PMTSmartIDs";
    str_PDHardwareIDs                = "PMTHardwareIDs";
    str_PDLevel0IDs                  = "PMTLevel0IDs";
    str_PDLevel1HardwareIDs          = "PMTLevel1HardwareIDs";
    str_PDLevel1InputNums            = "PMTLevel1InputNums";
    str_PDCopyNumbers                = "PMTCopyNumbers";
    str_InactivePDListInSmartIDs     = "InactivePMTListInSmartIDs";
    str_InactivePDs                  = "InactivePMTs";
    str_PDLevel1IDs                  = "PMTLevel1IDs";
    str_Level1LogicalToHardwareIDMap = "Level1LogicalToHardwareIDMap-PMT";
  } else if ( m_photDetConf == Rich::HPDConfig ) {
    str_NumberOfPDs                  = "NumberOfHPDs";
    str_PDSmartIDs                   = "HPDSmartIDs";
    str_PDHardwareIDs                = "HPDHardwareIDs";
    str_PDLevel0IDs                  = "HPDLevel0IDs";
    str_PDLevel1HardwareIDs          = "HPDLevel1HardwareIDs";
    str_PDLevel1InputNums            = "HPDLevel1InputNums";
    str_PDCopyNumbers                = "HPDCopyNumbers";
    str_InactivePDListInSmartIDs     = "InactiveHPDListInSmartIDs";
    str_InactivePDs                  = "InactiveHPDs";
    str_PDLevel1IDs                  = "HPDLevel1IDs";
    str_Level1LogicalToHardwareIDMap = "Level1LogicalToHardwareIDMap";
  } else
    return Error( "Unknown detector configuration. " );

  // load conditions
  _ri_debug << "Loading Conditions from " << m_detNumConds[rich] << endmsg;
  const auto numbers = condition( m_detNumConds[rich] );
  _ri_debug << m_detNumConds[rich] << " since:" << numbers->validSince().format( true )
            << " till:" << numbers->validTill().format( true ) << endmsg;

  SmartRef<Condition> inactives;
  if ( systemVersion() == 1 ) {
    inactives = condition( m_inactivePDConds[rich] );
    _ri_debug << "Inactive list since:" << inactives->validSince().format( true )
              << " till:" << inactives->validTill().format( true ) << endmsg;
  }

  // local typedefs for vector from Conditions
  typedef std::vector<LHCb::RichSmartID::KeyType> CondData;

  // number of PDs
  const unsigned int nPDs = numbers->param<int>( str_NumberOfPDs );
  _ri_verbo << "Condition " << str_NumberOfPDs << " = " << nPDs << endmsg;

  // vector of PD RichSmartIDs
  const auto& softIDs = numbers->paramVect<int>( str_PDSmartIDs );
  _ri_verbo << "Condition " << str_PDSmartIDs << " = " << softIDs << endmsg;

  // vector of PD hardware IDs
  const auto& hardIDs = numbers->paramVect<int>( str_PDHardwareIDs );
  _ri_verbo << "Condition " << str_PDHardwareIDs << " = " << hardIDs << endmsg;

  // vector of PD Level0 IDs
  const auto& l0IDs = numbers->paramVect<int>( str_PDLevel0IDs );
  _ri_verbo << "Condition " << str_PDLevel0IDs << " = " << l0IDs << endmsg;

  // vector of PD Level1 board Hardware IDs
  const auto& l1IDs = numbers->paramVect<int>( str_PDLevel1HardwareIDs );
  _ri_verbo << "Condition " << str_PDLevel1HardwareIDs << " = " << l1IDs << endmsg;

  // vector of PD Level1 input numbers
  const auto& l1Ins = numbers->paramVect<int>( str_PDLevel1InputNums );
  _ri_verbo << "Condition " << str_PDLevel1InputNums << " = " << l1Ins << endmsg;

  // vector of PD Copy numbers
  const auto& copyNs = numbers->paramVect<int>( str_PDCopyNumbers );
  _ri_verbo << "Condition " << str_PDCopyNumbers << " = " << copyNs << endmsg;

  // inactive PDs
  CondData   inacts;
  const bool inactivePDListInSmartIDs( numbers->exists( str_InactivePDListInSmartIDs ) || systemVersion() == 1 );
  _ri_verbo << "Condition " << str_InactivePDListInSmartIDs << " exists = " << inactivePDListInSmartIDs << endmsg;
  if ( inactivePDListInSmartIDs ) {
    // smartIDs
    _ri_debug << "Inactive PDs are taken from the smartID list" << endmsg;
    const auto& inactsHuman = ( systemVersion() == 1 ? inactives->paramVect<int>( str_InactivePDListInSmartIDs )
                                                     : numbers->paramVect<int>( str_InactivePDListInSmartIDs ) );

    inacts.reserve( inactsHuman.size() );
    for ( const auto inpd : inactsHuman ) {
      const LHCb::RichSmartID ID( Rich::DAQ::PDIdentifier( inpd ).smartID() );
      _ri_debug << "Inactive SmartID " << inpd << " : " << ID << endmsg;
      if ( ID.isValid() ) {
        inacts.push_back( ID.key() );
        if ( !std::any_of( softIDs.begin(), softIDs.end(),
                           [&ID]( const auto& sID ) { return ID == LHCb::RichSmartID( sID ); } ) ) {
          warning() << "Inactive SmartID in list of Active IDs : " << inpd << endmsg;
        }
      } else {
        error() << "Invalid SmartID in the list of inactive PDs : " << inpd << endmsg;
      }
    }
  }

  else {
    // hardware IDs
    _ri_debug << "Inactive PDs are taken from the hardware list" << endmsg;
    inacts.clear();
    for ( const auto& i : numbers->paramVect<int>( str_InactivePDs ) ) { inacts.push_back( i ); }
  }
  _ri_verbo << "Condition InactiveHPDs = " << inacts << endmsg;

  // check consistency
  if ( nPDs != softIDs.size() || //
       nPDs != hardIDs.size() || //
       nPDs != l1IDs.size() ||   //
       nPDs != l0IDs.size() ||   //
       nPDs != l1Ins.size() ||   //
       nPDs != copyNs.size() ) {
    error() << "Mismatch in " << rich                     //
            << " PD numbering schemes : # PDs = " << nPDs //
            << " # SmartIDs = " << softIDs.size()         //
            << " # HardIDs = " << hardIDs.size()          //
            << " # L0IDs = " << l0IDs.size()              //
            << " # L1BoardIDs = " << l1IDs.size()         //
            << " # L1InputIDs = " << l1Ins.size()         //
            << " # CopyNumbers = " << copyNs.size()       //
            << endmsg;
    return StatusCode::FAILURE;
  }

  // current number of L1 boards
  const auto saveL1size = m_l1IDs.size();

  // build cached mappings
  for ( auto iSoft( softIDs.begin() ), //
        iHard( hardIDs.begin() ),      //
        iL1( l1IDs.begin() ),          //
        iL1In( l1Ins.begin() ),        //
        iL0( l0IDs.begin() ),          //
        icopyN( copyNs.begin() );
        iSoft != softIDs.end() && //
        iHard != hardIDs.end() && //
        iL0 != l0IDs.end() &&     //
        iL1 != l1IDs.end() &&     //
        iL1In != l1Ins.end() &&   //
        icopyN != copyNs.end();
        ++iSoft, ++iHard, ++iL0, ++iL1, ++iL1In, ++icopyN ) {

    // get data
    const LHCb::RichSmartID32         pdID32( *iSoft ); // needed for 32->64 bit support
    const LHCb::RichSmartID           pdID( pdID32 );   // handles correct format conversion
    const Rich::DAQ::PDHardwareID     hardID( *iHard );
    const Rich::DAQ::Level1HardwareID L1ID( *iL1 );
    const Rich::DAQ::Level0ID         L0ID( *iL0 );
    const Rich::DAQ::HPD::Level1Input L1IN( *iL1In );
    const Rich::DAQ::PDCopyNumber     copyN( *icopyN );

    // debug printout
    _ri_verbo << "PD     " << pdID << " PDhardID " << hardID << " L0 " << L0ID << " L1 HardID " << L1ID << " L1 input "
              << L1IN << endmsg;

    // Sanity checks that this PD is not already in the maps
    if ( m_soft2hard.find( pdID ) != m_soft2hard.end() ) {
      error() << "Multiple entries for PD RichSmartID " << pdID;
      return StatusCode::FAILURE;
    }
    if ( m_hard2soft.find( hardID ) != m_hard2soft.end() ) {
      error() << "Multiple entries for PD hardware ID " << (std::string)hardID << " " << pdID << endmsg;
      return StatusCode::FAILURE;
    }
    if ( m_l0hard2soft.find( L0ID ) != m_l0hard2soft.end() ) {
      error() << "Multiple entries for PD L0 ID " << (std::string)L0ID << endmsg;
      return StatusCode::FAILURE;
    }
    if ( m_copyNumber2smartid.find( copyN ) != m_copyNumber2smartid.end() ) {
      error() << "Multiple entries for PD copy number " << (std::string)copyN << pdID << endmsg;
      return StatusCode::FAILURE;
    }

    // set up mappings etc.

    const auto myID = ( inactivePDListInSmartIDs ? pdID.key() : LHCb::RichSmartID::KeyType( *iHard ) );
    if ( std::find( inacts.begin(), inacts.end(), myID ) == inacts.end() ) {
      m_activePDSmartIDs.push_back( pdID );
      m_activePDHardIDs.push_back( hardID );
      _ri_debug << "PD " << pdID << " hardID " << hardID << " is ACTIVE" << endmsg;
    } else {
      if ( !pdIsActive( hardID ) ) {
        error() << "PD " << pdID << " hardID " << hardID << " listed twice in INACTIVE PD list !" << endmsg;
      } else {
        m_inactivePDSmartIDs.push_back( pdID );
        m_inactivePDHardIDs.push_back( hardID );
        _ri_debug << "PD " << pdID << " hardID " << hardID << " is INACTIVE" << endmsg;
      }
    }

    // Fill maps
    m_allPDHardIDs.push_back( hardID );
    m_allPDSmartIDs.push_back( pdID );
    bool OK = true;
    OK &= safeMapFill( pdID, hardID, m_soft2hard );
    OK &= safeMapFill( hardID, pdID, m_hard2soft );
    OK &= safeMapFill( L0ID, pdID, m_l0hard2soft );
    OK &= safeMapFill( pdID, L0ID, m_smartid2L0 );
    OK &= safeMapFill( hardID, L0ID, m_hardid2L0 );
    OK &= safeMapFill( pdID, L1ID, m_smartid2L1 );
    OK &= safeMapFill( hardID, L1ID, m_hardid2L1 );
    OK &= safeMapFill( pdID, L1IN, m_smartid2L1In );
    OK &= safeMapFill( hardID, L1IN, m_hardid2L1In );
    OK &= safeMapFill( L0ID, L1ID, m_l0ToL1 );
    OK &= safeMapFill( pdID, copyN, m_smartid2copyNumber );
    OK &= safeMapFill( copyN, pdID, m_copyNumber2smartid );
    m_l12smartids[L1ID].push_back( pdID );
    m_l12hardids[L1ID].push_back( hardID );
    const L1HardIDAndInput idAndInput( L1ID, L1IN );
    OK &= safeMapFill( idAndInput, hardID, m_L1HardIDAndInputToPDHardID );
    if ( std::find( m_l1IDs.rbegin(), m_l1IDs.rend(), L1ID ) == m_l1IDs.rend() ) {
      m_l1ToRich[L1ID] = rich;
      m_l1IDs.push_back( L1ID );
    }
    if ( !OK ) return StatusCode::FAILURE;

  } // end loop over conditions data

  // Sort PD lists
  std::stable_sort( m_activePDHardIDs.begin(), m_activePDHardIDs.end() );
  std::stable_sort( m_activePDSmartIDs.begin(), m_activePDSmartIDs.end() );
  std::stable_sort( m_inactivePDHardIDs.begin(), m_inactivePDHardIDs.end() );
  std::stable_sort( m_inactivePDSmartIDs.begin(), m_inactivePDSmartIDs.end() );
  std::stable_sort( m_allPDHardIDs.begin(), m_allPDHardIDs.end() );
  std::stable_sort( m_allPDSmartIDs.begin(), m_allPDSmartIDs.end() );

  // L1 mapping
  const std::string L1LogToHardMapName( str_Level1LogicalToHardwareIDMap );

  if ( numbers->exists( L1LogToHardMapName ) ) {
    for ( const auto& id : numbers->paramVect<std::string>( L1LogToHardMapName ) ) {
      // make a copy of the string
      auto data = id;
      // Strip extra " characters if present at start and end of string
      // To work around a small typo in the DB - Can be removed at some later date
      if ( data[0] == '"' ) { data = data.substr( 1, data.size() ); }
      if ( data[data.size() - 1] == '"' ) { data = data.substr( 0, data.size() - 1 ); }
      // Format of string is 'LogicalID/HardwareID'
      const auto slash = data.find_first_of( "/" );
      if ( slash == 0 ) {
        error() << "Badly formed " << L1LogToHardMapName << " for " << rich << endmsg;
        return StatusCode::FAILURE;
      }
      const Rich::DAQ::Level1LogicalID  logID( boost::lexical_cast<int>( data.substr( 0, slash ) ) );
      const Rich::DAQ::Level1HardwareID hardID( boost::lexical_cast<int>( data.substr( slash + 1 ) ) );
      const Rich::DetectorType          richTmp = this->richDetector( hardID );
      _ri_debug << richTmp << " L1 ID mapping : Logical=" << logID << " Hardware=" << hardID << endmsg;
      ( m_l1LogToHard[rich] )[logID] = hardID;
      m_l1HardToLog[hardID]          = logID;
    }
  } else {
    return Error( "Could not load Condition " + L1LogToHardMapName );
  }

  // L1 Logical ID to Copy Numbers
  // Create on the fly here. Should add to CondDB really ...
  unsigned int firstL1CopyN{0};
  for ( const auto& L1HID : m_l1IDs ) {
    if ( m_l1H2CopyN.find( L1HID ) == m_l1H2CopyN.end() ) {
      m_l1H2CopyN[L1HID] = Rich::DAQ::Level1CopyNumber( firstL1CopyN++ );
      _ri_debug << "L1 Copy Number " << m_l1H2CopyN[L1HID] << " -> HardwareID=" << L1HID
                << " LogicalID=" << level1LogicalID( L1HID ) << endmsg;
    }
  }

  _ri_debug << "Built mappings for " << boost::format( "%2i" ) % ( m_l1IDs.size() - saveL1size ) //
            << " L1 and " << nPDs << " PDs in " << rich << endmsg;

  return StatusCode::SUCCESS;
}

//=========================================================================
//  hardwareID
//=========================================================================
const Rich::DAQ::PDHardwareID DeRichSystem::hardwareID( const LHCb::RichSmartID& smartID ) const {
  // See if this RichSmartID is known
  const auto id = m_soft2hard.find( smartID.pdID() );
  if ( UNLIKELY( m_soft2hard.end() == id ) ) {
    std::ostringstream mess;
    mess << "Unknown PD RichSmartID " << smartID.pdID();
    throw GaudiException( mess.str(), "DeRichSystem::hardwareID", StatusCode::FAILURE );
  }

  // Found, so return hardware ID
  return ( *id ).second;
}

//=========================================================================
//  richSmartID
//=========================================================================
const LHCb::RichSmartID DeRichSystem::richSmartID( const Rich::DAQ::PDHardwareID& hID ) const {
  // See if this PD hardware ID is known
  const auto id = m_hard2soft.find( hID );
  if ( UNLIKELY( m_hard2soft.end() == id ) ) {
    throw GaudiException( "Unknown PD hardware ID " + (std::string)hID, "DeRichSystem::richSmartID",
                          StatusCode::FAILURE );
  }

  // Found, so return RichSmartID
  return ( *id ).second;
}

//=========================================================================
//  richSmartID
//=========================================================================
const LHCb::RichSmartID DeRichSystem::richSmartID( const Rich::DAQ::Level0ID& l0ID ) const {
  // See if this Level0 hardware ID is known
  const auto id = m_l0hard2soft.find( l0ID );
  if ( UNLIKELY( m_l0hard2soft.end() == id ) ) {
    throw GaudiException( "Unknown Level0 hardware ID " + (std::string)l0ID, "DeRichSystem::richSmartID",
                          StatusCode::FAILURE );
  }

  // Found, so return RichSmartID
  return ( *id ).second;
}

//=========================================================================
// level0ID
//=========================================================================
const Rich::DAQ::Level0ID DeRichSystem::level0ID( const LHCb::RichSmartID& smartID ) const {
  // See if this RichSmartID is known
  const auto id = m_smartid2L0.find( smartID.pdID() );
  if ( UNLIKELY( m_smartid2L0.end() == id ) ) {
    std::ostringstream mess;
    mess << "Unknown PD RichSmartID " << smartID.pdID();
    throw GaudiException( mess.str(), "DeRichSystem::level0ID", StatusCode::FAILURE );
  }

  // Found, so return Level 0 ID
  return ( *id ).second;
}

//=========================================================================
// level1HardwareID
//=========================================================================
const Rich::DAQ::Level1HardwareID DeRichSystem::level1HardwareID( const LHCb::RichSmartID& smartID ) const {
  // See if this RichSmartID is known
  const auto id = m_smartid2L1.find( smartID.pdID() );
  if ( UNLIKELY( m_smartid2L1.end() == id ) ) {
    std::ostringstream mess;
    mess << "Unknown PD RichSmartID " << smartID.pdID();
    throw GaudiException( mess.str(), "DeRichSystem::level1HardwareID", StatusCode::FAILURE );
  }
  // Found, so return Level1 board number
  return ( *id ).second;
}

//=========================================================================
// level1InputNum
// Obtain the Level1 input number for a given RichSmartID
//=========================================================================
const Rich::DAQ::HPD::Level1Input DeRichSystem::level1InputNum( const LHCb::RichSmartID& smartID ) const {
  // See if this RichSmartID is known
  const auto id = m_smartid2L1In.find( smartID.pdID() );
  if ( UNLIKELY( m_smartid2L1In.end() == id ) ) {
    std::ostringstream mess;
    mess << "Unknown PD RichSmartID " << smartID.pdID();
    throw GaudiException( mess.str(), "DeRichSystem::level1InputNum", StatusCode::FAILURE );
  }

  // Found, so return Level 1 input number
  return ( *id ).second;
}

//=========================================================================
// level0ID
//=========================================================================
const Rich::DAQ::Level0ID DeRichSystem::level0ID( const Rich::DAQ::PDHardwareID& hardID ) const {
  // See if this hardware ID is known
  const auto id = m_hardid2L0.find( hardID );
  if ( UNLIKELY( m_hardid2L0.end() == id ) ) {
    throw GaudiException( "Unknown PD hardware ID" + (std::string)hardID, "DeRichSystem::level0ID",
                          StatusCode::FAILURE );
  }

  // Found, so return Level 0 ID
  return ( *id ).second;
}

//=========================================================================
//  level1HardwareID
//=========================================================================
const Rich::DAQ::Level1HardwareID DeRichSystem::level1HardwareID( const Rich::DAQ::PDHardwareID& hardID ) const {
  // See if this hardware ID is known
  const auto id = m_hardid2L1.find( hardID );
  if ( UNLIKELY( m_hardid2L1.end() == id ) ) {
    throw GaudiException( "Unknown PD hardware ID" + (std::string)hardID, "DeRichSystem::level1HardwareID",
                          StatusCode::FAILURE );
  }

  // Found, so return Level 1 board number
  return ( *id ).second;
}

//=========================================================================
//  level1InputNum
/// Obtain the Level1 input number for a given PD hardware ID
//=========================================================================
const Rich::DAQ::HPD::Level1Input DeRichSystem::level1InputNum( const Rich::DAQ::PDHardwareID& hardID ) const {
  // See if this hardware ID is known
  const auto id = m_hardid2L1In.find( hardID );
  if ( UNLIKELY( m_hardid2L1In.end() == id ) ) {
    throw GaudiException( "Unknown PD hardware ID" + (std::string)hardID, "DeRichSystem::level1InputNum",
                          StatusCode::FAILURE );
  }

  // Found, so return Level 1 board input xnumber
  return ( *id ).second;
}

//=========================================================================
//  richSmartID from copy number
//=========================================================================
const LHCb::RichSmartID DeRichSystem::richSmartID( const Rich::DAQ::PDCopyNumber& copyNumber ) const {
  // See if this Level0 hardware ID is known
  const auto id = m_copyNumber2smartid.find( copyNumber );
  if ( UNLIKELY( m_copyNumber2smartid.end() == id ) ) {
    throw GaudiException( "Unknown PD Copy Number " + (std::string)copyNumber, "DeRichSystem::richSmartID",
                          StatusCode::FAILURE );
  }

  // Found, so return RichSmartID
  return ( *id ).second;
}

//=========================================================================
// Obtain the Copy Number number for a given RichSmartID
//=========================================================================
const Rich::DAQ::PDCopyNumber DeRichSystem::copyNumber( const LHCb::RichSmartID& smartID ) const {
  // See if this RichSmartID is known
  const auto id = m_smartid2copyNumber.find( smartID.pdID() );
  if ( UNLIKELY( m_smartid2copyNumber.end() == id ) ) {
    std::ostringstream mess;
    mess << "Unknown PD RichSmartID " << smartID.pdID();
    throw GaudiException( mess.str(), "DeRichSystem::copyNumber", StatusCode::FAILURE );
  }

  // Found, so return copy number
  return ( *id ).second;
}

//=========================================================================
//  l1PDSmartIDs
//=========================================================================
const LHCb::RichSmartID::Vector& DeRichSystem::l1PDSmartIDs( const Rich::DAQ::Level1HardwareID& l1ID ) const {
  // See if this L1 ID is known
  const auto id = m_l12smartids.find( l1ID );
  if ( UNLIKELY( m_l12smartids.end() == id ) ) {
    throw GaudiException( "Unknown RICH Level1 board ID " + (std::string)l1ID, "DeRichSystem::l1PDSmartIDs",
                          StatusCode::FAILURE );
  }

  // Found, so return list
  return ( *id ).second;
}

//=========================================================================
//  l1PDHardIDs
//=========================================================================
const Rich::DAQ::PDHardwareIDs& DeRichSystem::l1PDHardIDs( const Rich::DAQ::Level1HardwareID& l1ID ) const {
  // See if this L1 ID is known
  const auto id = m_l12hardids.find( l1ID );
  if ( UNLIKELY( m_l12hardids.end() == id ) ) {
    throw GaudiException( "Unknown RICH Level1 board ID " + (std::string)l1ID, "DeRichSystem::l1PDHardIDs",
                          StatusCode::FAILURE );
  }

  // Found, so return list
  return ( *id ).second;
}

//=========================================================================
// richDetector
//=========================================================================
Rich::DetectorType DeRichSystem::richDetector( const Rich::DAQ::Level1HardwareID& l1ID ) const {
  // See if this L1 ID is known
  const auto rich = m_l1ToRich.find( l1ID );
  if ( UNLIKELY( m_l1ToRich.end() == rich ) ) {
    throw GaudiException( "Unknown RICH Level1 board ID " + (std::string)l1ID, "DeRichSystem::richDetector()",
                          StatusCode::FAILURE );
  }

  // Found, so return RICH
  return ( *rich ).second;
}

//=========================================================================
// Obtain the Level1 hardware ID number for a Level1 logical ID
//=========================================================================
const Rich::DAQ::Level1HardwareID DeRichSystem::level1HardwareID( const Rich::DetectorType          rich,
                                                                  const Rich::DAQ::Level1LogicalID& logID ) const {
  const auto iID = m_l1LogToHard[rich].find( logID );
  if ( UNLIKELY( m_l1LogToHard[rich].end() == iID ) ) {
    throw GaudiException( "Unknown L1 logical ID " + (std::string)logID, "DeRichSystem::level1HardwareID",
                          StatusCode::FAILURE );
  }
  return ( *iID ).second;
}

//=========================================================================
// Obtain the Level1 hardware ID number for a Level0 lID
//=========================================================================
const Rich::DAQ::Level1HardwareID DeRichSystem::level1HardwareID( const Rich::DAQ::Level0ID& l0ID ) const {
  const auto iID = m_l0ToL1.find( l0ID );
  if ( UNLIKELY( m_l0ToL1.end() == iID ) ) {
    throw GaudiException( "Unknown L0 ID " + (std::string)l0ID, "DeRichSystem::level1HardwareID", StatusCode::FAILURE );
  }
  return ( *iID ).second;
}

//=========================================================================
// Obtain the Level1 logical ID number for a Level1 hardware ID
//=========================================================================
const Rich::DAQ::Level1LogicalID DeRichSystem::level1LogicalID( const Rich::DAQ::Level1HardwareID& hardID ) const {
  const auto iID = m_l1HardToLog.find( hardID );
  if ( UNLIKELY( m_l1HardToLog.end() == iID ) ) {
    throw GaudiException( "Unknown L1 hardware ID " + (std::string)hardID, "DeRichSystem::level1LogicalID",
                          StatusCode::FAILURE );
  }
  return ( *iID ).second;
}

//=========================================================================
// Access the PD hardware ID for the given L1 hardwareID and input number
//=========================================================================
const Rich::DAQ::PDHardwareID DeRichSystem::pdHardwareID( const Rich::DAQ::Level1HardwareID& L1HardID,
                                                          const Rich::DAQ::HPD::Level1Input& L1Input ) const {
  const L1HardIDAndInput key( L1HardID, L1Input );
  const auto             iID = m_L1HardIDAndInputToPDHardID.find( key );
  if ( UNLIKELY( m_L1HardIDAndInputToPDHardID.end() == iID ) ) {
    throw GaudiException( "Unknown L1 hardware ID " + (std::string)L1HardID + " and L1 input " + (std::string)L1Input +
                              " pair",
                          "DeRichSystem::hpdHardwareID", StatusCode::FAILURE );
  }
  return ( *iID ).second;
}

//=========================================================================
// L1 Logical ID to Copy Number
//=========================================================================
const Rich::DAQ::Level1CopyNumber DeRichSystem::copyNumber( const Rich::DAQ::Level1HardwareID& hardID ) const {
  const auto iCN = m_l1H2CopyN.find( hardID );
  if ( UNLIKELY( m_l1H2CopyN.end() == iCN ) ) {
    throw GaudiException( "Unknown L1 Hardware ID " + (std::string)hardID, "DeRichSystem::copyNumber",
                          StatusCode::FAILURE );
  }
  return ( *iCN ).second;
}

//=========================================================================
// getDePDLocation
//=========================================================================
std::string DeRichSystem::getDePDLocation( const LHCb::RichSmartID& smartID ) const {
  std::string loc;

  if ( smartID.idType() == LHCb::RichSmartID::MaPMTID ) {
    if ( deRich( smartID.rich() )->exists( "PMTPanelDetElemLocations" ) ) {
      const auto& panelLoc = deRich( smartID.rich() )->paramVect<std::string>( "PMTPanelDetElemLocations" );
      loc                  = panelLoc[smartID.panel()];
    } else {
      if ( smartID.rich() == Rich::Rich1 ) {
        loc = ( smartID.panel() == Rich::top ? DeRichLocations::Rich1Panel0 : DeRichLocations::Rich1Panel1 );
      } else {
        loc = ( smartID.panel() == Rich::left ? DeRichLocations::Rich2Panel0 : DeRichLocations::Rich2Panel1 );
      }
    }

    const auto aM     = smartID.pdCol();
    const auto aP     = smartID.pdNumInCol();
    const auto ast_aM = std::to_string( aM );
    const auto ast_aP = std::to_string( aP );
    return ( loc + "/MAPMT_MODULE:" + ast_aM + "/MAPMT:" + ast_aP );
  } else // HPDs
  {

    if ( deRich( smartID.rich() )->exists( "HPDPanelDetElemLocations" ) ) {
      const auto& panelLoc = deRich( smartID.rich() )->paramVect<std::string>( "HPDPanelDetElemLocations" );
      loc                  = panelLoc[smartID.panel()];
    } else {
      if ( smartID.rich() == Rich::Rich1 ) {
        loc = ( smartID.panel() == Rich::top ? DeRichLocations::Rich1Panel0 : DeRichLocations::Rich1Panel1 );
      } else {
        loc = ( smartID.panel() == Rich::left ? DeRichLocations::Rich2Panel0 : DeRichLocations::Rich2Panel1 );
      }
    }

    const auto cNumber = copyNumber( smartID );
    return loc + "/HPD:" + std::string( cNumber );
  }
}

//=========================================================================
//  getDeRichLocations
//=========================================================================
std::vector<std::string> DeRichSystem::getDeRichLocations() {
  // find the Rich detectos
  SmartDataPtr<DetectorElement> afterMag( dataSvc(), "/dd/Structure/LHCb/AfterMagnetRegion" );
  if ( !afterMag ) {
    throw GaudiException( "Could not load AfterMagnetRegion ", "DeRichBase::deRichSys()", StatusCode::FAILURE );
  }

  std::vector<std::string> deRichLocs;
  if ( afterMag->exists( "RichDetectorLocations" ) ) {
    deRichLocs = afterMag->paramVect<std::string>( "RichDetectorLocations" );
  } else {
    deRichLocs.push_back( DeRichLocations::Rich1 );
    deRichLocs.push_back( DeRichLocations::Rich2 );
  }

  return deRichLocs;
}

//===========================================================================
