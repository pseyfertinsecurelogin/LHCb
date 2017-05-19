#ifndef SRC_DQSCANTEST_H
#define SRC_DQSCANTEST_H 1
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "Kernel/IDQScanner.h"

#include "DetCond/ICondDBReader.h"

namespace DetCondTest {

/** @class DQScanTest DQScanTest.h src/DQScanTest.h
  *
  * Algorithm to test the behavior of an IDQScanner implementation.
  *
  * @author Marco Clemencic
  * @date 31/01/2012
  */
class DQScanTest: public GaudiAlgorithm {
public:
  typedef std::pair<unsigned int, unsigned int> IOVPropType;
  typedef std::vector<IOVPropType> IOVListPropType;

  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

private:

  /// Type/name of the IDQScanner instance.
  /// (property DQScanner)
  Gaudi::Property<std::string> m_DQScannerName
  {this, "DQScanner", "CondDBDQScanner", "Type/name of the IDQScanner instance to use."};

  /// List of IOVs (with time specified in seconds) to try to retrieve (property).
  Gaudi::Property<IOVListPropType> m_iovsProp
  { this, "IOVs", IOVListPropType{}, "List of IOVs (specified in seconds) to scan."};

  /// List of IOVs to try to retrieve.
  ICondDBReader::IOVList m_iovs;

  /// Pointer to the IDQScanner instance.
  IDQScanner *m_scanner = nullptr;
};

}

#endif // SRC_DQSCANTEST_H
