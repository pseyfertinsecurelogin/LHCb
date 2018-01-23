#ifndef VISPRIMVERTTOOL_VISPRIMVERTTOOL_H
#define VISPRIMVERTTOOL_VISPRIMVERTTOOL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiAlg/GaudiTool.h"

// from LHCb
#include "MCInterfaces/IVisPrimVertTool.h"

/** @class VisPrimVertTool VisPrimVertTool.h VisPrimVertTool.h
 *
 *  v1.3
 *  @author Christian Jacoby (LPHE, EPFL, Lausanne)
 *  @date   2004-03-18
 */
class VisPrimVertTool : public extends<GaudiTool, IVisPrimVertTool> {
public:
  using extends::extends;

  long countVertices() const override;
  bool isVisible(const LHCb::MCVertex* pv ) const override
  { return (countVisTracks(pv) >= m_nTracks) ; }
  bool isB( const LHCb::MCVertex* ) const override;
  long countVisTracks( const LHCb::MCVertex* ) const override;

private:

  // Number of tracks defining a PV: m_nTracks, by default= 2
  Gaudi::Property<long> m_nTracks {this, "TrackNum", 2 };
  Gaudi::Property<bool> m_veloAndSeed { this, "VeloAndSeed", true };

};
#endif // VISPRIMVERTTOOL_VISPRIMVERTTOOL_H
