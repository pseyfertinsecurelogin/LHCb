#ifndef SRC_TESTEVENTCOUNTER_H
#define SRC_TESTEVENTCOUNTER_H 1
#include <string>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"
#include "Kernel/IEventCounter.h"

class ISvcLocator;
struct IEventCounter;

namespace LHCbAlgsTests {

  /** @class TestEventCounter TestEventCounter.h src/TestEventCounter.h
   *
   *
   * @author Marco Clemencic
   * @date 11/06/2012
   */
  class TestEventCounter final : public GaudiAlgorithm
  {

  public:

    /// Standard constructor
    TestEventCounter(const std::string& name, ISvcLocator* pSvcLocator);
    virtual ~TestEventCounter() = default; ///< Destructor

    StatusCode initialize() override;    ///< Algorithm initialization
    StatusCode execute   () override;    ///< Algorithm execution
    StatusCode finalize  () override;    ///< Algorithm finalization

  private:

    std::string m_eventCounterName;

    IEventCounter* m_eventCounter = nullptr;

  };

} // namespace LHCbAlgsTests

#endif // SRC_TESTEVENTCOUNTER_H
