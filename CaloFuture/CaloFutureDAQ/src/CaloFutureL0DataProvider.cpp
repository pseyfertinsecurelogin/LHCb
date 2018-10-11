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
// Include files
// local
#include "CaloFutureL0DataProvider.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CaloFutureL0DataProvider
//
// 2005-01-10 : Olivier Deschamps
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( CaloFutureL0DataProvider )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CaloFutureL0DataProvider::CaloFutureL0DataProvider( const std::string& type,
                                        const std::string& name,
                                        const IInterface* parent )
: CaloFutureReadoutTool ( type, name , parent )
{

}
//=========================================================================
//  Initialisation, according to the name -> detector
//=========================================================================
StatusCode CaloFutureL0DataProvider::initialize ( ) {
  StatusCode sc = CaloFutureReadoutTool::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << "==> Initialize " << name() << endmsg;

  switch(m_detectorName) {
  case futuredetails::DetectorName_t::Ecal:
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Ecal );
    m_packedType = LHCb::RawBank::EcalPacked;
    m_shortType  = LHCb::RawBank::EcalTrig;
    m_errorType = LHCb::RawBank::EcalPackedError;
    break;
  case futuredetails::DetectorName_t::Hcal:
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Hcal );
    m_packedType = LHCb::RawBank::HcalPacked;
    m_shortType  = LHCb::RawBank::HcalTrig;
    m_errorType = LHCb::RawBank::HcalPackedError;
    break;
  case futuredetails::DetectorName_t::Prs:
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Prs );
    m_packedType = LHCb::RawBank::PrsPacked;
    m_shortType  = LHCb::RawBank::PrsTrig;
    m_errorType = LHCb::RawBank::PrsPackedError;
    break;
  case futuredetails::DetectorName_t::Spd:
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Prs ); // Prs FE for SPD
    m_packedType = LHCb::RawBank::PrsPacked;
    m_shortType  = LHCb::RawBank::PrsTrig;
    m_errorType = LHCb::RawBank::PrsPackedError;
    break;
  default:
    error() << "Unknown detector name " << toString(m_detectorName) << endmsg;
    return StatusCode::FAILURE;
  }

  long nCells = m_calo->numberOfCells();
  long nPins  = m_calo->numberOfPins();
  m_adcs.reserve( nCells + nPins  );
  clear();
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << " Initialisation OK" << endmsg;
  return StatusCode::SUCCESS;
}

//-------------------------------------------------------
// Public methods :
//-------------------------------------------------------

//-------------------------------------
void CaloFutureL0DataProvider::clear( ) {
  m_adcs.clear();
  m_tell1s = 0;
  m_readSources.clear();
}


//-------------------------------------
void CaloFutureL0DataProvider::cleanData(int feb ) {
  if(feb<0)return;
  CaloVector<LHCb::L0CaloAdc> temp;
  for(const auto& adc : m_adcs ) {
    if( m_calo->cellParam( adc.cellID() ).cardNumber() == feb)continue;
    temp.addEntry( adc, adc.cellID() );
  }
  m_adcs = std::move(temp);
}


//===================
//  Get ADC Container
//===================
const CaloVector<LHCb::L0CaloAdc>& CaloFutureL0DataProvider::l0Adcs(int source,bool clean){
  if (clean) clear();
  decodeTell1( !m_packed ? -1 : source);
  return m_adcs;
}
const CaloVector<LHCb::L0CaloAdc>& CaloFutureL0DataProvider::l0Adcs(std::vector<int> sources,bool clean){
  if( clean)clear();
  if( m_getRaw )getBanks();
  if( !m_packed) return l0Adcs(); // decode the single 'offline' bank
  for(auto i : sources) decodeTell1(i);
  return m_adcs;
}

//-------------------------------------------------------
int CaloFutureL0DataProvider::l0Adc (LHCb::CaloCellID id,int def){
  if( m_getRaw )getBanks();
  if( 0 >  m_adcs.index(id) )decodeCell( id );
  if( 0 >  m_adcs.index(id) )return def; // 0-suppressed data or non-valid CellID
  return m_adcs[id].adc();
}

