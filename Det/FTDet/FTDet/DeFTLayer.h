#ifndef DEFTLAYER_H
#define DEFTLAYER_H 1

// DetDesc
#include "DetDesc/DetectorElement.h"

// Kernel
#include "Kernel/FTChannelID.h"

// local
#include "FTDet/DeFTQuarter.h"

static const CLID CLID_DeFTLayer = 8604;

class DeFTLayer : public DetectorElement {

public: 
  /** Some typedefs */
  typedef std::vector<DeFTQuarter*> Quarters;

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

  /** Const method to return the quarter for a given channel id
   * @param  aChannel  an FT channel id
   * @return pointer to detector element
   */
  const DeFTQuarter* findQuarter(const LHCb::FTChannelID& aChannel) const;

  /** Const method to return the quarter for a given XYZ point
   * @param  aPoint the given point
   * @return const pointer to detector element
   */
  const DeFTQuarter* findQuarter(const Gaudi::XYZPoint& aPoint) const;

  /** flat vector of quarters
   * @return vector of quarters
   */
  const Quarters& quarters() const;

private:

  Quarters m_quarters;            ///< vector of quarters

};

/// Find quarter methods
inline const DeFTQuarter* DeFTLayer::findQuarter(const LHCb::FTChannelID& aChannel) const {
  return m_quarters[aChannel.quarter()];
}

#endif // DEFTLAYER_H
