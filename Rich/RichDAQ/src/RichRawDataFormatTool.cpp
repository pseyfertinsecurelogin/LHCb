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

//-----------------------------------------------------------------------------
/** @file RichRawDataFormatTool.cpp
 *
 *  Implementation file for class : Rich::RawDataFormatTool
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date 2004-12-18
 */
//-----------------------------------------------------------------------------

// local
#include "RichRawDataFormatTool.h"

// Rich DAQ software
using namespace Rich::DAQ;

//-----------------------------------------------------------------------------

// private namespace
namespace {
  /// Default 'fake' HPD RichSmartID
  static const LHCb::RichSmartID s_fakeHPDID( Rich::Rich1, Rich::top, 0, 0 );
} // namespace

// Standard constructor
RawDataFormatTool::RawDataFormatTool( const std::string& type, const std::string& name, const IInterface* parent )
    : Rich::ToolBase( type, name, parent ), m_richIsActive( Rich::NRiches, true ) {
  // interface
  declareInterface<IRawDataFormatTool>( this );

  // job opts
  declareProperty( "ZeroSuppressHitCut", m_zeroSuppresCut = 96 );
  declareProperty( "PrintSummary", m_summary = true );
  declareProperty( "MaxHPDOccupancy", m_maxHPDOc = 999999 );
  declareProperty( "DumpRawBanks", m_dumpBanks = false );
  declareProperty( "UseZeroSuppression", m_zeroSupp = true );
  declareProperty( "UseExtendedFormat", m_extendedFormat = false );
  declareProperty( "DecodeUsingODIN", m_decodeUseOdin = false );
  declareProperty( "CheckDataIntegrity", m_checkDataIntegrity = true );
  declareProperty( "CheckODINEventIDs", m_checkODINEventsIDs = false );
  declareProperty( "CheckRICHEventIDs", m_checkRICHEventsIDs = true );
  declareProperty( "CheckBXIDs", m_checkBxIDs = true );
  declareProperty( "CheckHPDL1IDs", m_hpdL1check = false );
  declareProperty( "UseFakeHPDID", m_useFakeHPDID = false );
  declareProperty( "VerboseErrors", m_verboseErrors = false );
  declareProperty( "ActiveRICHes", m_richIsActive );
  declareProperty( "PurgeHPDsFailIntegrityTest", m_purgeHPDsFailIntegrity = true );
  declareProperty( "HotPixelsToMask", m_hotChannels );
  declareProperty( "RawEventLocations",
                   m_rawEventLocations = {LHCb::RawEventLocation::Rich, LHCb::RawEventLocation::Default},
                   "List of possible locations of the RawEvent object in the"
                   " transient store. By default it is LHCb::RawEventLocation::Rich,"
                   " LHCb::RawEventLocation::Default." );
}

StatusCode RawDataFormatTool::initialize() {
  // Initialise base class
  const StatusCode sc = Rich::ToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  // RichDet
  m_richSys = getDet<DeRichSystem>( DeRichLocations::RichSystem );

  // report inactive RICHes
  if ( m_richIsActive.size() != Rich::NRiches ) { return Error( "Incorrectly configured active RICH options" ); }
  if ( !m_richIsActive[Rich::Rich1] ) { Warning( "Decoding for RICH1 disabled", StatusCode::SUCCESS ).ignore(); }
  if ( !m_richIsActive[Rich::Rich2] ) { Warning( "Decoding for RICH2 disabled", StatusCode::SUCCESS ).ignore(); }

  // if suppression is less than max possible number of (ALICE) hits, print a message.
  if ( m_maxHPDOc < HPD::BitsPerDataWord * HPD::MaxDataSizeALICE )
    info() << "Will suppress PDs with more than " << m_maxHPDOc << " digits" << endmsg;

  // messages if optional features are enabled
  if ( m_extendedFormat ) info() << "Will encode RawEvent using extended HPD format" << endmsg;
  if ( m_decodeUseOdin ) info() << "ODIN integrity checks are enabled" << endmsg;
  if ( m_hpdL1check ) info() << "HPD L1 ID checks are enabled" << endmsg;

  // warnings if checks are disabled
  if ( !m_checkDataIntegrity ) Warning( "HPD Data integrity checks are disabled", StatusCode::SUCCESS ).ignore();
  if ( m_checkODINEventsIDs ) Warning( "Ingress/ODIN Event ID checks are enabled", StatusCode::SUCCESS ).ignore();
  if ( !m_checkRICHEventsIDs ) Warning( "Internal RICH Event ID checks are disabled", StatusCode::SUCCESS ).ignore();
  if ( !m_checkBxIDs ) Warning( "Header BX ID checks are disabled", StatusCode::SUCCESS ).ignore();

  // Setup incident services
  incSvc()->addListener( this, IncidentType::BeginEvent );
  incSvc()->addListener( this, IncidentType::EndEvent );

  // Do we have pixels to suppress ?
  if ( !m_hotChannels.empty() ) {
    info() << "Hot HPD pixel list to suppress :-" << endmsg;
    m_pixelsToSuppress = true;
    for ( const auto& C : m_hotChannels ) {
      const LHCb::RichSmartID channel( C );
      m_hotPixels[channel.pdID()].insert( channel );
      info() << " " << channel << " (" << channel.key() << ")" << endmsg;
    }
  }

  // Initialise the RawEvent locations
  const bool usingDefaultLocation =
      ( 2 == m_rawEventLocations.size() && m_rawEventLocations[0] == LHCb::RawEventLocation::Rich &&
        m_rawEventLocations[1] == LHCb::RawEventLocation::Default );

  if ( !usingDefaultLocation ) {
    info() << "Using " << m_rawEventLocations << " as search path for the RawEvent object" << endmsg;
  }

  return sc;
}

// Method that handles various Gaudi "software events"
void RawDataFormatTool::handle( const Incident& incident ) {
  // Update prior to start of new event.
  if ( IncidentType::BeginEvent == incident.type() ) {
    InitEvent();
  }
  // End of event
  else if ( IncidentType::EndEvent == incident.type() ) {
    FinishEvent();
  }
}

StatusCode RawDataFormatTool::finalize() {

  // Print out L1 information
  if ( m_summary ) {
    for ( const auto& sum : m_l1decodeSummary ) {
      printL1Stats( sum.second, "RICH Level 1 : Decoding Summary " + sum.first );
    }
    printL1Stats( m_l1encodeSummary, "RICH Level 1 : Encoding Summary" );
  }

  // base class finalize
  return Rich::ToolBase::finalize();
}

