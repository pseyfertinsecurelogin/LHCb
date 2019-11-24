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
#if defined( __GNUC__ ) && __GNUC__ > 8
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-copy"
#endif
#include "Event/CaloAdc.h"
#include "Event/CaloDigit.h"
#if defined( __GNUC__ ) && __GNUC__ > 8
#  pragma GCC diagnostic pop
#endif

// local
#include "CaloEnergyFromRaw.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CaloEnergyFromRaw
//
// 2005-01-10 : Olivier Callot
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( CaloEnergyFromRaw )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CaloEnergyFromRaw::CaloEnergyFromRaw( const std::string& type, const std::string& name, const IInterface* parent )
    : base_class( type, name, parent ) {
  clear();
}

//=========================================================================
//  Initialisation, according to the name -> detector
//=========================================================================
StatusCode CaloEnergyFromRaw::initialize() {
  StatusCode sc = base_class::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;          // error printed already by GaudiAlgorithm
  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
    debug() << "==> Initialize " << name() << " Det = " << m_detectorName << endmsg;

  switch ( m_detectorName.value() ) {
  case details::DetectorName_t::Ecal:
    m_calo       = getDet<DeCalorimeter>( DeCalorimeterLocation::Ecal );
    m_packedType = LHCb::RawBank::EcalPacked;
    m_shortType  = LHCb::RawBank::EcalE;
    m_errorType  = LHCb::RawBank::EcalPackedError;
    break;
  case details::DetectorName_t::Hcal:
    m_calo       = getDet<DeCalorimeter>( DeCalorimeterLocation::Hcal );
    m_packedType = LHCb::RawBank::HcalPacked;
    m_shortType  = LHCb::RawBank::HcalE;
    m_errorType  = LHCb::RawBank::HcalPackedError;
    break;
  case details::DetectorName_t::Prs:
    m_calo       = getDet<DeCalorimeter>( DeCalorimeterLocation::Prs );
    m_packedType = LHCb::RawBank::PrsPacked;
    m_shortType  = LHCb::RawBank::PrsE;
    m_errorType  = LHCb::RawBank::PrsPackedError;
    break;
  default:
    error() << "Unknown detector name " << toString( m_detectorName ) << endmsg;
    return StatusCode::FAILURE;
  }

  long nCells = m_calo->numberOfCells();
  long nPins  = m_calo->numberOfPins();
  m_pinData.reserve( nPins );
  m_data.reserve( nCells );
  m_digits.reserve( nCells );
  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "Got detector element for " << m_detectorName << endmsg;
  return StatusCode::SUCCESS;
}

//-------------------------------------
void CaloEnergyFromRaw::clear() {
  m_data.clear();
  m_digits.clear();
  m_pinData.clear();
  m_readSources.clear();
}

//-------------------------------------
void CaloEnergyFromRaw::cleanData( int feb ) {
  if ( feb < 0 ) return;

  if ( m_calo->isPinCard( feb ) ) {
    m_pinData.erase( std::remove_if( m_pinData.begin(), m_pinData.end(),
                                     [&]( const LHCb::CaloAdc& adc ) {
                                       return m_calo->cellParam( adc.cellID() ).cardNumber() == feb;
                                     } ),
                     m_pinData.end() );
  } else {
    m_data.erase( std::remove_if( m_data.begin(), m_data.end(),
                                  [&]( const LHCb::CaloAdc& adc ) {
                                    return m_calo->cellParam( adc.cellID() ).cardNumber() == feb;
                                  } ),
                  m_data.end() );

    m_digits.erase( std::remove_if( m_digits.begin(), m_digits.end(),
                                    [&]( const LHCb::CaloDigit& dig ) {
                                      return m_calo->cellParam( dig.cellID() ).cardNumber() == feb;
                                    } ),
                    m_digits.end() );
  }
}

