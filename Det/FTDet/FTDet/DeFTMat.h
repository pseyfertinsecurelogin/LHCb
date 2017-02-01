#ifndef DEFTMAT_H
#define DEFTMAT_H 1

// DetDesc
#include "DetDesc/DetectorElement.h"

/// Kernel
#include "Kernel/FTChannelID.h"
#include "Kernel/LineTraj.h"
#include "Kernel/DetectorSegment.h"

#include "GaudiKernel/Plane3DTypes.h"

#include "DetDesc/IPVolume.h"

/** @class DeFTMat DeFTMat.h "FTDet/DeFTMat.h"
 *
 *  This is the detector element class of the Fibre Tracker (FT) mat.
 *
 *  @author Jeroen van Tilburg
 *  @date   2016-10-21
 */

static const CLID CLID_DeFTMat = 8606;

class DeFTMat : public DetectorElement {

public:

  /// Standard constructor
  using DetectorElement::DetectorElement;

  /** Initialization method */
  StatusCode initialize() override;

  /** Retrieves reference to class identifier
   *  @return The class identifier for this class
   */
  const CLID& clID() const override;

  /** Another reference to class identifier
   *  @return The class identifier for this class
   */
  static const CLID& classID() { return CLID_DeFTMat; }

  /** @return matID */
  unsigned int matID() const;

  /** @return moduleID */
  unsigned int moduleID() const;

  /** @return quarterID */
  unsigned int quarterID() const;

  /** @return  layerID */
  unsigned int layerID() const;

  /** @return stationID */
  unsigned int stationID() const;

  /** @return unique matID */
  unsigned int uniqueMatID() const;

  /** @return unique moduleID */
  unsigned int uniqueModuleID() const;

  /** @return unique quarterID */
  unsigned int uniqueQuarterID() const;

  /** @return  unique layerID */
  unsigned int uniqueLayerID() const;

  /** @return unique stationID */
  unsigned int uniqueStationID() const;

  /** Element id */
  LHCb::FTChannelID elementID() const;

  /** Set element id */
  void setElementID(const LHCb::FTChannelID& chanID);

  /** @return flag true if this quarter is bottom half */
  bool isBottom() const {return quarterID() == 0 || quarterID() == 1; }

  /** @return flag true if this quarter is top half */
  bool isTop() const {return quarterID() == 2 || quarterID() == 3; }

  /**
   * Return a sensitive volume identifier for a given point in the
   * global reference frame. This function is vital for Gauss.
   */
  int sensitiveVolumeID(const Gaudi::XYZPoint&) const override {
    return m_elementID;
  }

  /** Returns the pitch between two channels (250 micron) */
  double channelPitch() const { return m_channelPitch; }

  /** Get the length of the fibre in this mat */
  double fibreLength() const { return m_sizeY; }

  /** Returns the thickness of the fibre mat (1.3 mm) */
  double fibreMatThickness() const { return m_sizeZ; }

  /** Returns the global z position of the module */
  double globalZ() const { return m_globalZ; }

  /** Returns the xy-plane at z-middle the layer */
  const Gaudi::Plane3D& plane() const { return m_plane; }

  /** Main method to determine which channel was hit
   *  @param localX is the input x coordinate in the local frame.
   *  @param frac returns the corresponding fraction in the range 0-1.
   *  @return channel returns the corresponding readout channel
   */
  LHCb::FTChannelID calculateChannelAndFrac(double localX, double& frac) const;

  /** Main method to determine which channel was hit
   *  @param localX is the input x coordinate in the local frame.
   *  @return channel returns the corresponding readout channel
   */
  LHCb::FTChannelID calculateChannel(double localX) const;

  /** Get the list of SiPM channels traversed by the hit.
   *  The particle trajectory is a straight line defined by:
   *  @param provide local entry and exit point
   *  Fills a vector of FTChannelIDs, and a vector of the
   *  corresponding left edges (along x) in the local frame.
   */
  std::vector<std::pair<LHCb::FTChannelID, double>>
    calculateChannels(const double localEntry, const double localExit,
                      const unsigned int numOfAdditionalChannels ) const;

  /** Get the list of SiPM channels in the mat
   *  Need to make clear what happens if there is a gap
   *  @param empty
   *  Fills a vector of FTChannelIDs, and a vector of the
   *  corresponding left edges (along x) in the local frame.
   */
  std::vector<std::pair<LHCb::FTChannelID, double>>
    calculateChannels() const;