void RawDataFormatTool::printL1Stats( const L1TypeCount& count, const std::string& title ) const {

  // See if any summary info has been collected
  if ( !count.empty() ) {

    const std::string& LINES = "==================================================================="
                               "=====================================================";
    const std::string& lines = "-------------------------------------------------------------------"
                               "-----------------------------------------------------";

    // Statistical calculators
    const StatDivFunctor occ1( "%7.2f +-%5.2f" ), occ2( "%8.2f +-%5.2f" );

    // Printout
    info() << LINES << endmsg << "                             " << title << " : " << m_evtCount << " events" << endmsg;

    Rich::Map<Rich::DetectorType, unsigned long long> totWordSize, totBanks, totHits, nEvFills, s;
    Rich::DetectorType                                lastrich = Rich::InvalidDetector;
    for ( const auto& L1C : count ) {
      const Level1HardwareID L1HardID = L1C.first.l1HardwareID;
      Level1LogicalID        L1LogID( 0 );
      try {
        L1LogID = m_richSys->level1LogicalID( L1HardID );
      } catch ( ... ) { Warning( "Unknown L1 Hardware ID " + (std::string)L1HardID ).ignore(); }
      const BankVersion  version = L1C.first.bankVersion;
      Rich::DetectorType rich;
      try {
        rich = m_richSys->richDetector( L1HardID );
      } catch ( ... ) {
        Warning( "Unknown L1 Hardware ID " + (std::string)L1HardID ).ignore();
        rich = Rich::InvalidDetector;
      }
      const unsigned long long nBanks = L1C.second.nHPDs;
      totBanks[rich] += nBanks;
      const unsigned long long words = L1C.second.nWords;
      totWordSize[rich] += words;
      const unsigned long long hits = L1C.second.nHits;
      totHits[rich] += hits;
      const unsigned long long nFills = L1C.second.nFills;
      nEvFills[rich] += nFills;
      ++s[rich];

      if ( rich != lastrich ) {
        lastrich = rich;
        info() << lines << endmsg;
      }

      info() << " " << rich << " L1ID(hard/log) " << boost::format( "%2i/%2i" ) % L1HardID.data() % L1LogID.data();
      info() << " V" << boost::format( "%3i" ) % version;
      info() << " | L1 size =" << occ1( nBanks, nFills ) << " pds :" << occ2( words, nFills )
             << " words :" << occ2( hits, nFills ) << " hits / event" << endmsg;

    } // loop over L1s

    info() << lines << endmsg;
    if ( totBanks[Rich::Rich1] > 0 ) {
      const double evtCount1 =
          ( s[Rich::Rich1] > 0 ? (double)( nEvFills[Rich::Rich1] ) / (double)( s[Rich::Rich1] ) : 0 );
      info() << " Rich1 Average                   | L1 size =" << occ1( totBanks[Rich::Rich1], evtCount1 )
             << " pds :" << occ2( totWordSize[Rich::Rich1], evtCount1 )
             << " words :" << occ2( totHits[Rich::Rich1], evtCount1 ) << " hits / event" << endmsg;
    }
    if ( totBanks[Rich::Rich2] > 0 ) {
      const double evtCount2 =
          ( s[Rich::Rich2] > 0 ? (double)( nEvFills[Rich::Rich2] ) / (double)( s[Rich::Rich2] ) : 0 );
      info() << " Rich2 Average                   | L1 size =" << occ1( totBanks[Rich::Rich2], evtCount2 )
             << " pds :" << occ2( totWordSize[Rich::Rich2], evtCount2 )
             << " words :" << occ2( totHits[Rich::Rich2], evtCount2 ) << " hits / event" << endmsg;
    }

    info() << LINES << endmsg;

  } // end stats available
}

// Create data bank of correct version
// This function knows which bank objects to create for each version number
const PDDataBank* RawDataFormatTool::createDataBank( const LHCb::RichSmartID::Vector& smartIDs,
                                                     const BankVersion version, const LHCb::ODIN* odin ) const {

  // pointer to HPD data block
  PDDataBank* dataBank = nullptr;

  // Check bank is not empty
  if ( smartIDs.empty() ) {
    Warning( "RichSmartID vector is empty -> No data bank" ).ignore();
    return dataBank;
  }

  // Level 0 ID
  const Level0ID l0ID = m_richSys->level0ID( smartIDs.front().pdID() );

  if ( LHCb5 == version ) {
    // NB : Does not yet support creation of ALICE mode data blocks...  To Be Done.

    // First try the ZS format
    RichDAQ_LHCb5::ZeroSuppLHCb* zsData = new RichDAQ_LHCb5::ZeroSuppLHCb( l0ID, smartIDs, m_extendedFormat, odin );

    // If too big, use non ZS instead
    if ( !m_zeroSupp || zsData->tooBig() ) {
      delete zsData;
      dataBank = (PDDataBank*)new RichDAQ_LHCb5::NonZeroSuppLHCb( l0ID, smartIDs, m_extendedFormat, odin );
    } else {
      dataBank = (PDDataBank*)zsData;
    }
  } else if ( LHCb4 == version ) {
    // NB : Does not support creation of ALICE mode data blocks.

    // First try the ZS format
    RichDAQ_LHCb4::ZeroSuppLHCb* zsData = new RichDAQ_LHCb4::ZeroSuppLHCb( l0ID, smartIDs );

    // If too big, use non ZS instead
    if ( !m_zeroSupp || zsData->tooBig() ) {
      delete zsData;
      dataBank = (PDDataBank*)new RichDAQ_LHCb4::NonZeroSuppLHCb( l0ID, smartIDs );
    } else {
      dataBank = (PDDataBank*)zsData;
    }
  } else if ( LHCb3 == version ) {
    // NB : Does not support creation of ALICE mode data blocks.

    // First try the ZS format
    RichDAQ_LHCb3::ZeroSuppLHCb* zsData = new RichDAQ_LHCb3::ZeroSuppLHCb( l0ID, smartIDs );

    // If too big, use non ZS instead
    if ( !m_zeroSupp || zsData->tooBig() ) {
      delete zsData;
      dataBank = (PDDataBank*)new RichDAQ_LHCb3::NonZeroSuppLHCb( l0ID, smartIDs );
    } else {
      dataBank = (PDDataBank*)zsData;
    }
  } else if ( LHCb2 == version ) {
    // First try the ZS format
    RichDAQ_LHCb2::ZeroSuppLHCb* zsData = new RichDAQ_LHCb2::ZeroSuppLHCb( l0ID, smartIDs );

    // If too big, use non ZS instead
    if ( !m_zeroSupp || zsData->tooBig() ) {
      delete zsData;
      dataBank = (PDDataBank*)new RichDAQ_LHCb2::NonZeroSuppLHCb( l0ID, smartIDs );
    } else {
      dataBank = (PDDataBank*)zsData;
    }
  } else if ( LHCb1 == version ) {

    // Decide to zero suppress or not depending on number of hits
    if ( m_zeroSupp && smartIDs.size() < m_zeroSuppresCut ) {
      dataBank = (PDDataBank*)new RichDAQ_LHCb1::ZeroSuppLHCb( l0ID, smartIDs );
    } else {
      dataBank = (PDDataBank*)new RichDAQ_LHCb1::NonZeroSuppLHCb( l0ID, smartIDs );
    }
  } else if ( FlatList == version ) {

    dataBank = (PDDataBank*)new RichDAQ_FlatList::Data( l0ID, smartIDs );
  } else {
    std::ostringstream message;
    message << "Unknown RICH Raw Buffer version " << version << " -> No data bank";
    Warning( message.str() );
    return nullptr;
  }

  if ( msgLevel( MSG::VERBOSE ) ) {
    // Print out SmartIDs to encode
    verbose() << endmsg << "   Creating encoding HPD " << smartIDs.front().pdID() << " bank from " << smartIDs.size()
              << " RichSmartIDs :-" << endmsg;
    for ( const auto& ID : smartIDs ) { verbose() << "    " << ID << endmsg; }
    // Printout this bank
    verbose() << *dataBank << endmsg;
  }

  return dataBank;
}

