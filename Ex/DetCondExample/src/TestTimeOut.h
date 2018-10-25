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
#ifndef TESTTIMEOUT_H
#define TESTTIMEOUT_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"


/** @class TestTimeOut TestTimeOut.h
 *
 *  Algorithm used to test the connection time-out functionality od CondDBAccessSvc().
 *
 *  @author Marco CLEMENCIC
 *  @date   2006-08-31
 */
class TestTimeOut : public GaudiAlgorithm {
public:
  /// Standard constructor
  TestTimeOut( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~TestTimeOut( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

protected:

private:

  /// How many seconds to sleep (option TestTimeOut.SleepTime, default = 20).
  int m_sleepTime;

};
#endif // TESTTIMEOUT_H
