#ifndef MUONPADTEST_H
#define MUONPADTEST_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "MuonDAQ/IMuonRawBuffer.h"


/** @class MuonPadTest MuonPadTest.h
 *
 *
 *  @author Alessia Satta
 *  @date   2006-03-19
 */
class MuonPadTest : public GaudiAlgorithm {
public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  IMuonRawBuffer* m_MuonBuffer = nullptr;

};
#endif // MUONPADTEST_H
