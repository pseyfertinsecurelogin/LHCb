/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef DEFTMAT_H
#define DEFTMAT_H 1

// DetDesc
#include "DetDesc/DetectorElement.h"

/// Kernel
#include "Kernel/DetectorSegment.h"
#include "Kernel/FTChannelID.h"
#include "Kernel/LineTraj.h"

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
  void setElementID( const LHCb::FTChannelID& chanID );

  /** @return flag true if this quarter is bottom half */
  bool isBottom() const { return quarterID() == 0 || quarterID() == 1; }

  /** @return flag true if this quarter is top half */
  bool isTop() const { return quarterID() == 2 || quarterID() == 3; }

  /**
   * Return a sensitive volume identifier for a given point in the
   * global reference frame. This function is vital for Gauss.
   */
  int sensitiveVolumeID( const Gaudi::XYZPoint& ) const override { return m_elementID; }

  /** Returns the pitch between two channels (250 micron) */
  float channelPitch() const { return m_channelPitch; }

  /** Returns the width of the fibre mat */
  float fibreMatWidth() const { return m_sizeX; }

  /** Get the length of the fibre in this mat */
  float fibreLength() const { return m_sizeY; }

  /** Returns the thickness of the fibre mat (1.3 mm) */
  float fibreMatThickness() const { return m_sizeZ; }

  /** Returns the global z position of the module */
  float globalZ() const { return m_globalZ; }

  /** Returns the xy-plane at z-middle the layer */
  const Gaudi::Plane3D& plane() const { return m_plane; }

  /** Main method to determine which channel was hit
   *  @param localX is the input x coordinate in the local frame.
   *  @param frac returns the corresponding fraction in the range 0-1.
   *  @return channel returns the corresponding readout channel
   */
  LHCb::FTChannelID calculateChannelAndFrac( float localX, float& frac ) const;

  /** Get the list of SiPM channels between two channels
   *  @param provide first and last channels
   *  Fills a vector of FTChannelIDs, and a vector of the
   *  corresponding left edges (along x) in the local frame.
   */
  std::vector<std::pair<LHCb::FTChannelID, float>> calculateChannels( LHCb::FTChannelID thisChannel,
                                                                      LHCb::FTChannelID endChannel ) const;

  /** Get the list of SiPM channels traversed by the hit.
   *  The particle trajectory is a straight line defined by:
   *  @param provide local entry and exit point
   *  @param provide the number of additional channels to add
   *  Fills a vector of FTChannelIDs, and a vector of the
   *  corresponding left edges (along x) in the local frame.
   */
  std::vector<std::pair<LHCb::FTChannelID, float>>
  calculateChannels( const float localEntry, const float localExit, const unsigned int numOfAdditionalChannels ) const;

  /** Get the list of SiPM channels in the mat
   *  Need to make clear what happens if there is a gap
   *  @param empty
   *  Fills a vector of FTChannelIDs, and a vector of the
   *  corresponding left edges (along x) in the local frame.
   */
  std::vector<std::pair<LHCb::FTChannelID, float>> calculateChannels() const;

  /** Get the distance from the hit to the SiPM
   *  @param localPoint is the position of the half module in local coordinates
   *  @return the distance to the SiPM
   */
  float distanceToSiPM( const Gaudi::XYZPoint& localPoint ) const { return 0.5 * m_sizeY - localPoint.y(); };

  /** Get the local x from a channelID and its fraction */
  float localXfromChannel( const LHCb::FTChannelID channelID, const float frac ) const;

  /** Get the local x from a channelID and its fraction */
  float localXfromChannel( const LHCb::FTChannelID channelID, const int frac ) const {
    float uFromChannel = m_uBegin + ( 2 * channelID.channel() + 1 + frac ) * m_halfChannelPitch;
    if ( channelID.die() ) uFromChannel += m_dieGap;
    uFromChannel += channelID.sipm() * m_sipmPitch;
    return uFromChannel;
  }

  /** Calculate the distance between a global point and FTChannelID + fraction
   *  @param channelID is the FTChannelID of the cluster
   *  @param frac is the fraction of the cluster centre of gravity
   *  @return distance in mm
   */
  float distancePointToChannel( const Gaudi::XYZPoint& globalPoint, const LHCb::FTChannelID channelID,
                                const float frac ) const;

  /** Get the trajectory defined by the begin and end positions of a hit
   *   (channelID + fraction)
   *  @param channelID input FTChannelID
   *  @param frac input fraction
   */
  LHCb::LineTraj<double> trajectory( const LHCb::FTChannelID channelID, const float frac ) const;

  /** Get the line defined by the begin and end positions of a hit
   *   (channelID + fraction)
   *  @param channelID input FTChannelID
   *  @param frac input fraction
   */
  std::pair<Gaudi::XYZPointF, Gaudi::XYZPointF> endPoints( const LHCb::FTChannelID channelID, const float frac ) const;

  /** Get begin positions of a hit (channelID + integer fraction)
   *  @param channelID input FTChannelID
   *  @param frac input fraction
   */
  Gaudi::XYZPointF endPoint( const LHCb::FTChannelID channelID, const int frac ) const {
    return Gaudi::XYZPointF( m_mirrorPoint + m_ddx * localXfromChannel( channelID, frac ) );
  }

  /** Get the global slope of the mat in dx/dy */
  float dxdy() const { return m_dxdy; }

  /** Get the global slope of the mat in dz/dy */
  float dzdy() const { return m_dzdy; }

  /** Get the global height of the mat in y */
  float globaldy() const { return m_globaldy; }

  /** Get the Location of end of fibres at x=z=0 */
  const Gaudi::XYZPointF& mirrorPoint() const { return m_mirrorPoint; }

  /** Get the global direction vector for a local displacement in unit x */
  const Gaudi::XYZVectorF& ddx() const { return m_ddx; }

  /** Get the local u-coordinate of sensitive SiPM */
  float uBegin() const { return m_uBegin; }

  /** Get the half of the readout channel pitch */
  float halfChannelPitch() const { return m_halfChannelPitch; }

  /** Get the gap between channel 63 and 64 */
  float dieGap() const { return m_dieGap; }

  /** Get the pitch between SiPMs in mat */
  float sipmPitch() const { return m_sipmPitch; }

  /** Flag if there is a gap on the left of this channel */
  bool hasGapLeft( const LHCb::FTChannelID thisChannel ) const {
    return ( thisChannel.channel() == 0u || int( thisChannel.channel() ) == m_nChannelsInDie );
  }

  /** Flag if there is a gap on the right of this channel */
  bool hasGapRight( const LHCb::FTChannelID thisChannel ) const {
    return ( int( thisChannel.channel() ) == m_nChannelsInSiPM - 1 ||
             int( thisChannel.channel() ) == m_nChannelsInDie - 1 );
  }

