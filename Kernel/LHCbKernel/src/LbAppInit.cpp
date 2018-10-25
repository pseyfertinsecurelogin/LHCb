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
// Boost
#include "boost/format.hpp"

// from Gaudi
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/IProperty.h"
#include "GaudiKernel/IRndmEngine.h"
#include "GaudiKernel/IRndmGenSvc.h"
#include "GaudiKernel/RndmGenerators.h"
#include "GaudiAlg/IGenericTool.h"
#include "GaudiKernel/Memory.h"

// local
#include "Kernel/LbAppInit.h"
#include "Kernel/PlatformInfo.h"

#include "VectorClass/instrset.h"

//-----------------------------------------------------------------------------
// Implementation file for class : LbAppInit
//
// Marco Cattaneo, Sebastien Ponce
//-----------------------------------------------------------------------------

// Factory not declared here to avoid it appearing in the LHCbKernel library
// and in all component libraries depending on LHCbKernel. Moved to LHCbApps

namespace
{
  
  /** Generate a hash from a string
   *  Note this method generates undefined behaviour errors :-
   *  runtime error: left shift of 69513675 by 10 places cannot be represented in type 'int'
   *  runtime error: signed integer overflow: 643403938 + 1715636224 cannot be represented in type 'int'
   *  Suppress with compiler attribute. */
#if  defined (__GNUC__)
  __attribute__((no_sanitize_undefined))
#elif defined(__clang__) 
  // add something for clang ...
#endif
  int makeHash( const std::string_view s )
  {
    //--> Hash32 algorithm from Pere Mato
    int hash = 0;
    // NOTE : Do NOT modernise this with STL algorithms, as this breaks the attribute...
    for ( auto & c : s )
    {
      hash += c; 
      hash += (hash << 10); 
      hash ^= (hash >> 6);
    }
    hash += (hash << 3); 
    hash ^= (hash >> 11); 
    hash += (hash << 15);
    return hash;
  }

}

//=============================================================================
// Initialization
//=============================================================================
StatusCode LbAppInit::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if (sc.isFailure()) return sc;  // error printed already by GaudiAlgorithm

  auto appMgr = service("ApplicationMgr");
  auto propMgr = appMgr.as<IProperty>();
  std::string value ;
  sc = propMgr->getProperty("EvtMax", value);
  if (sc.isFailure())
    return Error(" Fatal error while retrieving Property EvtMax ");

  m_eventMax = std::atoi(value.c_str()) ;
  always()
    << "=================================================================="
    << endmsg;
  if (-1 == m_eventMax) {
    always() << "Requested to process all events on input file(s)" << endmsg;
  } else {
    always() << "Requested to process " << m_eventMax << " events" << endmsg;
  }
  always()
    << "=================================================================="
    << endmsg;


  sc = propMgr->getProperty("AppName", value);
  if (sc.isFailure())
    return Error(" Fatal error while retrieving Property AppName ");
  m_appName = value;

  sc = propMgr->getProperty("AppVersion", value);
  if (sc.isFailure())
    return Error(" Fatal error while retrieving Property AppVersion ");
  m_appVersion = value;

  if (m_preload) {
    IGenericTool* preloadTool = tool<IGenericTool>("PreloadGeometryTool");
    preloadTool->execute();
    release(preloadTool);
  }

  m_condDBInfo = service("CondDBCnvSvc", true);

  if( m_memPurgeLimit != -1 )
    warning() << "MemPurgeLimit property is obsolete, please remove it" << endmsg;

  return StatusCode::SUCCESS;
}

