/// local
#include "L0HCAlg.h"

/// STL
#include <fstream>
#include <numeric>

// From Event
#include "Event/L0CaloCandidate.h"
#include "Event/L0CaloAdc.h"

// From Kernel
#include "Kernel/CaloCellID.h"

//
//  Level-0 Herschel trigger simulation algorithm
//  Compute L0HC quantities from Herschel full information
//

DECLARE_ALGORITHM_FACTORY( L0HCAlg )


//=============================================================================
// Constructor
//=============================================================================
L0HCAlg::L0HCAlg( const std::string & name , ISvcLocator * pSvcLocator)
  : L0AlgBase( name , pSvcLocator )
  , m_validPrs()
  , m_validPrsInner()
  , m_rawOutput  ( 2 )
{
  declareProperty("L0DigitLocation",
                  m_l0digitLocation = LHCb::HCDigitLocation::L0);
  m_spdMult = std::vector< int >( 16 , 0 ) ;
}

//=============================================================================
// Destructor
//=============================================================================
L0HCAlg::~L0HCAlg() { }

//=============================================================================
// Initialization: Retrieve the Herschel L0 trigger bits from the TES where 
//				   they should have been placed by HCRawBankDecoder
//=============================================================================
StatusCode L0HCAlg::initialize() {
  StatusCode sc = L0AlgBase::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc; 

  if( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;

  // Get Herschel digits.
  LHCb::HCDigits* l0digits = getIfExists<LHCb::HCDigits>(m_l0digitLocation);
  warning() << "No default HCDigits location has yet been defined -- beware" << endmsg;
  if ( !l0digits ) {
    // should always be available ...
    return Error( "Cannot load the HCDigits data object", StatusCode::SUCCESS );
  }
  
  return StatusCode::SUCCESS;
}

//=============================================================================
// Execute: Compute the calo trigger information
//=============================================================================
StatusCode L0HCAlg::execute() {
  // Get the Herschel information. Adds it to the m_ecalFe[] objects
  addHerschelData( ) ;
  
  // Save the candidates in CaloProcessor data location (for L0DU) 
  LHCb::L0ProcessorDatas* L0Calo = new LHCb::L0ProcessorDatas() ;
  std::string procName =  LHCb::L0ProcessorDataLocation::Calo; // NO L0-context location for procData
  put( L0Calo, procName ) ;

  // Spd multiplicity counter
  int totSpdMult = std::accumulate( m_spdMult.begin() , m_spdMult.end() , 0 ) ;
  code = 0x10000 + ( totSpdMult << L0DUBase::Calo::Sum::Shift ) ;
  LHCb::L0ProcessorData* spdMult = 
    new LHCb::L0ProcessorData ( L0DUBase::Fiber::CaloSpdMult, code ) ;
  L0Calo -> add( spdMult ) ;

  // Debug now the L0 candidates
  if ( msgLevel( MSG::DEBUG ) ) {
    LHCb::L0ProcessorDatas::const_iterator item;
    debug() << "== L0ProcessorData Summary: " << L0Calo->size() 
            << " entries." << endmsg ;
    for( item = L0Calo->begin() ; L0Calo->end() != item ; ++item ) {
      LHCb::L0ProcessorData* cand = (*item);
      debug() << format( "Key %2d Word %8x", cand->key(), cand->word() ) 
              << endmsg ;
    }
  }
  
  // Now save the L0CaloCandidates: create a RAW bank and then decode it
  // Prepare the raw banks, and save them.
  m_rawOutput[ 0 ].clear() ;
  m_rawOutput[ 1 ].clear() ;

  // saveInRawEvent( IO , SLAVE , MASK ) 
  // IO    --- 0 for input link, 1 for output of Selection board 
  // SLAVE --- 0 for all selection board except hadron ones 
  // MASK  --- always 0 on MC, used only in real data 

  // Rhe SPD part 
  std::vector< int >::iterator itSpd ;
  int kk = 0 ;
  for ( itSpd = m_spdMult.begin() ; itSpd != m_spdMult.end() ; ++itSpd )
    saveInRawEvent( 0 , 0 , 0 , L0DUBase::CaloType::SpdMult , kk++ , 
                    (*itSpd) , 1 ) ;
  saveInRawEvent( 1 , 0 , 0 , L0DUBase::CaloType::SpdMult , 0 , 
                  totSpdMult , 1 ) ;

  // Store the RAW bank if required.
  if ( writeBanks() ) {
    m_nbEvents++;
    m_totRawSize = m_totRawSize + m_rawOutput[0].size() + 
      m_rawOutput[1].size();
    LHCb::RawEvent* raw = 
      getOrCreate<LHCb::RawEvent,LHCb::RawEvent>( LHCb::RawEventLocation::Default );
    raw->addBank( 0, LHCb::RawBank::L0Calo, m_bankVersion , m_rawOutput[0] );
    raw->addBank( 1, LHCb::RawBank::L0Calo, m_bankVersion , m_rawOutput[1] );
  }

  if( writeOnTES() ) {
    std::string name = dataLocation( LHCb::L0CaloCandidateLocation::Default ) ;
    std::string nameFull = dataLocation( LHCb::L0CaloCandidateLocation::Full ) ;
    LHCb::L0CaloCandidates * outFull = new LHCb::L0CaloCandidates( ) ;
    LHCb::L0CaloCandidates * out = new LHCb::L0CaloCandidates() ;
    put( outFull , nameFull , IgnoreRootInTES ) ;
    put( out, name , IgnoreRootInTES ) ;
    if ( msgLevel( MSG::DEBUG ) ) 
      debug() << "L0CaloCandidatesFromRawBank Registered output in TES" 
              << endmsg ;

    LHCb::RawBankReadoutStatus readoutStatus( LHCb::RawBank::L0Calo ) ;
    m_bankToTES -> convertRawBankToTES( m_rawOutput, outFull, out , 
                                        m_bankVersion , readoutStatus );
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
// Finalize.
//=============================================================================
StatusCode L0CaloAlg::finalize() {
  if ( 0 != m_nbEvents ) {
    info() << format( "Average bank size : %7.1f words RAW.", 
                      m_totRawSize/m_nbEvents )
           << endmsg ;
  }
  return L0AlgBase::finalize(); 
}

//=============================================================================
// Add the Spd information to the ECAL Front-end card
//=============================================================================
void L0CaloAlg::addSpdData( ) {

  for ( unsigned int i = 0 ; i < m_spdMult.size() ; ++i )  
    m_spdMult[ i ] = 0 ;

  LHCb::CaloCellID id;

  // Get digits. Sum in front-end cards.
  // Adds to the (possible) neighboring cards if at the border (row/col = 0)
  // and if the card has neighboring cards
  int card, row,  col  ;
  int down, left, corner  ;
  int spdCard( 0 ) , slot( 0 ) , crate( 0 ) , identif( 0 ) ;

  std::vector<LHCb::CaloCellID>& ids = m_PrsSpdIds.second;
  if ( msgLevel( MSG::DEBUG ) ) 
    debug() << "Found " << ids.size() << " SPD bits" << endmsg;

  for ( std::vector<LHCb::CaloCellID>::const_iterator itID = ids.begin();
        ids.end() != itID; ++itID ) {
    id = *itID;

    if ( msgLevel( MSG::VERBOSE ) ) verbose() << id << endmsg;

    m_ecal->cardAddress(id, card, row, col );
    m_ecal->cardNeighbors( card, down, left, corner );    
 
    m_ecalFe[card].setSpd( col, row );
    if ( (0 == row) && (0 <= down) ) 
      m_ecalFe[down].setSpd( col, nRowCaloCard );
    if ( (0 == col) && (0 <= left) ) 
      m_ecalFe[left].setSpd( nColCaloCard, row );
    if ( (0 == col) && (0 == row) && (0 <= corner) ) 
      m_ecalFe[corner].setSpd( nColCaloCard, nRowCaloCard );
    
    spdCard  = m_prs -> cardNumber( id ) ;
    slot     = m_prs -> cardSlot( spdCard ) ;
    crate    = m_prs -> cardCrate( spdCard ) ;
    identif = 2 * crate ; 
    if ( slot >= 8 ) identif += 1 ;
    m_spdMult[ identif ] += 1 ;
    if ( m_spdMult[ identif ] > 1023 ) m_spdMult[ identif ] = 1023 ;
  }
}

//=============================================================================
//  Save a candidate in the Raw bank for SPD
//=============================================================================
void L0CaloAlg::saveInRawEvent ( int io , int slave , int mask , int type, 
                                 int cardNumber , int spdMult ,
                                 unsigned int bank ) {
  //== Coding for Raw: IO (msb 1 bit) , Slave (2 bits) ,
  // Mask (1 bit) , type (4 bits), id (4 bits) and
  // multiplicity (lsb 10 bits). 
  if ( 0 == spdMult ) return;

  if (type != L0DUBase::CaloType::SpdMult ) {
    warning()<<"Should be of type CaloSpdMult and is of type " 
             << type << endmsg ;
    return ;
  }  

  int word = ( io << 31 ) | ( slave << 29 ) | ( mask << 28 ) | 
    ( type << 24 ) | ( cardNumber << 10 ) | spdMult ;
  m_rawOutput[ bank ].push_back( word );
} 

