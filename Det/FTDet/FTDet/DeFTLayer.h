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
#ifndef DEFTLAYER_H
#define DEFTLAYER_H 1

// DetDesc
#include "DetDesc/DetectorElement.h"

// Kernel
#include "Kernel/FTChannelID.h"

#include "GaudiKernel/Plane3DTypes.h"

// local
#include "FTDet/DeFTQuarter.h"

static const CLID CLID_DeFTLayer = 8604;

class DeFTLayer : public DetectorElement {

public: 

  /// Standard constructor
  using DetectorElement::DetectorElement;

  /// Initialization method
  StatusCode initialize() override;

  /** Retrieves reference to class identifier
   *  @return The class identifier for this class
   */
  const CLID& clID() const override;

  /** Another reference to class identifier
   *  @return The class identifier for this class
   */
  static const CLID& classID() { return CLID_DeFTLayer; }

  /** @return quarterID */
  unsigned int layerID() const { return m_layerID;}

  /** Returns the global z position of the layer */
  float globalZ() const { return m_globalZ; }

  /** Returns the xy-plane at z-middle the layer */
  const Gaudi::Plane3D& plane() const { return m_plane; }

  /** Returns the stereo angle of the layer */
  float stereoAngle() const { return m_stereoAngle; }

  /** Returns the dx/dy of the layer (ie. tan(m_stereoAngle)) */
  float dxdy() const { return m_dxdy; }

  /** Returns the dz/dy of the layer (ie. tan of the beam angle) */
  float dzdy() const { return m_dzdy; }

  /** Returns the size of the layer in x */
  float sizeX() const { return m_sizeX; }

  /** Returns the size of the layer in y */
  float sizeY() const { return m_sizeY; }

  /** Const method to return the quarter for a given XYZ point
   * @param  aPoint the given point
   * @return const pointer to detector element
   */
  const DeFTQuarter* findQuarter(const Gaudi::XYZPoint& aPoint) const;

  /** Find the FT Module where a global point is
   *  @return Pointer to the relevant Module
   */
  const DeFTModule* findModule(const Gaudi::XYZPoint& aPoint) const;

  /** Const method to return the quarter for a given channel id
   * @param  aChannel  an FT channel id
   * @return pointer to detector element
   */
  const DeFTQuarter* findQuarter(const LHCb::FTChannelID& aChannel) const;

  /** Find the FT Module corresponding to the channel id
   *  @return Pointer to the relevant module
   */
  const DeFTModule* findModule( const LHCb::FTChannelID& aChannel) const;

  /** flat vector of quarters
   * @return vector of quarters
   */
  const std::array<DeFTQuarter*,4>& quarters() const {return m_quarters;}

private:

  /// vector of quarters
  std::array<DeFTQuarter*,4> m_quarters{{nullptr,nullptr,nullptr, nullptr}};

  ///< flat vector of modules
  boost::container::static_vector<DeFTModule*,24> m_modules;

  unsigned int m_layerID;          ///< layer ID number
  float m_globalZ;                ///< Global z position of layer closest to y-axis
  Gaudi::Plane3D m_plane;          ///< xy-plane in the z-middle of the layer
  float m_dzdy;                   ///< dz/dy of the layer (tan of the beam angle)
  float m_stereoAngle;            ///< stereo angle of the layer
  float m_dxdy;                   ///< dx/dy of the layer (ie. tan(m_stereoAngle))
  float m_sizeX;                  ///< Size of the layer in x
  float m_sizeY;                  ///< Size of the layer in y

};

/// Find quarter methods
inline const DeFTQuarter* DeFTLayer::findQuarter(const LHCb::FTChannelID& aChannel) const {
  return m_quarters[aChannel.quarter()];
}

/// Find module methods
inline const DeFTModule* DeFTLayer::findModule(const LHCb::FTChannelID& aChannel) const {
  const DeFTQuarter* q = findQuarter(aChannel);
  return q ? q->findModule(aChannel) : nullptr;
}

#endif // DEFTLAYER_H
