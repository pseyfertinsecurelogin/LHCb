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
#ifndef SRC_DQSCANTEST_H
#define SRC_DQSCANTEST_H 1

#include <GaudiAlg/GaudiAlgorithm.h>
#include <GaudiKernel/Time.h>
#include <Kernel/IDQScanner.h>

namespace DetCondTest {
  struct IOV {
    Gaudi::Time since = Gaudi::Time::epoch(), until = Gaudi::Time::max();
  };
  using IOVList = std::vector<IOV>;
  std::ostream& operator<<( std::ostream& s, const DetCondTest::IOV& iov );
} // namespace DetCondTest

namespace Gaudi {
  namespace Parsers {
    // Note: to be kept in sync with the property in DetCondTest::DQScanTest
    StatusCode parse( DetCondTest::IOVList& result, const std::string& input );
  } // namespace Parsers
} // namespace Gaudi

namespace DetCondTest {

  /** @class DQScanTest DQScanTest.h src/DQScanTest.h
   *
   * Algorithm to test the behavior of an IDQScanner implementation.
   *
   * @author Marco Clemencic
   * @date 31/01/2012
   */
  class DQScanTest : public GaudiAlgorithm {
  public:
    /// Standard constructor
    using GaudiAlgorithm::GaudiAlgorithm;

    StatusCode initialize() override; ///< Algorithm initialization
    StatusCode execute() override;    ///< Algorithm execution
    StatusCode finalize() override;   ///< Algorithm finalization

  private:
    /// Type/name of the IDQScanner instance.
    /// (property DQScanner)
    Gaudi::Property<std::string> m_DQScannerName{this, "DQScanner", "CondDBDQScanner",
                                                 "Type/name of the IDQScanner instance to use."};

    /// List of IOVs (with time specified in seconds) to try to retrieve (property).
    Gaudi::Property<IOVList> m_iovs{this, "IOVs", IOVList{}, "List of IOVs (specified in seconds) to scan."};

    /// Pointer to the IDQScanner instance.
    IDQScanner* m_scanner = nullptr;
  };

} // namespace DetCondTest

#endif // SRC_DQSCANTEST_H