//=========================================================================
//  Return the specific ADCs for PIN diode
//  Warning : it requires a decoding adcs(...) method to be executed before
//=========================================================================
const std::vector<LHCb::CaloAdc>& CaloEnergyFromRaw::pinAdcs() { return m_pinData; }
//=========================================================================
//  Decode the adcs for ALL banks of one event
//=========================================================================
const std::vector<LHCb::CaloAdc>& CaloEnergyFromRaw::adcs() { return adcs( -1 ); }
//=========================================================================
//  Decode the adcs for a single bank (given by sourceID)
//  Decode ALL banks if source < 0
//=========================================================================
const std::vector<LHCb::CaloAdc>& CaloEnergyFromRaw::adcs( int source ) {
  clear();
  int  sourceID;
  bool decoded = false;
  bool found   = false;
  if ( m_getRaw ) getBanks();
  if ( m_banks.empty() ) {
    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "The banks container is empty" << endmsg;
  } else {
    for ( const auto& bank : m_banks ) {
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
const std::vector<LHCb::CaloAdc>& CaloEnergyFromRaw::adcs( const LHCb::RawBank& bank ) {
  clear();
  if ( !getData( bank ) ) clear();
  return m_data;
}

//=========================================================================
//  Decode the adcs of a single bank and convert to digit (given sourceID)
//=========================================================================
const std::vector<LHCb::CaloDigit>& CaloEnergyFromRaw::digits( int source ) {
  adcs( source );
  if ( !getDigits() ) m_digits.clear();
  return m_digits;
}

//=========================================================================
//  Decode the adcs of a single bank and convert to digit (given by bank pointer)
//=========================================================================
const std::vector<LHCb::CaloDigit>& CaloEnergyFromRaw::digits( const LHCb::RawBank& bank ) {
  adcs( bank );
  if ( !getDigits() ) m_digits.clear();
  return m_digits;
}

//=========================================================================
//  Decode the adcs for ALL banks bank and convert to digit - return m_digits
//=========================================================================
const std::vector<LHCb::CaloDigit>& CaloEnergyFromRaw::digits() {
  adcs();
  if ( !getDigits() ) m_digits.clear();
  return m_digits;
}

//=============================================================================
// Main method to decode the rawBank - fill m_data vector
//=============================================================================
bool CaloEnergyFromRaw::getData( const LHCb::RawBank& bank ) {
  if ( LHCb::RawBank::MagicPattern != bank.magic() ) return false; // do not decode when MagicPattern is bad
  // Get bank info
  const unsigned int* data     = bank.data();
  int                 size     = bank.size() / 4; // Bank size is in bytes
  int                 version  = bank.version();
  int                 sourceID = bank.sourceID();

  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
    debug() << "Decode bank " << &bank << " source " << sourceID << " version " << version << " size " << size
            << endmsg;

  if ( 0 == size ) m_status->addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Empty );

  // -----------------------------------------------
  // skip detector specific header line
  if ( m_extraHeader ) {
    ++data;
    --size;
  }
  // -----------------------------------------------

  if ( 1 > version || 3 < version ) {
    warning() << "Bank type " << bank.type() << " sourceID " << sourceID << " has version " << version
              << " which is not supported" << endmsg;

  } else if ( 1 == version ) {
    //******************************************************************
    //**** Simple coding, ID + adc in 32 bits.
    //******************************************************************
    while ( 0 != size ) {
      int id  = ( ( *data ) >> 16 ) & 0xFFFF;
      int adc = ( *data ) & 0xFFFF;
      if ( 32767 < adc ) adc |= 0xFFFF0000; //= negative value
      LHCb::CaloCellID cellId( id );
      LHCb::CaloAdc    temp( cellId, adc );

      // event dump
      if ( msgLevel( MSG::VERBOSE ) ) {
        verbose() << " |  SourceID : " << sourceID << " |  FeBoard : " << m_calo->cardNumber( cellId )
                  << " |  CaloCell " << cellId << " |  valid ? " << m_calo->valid( cellId ) << " |  ADC value = " << adc
                  << endmsg;
      }

      if ( 0 != cellId.index() ) {
        if ( !cellId.isPin() ) {
          m_data.push_back( temp );
        } else {
          m_pinData.push_back( temp );
        }
      }

      ++data;
      --size;
    }

  } else if ( 2 == version ) {
    //******************************************************************
    //**** 1 MHz compression format, Ecal and Hcal
    //******************************************************************
    // Get the FE-Cards associated to that bank (via condDB)
    std::vector<int> feCards = m_calo->tell1ToCards( sourceID );
    int              nCards  = feCards.size();
    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
      debug() << nCards << " FE-Cards are expected to be readout : " << feCards << " in Tell1 bank " << sourceID
              << endmsg;
    int prevCard = -1;
    while ( 0 != size ) {
      // Skip
      unsigned int word = *data++;
      size--;
      // Read bank header
      int lenTrig = word & 0x7F;
      int code    = ( word >> 14 ) & 0x1FF;
      int ctrl    = ( word >> 23 ) & 0x1FF;
      checkCtrl( ctrl, sourceID );
      // access chanID via condDB
      std::vector<LHCb::CaloCellID> chanID;
      // look for the FE-Card in the Tell1->cards vector
      int card = findCardbyCode( feCards, code );
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
        m_status->addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Incomplete );
        m_status->addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Corrupted );
        return false;
      }
      prevCard = card;

      // Start readout of the FE-board
      // First skip trigger bank ...
      int nSkip = ( lenTrig + 3 ) / 4; //== is in bytes, with padding
      data += nSkip;
      size -= nSkip;
      unsigned int pattern  = *data++;
      int          offset   = 0;
      unsigned int lastData = *data++;
      size -= 2;
      // ... and readout data
      for ( unsigned int bitNum = 0; 32 > bitNum; bitNum++ ) {
        int adc;
        if ( 31 < offset ) {
          offset -= 32;
          lastData = *data++;
          size--;
        }
        if ( 0 == ( pattern & ( 1 << bitNum ) ) ) { //.. short coding
          adc = ( ( lastData >> offset ) & 0xF ) - 8;
          offset += 4;
        } else {
          adc = ( ( lastData >> offset ) & 0xFFF );
          if ( 24 == offset ) adc &= 0xFF;
          if ( 28 == offset ) adc &= 0xF; //== clean-up extra bits
          offset += 12;
          if ( 32 < offset ) { //.. get the extra bits on next word
            lastData = *data++;
            size--;
            offset -= 32;
            int temp = ( lastData << ( 12 - offset ) ) & 0xFFF;
            adc += temp;
          }
          adc -= 256;
        }

        LHCb::CaloCellID id = LHCb::CaloCellID();
        if ( bitNum < chanID.size() ) id = chanID[bitNum];

        // event dump
        if ( msgLevel( MSG::VERBOSE ) ) {
          verbose() << " |  SourceID : " << sourceID << " |  FeBoard : " << m_calo->cardNumber( id )
                    << " |  Channel : " << bitNum << " |  CaloCell " << id << " |  valid ? " << m_calo->valid( id )
                    << " |  ADC value = " << adc << endmsg;
        }

        //== Keep only valid cells
        if ( 0 != id.index() ) {
          LHCb::CaloAdc temp( id, adc );
          if ( !id.isPin() ) {
            m_data.push_back( temp );
          } else {
            m_pinData.push_back( temp );
          }
        }
      }
    }
    // Check All cards have been read
    if ( !checkCards( nCards, feCards ) )
      m_status->addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Incomplete );

  } else if ( 3 == version ) {
    //******************************************************************
    //**** 1 MHz compression format, Preshower + SPD
    //******************************************************************

    // Get the FE-Cards associated to that bank (via condDB)
    std::vector<int> feCards = m_calo->tell1ToCards( sourceID );
    int              nCards  = feCards.size();
    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
      debug() << nCards << " FE-Cards are expected to be readout : " << feCards << " in Tell1 bank " << sourceID
              << endmsg;
    int prevCard = -1;
    while ( 0 != size ) {
      // Skip
      unsigned int word = *data++;
      size--;
      // Read bank header
      int lenTrig = word & 0x7F;
      int lenAdc  = ( word >> 7 ) & 0x7F;
      int code    = ( word >> 14 ) & 0x1FF;
      int ctrl    = ( word >> 23 ) & 0x1FF;
      checkCtrl( ctrl, sourceID );
      // access chanID via condDB
      // look for the FE-Card in the Tell1->cards vector
      int card = findCardbyCode( feCards, code );
      if ( card < 0 ) {
        Error( " FE-Card w/ [code : " + Gaudi::Utils::toString( code ) +
               " ] is not associated with TELL1 bank sourceID : " + Gaudi::Utils::toString( sourceID ) +
               " in condDB :  Cannot read that bank" )
            .ignore();
        Error( "Warning : previous data may be corrupted" ).ignore();
        if ( m_cleanCorrupted ) cleanData( prevCard );
        m_status->addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Corrupted |
                                           LHCb::RawBankReadoutStatus::Status::Incomplete );
        return false;
      }
      std::vector<LHCb::CaloCellID> chanID = m_calo->cardChannels( feCards[card] );
      feCards.erase( feCards.begin() + card );
      prevCard = card;

      // Read the FE-Board
      // skip the trigger bits
      int nSkip = ( lenTrig + 3 ) / 4; //== Length in byte, with padding
      size -= nSkip;
      data += nSkip;

      // read data
      int          offset   = 32; //== force read the first word, to have also the debug for it.
      unsigned int lastData = 0;

      while ( 0 < lenAdc ) {
        if ( 32 == offset ) {
          lastData = *data++;
          size--;
          offset = 0;
        }
        int          adc = ( lastData >> offset ) & 0x3FF;
        unsigned int num = ( lastData >> ( offset + 10 ) ) & 0x3F;

        LHCb::CaloCellID id = ( num < chanID.size() ? chanID[num] : LHCb::CaloCellID{} );

        // event dump
        if ( msgLevel( MSG::VERBOSE ) ) {
          verbose() << " |  SourceID : " << sourceID << " |  FeBoard : " << m_calo->cardNumber( id )
                    << " |  Channel : " << num << " |  CaloCell " << id << " |  valid ? " << m_calo->valid( id )
                    << " |  ADC value = " << adc << endmsg;
        }

        if ( 0 != id.index() ) {
          if ( !id.isPin() ) {
            m_data.emplace_back( id, adc );
          } else {
            m_pinData.emplace_back( id, adc );
          }
        }

        lenAdc--;
        offset += 16;
      }
    } //== DataSize
    // Check All cards have been read
    if ( !checkCards( nCards, feCards ) )
      m_status->addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Incomplete );
  } //== versions

  return true;
}

//=========================================================================
//  Main method to convert ADCs to CaloDigits
//  This is just a gain calibration (+ped shift) of the adc values.
//=========================================================================
bool CaloEnergyFromRaw::getDigits() {
  m_digits.clear();
  m_digits.reserve( m_data.size() );
  double pedShift = m_calo->pedestalShift();
  std::transform( m_data.begin(), m_data.end(), std::back_inserter( m_digits ),
                  [&]( const LHCb::CaloAdc& cadc ) -> LHCb::CaloDigit {
                    LHCb::CaloCellID id  = cadc.cellID();
                    int              adc = cadc.adc();
                    double           e   = ( double( adc ) - pedShift ) * m_calo->cellGain( id );
                    return {id, e};
                  } );
  return true;
}
