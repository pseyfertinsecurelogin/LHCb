#ifndef SRC_ODINBADTIMETEST_H
#define SRC_ODINBADTIMETEST_H 1
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

class IEventTimeDecoder;

namespace LHCbAlgsTests {

  /** @class ODINBadTimeTest ODINBadTimeTest.h src/ODINBadTimeTest.h
   *
   * Small test algorithm to test the handling of invalid GPS time in OdinTimeDecoder.
   *
   * @author Marco Clemencic
   * @date 18/10/2011
   */
  class ODINBadTimeTest final : public GaudiAlgorithm 
  {
  
public:

    /// Standard constructor
    ODINBadTimeTest(const std::string& name, ISvcLocator* pSvcLocator);
    virtual ~ODINBadTimeTest() = default; ///< Destructor

    virtual StatusCode initialize();    ///< Algorithm initialization
    virtual StatusCode execute   ();    ///< Algorithm execution
    virtual StatusCode finalize  ();    ///< Algorithm finalization
 
  private:
 
   IEventTimeDecoder *m_evtTimeTool = nullptr;

   bool m_called{false};

  };

}
#endif // SRC_ODINBADTIMETEST_H
