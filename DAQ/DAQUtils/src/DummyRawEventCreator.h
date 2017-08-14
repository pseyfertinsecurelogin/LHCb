#ifndef TESTS_DUMMYRAWEVENTCREATOR_H
#define TESTS_DUMMYRAWEVENTCREATOR_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

class IEventTimeDecoder;

namespace DAQEventTests {
/** @class DummyRawEventCreator DummyRawEventCreator.h
 *  Creates and fills dummy RawEvent
 *
 *  @author Markus Frank
 *  @date   2005-10-13
 */
class DummyRawEventCreator : public GaudiAlgorithm {
public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override;      ///< Algorithm initialization
  StatusCode execute() override;         ///< Algorithm execution

private:
  IEventTimeDecoder *m_eventTimeDecoder = nullptr;

};

} // namespace DAQEventTests

#endif // TESTS_DUMMYRAWEVENTCREATOR_H
