/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef MULTIDBTEST_H
#define MULTIDBTEST_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

struct Condition;
class AlignmentCondition;

/** @class MultiDBTest MultiDBTest.h
 *
 *  Algorithm to test the multi-db functionality.
 *
 *  @author Marco Clemencic
 *  @date   2005-05-12
 */
class MultiDBTest : public GaudiAlgorithm {
public:
  /// Standard constructor
  MultiDBTest( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:

  Condition *m_cond1, *m_cond2;
  AlignmentCondition *m_align1;

};
#endif // MULTIDBTEST_H
