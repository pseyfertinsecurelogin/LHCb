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
#ifndef _DEUTDETECTOR_H_
#define _DEUTDETECTOR_H_

#include <string>
#include <vector>
#include <memory>

#include "Kernel/UTChannelID.h"
#include "Kernel/LHCbID.h"
#include "Kernel/Trajectory.h"

#include "DetDesc/DetectorElement.h"
#include "UTDet/DeUTBaseElement.h"

#include "GaudiKernel/VectorMap.h"

class DeUTStation;
class DeUTSector;
class DeUTLayer;

namespace DeUTDetLocation {

  /// UT location in transient detector store
  inline const std::string UT = "/dd/Structure/LHCb/BeforeMagnetRegion/UT";

  const std::string& location();

  unsigned int detType();

}

/** @class DeUTDetector DeUTDetector.h UTDet/DeUTDetector.h
 *
 *  UT Detector Element class
 *
 *  All UT elements are modification from TT element classes
 *  that were originally written by Matt Needham)
 *
 *  @author Andy Beiter (based on code by Jianchun Wang, Matt Needham)
 *  @date   2018-09-04
 *
 */

constexpr CLID CLID_DeUTDetector = 9301;

namespace {
  constexpr int NBSTATION = 2;
  constexpr int NBLAYER   = 2; // nbr layers per station
  constexpr int NBREGION  = 3; // nbr regions per layer
}

class DeUTDetector : public DetectorElement  {

public:

  typedef std::vector<DeUTSector*> Sectors;
  typedef std::vector<DeUTLayer*> Layers;
  typedef std::vector<DeUTStation*> Stations;

  /** Constructor */
  DeUTDetector ( const std::string& name = "" ) ;

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID& classID(){return CLID_DeUTDetector;}

  /**
  * another reference to class identifier
  * @return the class identifier for this class
  */
  const CLID& clID () const override;

  /** initialization method
  * @return Status of initialisation
  */
  StatusCode initialize() override;

  /** @return number of first station */
  unsigned int firstStation() const;

  /** @return number of last station */
  unsigned int lastStation() const;

  /** @return number of stations */
  unsigned int nStation() const;

  /** Implementation of sensitive volume identifier for a given point in the
      global reference frame. This is the sensor number defined in the xml.
  */
  int sensitiveVolumeID(const Gaudi::XYZPoint& globalPos) const override;

  /**  locate the station based on a channel id
  @return  station */
  DeUTStation* findStation(const LHCb::UTChannelID aChannel) const;

  /** locate station based on a point
  @return station */
  DeUTStation* findStation(const Gaudi::XYZPoint & point)  const;

  /**
  *  short cut to pick up the station corresponding to a given nickname
  * @param nickname
  * @return station
  */
  DeUTStation* findStation(const std::string& nickname) const;

  /**
  *  short cut to pick up the station corresponding to a given nickname
  * @param nickname
  * @return layer
  */
  DeUTLayer* findLayer(const std::string& nickname) const;

  /**  locate the layer based on a channel id
  @return  layer */
  DeUTLayer* findLayer(const LHCb::UTChannelID aChannel) const;

  /** locate layer based on a point
   *return layer */
  DeUTLayer* findLayer(const Gaudi::XYZPoint& point) const;

  /** check contains channel
  *  @param  aChannel channel
  *  @return bool
  */
  bool contains(const LHCb::UTChannelID aChannel) const;

  /// Workaround to prevent hidden base class function
  inline bool isValid() const override { return ValidDataObject::isValid(); }
  /// Workaround to prevent hidden base class function
  inline bool isValid(const Gaudi::Time& t) const override
                              { return ValidDataObject::isValid(t); }
  /** check channel number is valid */
  bool isValid(const LHCb::UTChannelID aChannel) const;

  /** vector of stattions
  * @return vector of stations
  */
  const Stations& stations() const;

  /** flat vector of sectors
  * @return vector of sectors
  */
  const Sectors& sectors() const;

  /** flat vector of layers
  * @return vector of layers
  */
  const Layers& layers() const;

  /**
  *  short cut to pick up the wafer corresponding to x,y,z
  * @param  aPoint point in global frame
  * @return sector
  */
  DeUTSector* findSector(const Gaudi::XYZPoint& aPoint) const;

  /**
  *  short cut to pick up the wafer corresponding to a channel
  * @param  aChannel channel
  * @return sector
  */
  DeUTSector* findSector(const LHCb::UTChannelID aChannel) const;

  /**
  *  get the sector corresponding to the input channel
  * @param  aChannel channel
  * @return sector
  */
  DeUTSector* getSector(const LHCb::UTChannelID aChannel) const;

  /**
  *  short cut to pick up the wafer corresponding to a given nickname
  * @param nickname
  * @return sector
  */
  DeUTSector* findSector(const std::string& nickname) const;

 /**
  *  find top level DeUTBaseElement by nickname
  * @param nickname
  * @return base element
  */
  DeUTBaseElement* findTopLevelElement(const std::string& nickname) const;

  /** get the next channel left */
  LHCb::UTChannelID nextLeft(const LHCb::UTChannelID testChan) const;

  /** get the next channel right */
  LHCb::UTChannelID nextRight(const LHCb::UTChannelID testChan) const;

  /** get the trajectory
   @return trajectory
  */
  std::unique_ptr<LHCb::Trajectory<double>> trajectory(const LHCb::LHCbID& id,
                                                       const double offset) const;

  /** get the trajectory representing the first strip
   @return trajectory
  */
  std::unique_ptr<LHCb::Trajectory<double>> trajectoryFirstStrip(const LHCb::LHCbID& id) const;

