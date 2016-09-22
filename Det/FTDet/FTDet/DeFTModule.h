#ifndef DEFTMODULE_H
#define DEFTMODULE_H 1

// DetDesc
#include "DetDesc/DetectorElement.h"

/// Kernel
#include "Kernel/FTChannelID.h"


/** @class DeFTModule DeFTModule.h "FTDet/DeFTModule.h"
 *
 *  This is the detector element class of the Fibre Tracker (FT) station.
 *
 *  @author Jeroen van Tilburg
 *  @date   2016-07-18
 */

static const CLID CLID_DeFTModule = 8605;

class DeFTModule : public DetectorElement {

public: 

  /// Standard constructor
  DeFTModule( const std::string& name = "" );

  /** Initialization method */
  StatusCode initialize() override;

  /** Retrieves reference to class identifier
   *  @return The class identifier for this class
   */
  const CLID& clID() const override;

  /** Another reference to class identifier
   *  @return The class identifier for this class
   */
  static const CLID& classID() { return CLID_DeFTModule; }

  /** @return moduleID */
  unsigned int moduleID() const;

  /** @return quarterID */
  unsigned int quarterID() const;

  /** @return  layerID */
  unsigned int layerID() const;

  /** @return stationID */
  unsigned int stationID() const;

  /** Element id */
  LHCb::FTChannelID elementID() const;

  /** Set element id */
  void setElementID(const LHCb::FTChannelID& chanID);

  /**
   * Return a sensitive volume identifier for a given point in the
   * global reference frame. This function is vital for Gauss.
   */
  int sensitiveVolumeID(const Gaudi::XYZPoint&) const override {
    return m_elementID;
  };

  /** Returns the pitch between two channels (250 micron) */
  double channelPitch() const { return m_channelPitch; };

  /** Main method to determine which channel was hit
   *  @param localX is the input x coordinate in the local frame.
   *  @param frac returns the corresponding fraction in the range 0-1.
   *  @return channel returns the corresponding readout channel
   */
  LHCb::FTChannelID calculateChannelAndFrac(double localX, double& frac) const;

  /** Main method to determine which channel was hit, adding an offset to the channel
   *  @param localX is the input x coordinate in the local frame.
   *  @param offset is the number of channels that should be added to the channel.
   *  @return channel returns the corresponding readout channel
   */
  //LHCb::FTChannelID calculateChannel(double localX,
  //    int offset) const ;

  /** Get the list of SiPM channels traversed by the hit.
   *  The particle trajectory is a straight line defined by:
   *  @param provide local entry and exit point
   *  Fills a vector of FTChannelIDs, and a vector of the
   *  corresponding left edges (along x) in the local frame.
   */
  std::vector<std::pair<LHCb::FTChannelID, double>>
  calculateChannels(const double localEntry, const double localExit,
      const unsigned int numOfAdditionalChannels ) const;

  /** Get the distance from the hit to the SiPM
   *  @param localPoint is the position of the half module in local coordinates
   *  @return the distance to the SiPM
   */
  double distanceToSiPM( const Gaudi::XYZPoint& localPoint ) const {
    return 0.5 * m_fibreSizeY - localPoint.y();
  };

  /** Get the pseudoChannel for a given FTChannelID
   *  The pseudoChannel is defined for a full zone and runs
   *  in increasing global x. It has negative numbers for negative x.
   *  This function is useful for occupancy plots.
   *  @param the FTChannelID
   *  @return the pseudoChannel, between +- 6*16*128
   */
  int pseudoChannel( LHCb::FTChannelID channelID ) const;

  /** Get the local x from a channelID and its fraction */
  double localXfromChannel(const LHCb::FTChannelID channelID, const double frac) const;

  /** Calculate the distance between a global point and FTChannelID + fraction
   *  @param channelID is the FTChannelID of the cluster
   *  @param frac is the fraction of the cluster centre of gravity
   *  @return distance in mm
   */
  double distancePointToChannel(const Gaudi::XYZPoint& globalPoint,
      const LHCb::FTChannelID channelID, const double frac ) const;

