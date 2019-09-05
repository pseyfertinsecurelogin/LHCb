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

#include "Event/CaloAdc.h"
#include "Event/L0CaloAdc.h"

// local
#include "CaloFutureFillRawBuffer.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CaloFutureFillRawBuffer
//
// 2004-12-17 : Olivier Callot
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( CaloFutureFillRawBuffer )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CaloFutureFillRawBuffer::CaloFutureFillRawBuffer( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {
  //=== Default values according to the name of the algorithm !
  if ( "Ecal" == name.substr( 0, 4 ) ) {
    m_detectorName     = "Ecal";
    m_detectorLocation = DeCalorimeterLocation::Ecal;
    m_inputBank        = LHCb::CaloAdcLocation::Ecal;
    m_triggerBank      = LHCb::L0CaloAdcLocation::Ecal;
    m_bankType         = LHCb::RawBank::EcalE;
    m_triggerBankType  = LHCb::RawBank::EcalTrig;
  } else if ( "Hcal" == name.substr( 0, 4 ) ) {
    m_detectorName     = "Hcal";
    m_detectorLocation = DeCalorimeterLocation::Hcal;
    m_inputBank        = LHCb::CaloAdcLocation::Hcal;
    m_triggerBank      = LHCb::L0CaloAdcLocation::Hcal;
    m_bankType         = LHCb::RawBank::HcalE;
    m_triggerBankType  = LHCb::RawBank::HcalTrig;
  }

  declareProperty( "InputBank", m_inputBank );

  declareProperty( "FillWithPin", m_pin = false );
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode CaloFutureFillRawBuffer::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;              // error printed already by GaudiAlgorithm

  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Initialize" << endmsg;

  if ( "None" == m_detectorName ) { return Error( "Invalid algorithm name" + name(), StatusCode::FAILURE ); }

  m_calo = getDet<DeCalorimeter>( m_detectorLocation );

  if ( 2 == m_dataCodingType ) {
    //== TELL1 coding format: packed data, starting from Full banks
    m_numberOfBanks = m_calo->nTell1s();
    if ( "Ecal" == m_detectorName ) {
      m_bankType  = LHCb::RawBank::EcalPacked;
      m_inputBank = LHCb::CaloAdcLocation::FullEcal;
    } else {
      m_bankType  = LHCb::RawBank::HcalPacked;
      m_inputBank = LHCb::CaloAdcLocation::FullHcal;
    }
    info() << "Processing " << m_calo->nCards() << " FE-Cards and " << m_calo->nTell1s() << " TELL1" << endmsg;
  }

  m_nbEvents    = 0;
  m_totDataSize = 0;
  m_totTrigSize = 0;

  std::vector<unsigned int> a;
  a.reserve( 500 );
  for ( int kk = 0; m_numberOfBanks > kk; kk++ ) {
    m_banks.push_back( a );
    m_trigBanks.push_back( a );
    m_dataSize.push_back( 0. );
  }

  info() << "Data coding type " << m_dataCodingType << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode CaloFutureFillRawBuffer::execute() {

  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Execute" << endmsg;

  for ( int kk = 0; m_numberOfBanks > kk; kk++ ) {
    m_banks[kk].clear();
    m_trigBanks[kk].clear();
  }

  //== Build the data banks

  switch ( m_dataCodingType ) {
    case 1 : {
    //=========================================================================
    //  Fill the calorimeter data bank, simple structure: ID (upper 16 bits) + ADC
    //=========================================================================
    LHCb::CaloAdcs*                digs = get<LHCb::CaloAdcs>( m_inputBank );
    std::transform( digs->begin(), digs->end(), std::back_inserter( m_banks[0] ),
                     [](const auto& dig) {
      return ( dig->cellID().all() << 16 ) | ( dig->adc() & 0xFFFF );
    } );
    break;
    }
    case 2 :  {
    fillPackedBank();
    break;
    }
    default :  ASSUME(false);
  }

  //== Build the trigger banks

  if ( 1 == m_dataCodingType ) fillTriggerBank();

  int totDataSize = 0;
  int totTrigSize = 0;

  LHCb::RawEvent* rawEvent = get<LHCb::RawEvent>( LHCb::RawEventLocation::Default );
  for ( unsigned int kk = 0; m_banks.size() > kk; kk++ ) {
    rawEvent->addBank( kk, m_bankType, m_dataCodingType, m_banks[kk] );
    totDataSize += m_banks[kk].size();
    m_dataSize[kk] += m_banks[kk].size();
    if ( 1 == m_dataCodingType ) {
      rawEvent->addBank( kk, m_triggerBankType, 0, m_trigBanks[kk] );
      totTrigSize += m_trigBanks[kk].size();
    }
  }

  m_totDataSize += totDataSize;
  m_totTrigSize += totTrigSize;
  m_nbEvents++;

  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) {
    debug() << "Bank sizes: ";
    for ( unsigned int kk = 0; m_banks.size() > kk; kk++ ) {
      debug() << format( "%2d:%4d+%4d ", kk, m_banks[kk].size(), m_trigBanks[kk].size() );
    }
    debug() << endmsg << "Total Data bank size " << totDataSize << " + trigger " << totTrigSize << endmsg;
  }

  if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) ) {
    for ( unsigned int kk = 0; m_banks.size() > kk; kk++ ) {
      verbose() << "DATA bank : " << kk << endmsg;
      int                                       kl = 0;

      for ( auto itW : m_banks[kk] ) {
        verbose() << format( " %8x %11d   ", itW , itW );
        kl++;
        if ( 0 == kl % 4 ) verbose() << endmsg;
      }

      verbose() << endmsg << "TRIGGER bank size=" << m_trigBanks[kk].size() << "  " << endmsg;
      kl = 0;
      for ( auto itW : m_trigBanks[kk] ){
        verbose() << format( " %8x ", itW );
        kl++;
        if ( 0 == kl % 8 ) verbose() << endmsg;
      }
      verbose() << endmsg;
    }
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode CaloFutureFillRawBuffer::finalize() {

  if ( 0 < m_nbEvents ) {
    m_totDataSize /= m_nbEvents;
    m_totTrigSize /= m_nbEvents;
    info() << "Average event size : " << format( "%7.1f words, %7.1f for trigger", m_totDataSize, m_totTrigSize );
    double meanSize = 0.;
    double maxSize  = 0.;
    for ( auto& ds : m_dataSize ) {
      ds /= m_nbEvents;
      meanSize += ds;
      if ( maxSize < ds ) maxSize = ds;
    }
    meanSize /= m_dataSize.size();
    info() << format( "  Mean bank size %7.1f, maximum size %7.1f", meanSize, maxSize ) << endmsg;
  }

  return GaudiAlgorithm::finalize(); // must be called after all other actions
}

//=========================================================================
//  Packed data format, trigger and data in the same bank. Process ALL digits
//=========================================================================
void CaloFutureFillRawBuffer::fillPackedBank() {
  LHCb::CaloAdcs*   digs     = get<LHCb::CaloAdcs>( m_inputBank );
  LHCb::L0CaloAdcs* trigAdcs = get<LHCb::L0CaloAdcs>( m_triggerBank );

  for ( int kTell1 = 0; m_numberOfBanks > kTell1; kTell1++ ) {
    for ( int cardNum : m_calo->tell1ToCards( kTell1 )) {
      if ( m_calo->isPinCard( cardNum ) && !m_pin ) continue; // No sub-bank for PIN-FEB if not explicitely requested
      int sizeIndex = m_banks[kTell1].size();
      m_banks[kTell1].push_back( m_calo->cardCode( cardNum ) << 14 );
      std::vector<LHCb::CaloCellID> ids = m_calo->cardChannels( cardNum );

      //=== The trigger part is first
      int patternIndex = m_banks[kTell1].size();
      int patTrig      = 0;
      m_banks[kTell1].push_back( patTrig );
      int word   = 0;
      int offset = 0;
      int bNum   = 0;
      for ( LHCb::CaloCellID id : ids ) {
        LHCb::L0CaloAdc* trig = trigAdcs->object( id.all() );
        if ( trig ) {
          patTrig |= 1 << bNum;
          int adc = trig->adc();
          if ( 24 < offset ) {
            m_banks[kTell1].push_back( word );
            offset = 0;
            word   = 0;
          }
          word |= adc << offset;
          offset += 8;
        }
        bNum++;
      }
      if ( 0 != offset ) { m_banks[kTell1].push_back( word ); }
      int sizeTrig = 4 * ( m_banks[kTell1].size() - patternIndex ); // in byte
      //== If no trigger at all, remove even the pattern...
      if ( 4 == sizeTrig ) {
        m_banks[kTell1].pop_back();
        sizeTrig = 0;
      } else {
        m_banks[kTell1][patternIndex] = patTrig;
      }

      //=== Now the ADCs
      patternIndex = m_banks[kTell1].size();
      int pattern  = 0;
      m_banks[kTell1].push_back( pattern );
      word   = 0;
      offset = 0;
      bNum   = 0;

      for ( LHCb::CaloCellID id : ids ) {
        LHCb::CaloAdc*   dig = digs->object( id );
        int   adc = ( dig ? std::clamp( dig->adc() + 256, 0, 4095 ) : 256 );//== Default if non existing cell.
        if ( 248 <= adc && adc < 264 ) { //... store short
          adc -= 248;
          word |= adc << offset;
          offset += 4;
        } else { //... store long
          word |= adc << offset;
          pattern += ( 1 << bNum );
          offset += 12;
        }
        if ( 32 <= offset ) { //== Have we a full word or more ? Store it
          m_banks[kTell1].push_back( word );
          offset -= 32;
          word = adc >> ( 12 - offset ); //== upper bits if needed
        }
        bNum++;
      }
      if ( 0 != offset ) m_banks[kTell1].push_back( word );

      int sizeAdc                   = 4 * ( m_banks[kTell1].size() - patternIndex );
      m_banks[kTell1][patternIndex] = pattern;

      m_banks[kTell1][sizeIndex] |= ( sizeAdc << 7 ) + sizeTrig;
      m_totTrigSize += sizeTrig;

      if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
        debug() << format( "Tell1 %2d card %3d pattern %8x patTrig %8x size Adc %2d Trig %2d", kTell1, cardNum, pattern,
                           patTrig, sizeAdc, sizeTrig )
                << endmsg;
    }
  }
}

//=========================================================================
//  Fill the CalorimeterFuture trigger banks
//=========================================================================
void CaloFutureFillRawBuffer::fillTriggerBank() {

  int prevIndx  = -1;
  int nextIndex = -1;
  int word      = -1;

  LHCb::L0CaloAdcs*                trigAdcs = get<LHCb::L0CaloAdcs>( m_triggerBank );
  for ( auto itT = trigAdcs->begin(); trigAdcs->end() != itT; ++itT ) {
    LHCb::CaloCellID id        = ( *itT )->cellID();
    int              cellIndex = id.all();
    if ( cellIndex != nextIndex ) {
      if ( 0 <= prevIndx ) m_trigBanks[prevIndx].push_back( word );
      word      = ( cellIndex << 16 ) + ( ( *itT )->adc() << 8 );
      nextIndex = cellIndex + 1;
    } else {
      word += ( *itT )->adc();
    }

    if ( MSG::VERBOSE >= msgLevel() ) {
      verbose() << id << format( "trigValue %3d word %8x ", ( *itT )->adc(), word ) << endmsg;
    }
    prevIndx = 0;
  }
  if ( 0 <= prevIndx ) m_trigBanks[prevIndx].push_back( word );
}
//=============================================================================