  /** get the trajectory representing the last strip
   @return trajectory
  */
  std::unique_ptr<LHCb::Trajectory<double>> trajectoryLastStrip(const LHCb::LHCbID& id) const;

  /** get the number of strips in detector*/
  unsigned int nStrip() const;


  /** get the number of layers **/
  unsigned int nLayer() const;

  /** get the number of readout sectors **/
  unsigned int nReadoutSector() const;

  /** number of layers per station **/
  unsigned int nLayersPerStation() const;

  /**
  * fraction active channels
  * @return bool fraction active
  */
  double fractionActive() const;

  DeUTSector* getSector(unsigned int station,
                        unsigned int layer,
                        unsigned int region,
                        unsigned int sector,
                        unsigned int uniqueSector) const;

  void setOffset();

    /** find a list of sectors from channelIDs **/
  Sectors findSectors(const std::vector<LHCb::UTChannelID>& vec) const;

  /** find a list of sectors from list of nicknames  **/
  Sectors findSectors(const std::vector<std::string>& vec) const;

  /** get list of all disabled sectors */
  Sectors disabledSectors() const;

  /** get list of disabled beetles */
  std::vector<LHCb::UTChannelID> disabledBeetles() const;

protected:

  /** set the first Station number */
  void setFirstStation(const unsigned int iStation);

  /** set the strip number  */
  void setNstrip(const unsigned int nStrip);

  Stations m_stations;

  Sectors m_sectors;

  Layers m_layers;

  typedef GaudiUtils::VectorMap<unsigned int,DeUTSector*> SectorMap;
  mutable SectorMap m_sMap;

private:

  unsigned int m_firstStation = 0u;
  unsigned int m_nStrip = 0u;

  /** make flat list of lowest descendents  and also layers */
  void flatten();
  /** offsets on the "flatten" list of sectors in order to have quicker access */
  std::array<size_t, NBSTATION * NBLAYER * NBREGION> m_offset;
};

inline const std::string& DeUTDetLocation::location(){
  return ( DeUTDetLocation::UT );
}

inline unsigned int DeUTDetLocation::detType(){
  return ( LHCb::UTChannelID::detType::typeUT );
}

#include "UTDet/DeUTSector.h"
#include "UTDet/DeUTStation.h"
#include "UTDet/DeUTLayer.h"

inline unsigned int DeUTDetector::firstStation() const{
  return m_firstStation;
}

inline unsigned int DeUTDetector::lastStation() const{
  return m_firstStation + m_stations.size()-1u;
}

inline unsigned int DeUTDetector::nStation() const{
  return m_stations.size();
}

inline bool DeUTDetector::contains(const LHCb::UTChannelID aChannel) const{
  return ((aChannel.station() >= firstStation())
	  &&(aChannel.station() < lastStation()));
}


inline unsigned int DeUTDetector::nStrip() const{
  return m_nStrip;
}

inline void DeUTDetector::setNstrip(const unsigned int nStrip) {
  m_nStrip = nStrip;
}

inline const DeUTDetector::Stations& DeUTDetector::stations() const{
  return m_stations;
}

inline const DeUTDetector::Sectors&  DeUTDetector::sectors() const{
  return m_sectors;
}

inline const DeUTDetector::Layers& DeUTDetector::layers() const{
  return m_layers;
}

inline LHCb::UTChannelID DeUTDetector::nextLeft(const LHCb::UTChannelID
                                                aChannel) const {
  const DeUTSector* aSector = findSector(aChannel);
  return (0 != aSector ? aSector->nextLeft(aChannel) :
          LHCb::UTChannelID(0u) );
}

inline LHCb::UTChannelID DeUTDetector::nextRight(const LHCb::UTChannelID
                                                 aChannel) const {
  const DeUTSector* aSector = findSector(aChannel);
  return (0 != aSector ? aSector->nextRight(aChannel) :
          LHCb::UTChannelID(0u) );
}

inline bool DeUTDetector::isValid(const LHCb::UTChannelID aChannel) const {
  const DeUTSector* aSector = findSector(aChannel);
  return (aSector != 0 ? aSector->isStrip(aChannel.strip()) : false);
}

inline DeUTSector* DeUTDetector::findSector(const LHCb::UTChannelID aChannel) const{
 SectorMap::iterator iter = m_sMap.find(aChannel.uniqueSector());
 return (iter != m_sMap.end() ? iter->second : 0);
}

inline unsigned int DeUTDetector::nLayer() const{
  return layers().size();
}

inline unsigned int DeUTDetector::nReadoutSector() const{
  return sectors().size();
}

inline unsigned int DeUTDetector::nLayersPerStation() const{
  return nLayer()/nStation();
}

inline DeUTDetector::Sectors DeUTDetector::findSectors(const std::vector<LHCb::UTChannelID>& vec) const {
  std::vector<DeUTSector*> sectors; sectors.reserve(vec.size());
  for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
    DeUTSector* aSector = findSector(*iter);
    if (aSector != 0) sectors.push_back(aSector);
  }  // for

  // ensure unique list
  sectors.erase(std::unique(sectors.begin(), sectors.end()), sectors.end());
  return sectors;
}

inline DeUTDetector::Sectors DeUTDetector::findSectors(const std::vector<std::string>& vec) const {
  std::vector<DeUTSector*> sectors; sectors.reserve(vec.size());
  for (auto iter = vec.begin(); iter != vec.end(); ++iter){
    DeUTSector* aSector = findSector(*iter);
    if (aSector) sectors.push_back(aSector);
  }  // for

  // ensure unique list
  sectors.erase(std::unique(sectors.begin(), sectors.end()), sectors.end());
  return sectors;
}

#endif // _DEUTDETECTOR_H
