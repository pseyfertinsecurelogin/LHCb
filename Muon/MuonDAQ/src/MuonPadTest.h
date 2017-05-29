#ifndef MUONPADTEST_H
#define MUONPADTEST_H 1

#include <string>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"
#include "MuonDAQ/IMuonRawBuffer.h"

class IMuonRawBuffer;
class ISvcLocator;


/** @class MuonPadTest MuonPadTest.h
 *
 *
 *  @author Alessia Satta
 *  @date   2006-03-19
 */
class MuonPadTest : public GaudiAlgorithm {
public:
  /// Standard constructor
  MuonPadTest( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  IMuonRawBuffer* m_MuonBuffer;

};
#endif // MUONPADTEST_H
