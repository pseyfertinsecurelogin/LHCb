// $Id: DummyRawEventCreator.h,v 1.1 2008/07/21 14:50:55 cattanem Exp $
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
  DummyRawEventCreator( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~DummyRawEventCreator( );     ///< Destructor
  StatusCode initialize() override;      ///< Algorithm initialization
  StatusCode execute() override;         ///< Algorithm execution

protected:

private:
  IEventTimeDecoder *m_eventTimeDecoder;

};

} // namespace DAQEventTests

#endif // TESTS_DUMMYRAWEVENTCREATOR_H
