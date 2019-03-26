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
#ifndef DEFTMODULE_H
#define DEFTMODULE_H 1

// DetDesc
#include "DetDesc/DetectorElement.h"

// Local
#include "FTDet/DeFTMat.h"

/// Kernel
#include "GaudiKernel/Plane3DTypes.h"
#include "Kernel/FTChannelID.h"

/** @class DeFTModule DeFTModule.h "FTDet/DeFTModule.h"
 *
 *  This is the detector element class of the Fibre Tracker (FT) half module.
 *
 *  @author Jeroen van Tilburg
 *  @date   2016-07-18
 */

static const CLID CLID_DeFTModule = 8605;

class DeFTModule : public DetectorElement {

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
  void setElementID( const LHCb::FTChannelID& chanID );

  /** @return flag true if this quarter is bottom half */
  bool isBottom() const { return quarterID() == 0 || quarterID() == 1; }

  /** @return flag true if this quarter is top half */
  bool isTop() const { return quarterID() == 2 || quarterID() == 3; }

  /** @return Vector of pointers to the FT Mats */
  const std::array<DeFTMat*, 4>& mats() const { return m_mats; }

  /** Find the FT Mat where a global point is
   *  @return Pointer to the relevant Mat
   */
  const DeFTMat* findMat( const Gaudi::XYZPoint& point ) const;

  /** Const method to return the mat for a given channel id
   * @param  aChannel  an FT channel id
   * @return pointer to detector element
   */
  const DeFTMat* findMat( const LHCb::FTChannelID& aChannel ) const;

  /** Returns the xy-plane at z-middle the layer */
  const Gaudi::Plane3D& plane() const { return m_plane; }

  /** Get the pseudoChannel for a given FTChannelID
   *  The pseudoChannel is defined for a full quarter and runs
   *  in increasing global |x|.
   *  This function is useful for occupancy plots.
   *  @param the FTChannelID
   *  @return the pseudoChannel, between 0 and 6*16*128
   */
  int pseudoChannel( LHCb::FTChannelID channelID ) const;

  /** Get the FTChannelID corresponding to a given pseudoChannel
   *  The pseudoChannel is defined for a full quarter and runs
   *  in increasing global |x|.
   *  This function is useful to generate noise form afterpulses.
   *  @param pseudoChannel, between 0 and 16*128
   *  @return the corresponding FTChannelID
   */
  LHCb::FTChannelID channelFromPseudo( const int pseudoChannel ) const;

private:
  LHCb::FTChannelID m_elementID; ///< element ID

  /// vector of pointers to mats
  std::array<DeFTMat*, 4> m_mats{{nullptr, nullptr, nullptr, nullptr}};

  int            m_nChannelsInModule; ///< number of channels per module
  Gaudi::Plane3D m_plane;             ///< xy-plane in the z-middle of the module
  bool           m_reversed;          ///< Flag set when the pseudochannel-ordering is reversed

}; // end of class

inline unsigned int DeFTModule::moduleID() const { return m_elementID.module(); }

inline unsigned int DeFTModule::quarterID() const { return m_elementID.quarter(); }

inline unsigned int DeFTModule::layerID() const { return m_elementID.layer(); }

inline unsigned int DeFTModule::stationID() const { return m_elementID.station(); }

inline LHCb::FTChannelID DeFTModule::elementID() const { return m_elementID; }

inline void DeFTModule::setElementID( const LHCb::FTChannelID& chanID ) { m_elementID = chanID; }

/// Find mat method
inline const DeFTMat* DeFTModule::findMat( const LHCb::FTChannelID& aChannel ) const { return m_mats[aChannel.mat()]; }

#endif // DEFTMODULE_H
