/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

#include "CaloFutureRawToDigits.h"
#include "CaloFutureUtils/CaloFutureAlgUtils.h"
#include "GaudiAlg/FunctionalUtilities.h"

DECLARE_COMPONENT( CaloFutureRawToDigits )

//=============================================================================
// Standard creator, initializes variables
//=============================================================================
CaloFutureRawToDigits::CaloFutureRawToDigits( const std::string& name, ISvcLocator* pSvcLocator )
    : MultiTransformer(
          name, pSvcLocator,
          KeyValue{"RawEventLocation", Gaudi::Functional::concat_alternatives( LHCb::RawEventLocation::Calo,
                                                                               LHCb::RawEventLocation::Default )},
          {KeyValue{"OutputAdcData", {}}, KeyValue{"OutputDigitData", {}}, KeyValue{"OutputReadoutStatusData", {}}} ) {
  updateHandleLocation( *this, "OutputAdcData",
                        LHCb::CaloFutureAlgUtils::CaloFutureAdcLocation( name.substr( 6, 4 ) ) );
  updateHandleLocation( *this, "OutputDigitData",
                        LHCb::CaloFutureAlgUtils::CaloFutureUnfilteredDigitLocation( name.substr( 6, 4 ) ) );
  updateHandleLocation( *this, "OutputReadoutStatusData",
                        LHCb::CaloFutureAlgUtils::CaloFutureRawBankReadoutStatusLocation( name.substr( 6, 4 ) ) );
}

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode CaloFutureRawToDigits::initialize() {

  StatusCode sc = MultiTransformer::initialize();
  if ( sc.isFailure() ) return sc;

  // Retrieve the calorimeter we are working with.
  m_calo = getDet<DeCalorimeter>( LHCb::CaloFutureAlgUtils::DeCaloFutureLocation( m_detectorName ) );
  if ( !m_calo ) { return Error( "Detector information is not available!" ); }

  m_numberOfCells = m_calo->numberOfCells();

  if ( m_zsupMethod == "NO" && m_zsupThreshold >= 0 ) {
    return Error( "Zero suppression method: NO while Threshold>=0: Must be fixed in configuration" );
  }

  info() << m_detectorName.value() << " has " << m_numberOfCells << " cells. Zsup method " << m_zsupMethod.value()
         << " Threshold " << m_zsupThreshold.value() << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
std::tuple<LHCb::CaloAdcs, LHCb::CaloDigits, LHCb::RawBankReadoutStatus> CaloFutureRawToDigits::
                                                                         operator()( const LHCb::RawEvent& rawEvt ) const {

  // ------------------------------------
  // --- Get RawBank AND ReadoutStatus
  // ------------------------------------
  LHCb::span<const LHCb::RawBank*> banks;
  LHCb::RawBankReadoutStatus       status;

  bool m_packed = false;
  if ( !m_packedIsDefault ) {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Banks of short type are requested as default" << endmsg;
    banks  = rawEvt.banks( m_detectorName == "Ecal" ? LHCb::RawBank::EcalE : LHCb::RawBank::HcalE );
    status = LHCb::RawBankReadoutStatus( m_detectorName == "Ecal" ? LHCb::RawBank::EcalE : LHCb::RawBank::HcalE );
  } else {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Banks of packed type are requested as default" << endmsg;
    banks = rawEvt.banks( m_detectorName == "Ecal" ? LHCb::RawBank::EcalPacked : LHCb::RawBank::HcalPacked );
    status =
        LHCb::RawBankReadoutStatus( m_detectorName == "Ecal" ? LHCb::RawBank::EcalPacked : LHCb::RawBank::HcalPacked );
  }

  if ( 0 == banks.size() ) {
    if ( !m_packedIsDefault ) {
      if ( msgLevel( MSG::DEBUG ) )
        debug() << " Requested banks of short type has not been found ... try packed type" << endmsg;
      banks  = rawEvt.banks( m_detectorName == "Ecal" ? LHCb::RawBank::EcalPacked : LHCb::RawBank::HcalPacked );
      status = LHCb::RawBankReadoutStatus( m_detectorName == "Ecal" ? LHCb::RawBank::EcalPacked
                                                                    : LHCb::RawBank::HcalPacked );
    } else {
      if ( msgLevel( MSG::DEBUG ) )
        debug() << " Requested banks of packed type has not been found ... try short type" << endmsg;
      banks  = rawEvt.banks( m_detectorName == "Ecal" ? LHCb::RawBank::EcalE : LHCb::RawBank::HcalE );
      status = LHCb::RawBankReadoutStatus( m_detectorName == "Ecal" ? LHCb::RawBank::EcalE : LHCb::RawBank::HcalE );
    }

    m_noBanksCounter += banks.empty();
    if ( banks.empty() && msgLevel( MSG::DEBUG ) ) {
      debug() << "WARNING : None of short and packed banks have been found " << endmsg;
    } else {
      if ( !m_packedIsDefault ) {
        if ( msgLevel( MSG::DEBUG ) ) debug() << " Requested banks of packed type has been found" << endmsg;
        m_packed = true;
      } else {
        if ( msgLevel( MSG::DEBUG ) ) debug() << " Requested banks of short type has found" << endmsg;
      }
    }
  } else {
    if ( !m_packedIsDefault ) {
      if ( msgLevel( MSG::DEBUG ) ) debug() << " Requested banks of short type has been found" << endmsg;
    } else {
      if ( msgLevel( MSG::DEBUG ) ) debug() << " Requested banks of packed type has found" << endmsg;
      m_packed = true;
    }
  }

  // check whether the associated Error Bank is present or not
  for ( const auto& b :
        rawEvt.banks( m_detectorName == "Ecal" ? LHCb::RawBank::EcalPackedError : LHCb::RawBank::HcalPackedError ) )
    status.addStatus( b->sourceID(), LHCb::RawBankReadoutStatus::Status::ErrorBank );

  // check banks integrity + Magic pattern
  std::vector<int> sources;
  sources.reserve( banks.size() );
  for ( const auto& b : banks ) {
    if ( !b ) continue;
    sources.push_back( b->sourceID() );
    if ( LHCb::RawBank::MagicPattern != b->magic() ) {
      Error( "Bad MagicPattern for sourceID " + Gaudi::Utils::toString( b->sourceID() ), StatusCode::SUCCESS ).ignore();
      status.addStatus( b->sourceID(), LHCb::RawBankReadoutStatus::Status::BadMagicPattern );
    }
  }

  if ( m_packed ) { // TELL1 format : 1 source per TELL1
    for ( const auto& t : m_calo->tell1Params() ) {
      bool ok = std::any_of( sources.begin(), sources.end(), [&]( int n ) { return t.number() == n; } );
      status.addStatus( t.number(),
                        ok ? LHCb::RawBankReadoutStatus::Status::OK : LHCb::RawBankReadoutStatus::Status::Missing );
    }
  } else { // Offline format : single source 0
    status.addStatus( 0, sources.empty() ? LHCb::RawBankReadoutStatus::Status::Missing
                                         : LHCb::RawBankReadoutStatus::Status::OK );
  }

  // ------------------------------------
  // ---  Decode the rawBanks
  // ------------------------------------
  std::vector<LHCb::CaloAdc> dataVec;
  if ( banks.empty() ) {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "The banks container is empty" << endmsg;
  } else {
    std::vector<int> readSources;
    for ( const auto& bank : banks ) {
      int sourceID = bank->sourceID();

      auto it   = std::find( readSources.begin(), readSources.end(), sourceID );
      bool read = ( it != readSources.end() );
      if ( read ) {
        Warning( "Another bank with same sourceID " + Gaudi::Utils::toString( sourceID ) + " has already been read" )
            .ignore();
        status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::NonUnique );
      } else {
        readSources.push_back( sourceID );
      }
      if ( read ) continue;

      //--- decode the rawbanks
      std::vector<LHCb::CaloAdc> dataVecDec = decode( *bank, status, false ); // false is to get data, not pinData
      dataVec.insert( dataVec.end(), dataVecDec.begin(), dataVecDec.end() );

      if ( dataVec.size() == 0 )
        if ( msgLevel( MSG::DEBUG ) )
          debug() << "Error when decoding bank " << Gaudi::Utils::toString( sourceID )
                  << " -> incomplete data - May be corrupted" << endmsg;
    }
    readSources.clear();
  }

  // ------------------------------------
  // ---  Prepare the output
  // ------------------------------------
  LHCb::CaloAdcs   newAdcs;
  LHCb::CaloDigits newDigits;
  if ( msgLevel( MSG::DEBUG ) ) debug() << "Processing " << dataVec.size() << " Digits." << endmsg;

  enum { DefaultFlag, NeighbourFlag, SeedFlag };
  std::vector<int> caloFlags( m_numberOfCells, DefaultFlag );
  int              index;

  // == Apply the threshold. If 2DZsup, tag also the neighbours
  for ( const auto& anAdc : dataVec ) {
    LHCb::CaloCellID id = anAdc.cellID();
    index               = m_calo->cellIndex( id );
    int digAdc          = anAdc.adc();
    if ( m_zsupThreshold <= digAdc ) {
      verbose() << id << format( " Energy adc %4d", digAdc );
      if ( m_zsupThreshold <= digAdc ) debug() << " seed";
      verbose() << endmsg;
      caloFlags[index] = SeedFlag;
      if ( m_zsupMethod == "2D" ) {
        for ( const auto& neighbour : m_calo->neighborCells( id ) ) {
          int& neighFlag = caloFlags[m_calo->cellIndex( neighbour )];
          if ( neighFlag != SeedFlag ) neighFlag = NeighbourFlag;
        }
      }
    }
  }

  // write tagged data as CaloAdc and CaloDigits
  double pedShift = m_calo->pedestalShift();
  for ( const auto& anAdc : dataVec ) {
    LHCb::CaloCellID id = anAdc.cellID();
    index               = m_calo->cellIndex( id );
    if ( DefaultFlag == caloFlags[index] ) continue;
    if ( NeighbourFlag == caloFlags[index] && anAdc.adc() < m_zsupNeighbour ) continue;

    double e = ( double( anAdc.adc() ) - pedShift ) * m_calo->cellGain( id );
    try {
      auto adc = std::make_unique<LHCb::CaloAdc>( id, anAdc.adc() );
      newAdcs.insert( adc.get() );
      adc.release();
      auto digit = std::make_unique<LHCb::CaloDigit>( id, e );
      newDigits.insert( digit.get() );
      digit.release();
    } catch ( GaudiException& exc ) {
      ++m_duplicateADCDigits; // Duplicate ADC/Digit
      std::ostringstream os( "" );
      os << "Duplicate ADC/Digit for channel " << id << std::endl;
      Warning( os.str(), StatusCode::SUCCESS ).ignore();
      int card  = m_calo->cardNumber( id );
      int tell1 = m_calo->cardToTell1( card );
      // to be completed --- LHCb::RawBankReadoutStatus& status = m_adcTool->status();
      status.addStatus( tell1, LHCb::RawBankReadoutStatus::Status::DuplicateEntry );
    }

    if ( NeighbourFlag == caloFlags[index] )
      verbose() << id << " added as Neighbour." << endmsg;
    else
      verbose() << id << " added as Seed.    " << endmsg;
  }

  if ( msgLevel( MSG::DEBUG ) ) debug() << format( "Have stored %5d CaloAdcs.", newAdcs.size() ) << endmsg;
  if ( msgLevel( MSG::DEBUG ) ) debug() << format( "Have stored %5d CaloDigits.", newDigits.size() ) << endmsg;

  return std::make_tuple( std::move( newAdcs ), std::move( newDigits ), std::move( status ) );
}
//=============================================================================

