#ifndef MUONTAEREC_H
#define MUONTAEREC_H 1

#include <string>
#include <utility>
#include <vector>

#include "Event/MuonCoord.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"
#include "MuonDAQ/IMuonRawBuffer.h"
#include "MuonDet/DeMuonDetector.h"
#include "MuonDet/MuonBasicGeometry.h"

class DeMuonDetector;
class IMuonRawBuffer;
class ISvcLocator;
class MuonLayout;
namespace LHCb {
class MuonTileID;
}  // namespace LHCb



/** @class MuonTAERec MuonTAERec.h
 *
 *
 *  @author Alessia Satta
 *  @date   2008-10-28
 */
class MuonTAERec : public GaudiAlgorithm {
public:
  /// Standard constructor
  MuonTAERec( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  /// Copy MuonTileID from digits to coords with no logical map (1:1 copy)
  StatusCode addCoordsNoMap(LHCb::MuonCoords *coords,
                            std::vector<std::pair<LHCb::MuonTileID ,
                            unsigned int> > & digit,
                            const int & station,
                            const int & region);

  /// Copy MuonTileID from digits to coord by crossing the digits
  StatusCode addCoordsCrossingMap(LHCb::MuonCoords *coords,
                                  std::vector<std::pair<LHCb::MuonTileID,
                                  unsigned int> > &digit,
                                  const int & station,
                                  const int & region);

  /// fills in the two readout layouts by querying the DeMuonRegion
  StatusCode makeStripLayouts(int station , int region,
                              MuonLayout &layout1,
                              MuonLayout &layout2);

bool m_ignoreUncrossedStrips;
  // Number of stations
  int m_NStation;
  // Number of regions
  int m_NRegion;
  DeMuonDetector* m_muonDetector;
  IMuonRawBuffer* m_muonBuffer;
  bool m_ignoreExecution;
  std::string m_offsetLoc[15];
  int m_TAENum;

  std::vector<std::pair<LHCb::MuonTileID,unsigned int> >  m_logChannels;
};

#endif // MUONTAEREC_H
