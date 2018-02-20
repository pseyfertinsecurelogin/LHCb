
// local
#include "RichRawBankDecoder.h"

//-----------------------------------------------------------------------------
// Implementation file for class : RichRawBankDecoder
//
// 2016-09-21 : Chris Jones
//-----------------------------------------------------------------------------

using namespace Rich::Future;
using namespace Rich::Future::DAQ;
using namespace Rich::DAQ;

// private namespace
namespace
{
  /// Default 'fake' HPD RichSmartID
  static const LHCb::RichSmartID s_fakeHPDID( Rich::Rich1,Rich::top,0,0 );
}

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
RawBankDecoder::RawBankDecoder( const std::string& name,
                                ISvcLocator* pSvcLocator )
  : Transformer ( name, pSvcLocator,
                  { KeyValue{ "RawEventLocation",
                        concat_alternatives( { LHCb::RawEventLocation::Rich,
                                               LHCb::RawEventLocation::Default } ) },
                    KeyValue{ "OdinLocation", LHCb::ODINLocation::Default } },
                  { KeyValue{ "DecodedDataLocation", L1MapLocation::Default } } )
{
  //setProperty( "OutputLevel", MSG::VERBOSE );
}

//=============================================================================

StatusCode RawBankDecoder::initialize()
{
  // Initialise base class
  const auto sc = Transformer::initialize();
  if ( !sc ) return sc;

  // RichDet
  m_richSys = getDet<DeRichSystem>( DeRichLocations::RichSystem );

  // report inactive RICHes
  if ( !m_richIsActive[Rich::Rich1] )
  { Warning("Decoding for RICH1 disabled",StatusCode::SUCCESS).ignore(); }
  if ( !m_richIsActive[Rich::Rich2] )
  { Warning("Decoding for RICH2 disabled",StatusCode::SUCCESS).ignore(); }

  // if suppression is less than max possible number of (ALICE) hits, print a message.
  if ( m_maxHPDOc < BitsPerDataWord * MaxDataSizeALICE )
    info() << "Will suppress PDs with more than " << m_maxHPDOc << " digits" << endmsg;

  // messages if optional features are enabled
  if ( m_decodeUseOdin )
    info() << "ODIN integrity checks are enabled" << endmsg;
  if ( m_hpdL1check )
    info() << "HPD L1 ID checks are enabled" << endmsg;

  // warnings if checks are disabled
  if ( !m_checkDataIntegrity )
    Warning( "HPD Data integrity checks are disabled",     StatusCode::SUCCESS ).ignore();
  if ( m_checkODINEventsIDs )
    Warning( "Ingress/ODIN Event ID checks are enabled",   StatusCode::SUCCESS ).ignore();
  if ( !m_checkRICHEventsIDs )
    Warning( "Internal RICH Event ID checks are disabled", StatusCode::SUCCESS ).ignore();
  if ( !m_checkBxIDs )
    Warning( "Header BX ID checks are disabled",           StatusCode::SUCCESS ).ignore();

  // Do we have pixels to suppress ?
  if ( !m_hotChannels.empty() )
  {
    info() << "Hot HPD pixel list to suppress :-" << endmsg;
    m_pixelsToSuppress = true;
    for ( const auto& C : m_hotChannels )
    {
      const LHCb::RichSmartID channel(C);
      m_hotPixels[channel.pdID()].insert(channel);
      info() << " " << channel << " (" << channel.key() << ")" << endmsg;
    }
  }

  return sc;
}

//=============================================================================

L1Map RawBankDecoder::operator()( const LHCb::RawEvent& rawEvent,
                                  const LHCb::ODIN& odin ) const
{
  // Make the data map to return
  L1Map decodedData;

  // Get the banks for the Rich
  const auto & richBanks = rawEvent.banks( LHCb::RawBank::Rich );

  // reserve top level size
  decodedData.reserve( richBanks.size() );

  // Loop over data banks
  for ( const auto * bank : richBanks )
  {
    // test bank is OK
    if ( bank )
    {
      // Decode this bank
      try
      {
        decodeToSmartIDs( *bank, odin, decodedData );
      }
      catch ( const GaudiException & expt )
      {
        // Print error message
        std::ostringstream mess;
        mess << "Error decoding bank ID=" << bank->sourceID() 
             << " version=" << bankVersion(*bank)
             << " '" << expt.message() << "' '" << expt.tag() << "'";
        Error( mess.str() ).ignore();
        // dump the full bank
        if ( m_verboseErrors ) dumpRawBank( *bank, error() );
      }
    }
    else
    {
      Error( "Retrieved null pointer to RawBank" ).ignore();
    }
  }
   
  // do not print if faking HPDID, since smartIDs.size() then has no meaning
  if ( !m_useFakeHPDID )
  {
    _ri_debug << "Decoded in total " << richBanks.size()
              << " RICH Level1 bank(s)" << endmsg;
  }
    
  // return the fill map
  return decodedData;
}

//=============================================================================