//=============================================================================
// Main method to decode the rawBank
//=============================================================================
std::vector<LHCb::CaloAdc> CaloFutureRawToDigits::decode( const LHCb::RawBank& bank, LHCb::RawBankReadoutStatus status,
                                                          bool getPinData ) const {

  long                       nCells = m_calo->numberOfCells();
  long                       nPins  = m_calo->numberOfPins();
  std::vector<LHCb::CaloAdc> pinDataVec;
  std::vector<LHCb::CaloAdc> dataVec;
  pinDataVec.reserve( nPins );
  dataVec.reserve( nCells );

  if ( LHCb::RawBank::MagicPattern != bank.magic() ) return dataVec; // false;// do not decode when MagicPattern is bad
  // Get bank info
  const unsigned int* data     = bank.data();
  int                 size     = bank.size() / 4; // Bank size is in bytes
  int                 version  = bank.version();
  int                 sourceID = bank.sourceID();

  if ( msgLevel( MSG::DEBUG ) )
    debug() << "Decode bank " << &bank << " source " << sourceID << " version " << version << " size " << size
            << endmsg;

  if ( 0 == size ) status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Empty );

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
      verbose() << " |  SourceID : " << sourceID << " |  FeBoard : " << m_calo->cardNumber( cellId ) << " |  CaloCell "
                << cellId << " |  valid ? " << m_calo->valid( cellId ) << " |  ADC value = " << adc << endmsg;

      if ( 0 != cellId.index() ) {
        if ( !cellId.isPin() ) {
          dataVec.push_back( temp );
        } else {
          pinDataVec.push_back( temp );
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
    if ( msgLevel( MSG::DEBUG ) )
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
      checkCtrl( ctrl, sourceID, status );
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
        if ( m_cleanCorrupted ) {
          if ( m_calo->isPinCard( prevCard ) ) {
            pinDataVec.erase( std::remove_if( pinDataVec.begin(), pinDataVec.end(),
                                              [&]( const LHCb::CaloAdc& adc ) {
                                                return m_calo->cellParam( adc.cellID() ).cardNumber() == prevCard;
                                              } ),
                              pinDataVec.end() );
          } else {
            dataVec.erase( std::remove_if( dataVec.begin(), dataVec.end(),
                                           [&]( const LHCb::CaloAdc& adc ) {
                                             return m_calo->cellParam( adc.cellID() ).cardNumber() == prevCard;
                                           } ),
                           dataVec.end() );
          }
        }
        status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Incomplete );
        status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Corrupted );
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
        verbose() << " |  SourceID : " << sourceID << " |  FeBoard : " << m_calo->cardNumber( id )
                  << " |  Channel : " << bitNum << " |  CaloCell " << id << " |  valid ? " << m_calo->valid( id )
                  << " |  ADC value = " << adc << endmsg;

        //== Keep only valid cells
        if ( 0 != id.index() ) {
          LHCb::CaloAdc temp( id, adc );
          if ( !id.isPin() ) {
            dataVec.push_back( temp );
          } else {
            pinDataVec.push_back( temp );
          }
        }
      }
    }
    // Check All cards have been read
    if ( !checkCards( nCards, feCards ) ) status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Incomplete );

  } else if ( 3 == version ) {
    //******************************************************************
    //**** 1 MHz compression format, Preshower + SPD
    //******************************************************************

    // Get the FE-Cards associated to that bank (via condDB)
    std::vector<int> feCards = m_calo->tell1ToCards( sourceID );
    int              nCards  = feCards.size();
    if ( msgLevel( MSG::DEBUG ) )
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
      checkCtrl( ctrl, sourceID, status );
      // access chanID via condDB
      // look for the FE-Card in the Tell1->cards vector
      int card = findCardbyCode( feCards, code );
      if ( card < 0 ) {
        Error( " FE-Card w/ [code : " + Gaudi::Utils::toString( code ) +
               " ] is not associated with TELL1 bank sourceID : " + Gaudi::Utils::toString( sourceID ) +
               " in condDB :  Cannot read that bank" )
            .ignore();

        Error( "Warning : previous data may be corrupted" ).ignore();
        if ( m_cleanCorrupted ) {
          if ( m_calo->isPinCard( prevCard ) ) {
            pinDataVec.erase( std::remove_if( pinDataVec.begin(), pinDataVec.end(),
                                              [&]( const LHCb::CaloAdc& adc ) {
                                                return m_calo->cellParam( adc.cellID() ).cardNumber() == prevCard;
                                              } ),
                              pinDataVec.end() );
          } else {
            dataVec.erase( std::remove_if( dataVec.begin(), dataVec.end(),
                                           [&]( const LHCb::CaloAdc& adc ) {
                                             return m_calo->cellParam( adc.cellID() ).cardNumber() == prevCard;
                                           } ),
                           dataVec.end() );
          }
        }

        status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Corrupted |
                                        LHCb::RawBankReadoutStatus::Status::Incomplete );
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
        verbose() << " |  SourceID : " << sourceID << " |  FeBoard : " << m_calo->cardNumber( id )
                  << " |  Channel : " << num << " |  CaloCell " << id << " |  valid ? " << m_calo->valid( id )
                  << " |  ADC value = " << adc << endmsg;

        if ( 0 != id.index() ) {
          if ( !id.isPin() ) {
            dataVec.emplace_back( id, adc );
          } else {
            pinDataVec.emplace_back( id, adc );
          }
        }

        lenAdc--;
        offset += 16;
      }
    } //== DataSize
    // Check All cards have been read
    if ( !checkCards( nCards, feCards ) ) status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Incomplete );
  } //== versions
  return ( getPinData ? pinDataVec : dataVec );
}

