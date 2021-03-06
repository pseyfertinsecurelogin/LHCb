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
#ifndef _DeSTDetector_H_
#define _DeSTDetector_H_

#include <memory>
#include <string>
#include <vector>

#include "Kernel/LHCbID.h"
#include "Kernel/STChannelID.h"
#include "Kernel/Trajectory.h"

#include "DetDesc/DetectorElement.h"
#include "STDet/DeSTBaseElement.h"

#include "GaudiKernel/VectorMap.h"

/** @class DeSTDetector DeSTDetector.h "STDet/DeSTDetector.h"
 *
 *  Base class for TT and IT Detector Elements
 *
 *  @author Matthew Needham Matthew.Needham@cern.ch
 *
 * <b> Additional information: </b>
 * \li <a href="http://doc.cern.ch//archive/electronic/cern/others/LHB/internal/lhcb-2006-034.pdf"><b>LHCb note on
 STDet</b></a> \li  <a href="http://ckm.physik.unizh.ch/software"><b>ST Software page</b></a><p>
*/

class DeSTStation;
class DeSTSector;
class DeSTLayer;

namespace DeSTDetLocation {

  /// TT location in transient detector store
  inline const std::string TT = "/dd/Structure/LHCb/BeforeMagnetRegion/TT";

  /// IT location in transient detector store
  inline const std::string IT = "/dd/Structure/LHCb/AfterMagnetRegion/T/IT";

  /// "short cut" if you know the type, ie TT or IT
  const std::string& location( const std::string& type );

  /// type if you know the location
  unsigned int detType( const std::string& type );

} // namespace DeSTDetLocation

class DeSTDetector : public DetectorElement {

public:
  typedef std::vector<DeSTSector*>  Sectors;
  typedef std::vector<DeSTLayer*>   Layers;
  typedef std::vector<DeSTStation*> Stations;

  /** Constructor */
  DeSTDetector( const std::string& name = "" );

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
  int sensitiveVolumeID( const Gaudi::XYZPoint& globalPos ) const override;

  /**  locate the station based on a channel id
  @return  station */
  DeSTStation* findStation( const LHCb::STChannelID aChannel ) const;

  /** locate station based on a point
  @return station */
  DeSTStation* findStation( const Gaudi::XYZPoint& point ) const;

  /**
   *  short cut to pick up the station corresponding to a given nickname
   * @param nickname
   * @return station
   */
  DeSTStation* findStation( const std::string& nickname ) const;

  /**
   *  short cut to pick up the station corresponding to a given nickname
   * @param nickname
   * @return layer
   */
  DeSTLayer* findLayer( const std::string& nickname ) const;

  /**  locate the layer based on a channel id
  @return  layer */
  DeSTLayer* findLayer( const LHCb::STChannelID aChannel ) const;

  /** locate layer based on a point
   *return layer */
  DeSTLayer* findLayer( const Gaudi::XYZPoint& point ) const;

  /** check contains channel
   *  @param  aChannel channel
   *  @return bool
   */
  bool contains( const LHCb::STChannelID aChannel ) const;

  /// Workaround to prevent hidden base class function
  inline bool isValid() const override { return ValidDataObject::isValid(); }
  /// Workaround to prevent hidden base class function
  inline bool isValid( const Gaudi::Time& t ) const override { return ValidDataObject::isValid( t ); }
  /** check channel number is valid */
  bool isValid( const LHCb::STChannelID aChannel ) const;

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
  virtual DeSTSector* findSector( const Gaudi::XYZPoint& aPoint ) const = 0;

  /**
   *  short cut to pick up the wafer corresponding to a channel
   * @param  aChannel channel
   * @return sector
   */
  DeSTSector* findSector( const LHCb::STChannelID aChannel ) const;

  /**
   *  get the sector corresponding to the input channel
   * @param  aChannel channel
   * @return sector
   */
  virtual DeSTSector* getSector( const LHCb::STChannelID aChannel ) const { return findSector( aChannel ); };

  /**
   *  short cut to pick up the wafer corresponding to a given nickname
   * @param nickname
   * @return sector
   */
  DeSTSector* findSector( const std::string& nickname ) const;

  /**
   *  find top level DeSTBaseElement by nickname
   * @param nickname
   * @return base element
   */
  virtual DeSTBaseElement* findTopLevelElement( const std::string& nickname ) const = 0;

  /** get the next channel left */
  LHCb::STChannelID nextLeft( const LHCb::STChannelID testChan ) const;

  /** get the next channel right */
  LHCb::STChannelID nextRight( const LHCb::STChannelID testChan ) const;

  /** get the trajectory
   @return trajectory
  */
  std::unique_ptr<LHCb::Trajectory<double>> trajectory( const LHCb::LHCbID& id, const double offset ) const;

  /** get the trajectory representing the first strip
   @return trajectory
  */
  std::unique_ptr<LHCb::Trajectory<double>> trajectoryFirstStrip( const LHCb::LHCbID& id ) const;

