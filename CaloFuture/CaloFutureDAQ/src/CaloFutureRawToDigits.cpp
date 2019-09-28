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

namespace {
  LHCb::RawBank::BankType alternate( LHCb::RawBank::BankType b ) {
    assert( b == LHCb::RawBank::EcalE || b == LHCb::RawBank::EcalPacked || b == LHCb::RawBank::HcalE ||
            b == LHCb::RawBank::HcalPacked );
    switch ( b ) {
    case LHCb::RawBank::EcalE:
      return LHCb::RawBank::EcalPacked;
    case LHCb::RawBank::EcalPacked:
      return LHCb::RawBank::EcalE;
    case LHCb::RawBank::HcalE:
      return LHCb::RawBank::HcalPacked;
    case LHCb::RawBank::HcalPacked:
      return LHCb::RawBank::HcalE;
    default:
      throw std::invalid_argument( "alternate: bad BankType" );
    }
  }
  const char* label( LHCb::RawBank::BankType b ) {
    assert( b == LHCb::RawBank::EcalE || b == LHCb::RawBank::EcalPacked || b == LHCb::RawBank::HcalE ||
            b == LHCb::RawBank::HcalPacked );
    switch ( b ) {
    case LHCb::RawBank::EcalE:
    case LHCb::RawBank::HcalE:
      return "short";
    case LHCb::RawBank::EcalPacked:
    case LHCb::RawBank::HcalPacked:
      return "packed";
    default:
      throw std::invalid_argument( "label: bad BankType" );
    }
  }

  template <typename T, std::ptrdiff_t N>
  constexpr auto pop( gsl::span<T, N>& v ) {
    auto first = v[0];
    v          = v.template subspan<1>();
    return first;
  }
  template <typename T, std::ptrdiff_t N>
  constexpr void advance( gsl::span<T, N>& v, int s ) {
    v = v.subspan( s );
  }

} // namespace

DECLARE_COMPONENT( CaloFutureRawToDigits )