const PDDataBank* RawDataFormatTool::createDataBank( const LongType* dataStart, const unsigned int dataSize,
                                                     const BankVersion version ) const {
  PDDataBank* dataBank = nullptr;

  if ( LHCb5 == version ) {

    // Quick check of header for HPD data type
    const bool isZS    = RichDAQ_LHCb5::Header::zeroSuppressed( dataStart );
    const bool isAlice = RichDAQ_LHCb5::Header::aliceMode( dataStart );

    // Decide to zero suppress or not depending on number of hits
    if ( isZS ) {
      if ( UNLIKELY( isAlice ) ) {
        static RichDAQ_LHCb5::ZeroSuppAlice b;
        b.reset( dataStart );
        dataBank = &b;
      } else {
        static RichDAQ_LHCb5::ZeroSuppLHCb b;
        b.reset( dataStart );
        dataBank = &b;
      }
    } else {
      if ( UNLIKELY( isAlice ) ) {
        static RichDAQ_LHCb5::NonZeroSuppAlice b;
        b.reset( dataStart );
        dataBank = &b;
      } else {
        static RichDAQ_LHCb5::NonZeroSuppLHCb b;
        b.reset( dataStart );
        dataBank = &b;
      }
    }
  } else if ( LHCb4 == version ) {

    // Quick check of header for HPD data type
    const bool isZS    = RichDAQ_LHCb4::Header::zeroSuppressed( dataStart );
    const bool isAlice = RichDAQ_LHCb4::Header::aliceMode( dataStart );

    // Decide to zero suppress or not depending on number of hits
    if ( isZS ) {
      if ( UNLIKELY( isAlice ) ) {
        Warning( "LHCb4 data format does not support ZS Alice mode data" ).ignore();
      } else {
        static RichDAQ_LHCb4::ZeroSuppLHCb b;
        b.reset( dataStart );
        dataBank = &b;
      }
    } else {
      if ( UNLIKELY( isAlice ) ) {
        static RichDAQ_LHCb4::NonZeroSuppAlice b;
        b.reset( dataStart );
        dataBank = &b;
      } else {
        static RichDAQ_LHCb4::NonZeroSuppLHCb b;
        b.reset( dataStart );
        dataBank = &b;
      }
    }
  } else if ( LHCb3 == version ) {

    // Quick check of header for HPD data type
    const bool isZS    = RichDAQ_LHCb3::Header::zeroSuppressed( dataStart );
    const bool isAlice = RichDAQ_LHCb3::Header::aliceMode( dataStart );

    // Decide to zero suppress or not depending on number of hits
    if ( isZS ) {
      if ( UNLIKELY( isAlice ) ) {
        Warning( "LHCb3 data format does not support ZS Alice mode data" ).ignore();
      } else {
        static RichDAQ_LHCb3::ZeroSuppLHCb b;
        b.reset( dataStart );
        dataBank = &b;
      }
    } else {
      if ( UNLIKELY( isAlice ) ) {
        static RichDAQ_LHCb3::NonZeroSuppAlice b;
        b.reset( dataStart );
        dataBank = &b;
      } else {
        static RichDAQ_LHCb3::NonZeroSuppLHCb b;
        b.reset( dataStart );
        dataBank = &b;
      }
    }
  } else if ( LHCb2 == version ) {

    // Quick check of header for HPD data type
    const bool isZS = RichDAQ_LHCb2::Header::zeroSuppressed( dataStart );

    // Decide to zero suppress or not depending on number of hits
    if ( isZS ) {
      static RichDAQ_LHCb2::ZeroSuppLHCb b;
      b.reset( dataStart, dataSize );
      dataBank = &b;
    } else {
      static RichDAQ_LHCb2::NonZeroSuppLHCb b;
      b.reset( dataStart );
      dataBank = &b;
    }
  } else if ( LHCb1 == version ) {

    // Quick check of header for HPD data type
    const bool isZS = RichDAQ_LHCb1::Header::zeroSuppressed( dataStart );

    // Decide to zero suppress or not depending on number of hits
    if ( isZS ) {
      static RichDAQ_LHCb1::ZeroSuppLHCb b;
      b.reset( dataStart, dataSize );
      dataBank = &b;
    } else {
      static RichDAQ_LHCb1::NonZeroSuppLHCb b;
      b.reset( dataStart );
      dataBank = &b;
    }
  } else if ( FlatList == version ) {

    static RichDAQ_FlatList::Data b;
    b.reset( dataStart );
    dataBank = &b;
  } else {
    std::ostringstream message;
    message << "Unknown RICH Raw Buffer version " << version << " -> No data bank";
    Exception( message.str() );
  }

  // Printout this bank
  if ( dataBank && msgLevel( MSG::VERBOSE ) ) {
    verbose() << endmsg << "Created HPD Data Bank for Decoding :-" << endmsg;
    verbose() << *dataBank << endmsg;
  }

  return dataBank;
}

void RawDataFormatTool::fillRawEvent( const LHCb::RichSmartID::Vector& smartIDs, const BankVersion version ) const {
  m_hasBeenCalled = true;

  // new rich data map
  L1Map L1Data = dummyMap();

  // Loop over digits and sort according to L1 and HPD
  for ( const auto& dig : smartIDs ) {

    // Get Level 1 board number and input
    const auto L1ID    = m_richSys->level1HardwareID( dig );
    const auto L1Input = m_richSys->level1InputNum( dig );

    // Get reference to correct data group
    auto& ingress = L1Data[L1ID];
    auto& hpds    = ingress[L1Input.ingressID()];
    auto& pdInfo  = ( hpds.pdData() )[L1Input];

    _ri_debug << dig << " mapped to L1ID " << L1ID << " L1Input " << L1Input << endmsg;

    // Finally, insert this pixel into correct place
    pdInfo.smartIDs().push_back( dig );

    // Set the HPD information
    pdInfo.setPdID( dig.pdID() );
  }

  // Get some ODIN info
  _ri_verbo << "ODIN : " << *odin() << endmsg;

  // Loop over Level1 board and make a RAWBank for each
  for ( const auto& L1 : L1Data ) {
    _ri_debug << "Creating Bank for L1 board " << L1.first << endmsg;

    // Make a new data bank
    RAWBank dataBank;

    // local hit and HPD count
    ShortType nHits( 0 ), nHPDs( 0 );

    // Loop over ingresses for this L1 board
    L1IngressID ingress( 0 );
    for ( auto iIngress = L1.second.begin(); iIngress != L1.second.end(); ++iIngress, ++ingress ) {

      // If 2007 data format, add Ingress header word to the raw bank
      if ( version != LHCb0 && version != LHCb1 && version != LHCb2 && version != LHCb3 && version != LHCb4 &&
           version != MaPMT0 ) {
        // create ingress header word
        L1IngressHeader ingressWord;
        // set ingress number
        ingressWord.setIngressID( ingress );
        // set data from ODIN
        ingressWord.readFromODIN( odin() );
        // fill with HPDs with data
        for ( const auto& HPD : ( *iIngress ).second.pdData() ) {
          ingressWord.setHPDActive( HPD.first.l1InputWithinIngress() );
        }
        // fill header into raw data
        ingressWord.fillRAWBank( dataBank );
        _ri_debug << " Ingress " << ingress << " : " << ingressWord << endmsg;
      }

      _ri_debug << "  Found " << ( *iIngress ).second.pdData().size() << " HPD(s) with data" << endmsg;

      // Loop over each active HPD for this Level1 board ingress and fill RAWBank
      for ( const auto& HPD : ( *iIngress ).second.pdData() ) {
        _ri_debug << "   HPD " << HPD.second.pdID() << " L1input=" << HPD.first << " Found "
                  << HPD.second.smartIDs().size() << " hits" << endmsg;
        if ( version != MaPMT0 ) {
          // Get raw data bank for this HPD, and fill into RAWBank
          std::unique_ptr<const PDDataBank> hpdData( createDataBank( HPD.second.smartIDs(), version, odin() ) );
          if ( hpdData.get() ) {
            hpdData->fillRAWBank( dataBank );
            nHits += HPD.second.smartIDs().size();
            ++nHPDs;
          } else {
            Warning( "No RICH Data Bank Created" ).ignore();
          }
        } else // Special MaPMT0 version
        {
          for ( const auto& dig : HPD.second.smartIDs() ) { dataBank.push_back( dig.key() ); }
        }

      } // end photon detector loop

    } // ingress loop

    // Add this bank to the Raw buffer
    m_currentTAE = "";
    rawEvent()->addBank( L1.first.data(), LHCb::RawBank::Rich, version, dataBank );

    if ( m_summary ) {
      // Count the number of banks and size
      L1CountAndSize& cands = m_l1encodeSummary[L1IDandV( version, L1.first )];
      // Increment bank size
      cands.nWords += dataBank.size(); // L1 headers + data words
      // Increment hit occupancy
      cands.nHits += nHits;
      // Count number of HPD banks
      cands.nHPDs += nHPDs;
      // Count fills
      ++cands.nFills;
    }

    _ri_debug << "Encoded " << boost::format( "%2i" ) % L1.second.size();
    _ri_debug << " ingresses for Level1 Bank " << boost::format( "%2i" ) % L1.first.data();
    _ri_debug << " : Size " << boost::format( "%4i" ) % dataBank.size() << " words : Version " << version << endmsg;

  } // Level1 loop
}

