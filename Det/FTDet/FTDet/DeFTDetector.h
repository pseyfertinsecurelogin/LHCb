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
#ifndef DEFTDETECTOR_H
#define DEFTDETECTOR_H 1

// DetDesc
#include "DetDesc/DetectorElement.h"

// Kernel
#include "Kernel/FTChannelID.h"

// Local
#include "DeFTLayer.h"
#include "DeFTMat.h"
#include "DeFTModule.h"
#include "DeFTQuarter.h"
#include "DeFTStation.h"

/** @class DeFTDetector DeFTDetector.h "FTDet/DeFTDetector.h"
 *
 *  This is the top-level detector element class of the Fibre Tracker (FT).
 *  It has member variables holding pointers to the detector sub-structure
 *  det. elements (Stations, Layers, Quarters, Modules). Provides methods for
 *  the correspondence geometrical point <--> FTChannelID.
 *
 *  An T station (e.g. T1) contains 4 layers (x,u,v,x). The layers
 *  contain 4 quarters each. The quarters have 5 or 6 modules each.
 *  The modules are split in an upper and
 *  lower side.
 *
 *  The numbering scheme for the FT modules in the digitization is:
 *
 *  @verbatim
                                                        ^ Y
          Quarter 3                      Quarter 2      |
       ______________________   ______________________  |
       |  |  |  |  |  |  |  |   |  |  |  |  |  |  |  |  |
       |  |  |  |  |  |  |  |   |  |  |  |  |  |  |  |  |
       |  |  |  |  |  |  |  |   |  |  |  |  |  |  |  |  |
       | 6| 5| 4| 3| 2| 1| 0|   | 0| 1| 2| 3| 4| 5| 6|  |
       |  |  |  |  |  |  |  |   |  |  |  |  |  |  |  |  |
       |  |  |  |  |  |  |  |   |  |  |  |  |  |  |  |  |
       |  |  |  |  |  |  |  |   |  |  |  |  |  |  |  |  |
       |  |  |  |  |  |  |__|   |__|  |  |  |  |  |  |  |
       |--|--|--|--|--|--|__     __|--|--|--|--|--|--|  |
       |  |  |  |  |  |  |  |   |  |  |  |  |  |  |  |  |
       |  |  |  |  |  |  |  |   |  |  |  |  |  |  |  |  |
       |  |  |  |  |  |  |  |   |  |  |  |  |  |  |  |  |
       |  |  |  |  |  |  |  |   |  |  |  |  |  |  |  |  |
       | 6| 5| 4| 3| 2| 1| 0|   | 0| 1| 2| 3| 4| 5| 6|  |
       |  |  |  |  |  |  |  |   |  |  |  |  |  |  |  |  |
       |__|__|__|__|__|__|__|   |__|__|__|__|__|__|__|  |
                                                        |
           Quarter 1                      Quarter 0     |
                                                        |
   <-----------------------------------------------------
   X

 *  @endverbatim
 *
 *  @author Jeroen van Tilburg
 *  @date   2016-07-18
 */

namespace DeFTDetectorLocation {
  // FT location defined in the XmlDDDB
  inline const std::string Default = "/dd/Structure/LHCb/AfterMagnetRegion/T/FT";
} // namespace DeFTDetectorLocation

constexpr CLID CLID_DeFTDetector = 8601;

class DeFTDetector : public DetectorElement {

public:
  static constexpr uint16_t nQuarters = 48;

  /// Standard constructor
  using DetectorElement::DetectorElement;

  /** Initialization method
   *  @return Status of initialization
   */
  StatusCode initialize() override;

  /** Finalization method - delete objects created with new
   *  @return Status of finalization
   */
  StatusCode finalize();

  /** Retrieves reference to class identifier
   *  @return The class identifier for this class
   */
  const CLID& clID() const override;

  /** Another reference to class identifier
   *  @return The class identifier for this class
   */
  static const CLID& classID() { return CLID_DeFTDetector; }

  /** @return Ft version  */
  int version() const { return m_FTversion; }

  /** @return Vector of pointers to the FT Stations */
  const std::array<DeFTStation*, 3>& stations() const { return m_stations; }

  /** Find the FT Station where a global point is
   *  @return Pointer to the relevant Station
   */
  const DeFTStation* findStation( const Gaudi::XYZPoint& point ) const;

  /** Find the FT Layer where a global point is
   *  @return Pointer to the relevant Layer
   */
  const DeFTLayer* findLayer( const Gaudi::XYZPoint& point ) const;