void RawBankDecoder::decodeToSmartIDs( const LHCb::RawBank & bank,
                                       const LHCb::ODIN& odin,
                                       L1Map & decodedData ) const
{

  // Check magic code for general data corruption
  if ( UNLIKELY( LHCb::RawBank::MagicPattern != bank.magic() ) )
  {
    std::ostringstream mess;
    mess << "Magic Pattern mis-match : Expected " << LHCb::RawBank::MagicPattern
         << " Obtained " << bank.magic();
    Error( mess.str() ).ignore();
  }
  else
  { // All OK so decode

    // Check this is a RICH bank
    if ( bank.type() != LHCb::RawBank::Rich )
    {
      std::ostringstream message;
      message << "BankType is not RICH : type = " << bank.type();
      Exception( message.str() );
    }

    // Get L1 ID
    const Level1HardwareID L1ID ( bank.sourceID() );

    // Get bank version
    const auto version = bankVersion( bank );

    // Are we decoding this bank ?
    if ( okToDecode(L1ID) )
    {

      // if configured, dump raw event before decoding
      if      ( msgLevel(MSG::VERBOSE) ) { dumpRawBank( bank, verbose() ); }
      else if ( m_dumpBanks            ) { dumpRawBank( bank, info()    ); }

      // Now, delegate the work to a version of the decoding appropriate to the version
      if      ( version == LHCb5    ||
                version == FlatList  )
      {
        decodeToSmartIDs_2007(bank,odin,decodedData);
      }
      else if ( version == LHCb3 ||
                version == LHCb4  ) // RICH 2006 Testbeam
      {
        decodeToSmartIDs_2006TB(bank,decodedData);
      }
      else if ( version == LHCb0 ||
                version == LHCb1 ||
                version == LHCb2  )  // DC04 or DC06
      {
        decodeToSmartIDs_DC0406(bank,decodedData);
      }
      else if ( MaPMT0 == version )
      {
        decodeToSmartIDs_MaPMT0(bank,decodedData);
      }
      else // Some problem ...
      {
        std::ostringstream mess;
        mess << "Unknown RICH L1 version number " << version;
        Exception( mess.str() );
      }

    } // L1 is decoded

  } // magic OK

}

//=============================================================================

std::unique_ptr<const Rich::DAQ::HPDDataBank> 
RawBankDecoder::createDataBank( const LongType * dataStart,
                                const unsigned int dataSize,
                                const BankVersion version ) const
{
  std::unique_ptr<const Rich::DAQ::HPDDataBank> dataBank;

  if ( LHCb5 == version )
  {

    // Quick check of header for HPD data type
    const bool isZS    = RichDAQ_LHCb5::Header::zeroSuppressed(dataStart);
    const bool isAlice = RichDAQ_LHCb5::Header::aliceMode(dataStart);

    // Construct the correct data block
    if ( isZS )
    {
      if ( UNLIKELY(isAlice) )
      {
        dataBank = std::make_unique<RichDAQ_LHCb5::ZeroSuppAlice>( dataStart );
      }
      else
      {
        dataBank = std::make_unique<RichDAQ_LHCb5::ZeroSuppLHCb>( dataStart );
      }
    }
    else
    {
      if ( UNLIKELY(isAlice) )
      {
        dataBank = std::make_unique<RichDAQ_LHCb5::NonZeroSuppAlice>( dataStart );
      }
      else
      {
        dataBank = std::make_unique<RichDAQ_LHCb5::NonZeroSuppLHCb>( dataStart );
      }
    }

  }
  else if ( LHCb4 == version )
  {

    // Quick check of header for HPD data type
    const bool isZS    = RichDAQ_LHCb4::Header::zeroSuppressed(dataStart);
    const bool isAlice = RichDAQ_LHCb4::Header::aliceMode(dataStart);

    // Decide to zero suppress or not depending on number of hits
    if ( isZS )
    {
      if ( UNLIKELY(isAlice) )
      {
        Warning ( "LHCb4 data format does not support ZS Alice mode data" ).ignore();
      }
      else
      {
        dataBank = std::make_unique<RichDAQ_LHCb4::ZeroSuppLHCb>( dataStart );
      }
    }
    else
    {
      if ( UNLIKELY(isAlice) )
      {
        dataBank = std::make_unique<RichDAQ_LHCb4::NonZeroSuppAlice>( dataStart );
      }
      else
      {
        dataBank = std::make_unique<RichDAQ_LHCb4::NonZeroSuppLHCb>( dataStart );
      }
    }

  }
  else if ( LHCb3 == version )
  {

    // Quick check of header for HPD data type
    const bool isZS    = RichDAQ_LHCb3::Header::zeroSuppressed(dataStart);
    const bool isAlice = RichDAQ_LHCb3::Header::aliceMode(dataStart);

    // Decide to zero suppress or not depending on number of hits
    if ( isZS )
    {
      if ( UNLIKELY(isAlice) )
      {
        Warning ( "LHCb3 data format does not support ZS Alice mode data" ).ignore();
      }
      else
      {
        dataBank = std::make_unique<RichDAQ_LHCb3::ZeroSuppLHCb>( dataStart );
      }
    }
    else
    {
      if ( UNLIKELY(isAlice) )
      {
        dataBank = std::make_unique<RichDAQ_LHCb3::NonZeroSuppAlice>( dataStart );
      }
      else
      {
        dataBank = std::make_unique<RichDAQ_LHCb3::NonZeroSuppLHCb>( dataStart );
      }
    }

  }
  else if ( LHCb2 == version )
  {

    // Quick check of header for HPD data type
    const bool isZS = RichDAQ_LHCb2::Header::zeroSuppressed(dataStart);

    // Decide to zero suppress or not depending on number of hits
    if ( isZS )
    {
      dataBank = std::make_unique<RichDAQ_LHCb2::ZeroSuppLHCb>( dataStart, dataSize );
    }
    else
    {
      dataBank = std::make_unique<RichDAQ_LHCb2::NonZeroSuppLHCb>( dataStart );
    }

  }
  else if ( LHCb1 == version )
  {

    // Quick check of header for HPD data type
    const bool isZS = RichDAQ_LHCb1::Header::zeroSuppressed(dataStart);
    
    // Decide to zero suppress or not depending on number of hits
    if ( isZS )
    {
      dataBank = std::make_unique<RichDAQ_LHCb1::ZeroSuppLHCb>( dataStart, dataSize );
    }
    else
    {
      dataBank = std::make_unique<RichDAQ_LHCb1::NonZeroSuppLHCb>( dataStart );
    }

  }
  else if ( FlatList == version )
  {
    dataBank = std::make_unique<RichDAQ_FlatList::Data>( dataStart );
  }
  else
  {
    std::ostringstream message;
    message << "Unknown RICH Raw Buffer version " << version << " -> No data bank";
    Exception ( message.str() );
  }

  // Printout this bank
  if ( UNLIKELY( dataBank && msgLevel(MSG::VERBOSE) ) )
  {
    verbose() << endmsg
              << "Created HPD Data Bank for Decoding :-" << endmsg;
    verbose() << *dataBank << endmsg;
  }

  return dataBank;
}