void RawDataFormatTool::decodeToSmartIDs( const LHCb::RawBank& bank, L1Map& decodedData ) const {

  // Check magic code for general data couurption
  if ( LHCb::RawBank::MagicPattern != bank.magic() ) {
    std::ostringstream mess;
    mess << "Magic Pattern mis-match : Expected " << LHCb::RawBank::MagicPattern << " Obtained " << bank.magic();
    Error( mess.str() ).ignore();
  } else { // All OK so decode

    // Check this is a RICH bank
    if ( bank.type() != LHCb::RawBank::Rich ) {
      std::ostringstream message;
      message << "BankType is not RICH : type = " << bank.type();
      Exception( message.str() );
    }

    // Get L1 ID
    const Level1HardwareID L1ID( bank.sourceID() );

    // Get bank version
    const auto version = bankVersion( bank );

    // Is the RICH this L1 ID is for active ?
    const auto rich = m_richSys->richDetector( L1ID );
    if ( rich == Rich::InvalidDetector ) {
      std::ostringstream mess;
      mess << "L1 bank " << L1ID << " has an invalid RICH detector mapping -> Bank skipped";
      Warning( mess.str() ).ignore();
    } else if ( m_richIsActive[rich] ) {

      // if configured, dump raw event before decoding
      if ( msgLevel( MSG::VERBOSE ) ) {
        dumpRawBank( bank, verbose() );
      } else if ( m_dumpBanks ) {
        dumpRawBank( bank, info() );
      }

      // Flag tool as having been used this event
      m_hasBeenCalled = true;

      // Count decoded L1IDs
      ++m_l1IdsDecoded[L1ID];

      // Now, delegate the work to a version of the decoding appropriate to the version
      if ( version == LHCb5 || version == FlatList ) {
        decodeToSmartIDs_2007( bank, decodedData );
      } else if ( version == LHCb3 || version == LHCb4 ) // RICH 2006 Testbeam
      {
        decodeToSmartIDs_2006TB( bank, decodedData );
      } else if ( version == LHCb0 || version == LHCb1 || version == LHCb2 ) // DC04 or DC06
      {
        decodeToSmartIDs_DC0406( bank, decodedData );
      } else if ( MaPMT0 == version ) {
        decodeToSmartIDs_MaPMT0( bank, decodedData );
      } else // Some problem ...
      {
        std::ostringstream mess;
        mess << "Unknown RICH L1 version number " << version;
        Exception( mess.str() );
      }

    } // active RICH

  } // magic OK
}

void RawDataFormatTool::decodeToSmartIDs_MaPMT0( const LHCb::RawBank& bank, Rich::DAQ::L1Map& decodedData ) const {
  // Get L1 ID
  const Level1HardwareID L1ID( bank.sourceID() );
  _ri_debug << "FlatList Decoding L1 bank " << L1ID << endmsg;

  // Data bank size in 32 bit words
  const int bankSize = bank.size() / 4;

  // Get bank version
  const auto version = bankVersion( bank );

  // various counts
  DetectorArray<unsigned int>                decodedHits = {{0, 0}};
  DetectorArray<std::set<LHCb::RichSmartID>> pdSet;

  // If we have some words to process, start the decoding
  if ( bankSize > 0 ) {

    // Get Ingress map to decode into for this L1 board
    IngressMap& ingressMap = decodedData[L1ID];

    // Loop over bank, Fill data into RichSmartIDs
    int lineC( 0 );
    while ( lineC < bankSize ) {
      // Read the smartID direct from the banks
      const LHCb::RichSmartID id( LHCb::RichSmartID32( bank.data()[lineC++] ) );
      if ( !id.isValid() ) {
        Warning( "Invalid RichSmartID read from FlatList data format" ).ignore();
      } else {
        _ri_debug << " -> " << id << endmsg;

        // Get the L1 input from the DB
        const auto l1Input = m_richSys->level1InputNum( id );

        // Do we have an entry for this Ingress ID ?
        if ( ingressMap.find( l1Input.ingressID() ) == ingressMap.end() ) {
          auto&           info = ingressMap[l1Input.ingressID()];
          L1IngressHeader iHeader;
          iHeader.setIngressID( l1Input.ingressID() );
          info.setIngressHeader( iHeader );
        }
        auto& ingressInfo = ingressMap[l1Input.ingressID()];
        auto& hpdMap      = ingressInfo.pdData();

        // Does this PD have an entry
        if ( hpdMap.find( l1Input ) == hpdMap.end() ) {
          hpdMap[l1Input] = PDInfo();
          // Set the PD ID
          hpdMap[l1Input].setPdID( id.pdID() );
          // set the header
          PDInfo::Header header;
          // CRJ - Comment out until decide what to do about maPMT Level0 IDs ...
          //           const Level0ID l0id = m_richSys->level0ID(id);
          //           if ( ! header.setL0ID( l0id ) )
          //           {
          //             std::ostringstream mess;
          //             mess << "Failed to set L0ID " << l0id;
          //             Warning( mess.str() ).ignore();
          //           }
          hpdMap[l1Input].setHeader( header );
        }
        PDInfo& pdInfo = hpdMap[l1Input];

        // add the hit to the list
        pdInfo.smartIDs().push_back( id );

        // count the hits and hpds
        const auto rich = id.rich();
        ++decodedHits[rich];
        pdSet[rich].insert( id.pdID() );
      }
    }
  }

  // Add to the total number of decoded hits
  decodedData.addToTotalHits( decodedHits );
  for ( const auto rich : {Rich::Rich1, Rich::Rich2} ) { decodedData.addToActivePDs( rich, pdSet[rich].size() ); }
  if ( m_summary ) {
    // Count the number of banks and size
    auto& cands = ( m_l1decodeSummary[m_currentTAE] )[L1IDandV( version, L1ID )];
    // Increment bank size
    cands.nWords += bank.size() / 4; // 2 L1 headers + data words
    // Increment hit occupancy
    cands.nHits += decodedHits[Rich::Rich1] + decodedHits[Rich::Rich2];
    // Count number of HPD banks
    cands.nHPDs += pdSet[Rich::Rich1].size() + pdSet[Rich::Rich2].size();
    // Count fills
    ++cands.nFills;
  }
}

