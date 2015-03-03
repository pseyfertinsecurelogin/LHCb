// $Id: TestCacheInjection.h,v 1.2 2006-01-19 18:32:11 marcocle Exp $
#ifndef TESTCACHEINJECTION_H 
#define TESTCACHEINJECTION_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

// Forward declarations
class ICondDBAccessSvc;

/** @class TestCacheInjection TestCacheInjection.h
 *  
 *  Algorithm that tests the injection of condition object into CondDBAccessSvc's cache.
 *
 *  @author Marco Clemencic
 *  @date   2005-06-20
 */
class TestCacheInjection : public GaudiAlgorithm {
public: 
  /// Standard constructor
  TestCacheInjection( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~TestCacheInjection( ); ///< Destructor

  virtual StatusCode initialize();    ///< Algorithm initialization
  virtual StatusCode execute   ();    ///< Algorithm execution
  virtual StatusCode finalize  ();    ///< Algorithm finalization

private:

  StatusCode i_injectData();

  /// Handle to the CondDBAccessSvc
  ICondDBAccessSvc* m_dbAccSvc;

  size_t m_evtCount;

  Condition *m_cond1;
  Condition *m_cond2;
  Condition *m_cond3;
  Condition *m_cond4;

  virtual StatusCode null_op() { return StatusCode::SUCCESS; }

};
#endif // TESTCACHEINJECTION_H