//========================
//  Check FE-Cards is PIN
//========================
bool CaloFutureRawToDigits::checkCards( int nCards, const std::vector<int>& feCards ) const {
  bool check = true;
  if ( msgLevel( MSG::DEBUG ) )
    debug() << nCards - feCards.size() << "FE-Cards have been read among the " << nCards << " expected" << endmsg;
  for ( auto i : feCards ) {
    if ( msgLevel( MSG::DEBUG ) )
      debug() << " Unread FE-Cards : " << m_calo->cardCode( i ) << "  - Is it a PinDiode readout FE-Card ? "
              << m_calo->isPinCard( i ) << endmsg;
    if ( m_calo->isPmtCard( i ) ) {
      Warning( " The standard (PMT) FE-Card " + Gaudi::Utils::toString( m_calo->cardCode( i ) ) +
               " expected in TELL1 bank has not been read !!" )
          .ignore();
      check = false;
    }
  }
  return check;
}

//===========================
//  Find Card number by code
//===========================
int CaloFutureRawToDigits::findCardbyCode( const std::vector<int>& feCards, int code ) const {
  auto i = std::find_if( feCards.begin(), feCards.end(), [&]( int ife ) { return code == m_calo->cardCode( ife ); } );
  if ( i == feCards.end() ) {
    Error( "FE-Card [code : " + Gaudi::Utils::toString( code ) + "] does not match the condDB cabling scheme  ",
           StatusCode::SUCCESS )
        .ignore();
    return -1;
  }
  if ( msgLevel( MSG::DEBUG ) )
    debug() << " FE-Card [code : " << code << " | crate : " << m_calo->cardParam( *i ).crate()
            << " slot : " << m_calo->cardParam( *i ).slot() << "] has been found with (num : " << *i << ")  in condDB"
            << endmsg;
  return std::distance( feCards.begin(), i );
}