void RawDataFormatTool::decodeToSmartIDs_2007( const LHCb::RawBank& bank, L1Map& decodedData ) const {
  using namespace Rich::DAQ::HPD;

  // Get L1 ID
  const Level1HardwareID L1ID( bank.sourceID() );
  _ri_debug << "Decoding L1 bank " << L1ID << endmsg;

  // various counts
  DetectorArray<unsigned int> nHPDbanks{{0, 0}}, decodedHits{{0, 0}};

  // Data bank size in 32 bit words
  const int bankSize = bank.size() / 4;

  // Get bank version
  const auto version = bankVersion( bank );

  // Flag to indicate if a given L1 bank has been printed out in case of an error
  bool l1BankErrorDump = true;

  // If we have some words to process, start the decoding
  if ( bankSize > 0 ) {

    // Get Ingress map to decode into for this L1 board
    auto& ingressMap = decodedData[L1ID];

    // Loop over bank, find headers and produce a data bank for each
    // Fill data into RichSmartIDs
    int lineC( 0 );
    while ( lineC < bankSize ) {

      // First, the ingress header word (and increment to the next line)
      const L1IngressHeader ingressWord( bank.data()[lineC++] );
      _ri_debug << " Ingress " << ingressWord << endmsg;

      // Get data for this ingress
      auto& ingressInfo = ingressMap[ingressWord.ingressID()];

      // Set ingress header in decoded data map
      ingressInfo.setIngressHeader( ingressWord );

      // Compare Ingress header to the ODIN
      _ri_verbo << "ODIN : EventNumber=" << EventID( odin()->eventNumber() ) << " BunchID=" << BXID( odin()->bunchId() )
                << endmsg;
      const bool odinOK =
          ( !m_decodeUseOdin ? true
                             : ( !m_checkODINEventsIDs || ingressWord.eventID() == EventID( odin()->eventNumber() ) ) &&
                                   ( !m_checkBxIDs || ingressWord.bxID() == BXID( odin()->bunchId() ) ) );
      if ( !odinOK ) {
        std::ostringstream mess;
        mess << "ODIN Mismatch : L1ID " << L1ID << " : ODIN EvID=" << EventID( odin()->eventNumber() )
             << " BxID=" << BXID( odin()->bunchId() ) << " : L1IngressHeader " << ingressWord << " -> Data Suppressed";
        Error( mess.str() ).ignore();
      }

      // get list of active ingress inputs
      static L1IngressInputs inputs;
      ingressWord.activeHPDInputs( inputs );
      _ri_debug << "  Found " << inputs.size() << " PDs with data blocks : " << inputs << endmsg;

      // Check the Ingress supression flag
      if ( ingressWord.hpdsSuppressed() ) {
        std::ostringstream mess;
        mess << "L1 board " << L1ID << " : Ingress " << ingressWord << " is HARDWARE suppressed";
        Warning( mess.str(), StatusCode::SUCCESS, 1 ).ignore();
        // procStatus()->addAlgorithmStatus( name(), "RICH", mess.str(), -1, false );
      } else {
        // Ingress is OK, so read HPD data

        // Loop over active HPDs
        for ( const auto& HPD : inputs ) {

          // Create data bank and decode into RichSmartIDs
          const auto* hpdBank( createDataBank( &bank.data()[lineC], // pointer to start of data
                                               0,                   // Not needed here (to be removed). Must be 0 though
                                               version ) );

          // is this HPD suppressed ?
          const bool hpdIsSuppressed = hpdBank->suppressed();
          if ( hpdIsSuppressed ) {
            std::ostringstream mess;
            mess << "L1 board " << L1ID << " : Ingress " << ingressWord.ingressID() << " Input " << HPD
                 << " is HARDWARE suppressed";
            Warning( mess.str(), StatusCode::SUCCESS, 0 ).ignore();
          }

          // Try to add a new PDInfo to map
          const Level1Input l1Input( ingressWord.ingressID(), HPD );
          const auto        hpdInsert =
              ingressInfo.pdData().emplace( l1Input, PDInfo( LHCb::RichSmartID(), hpdBank->primaryHeaderWord(),
                                                             hpdBank->extendedHeaderWords(), hpdBank->footerWords() ) );
          // disable test (gives wrong warnings in TAE events)
          // if ( !p.second )
          //{
          //  std::ostringstream mess;
          //  mess << "Found multiple data blocks L1=" << L1ID << " input=" << l1Input;
          //  Warning( mess.str() );
          //}
          auto& pdInfo = hpdInsert.first->second;

          // Only try and decode this HPD if ODIN test was OK
          if ( odinOK && !hpdIsSuppressed ) {

            // get HPD RichSmartID
            // do in a try block incase HPD ID is unknown
            LHCb::RichSmartID pdID;
            try {
              pdID = ( m_useFakeHPDID ? s_fakeHPDID : m_richSys->richSmartID( hpdBank->level0ID() ) );
            } catch ( const GaudiException& expt ) {
              std::ostringstream errMsg;
              errMsg << "'" << expt.message() << "' | L1HardID=" << L1ID << " Ingress=" << ingressWord.ingressID()
                     << " Input=" << HPD;
              Error( errMsg.str() ).ignore();
            }
            // If the HPD smartID was successfully found, continue with decoding
            if ( pdID.isValid() ) {

              _ri_debug << "   Decoding HPD " << pdID << endmsg;

              // save HPD ID
              pdInfo.setPdID( pdID );

              // local hit count
              unsigned int hpdHitCount( 0 );

              // smartIDs
              auto& newids = pdInfo.smartIDs();

              // Compare Event IDs for errors
              bool OK =
                  ( hpdIsSuppressed ? true : !m_checkRICHEventsIDs || ingressWord.eventID() == hpdBank->eventID() );
              if ( !OK ) {
                std::ostringstream mess;
                mess << "EventID Mismatch : HPD L0ID=" << hpdBank->level0ID() << " " << pdID;
                Error( mess.str() ).ignore();
              } else {

                // Check this HPD is connected to the expected L1 board
                OK = true;                      // default to OK
                if ( UNLIKELY( m_hpdL1check ) ) // Do check ?
                {
                  const auto db_L1ID = m_richSys->level1HardwareID( hpdBank->level0ID() );
                  OK                 = ( L1ID == db_L1ID );
                  if ( !OK ) {
                    std::ostringstream mess;
                    mess << "HPD L0ID=" << hpdBank->level0ID() << " found in L1HardID=" << L1ID
                         << " but database says it should be in L1HardID=" << db_L1ID << " -> rejected";
                    Error( mess.str() ).ignore();
                  }
                }
                if ( OK ) // only carry on if OK
                {

                  // decode to smartIDs
                  hpdHitCount = hpdBank->fillRichSmartIDs( newids, pdID );

                  // Do data integrity checks
                  OK = ( !m_checkDataIntegrity || hpdBank->checkDataIntegrity( newids, this ) );
                  if ( !OK ) {
                    std::ostringstream mess;
                    mess << "HPD L0ID=" << hpdBank->level0ID() << " " << pdID << " data block failed integrity check";
                    Error( mess.str() ).ignore();
                    if ( m_purgeHPDsFailIntegrity ) { newids.clear(); }
                  }

                  // Is all 'OK' but header is in extended mode ?
                  if ( OK && hpdBank->nHeaderWords() > 1 ) {
                    std::ostringstream mess;
                    mess << "HPD L0ID=" << hpdBank->level0ID() << " " << pdID
                         << " in extended mode for UNKNOWN reasons";
                    Error( mess.str() ).ignore();
                  }

                  if ( msgLevel( MSG::VERBOSE ) && hpdHitCount > 0 ) {
                    // printout decoded RichSmartIDs
                    verbose() << " Decoded RichSmartIDs :-" << endmsg;
                    for ( const auto& ID : newids ) { verbose() << "   " << ID << endmsg; }
                  }
                }

              } // event IDs OK

              // is data OK
              if ( OK ) {

                // apply suppression of high occupancy HPDs
                if ( hpdHitCount < m_maxHPDOc ) {
                  const auto rich = pdID.rich();
                  ++nHPDbanks[rich];
                  decodedHits[rich] += hpdHitCount;

                  // suppress hot pixels
                  suppressHotPixels( pdID, newids );
                } else {
                  std::ostringstream hpd;
                  hpd << pdID.panelID();
                  Warning( "Forced suppression of HPD " + hpd.str(), StatusCode::SUCCESS, 0 ).ignore();
                  newids.clear();
                }
              } else if ( m_verboseErrors ) {
                // decoding error ....
                error() << "Error in decoding -> Data is rejected for HPD " << pdID << endmsg;

                if ( l1BankErrorDump ) {
                  error() << " -> Dump of raw L1 data :-" << endmsg;
                  dumpRawBank( bank, error() );
                  l1BankErrorDump = false;
                }

                error() << " -> Ingress header : " << ingressWord << endmsg;
                error() << "  -> ";
                rawDump( error(), ingressWord.data() );
                error() << endmsg;
                error() << " -> HPD data block : " << endmsg;
                error() << "  -> HPD Event ID = " << hpdBank->eventID() << " ";
                rawDump( error(), hpdBank->eventID().data(), hpdBank->eventID().activeBits() );
                error() << endmsg;
                error() << *hpdBank << endmsg;
              }

            } // HPD ID OK

          } // ODIN OK and not suppressed

          // Increment line number to next data block
          lineC += hpdBank->nTotalWords();

        } // active HPDs

      } // ingress not suppressed

    } // bank while loop

  } // data bank not empty

  // Add to the total number of decoded hits
  decodedData.addToTotalHits( decodedHits );
  decodedData.addToActivePDs( nHPDbanks );

  if ( m_summary ) {
    // Count the number of banks and size
    auto& cands = ( m_l1decodeSummary[m_currentTAE] )[L1IDandV( version, L1ID )];
    // Increment bank size
    cands.nWords += bank.size() / 4; // 2 L1 headers + data words
    // Increment hit occupancy
    cands.nHits += decodedHits[Rich::Rich1] + decodedHits[Rich::Rich2];
    // Count number of HPD banks
    cands.nHPDs += ( nHPDbanks[Rich::Rich1] + nHPDbanks[Rich::Rich2] );
    // Count fills
    ++cands.nFills;
  }

  // debug printout
  _ri_debug << "Decoded " << boost::format( "%2i" ) % ( nHPDbanks[Rich::Rich1] + nHPDbanks[Rich::Rich2] );
  _ri_debug << " PDs from Level1 Bank ID = " << boost::format( "%2i" ) % L1ID.data();
  _ri_debug << " : Size " << boost::format( "%4i" ) % ( bank.size() / 4 ) << " words : Version " << version << endmsg;
}

