#ifndef MUONTELL1CONFIG_H  
#define MUONTELL1CONFIG_H 1

#include <string>
#include <vector>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/extends.h"
#include "MuonDAQ/IMuonTell1Config.h"            // Interface
#include "MuonDet/DeMuonDetector.h"
#include "MuonTell1ConfigData.h"

class DeMuonDetector;
class IInterface;
struct IMuonTell1Config;


/** @class MuonTell1Config MuonTell1Config.h
 *  
 *
 *  @author 
 *  @date   2008-02-29
 */
class MuonTell1Config : public extends<GaudiTool, IMuonTell1Config> {
public: 
  /// Standard constructor
  MuonTell1Config( const std::string& type, 
                   const std::string& name,
                   const IInterface* parent);

  StatusCode initialize() override;

  StatusCode configTell1(int Tell1) const override;

private:
 DeMuonDetector* m_muonDet = nullptr;
};
#endif // MUONTELL1CONFIG_H
