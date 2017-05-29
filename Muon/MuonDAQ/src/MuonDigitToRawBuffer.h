#ifndef MUONDIGITTORAWBUFFER_H
#define MUONDIGITTORAWBUFFER_H 1

// Include files
// from STL
#include <string>
#include <vector>

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"
#include "Kernel/MuonTileID.h"
#include "MuonDet/DeMuonDetector.h"
#include "MuonDet/MuonDAQHelper.h"

class DeMuonDetector;
class ISvcLocator;

/** @class MuonDigitToRawBuffer MuonDigitToRawBuffer.h
 *
 *
 *  @author Alessia Satta
 *  @date   2004-01-19
 */
class MuonDigitToRawBuffer : public GaudiAlgorithm {
public:
  /// Standard constructor
  MuonDigitToRawBuffer( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

  StatusCode ProcessDC06();
  StatusCode ProcessV1();
  StatusCode ProcessDigitDC06();
  StatusCode ProcessDigitV1();
  StatusCode ProcessPads();

private:
  DeMuonDetector* m_muonDet = nullptr;

  std::vector<unsigned int> m_digitsInODE[MuonDAQHelper_maxODENumber];
  std::vector<unsigned int> m_digitsInL1[MuonDAQHelper_maxTell1Number];
  unsigned int firedInODE[MuonDAQHelper_maxODENumber];
  unsigned int firedInPP[MuonDAQHelper_maxTell1Number*4];

  std::vector<unsigned int> m_padInL1[MuonDAQHelper_maxTell1Number];

  long m_TotL1Board = 0;
  unsigned int m_M1Tell1 = 0;

  unsigned int m_vtype;

};
#endif // MUONDIGITTORAWBUFFER_H
