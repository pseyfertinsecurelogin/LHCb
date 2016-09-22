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

  typedef std::vector<DeFTLayer*>       Layers;
public: 

  /// Standard constructor
  DeFTStation( const std::string& name = "" );

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

  /** @return Vector of pointers to the FT Layers */
  const Layers&   layers()   const { return m_layers;   }

  /** Find the FT Layer where a global point is
   *  @return Pointer to the relevant Layer
   */
  const DeFTLayer* findLayer(const Gaudi::XYZPoint& point) const;

  /** Const method to return the layer for a given channel id
   * @param  aChannel  an OT channel id
   * @return pointer to detector element
   */
  const DeFTLayer* findLayer(const LHCb::FTChannelID& aChannel) const;

private: // private data members

  Layers m_layers;               ///< vector of pointers to layers

}; //end of class

/// Find layer methods
inline const DeFTLayer* DeFTStation::findLayer(const LHCb::FTChannelID& aChannel) const {
  return m_layers[aChannel.layer()];
}

#endif // DEFTSTATION_H