//-------------------------------------------------------
// Protected methods :
//-------------------------------------------------------
bool CaloFutureL0DataProvider::decodeCell(LHCb::CaloCellID id ){
  // for packed banks only (sourceID = Tell1 ID)

  int tell1 = -1;// Decode the whole 0-suppressed bank by default (single bank)
  bool read = false;

  if( m_packed){
    int card = m_calo->cardNumber (id)   ; // Fe-Card from cellId
    if(card<0)return false;
    tell1 = m_calo->cardToTell1(card); // Tell1 from FE-Card
    if(tell1<0)return false;
    read = std::any_of( m_readSources.begin(), m_readSources.end(),
                        [&](int i) { return i == tell1 ; } );
  }else{
    read = std::any_of( m_readSources.begin(), m_readSources.end(),
                        [](int i) { return i == 0; } );
  }
  return read || decodeTell1( tell1 );
}
//-------------------------------------------------------
bool CaloFutureL0DataProvider::decodeTell1 (int source) {
  if( source < 0)clear(); // re-init for full decoding
  bool decoded = false;
  bool found  = false;
  if (m_getRaw) getBanks();

  for( const LHCb::RawBank* bank : m_banks ) {
    int sourceID       = bank->sourceID();
    if( source >= 0 && source != sourceID )continue;
    found = true;

    if(checkSrc( sourceID ))continue;

    decoded = ( ( futuredetails::DetectorName_t::Spd == m_detectorName || futuredetails::DetectorName_t::Prs == m_detectorName ) ?
                decodePrsTriggerBank( *bank) : decodeBank ( *bank ) );
    if( !decoded ){
      if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
        debug() << "Error when decoding bank " << Gaudi::Utils::toString(sourceID)
                << " -> incomplete data - May be corrupted"<<endmsg;
    }
    ++m_tell1s; // count the number of decoded TELL1
  }
  if( !found ){
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() <<"rawBank sourceID : " + Gaudi::Utils::toString(source) + " has not been found"<< endmsg;
  }
  return decoded;
}
//==================================
// Main method to decode the rawBank
//==================================
bool CaloFutureL0DataProvider::decodeBank( const LHCb::RawBank& bank ){
  if( LHCb::RawBank::MagicPattern != bank.magic() )return false;// do not decode when MagicPattern is bad
  // Get bank info
  const unsigned int* data       = bank.begin<unsigned int>();
  const unsigned int* const end  = bank.end<unsigned int>();
  int version        = bank.version();
  int sourceID       = bank.sourceID();
  unsigned int lastData       = 0;

  if(data == end)m_status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Empty);

  if ( msgLevel( MSG::DEBUG) )
    debug() << "Decode bank " << &bank << " source " << sourceID
            << " version " << version << " size " << std::distance(data,end) << endmsg;

  // -----------------------------------------------
  // skip detector specific header line
  if(m_extraHeader) ++data ;
  // -----------------------------------------------



  //=== Offline coding
  if ( 2 > version ) {
    while ( data < end ) {
      int lastID   = (*data) >> 16;
      int adc1     = (*data)>>8 & 0xFF;
      int adc2     = (*data)    & 0xFF;
      ++data;

      LHCb::CaloCellID id1( lastID );
      if ( 0 != adc1 )fillL0ADC( id1, adc1, sourceID);

      //event dump
      if ( msgLevel( MSG::VERBOSE) ) {
        verbose() << " |  SourceID : " << sourceID
                << " |  FeBoard : " << m_calo->cardNumber(id1)
                << " |  CaloFutureCell " << id1
                << " |  valid ? " << m_calo->valid(id1)
                << " |  ADC value = " << adc1 << endmsg;
      }

      LHCb::CaloCellID id2( ++lastID );
      if ( 0 != adc2 )fillL0ADC(id2,adc2,sourceID);


      //event dump
      if ( msgLevel( MSG::VERBOSE) ) {
        verbose() << " |  SourceID : " << sourceID
                << " |  FeBoard : " << m_calo->cardNumber(id2)
                << " |  CaloFutureCell " << id2
                << " |  valid ? " << m_calo->valid(id2)
                << " |  ADC value = " << adc2 << endmsg;
      }
    }
  } else if ( 2 == version ) {
    //==== Codage for 1 MHz
    // Get the FE-Cards associated to that bank (via condDB)
    std::vector<int> feCards = m_calo->tell1ToCards( sourceID );
    int nCards = feCards.size();
    if ( msgLevel( MSG::DEBUG) )debug() << nCards << " FE-Cards are expected to be readout : "
                                        << feCards << " in Tell1 bank " << sourceID << endmsg;

    int prevCard = -1;

    while ( data < end ) {
      int word = *data++;
      int lenTrig = word & 0x3F;
      int lenAdc  = (word >> 7 ) & 0x3F;
      //lenAdc += 4; //PATCH FOR COMMISSIONING DATA WITH TELL1 F/W v2.3

      int code  = (word >> 14 ) & 0x1FF;
      int ctrl    = (word >> 23) &  0x1FF;
      checkCtrl( ctrl,sourceID );
      // access chanID via condDB
      std::vector<LHCb::CaloCellID> chanID  ;
      int card = findCardbyCode(feCards,code);
      if( 0 <= card ){
        chanID = m_calo->cardChannels( feCards[card] );
        feCards.erase(feCards.begin()+card);
      }else{
        Error(" FE-Card w/ [code : " + Gaudi::Utils::toString(code)
              + " ] is not associated with TELL1 bank sourceID : " +Gaudi::Utils::toString(sourceID)
              + " in condDB :  Cannot read that bank").ignore();
        Error("Warning : previous data may be corrupted").ignore();
        if(m_cleanCorrupted)cleanData(prevCard);
        m_status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Incomplete );
        m_status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Corrupted  );
        return false;
      }
      prevCard = card;

      // Start readout of the FE-board (trigger data)
      if ( 0 < lenTrig ) {
        int pattern  = *data++;
        int offset   = 0;
        if ( msgLevel( MSG::DEBUG) )debug() << format( " pattern %8x lenTrig %2d", pattern, lenTrig ) << endmsg;
        lastData  = *data++;
        for (unsigned int bitNum = 0 ; 32 > bitNum ; bitNum++ ) {
          if ( 0 != (pattern & (1<<bitNum)) ) {
            if ( 31 < offset ) {
              offset   = 0;
              lastData = *data++;
            }

            if( offset == 0)
              if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
                debug() << "Data word : " << lastData << endmsg;

            LHCb::CaloCellID id = LHCb::CaloCellID();
            if(bitNum < chanID.size())id= chanID[ bitNum ];

            int adc = ( lastData >> offset ) & 0xFF;

            // event dump
            if ( msgLevel( MSG::VERBOSE) ) {
              verbose() << " |  SourceID : " << sourceID
                        << " |  FeBoard : " << code
                        << " |  Channel : " << bitNum
                        << " |  CaloFutureCell " << id
                        << " |  valid ? " << m_calo->valid(id)
                        << " |  ADC value = " << adc << endmsg;
            }

            if ( 0 != id.index() )fillL0ADC(id,adc,sourceID);

            offset += 8;
          }
        }
      }
      // Skip ADC data
      data     += (lenAdc+3)/4;  //== length in byte, skip words
    } // another card
    // Check All cards have been read
    if(!checkCards(nCards,feCards))m_status.addStatus(sourceID, LHCb::RawBankReadoutStatus::Status::Incomplete);
  } // version
  return true;
}



