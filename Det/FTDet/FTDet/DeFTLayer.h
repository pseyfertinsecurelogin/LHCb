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
  /** Some typedefs */
  typedef std::vector<DeFTQuarter*> Quarters;
  typedef std::vector<DeFTModule*> Modules;

  /// Standard constructor
  DeFTLayer( const std::string& name = "" );

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

  /** Returns the global z position of the layer */
  double globalZ() const { return m_globalZ; };

  /** Returns the xy-plane at z-middle the layer */
  Gaudi::Plane3D plane() const { return m_plane; };

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
  const Quarters& quarters() const;

private:

  Quarters m_quarters;             ///< vector of quarters
  Modules m_modules;               ///< vector of modules

  double m_globalZ;                ///< Global z position of layer closest to y-axis
  Gaudi::Plane3D m_plane;          ///< xy-plane in the z-middle of the layer
};

/// Find quarter methods
inline const DeFTQuarter* DeFTLayer::findQuarter(const LHCb::FTChannelID& aChannel) const {
  return m_quarters[aChannel.quarter()];
}

/// Find module methods
inline const DeFTModule* DeFTLayer::findModule(const LHCb::FTChannelID& aChannel) const {
  const DeFTQuarter* q = findQuarter(aChannel);
  return q ? q->findModule(aChannel) : 0;
}

#endif // DEFTLAYER_H