  /** Get the distance from the hit to the SiPM
   *  @param localPoint is the position of the half module in local coordinates
   *  @return the distance to the SiPM
   */
  double distanceToSiPM( const Gaudi::XYZPoint& localPoint ) const {
    return 0.5 * m_sizeY - localPoint.y();
  };

  /** Get the local x from a channelID and its fraction */
  double localXfromChannel(const LHCb::FTChannelID channelID, const double frac) const;

  /** Calculate the distance between a global point and FTChannelID + fraction
   *  @param channelID is the FTChannelID of the cluster
   *  @param frac is the fraction of the cluster centre of gravity
   *  @return distance in mm
   */
  double distancePointToChannel(const Gaudi::XYZPoint& globalPoint,
      const LHCb::FTChannelID channelID, const double frac ) const;

  /** Get the trajectory defined by the begin and end positions of a hit
   *   (channelID + fraction)
   *  @param channelID input FTChannelID
   *  @param frac input fraction
   */
  std::unique_ptr<LHCb::Trajectory> trajectory(const LHCb::FTChannelID channelID,
      const double frac) const;

  /** Get the line defined by the begin and end positions of a hit
   *   (channelID + fraction)
   *  @param channelID input FTChannelID
   *  @param frac input fraction
   */
  std::pair<Gaudi::XYZPoint,Gaudi::XYZPoint> endPoints(
      const LHCb::FTChannelID channelID, const double frac) const;

  /** Flag if there is a gap on the left of this channel */
  bool hasGapLeft( const LHCb::FTChannelID thisChannel ) const {
    return ( thisChannel.channel() == 0u ||
        int(thisChannel.channel()) == m_nChannelsInDie ) ;
  }

  /** Flag if there is a gap on the right of this channel */
  bool hasGapRight( const LHCb::FTChannelID thisChannel ) const {
    return ( int(thisChannel.channel()) == m_nChannelsInSiPM - 1 ||
        int(thisChannel.channel()) == m_nChannelsInDie - 1) ;
  }


private :
  LHCb::FTChannelID m_elementID;   ///< element ID

  int m_nChannelsInSiPM;           ///< number of channels per SiPM
  int m_nChannelsInDie;            ///< number of channels per die
  int m_nSiPMsInMat;               ///< number of SiPM arrays per mat
  int m_nDiesInSiPM;               ///< number of dies per SiPM

  Gaudi::Plane3D m_plane;          ///< xy-plane in the z-middle of the module
  double m_globalZ;                ///< Global z position of module closest to y-axis
  double m_uBegin;                 ///< start in local u-coordinate of sensitive SiPM
  double m_airGap;                 ///< air gap
  double m_deadRegion;             ///< dead region
  double m_dieGap;                 ///< gap between channel 63 and 64
  double m_channelPitch;           ///< readout channel pitch (250 micron)
  double m_sipmPitch;              ///< pitch between SiPMs in mat
  double m_diePitch;               ///< pitch between dies in SiPM
  double m_sizeY;                  ///< Length in y of the fibre in the mat
  double m_sizeZ;                  ///< Thickness of the fibre mat (nominal: 1.3 mm)

}; // end of class

inline unsigned int DeFTMat::matID() const {
  return m_elementID.mat();
}

inline unsigned int DeFTMat::moduleID() const {
  return m_elementID.module();
}

inline unsigned int DeFTMat::quarterID() const {
  return m_elementID.quarter();
}

inline unsigned int DeFTMat::layerID() const {
  return m_elementID.layer();
}

inline unsigned int DeFTMat::stationID() const {
  return m_elementID.station();
}

inline unsigned int DeFTMat::uniqueMatID() const {
  return m_elementID.uniqueMat();
}

inline unsigned int DeFTMat::uniqueModuleID() const {
  return m_elementID.uniqueModule();
}

inline unsigned int DeFTMat::uniqueQuarterID() const {
  return m_elementID.uniqueQuarter();
}

inline unsigned int DeFTMat::uniqueLayerID() const {
  return m_elementID.uniqueLayer();
}


inline LHCb::FTChannelID DeFTMat::elementID() const {
  return m_elementID;
}

inline void DeFTMat::setElementID(const LHCb::FTChannelID& chanID) {
  m_elementID = chanID;
}

#endif // DeFTMat_H
