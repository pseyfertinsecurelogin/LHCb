#ifndef TESTUMSERROR_H
#define TESTUMSERROR_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

// Forward declarations
class ICondDBAccessSvc;

/** @class TestUMSError TestUMSError.h
 *
 *  Algorithm that tests the injection of condition object into CondDBAccessSvc's cache.
 *
 *  @author Marco Clemencic
 *  @date   2005-06-20
 */
class TestUMSError : public GaudiAlgorithm {
public:
  /// Standard constructor
  TestUMSError( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~TestUMSError( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:

  StatusCode i_injectData();

  /// Handle to the CondDBAccessSvc
  ICondDBAccessSvc* m_dbAccSvc;

  size_t m_evtCount;

  //IUpdateManagerSvc *m_ums;

  virtual StatusCode null_op() { return StatusCode::SUCCESS; }

};
#endif // TESTUMSERROR_H