//=============================================================================
// Standard creator, initializes variables
//=============================================================================
CaloFutureRawToDigits::CaloFutureRawToDigits( const std::string& name, ISvcLocator* pSvcLocator )
    : MultiTransformer(
          name, pSvcLocator,
          {KeyValue{"RawEventLocation", Gaudi::Functional::concat_alternatives( LHCb::RawEventLocation::Calo,
                                                                                LHCb::RawEventLocation::Default )},
           KeyValue{"DetectorLocation", LHCb::CaloFutureAlgUtils::DeCaloFutureLocation( name.substr( 6, 4 ) )}},
          {KeyValue{"OutputAdcData", LHCb::CaloFutureAlgUtils::CaloFutureAdcLocation( name.substr( 6, 4 ) )},
           KeyValue{"OutputDigitData",
                    LHCb::CaloFutureAlgUtils::CaloFutureUnfilteredDigitLocation( name.substr( 6, 4 ) )},
           KeyValue{"OutputReadoutStatusData",
                    LHCb::CaloFutureAlgUtils::CaloFutureRawBankReadoutStatusLocation( name.substr( 6, 4 ) )}} ) {}

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode CaloFutureRawToDigits::initialize() {

  StatusCode sc = MultiTransformer::initialize();
  if ( sc.isFailure() ) return sc;

  if ( m_zsupMethod == "NO" && m_zsupThreshold >= 0 ) {
    error() << "Zero suppression method: NO while Threshold>=0: Must be fixed in configuration" << endmsg;
    return StatusCode::FAILURE;
  }
  info() << "Zsup method " << m_zsupMethod.value() << " Threshold " << m_zsupThreshold.value() << endmsg;
  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
std::tuple<LHCb::CaloAdcs, LHCb::CaloDigits, LHCb::RawBankReadoutStatus> CaloFutureRawToDigits::
                                                                         operator()( const LHCb::RawEvent& rawEvt, const DeCalorimeter& calo ) const {

  // ------------------------------------
  // --- Get RawBank AND ReadoutStatus
  // ------------------------------------

  auto indx = calo.index();
  assert( indx == CaloCellCode::CaloIndex::EcalCalo || indx == CaloCellCode::CaloIndex::HcalCalo );

  bool                    isEcal = ( indx == CaloCellCode::CaloIndex::EcalCalo );
  LHCb::RawBank::BankType bank_type =
      ( isEcal ? ( m_packedIsDefault ? LHCb::RawBank::EcalPacked : LHCb::RawBank::EcalE )
               : ( m_packedIsDefault ? LHCb::RawBank::HcalPacked : LHCb::RawBank::HcalE ) );
  if ( msgLevel( MSG::DEBUG ) )
    debug() << "Banks of " << label( bank_type ) << " type are requested as default" << endmsg;

  LHCb::span<const LHCb::RawBank*> banks = rawEvt.banks( bank_type );
  if ( banks.empty() ) {
    if ( msgLevel( MSG::DEBUG ) )
      debug() << " Requested banks of " << label( bank_type ) << " type has not been found ... try "
              << label( alternate( bank_type ) ) << " type" << endmsg;
    bank_type = alternate( bank_type );
    banks     = rawEvt.banks( bank_type );
  }
  if ( msgLevel( MSG::DEBUG ) ) {
    if ( banks.empty() )
      debug() << "WARNING : None of short and packed banks have been found " << endmsg;
    else
      debug() << " Requested banks of " << label( bank_type ) << " type has been found" << endmsg;
  }
  LHCb::RawBankReadoutStatus status = LHCb::RawBankReadoutStatus( bank_type );
  bool packed = ( bank_type == LHCb::RawBank::EcalPacked || bank_type == LHCb::RawBank::HcalPacked );
  m_noBanksCounter += banks.empty();
  if ( banks.empty() && msgLevel( MSG::DEBUG ) ) debug() << "The banks container is empty" << endmsg;

  // check whether the associated Error Bank is present or not
  for ( const auto& b : rawEvt.banks( isEcal ? LHCb::RawBank::EcalPackedError : LHCb::RawBank::HcalPackedError ) )
    status.addStatus( b->sourceID(), LHCb::RawBankReadoutStatus::Status::ErrorBank );

  // check banks integrity + Magic pattern
  std::vector<int> sources;
  sources.reserve( banks.size() );
  for ( const auto& b : banks ) {
    if ( !b ) continue;
    sources.push_back( b->sourceID() );
    if ( LHCb::RawBank::MagicPattern != b->magic() ) {
      error() << "Bad MagicPattern for sourceID " << Gaudi::Utils::toString( b->sourceID() ) << endmsg;
      status.addStatus( b->sourceID(), LHCb::RawBankReadoutStatus::Status::BadMagicPattern );
    }
  }

  if ( packed ) { // TELL1 format : 1 source per TELL1
    for ( const auto& t : calo.tell1Params() ) {
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

  std::vector<int> readSources;
  for ( const auto& bank : banks ) {
    int sourceID = bank->sourceID();

    auto it   = std::find( readSources.begin(), readSources.end(), sourceID );
    bool read = ( it != readSources.end() );
    if ( read ) {
      warning() << "Another bank with same sourceID " << sourceID << " has already been read" << endmsg;
      status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::NonUnique );
      continue;
    }

    readSources.push_back( sourceID );

    //--- decode the rawbanks
    std::vector<LHCb::CaloAdc> dataVecDec =
        decode_<Decode::Cell>( *bank, status, calo ); // false is to get data, not pinData
    dataVec.insert( dataVec.end(), dataVecDec.begin(), dataVecDec.end() );

    if ( dataVecDec.empty() )
      if ( msgLevel( MSG::DEBUG ) )
        debug() << "Error when decoding bank " << sourceID << " -> incomplete data - May be corrupted" << endmsg;
  }

  // ------------------------------------
  // ---  Prepare the output
  // ------------------------------------
  LHCb::CaloAdcs   newAdcs;
  LHCb::CaloDigits newDigits;
  if ( msgLevel( MSG::DEBUG ) ) debug() << "Processing " << dataVec.size() << " Digits." << endmsg;

  enum { DefaultFlag, NeighbourFlag, SeedFlag };
  std::vector<int> caloFlags( calo.numberOfCells(), DefaultFlag );

  // == Apply the threshold. If 2DZsup, tag also the neighbours
  for ( const auto& anAdc : dataVec ) {
    LHCb::CaloCellID id     = anAdc.cellID();
    int              index  = calo.cellIndex( id );
    int              digAdc = anAdc.adc();
    if ( m_zsupThreshold <= digAdc ) {
      verbose() << id << format( " Energy adc %4d", digAdc );
      if ( m_zsupThreshold <= digAdc ) debug() << " seed";
      verbose() << endmsg;
      caloFlags[index] = SeedFlag;
      if ( m_zsupMethod == "2D" ) {
        for ( const auto& neighbour : calo.neighborCells( id ) ) {
          int& neighFlag = caloFlags[calo.cellIndex( neighbour )];
          if ( neighFlag != SeedFlag ) neighFlag = NeighbourFlag;
        }
      }
    }
  }

  // write tagged data as CaloAdc and CaloDigits
  double pedShift = calo.pedestalShift();
  for ( const auto& anAdc : dataVec ) {
    LHCb::CaloCellID id    = anAdc.cellID();
    int              index = calo.cellIndex( id );
    if ( DefaultFlag == caloFlags[index] ) continue;
    if ( NeighbourFlag == caloFlags[index] && anAdc.adc() < m_zsupNeighbour ) continue;

    double e = ( double( anAdc.adc() ) - pedShift ) * calo.cellGain( id );
    try {
      auto adc = std::make_unique<LHCb::CaloAdc>( id, anAdc.adc() );
      newAdcs.insert( adc.get() );
      adc.release();
      auto digit = std::make_unique<LHCb::CaloDigit>( id, e );
      newDigits.insert( digit.get() );
      digit.release();
    } catch ( GaudiException& exc ) {
      ++m_duplicateADCDigits; // Duplicate ADC/Digit
      warning() << "Duplicate ADC/Digit for channel " << id << endmsg;
      int card  = calo.cardNumber( id );
      int tell1 = calo.cardToTell1( card );
      // to be completed --- LHCb::RawBankReadoutStatus& status = m_adcTool->status();
      status.addStatus( tell1, LHCb::RawBankReadoutStatus::Status::DuplicateEntry );
    }

    if ( NeighbourFlag == caloFlags[index] )
      verbose() << id << " added as Neighbour." << endmsg;
    else
      verbose() << id << " added as Seed.    " << endmsg;
  }

  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << format( "Have stored %5d CaloAdcs.", newAdcs.size() ) << endmsg;
    debug() << format( "Have stored %5d CaloDigits.", newDigits.size() ) << endmsg;
  }

  return {std::move( newAdcs ), std::move( newDigits ), std::move( status )};
}
//=============================================================================
template <CaloFutureRawToDigits::Decode decode>
std::vector<LHCb::CaloAdc> CaloFutureRawToDigits::decode_v1( int sourceID, LHCb::span<const unsigned int> data,
                                                             LHCb::RawBankReadoutStatus&,
                                                             const DeCalorimeter& calo ) const {
  std::vector<LHCb::CaloAdc> dataVec;
  dataVec.reserve( decode == Decode::PinData ? calo.numberOfPins() : calo.numberOfCells() );
  //******************************************************************
  //**** Simple coding, ID + adc in 32 bits.
  //******************************************************************
  while ( !data.empty() ) {
    auto d   = pop( data );
    int  adc = d & 0xFFFF;
    if ( 32767 < adc ) adc |= 0xFFFF0000; //= negative value
    LHCb::CaloCellID cellId( ( d >> 16 ) & 0xFFFF );
    LHCb::CaloAdc    temp( cellId, adc );
    // event dump
    verbose() << " |  SourceID : " << sourceID << " |  FeBoard : " << calo.cardNumber( cellId ) << " |  CaloCell "
              << cellId << " |  valid ? " << calo.valid( cellId ) << " |  ADC value = " << adc << endmsg;

    if ( 0 != cellId.index() && cellId.isPin() == ( decode == Decode::PinData ) ) dataVec.push_back( temp );
  }
  return dataVec;
}
//=============================================================================
template <CaloFutureRawToDigits::Decode decode>
std::vector<LHCb::CaloAdc> CaloFutureRawToDigits::decode_v2( int sourceID, LHCb::span<const unsigned int> data,
                                                             LHCb::RawBankReadoutStatus& status,
                                                             const DeCalorimeter&        calo ) const {
  std::vector<LHCb::CaloAdc> dataVec;
  dataVec.reserve( decode == Decode::PinData ? calo.numberOfPins() : calo.numberOfCells() );
  //******************************************************************
  //**** 1 MHz compression format, Ecal and Hcal
  //******************************************************************
  // Get the FE-Cards associated to that bank (via condDB)
  std::vector<int> feCards = calo.tell1ToCards( sourceID );
  int              nCards  = feCards.size();
  if ( msgLevel( MSG::DEBUG ) )
    debug() << nCards << " FE-Cards are expected to be readout : " << feCards << " in Tell1 bank " << sourceID
            << endmsg;
  int prevCard = -1;
  while ( !data.empty() ) {
    // Skip
    unsigned int word = pop( data );
    // Read bank header
    int lenTrig = word & 0x7F;
    int code    = ( word >> 14 ) & 0x1FF;
    int ctrl    = ( word >> 23 ) & 0x1FF;
    checkCtrl( ctrl, sourceID, status );
    // access chanID via condDB
    std::vector<LHCb::CaloCellID> chanID;
    // look for the FE-Card in the Tell1->cards vector
    int card = findCardbyCode( calo, feCards, code );
    if ( 0 <= card ) {
      chanID = calo.cardChannels( feCards[card] );
      feCards.erase( feCards.begin() + card );
    } else {
      error() << " FE-Card w/ [code : " << Gaudi::Utils::toString( code )
              << " ] is not associated with TELL1 bank sourceID : " << sourceID << " in condDB :  Cannot read that bank"
              << endmsg;

      error() << "Warning : previous data may be corrupted" << endmsg;
      if ( m_cleanCorrupted && calo.isPinCard( prevCard ) == ( decode == Decode::PinData ) ) {
        auto hasBadCardNumber = [&]( const LHCb::CaloAdc& adc ) {
          return calo.cellParam( adc.cellID() ).cardNumber() == prevCard;
        };
        dataVec.erase( std::remove_if( dataVec.begin(), dataVec.end(), hasBadCardNumber ), dataVec.end() );
      }
      status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Incomplete );
      status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Corrupted );
    }
    prevCard = card;

    // Start readout of the FE-board
    // First skip trigger bank ...
    advance( data, ( lenTrig + 3 ) / 4 ); //== is in bytes, with padding
    unsigned int pattern  = pop( data );
    int          offset   = 0;
    unsigned int lastData = pop( data );
    // ... and readout data
    for ( unsigned int bitNum = 0; 32 > bitNum; bitNum++ ) {
      if ( 31 < offset ) {
        offset -= 32;
        lastData = pop( data );
      }
      int adc;
      if ( 0 == ( pattern & ( 1 << bitNum ) ) ) { //.. short coding
        adc = ( ( lastData >> offset ) & 0xF ) - 8;
        offset += 4;
      } else {
        adc = ( ( lastData >> offset ) & 0xFFF );
        if ( 24 == offset ) adc &= 0xFF;
        if ( 28 == offset ) adc &= 0xF; //== clean-up extra bits
        offset += 12;
        if ( 32 < offset ) { //.. get the extra bits on next word
          lastData = pop( data );
          offset -= 32;
          int temp = ( lastData << ( 12 - offset ) ) & 0xFFF;
          adc += temp;
        }
        adc -= 256;
      }

      LHCb::CaloCellID id = ( bitNum < chanID.size() ? chanID[bitNum] : LHCb::CaloCellID() );

      // event dump
      verbose() << " |  SourceID : " << sourceID << " |  FeBoard : " << calo.cardNumber( id )
                << " |  Channel : " << bitNum << " |  CaloCell " << id << " |  valid ? " << calo.valid( id )
                << " |  ADC value = " << adc << endmsg;

      //== Keep only valid cells
      if ( 0 != id.index() ) {
        LHCb::CaloAdc temp( id, adc );
        if ( id.isPin() == ( decode == Decode::PinData ) ) dataVec.push_back( temp );
      }
    }
  }
  // Check All cards have been read
  if ( !checkCards( calo, nCards, feCards ) )
    status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Incomplete );

  return dataVec;
}
//=============================================================================
template <CaloFutureRawToDigits::Decode decode>
std::vector<LHCb::CaloAdc> CaloFutureRawToDigits::decode_v3( int sourceID, LHCb::span<const unsigned int> data,
                                                             LHCb::RawBankReadoutStatus& status,
                                                             const DeCalorimeter&        calo ) const {
  std::vector<LHCb::CaloAdc> dataVec;
  dataVec.reserve( decode == Decode::PinData ? calo.numberOfPins() : calo.numberOfCells() );
  //******************************************************************
  //**** 1 MHz compression format, Preshower + SPD
  //******************************************************************

  // Get the FE-Cards associated to that bank (via condDB)
  std::vector<int> feCards = calo.tell1ToCards( sourceID );
  int              nCards  = feCards.size();
  if ( msgLevel( MSG::DEBUG ) )
    debug() << nCards << " FE-Cards are expected to be readout : " << feCards << " in Tell1 bank " << sourceID
            << endmsg;
  int prevCard = -1;
  while ( !data.empty() ) {
    // Skip
    unsigned int word = pop( data );
    // Read bank header
    int lenTrig = word & 0x7F;
    int lenAdc  = ( word >> 7 ) & 0x7F;
    int code    = ( word >> 14 ) & 0x1FF;
    int ctrl    = ( word >> 23 ) & 0x1FF;
    checkCtrl( ctrl, sourceID, status );
    // access chanID via condDB
    // look for the FE-Card in the Tell1->cards vector
    int card = findCardbyCode( calo, feCards, code );
    if ( card < 0 ) {
      error() << " FE-Card w/ [code : " << code << " ] is not associated with TELL1 bank sourceID : " << sourceID
              << " in condDB :  Cannot read that bank" << endmsg;
      error() << "Warning : previous data may be corrupted" << endmsg;
      if ( m_cleanCorrupted && calo.isPinCard( prevCard ) == ( decode == Decode::PinData ) ) {
        auto hasBadCardNumber = [&]( const LHCb::CaloAdc& adc ) {
          return calo.cellParam( adc.cellID() ).cardNumber() == prevCard;
        };
        dataVec.erase( std::remove_if( dataVec.begin(), dataVec.end(), hasBadCardNumber ), dataVec.end() );
      }

      status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Corrupted |
                                      LHCb::RawBankReadoutStatus::Status::Incomplete );
    }
    std::vector<LHCb::CaloCellID> chanID = calo.cardChannels( feCards[card] );
    feCards.erase( feCards.begin() + card );
    prevCard = card;

    // Read the FE-Board
    // skip the trigger bits
    advance( data, ( lenTrig + 3 ) / 4 ); //== Length in byte, with padding

    // read data
    int          offset   = 32; //== force read the first word, to have also the debug for it.
    unsigned int lastData = 0;

    while ( 0 < lenAdc ) {
      if ( 32 == offset ) {
        lastData = pop( data );
        offset   = 0;
      }
      int          adc = ( lastData >> offset ) & 0x3FF;
      unsigned int num = ( lastData >> ( offset + 10 ) ) & 0x3F;

      LHCb::CaloCellID id = ( num < chanID.size() ? chanID[num] : LHCb::CaloCellID{} );

      // event dump
      verbose() << " |  SourceID : " << sourceID << " |  FeBoard : " << calo.cardNumber( id ) << " |  Channel : " << num
                << " |  CaloCell " << id << " |  valid ? " << calo.valid( id ) << " |  ADC value = " << adc << endmsg;

      if ( 0 != id.index() && id.isPin() == ( decode == Decode::PinData ) ) dataVec.emplace_back( id, adc );

      --lenAdc;
      offset += 16;
    }
  } //== DataSize
  // Check All cards have been read
  if ( !checkCards( calo, nCards, feCards ) )
    status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Incomplete );
  return dataVec;
}
//=============================================================================
// Main method to decode the rawBank
//=============================================================================
template <CaloFutureRawToDigits::Decode decode>
std::vector<LHCb::CaloAdc> CaloFutureRawToDigits::decode_( const LHCb::RawBank&        bank,
                                                           LHCb::RawBankReadoutStatus& status,
                                                           const DeCalorimeter&        calo ) const {
  if ( LHCb::RawBank::MagicPattern != bank.magic() ) return {}; // do not try to decode when MagicPattern is bad
  // Get bank info
  auto data     = bank.range<unsigned int>();
  int  version  = bank.version();
  int  sourceID = bank.sourceID();

  if ( msgLevel( MSG::DEBUG ) )
    debug() << "Decode bank " << &bank << " source " << sourceID << " version " << version << " size " << data.size()
            << endmsg;

  if ( data.empty() ) status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Empty );

  // skip detector specific header line
  if ( m_extraHeader ) pop( data ); // drop first word

  switch ( version ) {
  case 1:
    return decode_v1<decode>( sourceID, data, status, calo );
  case 2:
    return decode_v2<decode>( sourceID, data, status, calo );
  case 3:
    return decode_v3<decode>( sourceID, data, status, calo );
  default:
    warning() << "Bank type " << bank.type() << " sourceID " << sourceID << " has version " << version
              << " which is not supported" << endmsg;
    return {};
  }
}

