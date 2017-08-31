#ifndef MUONTELL1CONFIG_H
#define MUONTELL1CONFIG_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "MuonDAQ/IMuonTell1Config.h"            // Interface
#include "MuonTell1ConfigData.h"
#include "MuonDet/DeMuonDetector.h"


/** @class MuonTell1Config MuonTell1Config.h
 *
 *
 *  @author
 *  @date   2008-02-29
 */
class MuonTell1Config : public extends<GaudiTool, IMuonTell1Config> {
public:
  /// Standard constructor
  using base_class::base_class;

  StatusCode initialize() override;

  StatusCode configTell1(int Tell1) const override;

private:
 DeMuonDetector* m_muonDet = nullptr;
};
#endif // MUONTELL1CONFIG_H
