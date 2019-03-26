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

#include "CaloTriggerAdcsFromRaw.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CaloTriggerAdcsFromRaw
//
// 2005-01-05 : Olivier Callot
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( CaloTriggerAdcsFromRaw )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CaloTriggerAdcsFromRaw::CaloTriggerAdcsFromRaw( const std::string& type, const std::string& name,
                                                const IInterface* parent )
    : base_class( type, name, parent ) {
  // clear data
  clear();
}

//=========================================================================
//  Initialise
//=========================================================================
StatusCode CaloTriggerAdcsFromRaw::initialize() {
  StatusCode sc = base_class::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;          // error printed already by GaudiTool

  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Initialize " << name() << endmsg;

  switch ( m_detectorName ) {
  case details::DetectorName_t::Ecal:
    m_calo       = getDet<DeCalorimeter>( DeCalorimeterLocation::Ecal );
    m_packedType = LHCb::RawBank::EcalPacked;
    m_shortType  = LHCb::RawBank::EcalTrig;
    m_errorType  = LHCb::RawBank::EcalPackedError;
    break;
  case details::DetectorName_t::Hcal:
    m_calo       = getDet<DeCalorimeter>( DeCalorimeterLocation::Hcal );
    m_packedType = LHCb::RawBank::HcalPacked;
    m_shortType  = LHCb::RawBank::HcalTrig;
    m_errorType  = LHCb::RawBank::HcalPackedError;
    break;
  default:
    error() << "Unknown detector name '" << toString( m_detectorName )
            << "'. Set it by option 'DetectorName', should be Ecal or Hcal" << endmsg;
    return StatusCode::FAILURE;
  }

  long nCells = m_calo->numberOfCells();
  long nPins  = m_calo->numberOfPins();
  m_data.reserve( nCells );
  m_pinData.reserve( nPins );
  clear();
  return StatusCode::SUCCESS;
}

//-------------------------------------
void CaloTriggerAdcsFromRaw::clear() {
  m_data.clear();
  m_pinData.clear();
  m_readSources.clear();
}

void CaloTriggerAdcsFromRaw::cleanData( int feb ) {
  if ( feb < 0 ) return;
  if ( m_calo->isPinCard( feb ) ) {
    m_pinData.erase( std::remove_if( m_pinData.begin(), m_pinData.end(),
                                     [&]( const LHCb::L0CaloAdc& adc ) {
                                       return m_calo->cellParam( adc.cellID() ).cardNumber() == feb;
                                     } ),
                     m_pinData.end() );
  } else {
    m_data.erase( std::remove_if( m_data.begin(), m_data.end(),
                                  [&]( const LHCb::L0CaloAdc& adc ) {
                                    return m_calo->cellParam( adc.cellID() ).cardNumber() == feb;
                                  } ),
                  m_data.end() );
  }
}

//=========================================================================
//  Return the specific ADCs for PIN diode
//  Warning : it requires a decoding adcs(...) method to be executed before
//=========================================================================
const std::vector<LHCb::L0CaloAdc>& CaloTriggerAdcsFromRaw::pinAdcs() { return m_pinData; }
//=========================================================================
//  Decode the adcs for ALL banks of one event
//=========================================================================
const std::vector<LHCb::L0CaloAdc>& CaloTriggerAdcsFromRaw::adcs() { return adcs( -1 ); }
//=========================================================================
//  Decode the adcs for a single bank (given by sourceID)
//  Decode ALL banks if source < 0
//=========================================================================
const std::vector<LHCb::L0CaloAdc>& CaloTriggerAdcsFromRaw::adcs( int source ) {
  clear();
  bool decoded = false;
  bool found   = false;
  int  sourceID;
  if ( m_getRaw ) getBanks();
  if ( m_banks.empty() ) {
    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "The banks container is empty" << endmsg;
  } else {
    for ( const LHCb::RawBank* bank : m_banks ) {
      sourceID = bank->sourceID();
      if ( source >= 0 && source != sourceID ) continue;
      found = true;
      if ( checkSrc( sourceID ) ) continue;
      decoded = getData( *bank );
      if ( !decoded ) {
        if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
          debug() << "Error when decoding bank " << Gaudi::Utils::toString( sourceID )
                  << " -> incomplete data - May be corrupted" << endmsg;
      }
    }
  }
  if ( !found ) {
    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
      debug() << "rawBank sourceID : " << Gaudi::Utils::toString( source ) << " has not been found" << endmsg;
  }
  return m_data;
}

//=========================================================================
//  Decode the adcs of a single bank (given by bank pointer)
//=========================================================================
const std::vector<LHCb::L0CaloAdc>& CaloTriggerAdcsFromRaw::adcs( const LHCb::RawBank& bank ) {
  clear();
  if ( !getData( bank ) ) clear();
  return m_data;
}

//=============================================================================
// Main method to decode the rawBank - fill m_data vector
//=============================================================================