  /** Get the begin and end positions of a hit (channelID + fraction)
   *  @param channelID input FTChannelID
   *  @param frac input fraction
   *  @param beginPoint returns the 3D point closest to y=0
   *  @param endPoint returns the 3D point closest to the SiPM
   */
  void getPositions(const LHCb::FTChannelID channelID, const double frac,
      Gaudi::XYZPoint& beginPoint, Gaudi::XYZPoint& endPoint) const;

  /** Flag if the local x is in or above the beam pipe hole */
  bool inHole( double localX ) const {
    return m_moduleID == 0 &&
        (localX > 0.5*m_fibreSizeX - m_holeSizeX ||
            localX < -0.5*m_fibreSizeX - m_holeSizeX) ;
  }

  /** Get the length of the fibre in this module */
  double fibreLength( const Gaudi::XYZPoint& localPoint ) const {
    return inHole( localPoint.x() ) ? m_fibreSizeY-m_holeSizeY : m_fibreSizeY;
  }

  /** Flag if there is a gap on the left of this channel */
  bool hasGapLeft( const LHCb::FTChannelID thisChannel ) const {
    return ( thisChannel.channel() == 0u ||
        int(thisChannel.channel()) == m_nChannelsInDie ) ;
  };

  /** Flag if there is a gap on the right of this channel */
  bool hasGapRight( const LHCb::FTChannelID thisChannel ) const {
    return ( int(thisChannel.channel()) == m_nChannelsInSiPM - 1 ||
        int(thisChannel.channel()) == m_nChannelsInDie - 1) ;
  };


private :
  unsigned int m_stationID;        ///< station ID number
  unsigned int m_layerID;          ///< layer ID number
  unsigned int m_quarterID;        ///< quarter ID number
  unsigned int m_moduleID;         ///< module ID number
  LHCb::FTChannelID m_elementID;   ///< element ID

  int m_nChannelsInSiPM;  ///< number of channels per SiPM
  int m_nChannelsInDie;   ///< number of channels per die
  int m_nSiPMsInMat;      ///< number of SiPM arrays per mat
  int m_nMats;            ///< number of mats per module
  int m_nDiesInSiPM;      ///< number of dies per SiPM
  int m_nSiPMsInModule;   ///< number of SiPM arrays per module
  int m_nChannelsInModule;///< number of channels per module

  double m_uBegin;                 ///< start in local u-coordinate of sensitive SiPM
  double m_airGap;                 ///< air gap
  double m_deadRegion;             ///< dead region
  double m_dieGap;                 ///< gap between channel 63 and 64
  double m_channelPitch;           ///< readout channel pitch (250 micron)
  double m_sipmPitch;              ///< pitch between SiPMs in mat
  double m_diePitch;               ///< pitch between dies in SiPM
  double m_matPitch;               ///< pitch between mats
  double m_fibreSizeX;             ///< Length in x of all fibres in the module
  double m_fibreSizeY;             ///< Length in y of the fibre in the module
  double m_holeSizeX;              ///< Size in x of the hole in the module
  double m_holeSizeY;              ///< Size in y of the hole in the module
  bool m_reversed;                 ///< Flag set when the pseudochannel-ordering is reversed

}; // end of class

inline unsigned int DeFTModule::moduleID() const {
  return m_moduleID;
}

inline unsigned int DeFTModule::quarterID() const {
  return m_quarterID;
}

inline unsigned int DeFTModule::layerID() const {
  return m_layerID;
}

inline unsigned int DeFTModule::stationID() const {
  return m_stationID;
}

inline LHCb::FTChannelID DeFTModule::elementID() const {
  return m_elementID;
}

inline void DeFTModule::setElementID(const LHCb::FTChannelID& chanID) {
  m_elementID = chanID;
}

#endif // DEFTMODULE_H