//=============================================================================

void RawBankDecoder::decodeToSmartIDs_2007( const LHCb::RawBank & bank,
                                            const LHCb::ODIN& odin,
                                            L1Map & decodedData ) const
{
  // Get L1 ID
  const Level1HardwareID L1ID ( bank.sourceID() );
  _ri_debug << "Decoding L1 bank " << L1ID << endmsg;

  // various counts
  DetectorArray<unsigned int> nHPDbanks{{0,0}}, decodedHits{{0,0}};

  // Data bank size in 32 bit words
  const auto bankSize = bank.size() / 4;

  // Get bank version
  const auto version = bankVersion( bank );

  // Flag to indicate if a given L1 bank has been printed out in case of an error
  bool l1BankErrorDump = true;

  // List of active L1 ingress inputs (define here so can be reused for each ingress)
  L1IngressInputs inputs;

  // If we have some words to process, start the decoding
  if ( bankSize > 0 )
  {

    // Get Ingress map to decode into for this L1 board
    decodedData.emplace_back( std::piecewise_construct, 
                              std::forward_as_tuple(L1ID), 
                              std::forward_as_tuple() );
    auto & ingressMap = decodedData.back().second;

    // reserve size
    ingressMap.reserve( Rich::DAQ::NumIngressPerL1 );

    // Loop over bank, find headers and produce a data bank for each
    // Fill data into RichSmartIDs
    int lineC(0);
    while ( lineC < bankSize )
    {

      // First, the ingress header word (and increment to the next line)
      const L1IngressHeader ingressWord( bank.data()[lineC++] );
      _ri_debug << " Ingress " << ingressWord << endmsg;

      // Get data for this ingress
      ingressMap.emplace_back( std::piecewise_construct, 
                               std::forward_as_tuple(ingressWord.ingressID()), 
                               std::forward_as_tuple() );
      auto & ingressInfo = ingressMap.back().second;

      // Set ingress header in decoded data map
      ingressInfo.setIngressHeader( ingressWord );

      // Compare Ingress header to the ODIN
      _ri_verbo << "ODIN : EventNumber=" << EventID(odin.eventNumber())
                << " BunchID=" << BXID(odin.bunchId()) << endmsg;
      const bool odinOK
        = ( !m_decodeUseOdin ? true :
            ( !m_checkODINEventsIDs || ingressWord.eventID() == EventID(odin.eventNumber()) ) &&
            ( !m_checkBxIDs         || ingressWord.bxID()    == BXID   (odin.bunchId())     ) );
      if ( !odinOK )
      {
        std::ostringstream mess;
        mess << "ODIN Mismatch : L1ID " << L1ID
             << " : ODIN EvID="         << EventID(odin.eventNumber())
             << " BxID="                << BXID(odin.bunchId())
             << " : L1IngressHeader "   << ingressWord
             << " -> Data Suppressed";
        Error( mess.str() ).ignore();
      }

      // get list of active ingress inputs
      ingressWord.activeHPDInputs(inputs);
      _ri_debug << "  Found " << inputs.size() << " PDs with data blocks : " << inputs
                << endmsg;
      
      // Check the Ingress supression flag
      if ( ingressWord.hpdsSuppressed() )
      {
        std::ostringstream mess;
        mess << "L1 board " << L1ID << " : Ingress " << ingressWord
             << " is HARDWARE suppressed";
        Warning( mess.str(), StatusCode::SUCCESS, 1 ).ignore();
        //procStatus()->addAlgorithmStatus( name(), "RICH", mess.str(), -1, false );
      }
      else
      {
        // Ingress is OK, so read HPD data

        // reserve size
        ingressInfo.pdData().reserve( inputs.size() );

        // Loop over active HPDs
        for ( const auto& HPD : inputs )
        {

          // Create data bank and decode into RichSmartIDs
          const auto hpdBank 
            ( createDataBank( &bank.data()[lineC], // pointer to start of data
                              0, // Not needed here (to be removed). Must be 0 though
                              version ) );

          // is this HPD suppressed ?
          const bool hpdIsSuppressed = hpdBank->suppressed();
          if ( hpdIsSuppressed )
          {
            std::ostringstream mess;
            mess << "L1 board " << L1ID << " : Ingress "
                 << ingressWord.ingressID() << " Input " << HPD << " is HARDWARE suppressed";
            Warning( mess.str(), StatusCode::SUCCESS, 0 ).ignore();
          }

          // Is the PD in extended mode
          const bool isExtend = hpdBank->isExtended();

          // Try to add a new HPDInfo to container
          if ( !isExtend )
          {
            ingressInfo.pdData().emplace_back( std::piecewise_construct, 
                                               std::forward_as_tuple( ingressWord.ingressID(), HPD ), 
                                               std::forward_as_tuple( LHCb::RichSmartID(),
                                                                      hpdBank->primaryHeaderWord() ) );
          }
          else
          {
            ingressInfo.pdData().emplace_back( std::piecewise_construct, 
                                               std::forward_as_tuple( ingressWord.ingressID(), HPD ),  
                                               std::forward_as_tuple( LHCb::RichSmartID(),
                                                                      hpdBank->primaryHeaderWord(),
                                                                      hpdBank->extendedHeaderWords(),
                                                                      hpdBank->footerWords() ) );
          }
          auto & hpdInfo = ingressInfo.pdData().back().second;
         
          // Only try and decode this HPD if ODIN test was OK
          if ( odinOK && !hpdIsSuppressed )
          {

            // get HPD RichSmartID
            // do in a try block incase HPD ID is unknown
            LHCb::RichSmartID hpdID;
            try
            {
              hpdID = ( m_useFakeHPDID ? s_fakeHPDID :
                        m_richSys->richSmartID( hpdBank->level0ID() ) );
            }
            catch ( const GaudiException & expt )
            {
              std::ostringstream errMsg;
              errMsg << "'" << expt.message() << "' | L1HardID=" << L1ID
                     << " Ingress=" << ingressWord.ingressID() << " Input=" << HPD;
              Error( errMsg.str() ).ignore();
            }
            // If the HPD smartID was successfully found, continue with decoding
            if ( hpdID.isValid() )
            {

              _ri_debug << "   Decoding HPD " << hpdID << endmsg;

              // save HPD ID
              hpdInfo.setPdID(hpdID);

              // local hit count
              unsigned int hpdHitCount(0);

              // smartIDs
              auto & newids = hpdInfo.smartIDs();

              // Compare Event IDs for errors
              bool OK = ( hpdIsSuppressed ? true :
                          !m_checkRICHEventsIDs || ingressWord.eventID() == hpdBank->eventID() );
              if ( !OK )
              {
                std::ostringstream mess;
                mess << "EventID Mismatch : HPD L0ID="
                     <<  hpdBank->level0ID() << " " << hpdID;
                Error( mess.str() ).ignore();
              }
              else
              {

                // Check this HPD is connected to the expected L1 board
                OK = true;                    // default to OK
                if ( UNLIKELY(m_hpdL1check) ) // Do check ?
                {
                  const auto db_L1ID = m_richSys->level1HardwareID(hpdBank->level0ID());
                  OK = ( L1ID == db_L1ID );
                  if ( !OK )
                  {
                    std::ostringstream mess;
                    mess << "HPD L0ID=" << hpdBank->level0ID() << " found in L1HardID=" << L1ID
                         << " but database says it should be in L1HardID=" << db_L1ID
                         << " -> rejected";
                    Error( mess.str() ).ignore();
                  }
                }
                if ( OK ) // only carry on if OK
                {

                  // decode to smartIDs
                  hpdHitCount = hpdBank->fillRichSmartIDs( newids, hpdID );

                  // Do data integrity checks
                  OK = ( !m_checkDataIntegrity || hpdBank->checkDataIntegrity(newids,this) );
                  if ( !OK )
                  {
                    std::ostringstream mess;
                    mess << "HPD L0ID=" << hpdBank->level0ID() << " " << hpdID
                         << " data block failed integrity check";
                    Error( mess.str() ).ignore();
                    if ( m_purgeHPDsFailIntegrity ) { newids.clear(); }
                  }

                  // Is all 'OK' but header is in extended mode ?
                  if ( OK && isExtend )
                  {
                    std::ostringstream mess;
                    mess << "HPD L0ID=" << hpdBank->level0ID() << " " << hpdID
                         << " in extended mode for UNKNOWN reasons";
                    Error( mess.str() ).ignore();
                  }

                  if ( msgLevel(MSG::VERBOSE) && hpdHitCount>0 )
                  {
                    // printout decoded RichSmartIDs
                    verbose() << " Decoded RichSmartIDs :-" << endmsg;
                    for ( const auto& ID : newids ) { verbose() << "   " << ID << endmsg; }
                  }

                }

              } // event IDs OK

              // is data OK
              if ( OK )
              {

                // apply suppression of high occupancy HPDs
                if ( hpdHitCount < m_maxHPDOc )
                {
                  const auto rich = hpdID.rich();
                  // counts
                  ++nHPDbanks[rich];
                  decodedHits[rich] += hpdHitCount;
                  // suppress hot pixels
                  suppressHotPixels(hpdID,newids);
                }
                else
                {
                  std::ostringstream hpd;
                  hpd << hpdID.panelID();
                  Warning( "Forced suppression of HPD "+hpd.str(), StatusCode::SUCCESS, 0 ).ignore();
                  newids.clear();
                }

              }
              else if ( m_verboseErrors )
              {
                // decoding error ....
                error() << "Error in decoding -> Data is rejected for HPD " << hpdID << endmsg;

                if ( l1BankErrorDump )
                {
                  error() << " -> Dump of raw L1 data :-" << endmsg;
                  dumpRawBank( bank, error() );
                  l1BankErrorDump = false;
                }

                error() << " -> Ingress header : " << ingressWord << endmsg;
                error() << "  -> ";
                rawDump(error(),ingressWord.data());
                error() << endmsg;
                error() << " -> HPD data block : " << endmsg;
                error() << "  -> HPD Event ID = " << hpdBank->eventID() << " ";
                rawDump(error(),hpdBank->eventID().data(),hpdBank->eventID().activeBits());
                error() << endmsg;
                error() << *hpdBank << endmsg;

              }

            } // HPD ID OK

          } // ODIN OK and not suppressed

          // Increment line number to next data block
          lineC += hpdBank->nTotalWords();

        } // active HPDs

      } // ingress not suppressed

      // clear the inputs for next time
      inputs.clear();

    } // bank while loop

  } // data bank not empty

  // Add to the total number of decoded hits
  decodedData.addToTotalHits( decodedHits );
  decodedData.addToActivePDs( nHPDbanks   );

  // debug printout
  _ri_debug << "Decoded " << boost::format("%2i") % (nHPDbanks[Rich::Rich1]+nHPDbanks[Rich::Rich2]);
  _ri_debug << " PDs from Level1 Bank ID = "
            << boost::format("%2i") % L1ID.data();
  _ri_debug << " : Size " << boost::format("%4i") % (bank.size()/4) << " words : Version "
            << version << endmsg;

}

