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
#include <exception>
#include <stdexcept>

// from Gaudi
#include "GaudiAlg/Tuples.h"

// local
#include "PrepareVeloFullRawBuffer.h"

// data model
#include "Event/RawEvent.h"

// stl
#include <algorithm>

//-----------------------------------------------------------------------------

// Implementation file for class : PrepareVeloFullRawBuffer
//
// 2006-03-22 : Tomasz Szumlak
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( PrepareVeloFullRawBuffer )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PrepareVeloFullRawBuffer::PrepareVeloFullRawBuffer( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiTupleAlg( name, pSvcLocator )
    , m_rawEvent( 0 )
    , m_veloADCData( 0 )
    , m_veloADCPartialData( 0 )
    , m_veloPedestals( 0 )
    , m_veloPedestalsLoc( VeloFullBankLocation::Pedestals )
    , m_adcBankPresent( false )
    , m_pedBankPresent( false ) {

  declareProperty( "RunWithODIN", m_runWithODIN = true );
  declareProperty( "RawEventLocation", m_rawEventLoc = LHCb::RawEventLocation::Default );
  declareProperty( "ADCLocation", m_veloADCDataLoc = VeloFullBankLocation::Default );
  declareProperty( "ADCPartialLoc", m_veloADCPartialDataLoc = "Raw/Velo/PreparedPartialADC" );
  declareProperty( "RoundRobin", m_roundRobin = false );
  declareProperty( "IgnoreErrorBanks", m_ignoreErrorBanks = true );
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode PrepareVeloFullRawBuffer::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;              // error printed already by GaudiAlgorithm

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;
  //
  //  setHistoTopDir( "Vetra/" );
  return StatusCode::SUCCESS;
}
//=============================================================================
// Main execution
//=============================================================================
StatusCode PrepareVeloFullRawBuffer::execute() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;
  //
  StatusCode rawEvtStatus = getRawEvent();

  if ( rawEvtStatus.isSuccess() ) {

    getRawBanks();
    // if there is adc bank or pedestal bank begin ordering

    if ( adcBankFlag() || pedBankFlag() ) {

      if ( adcBankFlag() || pedBankFlag() ) createOrderedSections();
      writeVeloFull();
      // flush the memory after each event
      resetMemory();

    } else if ( m_roundRobin ) {

      writeVeloFull();
    }

  } else {

    return ( StatusCode::SUCCESS );
  }
  //
  if ( msgLevel( MSG::DEBUG ) ) debug() << " end of execute" << endmsg;

  return ( StatusCode::SUCCESS );
}