  /** Find the FT Module where a global point is
   *  @return Pointer to the relevant Module
   */
  const DeFTQuarter* findQuarter( const Gaudi::XYZPoint& point ) const;

  /** Find the FT Module where a global point is
   *  @return Pointer to the relevant Module
   */
  const DeFTModule* findModule( const Gaudi::XYZPoint& point ) const;

  /** Find the FT Module where a global point is
   *  @return Pointer to the relevant Module
   */
  const DeFTMat* findMat( const Gaudi::XYZPoint& point ) const;

  /** Find the FT Station corresponding to the channel id
   *  @return Pointer to the relevant station
   */
  const DeFTStation* findStation( const LHCb::FTChannelID& id ) const;

  /** Find the FT Layer corresponding to the channel id
   *  @return Pointer to the relevant layer
   */
  const DeFTLayer* findLayer( const LHCb::FTChannelID& id ) const;

  /** Find the FT Quarter corresponding to the channel id
   *  @return Pointer to the relevant quarter
   */
  const DeFTQuarter* findQuarter( const LHCb::FTChannelID& id ) const;

  /** Find the FT Module corresponding to the channel id
   *  @return Pointer to the relevant module
   */
  const DeFTModule* findModule( const LHCb::FTChannelID& id ) const;

  /** Find the FT Mat corresponding to the channel id
   *  @return Pointer to the relevant module
   */
  const DeFTMat* findMat( const LHCb::FTChannelID& id ) const;

  /** Find the FT Quarter from the sequential number
   *  @return Pointer to the relevant quarter
   */
  const DeFTQuarter* quarter( int iQuarter ) const { return m_quarters[iQuarter]; };

  /**
   * Return a sensitive volume identifier for a given point in the
   * global reference frame. This function is vital for Gauss.
   */
  int sensitiveVolumeID( const Gaudi::XYZPoint& point ) const override {
    const DeFTMat* mat = findMat( point );
    return mat ? mat->sensitiveVolumeID( point ) : -1;
  }

  /** Get a random channelID using a seed between 0 and 1 */
  LHCb::FTChannelID getRandomChannelFromSeed( const double seed ) const;

  /// Get a random FTChannelID from a pseudoChannel and a seed (useful for the AP noise)
  LHCb::FTChannelID getRandomChannelFromPseudo( const int pseudoChannel, const double seed ) const;

  /// Get the total number of channels in the FT detector
  int nStations() const { return m_nStations; }

  /// Get the total number of channels in the FT detector
  int nChannels() const { return m_nTotChannels; }

private: // private data members
  unsigned int m_FTversion = 0;

  /// vector of pointers to stations
  std::array<DeFTStation*, 3> m_stations{{nullptr, nullptr, nullptr}};
  unsigned int                m_nStations;

  /// flact vector of pointers to quarters for fast access
  std::array<DeFTQuarter*, nQuarters> m_quarters;

  unsigned int m_nModulesT1;
  unsigned int m_nModulesT2;
  unsigned int m_nModulesT3;
  unsigned int m_nLayers;
  unsigned int m_nQuarters;
  unsigned int m_nTotQuarters;
  unsigned int m_nChannelsInModule;
  unsigned int m_nTotChannels;

  /// Use a single MsgStream instance (created in initialize)
  std::unique_ptr<MsgStream> m_msg;

}; // end of class

/// Find station methods
inline const DeFTStation* DeFTDetector::findStation( const LHCb::FTChannelID& aChannel ) const {
  return m_stations[aChannel.station() - 1u];
}

/// Find layer methods
inline const DeFTLayer* DeFTDetector::findLayer( const LHCb::FTChannelID& aChannel ) const {
  const DeFTStation* s = findStation( aChannel );
  return s ? s->findLayer( aChannel ) : 0;
}

/// Find quarter methods
inline const DeFTQuarter* DeFTDetector::findQuarter( const LHCb::FTChannelID& aChannel ) const {
  const DeFTLayer* l = findLayer( aChannel );
  return l ? l->findQuarter( aChannel ) : 0;
}

/// Find module methods
inline const DeFTModule* DeFTDetector::findModule( const LHCb::FTChannelID& aChannel ) const {
  const DeFTQuarter* q = findQuarter( aChannel );
  return q ? q->findModule( aChannel ) : 0;
}

/// Find module methods
inline const DeFTMat* DeFTDetector::findMat( const LHCb::FTChannelID& aChannel ) const {
  const DeFTModule* m = findModule( aChannel );
  return m ? m->findMat( aChannel ) : 0;
}

#endif // DEFTDETECTOR_H
