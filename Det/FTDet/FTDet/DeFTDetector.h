#ifndef DEFTDETECTOR_H
#define DEFTDETECTOR_H 1

// DetDesc
#include "DetDesc/DetectorElement.h"

// Kernel
#include "Kernel/FTChannelID.h"

// Local
#include "DeFTStation.h"
#include "DeFTLayer.h"
#include "DeFTQuarter.h"
#include "DeFTModule.h"

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

namespace DeFTDetectorLocation{
// FT location defined in the XmlDDDB
static const std::string& Default = "/dd/Structure/LHCb/AfterMagnetRegion/T/FT";
}

static const CLID CLID_DeFTDetector = 8601;

class DeFTDetector : public DetectorElement {

public:

  // Typedefs
  typedef std::vector<DeFTStation*>     Stations;

  /// Standard constructor
  DeFTDetector( const std::string& name = "" );

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
  const Stations& stations() const { return m_stations; }

  /** Find the FT Station where a global point is
   *  @return Pointer to the relevant Station
   */
  const DeFTStation* findStation(const Gaudi::XYZPoint& point) const;

  /** Find the FT Layer where a global point is
   *  @return Pointer to the relevant Layer
   */
  const DeFTLayer* findLayer(const Gaudi::XYZPoint& point) const;

  /** Find the FT Module where a global point is
   *  @return Pointer to the relevant Module
   */
  const DeFTQuarter* findQuarter(const Gaudi::XYZPoint& point) const;

  /** Find the FT Module where a global point is
   *  @return Pointer to the relevant Module
   */
  const DeFTModule* findModule(const Gaudi::XYZPoint& point) const;

  /** Find the FT Station corresponding to the channel id
   *  @return Pointer to the relevant station
   */
  const DeFTStation* findStation( const LHCb::FTChannelID id ) const;

  /** Find the FT Layer corresponding to the channel id
   *  @return Pointer to the relevant layer
   */
  const DeFTLayer* findLayer( const LHCb::FTChannelID id ) const;

  /** Find the FT Quarter corresponding to the channel id
   *  @return Pointer to the relevant quarter
   */
  const DeFTQuarter* findQuarter( const LHCb::FTChannelID id ) const;

  /** Find the FT Module corresponding to the channel id
   *  @return Pointer to the relevant module
   */
  const DeFTModule* findModule( const LHCb::FTChannelID id ) const;

  /** Get a random channelID using a seed between 0 and 1 */
  const LHCb::FTChannelID getRandomChannelFromSeed(const double seed) const;

private: // private data members

  unsigned int m_FTversion = 0;
  Stations m_stations;           ///< vector of pointers to stations

  unsigned int m_nTotChannels;
  unsigned int m_nModulesT1;
  unsigned int m_nModulesT2;
  unsigned int m_nModulesT3;
  unsigned int m_nLayers;
  unsigned int m_nQuarters;
  unsigned int m_nSiPMs;
  unsigned int m_nChannels;

  /// Use a single MsgStream instance (created in initialize)
  std::unique_ptr<MsgStream> m_msg;

}; //end of class

/// Find station methods
inline const DeFTStation* DeFTDetector::findStation(const LHCb::FTChannelID aChannel) const {
  return m_stations[aChannel.station()-1u];
}

/// Find layer methods
inline const DeFTLayer* DeFTDetector::findLayer(const LHCb::FTChannelID aChannel) const {
  const DeFTStation* s = findStation(aChannel);
  return s ? s->findLayer(aChannel) : 0;
}

/// Find quarter methods
inline const DeFTQuarter* DeFTDetector::findQuarter(const LHCb::FTChannelID aChannel) const {
  const DeFTLayer* l = findLayer(aChannel);
  return l ? l->findQuarter(aChannel) : 0;
}

/// Find module methods
inline const DeFTModule* DeFTDetector::findModule(const LHCb::FTChannelID aChannel) const {
  const DeFTQuarter* q = findQuarter(aChannel);
  return q ? q->findModule(aChannel) : 0;
}

#endif // DEFTDETECTOR_H