void RawDataFormatTool::suppressHotPixels( const LHCb::RichSmartID& pdID, LHCb::RichSmartID::Vector& newids ) const {
  // clean out hot pixels enabled at all ?
  if ( m_pixelsToSuppress ) {
    // Does this HPD have some pixels to suppress
    const auto iHPDSup = m_hotPixels.find( pdID );
    if ( iHPDSup != m_hotPixels.end() ) {
      // temporary copy of original hit channels
      const auto oldids = newids;

      // clear hits before suppression
      newids.clear();

      // loop over original hits and save only those not supressed
      for ( const auto& ID : oldids ) {
        if ( iHPDSup->second.find( ID ) == iHPDSup->second.end() ) {
          // not suppressed, so keep
          newids.push_back( ID );
        } else {
          // Get detector info
          const auto l0ID    = m_richSys->level0ID( pdID );
          const auto l1ID    = m_richSys->level1HardwareID( pdID );
          const auto l1Input = m_richSys->level1InputNum( pdID );
          // Print warning
          std::ostringstream mess;
          mess << "L1HardID=" << l1ID << " L1Input=" << l1Input << " L0ID=" << l0ID << " " << ID
               << " is software SUPPRESSED";
          Warning( mess.str(), StatusCode::SUCCESS );
        }

      } // loop over pixels

    } // this HPD has pixels to suppress

  } // hot pixel suppression enabled
}

