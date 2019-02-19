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
#ifndef FTDET_DEFTQUARTER_H
#define FTDET_DEFTQUARTER_H 1

// DetDesc
#include "DetDesc/DetectorElement.h"

// Kernel
#include "Kernel/FTChannelID.h"

// Local
#include "FTDet/DeFTModule.h"

/** @class DeFTQuarter DeFTQuarter.h "FTDet/DeFTQuarter.h"
 *
 *  This is the detector element class for a Fibre Tracker Quarter.
 *
 *  @author Jeroen van Tilburg
 *  @date   2016-07-18
 *
 */

static const CLID CLID_DeFTQuarter = 8603;

class DeFTQuarter : public DetectorElement {

public:
  /** Some typedefs */
  typedef boost::container::static_vector<DeFTModule*, 6> Modules;

  /// Standard constructor
  using DetectorElement::DetectorElement;

  /** Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  const CLID& clID() const override;

  /** Another reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID& classID() { return CLID_DeFTQuarter; };

  /** Initialization method */
  StatusCode initialize() override;

  /** Const method to return the module for a given channel id
   * @param  aChannel  an FT channel id
   * @return pointer to detector element
   */
  const DeFTModule* findModule( const LHCb::FTChannelID aChannel ) const;

  /** Const method to return the module for a given XYZ point
   * @param  aPoint the given point
   * @return const pointer to detector element
   */
  const DeFTModule* findModule( const Gaudi::XYZPoint& aPoint ) const;

  /** Flat vector of all FT modules
   * @return vector of modules
   */
  const Modules& modules() const { return m_modules; }

  /** @return quarterID */
  unsigned int quarterID() const { return m_quarterID; }

  /** @return flag true if this quarter is bottom half */
  bool isBottom() const { return m_quarterID == 0 || m_quarterID == 1; }

  /** @return flag true if this quarter is top half */
  bool isTop() const { return m_quarterID == 2 || m_quarterID == 3; }

private:
  unsigned int m_quarterID; ///< quarter ID number
  Modules      m_modules;   ///< vector of modules

}; // end of class

/// Find module methods
inline const DeFTModule* DeFTQuarter::findModule( const LHCb::FTChannelID aChannel ) const {
  return aChannel.module() < m_modules.size() ? m_modules[aChannel.module()] : nullptr;
}

#endif // FTDET_DEFTQUARTER_H
