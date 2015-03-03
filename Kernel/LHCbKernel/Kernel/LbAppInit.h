// $Id: LbAppInit.h,v 1.9 2009-03-05 14:49:58 cattanem Exp $
#ifndef LBAPPINIT_H 
#define LBAPPINIT_H 1

// Include files
#include <string>
#include <vector>

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
 *  @author Marco Cattaneo
 *  @date   2005-12-21
 */

class LbAppInit : public GaudiAlgorithm {

public: 

  /// Standard constructor
  LbAppInit( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~LbAppInit( ); ///< Destructor

  virtual StatusCode initialize();     ///< Algorithm initialization
  virtual StatusCode execute   ();     ///< Algorithm execution
  virtual StatusCode finalize  ();     ///< Algorithm finalization

protected:

  /// Return number of events processed
  int  eventCounter() const
  { 
    return m_eventCounter; 
  }

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
  void printEventRun( longlong evt,
                      int run, 
                      std::vector<long int> *seeds = 0, 
                      Gaudi::Time time = 0              ) const;

  /** Initialize the random number engine with the given seeds
   *  @param[in] seeds Vector of seeds
   *  @return    StatusCode
   */
  StatusCode initRndm( std::vector<long int>& seeds );

  /** Get a vector of seeds. First three seeds are built from seed1 and seed2
   *  Last seed is hash including seed1, seed2 and name()
   *  If m_singleSeed is true, only last seed is returned 
   *  @param[in] seed1 First  seed (typically run number)
   *  @param[in] seed2 Second seed (typically event number)
   *  @return    vector of seeds
   */
  std::vector<long int> getSeeds( unsigned int seed1, ulonglong seed2 );

  /** Is printing OK for this event ?
   *  Checks the PrintFreq option to see how frequently mesages should be printed
   *  @retval true Messages should be printed this event
   *  @retval false Messages should be suppressed this event
   */
  inline bool okToPrint() const
  {
    return ( m_printFreq > 0 && 0 == (m_eventCounter-1)%m_printFreq );
  }

private:

  /// Property to skip some random numbers (default is zero)
  int  m_skipFactor;

  /// Property to use only one seed (default is false)
  bool m_singleSeed;

  /// Property to preload the geometry (default is false)
  bool m_preload;

  /** @brief Property for event print frequency
   *  Values > 0 indicate the rate at which event messages should be printed
   *  Values < 0 suppress event messages entirely
   */
  int m_printFreq;

  /// Property to print also the event time (default is false)
  bool m_printTime;
  
  // Member data
  IRndmEngine*  m_engine;       ///< Pointer to random number engine
  IRndmGenSvc*  m_randSvc;      ///< Pointer to random number service
  ICondDBInfo*  m_condDBInfo;   ///< Pointer to Info interface of CondDB service
  int  m_eventCounter;          ///< Number of events processed
  int  m_eventMax;     ///< Number of events requested (ApplicationMgr.EvtMax)
  std::string   m_appName;      ///< Application Name
  std::string   m_appVersion;   ///< Application Version
};

#endif // LBAPPINIT_H
