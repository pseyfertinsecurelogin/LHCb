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
#include "DecodeVeloFullRawBuffer.h"

// Kernel
#include "Kernel/VeloEventFunctor.h"

// STL
#include <vector>

//-----------------------------------------------------------------------------
// Implementation file for class : DecodeVeloFullRawBuffer
//
// 2006-04-21 : Tomasz Szumlak
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( DecodeVeloFullRawBuffer )

//=============================================================================
// Initialization
//=============================================================================
StatusCode DecodeVeloFullRawBuffer::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;              // error printed already by GaudiAlgorithm

  debug() << "==> Initialize" << endmsg;

  // Check cable order configuration.  If the configuration is empty,
  // create a configuration with default order.  Otherwise perform
  // sanity checks and bail out when encountering nonsense.
  if ( m_cableOrder.empty() ) {
    for ( unsigned int i = 0; i < 4; ++i ) { m_cableOrder.value().push_back( i ); }
    std::reverse( m_cableOrder.begin(), m_cableOrder.end() );
  } else if ( 4 != m_cableOrder.size() ) {
    error() << "The cable order configuration must have exactly 4 entries." << endmsg;
    return StatusCode::FAILURE;
  } else {
    std::vector<unsigned int> used;
    for ( unsigned int i = 0; i < 4; ++i ) {
      if ( m_cableOrder[i] > 3 ) {
        error() << "Only cable positions 0-3 are allowed." << endmsg;
        return StatusCode::FAILURE;
      } else if ( used.end() != std::find( used.begin(), used.end(), m_cableOrder[i] ) ) {
        error() << "Each cable position must appear exactly once." << endmsg;
        return StatusCode::FAILURE;
      }
      used.push_back( m_cableOrder[i] );
    }
  }
  return StatusCode::SUCCESS;
}
//=============================================================================
// Main execution
//=============================================================================
StatusCode DecodeVeloFullRawBuffer::execute() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;
  //
  getData();
  // decode ADC, Pedestals and EvtInfos data
  decodeData();
  sortAndWriteDecodedData();
  resetMemory();
  //
  return StatusCode::SUCCESS;
}

