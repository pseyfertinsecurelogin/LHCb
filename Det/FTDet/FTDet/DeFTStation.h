/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef DEFTSTATION_H
#define DEFTSTATION_H 1

// DetDesc
#include "DetDesc/DetectorElement.h"

// Kernel
#include "Kernel/FTChannelID.h"

// Local
#include "FTDet/DeFTLayer.h"

/** @class DeFTStation DeFTStation.h "FTDet/DeFTStation.h"
 *
 *  This is the detector element class of the Fibre Tracker (FT) station.
 *
 *  @author Jeroen van Tilburg
 *  @date   2016-07-18
 */

static const CLID CLID_DeFTStation = 8602;

class DeFTStation : public DetectorElement {

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
  static const CLID& classID() { return CLID_DeFTStation; }

  /** @return stationID */
  unsigned int stationID() const { return m_stationID;}

  /** @return Vector of pointers to the FT Layers */
  const std::array<DeFTLayer*,4>&  layers()  const { return m_layers; }

   /** Find the FT Layer where a global point is
   *  @return Pointer to the relevant Layer
   */
  const DeFTLayer* findLayer(const Gaudi::XYZPoint& point) const;

  /** Const method to return the layer for a given channel id
   * @param  aChannel an FT channel id
   * @return pointer to detector element
   */
  const DeFTLayer* findLayer(const LHCb::FTChannelID& aChannel) const;

private: // private data members

  /// array of pointers to layers
  std::array<DeFTLayer*,4> m_layers{{nullptr,nullptr,nullptr, nullptr}};

  unsigned int m_stationID;      ///< station ID number

}; //end of class

/// Find layer methods
inline const DeFTLayer* DeFTStation::findLayer(const LHCb::FTChannelID& aChannel) const {
  return m_layers[aChannel.layer()];
}

#endif // DEFTSTATION_H
