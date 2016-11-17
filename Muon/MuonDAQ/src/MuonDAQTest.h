#ifndef MUONDAQTEST_H
#define MUONDAQTEST_H 1

// Include files

#include "MuonDAQ/IMuonRawBuffer.h"
#include "Event/MuonDigit.h"


// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
/** @class MuonDAQTest MuonDAQTest.h
 *
 *
 *  @author Alessia Satta
 *  @date   2005-10-19
 */
class MuonDAQTest : public GaudiAlgorithm {
public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  IMuonRawBuffer* m_MuonBuffer = nullptr;
};
#endif // MUONDAQTEST_H