void CaloFutureRawToDigits::checkCtrl( int ctrl, int sourceID, LHCb::RawBankReadoutStatus status ) const {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "Control word :" << ctrl << endmsg;
  if ( 0 != ( 0x1 & ctrl ) || 0 != ( 0x20 & ctrl ) || 0 != ( 0x40 & ctrl ) ) {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Tell1 error bits have been detected in data" << endmsg;
    if ( 0 != ( 0x1 & ctrl ) ) status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Tell1Error );
    if ( 0 != ( 0x20 & ctrl ) ) status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Tell1Sync );
    if ( 0 != ( 0x40 & ctrl ) ) status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Tell1Link );
  }
}

//   //=========================================================================
//   //  Main method to convert ADCs to CaloDigits
//   //  This is just a gain calibration (+ped shift) of the adc values.
//   //=========================================================================
//   std::vector<LHCb::CaloDigit> CaloEnergyFromRaw::getDigits ( const std::vector<LHCb::CaloAdc>& dataVec) {
//
//     std::vector<LHCb::CaloDigit>  digitsVec;
//     long nCells = m_calo->numberOfCells();
//     digitsVec.reserve( nCells );
//     double pedShift = m_calo->pedestalShift();
//     std::transform( dataVec.begin(), dataVec.end(),
//                     std::back_inserter(digitsVec),
//                     [&](const LHCb::CaloAdc& cadc) -> LHCb::CaloDigit {
//                        LHCb::CaloCellID id = cadc.cellID();
//                        int adc = cadc.adc();
//                        double e = ( double(adc) - pedShift ) * m_calo->cellGain( id );
//                        return { id, e };
//     } );
//     return digitsVec;
//   }