//=============================================================================
StatusCode PrepareVeloFullRawBuffer::getRawEvent() {
  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << " ==> getRawEvent() " << endmsg;
    debug() << "--------------------" << endmsg;
  }
  //
  // get the RawEvent from default TES location
  m_rawEvent = getIfExists<LHCb::RawEvent>( m_rawEventLoc );
  if ( NULL == m_rawEvent ) {
    return Error( " ==> There is no RawEvent at: " + m_rawEventLoc );
  } else {
    if ( msgLevel( MSG::DEBUG ) )
      debug() << " ==> The RawEvent has been read-in from location: " << m_rawEventLoc << endmsg;
  }
  //
  return ( StatusCode::SUCCESS );
}
//=============================================================================
StatusCode PrepareVeloFullRawBuffer::getRawBanks() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << " ==> getRawBanks() " << endmsg;
  // check if there is non-zero suppressed bank present
  const auto& fullBanks  = m_rawEvent->banks( LHCb::RawBank::VeloFull );
  const auto& errorBanks = m_rawEvent->banks( LHCb::RawBank::VeloError );

  // if so get all info needed for decoding
  if ( !fullBanks.empty() ) {

    setADCBankFlag();
    m_veloADCData        = new VeloFullBanks();
    m_veloADCPartialData = new VeloFullBanks();
    //

    for ( auto bIt = fullBanks.begin(); bIt != fullBanks.end(); bIt++ ) {

      const LHCb::RawBank* aBank = ( *bIt );
      // protection against wrong magic pattern

      if ( LHCb::RawBank::MagicPattern != aBank->magic() ) {

        info() << " --> Bank with source ID: " << ( aBank->sourceID() ) << " is corrupted! Skipping to the next event"
               << endmsg;
        return ( StatusCode::SUCCESS );
      }

      // get the sensor number == sourceID
      int sensor = aBank->sourceID();

      if ( !m_ignoreErrorBanks ) {

        // handle the data sent out together with an error bank properly
        auto isError = std::find_if( errorBanks.begin(), errorBanks.end(), errorBankFinder( sensor ) );
        if ( isError != errorBanks.end() ) {

          m_partialData2Decode[( aBank->sourceID() )] = {aBank->size(), aBank->data()};

          m_errorBanks2Check[( aBank->sourceID() )] = {( *isError )->size(), ( *isError )->data()};

        } else {

          m_fullData2Decode[( aBank->sourceID() )] = {aBank->size(), aBank->data()};
        }

      } else {

        // put all the nzs data in one container
        m_fullData2Decode[( aBank->sourceID() )] = {aBank->size(), aBank->data()};
      }
    }
  }

  // check if there is pedestal bank present
  const auto& pedBanks = m_rawEvent->banks( LHCb::RawBank::VeloPedestal );
  // if so get informations about it

  if ( !pedBanks.empty() ) {

    setPedBankFlag();
    m_veloPedestals = new VeloFullBanks();
    //

    if ( msgLevel( MSG::DEBUG ) ) debug() << "VeloPed bank detected of size: " << pedBanks.size() << endmsg;

    for ( const LHCb::RawBank* aBank : pedBanks ) {
      m_pedestalData2Decode[aBank->sourceID()] = {aBank->size(), aBank->data()};
    }
  }

  // check if there is ODIN bank present
  const auto& odinBank = m_rawEvent->banks( LHCb::RawBank::ODIN );
  // cache the bank if present

  if ( odinBank.empty() ) {

    if ( m_runWithODIN ) { return Error( " ==> ODIN bank missing!", StatusCode::FAILURE ); }
  }

  //
  if ( fullBanks.empty() ) {

    m_veloADCData         = new VeloFullBanks();
    m_veloADCPartialData  = new VeloFullBanks();
    unsigned int msgCount = 0;
    if ( msgLevel( MSG::DEBUG ) ) msgCount = 10;
    Warning( " --> There are no NZS banks!", StatusCode::SUCCESS, msgCount ).ignore();
  }
  //
  return StatusCode::SUCCESS;
}
//=============================================================================
// /// ///////////////////////////////////////////////////////////////// /// //
// if there are both banks type order the data and create new data object    //
// and pass it to the next stage of the processing                           //
// /// ///////////////////////////////////////////////////////////////// /// //
void PrepareVeloFullRawBuffer::createOrderedSections() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << " ==> createOrderedSection() " << endmsg;
  //
  if ( adcBankFlag() ) {

    int  type_1  = LHCb::RawBank::VeloFull;
    auto full_IT = m_fullData2Decode.begin();

    for ( ; full_IT != m_fullData2Decode.end(); ++full_IT ) {

      VeloFullBank* aBank = new VeloFullBank( ( *full_IT ).first, ( *full_IT ).second.second, type_1 );
      m_veloADCData->insert( aBank );
    }

    for ( auto part_IT = m_partialData2Decode.begin(); part_IT != m_partialData2Decode.end(); ++part_IT ) {

      if ( WORD2BYTE * FPGAx4 == ( *part_IT ).second.first ) {
        // if (msgLevel( MSG::DEBUG )) debug()<< " --> Will write partial data! " <<endmsg;
        // if (msgLevel( MSG::DEBUG )) debug()<< " --> source id: " << ((*part_IT).first) <<endmsg;
        info() << " --> Will write partial data! " << endmsg;
        info() << " --> source id: " << ( ( *part_IT ).first ) << endmsg;

        VeloFullBank* aBank = new VeloFullBank( ( *part_IT ).first, ( *part_IT ).second.second, type_1 );
        m_veloADCPartialData->insert( aBank );

      } else {

        // --> partial data decoding using information form the associated EB
      }
    }
  }
  //
  if ( pedBankFlag() ) {

    int type_2 = LHCb::RawBank::VeloPedestal;

    for ( auto ped_IT = m_pedestalData2Decode.begin(); ped_IT != m_pedestalData2Decode.end(); ++ped_IT ) {

      VeloFullBank* aBank = new VeloFullBank( ( *ped_IT ).first, ( *ped_IT ).second.second, type_2 );
      m_veloPedestals->insert( aBank );
    }
  }
}
//=============================================================================
StatusCode PrepareVeloFullRawBuffer::writeVeloFull() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << " ==> writeVeloFull() " << endmsg;
  //

  if ( adcBankFlag() || m_roundRobin ) {

    if ( msgLevel( MSG::DEBUG ) )
      debug() << "Registered container with bank data of size: " << m_veloADCData->size() << ", at" << m_veloADCDataLoc
              << endmsg;

    put( m_veloADCData, m_veloADCDataLoc );
    put( m_veloADCPartialData, m_veloADCPartialDataLoc );
  }
  //

  if ( pedBankFlag() ) {

    if ( msgLevel( MSG::DEBUG ) )
      debug() << "Registered container with bank data of size: " << m_veloPedestals->size() << ", at"
              << m_veloPedestalsLoc << endmsg;

    put( m_veloPedestals, m_veloPedestalsLoc );
  }
  //
  return ( StatusCode::SUCCESS );
}
//=============================================================================
void PrepareVeloFullRawBuffer::resetMemory() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << " ==> resetMemory() " << endmsg;
  //
  m_fullData2Decode.clear();
  m_partialData2Decode.clear();
  m_pedestalData2Decode.clear();
  m_errorBanks2Check.clear();

  if ( adcBankFlag() ) unsetADCBankFlag();
  if ( pedBankFlag() ) unsetPedBankFlag();
}
//=============================================================================
// Note only for use in emergency debugging (not referenced in code presently)
void PrepareVeloFullRawBuffer::dumpADCs( const dataVec& inADCs ) {
  info() << " Dumping ADC values for all channels (1 TELL1)" << endmsg;
  //
  cdatIt adcIt;
  int    ii = 0;
  for ( adcIt = inADCs.begin(); adcIt != inADCs.end(); adcIt++, ii++ ) {
    if ( !( ii % 32 ) ) { info() << " Beggining dumping ALink number: " << ii / 32 << endmsg; }
    //
    info() << "[Beetle, Channel] = "
           << "[" << ii / 128 << ", " << ii % 32 << "]" << ( *adcIt ) << std::endl;
  }
}
//=============================================================================
void PrepareVeloFullRawBuffer::setADCBankFlag() { m_adcBankPresent = true; }
//=============================================================================
void PrepareVeloFullRawBuffer::unsetADCBankFlag() { m_adcBankPresent = false; }
//=============================================================================
void PrepareVeloFullRawBuffer::setPedBankFlag() { m_pedBankPresent = true; }
//=============================================================================
void PrepareVeloFullRawBuffer::unsetPedBankFlag() { m_pedBankPresent = false; }
//=============================================================================
bool PrepareVeloFullRawBuffer::adcBankFlag() { return ( m_adcBankPresent ); }
//=============================================================================
bool PrepareVeloFullRawBuffer::pedBankFlag() { return ( m_pedBankPresent ); }
//=============================================================================
