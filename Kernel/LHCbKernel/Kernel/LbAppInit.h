#ifndef LBAPPINIT_H
#define LBAPPINIT_H 1

// STL
#include <string>
#include <vector>
#include <mutex>
#include <atomic>

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/Time.h"

// from LHCbKernel
#include "Kernel/ICondDBInfo.h"

// Forward declarations
class IRndmEngine;
class IRndmGenSvc;

/** @class LbAppInit LbAppInit.h
 *
 *  General LHCb Initialisation algorithm
 *
 *  @author Marco Cattaneo, Sebastien Ponce
 */

class LbAppInit : public GaudiAlgorithm {

public:

  /// inherit constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override;     ///< Algorithm initialization
  StatusCode finalize  () override;     ///< Algorithm finalization

protected:

  /// Check memory status and print info if it raised too much
  void checkMem() const;

  /// Return number of events processed
  long eventCounter() const {
    return m_evtCounter;
  }

  /// Return name of application being run
  const std::string& appName() const {
    return m_appName;
  }

  /// Return version of application being run
  const std::string& appVersion() const {
    return m_appVersion;
  }

  /** Get the list of active CondDB tags
   *  @return vector of DB name, tag pairs. Empty if DB not available
   */
  const std::vector<LHCb::CondDBNameTagPair> condDBTags() const;

  /** Print the run number, event number and optional vector of seeds
   *  @param[in] evt event number
   *  @param[in] run run number
   *  @param[in] seeds (optional) vector of seeds
   *  @param[in] time (optional) time of the event
   */
  void printEventRun(long long evt,
                      int run,
                      std::vector<long int> *seeds = 0,
                      Gaudi::Time time = 0             ) const;

  /** Initialize the random number engine with the given seeds
   *  @param[in] seeds Vector of seeds
   *  @return    StatusCode
   */
  StatusCode initRndm(const std::vector<long int>& seeds) const;

  /** Get a vector of seeds. First three seeds are built from seed1 and seed2
   *  Last seed is hash including seed1, seed2 and name()
   *  If m_singleSeed is true, only last seed is returned
   *  @param[in] seed1 First  seed (typically run number)
   *  @param[in] seed2 Second seed (typically event number)
   *  @return    vector of seeds
   */
  std::vector<long int> getSeeds(unsigned int seed1, unsigned long long seed2) const;

  /** Is printing OK for this event ?
   *  Checks the PrintFreq option to see how frequently mesages should be printed
   *  @retval true Messages should be printed this event
   *  @retval false Messages should be suppressed this event
   */
  inline bool okToPrint() const {
    return (m_printFreq > 0 && 0 == (eventCounter()-1)%m_printFreq);
  }

private:

  void releaseMemoryPools() const; ///< Release memory pools

private:

  Gaudi::Property<int> m_skipFactor{this, "SkipFactor", 0, "skip some random numbers"};
  Gaudi::Property<bool> m_singleSeed{this, "SingleSeed", false, "use only one seed"};
  Gaudi::Property<bool> m_preload{this, "PreloadGeometry", false, "preload the geometry"};
  Gaudi::Property<int> m_printFreq{this, "PrintFreq", 1, "event print frequency. > 0 indicate the rate at which event messages should be printed, < 0 suppress event messages entirely"};
  Gaudi::Property<bool> m_printTime{this, "PrintEventTime", false, "print also the event time"};
  Gaudi::Property< unsigned long long> m_increment{this, "Increment", 100, "Number of events to measure memory"};
  Gaudi::Property<unsigned long long> m_memPurgeLimit{this, "MemoryPurgeLimit", 3400 * 1000, "The memory threshold at which to trigger a purge of the boost pools"}; // default to 3.4GB
  Gaudi::Property<long long> m_minMemDelta{this, "MinMemoryDelta", 16, "The minimum change in memory usage (KB) to trigger a message"}; // defaults to 16KB
  
  mutable SmartIF<IRndmGenSvc> m_randSvc;  ///< Pointer to random number service
  mutable IRndmEngine* m_engine{0};        ///< Pointer to random number engine
  mutable std::atomic_ullong m_lastMem{0}; ///< Last memory usage
  mutable std::mutex m_mutex;              ///< Mutex to secure non thread safe code, that is on demand initializations

  SmartIF<ICondDBInfo> m_condDBInfo; ///< Pointer to Info interface of CondDB service
  std::atomic_long m_evtCounter{0};  ///< Pointer to EventCounter interface
  long m_eventMax{0};                ///< Number of events requested (ApplicationMgr.EvtMax)
  std::string m_appName{""};         ///< Application Name
  std::string m_appVersion{""};      ///< Application Version

};

#endif // LBAPPINIT_H
