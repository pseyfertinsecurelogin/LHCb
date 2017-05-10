#ifndef L0ETC_H
#define L0ETC_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTupleAlg.h"
/** @class L0ETC L0ETC.h
 *
 *  Writes out ETC for L0 stripping
 *
 *  @author Patrick Koppenburg
 *  @date   2007-07-13
 */
class L0ETC : public GaudiTupleAlg {
public:
  /// Standard constructor
  L0ETC( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

private:
  /// ETC name
  std::string m_collectionName;
  /// L0 channels
  std::vector<std::string> m_l0channels ;

  long m_events = 0; ///< Number of events

};
#endif // L0ETC_H