StatusCode LbAppInit::start() {
  StatusCode sc = GaudiAlgorithm::start(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  // make sure PlatformInfo instances report the right instructions set level
  // (it may not be the same as during application start if we use checkpointing)
  LHCb::PlatformInfo::s_hostInstrSetLevel = static_cast<std::uint16_t>( instrset_detect() );

  return sc;
}

//=============================================================================
// checkMem
//=============================================================================
void LbAppInit::checkMem() const {

  if (msgLevel(MSG::DEBUG)) debug() << "==> LbAppInit()::execute" << endmsg;

  if (UNLIKELY(0 == m_lastMem)) {
    m_lastMem = System::virtualMemory();
  } else if (UNLIKELY(0 == m_evtCounter%m_increment && m_increment > 0)) {
    unsigned long long mem     = System::virtualMemory();
    const    long long memDiff = (long long)(mem-m_lastMem);
    if (UNLIKELY(std::abs(memDiff) >= m_minMemDelta)) {
      info() << "Memory has changed from " << m_lastMem << " to " << mem << " KB"
             << " (" << memDiff << "KB, " << 100.*memDiff/m_lastMem << "%)"
             << " in last " << m_increment.value() << " events" << endmsg ;
      m_lastMem = mem;
    }
  }
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode LbAppInit::finalize() {
  always()
    << "=================================================================="
    << endmsg;;
  always() << eventCounter() << " events processed" << endmsg;
  always()
    << "=================================================================="
    << endmsg;

  if ((-1 != m_eventMax) && (m_eventMax != eventCounter())) {
    warning()
      << "Should have processed " << m_eventMax << " events" << endmsg;
    warning()
      << "=================================================================="
      << endmsg;
  }

  return GaudiAlgorithm::finalize();  // must be called after all other actions
}

void LbAppInit::printEventRun(long long event, int run,
                               std::vector<long int>* seeds,
                               Gaudi::Time time) const {
  if (this->okToPrint()) {
    info() << "Evt " << event << ",  Run " << run;
    if (m_printTime) info() << ", UTC time "
                             << time.format(false,"%Y-%m-%d %H:%M:%S")
                             << "." << time.nanoformat(6);
    info() << ",  Nr. in job = " << eventCounter();
    if (0 != seeds) info() << " with seeds " << *seeds;
    info() << endmsg;
  }
}

StatusCode LbAppInit::initRndm(const std::vector<long int>& seeds) const {
  // Get the random number engine (if not already done)
  std::call_once( m_randSvc_init, [&]() { m_randSvc = service("RndmGenSvc", true);
                                          if (m_randSvc) m_engine = m_randSvc->engine();
                                        } );
  if (!m_randSvc) return Error("Random number service not found!");
  if (!m_engine) return Error("Random number engine not found!");

  StatusCode sc = m_engine->setSeeds(seeds);
  if (sc.isFailure()) return Error("Unable to set random number seeds", sc);

  // Optionally skip some random numbers
  if (0 < m_skipFactor) {
    if (this->okToPrint())
    {
      info() << "Skipping " << m_skipFactor << " random numbers" << endmsg;
    }
    int shots  = m_skipFactor;
    double sum = 0.;
    Rndm::Numbers gauss;
    gauss.initialize(m_randSvc.get() , Rndm::Gauss(0.,1.0));
    while(0 < --shots) { sum += gauss() * sum ; }
  }

  if (msgLevel(MSG::DEBUG)) debug() << "using seeds " << seeds << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
// Get the random number seeds vector to be used for this event
//=============================================================================
std::vector<long int> LbAppInit::getSeeds(unsigned int seed1, unsigned long long seed2) const {

  std::vector<long int> seeds;

  // CLHEP engne requires positive int
  int seed1a = seed1 & 0x7FFFFFFF;

  // Make two 31 bit seeds out of seed2
  int seed2a = (int)(seed2 & 0x7FFFFFFF);
  int seed2b = (int)((seed2 >> 32) & 0x7FFFFFFF);

  if (!m_singleSeed) {
    if (0 != seed1a) seeds.push_back(seed1a);
    if (0 != seed2a) seeds.push_back(seed2a);
    if (0 != seed2b) seeds.push_back(seed2b);
  } else {
    warning() << "Using only one 24 bit random number seed" << endmsg;
  }

  // Get last seed by hashing string containing seed1 and seed2
  const std::string s = name() +
    (boost::io::str(boost::format("_%1%_%2%")
                      % boost::io::group(std::setfill('0'), std::hex, std::setw(8),  seed1)
                      % boost::io::group(std::setfill('0'), std::hex, std::setw(16), seed2)));
  const auto hash = makeHash(s);

  // CLHEP uses the last seed as a seed (only 24 bits used) but also to generate
  // more pseudorandom seeds to populate the "seeds" vector to its capacity of 24
  // seeds. For this generation, 31 bits are used
  seeds.push_back(abs(hash));
  seeds.push_back(0);

  return seeds;
}

//=============================================================================
// Return vector of condDB tags used by CondDBSvc
//=============================================================================
const std::vector<LHCb::CondDBNameTagPair> LbAppInit::condDBTags() const {
  std::vector<LHCb::CondDBNameTagPair> tmp;
  m_condDBInfo->defaultTags(tmp);
  return tmp;
}

//=============================================================================