void RawDataFormatTool::decodeToSmartIDs_2006TB( const LHCb::RawBank& bank, L1Map& decodedData ) const {
  using namespace Rich::DAQ::HPD;

  // Get L1 ID
  const Level1HardwareID L1ID( bank.sourceID() );

  // HPD count
  DetectorArray<unsigned int> nHPDbanks{{0, 0}}, decodedHits{{0, 0}};

  // Data bank size in words
  const int bankSize = bank.size() / 4;

  // Get bank version
  const auto version = bankVersion( bank );

  if ( bankSize > 0 ) {

    // ... otherwise, must have at least 2 entries
    if ( bankSize < 2 ) { Exception( "Non-empty RICH Bank size is less than 2 !" ); }

    // Get Ingress map to decode into for this L1 board
    IngressMap& ingressMap = decodedData[L1ID];

    // This data version does not have ingress info, so just put all data into ingress 0
    const L1IngressID ingressNum( 0 );

    // Get data for this ingress
    auto& ingressInfo = ingressMap[ingressNum];

    // Make up L1 input numbers
    Level1Input l1Input( 0 );

    // Loop over bank, find headers and produce a data bank for each
    // Fill data into RichSmartIDs
    int lineC( 0 );
    while ( lineC < bankSize ) {

      // Create data bank and decode into RichSmartIDs
      const auto* hpdBank( createDataBank( &bank.data()[lineC], // pointer to start of data
                                           0,                   // Not needed here (to be removed). Must be 0 though
                                           version ) );

      // get HPD RichSmartID
      const LHCb::RichSmartID pdID = ( m_useFakeHPDID ? s_fakeHPDID : m_richSys->richSmartID( hpdBank->level0ID() ) );

      // decode to smartIDs
      auto& pdInfo = ( ingressInfo.pdData() )[l1Input];
      pdInfo.setPdID( pdID );
      ++l1Input;
      auto&      newids      = pdInfo.smartIDs();
      const auto hpdHitCount = hpdBank->fillRichSmartIDs( newids, pdID );

      // Do data integrity checks
      const bool OK = ( !m_checkDataIntegrity || hpdBank->checkDataIntegrity( newids, this ) );
      if ( !OK || msgLevel( MSG::VERBOSE ) ) {
        // printout decoded RichSmartIDs
        verbose() << " Decoded RichSmartIDs :-" << endmsg;
        for ( const auto& ID : newids ) { verbose() << "   " << ID << endmsg; }
      }
      if ( !OK && m_purgeHPDsFailIntegrity ) { newids.clear(); }

      // is data OK
      if ( OK ) {

        // apply suppression of high occupancy HPDs
        if ( hpdHitCount < m_maxHPDOc ) {
          const auto rich = pdID.rich();
          ++nHPDbanks[rich];
          decodedHits[rich] += hpdHitCount;
        } else {
          std::ostringstream hpd;
          hpd << pdID.panelID();
          Warning( "Forced suppression of HPD " + hpd.str(), StatusCode::SUCCESS, 0 ).ignore();
          newids.clear();
        }
      } else if ( m_verboseErrors ) {
        // decoding error ....
        error() << "Corruption in decoding -> Data is rejected for HPD " << pdID << endmsg;

        error() << " -> Dump of offending raw L1 data :-" << endmsg;
        dumpRawBank( bank, error() );

        error() << " -> Badly decoded HPD :-" << endmsg;
        error() << *hpdBank << endmsg;
      }

      // Increment line number to next data block
      lineC += hpdBank->nTotalWords();

    } // bank while loop

  } // data bank not empty

  // Add to the total number of decoded hits
  decodedData.addToTotalHits( decodedHits );
  decodedData.addToActivePDs( nHPDbanks );

  if ( m_summary ) {
    // Count the number of banks and size
    L1CountAndSize& cands = ( m_l1decodeSummary[m_currentTAE] )[L1IDandV( version, L1ID )];
    // Increment bank size
    cands.nWords += bank.size() / 4; // 2 L1 headers + data words
    // Increment hit occupancy
    cands.nHits += decodedHits[Rich::Rich1] + decodedHits[Rich::Rich2];
    // Count number of HPD banks
    cands.nHPDs += ( nHPDbanks[Rich::Rich1] + nHPDbanks[Rich::Rich2] );
    // Count fills
    ++cands.nFills;
  }

  // debug printout
  _ri_debug << "Decoded " << boost::format( "%2i" ) % ( nHPDbanks[Rich::Rich1] + nHPDbanks[Rich::Rich2] );
  _ri_debug << " PDs from Level1 Bank " << boost::format( "%2i" ) % L1ID.data();
  _ri_debug << " : Size " << boost::format( "%4i" ) % ( bank.size() / 4 ) << " words : Version " << version << endmsg;
}

void RawDataFormatTool::decodeToSmartIDs_DC0406( const LHCb::RawBank& bank, L1Map& decodedData ) const {
  // import HPD parameters
  using namespace Rich::DAQ::HPD;

  // Get L1 ID
  const Level1HardwareID base_L1ID( bank.sourceID() );

  // Get max data size for LHCb mode
  const ShortType maxDataSize = MaxDataSize;

  // HPD count
  DetectorArray<unsigned int> nHPDbanks{{0, 0}}, decodedHits{{0, 0}};

  // Data bank size in words
  const int bankSize = bank.size() / 4;

  // Get bank version
  const auto version = bankVersion( bank );

  // Header type
  typedef RichDAQHeaderV1::RichDAQHeaderPD MyHeader;

  // Is this an empty bank ?
  if ( bankSize > 0 ) {

    // ... otherwise, must have at least 2 entries
    if ( bankSize < 2 ) { Exception( "Non-empty RICH Bank size is less than 2 !" ); }

    // Make up L1 input numbers when using fake PDIDs
    Level1Input fake_l1Input( 0 );

    // Loop over bank, find headers and produce a data bank for each
    // Fill data into RichSmartIDs
    int lineC( 0 );
    while ( lineC < bankSize ) {

      // Find HPD bank start
      const MyHeader header( &bank.data()[lineC] );

      // Is this a true header
      if ( header.startPD() ) {

        _ri_verbo << " Found HPD header at line " << lineC << " of " << bankSize << endmsg;

        // Store start line for header
        const int lineHeader = lineC;

        // Find last line of block
        int lineLast = lineC;
        if ( header.zeroSuppressed() ) {
          // For ZS blocks, have to search for the hext header to define the block length

          bool cont = true;
          while ( cont && lineC < bankSize ) {
            ++lineC;
            // Test if this is the last line of data or a new header word
            if ( lineC == bankSize || MyHeader( &bank.data()[lineC] ).startPD() ) {
              lineLast = lineC - 1;
              cont     = false;
            }
          }

          _ri_verbo << "  -> Bank is zero surpressed : ends at " << lineLast << endmsg;
        } else {
          // non-ZS blocks have fixed length, so skip straight to the end

          lineC += 1 + maxDataSize; // data block + header
          lineLast = lineC - 1;

          _ri_verbo << "  -> Bank is non zero surpressed : ends at " << lineLast << endmsg;
        }

        // Check data size
        const unsigned int dataSize = lineLast - lineHeader;
        if ( dataSize < 1 ) {
          std::ostringstream message;
          message << "Invalid HPD data block size : " << dataSize;
          Exception( message.str() );
        }

        // Create data bank and decode into RichSmartIDs
        const auto* hpdBank( createDataBank( &bank.data()[lineHeader], dataSize, version ) );

        // get HPD RichSmartID
        const LHCb::RichSmartID pdID = ( m_useFakeHPDID ? s_fakeHPDID : m_richSys->richSmartID( hpdBank->level0ID() ) );

        // L1 ID
        const auto L1ID = m_richSys->level1HardwareID( pdID );
        if ( L1ID != base_L1ID ) {
          error() << "L1ID Mis-match" << endmsg;
          error() << "  -> base : " << base_L1ID << endmsg;
          error() << "  -> HPD  : " << L1ID << endmsg;
        }

        // Get Ingress map to decode into for this L1 board
        auto& ingressMap = decodedData[L1ID];

        // L1 input number
        const Level1Input l1Input = ( m_useFakeHPDID ? fake_l1Input : m_richSys->level1InputNum( pdID ) );
        if ( m_useFakeHPDID ) ++fake_l1Input;

        // Ingress info
        auto& ingressInfo = ingressMap[l1Input.ingressID()];

        // get HPD data
        auto& pdInfo = ( ingressInfo.pdData() )[l1Input];
        pdInfo.setPdID( pdID );
        auto& newids = pdInfo.smartIDs();

        // get hit count
        const auto hpdHitCount = hpdBank->fillRichSmartIDs( newids, pdID );

        // apply suppression of high occupancy HPDs
        if ( hpdHitCount < m_maxHPDOc ) {
          const auto rich = pdID.rich();
          ++nHPDbanks[rich];
          decodedHits[rich] += hpdHitCount;
        } else {
          std::ostringstream hpd;
          hpd << pdID.panelID();
          Warning( "Suppressed HPD in " + hpd.str(), StatusCode::SUCCESS, 0 ).ignore();
          newids.clear();
        }
      } else // Not a data header line
      {
        ++lineC;
      }

    } // bank while loop

  } // data bank not empty

  // Add to the total number of decoded hits
  decodedData.addToTotalHits( decodedHits );
  decodedData.addToActivePDs( nHPDbanks );

  if ( m_summary ) {
    // Count the number of banks and size
    auto& cands = ( m_l1decodeSummary[m_currentTAE] )[L1IDandV( version, base_L1ID )];
    // Increment bank size
    cands.nWords += bank.size() / 4; // 2 L1 headers + data words
    // Increment hit occupancy
    cands.nHits += decodedHits[Rich::Rich1] + decodedHits[Rich::Rich2];
    // Count number of HPD banks
    cands.nHPDs += ( nHPDbanks[Rich::Rich1] + nHPDbanks[Rich::Rich2] );
    // Count fills
    ++cands.nFills;
  }

  // debug printout
  _ri_debug << "Decoded " << boost::format( "%2i" ) % ( nHPDbanks[Rich::Rich1] + nHPDbanks[Rich::Rich2] );
  _ri_debug << " PDs from Level1 Bank " << boost::format( "%2i" ) % base_L1ID.data();
  _ri_debug << " : Size " << boost::format( "%4i" ) % ( bank.size() / 4 ) << " words : Version " << version << endmsg;
}

