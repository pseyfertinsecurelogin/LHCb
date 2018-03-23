// Include files

// local
#include "CaloFutureTriggerBitsFromRaw.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CaloFutureTriggerBitsFromRaw
//
// 2005-01-06 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( CaloFutureTriggerBitsFromRaw )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CaloFutureTriggerBitsFromRaw::CaloFutureTriggerBitsFromRaw( const std::string& type,
                                                const std::string& name,
                                                const IInterface* parent )
  : CaloFutureReadoutTool ( type, name , parent )
{
  declareInterface<ICaloFutureTriggerBitsFromRaw>(this);
  clear();
}

//=========================================================================
//  Initialisation
//=========================================================================
StatusCode CaloFutureTriggerBitsFromRaw::initialize ( ) {
  StatusCode sc = CaloFutureReadoutTool::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiTool

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << "==> Initialize " << name() << endmsg;

  m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Prs );
  m_packedType = LHCb::RawBank::PrsPacked;
  m_shortType  = LHCb::RawBank::PrsTrig;
  m_errorType = LHCb::RawBank::PrsPackedError;

  long nCells = m_calo->numberOfCells();
  (m_data.first).reserve( nCells );
  (m_data.second).reserve( nCells );
  return StatusCode::SUCCESS;
}

//-------------------------------------
void CaloFutureTriggerBitsFromRaw::clear( ) {
  m_data.first.clear();
  m_data.second.clear();
  m_readSources.clear();
}

void CaloFutureTriggerBitsFromRaw::cleanData(int feb ) {
  if(feb<0)return;
  LHCb::CaloFuture::FiredCells& prs = m_data.first;
  LHCb::CaloFuture::FiredCells& spd = m_data.second;
  LHCb::CaloFuture::FiredCells tempSpd ;
  for(auto ispd = spd.begin();ispd != spd.end();++ispd){
    if( m_calo->cellParam( *ispd ).cardNumber() == feb)continue;
    tempSpd.push_back( *ispd );
  }
  LHCb::CaloFuture::FiredCells tempPrs ;
  for(auto iprs = prs.begin();iprs!=prs.end();++iprs){
    if( m_calo->cellParam( *iprs ).cardNumber() == feb)continue;
    tempPrs.push_back( *iprs );
  }
  clear();
  m_data = std::make_pair( std::move(tempPrs), std::move(tempSpd) );
}




//=========================================================================
//  Return prs or spd cells independently
//  Warning if both method are invoqued consecutively
//  the FULL decoding is run twice
//  --> in this case it's better to invoque prsSpdCells() method
//=========================================================================
const LHCb::CaloFuture::FiredCells& CaloFutureTriggerBitsFromRaw::prsCells () {
  const LHCb::CaloFuture::PrsSpdFiredCells& allCells = prsSpdCells();
  return allCells.first;
}
const LHCb::CaloFuture::FiredCells& CaloFutureTriggerBitsFromRaw::spdCells () {
  const LHCb::CaloFuture::PrsSpdFiredCells& allCells = prsSpdCells();
  return allCells.second;
}
//=========================================================================
//  Decode the fired Cells for ALL banks of one event
//=========================================================================
const LHCb::CaloFuture::PrsSpdFiredCells& CaloFutureTriggerBitsFromRaw::prsSpdCells () {
  return prsSpdCells( -1 );
}
//=========================================================================
//  Decode the TriggerBit for a single bank (given by sourceID)
//  Decode ALL banks if source < 0
//=========================================================================
const LHCb::CaloFuture::PrsSpdFiredCells& CaloFutureTriggerBitsFromRaw::prsSpdCells (int source ) {

  clear();
  bool decoded = false;
  bool found   = false;
  int sourceID     ;
  if(m_getRaw)getBanks();
  if( m_banks.empty() ){
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << "The banks container is empty" << endmsg;
  }else{
    for( const auto& itB : m_banks) {
      sourceID       = itB->sourceID();
      if( source >= 0 && source != sourceID )continue;
      found = true;
      if(checkSrc( sourceID ))continue;
      decoded = getData ( *itB );
      if( !decoded ){
        if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
          debug() << "Error when decoding bank " << Gaudi::Utils::toString(sourceID)
                  << " -> incomplete data - May be corrupted" << endmsg;
      }
    }
  }
  if( !found ){
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << "rawBank sourceID : " << Gaudi::Utils::toString(source) << " has not been found"<<endmsg;
  }
  return m_data;
}

//=========================================================================
//  Return appropriate containers (prs.or.spd) for a single bank (given by pointer)
//=========================================================================

const LHCb::CaloFuture::PrsSpdFiredCells& CaloFutureTriggerBitsFromRaw::prsSpdCells ( const LHCb::RawBank& bank ) {
  clear();
  if( ! getData( bank ))clear();
  return m_data;
}

//=========================================================================
//  Main decoding method fill both m_prsCells and m_spdCells containers.
//=========================================================================
bool CaloFutureTriggerBitsFromRaw::getData(const  LHCb::RawBank& bank ) {
  if( LHCb::RawBank::MagicPattern != bank.magic() )return false;// do not decode when MagicPattern is bad

  const unsigned int* data       = bank.begin<unsigned int>();
  const unsigned int* const end  = bank.end<unsigned int>();
  int version        = bank.version();
  int sourceID       = bank.sourceID();
  int lastData       = 0;

  if (data == end) m_status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Empty);

  if ( msgLevel( MSG::DEBUG) )debug() << "Decode bank " << &bank << " source " << sourceID
                                      << " version " << version << " size " << std::distance(data,end) << endmsg;

  // -----------------------------------------------
  // skip detector specific header line
  if(m_extraHeader) ++data ;
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

        if ( spdData & 1 ) m_data.second.push_back( spdId);
        if ( prsData & 1 ) m_data.first.push_back( id );

        //event dump
        if ( msgLevel(MSG::VERBOSE) ) {
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
          if ( 0 != (item & 2) ) {
            LHCb::CaloCellID id ( spdId );   // SPD
            m_data.second.push_back( id );
          }
          if ( 0 != (item & 1) ) {
            m_data.first.push_back( prsId );
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
      checkCtrl(ctrl,sourceID);

      if ( msgLevel( MSG::DEBUG) )debug() << "Read FE-board ["<< code << "] linked to TELL1 bank " << sourceID << endmsg;
      // access chanID via condDB
      std::vector<LHCb::CaloCellID> chanID  ;
      // look for the FE-Card in the Tell1->cards vector
      int card = findCardbyCode(feCards,code);
      if( 0 <= card ){
        chanID = m_calo->cardChannels( feCards[card] );
        feCards.erase(feCards.begin()+card);
      }else{
        Error(" FE-Card w/ [code : " + Gaudi::Utils::toString(code) + " ] is not associated with TELL1 bank sourceID : "
              +Gaudi::Utils::toString(sourceID)
              + " in condDB :  Cannot read that bank").ignore();
        Error("Warning : previous data may be corrupted").ignore();
        if(m_cleanCorrupted)cleanData(prevCard);
        m_status.addStatus( sourceID,   LHCb::RawBankReadoutStatus::Status::Incomplete);
        m_status.addStatus( sourceID,   LHCb::RawBankReadoutStatus::Status::Corrupted );
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
          if ( 0 != isPrs ) m_data.first.push_back( id );
          if ( 0 != isSpd ) {
            LHCb::CaloCellID spdId( 0, id.area(), id.row(), id.col() );
            m_data.second.push_back( spdId );
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
//=============================================================================