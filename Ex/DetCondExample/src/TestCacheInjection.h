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

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

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