void RawDataFormatTool::decodeToSmartIDs( const RawEventLocations& taeLocations, L1Map& decodedData ) const {
  // set the been used flag
  m_hasBeenCalled = true;

  // clear the L1 ID map
  m_l1IdsDecoded.clear();

  // Loop over all RawEvent locations
  for ( const auto& loc : taeLocations ) {
    // Set the current TAE type (for use by odin and raw event methods)
    m_currentTAE = loc;
    if ( !m_currentTAE.empty() ) m_currentTAE += "/";

    // Get the raw event
    auto* rawEv = rawEvent();
    if ( !rawEv ) continue;

    // Get the banks for the Rich
    const auto& richBanks = rawEv->banks( LHCb::RawBank::Rich );

    // Loop over data banks
    for ( const LHCb::RawBank* bank : richBanks ) {
      // test bank is OK
      if ( bank ) {
        // Decode this bank
        try {
          decodeToSmartIDs( *bank, decodedData );
        } catch ( const GaudiException& expt ) {
          // Print error message
          std::ostringstream mess;
          mess << "Error decoding bank ID=" << bank->sourceID() << " version=" << bankVersion( *bank ) << " '"
               << expt.message() << "' '" << expt.tag() << "'";
          Error( mess.str() ).ignore();
          // dump the full bank
          if ( m_verboseErrors ) dumpRawBank( *bank, error() );
        }
      } else {
        Error( "Retrieved null pointer to RawBank" ).ignore();
      }
    }

    // do not print if faking PDID, since smartIDs.size() then has no meaning
    if ( !m_useFakeHPDID ) {
      _ri_debug << "Decoded in total " << richBanks.size() << " RICH Level1 bank(s) for RawEvent '" << loc << "'"
                << endmsg;
    }

  } // loop over RawEvent locations in the TES

  // loop over the L1 map to check we only got each L1 ID once
  // (only if not using more than one RawEvent)
  // CRJ : Is this check really needed ?
  if ( taeLocations.size() == 1 ) {
    for ( const auto& L1 : m_l1IdsDecoded ) {
      if ( L1.second > 1 ) {
        std::ostringstream mess;
        mess << "Found multiple RawBanks (" << L1.second << ") for L1 ID " << L1.first;
        Warning( mess.str() ).ignore();
      }
    }
  }
}

LHCb::RawEvent* RawDataFormatTool::rawEvent() const {
  auto*& raw = m_rawEvent[m_currentTAE];
  if ( UNLIKELY( !raw ) ) {
    for ( const auto& p : m_rawEventLocations ) {
      raw = getIfExists<LHCb::RawEvent>( m_currentTAE + p );
      if ( raw ) { break; }
    }
    if ( !raw ) { Warning( "Failed to locate RawEvent" ).ignore(); }
  }
  return raw;
}

const Rich::DAQ::L1Map& RawDataFormatTool::dummyMap() const {
  static Rich::DAQ::L1Map dummyMap;
  if ( dummyMap.empty() ) {
    // create an ingressmap for each L1 board
    Rich::DAQ::IngressMap ingressMap;
    for ( unsigned int input = 0; input < Rich::DAQ::HPD::NumIngressPerL1; ++input ) {
      L1IngressHeader header;
      header.setIngressID( L1IngressID( input ) );
      ingressMap[L1IngressID( input )].setIngressHeader( header );
    }
    // create a dummy L1data object with a default ingress map for each L1 board
    for ( const auto& ID : m_richSys->level1HardwareIDs() ) { dummyMap[ID] = ingressMap; }
    _ri_debug << "Created " << dummyMap.size()
              << " entries in empty L1 map : L1HardwareIDs = " << m_richSys->level1HardwareIDs() << endmsg;
  }
  return dummyMap;
}

void RawDataFormatTool::dumpRawBank( const LHCb::RawBank& bank, MsgStream& os ) const {

  // Get bank version and ID
  const Level1HardwareID L1ID( bank.sourceID() );
  const auto             version = bankVersion( bank );

  // Data bank size in words
  const int bankSize = bank.size() / 4;

  std::ostringstream magicAsHex;
  magicAsHex << std::hex << bank.magic();
  os << "RawBank version=" << version << " L1ID=" << L1ID << " datasize=" << bankSize << " magic=" << magicAsHex.str()
     << endmsg;

  // Printout raw data

  // Is this an empty bank ?
  if ( bankSize > 0 ) {
    const std::string& LINES = "-------------------------------------------------------------------"
                               "---------------------------------------------";

    // Bit numbers
    os << LINES << endmsg;
    os << "      bit |";
    for ( int iCol = 31; iCol >= 0; --iCol ) { os << boost::format( "%3i" ) % iCol; }
    os << endmsg;
    os << LINES << endmsg;
    typedef unsigned int Dtype;
    for ( const Dtype* dataW = bank.begin<Dtype>(); dataW != bank.end<Dtype>(); ++dataW ) {
      rawDump( os, *dataW );
      os << endmsg;
    }
    os << LINES << endmsg;
  } else {
    os << "  -> Bank is empty" << endmsg;
  }
}

//-----------------------------------------------------------------------------

DECLARE_COMPONENT( RawDataFormatTool )

//-----------------------------------------------------------------------------
