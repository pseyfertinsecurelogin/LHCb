
#include <boost/format.hpp>
#include <boost/format/group.hpp>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <string>
#include <vector>

#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GaudiCommonImp.h"
#include "GaudiAlg/IGenericTool.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/IProperty.h"
#include "GaudiKernel/IRndmEngine.h"
#include "GaudiKernel/IRndmGenSvc.h"
#include "GaudiKernel/IService.h"
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/Memory.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/RndmGenerators.h"
// from Gaudi
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/Time.h"
#include "Kernel/CondDBNameTagPair.h"
#include "Kernel/ICondDBInfo.h"
#include "Kernel/IEventCounter.h"
// local
#include "Kernel/LbAppInit.h"

//-----------------------------------------------------------------------------
// Implementation file for class : LbAppInit
//
// 2005-12-21 : Marco Cattaneo
//-----------------------------------------------------------------------------

// Factory not declared here to avoid it appearing in the LHCbKernel library
// and in all component libraries depending on LHCbKernel. Moved to LHCbApps

//=============================================================================
// Initialization
//=============================================================================
StatusCode LbAppInit::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  auto appMgr = service( "ApplicationMgr" );
  auto propMgr = appMgr.as<IProperty>();
  std::string value ;
  sc = propMgr->getProperty( "EvtMax", value );
  if( sc.isFailure() )
    return Error( " Fatal error while retrieving Property EvtMax " );

  m_eventMax = std::atoi(value.c_str()) ;
  always()
    << "=================================================================="
    << endmsg;
  if( -1 == m_eventMax ) {
    always() << "Requested to process all events on input file(s)" << endmsg;
  }
  else {
    always() << "Requested to process " << m_eventMax << " events" << endmsg;
  }
  always()
    << "=================================================================="
    << endmsg;


  sc = propMgr->getProperty( "AppName", value );
  if( sc.isFailure() )
    return Error( " Fatal error while retrieving Property AppName " );
  m_appName = value;

  sc = propMgr->getProperty( "AppVersion", value );
  if( sc.isFailure() )
    return Error( " Fatal error while retrieving Property AppVersion " );
  m_appVersion = value;

  if( m_preload ) {
    IGenericTool* preloadTool = tool<IGenericTool>( "PreloadGeometryTool" );
    preloadTool->execute();
    release(preloadTool);
  }

  m_condDBInfo = service("CondDBCnvSvc", true );

  // Retrieve event counter tool
  m_evtCounter = tool<IEventCounter>(m_evtCounterName);

  if( m_memPurgeLimit != -1 ) 
    warning() << "MemPurgeLimit property is obsolete, please remove it" << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode LbAppInit::execute()
{

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> LbAppInit()::execute" << endmsg;

  const unsigned long long nev = eventCounter();

  if ( UNLIKELY( 0 == m_lastMem ) )
  {
    m_lastMem = System::virtualMemory();
  }
  else if ( UNLIKELY( 0 == nev%m_increment && m_increment > 0 ) )
  {
    unsigned long long mem     = System::virtualMemory();
    const    long long memDiff = (long long)(mem-m_lastMem);
    if ( UNLIKELY( std::abs(memDiff) >= m_minMemDelta ) )
    {
      info() << "Memory has changed from " << m_lastMem << " to " << mem << " KB"
             << " (" << memDiff << "KB, " << 100.*memDiff/m_lastMem << "%)"
             << " in last " << m_increment << " events" << endmsg ;
      m_lastMem = mem;
    }
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode LbAppInit::finalize()
{
  always()
    << "=================================================================="
    << endmsg;;
  always() << eventCounter() << " events processed" << endmsg;
  always()
    << "=================================================================="
    << endmsg;

  if( (-1 != m_eventMax) && (m_eventMax != eventCounter()) ) {
    warning()
      << "Should have processed " << m_eventMax << " events" << endmsg;
    warning()
      << "=================================================================="
      << endmsg;
  }

  return GaudiAlgorithm::finalize();  // must be called after all other actions
}

//=============================================================================
void LbAppInit::printEventRun( long long event, int run,
                               std::vector<long int>* seeds,
                               Gaudi::Time time ) const
{
  if ( this->okToPrint() )
  {
    info() << "Evt " << event << ",  Run " << run;
    if( m_printTime ) info() << ", UTC time "
                             << time.format(false,"%Y-%m-%d %H:%M:%S")
                             << "." << time.nanoformat(6);
    info() << ",  Nr. in job = " << eventCounter();
    if( 0 != seeds ) info() << " with seeds " << *seeds;
    info() << endmsg;
  }
}

//=============================================================================

StatusCode LbAppInit::initRndm( const std::vector<long int>& seeds )
{
  // Get the random number engine if not already done
  if( !m_randSvc ) {
    m_randSvc = service( "RndmGenSvc", true );
    if( !m_randSvc ) return Error( "Random number service not found!" );
  }
  if( !m_engine  ) {
    m_engine  = m_randSvc->engine();
    if( !m_engine ) return Error( "Random number engine not found!" );
  }
  StatusCode sc = m_engine->setSeeds( seeds );
  if( sc.isFailure() ) return Error( "Unable to set random number seeds", sc );

  // Optionally skip some random numbers
  if( 0 < m_skipFactor )
  {
    if ( this->okToPrint() )
    {
      info() << "Skipping " << m_skipFactor << " random numbers" << endmsg;
    }
    int shots  = m_skipFactor;
    double sum = 0.;
    Rndm::Numbers gauss;
    gauss.initialize( m_randSvc.get() , Rndm::Gauss(0.,1.0) );
    while( 0 < --shots ) { sum += gauss() * sum ; }
  }

  if(msgLevel(MSG::DEBUG)) debug() << "using seeds " << seeds << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
// Get the random number seeds vector to be used for this event
//=============================================================================
std::vector<long int> LbAppInit::getSeeds( unsigned int seed1, unsigned long long seed2 )
{

  std::vector<long int> seeds;

  // CLHEP engne requires positive int
  int seed1a = seed1 & 0x7FFFFFFF;

  // Make two 31 bit seeds out of seed2
  int seed2a = (int)(seed2 & 0x7FFFFFFF);
  int seed2b = (int)((seed2 >> 32) & 0x7FFFFFFF);

  if( !m_singleSeed ) {
    if( 0 != seed1a ) seeds.push_back( seed1a );
    if( 0 != seed2a ) seeds.push_back( seed2a );
    if( 0 != seed2b ) seeds.push_back( seed2b );
  }
  else {
    warning() << "Using only one 24 bit random number seed" << endmsg;
  }

  // Get last seed by hashing string containing seed1 and seed2
  const std::string s = name() +
    ( boost::io::str( boost::format( "_%1%_%2%" )
                      % boost::io::group( std::setfill('0'), std::hex, std::setw(8),  seed1 )
                      % boost::io::group( std::setfill('0'), std::hex, std::setw(16), seed2 ) ) );

  //--> Hash32 algorithm from Pere Mato
  int hash = 0;
  for( std::string::const_iterator iC = s.begin(); s.end() != iC; ++iC ) {
    hash += *iC; hash += (hash << 10); hash ^= (hash >> 6);
  }
  hash += (hash << 3); hash ^= (hash >> 11); hash += (hash << 15);
  //<--

  // CLHEP uses the last seed as a seed (only 24 bits used) but also to generate
  // more pseudorandom seeds to populate the "seeds" vector to its capacity of 24
  // seeds. For this generation, 31 bits are used
  seeds.push_back( abs(hash) );
  seeds.push_back( 0 );

  return seeds;
}

//=============================================================================
// Return vector of condDB tags used by CondDBSvc
//=============================================================================
const std::vector<LHCb::CondDBNameTagPair> LbAppInit::condDBTags() {
  std::vector<LHCb::CondDBNameTagPair> tmp;
  m_condDBInfo->defaultTags(tmp);
  return tmp;
}

//=============================================================================
// Return number of the current event
//=============================================================================
long LbAppInit::eventCounter() const {
  return m_evtCounter->getEventCounter();
}

//=============================================================================