//========================
//  Check FE-Cards is PIN
//========================
bool CaloFutureRawToDigits::checkCards( const DeCalorimeter& calo, int nCards, const std::vector<int>& feCards ) const {
  bool check = true;
  if ( msgLevel( MSG::DEBUG ) )
    debug() << nCards - feCards.size() << "FE-Cards have been read among the " << nCards << " expected" << endmsg;
  for ( auto i : feCards ) {
    if ( msgLevel( MSG::DEBUG ) )
      debug() << " Unread FE-Cards : " << calo.cardCode( i ) << "  - Is it a PinDiode readout FE-Card ? "
              << calo.isPinCard( i ) << endmsg;
    if ( calo.isPmtCard( i ) ) {
      warning() << " The standard (PMT) FE-Card " << calo.cardCode( i )
                << " expected in TELL1 bank has not been read !!" << endmsg;
      check = false;
    }
  }
  return check;
}

//===========================
//  Find Card number by code
//===========================
int CaloFutureRawToDigits::findCardbyCode( const DeCalorimeter& calo, const std::vector<int>& feCards,
                                           int code ) const {
  auto i = std::find_if( feCards.begin(), feCards.end(), [&]( int ife ) { return code == calo.cardCode( ife ); } );
  if ( i == feCards.end() ) {
    error() << "FE-Card [code : " << code << "] does not match the condDB cabling scheme  " << endmsg;
    return -1;
  }
  if ( msgLevel( MSG::DEBUG ) )
    debug() << " FE-Card [code : " << code << " | crate : " << calo.cardParam( *i ).crate()
            << " slot : " << calo.cardParam( *i ).slot() << "] has been found with (num : " << *i << ")  in condDB"
            << endmsg;
  return std::distance( feCards.begin(), i );
}

void CaloFutureRawToDigits::checkCtrl( int ctrl, int sourceID, LHCb::RawBankReadoutStatus& status ) const {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "Control word :" << ctrl << endmsg;
  if ( 0 != ( 0x01 & ctrl ) || 0 != ( 0x20 & ctrl ) || 0 != ( 0x40 & ctrl ) ) {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Tell1 error bits have been detected in data" << endmsg;
    if ( 0 != ( 0x01 & ctrl ) ) status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Tell1Error );
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
//     long nCells = calo.numberOfCells();
//     digitsVec.reserve( nCells );
//     double pedShift = calo.pedestalShift();
//     std::transform( dataVec.begin(), dataVec.end(),
//                     std::back_inserter(digitsVec),
//                     [&](const LHCb::CaloAdc& cadc) -> LHCb::CaloDigit {
//                        LHCb::CaloCellID id = cadc.cellID();
//                        double e = ( double(cadc.adc()) - pedShift ) * calo.cellGain( id );
//                        return { id, e };
//     } );
//     return digitsVec;
//   }
