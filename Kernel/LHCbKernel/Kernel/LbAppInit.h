#ifndef LBAPPINIT_H
#define LBAPPINIT_H 1

// STL
#include <string>
#include <vector>

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/Time.h"

// from LHCbKernel
#include "Kernel/ICondDBInfo.h"
#include "Kernel/IEventCounter.h"

// Forward declarations
class IRndmEngine;
class IRndmGenSvc;

/** @class LbAppInit LbAppInit.h
 *
 *  General LHCb Initialisation algorithm
 *
 *  @author Marco Cattaneo
 *  @date   2005-12-21
 */

class LbAppInit : public GaudiAlgorithm
{

public:

  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override;     ///< Algorithm initialization
  StatusCode start     () override;     ///< Algorithm preparation for execution
  StatusCode execute   () override;     ///< Algorithm execution
  StatusCode finalize  () override;     ///< Algorithm finalization

protected:

  /// Return number of events processed
  long  eventCounter() const;

  /// Return name of application being run
  const std::string& appName() const
  {
    return m_appName;
  }

  /// Return version of application being run
  const std::string& appVersion() const
  {
    return m_appVersion;
  }

  /** Get the list of active CondDB tags
   *  @return vector of DB name, tag pairs. Empty if DB not available
   */
  const std::vector<LHCb::CondDBNameTagPair> condDBTags();

  /** Print the run number, event number and optional vector of seeds
   *  @param[in] evt event number
   *  @param[in] run run number
   *  @param[in] seeds (optional) vector of seeds
   *  @param[in] time (optional) time of the event
   */
  void printEventRun( long long evt,
                      int run,
                      std::vector<long int> *seeds = 0,
                      Gaudi::Time time = 0              ) const;

  /** Initialize the random number engine with the given seeds
   *  @param[in] seeds Vector of seeds
   *  @return    StatusCode
   */
  StatusCode initRndm( const std::vector<long int>& seeds );

  /** Get a vector of seeds. First three seeds are built from seed1 and seed2
   *  Last seed is hash including seed1, seed2 and name()
   *  If m_singleSeed is true, only last seed is returned
   *  @param[in] seed1 First  seed (typically run number)
   *  @param[in] seed2 Second seed (typically event number)
   *  @return    vector of seeds
   */
  std::vector<long int> getSeeds( unsigned int seed1, unsigned long long seed2 );

  /** Is printing OK for this event ?
   *  Checks the PrintFreq option to see how frequently mesages should be printed
   *  @retval true Messages should be printed this event
   *  @retval false Messages should be suppressed this event
   */
  inline bool okToPrint() const
  {
    return ( m_printFreq > 0 && 0 == (eventCounter()-1)%m_printFreq );
  }

private:

  Gaudi::Property<int>  m_skipFactor { this, "SkipFactor",          0 }; ///< Skip some random numbers
  Gaudi::Property<bool> m_singleSeed { this, "SingleSeed",      false }; ///< Use only one seed
  Gaudi::Property<bool> m_preload    { this, "PreloadGeometry", false }; ///< Preload the geometry

  /** @brief Property for event print frequency
   *  Values > 0 indicate the rate at which event messages should be printed
   *  Values < 0 suppress event messages entirely
   */
  Gaudi::Property<int>           m_printFreq      { this, "PrintFreq",             1 };
  Gaudi::Property<bool>          m_printTime      { this, "PrintEventTime",    false }; ///< Print also the event time
  Gaudi::Property<std::string>   m_evtCounterName { this, "EvtCounter", "EvtCounter" }; ///< Name of EventCounter tool
  Gaudi::Property<unsigned long long> m_increment { this, "Increment",           100 }; ///< Number of events to measure memory on
  Gaudi::Property<long long>     m_memPurgeLimit  { this, "MemoryPurgeLimit",     -1 }; ///< OBSOLETE
  Gaudi::Property<long long>     m_minMemDelta    { this, "MinMemoryDelta",       16 }; ///< Minimum memory delta (kB) to trigger a message

  // Member data
  IRndmEngine*         m_engine = nullptr;     ///< Pointer to random number engine
  SmartIF<IRndmGenSvc> m_randSvc;              ///< Pointer to random number service
  SmartIF<ICondDBInfo> m_condDBInfo;           ///< Pointer to Info interface of CondDB service
  IEventCounter*       m_evtCounter = nullptr; ///< Pointer to EventCounter interface
  long long            m_eventMax = 0;         ///< Number of events requested (ApplicationMgr.EvtMax)
  std::string          m_appName = "";         ///< Application Name
  std::string          m_appVersion = "";      ///< Application Version
  unsigned long long   m_lastMem = 0;          ///< Last memory usage

};
#endif // LBAPPINIT_H
