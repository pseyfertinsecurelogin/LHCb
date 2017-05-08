/// local
#include "L0HCAlg.h"

/// STL
#include <fstream>
#include <numeric>

// From Event
//#include "Event/L0CaloCandidate.h"
//#include "Event/L0CaloAdc.h"

// From Kernel
//#include "Kernel/CaloCellID.h"

//
//  Level-0 Herschel trigger emulation algorithm
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
  m_HCMult_B = std::vector< int >( 12 , 0 ) ;
  m_HCMult_F = std::vector< int >(  8 , 0 ) ;
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
  
  // Check if the mapping is available in the conditions database.
  const std::string location = "Conditions/ReadoutConf/HC/Mapping";
  if (existDet<Condition>(location)) {
    registerCondition(location, m_cond, &L0HCAlg::cacheMapping);
    // First update.
    sc = updMgrSvc()->update(this);
    if (sc.isFailure()) {
      return Error("Cannot update mapping.", StatusCode::FAILURE);
    }
  } else {
    warning() << "Cannot find " << location << " in database" << endmsg;
    mapChannels(m_channelsB0, 0, true);
    mapChannels(m_channelsB1, 1, true);
    mapChannels(m_channelsB2, 2, true);
    mapChannels(m_channelsF1, 1, false);
    mapChannels(m_channelsF2, 2, false);
  }
  
  // Get Herschel digits.
  l0digits = getIfExists<LHCb::HCDigits>(m_l0digitLocation);
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
  addHCData( ) ;
  
  // Save the candidates in HCProcessor data location (for L0DU) 
  LHCb::L0ProcessorDatas* L0HC = new LHCb::L0ProcessorDatas() ;
  std::string procName =  LHCb::L0ProcessorDataLocation::HC;
  put( L0HC, procName ) ;

  // Herschel B-side multiplicity counter
  int totHCMult_B = std::accumulate( m_HCMult_B.begin() , m_HCMult_B.end() , 0 ) ;
  code = 0x10000 + ( totHCMult_B << L0DUBase::Calo::Sum::Shift ) ; // what bit-shift should be employed here?
  LHCb::L0ProcessorData* HCMult_B = 
    new LHCb::L0ProcessorData ( L0DUBase::Fiber::CaloPi0Global, code ) ;
  L0HC -> add( HCMult_B ) ;

  // Herschel F-side multiplicity counter
  int totHCMult_F = std::accumulate( m_HCMult_F.begin() , m_HCMult_F.end() , 0 ) ;
  code = 0x10000 + ( totHCMult_F << L0DUBase::Calo::Sum::Shift ) ;
  LHCb::L0ProcessorData* HCMult_F = 
    new LHCb::L0ProcessorData ( L0DUBase::Fiber::CaloPi0Local, code ) ;
  L0HC -> add( HCMult_F ) ;
  
  // Debug now the L0 candidates
  if ( msgLevel( MSG::DEBUG ) ) {
    LHCb::L0ProcessorDatas::const_iterator item;
    debug() << "== L0ProcessorData Summary: " << L0HC->size() 
            << " entries." << endmsg ;
    for( item = L0HC->begin() ; L0HC->end() != item ; ++item ) {
      LHCb::L0ProcessorData* cand = (*item);
      debug() << format( "Key %2d Word %8x", cand->key(), cand->word() ) 
              << endmsg ;
    }
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
// Finalize.
//=============================================================================
StatusCode L0HCAlg::finalize() {
  if ( 0 != m_nbEvents ) {
    info() << format( "Average bank size : %7.1f words RAW.", 
                      m_totRawSize/m_nbEvents )
           << endmsg ;
  }
  return L0AlgBase::finalize(); 
}

//=============================================================================
// Add the Herschel information 
//=============================================================================
void L0HCAlg::addHCData( ) {

  // Assign zeros to m_HCMult_B and m_HCMult_F
  for ( unsigned int i = 0 ; i < m_HCMult_B.size() ; ++i )  
    m_HCMult_B[ i ] = 0 ;
  for ( unsigned int i = 0 ; i < m_HCMult_F.size() ; ++i )  
    m_HCMult_F[ i ] = 0 ;
  
  // Construct F-side sum
  for (unsigned int i = 0; i < 2; ++i) {
  	// Loop over all quadrants on this station
    for (unsigned int j = 0; j < 4; ++j) {
      // Find CellID for this quadrant
      LHCb::HCCellID id(m_channels[i][j]);
      
      // Retrieve the L0 digit 
      const LHCb::HCDigit* digit = l0digits->object(id); // ??TODO this ID is wrong.
      
      if (!digit) {
        warning() << "Cannot retrieve digit for " << ch << endmsg;
        continue;
      }
      
      // Extract and store the result
      int adc = digit->adc();
      m_HCMult_F[ i*4 + j ] = adc;
    }
  }
      
  // Construct B-side sum
  for (unsigned int i = 2; i < 5; ++i) {
  	// Loop over all quadrants on this station
    for (unsigned int j = 0; j < 4; ++j) {
      // Find CellID for this quadrant
      LHCb::HCCellID id(m_channels[i][j]);
      
      // Retrieve the L0 digit 
      const LHCb::HCDigit* digit = l0digits->object(id); // ??TODO this ID is wrong.
      
      if (!digit) {
        warning() << "Cannot retrieve digit for " << ch << endmsg;
        continue;
      }
      
      // Extract and store the result
      int adc = digit->adc();
      m_HCMult_F[ (i-2)*4 + j ] = adc;
    }
  }
}

//=============================================================================
//  Save the Herschel channel mapping
//=============================================================================
StatusCode L0HCAlg::cacheMapping() {
  // Extract crate IDs and channel numbers from CondDB
  m_crateB = m_cond->param<int>("CrateB");
  m_crateF = m_cond->param<int>("CrateF");
  m_channelsB0 = m_cond->paramVect<int>("ChannelsB0");
  m_channelsB1 = m_cond->paramVect<int>("ChannelsB1");
  m_channelsB2 = m_cond->paramVect<int>("ChannelsB2");
  m_channelsF1 = m_cond->paramVect<int>("ChannelsF1");
  m_channelsF2 = m_cond->paramVect<int>("ChannelsF2");
  mapChannels(m_channelsB0, 0, true);
  mapChannels(m_channelsB1, 1, true);
  mapChannels(m_channelsB2, 2, true);
  mapChannels(m_channelsF1, 1, false);
  mapChannels(m_channelsF2, 2, false);
  return StatusCode::SUCCESS;
}

//=============================================================================
// Setup the channel map for a given station.
//=============================================================================
bool L0HCAlg::mapChannels(const std::vector<int>& channels,
                                    const unsigned int station,
                                    const bool bwd) {
  // Indices 0,1 are F-side; 2,3,4 are B-side                                  
  const unsigned int offset = bwd ? 0 : 2;
  
  // Check if the input is valid and if not return 0 mapping
  if (channels.size()) {
    std::string s = bwd ? "B" : "F";
    s += std::to_string(station);
    warning() << "Invalid channel map for station " << s << endmsg;
    m_channels[station + offset].assign(4, 0);
    return false;
  }
  
  // Determine whether dealing with F-side or B-side crate
  const unsigned int crate = bwd ? m_crateB : m_crateF;
  
  // Resize mapping vector for 4 quadrants
  m_channels[station + offset].resize(4);

  // Loop over the four quadrants and assign channel numbers
  for (unsigned int i = 0; i < 4; ++i) {
    if (channels[i] < 0) {
      std::string s = bwd ? "B" : "F";
      s += std::to_string(station);
      warning() << "Invalid channel number " << channels[i]
                << " for quadrant " << s << i << endmsg;
      continue;
    }
    m_channels[station + offset][i] = (crate << 6) | channels[i];
  }
  return true;
}
