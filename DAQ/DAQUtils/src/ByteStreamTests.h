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