private:
  LHCb::FTChannelID m_elementID; ///< element ID

  int m_nChannelsInSiPM; ///< number of channels per SiPM
  int m_nChannelsInDie;  ///< number of channels per die
  int m_nSiPMsInMat;     ///< number of SiPM arrays per mat
  int m_nDiesInSiPM;     ///< number of dies per SiPM

  Gaudi::Plane3D   m_plane;        ///< xy-plane in the z-middle of the module
  Gaudi::XYZPointF m_sipmPoint;    ///< Location of end of fibres at x=z=0
  float            m_globalZ;      ///< Global z position of module closest to y-axis
  float            m_airGap;       ///< air gap
  float            m_deadRegion;   ///< dead region
  float            m_channelPitch; ///< readout channel pitch (250 micron)
  float            m_diePitch;     ///< pitch between dies in SiPM
  float            m_sizeX;        ///< Width in x of the mat
  float            m_sizeY;        ///< Length in y of the fibre in the mat
  float            m_sizeZ;        ///< Thickness of the fibre mat (nominal: 1.3 mm)

  // Parameters needed for decoding
  Gaudi::XYZPointF  m_mirrorPoint;      ///< Location of end of fibres at x=z=0
  Gaudi::XYZVectorF m_ddx;              ///< Global direction vector for a local displacement in unit x
  float             m_uBegin;           ///< start in local u-coordinate of sensitive SiPM
  float             m_halfChannelPitch; ///< half of the readout channel pitch (125 micron)
  float             m_dieGap;           ///< gap between channel 63 and 64
  float             m_sipmPitch;        ///< pitch between SiPMs in mat
  float             m_dxdy;             ///< Global slope dx/dy for a fibre mat
  float             m_dzdy;             ///< Global slope dz/dy for a fibre mat
  float             m_globaldy;         ///< Length of a fibre projected along global y

}; // end of class

inline unsigned int DeFTMat::matID() const { return m_elementID.mat(); }

inline unsigned int DeFTMat::moduleID() const { return m_elementID.module(); }

inline unsigned int DeFTMat::quarterID() const { return m_elementID.quarter(); }

inline unsigned int DeFTMat::layerID() const { return m_elementID.layer(); }

inline unsigned int DeFTMat::stationID() const { return m_elementID.station(); }

inline unsigned int DeFTMat::uniqueMatID() const { return m_elementID.uniqueMat(); }

inline unsigned int DeFTMat::uniqueModuleID() const { return m_elementID.uniqueModule(); }

inline unsigned int DeFTMat::uniqueQuarterID() const { return m_elementID.uniqueQuarter(); }

inline unsigned int DeFTMat::uniqueLayerID() const { return m_elementID.uniqueLayer(); }

inline LHCb::FTChannelID DeFTMat::elementID() const { return m_elementID; }

inline void DeFTMat::setElementID( const LHCb::FTChannelID& chanID ) { m_elementID = chanID; }

#endif // DeFTMat_H
