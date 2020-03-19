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
#include "SmartVeloErrorBankDecoder.h"

//-----------------------------------------------------------------------------
// Implementation file for class : SmartVeloErrorBankDecoder
//
// 2008-08-24 : Tomasz Szumlak
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( SmartVeloErrorBankDecoder )

using namespace VeloTELL1;

//=============================================================================
// Initialization
//=============================================================================
StatusCode SmartVeloErrorBankDecoder::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;              // error printed already by GaudiAlgorithm

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

  // Initialise the RawEvent locations
  bool usingDefaultLocation = m_rawEventLocations.empty();

  if ( std::find( m_rawEventLocations.begin(), m_rawEventLocations.end(), LHCb::RawEventLocation::Default ) ==
       m_rawEventLocations.end() ) {
    // append the defaults to the search path
    m_rawEventLocations.value().push_back( LHCb::RawEventLocation::Other );
    m_rawEventLocations.value().push_back( LHCb::RawEventLocation::Default );
  }

  if ( !usingDefaultLocation ) {
    info() << "Using '" << m_rawEventLocations << "' as search path for the RawEvent object" << endmsg;
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode SmartVeloErrorBankDecoder::execute() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;
  //
  StatusCode rawEvtStatus = getRawEvent();
  StatusCode bankStatus;
  if ( rawEvtStatus.isSuccess() ) bankStatus = cacheErrorRawBanks();
  if ( bankStatus.isSuccess() ) {
    bankStatus = storeErrorRawBanks();
  } else {
    // go to the next event
    bankStatus = StatusCode::SUCCESS;
  }

  if ( !bankStatus.isSuccess() ) {
    warning() << "storing error banks aborted (possible raw bank corruption)." << endmsg;
    bankStatus = StatusCode::SUCCESS;
  }
  //
  return ( bankStatus );
}