bool CaloTriggerAdcsFromRaw::getData( const LHCb::RawBank& bank ) {
  if ( LHCb::RawBank::MagicPattern != bank.magic() ) return false; // do not decode when MagicPattern is bad
  const unsigned int*       data     = bank.begin<unsigned int>();
  const unsigned int* const end      = bank.end<unsigned int>();
  int                       version  = bank.version();
  int                       sourceID = bank.sourceID();
  int                       lastData = 0;

  if ( data == end ) m_status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Empty );

  if ( msgLevel( MSG::DEBUG ) )
    debug() << "Decode bank " << &bank << " source " << sourceID << "version " << version << " size " << ( end - data )
            << endmsg;

  // -----------------------------------------------
  // skip detector specific header line
  if ( m_extraHeader ) ++data;
  // -----------------------------------------------

  //=== Offline coding
  if ( 2 > version ) {
    while ( data < end ) {
      int lastID = ( *data ) >> 16;
      int adc1   = ( *data ) >> 8 & 0xFF;
      int adc2   = ( *data ) & 0xFF;
      ++data;

      LHCb::CaloCellID id1( lastID );
      if ( 0 != adc1 ) {
        if ( !id1.isPin() ) {
          m_data.emplace_back( id1, adc1 );
        } else {
          m_pinData.emplace_back( id1, adc1 );
        }
      }

      // event dump
      if ( msgLevel( MSG::VERBOSE ) ) {
        verbose() << " |  SourceID : " << sourceID << " |  FeBoard : " << m_calo->cardNumber( id1 ) << " |  CaloCell "
                  << id1 << " |  valid ? " << m_calo->valid( id1 ) << " |  ADC value = " << adc1 << endmsg;
      }

      LHCb::CaloCellID id2( ++lastID );
      if ( 0 != adc2 ) {
        if ( !id2.isPin() ) {
          m_data.emplace_back( id2, adc2 );
        } else {
          m_pinData.emplace_back( id2, adc2 );
        }
      }

      // event dump
      if ( msgLevel( MSG::VERBOSE ) ) {
        verbose() << " |  SourceID : " << sourceID << " |  FeBoard : " << m_calo->cardNumber( id2 ) << " |  CaloCell "
                  << id2 << " |  valid ? " << m_calo->valid( id2 ) << " |  ADC value = " << adc2 << endmsg;
      }
    }
  } else if ( 2 == version ) {
    //==== Codage for 1 MHz
    // Get the FE-Cards associated to that bank (via condDB)
    std::vector<int> feCards = m_calo->tell1ToCards( sourceID );
    int              nCards  = feCards.size();
    if ( msgLevel( MSG::DEBUG ) )
      debug() << nCards << " FE-Cards are expected to be readout : " << feCards << " in Tell1 bank " << sourceID
              << endmsg;
    int lenAdc  = 0;
    int lenTrig = 0;

    int prevCard = -1;
    while ( data < end ) {
      int word = *data++;
      lenTrig  = word & 0x3F;
      lenAdc   = ( word >> 7 ) & 0x3F;
      int code = ( word >> 14 ) & 0x1FF;
      int ctrl = ( word >> 23 ) & 0x1FF;
      checkCtrl( ctrl, sourceID );

      // access chanID via condDB
      std::vector<LHCb::CaloCellID> chanID;
      int                           card = findCardbyCode( feCards, code );
      if ( 0 <= card ) {
        chanID = m_calo->cardChannels( feCards[card] );
        feCards.erase( feCards.begin() + card );
      } else {
        Error( " FE-Card w/ [code : " + Gaudi::Utils::toString( code ) +
               " ] is not associated with TELL1 bank sourceID : " + Gaudi::Utils::toString( sourceID ) +
               " in condDB :  Cannot read that bank" )
            .ignore();
        Error( "Warning : previous data may be corrupted" ).ignore();
        if ( m_cleanCorrupted ) cleanData( prevCard );
        m_status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Incomplete );
        m_status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Corrupted );
        return false;
      }
      prevCard = card;

      // Start readout of the FE-board (trigger data)
      if ( 0 < lenTrig ) {
        int pattern = *data++;
        int offset  = 0;
        lastData    = *data++;
        for ( unsigned int bitNum = 0; 32 > bitNum; bitNum++ ) {
          if ( 0 != ( pattern & ( 1 << bitNum ) ) ) {
            if ( 31 < offset ) {
              offset   = 0;
              lastData = *data++;
            }

            LHCb::CaloCellID id  = ( bitNum < chanID.size() ? chanID[bitNum] : LHCb::CaloCellID() );
            int              adc = ( lastData >> offset ) & 0xFF;

            // event dump
            if ( msgLevel( MSG::VERBOSE ) ) {
              verbose() << " |  SourceID : " << sourceID << " |  FeBoard : " << code << " |  Channel : " << bitNum
                        << " |  CaloCell " << id << " |  valid ? " << m_calo->valid( id ) << " |  ADC value = " << adc
                        << endmsg;
            }

            if ( 0 != id.index() ) {
              if ( !id.isPin() ) {
                m_data.emplace_back( id, adc );
              } else {
                m_pinData.emplace_back( id, adc );
              }
            }
            offset += 8;
          }
        }
      }
      // Skip ADC data
      int nSkip = ( lenAdc + 3 ) / 4; //== length in byte, skip words
      data += nSkip;
    } // another card
    // Check All cards have been read
    if ( !checkCards( nCards, feCards ) )
      m_status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Incomplete );
  } // version
  return true;
}
//=============================================================================