//=============================================================================
StatusCode DecodeVeloFullRawBuffer::getData() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << " ==> getData() " << endmsg;
  //
  m_veloADCs = m_veloADCLocation.getIfExists();
  if ( !m_veloADCs ) {
    if ( msgLevel( MSG::DEBUG ) ) debug() << " ==> There is no data banks at: " << m_veloADCLocation.objKey() << endmsg;
  } else {
    // get the data banks from default TES location
    if ( msgLevel( MSG::DEBUG ) )
      debug() << " ==> The data banks have been read-in from location: " << m_veloADCLocation.objKey()
              << ", size of data container (number of read-out TELL1s): " << m_veloADCs->size() << endmsg;
    // create container for decoded data
    m_decodedADC    = new LHCb::VeloTELL1Datas();
    m_decodedHeader = new LHCb::VeloTELL1Datas();
    m_evtInfo       = new EvtInfos();
    //
    setADCDataFlag();
  }

  m_veloPartialADCs = m_veloPartialADCLocation.getIfExists();
  if ( !m_veloPartialADCs ) {

    if ( msgLevel( MSG::DEBUG ) )
      debug() << " ==> There is no data banks at: " << m_veloPartialADCLocation.objKey() << endmsg;

  } else {

    // get the partial data banks from default TES location
    if ( msgLevel( MSG::DEBUG ) )
      debug() << " ==> The data banks have been read-in from location: " << m_veloPartialADCLocation.objKey()
              << ", size of data container (number of read-out TELL1s): " << m_veloPartialADCs->size() << endmsg;

    // --> create container for decoded data
    m_decodedPartialADC = new LHCb::VeloTELL1Datas();
  }

  //
  m_veloPeds = m_veloPedLocation.getIfExists();
  if ( !m_veloPeds ) {
    if ( msgLevel( MSG::DEBUG ) ) debug() << " ==> There is no Pedestals at: " << m_veloPedLocation.objKey() << endmsg;
  } else {
    // get the pedestals banks from Pedestals TES location
    if ( msgLevel( MSG::DEBUG ) )
      debug() << " ==> The ped. banks have been read-in from location: " << m_veloPedLocation.objKey()
              << ", size of pedestals container (number of read-out TELL1s): " << m_veloPeds->size() << endmsg;
    // create container for decoded Ped
    m_decodedPed = new LHCb::VeloTELL1Datas();
    setPedDataFlag();
  }
  //
  return ( StatusCode::SUCCESS );
}
//=============================================================================
StatusCode DecodeVeloFullRawBuffer::decodeData() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << " ==> decodeData() " << endmsg;
  //
  if ( adcDataFlag() ) {
    // decode both ADC and ADCHeaders
    //
    for ( const auto& sens : *m_veloADCs ) {
      m_ADCDecoder.decode( sens, m_signADC );
      m_HeaderDecoder.decode( sens, m_signHeader );
      //
      LHCb::VeloTELL1Data* adcData    = new LHCb::VeloTELL1Data( ( sens )->key(), VeloFull );
      LHCb::VeloTELL1Data* headerData = new LHCb::VeloTELL1Data( ( sens )->key(), VeloHeader );
      EvtInfo*             anInfo     = new EvtInfo( ( sens )->key() );
      // data coming from TELL1 board is unsigned int
      // for the subsequent algorithms we required signed
      // values; change from unsigned int -> signed int
      // In the following we apply the cable reordering (sector correction)
      // is necessary for the NZS data to agree with the ZS data.
      // It corrects for the fact that the data cables between
      // repeater board and Tell1 have to be connected in reverse
      // to their numbering (data 0 -> tell1 input 3,data 1-> tell1 input 2 etc)

      if ( m_sectorCorrection.value() ) { // need to correct for wrong cabling
        int counter = 0;
        for ( scdatIt iT = m_signADC.begin(); iT != m_signADC.end(); ++iT ) {
          int channelposition = counter;
          channelposition     = m_cableOrder[channelposition / 512] * 512 + channelposition % 512;

          if ( msgLevel( MSG::DEBUG ) ) debug() << "ADCbanks: " << channelposition << " " << ( *iT ) << endmsg;

          m_signADCReordered[channelposition] = static_cast<signed int>( *iT );

          counter++;
        }
        counter = 0;
        for ( scdatIt iT = m_signHeader.begin(); iT != m_signHeader.end(); iT++ ) {
          int channelposition = counter;
          channelposition     = m_cableOrder[channelposition / 64] * 64 + channelposition % 64;

          if ( msgLevel( MSG::DEBUG ) ) debug() << "Header: " << channelposition << " " << ( *iT ) << endmsg;
          m_signHeaderReordered[channelposition] = ( static_cast<signed int>( *iT ) );
          counter++;
        }
        adcData->setDecodedData( m_signADCReordered );
        headerData->setDecodedData( m_signHeaderReordered );
      } else { // no cable reordering requested
        adcData->setDecodedData( m_signADC );
        headerData->setDecodedData( m_signHeader );
      }

      anInfo->setEvtInfo( sens->getEvtInfo() );
      //
      m_decodedADC->insert( adcData );
      m_decodedHeader->insert( headerData );
      m_evtInfo->insert( anInfo );
    }
  }

  if ( m_veloPartialADCs ) {
    // --> this part is responsible for the decoding of the partial data
    //     associated with an error bank

    for ( auto partIt = m_veloPartialADCs->begin(); partIt != m_veloPartialADCs->end(); ++partIt ) {

      m_ADCPartialDecoder.decode( *partIt, m_signPartialADC );
      LHCb::VeloTELL1Data* partData = new LHCb::VeloTELL1Data( ( *partIt )->key(), VeloFull );

      if ( m_sectorCorrection.value() ) { // need to correct for wrong cabling

        int     counter = 0;
        scdatIt pIT     = m_signPartialADC.begin();

        for ( ; pIT != m_signPartialADC.end(); ++pIT ) {

          int channelposition = counter;
          channelposition     = m_cableOrder[channelposition / 512] * 512 + channelposition % 512;
          m_signPartialADCReordered[channelposition] = static_cast<signed int>( *pIT );
          counter++;
        }

        partData->setDecodedData( m_signPartialADCReordered );

      } else { // --> no cable reordering requested

        partData->setDecodedData( m_signPartialADC );
      }

      m_decodedPartialADC->insert( partData );
    }
  }

  //
  if ( pedDataFlag() ) {
    for ( const auto& sens : *m_veloPeds ) {
      m_PedDecoder.decode( sens, m_signPed );
      auto pedData = std::make_unique<LHCb::VeloTELL1Data>( sens->key(), VeloPedestal );

      if ( m_sectorCorrection.value() ) { // need to correct for wrong cabling
        int counter = 0;
        for ( scdatIt iT = m_signPed.begin(); iT != m_signPed.end(); iT++ ) {
          int channelposition = counter;
          channelposition     = m_cableOrder[channelposition / 512] * 512 + channelposition % 512;
          if ( msgLevel( MSG::DEBUG ) ) debug() << "Ped bank: " << channelposition << " " << ( *iT ) << endmsg;
          m_signPedReordered[channelposition] = static_cast<signed int>( *iT );
          counter++;
        }
        pedData->setDecodedData( m_signPedReordered );
      } else { // no cable reordering requested
        pedData->setDecodedData( m_signPed );
      }
      m_decodedPed->insert( pedData.release() );
    }
  }
  //
  if ( ( !adcDataFlag() ) && ( !pedDataFlag() ) ) {
    if ( msgLevel( MSG::DEBUG ) ) debug() << " ==> No data decoded! " << endmsg;
    return ( StatusCode::SUCCESS );
  } else {
    return ( StatusCode::SUCCESS );
  }
}
//=============================================================================
void DecodeVeloFullRawBuffer::sortAndWriteDecodedData() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << " ==> writeDecodedData() " << endmsg;
  //
  if ( adcDataFlag() ) {
    // sort the output containers by key (TELL1 number)
    std::stable_sort( m_decodedADC->begin(), m_decodedADC->end(), VeloEventFunctor::Less_by_key );
    std::stable_sort( m_decodedHeader->begin(), m_decodedHeader->end(), VeloEventFunctor::Less_by_key );
    std::stable_sort( m_evtInfo->begin(), m_evtInfo->end(), VeloEventFunctor::Less_by_key );
    // put the data to TES
    m_decodedADCLocation.put( m_decodedADC );
    m_decodedHeaderLocation.put( m_decodedHeader );
    m_evtInfoLocation.put( m_evtInfo );
  }

  if ( m_decodedPartialADC ) {
    if ( !m_decodedPartialADC->empty() )
      if ( msgLevel( MSG::DEBUG ) ) debug() << " --> Write some data " << endmsg;
    m_decodedPartialADCLocation.put( m_decodedPartialADC );
  }

  if ( pedDataFlag() ) {
    // sort decoded pedestals
    std::stable_sort( m_decodedPed->begin(), m_decodedPed->end(), VeloEventFunctor::Less_by_key );
    // put decoded peds in TES
    m_decodedPedLocation.put( m_decodedPed );
  }
}
//=============================================================================
void DecodeVeloFullRawBuffer::setADCDataFlag() { m_adcDataPresent = true; }
//=============================================================================
void DecodeVeloFullRawBuffer::setPedDataFlag() { m_pedDataPresent = true; }
//=============================================================================
bool DecodeVeloFullRawBuffer::adcDataFlag() { return m_adcDataPresent; }
//=============================================================================
bool DecodeVeloFullRawBuffer::pedDataFlag() { return m_pedDataPresent; }
//=============================================================================
void DecodeVeloFullRawBuffer::unsetADCDataFlag() { m_adcDataPresent = false; }
//=============================================================================
void DecodeVeloFullRawBuffer::unsetPedDataFlag() { m_pedDataPresent = false; }
//
void DecodeVeloFullRawBuffer::resetMemory() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << " ==> resetMemory() " << endmsg;
  //
  unsetADCDataFlag();
  unsetPedDataFlag();
}
//