  /** get the trajectory representing the last strip
   @return trajectory
  */
  std::unique_ptr<LHCb::Trajectory<double>> trajectoryLastStrip( const LHCb::LHCbID& id ) const;

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

  /** find a list of sectors from channelIDs **/
  Sectors findSectors( const std::vector<LHCb::STChannelID>& vec ) const;

  /** find a list of sectors from list of nicknames  **/
  Sectors findSectors( const std::vector<std::string>& vec ) const;

  /** get list of all disabled sectors */
  Sectors disabledSectors() const;

  /** get list of disabled beetles */
  std::vector<LHCb::STChannelID> disabledBeetles() const;

protected:
  /** set the first Station number */
  void setFirstStation( const unsigned int iStation );

  /** set the strip number  */
  void setNstrip( const unsigned int nStrip );

  Stations m_stations;

  Sectors m_sectors;

  Layers m_layers;

  typedef GaudiUtils::VectorMap<unsigned int, DeSTSector*> SectorMap;
  mutable SectorMap                                        m_sMap;

private:
  unsigned int m_firstStation = 0u;
  unsigned int m_nStrip       = 0u;
};

inline const std::string& DeSTDetLocation::location( const std::string& type ) {
  return ( type == "TT" ? DeSTDetLocation::TT : DeSTDetLocation::IT );
}

inline unsigned int DeSTDetLocation::detType( const std::string& type ) {
  return ( type == "TT" ? LHCb::STChannelID::detType::typeTT : LHCb::STChannelID::detType::typeIT );
}

#include "STDet/DeSTLayer.h"
#include "STDet/DeSTSector.h"
#include "STDet/DeSTStation.h"

inline unsigned int DeSTDetector::firstStation() const { return m_firstStation; }

inline unsigned int DeSTDetector::lastStation() const { return m_firstStation + m_stations.size() - 1u; }

inline unsigned int DeSTDetector::nStation() const { return m_stations.size(); }

inline bool DeSTDetector::contains( const LHCb::STChannelID aChannel ) const {
  return ( ( aChannel.station() >= firstStation() ) && ( aChannel.station() < lastStation() ) );
}

inline unsigned int DeSTDetector::nStrip() const { return m_nStrip; }

inline void DeSTDetector::setNstrip( const unsigned int nStrip ) { m_nStrip = nStrip; }

inline const DeSTDetector::Stations& DeSTDetector::stations() const { return m_stations; }

inline const DeSTDetector::Sectors& DeSTDetector::sectors() const { return m_sectors; }

inline const DeSTDetector::Layers& DeSTDetector::layers() const { return m_layers; }

inline LHCb::STChannelID DeSTDetector::nextLeft( const LHCb::STChannelID aChannel ) const {
  const DeSTSector* aSector = findSector( aChannel );
  return ( 0 != aSector ? aSector->nextLeft( aChannel ) : LHCb::STChannelID( 0u ) );
}

inline LHCb::STChannelID DeSTDetector::nextRight( const LHCb::STChannelID aChannel ) const {
  const DeSTSector* aSector = findSector( aChannel );
  return ( 0 != aSector ? aSector->nextRight( aChannel ) : LHCb::STChannelID( 0u ) );
}

inline bool DeSTDetector::isValid( const LHCb::STChannelID aChannel ) const {
  const DeSTSector* aSector = findSector( aChannel );
  return ( aSector != 0 ? aSector->isStrip( aChannel.strip() ) : false );
}

inline DeSTSector* DeSTDetector::findSector( const LHCb::STChannelID aChannel ) const {
  SectorMap::iterator iter = m_sMap.find( aChannel.uniqueSector() );
  return ( iter != m_sMap.end() ? iter->second : 0 );
}

inline unsigned int DeSTDetector::nLayer() const { return layers().size(); }

inline unsigned int DeSTDetector::nReadoutSector() const { return sectors().size(); }

inline unsigned int DeSTDetector::nLayersPerStation() const { return nLayer() / nStation(); }

inline DeSTDetector::Sectors DeSTDetector::findSectors( const std::vector<LHCb::STChannelID>& vec ) const {
  std::vector<DeSTSector*> sectors;
  sectors.reserve( vec.size() );
  for ( auto iter = vec.begin(); iter != vec.end(); ++iter ) {
    DeSTSector* aSector = findSector( *iter );
    if ( aSector != 0 ) sectors.push_back( aSector );
  } // for

  // ensure unique list
  sectors.erase( std::unique( sectors.begin(), sectors.end() ), sectors.end() );
  return sectors;
}

inline DeSTDetector::Sectors DeSTDetector::findSectors( const std::vector<std::string>& vec ) const {
  std::vector<DeSTSector*> sectors;
  sectors.reserve( vec.size() );
  for ( auto iter = vec.begin(); iter != vec.end(); ++iter ) {
    DeSTSector* aSector = findSector( *iter );
    if ( aSector ) sectors.push_back( aSector );
  } // for

  // ensure unique list
  sectors.erase( std::unique( sectors.begin(), sectors.end() ), sectors.end() );
  return sectors;
}

#endif // _DeSTDetector_H