//=============================================================================

void RawBankDecoder::decodeToSmartIDs_2006TB( const LHCb::RawBank & bank,
                                              L1Map & decodedData ) const
{

  // Get L1 ID
  const Level1HardwareID L1ID ( bank.sourceID() );

  // counts
  DetectorArray<unsigned int> nHPDbanks{{0,0}}, decodedHits{{0,0}};

  // Data bank size in words
  const auto bankSize = bank.size() / 4;

  // Get bank version
  const auto version = bankVersion( bank );

  if ( bankSize > 0 )
  {

    // ... otherwise, must have at least 2 entries
    if ( bankSize < 2 )
    {
      Exception( "Non-empty RICH Bank size is less than 2 !" );
    }

    // Get Ingress map to decode into for this L1 board
    decodedData.emplace_back( L1ID, IngressMap() );
    auto & ingressMap = decodedData.back().second;

    // This data version does not have ingress info, so just put all data into ingress 0
    const L1IngressID ingressNum(0);

    // Get data for this ingress
    if ( ingressMap.empty() ) { ingressMap.emplace_back( ingressNum, IngressInfo() ); }
    auto & ingressInfo = ingressMap.back().second;

    // Make up L1 input numbers
    Level1Input l1Input(0);

    // Loop over bank, find headers and produce a data bank for each
    // Fill data into RichSmartIDs
    int lineC(0);
    while ( lineC < bankSize )
    {

      // Create data bank and decode into RichSmartIDs
      const auto hpdBank ( createDataBank( &bank.data()[lineC], // pointer to start of data
                                           0, // Not needed here (to be removed). Must be 0 though
                                           version ) );

      // get HPD RichSmartID
      const LHCb::RichSmartID hpdID = ( m_useFakeHPDID ? s_fakeHPDID :
                                        m_richSys->richSmartID( hpdBank->level0ID() ) );

      // decode to smartIDs
      ingressInfo.pdData().emplace_back( l1Input, PDInfo() );
      auto & hpdInfo = ingressInfo.pdData().back().second;
      hpdInfo.setPdID(hpdID);
      ++l1Input;
      auto & newids = hpdInfo.smartIDs();
      const auto hpdHitCount = hpdBank->fillRichSmartIDs( newids, hpdID );

      // Do data integrity checks
      const bool OK = ( !m_checkDataIntegrity || hpdBank->checkDataIntegrity(newids,this) );
      if ( !OK || msgLevel(MSG::VERBOSE) )
      {
        // printout decoded RichSmartIDs
        verbose() << " Decoded RichSmartIDs :-" << endmsg;
        for ( const auto& ID : newids ) { verbose() << "   " << ID << endmsg; }
      }
      if ( !OK && m_purgeHPDsFailIntegrity ) { newids.clear(); }

      // is data OK
      if ( OK )
      {

        // apply suppression of high occupancy HPDs
        if ( hpdHitCount < m_maxHPDOc )
        {
          const auto rich = hpdID.rich();
          ++nHPDbanks[rich];
          decodedHits[rich] += hpdHitCount;
        }
        else
        {
          std::ostringstream hpd;
          hpd << hpdID.panelID();
          Warning( "Forced suppression of HPD "+hpd.str(), StatusCode::SUCCESS, 0 ).ignore();
          newids.clear();
        }

      }
      else if ( m_verboseErrors )
      {
        // decoding error ....
        error() << "Corruption in decoding -> Data is rejected for HPD " << hpdID << endmsg;

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
  decodedData.addToActivePDs( nHPDbanks   );

  // debug printout
  _ri_debug << "Decoded " << boost::format("%2i") % (nHPDbanks[Rich::Rich1]+nHPDbanks[Rich::Rich2]);
  _ri_debug << " PDs from Level1 Bank "
            << boost::format("%2i") % L1ID.data();
  _ri_debug << " : Size " << boost::format("%4i") % (bank.size()/4) << " words : Version "
            << version << endmsg;

}

//=============================================================================

void RawBankDecoder::decodeToSmartIDs_DC0406( const LHCb::RawBank & bank,
                                              L1Map & decodedData ) const
{

  // Get L1 ID
  const Level1HardwareID base_L1ID ( bank.sourceID() );

  // Get max data size for LHCb mode
  const ShortType maxDataSize = MaxDataSize;

  // HPD count
  DetectorArray<unsigned int> nHPDbanks{{0,0}}, decodedHits{{0,0}};

  // Data bank size in words
  const auto bankSize = bank.size() / 4;

  // Get bank version
  const auto version = bankVersion( bank );

  // Header type
  typedef RichDAQHeaderV1::RichDAQHeaderPD MyHeader;

  // Is this an empty bank ?
  if ( bankSize > 0 )
  {

    // ... otherwise, must have at least 2 entries
    if ( bankSize < 2 )
    {
      Exception( "Non-empty RICH Bank size is less than 2 !" );
    }

    // Make up L1 input numbers when using fake HPDIDs
    Level1Input fake_l1Input(0);

    // Loop over bank, find headers and produce a data bank for each
    // Fill data into RichSmartIDs
    int lineC(0);
    while ( lineC < bankSize )
    {

      // Find HPD bank start
      const MyHeader header ( &bank.data()[lineC] );

      // Is this a true header
      if ( header.startPD() )
      {

        _ri_verbo << " Found HPD header at line " << lineC << " of " << bankSize << endmsg;

        // Store start line for header
        const int lineHeader = lineC;

        // Find last line of block
        int lineLast = lineC;
        if ( header.zeroSuppressed() )
        {
          // For ZS blocks, have to search for the hext header to define the block length

          bool cont = true;
          while ( cont && lineC < bankSize )
          {
            ++lineC;
            // Test if this is the last line of data or a new header word
            if ( lineC == bankSize || MyHeader(&bank.data()[lineC]).startPD() )
            {
              lineLast = lineC-1;
              cont = false;
            }
          }

          _ri_verbo << "  -> Bank is zero surpressed : ends at " << lineLast << endmsg;

        }
        else
        {
          // non-ZS blocks have fixed length, so skip straight to the end

          lineC   += 1 + maxDataSize; // data block + header
          lineLast = lineC - 1;

          _ri_verbo << "  -> Bank is non zero surpressed : ends at " << lineLast << endmsg;

        }

        // Check data size
        const unsigned int dataSize = lineLast-lineHeader;
        if ( dataSize < 1 )
        {
          std::ostringstream message;
          message << "Invalid HPD data block size : " << dataSize;
          Exception( message.str() );
        }

        // Create data bank and decode into RichSmartIDs
        const auto hpdBank ( createDataBank( &bank.data()[lineHeader],
                                             dataSize,
                                             version ) );

        // get HPD RichSmartID
        const LHCb::RichSmartID hpdID = ( m_useFakeHPDID ? s_fakeHPDID :
                                          m_richSys->richSmartID( hpdBank->level0ID() ) );

        // L1 ID
        const auto L1ID = m_richSys->level1HardwareID(hpdID);
        if ( L1ID != base_L1ID )
        {
          error() << "L1ID Mis-match" << endmsg;
          error() << "  -> base : " << base_L1ID << endmsg;
          error() << "  -> HPD  : " << L1ID << endmsg;
        }

        // Get Ingress map to decode into for this L1 board
        decodedData.emplace_back( L1ID, IngressMap() );
        auto & ingressMap = decodedData.back().second;

        // L1 input number
        const Level1Input l1Input = ( m_useFakeHPDID ?
                                      fake_l1Input :
                                      m_richSys->level1InputNum(hpdID) );
        if ( m_useFakeHPDID ) ++fake_l1Input;

        // Ingress info
        ingressMap.emplace_back( l1Input.ingressID(), IngressInfo() );
        auto & ingressInfo = ingressMap.back().second;

        // get HPD data
        ingressInfo.pdData().emplace_back( l1Input, PDInfo() );
        auto & hpdInfo = ingressInfo.pdData().back().second;
        hpdInfo.setPdID(hpdID);
        auto & newids = hpdInfo.smartIDs();

        // get hit count
        const auto hpdHitCount = hpdBank->fillRichSmartIDs( newids, hpdID );

        // apply suppression of high occupancy HPDs
        if ( hpdHitCount < m_maxHPDOc )
        {
          const auto rich = hpdID.rich();
          ++nHPDbanks[rich];
          decodedHits[rich] += hpdHitCount;
        }
        else
        {
          std::ostringstream hpd;
          hpd << hpdID.panelID();
          Warning( "Suppressed HPD in "+hpd.str(), StatusCode::SUCCESS, 0 ).ignore();
          newids.clear();
        }

      }
      else // Not a data header line
      {
        ++lineC;
      }

    } // bank while loop

  } // data bank not empty

  // Add to the total number of decoded hits
  decodedData.addToTotalHits( decodedHits );
  decodedData.addToActivePDs( nHPDbanks   );

  // debug printout
  _ri_debug << "Decoded " << boost::format("%2i") % (nHPDbanks[Rich::Rich1]+nHPDbanks[Rich::Rich2]);
  _ri_debug << " PDs from Level1 Bank "
            << boost::format("%2i") % base_L1ID.data();
  _ri_debug << " : Size " << boost::format("%4i") % (bank.size()/4) << " words : Version "
            << version << endmsg;

}

//=============================================================================

void
RawBankDecoder::decodeToSmartIDs_MaPMT0( const LHCb::RawBank & bank,
                                         L1Map & decodedData ) const
{
  // Get L1 ID
  const Level1HardwareID L1ID ( bank.sourceID() );
  _ri_debug << "FlatList Decoding L1 bank " << L1ID << endmsg;

  // Data bank size in 32 bit words
  const auto bankSize = bank.size() / 4;

  // various counts
  DetectorArray<unsigned int> decodedHits{{0,0}};

  // If we have some words to process, start the decoding
  if ( bankSize > 0 )
  {

    // Get Ingress map to decode into for this L1 board
    decodedData.emplace_back( std::piecewise_construct, 
                              std::forward_as_tuple(L1ID), 
                              std::forward_as_tuple() );
    auto & ingressMap = decodedData.back().second;

    // reserve size
    ingressMap.reserve( Rich::DAQ::NumIngressPerL1 );

    // cache the last PD info
    PDInfo * last_pdInfo = nullptr;
    LHCb::RichSmartID last_pdID;

    // Loop over bank, Fill data into RichSmartIDs
    int lineC(0);
    while ( lineC < bankSize )
    {
      // Read the smartID direct from the banks
      const LHCb::RichSmartID id( LHCb::RichSmartID32( bank.data()[lineC++] ) );
      if ( !id.isValid() )
      {
        Warning( "Invalid RichSmartID read from FlatList data format" ).ignore();
      }
      else
      {
        _ri_debug << " -> " << id << endmsg;

        // Get the L1 input from the DB
        const auto l1Input = m_richSys->level1InputNum(id);

        // The ingress info to fill
        IngressInfo * ingressInfo = nullptr;

        // Do we have an entry for this Ingress ID ?
        const auto inIt = std::find_if( ingressMap.begin(), ingressMap.end(),
                                        [&l1Input]( const auto & i )
                                        { return l1Input.ingressID() == i.first; } );
        if ( UNLIKELY( inIt == ingressMap.end() ) )
        {
          ingressMap.emplace_back( std::piecewise_construct, 
                                   std::forward_as_tuple(l1Input.ingressID()), 
                                   std::forward_as_tuple() );
          ingressInfo = &(ingressMap.back().second);
          L1IngressHeader iHeader;
          iHeader.setIngressID( l1Input.ingressID() );
          ingressInfo->setIngressHeader(iHeader);
          // reserve size (guess as we don't know here...)
          ingressInfo->pdData().reserve(32);
        }
        else
        {
          // use existing entry
          ingressInfo = &(inIt->second);
        }

        // the PD data map
        auto & pdMap = ingressInfo->pdData();

        // The PD info object to fill
        PDInfo * pdInfo = nullptr;

        // The PD ID
        const auto pdID = id.pdID();

        // The RICH
        const auto rich = id.rich();

        // Has PD changed ?
        if ( pdID != last_pdID || !last_pdInfo )
        {

          // Does this PD have an entry ?
          const auto pdIt = std::find_if( pdMap.begin(), pdMap.end(),
                                          [&l1Input]( const auto& i ) 
                                          { return l1Input == i.first; } );
          if ( UNLIKELY( pdIt == pdMap.end() ) )
          {
            // make a new entry
            pdMap.emplace_back( std::piecewise_construct, 
                                std::forward_as_tuple( l1Input ),
                                std::forward_as_tuple(         ) );
            pdInfo = &(pdMap.back().second);
            // Set the PD ID
            pdInfo->setPdID( pdID );
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
            pdInfo->setHeader( header );
            // reserve size (guess) in hit vector
            pdInfo->smartIDs().reserve(16);
            // Add to active PD count for current rich
            decodedData.addToActivePDs( rich );
          }
          else
          {
            // use found entry
            pdInfo = &(pdIt->second);
          }

          // update the PD cache
          last_pdID   = pdID;
          last_pdInfo = pdInfo;

        }
        else
        {
          // use last PD cache
          pdInfo = last_pdInfo;
        }

        // add the hit to the list
        pdInfo->smartIDs().emplace_back( id );

        // count the hits 
        ++decodedHits[rich];

      }
    }

  } // bank not empty

  // Add to the total number of decoded hits
  decodedData.addToTotalHits( decodedHits );

}

//=============================================================================

void RawBankDecoder::suppressHotPixels( const LHCb::RichSmartID& hpdID,
                                        LHCb::RichSmartID::Vector & newids ) const
{
  // clean out hot pixels enabled at all ?
  if ( m_pixelsToSuppress )
  {
    // Does this HPD have some pixels to suppress
    const auto iHPDSup = m_hotPixels.find(hpdID);
    if ( iHPDSup != m_hotPixels.end() )
    {
      // temporary copy of original hit channels
      const auto oldids = newids;

      // clear hits before suppression
      newids.clear();

      // loop over original hits and save only those not supressed
      for ( const auto& ID : oldids )
      {
        if ( iHPDSup->second.find(ID) == iHPDSup->second.end() )
        {
          // not suppressed, so keep
          newids.push_back(ID);
        }
        else
        {
          // Get detector info
          const auto l0ID    = m_richSys->level0ID(hpdID);
          const auto l1ID    = m_richSys->level1HardwareID(hpdID);
          const auto l1Input = m_richSys->level1InputNum(hpdID);
          // Print warning
          std::ostringstream mess;
          mess << "L1HardID=" << l1ID << " L1Input=" << l1Input
               << " L0ID=" << l0ID << " " << ID << " is software SUPPRESSED";
          Warning( mess.str(), StatusCode::SUCCESS ).ignore();
        }

      } // loop over pixels

    } // this HPD has pixels to suppress

  } // hot pixel suppression enabled

}

//=============================================================================

void RawBankDecoder::dumpRawBank( const LHCb::RawBank & bank,
                                  MsgStream & os ) const
{

  // Get bank version and ID
  const Level1HardwareID L1ID ( bank.sourceID() );
  const auto version = bankVersion( bank );

  // Data bank size in words
  const auto bankSize = bank.size() / 4;

  std::ostringstream magicAsHex;
  magicAsHex << std::hex << bank.magic();
  os << "RawBank version=" << version << " L1ID=" << L1ID << " datasize=" << bankSize
     << " magic=" << magicAsHex.str() << endmsg;

  // Printout raw data

  // Is this an empty bank ?
  if ( bankSize > 0 )
  {
    const std::string & LINES =
      "----------------------------------------------------------------------------------------------------------------";

    // Bit numbers
    os << LINES << endmsg;
    os << "      bit |";
    for ( int iCol = 31; iCol >= 0; --iCol )
    {
      os << boost::format("%3i") % iCol;
    }
    os << endmsg;
    os << LINES << endmsg;
    typedef unsigned int Dtype;
    for ( const Dtype * dataW = bank.begin<Dtype>(); dataW != bank.end<Dtype>(); ++dataW )
    {
      rawDump( os, *dataW );
      os << endmsg;
    }
    os << LINES << endmsg;

  }
  else
  {
    os << "  -> Bank is empty" << endmsg;
  }

}

//=============================================================================

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( RawBankDecoder )

//=============================================================================