//=============================================================================
StatusCode SmartVeloErrorBankDecoder::getRawEvent() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << " ==> getRawEvent() " << endmsg;
  if ( msgLevel( MSG::DEBUG ) ) debug() << "--------------------" << endmsg;
  //

  // Retrieve the RawEvent:
  m_rawEvent = nullptr;
  for ( const auto& p : m_rawEventLocations ) {
    m_rawEvent = getIfExists<LHCb::RawEvent>( p );
    if ( m_rawEvent ) {
      if ( msgLevel( MSG::DEBUG ) ) debug() << " ==> The RawEvent has been read-in from location: " << p << endmsg;
      break;
    }
  }

  if ( !m_rawEvent ) {
    error() << " ==> There is no RawEvent at: " << m_rawEventLocations << endmsg;
    return ( StatusCode::FAILURE );
  }

  //
  return ( StatusCode::SUCCESS );
}
//=============================================================================
StatusCode SmartVeloErrorBankDecoder::cacheErrorRawBanks() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << " ==> cacheErrorRawBanks() " << endmsg;
  // check if there is error bank present
  m_cachedBanks.clear();
  m_bankLength.clear();
  const auto& errorBank = m_rawEvent->banks( LHCb::RawBank::VeloError );
  // if so write out the banks
  if ( errorBank.size() != 0 ) {
    m_errorBank = new VeloErrorBanks();
    //
    if ( msgLevel( MSG::DEBUG ) ) debug() << " --> Error bank detected of size: " << errorBank.size() << endmsg;
    ITPair data;
    for ( const LHCb::RawBank* aBank : errorBank ) {

      // --> protect against corrupted banks
      if ( LHCb::RawBank::MagicPattern != aBank->magic() ) return ( StatusCode::FAILURE );

      // get the sensor number == sourceID if there is no full data
      unsigned int tell1  = aBank->sourceID();
      m_bankLength[tell1] = aBank->size();
      m_bankVersion       = aBank->version();
      m_bankType          = aBank->type();
      m_magicPattern      = aBank->magic();
      if ( m_printBankHeader.value() ) {
        info() << " --> src Id: " << tell1 << endmsg;
        info() << " --> type of bank: " << ( aBank->typeName() ) << endmsg;
        info() << " --> bank header size: " << ( aBank->hdrSize() ) << endmsg;
        info() << " --> body size: " << ( aBank->size() ) << endmsg;
        info() << " --> total size: " << ( aBank->totalSize() ) << endmsg;
      }
      // get pointer to the bank data
      data.first               = aBank->begin<unsigned int>();
      const unsigned int* inIT = aBank->begin<unsigned int>();
      const unsigned int* dat  = aBank->data();
      if ( msgLevel( MSG::DEBUG ) ) debug() << " --> src Id: " << tell1 << endmsg;
      for ( int step = 0; step < 6; ++step ) {
        if ( msgLevel( MSG::DEBUG ) ) debug() << " data ptr: " << step << " : " << ( *( inIT + step ) ) << endmsg;
        if ( msgLevel( MSG::DEBUG ) ) debug() << " data tab: " << step << " : " << ( *( dat + step ) ) << endmsg;
      }
      data.second          = aBank->end<unsigned int>();
      m_cachedBanks[tell1] = data;
      int dist             = std::distance( data.first, data.second );
      if ( msgLevel( MSG::DEBUG ) ) debug() << " --> bank body size: " << ( dist * sizeof( unsigned int ) ) << endmsg;
    }
  } else {
    Info( " --> No error bank detected - skipping to the next event " ).ignore();
    return ( StatusCode::FAILURE );
  }
  if ( msgLevel( MSG::DEBUG ) )
    debug() << " --> cached error bank strucure's size:" << ( m_cachedBanks.size() ) << endmsg;
  return ( StatusCode::SUCCESS );
}
//=============================================================================
StatusCode SmartVeloErrorBankDecoder::storeErrorRawBanks() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << " ==> storeErrorRawBanks() " << endmsg;
  //
  for ( auto bankIT = m_cachedBanks.begin(); bankIT != m_cachedBanks.end(); ++bankIT ) {
    EvtInfo  anEvtInfo( bankIT->first );
    allEvt   evtInfoData;
    allError errorInfoData;
    dataVec  sources;
    // get information on error sources
    SECTORS sectors = errorDetector( bankIT->first );
    if ( sectors.size() != SOURCES ) {
      if ( msgLevel( MSG::DEBUG ) )
        debug() << " --> Error detected for " << ( sectors.size() ) << " PPFPGA(s) " << endmsg;
      warning() << "Impossible number of PPFPGAs: " << sectors.size() << endmsg;
      return StatusCode::FAILURE;
    }

    VeloErrorBank* err = new VeloErrorBank( bankIT->first );

    // store words in error bank
    SECTORS::iterator secIT = sectors.begin();
    for ( ; secIT != sectors.end(); ++secIT ) {
      // write fixed words
      errorInfo   anErrorInfo;
      ErrorBankIT begin = secIT->second.first - INIT_SHIFT;
      ErrorBankIT end   = ( secIT->second.first + 1 );
      for ( ; begin != end; ++begin ) { anErrorInfo.push_back( *begin ); }
      if ( secIT->second.second == 0 ) {
        anErrorInfo.push_back( *end );
        anErrorInfo.push_back( *( ++end ) );
      }
      errorInfoData.push_back( anErrorInfo );
      // write optional part
      evtInfo anInfo;
      if ( secIT->second.second != 0 ) {
        // error was sent for this fpga build evt info section
        // encode properly the evt section block
        sources.push_back( secIT->first );
        ErrorBankIT begin = secIT->second.first - INIT_SHIFT;
        anInfo.push_back( *begin );
        anInfo.push_back( *( ++begin ) );
        unsigned int word = ( *( ++begin ) );
        word              = ( word >> bitShift16 ) & bitMask16;
        anInfo.push_back( word );
        begin           = secIT->second.second;
        ErrorBankIT end = begin + EVT_WORDS;
        for ( ; begin != end; ++begin ) { anInfo.push_back( *begin ); }
      }
      evtInfoData.push_back( anInfo );
    }
    anEvtInfo.setEvtInfo( evtInfoData );
    err->setEvtInfoSection( std::move( anEvtInfo ) );
    err->setErrorInfoSection( errorInfoData );
    if ( msgLevel( MSG::DEBUG ) )
      debug() << " bank lenght: " << m_bankLength[bankIT->first] << " for tell1: " << ( bankIT->first ) << endmsg;
    err->setBankLength( m_bankLength[bankIT->first] );
    err->setBankType( m_bankType );
    err->setBankVersion( m_bankVersion );
    err->setMagicPattern( m_magicPattern );
    if ( msgLevel( MSG::DEBUG ) ) debug() << " --> Detected errors: " << ( sources.size() ) << endmsg;
    err->setErrorSources( sources );
    m_errorBank->insert( err );
  }
  //
  m_errorBankLoc.put( m_errorBank );
  //
  return ( StatusCode::SUCCESS );
}
//=============================================================================
SmartVeloErrorBankDecoder::SECTORS SmartVeloErrorBankDecoder::errorDetector( unsigned int tell1 ) {
  if ( msgLevel( MSG::DEBUG ) ) debug() << " ==> errorDetector() " << endmsg;
  //
  unsigned int fpga = 0;
  SECTORS      sectors;
  auto         bankIT = m_cachedBanks.find( tell1 );
  if ( bankIT != m_cachedBanks.end() ) {
    auto        iTPair = bankIT->second;
    ErrorBankIT iT     = iTPair.first;
    // we can skip first four words - see note EDMS 694818
    iT += INIT_SHIFT;
    // check if the file has expected format
    ITPair aPair( 0, 0 );
    for ( ; iT != iTPair.second; ++iT ) {
      if ( ( ( *iT & bitMask8 ) == EBMarker0 ) && ( ( ( *iT >> bitShift8 ) & bitMask8 ) == EBMarker ) ) {
        // cache this iterator
        aPair.first = iT;
        if ( ( ( *iT >> bitShift16 ) & bitMask16 ) == ERROR_BANK_SIZE ) {
          // cache iterator to the first word
          aPair.second = ( iT + 1 );
        } else {
          // store nothing
          aPair.second = 0;
        }
        sectors[fpga] = aPair;
        ++fpga;
      }
    }
  }
  //
  return ( sectors );
}
//--