//==================================
bool CaloFutureL0DataProvider::decodePrsTriggerBank( const LHCb::RawBank& bank ) {

  if( LHCb::RawBank::MagicPattern != bank.magic() )return false;// do not decode when MagicPattern is bad


  const unsigned int* data       = bank.begin<unsigned int>();
  const unsigned int* const end  = bank.end<unsigned int>();
  int version        = bank.version();
  int sourceID       = bank.sourceID();
  unsigned int lastData       = 0;

  m_status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::OK);

  if ( msgLevel( MSG::DEBUG) )
    debug() << "Decode Prs bank " << &bank << " source " << sourceID
            << " version " << version << " size " << std::distance(data,end) << endmsg;


  // -----------------------------------------------
  // skip detector specific header line
  if(m_extraHeader){
    ++data ;
  }
  // -----------------------------------------------



  //=== Offline coding: a CellID, 8 SPD bits, 8 Prs bits
  if ( 1 == version ) {
    while ( data != end ) {
      int spdData = (*data >> 8 ) & 0xFF;
      int prsData = (*data) & 0xFF;
      int lastID  = (*data) >> 16;
        ++data;
        for ( unsigned int kk = 0; 8 > kk; ++kk ) {

          LHCb::CaloCellID id( lastID+kk );
          LHCb::CaloCellID spdId( (lastID+kk) & 0x3FFF );


          if ( futuredetails::DetectorName_t::Spd == m_detectorName)
            fillL0ADC(spdId,(spdData&1),sourceID);
          else
            fillL0ADC(id,(prsData&1),sourceID);


          //event dump
          if ( msgLevel( MSG::VERBOSE) ) {
            verbose() << " |  SourceID : " << sourceID
                      << " |  FeBoard : " << m_calo->cardNumber(id)
                      << " |  CaloFutureCell " << id
                      << " |  valid ? " << m_calo->valid(id)
                      << " |  Prs/Spd  = " << (prsData & 1) << "/" << (spdData & 1) << endmsg;
          }

          spdData = spdData >> 1;
          prsData = prsData >> 1;
        }
    }
    //=== Compact coding: a CellID, and its Prs/SPD bits
  } else if ( 2 == version ) {
    while ( data != end ) {
      int word = *data;
      while ( 0 != word ) {
        int item = word & 0xFFFF;
        word = ( word >> 16 ) & 0xFFFF;
        int spdId =  (item&0xFFFC) >> 2;

        //event dump
        LHCb::CaloCellID prsId( spdId + 0x4000 );   // Prs
        if ( msgLevel( MSG::VERBOSE) ) {
          verbose() << " |  SourceID : " << sourceID
                    << " |  FeBoard : " << m_calo->cardNumber( prsId )
                    << " |  CaloFutureCell " << prsId
                    << " |  valid ? " << m_calo->valid( prsId )
                    << " |  Prs/Spd  = " << (item&1) << "/" << (item&2) << endmsg;
        }


        if( 0 != spdId ){
          if ( futuredetails::DetectorName_t::Spd == m_detectorName) {
            if( (item&2) != 0 ){
              LHCb::CaloCellID id ( spdId );   // SPD
              fillL0ADC(id,1,sourceID);
            }
          }
          else {
            if( (item&1) != 0 ){
              fillL0ADC(prsId,1,sourceID);
            }
          }
        }
      }
      ++data;
    }
    //==== Codage for 1 MHz
  } else if ( 3 == version ) {
    // Get the FE-Cards associated to that bank (via condDB)
    std::vector<int> feCards = m_calo->tell1ToCards( sourceID );
    int nCards = feCards.size();
    if ( msgLevel( MSG::DEBUG) )debug() << nCards << " FE-Cards are expected to be readout : "
                                        << feCards << " in Tell1 bank " << sourceID << endmsg;

    int offset   = 0;
    int lenAdc   = 0;
    int lenTrig  = 0;

    int prevCard = -1;

    while( data != end ) {
      int word = *data++;
      lenTrig = word & 0x7F;
      lenAdc  = (word >> 7 ) & 0x7F;
      if ( msgLevel( MSG::DEBUG) ) {
        debug() << format( "  Header data %8x size %4d lenAdc%3d lenTrig%3d",
                           word, std::distance(data,end), lenAdc, lenTrig )
                << endmsg;
      }
      int code  = (word >>14 ) & 0x1FF;
      int ctrl    = (word >> 23) &  0x1FF;
      checkCtrl( ctrl,sourceID );
      if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
        debug() << "Read FE-board ["<< code << "] linked to TELL1 bank " << sourceID << endmsg;
      // access chanID via condDB
      std::vector<LHCb::CaloCellID> chanID  ;
      // look for the FE-Card in the Tell1->cards vector
      int card = findCardbyCode(feCards,code);
      if( 0 <= card ){
        chanID = m_calo->cardChannels( feCards[card] );
        feCards.erase(feCards.begin()+card);
      }else{
        Error(" FE-Card w/ [code : " + Gaudi::Utils::toString(code)
              + " ] is not associated with TELL1 bank sourceID : " +Gaudi::Utils::toString(sourceID)
              + " in condDB :  Cannot read that bank").ignore();
        Error("Warning : previous data may be corrupted").ignore();
        if(m_cleanCorrupted)cleanData(prevCard);
        m_status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Corrupted | LHCb::RawBankReadoutStatus::Status::Incomplete);
        return false;
      }
      prevCard = card;

      // Process FE-data decoding
      offset   = 32;
      while ( 0 < lenTrig ) {
        if ( 32 == offset ) {
          lastData = *data++;
          offset = 0;
        }
        unsigned int num   = ( lastData >> offset ) & 0x3F;
        int isPrs = ( lastData >> (offset +6)) & 1;
        int isSpd = ( lastData >> (offset +7)) & 1;
        offset += 8;
        lenTrig--;

        LHCb::CaloCellID id = LHCb::CaloCellID();
        if(num < chanID.size())id= chanID[ num ];


        // event dump
        if ( msgLevel( MSG::VERBOSE) ) {
          verbose() << " |  SourceID : " << sourceID
                    << " |  FeBoard : " << code
                    << " |  Channel : " << num
                    << " |  CaloFutureCell " << id
                    << " |  valid ? " << m_calo->valid(id)
                    << " |  Prs/Spd  = " << isPrs << "/" << isSpd << endmsg;
        }


        if ( 0 != id.index() ){
          if ( futuredetails::DetectorName_t::Spd == m_detectorName) {
            LHCb::CaloCellID spdId( 0, id.area(), id.row(), id.col() );
            fillL0ADC(spdId,isSpd,sourceID);
         }
          else {
            fillL0ADC(id,isPrs,sourceID);
          }
        }
      }

      int nSkip = (lenAdc+1 ) / 2;  // Length in number of words
      data     += nSkip;
    } //== DataSize
    // Check All cards have been read
    if(!checkCards(nCards,feCards))m_status.addStatus(sourceID, LHCb::RawBankReadoutStatus::Status::Incomplete);
  } //== versions
  return true;
}
