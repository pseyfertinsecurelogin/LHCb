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
// $Id: ByteStreamTests.h,v 1.1 2008/07/21 14:50:55 cattanem Exp $
#ifndef TESTS_BYTESTREAMTESTS_H
#define TESTS_BYTESTREAMTESTS_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"


namespace DAQEventTests {
/** @class ByteStreamTests ByteStreamTests.h tests/ByteStreamTests.h
 *
 *
 *  @author Matt Needham
 *  @date   2005-10-13
 */
class ByteStreamTests : public GaudiAlgorithm {
public:
  /// Standard constructor
  ByteStreamTests( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~ByteStreamTests( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute() override;       ///< Algorithm execution

protected:

private:

};

} // namespace DAQEventTests

#endif // TESTS_BYTESTREAMTESTS_H
