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
#ifndef _DEUTSECTOR_H_
#define _DEUTSECTOR_H_

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Kernel/LHCbConstants.h"
#include "Kernel/LineTraj.h"
#include "Kernel/UTChannelID.h"
#include "UTDet/DeUTBaseElement.h"
#include "UTDet/DeUTSensor.h"

#include "GaudiKernel/Plane3DTypes.h"
#include "LHCbMath/LineTypes.h"

/** @class DeUTSector DeUTSector.h UTDet/DeUTSector.h
 *
 *  Class representing a UT Sector
 *
 *  @author Andy Beiter (based on code by Jianchun Wang, Matt Needham)
 *  @date   2018-09-04
 *
 */

class DeUTSensor;
class DeUTModule;

static const CLID CLID_DeUTSector = 9320;

class DeUTSector : public DeUTBaseElement {

public:
  /** status enum
   * <b> For details on definitions see:</b>
   * \li <a href="http://ckm.physik.unizh.ch/software/det/deadStrips.php"><b>documentation</b></a><p>
   */
  enum Status {
    OK              = 0,
    Open            = 1,
    Short           = 2,
    Pinhole         = 3,
    ReadoutProblems = 4,
    NotBonded       = 5,
    LowGain         = 6,
    Noisy           = 7,
    OtherFault      = 9,
    Dead            = 10,
    UnknownStatus   = 100
  };

  typedef std::vector<DeUTSensor*> Sensors;
  typedef LHCb::LineTraj<double>   UTTraj;

  /** parent type */
  typedef UTDetTraits<DeUTSector>::parent parent_type;

  /** Constructor */
  DeUTSector( const std::string& name = "" );

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID& classID() { return CLID_DeUTSector; }

  /**
   * another reference to class identifier
   * @return the class identifier for this class
   */
  const CLID& clID() const override;

  /**
   * Retrives the hybrid type
   * @return the hybrid type
   */
  std::string hybridType() const;

  /** initialization method
   * @return Status of initialisation
   */
  StatusCode initialize() override;

  /** column number */
  unsigned int column() const;

  /** row Number.... */
  unsigned int row() const;

  /** production ID --> in fact parent ID */
  unsigned int prodID() const;

  /** get the noise of the corresponding strip
   * @param aChannel channel
   * @return float noise of the strip
   */
  float noise( const LHCb::UTChannelID& aChannel ) const;

  /** get the average noise in the sector
   * @return float average noise
   */
  float sectorNoise() const;

  /** get the average noise of a beetle
   * @param beetle beetle number (1-4)
   * @return float average noise
   */
  float beetleNoise( const unsigned int& beetle ) const;

  /** get the average noise of a beetle port
   * @param beetle beetle number (1-4)
   * @param port beetle port number (1-3)
   * @return float average noise
   */
  float portNoise( const unsigned int& beetle, const unsigned int& port ) const;

  /** set the Noise of the corresponding strip
   * @param strip strip number
   * @param value Noise value
   */
  void setNoise( const unsigned int& strip, const float& value );

  /** set the Noise of the corresponding channel
   * @param chan channel
   * @param value Noise value
   */
  void setNoise( const LHCb::UTChannelID& chan, const float& value );

  /** set the Noise vector
   * @param values Noise vector
   */
  void setNoise( const std::vector<float>& values );

  /** get the Noise of the corresponding strip
   * @param aChannel channel
   * @return float noise of the strip
   */
  float rawNoise( const LHCb::UTChannelID& aChannel ) const;

  /** get the average raw noise in the sector
   * @return float average noise
   */
  float rawSectorNoise() const;

  /** get the average raw noise of a beetle
   * @param beetle beetle number (1-4)
   * @return float average noise
   */
  float rawBeetleNoise( const unsigned int& beetle ) const;

  /** get the average raw noise of a beetle port
   * @param beetle beetle number (1-4)
   * @param port beetle port number (1-3)
   * @return float average noise
   */
  float rawPortNoise( const unsigned int& beetle, const unsigned int& port ) const;

  /** get the common mode noise of the corresponding strip
   * @param aChannel channel
   * @return float noise of the strip
   */
  float cmNoise( const LHCb::UTChannelID& aChannel ) const;

  /** get the average common noise in the sector
   * @return float average noise
   */
  float cmSectorNoise() const;

  /** get the average common mode noise of a beetle
   * @param beetle beetle number (1-4)
   * @return float average noise
   */
  float cmBeetleNoise( const unsigned int& beetle ) const;

  /** get the average common mode noise of a beetle port
   * @param beetle beetle number (1-4)
   * @param port beetle port number (1-3)
   * @return float average noise
   */
  float cmPortNoise( const unsigned int& beetle, const unsigned int& port ) const;

  /** set the cmNoise of the corresponding strip
   * @param strip strip number
   * @param value cmNoise value
   */
  void setCMNoise( const unsigned int& strip, const float& value );

  /** set the cmNoise of the corresponding channel
   * @param chan channel
   * @param value cmNoise value
   */
  void setCMNoise( const LHCb::UTChannelID& chan, const float& value );

  /** set the cmNoise vector
   * @param values cmNoise vector
   */
  void setCMNoise( const std::vector<float>& values );

  /** set the ACD count from the electron number vector
   * @param values
   */
  void setADCConversion( const std::vector<double>& values );

  /** get the ADC count from the electron number
   * @param e electron number
   * @param aChannel channel
   * @return ADC count
   */
  double toADC( const double& e, const LHCb::UTChannelID& aChannel ) const;

  /** get the ADC count from the electron number
   * @param e electron number
   * @param aStrip strip number
   * @return ADC count
   */
  double toADC( const double& e, const unsigned int& aStrip ) const;

  /** get the electron number from the ADC count
   * @param val ADV count
   * @param aChannel channel
   * @return electron number
   */
  double toElectron( const double& val, const LHCb::UTChannelID& aChannel ) const;

  /** get the electron number from the ADC count
   * @param val ADV count
   * @param aStrip strip number
   * @return electron number
   */
  double toElectron( const double& val, const unsigned int& aStrip ) const;

  /** sector identfier
   * @return id
   */
  unsigned int id() const;

  /** set sector id */
  void setID( const unsigned int id );

  /** check whether contains
   *  @param  aChannel channel
   *  @return bool
   */
  bool contains( const LHCb::UTChannelID aChannel ) const override;

  /** detector pitch
   * @return pitch
   */
  double pitch() const;

  /** number of strips
   * @return number of strips
   */
  unsigned int nStrip() const;

  /**
   * check if valid strip number
   *
   */
  bool isStrip( const unsigned int strip ) const;

  /** trajectory
   * @return trajectory for the fit
   */
  LHCb::LineTraj<double> trajectory( const LHCb::UTChannelID& aChan, const double offset ) const;

  /** trajectory
   * @return trajectory of the first strip
   */
  LHCb::LineTraj<double> trajectoryFirstStrip() const;

  /** trajectory
   * @return trajectory of the last strip
   */
  LHCb::LineTraj<double> trajectoryLastStrip() const;

  /** Trajectory<double> parameterized along y-axis */
  void trajectory( unsigned int strip, double offset, double& dxdy, double& dzdy, double& xAtYEq0, double& zAtYEq0,
                   double& ybegin, double& yend ) const;

  /**
   * @return total capacitance
   * ie sensors, cable + pitch adaptor
   */
  double capacitance() const;

  /**
   * @return sensor Capacitance
   */
  double sensorCapacitance() const;

  /** strip length
   * @return strip length
   */
  double stripLength() const;

  /** thickness
   * @return double thickness
   */
  double thickness() const;

  /** get the next channel left
   * @return next chan left
   */
  LHCb::UTChannelID nextLeft( const LHCb::UTChannelID testChan ) const;

  /** get the next channel right
   * @return next chan left
   */
  LHCb::UTChannelID nextRight( const LHCb::UTChannelID testChan ) const;

  /// Workaround to prevent hidden base class function
  inline const std::type_info& type( const std::string& name ) const override {
    return ParamValidDataObject::type( name );
  }
  /**
   * @return std::string type
   */
  std::string type() const;

  /** @return double stereo angle */
  double angle() const;

  /** @return double sin of stereo angle */
  double sinAngle() const;

  /** @return cosine of stereo angle */
  double cosAngle() const;

  /** @return check if is a stereo ladder */
  bool isStereo() const;

  /** beetle corresponding to channel  1-3 (IT) 1-4 (TT)*/
  unsigned int beetle( const LHCb::UTChannelID& chan ) const;

  /** beetle corresponding to channel  1-3 (IT) 1-4 (TT)*/
  unsigned int beetle( const unsigned int strip ) const;

  /** n beetle
   * @return double nBeetles
   */
  unsigned int nBeetle() const;

  /** measured efficiency
   * @ return double measured Eff
   */
  double measEff() const;

  /** set measured Eff of sector  */
  void setMeasEff( const double measEff );

  /** Status of sector
   @return Status of readout sector
  */
  Status sectorStatus() const;

  /** Status of the Beetle corresponding to strip */
  Status beetleStatus( const LHCb::UTChannelID& chan ) const;

  /** Status of the Beetle with given id  1-3 (IT), 1-4 (TT) */
  Status beetleStatus( const unsigned int id ) const;

  /** vector of beetle status */
  std::vector<DeUTSector::Status> beetleStatus() const;

  /** Status of channel */
  Status stripStatus( const LHCb::UTChannelID& chan ) const;

  /** get vector of strip status for all strips in sector */
  std::vector<Status> stripStatus() const;

  /** set the sector status */
  void setSectorStatus( const Status& newStatus );

  /** set vector of beetleStatus
   * @param unsigned int beetle [numbering from 1]
   * @param Status newStatus
   **/
  void setBeetleStatus( const unsigned int beetle, const Status& newStatus );

  /** set vector of beetleStatus
   * @param LHCb::UTChannelID chan id of beetle
   * @param Status newStatus
   **/
  void setBeetleStatus( const LHCb::UTChannelID& chan, const Status& newStatus );

  /** set vector of beetleStatus
   * @param unsigned int strip [numbering from 1]
   * @param Status newStatus
   **/
  void setStripStatus( const unsigned int strip, const Status& newStatus );

  /** set vector of beetleStatus
   * @param LHCb::UTChannelID chan id of strip
   * @param Status newStatus
   **/
  void setStripStatus( const LHCb::UTChannelID& chan, const Status& newStatus );

  /** short cut for strip status ok
   * @return isOKStrip
   */
  bool isOKStrip( const LHCb::UTChannelID& chan ) const;

  /** strip to channel
   * @param strip
   * @return corresponding channel */
  LHCb::UTChannelID stripToChan( const unsigned int strip ) const;

  /** version */
  std::string versionString() const;

  /** dead width */
  double deadWidth() const;

  /** print to stream */
  std::ostream& printOut( std::ostream& os ) const override;

  /** print to msgstream */
  MsgStream& printOut( MsgStream& os ) const override;

  /** flat vector of sensors
   * @return vector of sensors
   */
  const Sensors& sensors() const;

  /** locate sensor based on a point
   * @return module */
  DeUTSensor* findSensor( const Gaudi::XYZPoint& point ) const;

  /** find the middle sensor. rounding down if odd **/
  DeUTSensor* middleSensor() const;

  /** check if inside the active area
   * @param  point point in global frame
   * @param  tol   tolerance
   * @return bool isInside
   **/
  bool globalInActive( const Gaudi::XYZPoint& point, Gaudi::XYZPoint tol = Gaudi::XYZPoint( 0., 0., 0. ) ) const;

  /** globalInActive
   * @param  point point in global frame
   * @param  tol   tolerance
   * @return bool in bondgap
   */
  bool globalInBondGap( const Gaudi::XYZPoint& point, double tol = 0 ) const;

  /**
   * Nickname for the sensor
   **/
  const std::string& nickname() const;

  /**
   * fraction active channels
   * @return bool fraction active
   */
  double fractionActive() const;

  /** direct access to the status condition, for experts only */
  const Condition* statusCondition() const;

  /** direct access to the noise condition, for experts only */
  const Condition* noiseCondition() const;

  /** x sense of local frame relative to global */
  bool xInverted() const;

  /** y sense of local frame relative to global */
  bool yInverted() const;

  std::string conditionsPathName() const;

  /** module type */
  std::string moduleType() const;

protected:
  bool m_isStereo = false;

  StatusCode registerConditionsCallbacks();
  StatusCode cacheInfo();

  Sensors     m_sensors;
  double      m_thickness = 0.0;
  std::string m_nickname;

private:
  typedef std::map<unsigned int, Status> StatusMap;

  parent_type* m_parent = nullptr;
  unsigned int m_row    = 0u;
  std::string  m_hybridType;
  std::string  m_conditionPathName;

  std::string moduleNumber( const unsigned int& chan, const unsigned int& reg ) const;

  StatusCode             updateStatusCondition();
  StatusCode             updateNoiseCondition();
  void                   toEnumMap( const std::map<int, int>& input, DeUTSector::StatusMap& output );
  LHCb::LineTraj<double> createTraj( const unsigned int strip, const double offset ) const;
  void setStatusCondition( const std::string& type, const unsigned int entry, const DeUTSector::Status& newStatus );

  unsigned int m_firstStrip  = 1;
  unsigned int m_firstBeetle = 1;
  unsigned int m_id          = 0u;
  double       m_pitch       = 0.0;
  unsigned int m_nStrip      = 0u;
  double       m_capacitance = 0.0;
  double       m_stripLength = 0.0;
  // std::pair<double, double> m_range;

  double      m_deadWidth = 0.0;
  std::string m_type;

  double           m_dxdy = 0.0;
  double           m_dzdy = 0.0;
  double           m_dy   = 0.0;
  Gaudi::XYZVector m_dp0di;
  Gaudi::XYZPoint  m_p0;
  double           m_angle    = 0.0;
  double           m_cosAngle = 0.0;
  double           m_sinAngle = 0.0;
  mutable double   m_measEff  = 0.0;

  // status info
  Status            m_status = OK;
  mutable StatusMap m_beetleStatus;
  mutable StatusMap m_stripStatus;
  std::string       m_statusString  = "Status";
  std::string       m_versionString = "DC06";

  // Noise info
  std::string         m_noiseString = "Noise";
  std::vector<float>  m_noiseValues;
  std::vector<double> m_electronsPerADC;
  std::vector<float>  m_cmModeValues;
};

inline unsigned int DeUTSector::id() const { return m_id; }

inline void DeUTSector::setID( unsigned int id ) { m_id = id; }

inline bool DeUTSector::contains( const LHCb::UTChannelID aChannel ) const {
  return ( aChannel.uniqueSector() == elementID().uniqueSector() );
}

inline double DeUTSector::pitch() const { return m_pitch; }

inline unsigned int DeUTSector::nStrip() const { return m_nStrip; }

inline std::string DeUTSector::versionString() const { return m_versionString; }

inline double DeUTSector::deadWidth() const { return m_deadWidth; }

inline bool DeUTSector::isStrip( const unsigned int strip ) const {
  return ( strip >= m_firstStrip && strip < m_firstStrip + m_nStrip );
}

inline double DeUTSector::capacitance() const { return m_capacitance; }

inline double DeUTSector::sensorCapacitance() const {
  const Sensors& theSensors = sensors();
  return theSensors.size() * theSensors.front()->capacitance();
  ;
}

inline double DeUTSector::stripLength() const { return m_stripLength; }

inline double DeUTSector::thickness() const { return m_thickness; }

inline std::string DeUTSector::type() const { return m_type; }

inline double DeUTSector::angle() const { return m_angle; }

inline double DeUTSector::sinAngle() const { return m_sinAngle; }

inline double DeUTSector::cosAngle() const { return m_cosAngle; }

inline bool DeUTSector::isStereo() const { return m_isStereo; }

inline void DeUTSector::trajectory( unsigned int strip, double offset, double& dxdy, double& dzdy, double& xAtYEq0,
                                    double& zAtYEq0, double& ybegin, double& yend ) const {
  const double numstrips = offset + strip - m_firstStrip;
  dxdy                   = m_dxdy;
  dzdy                   = m_dzdy;
  xAtYEq0                = m_p0.x() + numstrips * m_dp0di.x();
  zAtYEq0                = m_p0.z() + numstrips * m_dp0di.z();
  ybegin                 = m_p0.y() + numstrips * m_dp0di.y();
  yend                   = ybegin + m_dy;
}

inline unsigned int DeUTSector::beetle( const LHCb::UTChannelID& chan ) const { return beetle( chan.strip() ); }

inline unsigned int DeUTSector::beetle( const unsigned int strip ) const {
  return ( ( strip - 1u ) / LHCbConstants::nStripsInBeetle ) + 1u;
}

inline unsigned int DeUTSector::nBeetle() const { return nStrip() / LHCbConstants::nStripsInBeetle; }

inline void DeUTSector::setNoise( const LHCb::UTChannelID& chan, const float& value ) {
  // just delegate
  setNoise( chan.strip(), value );
}

inline void DeUTSector::setCMNoise( const LHCb::UTChannelID& chan, const float& value ) {
  // just delegate
  setCMNoise( chan.strip(), value );
}

inline DeUTSector::Status DeUTSector::sectorStatus() const { return m_status; }

inline DeUTSector::Status DeUTSector::beetleStatus( const LHCb::UTChannelID& chan ) const {
  return beetleStatus( beetle( chan ) );
}

inline void DeUTSector::setBeetleStatus( const LHCb::UTChannelID& chan, const DeUTSector::Status& newStatus ) {
  // just delegate
  setBeetleStatus( beetle( chan ), newStatus );
}

inline void DeUTSector::setStripStatus( const LHCb::UTChannelID& chan, const DeUTSector::Status& newStatus ) {
  // just delegate
  setStripStatus( chan.strip(), newStatus );
}

inline DeUTSector::Status DeUTSector::beetleStatus( const unsigned int id ) const {
  DeUTSector::Status theStatus = sectorStatus();
  if ( theStatus == DeUTSector::OK ) {
    DeUTSector::StatusMap::const_iterator iter = m_beetleStatus.find( id );
    if ( iter != m_beetleStatus.end() ) theStatus = iter->second;
  }
  return theStatus;
}

inline DeUTSector::Status DeUTSector::stripStatus( const LHCb::UTChannelID& chan ) const {
  DeUTSector::Status theStatus = beetleStatus( chan );
  if ( theStatus == DeUTSector::OK ) {
    DeUTSector::StatusMap::const_iterator iter = m_stripStatus.find( chan.strip() );
    if ( iter != m_stripStatus.end() ) theStatus = iter->second;
  }
  return theStatus;
}

inline bool DeUTSector::isOKStrip( const LHCb::UTChannelID& chan ) const {
  return ( stripStatus( chan ) == DeUTSector::OK ? true : false );
}

inline std::vector<DeUTSector::Status> DeUTSector::beetleStatus() const {
  std::vector<Status> vec;
  vec.resize( nBeetle() );
  for ( unsigned int iBeetle = m_firstBeetle; iBeetle <= nBeetle(); ++iBeetle ) {
    if ( sectorStatus() != DeUTSector::OK ) {
      vec[iBeetle - 1] = sectorStatus();
      continue;
    }
    DeUTSector::StatusMap::const_iterator iter = m_beetleStatus.find( iBeetle );
    if ( iter != m_beetleStatus.end() ) {
      vec[iBeetle - 1] = iter->second;
    } else {
      vec[iBeetle - 1] = DeUTSector::OK;
    }
  } // nStrip
  return vec;
}

inline std::vector<DeUTSector::Status> DeUTSector::stripStatus() const {
  std::vector<Status> vec;
  vec.resize( nStrip() );
  for ( unsigned int iStrip = m_firstStrip; iStrip <= nStrip(); ++iStrip ) {
    if ( sectorStatus() != DeUTSector::OK ) {
      vec[iStrip - 1] = sectorStatus();
      continue;
    }
    LHCb::UTChannelID chan = stripToChan( iStrip );
    if ( beetleStatus( chan ) != DeUTSector::OK ) {
      vec[iStrip - 1] = beetleStatus( chan );
      continue;
    }
    DeUTSector::StatusMap::const_iterator iter = m_stripStatus.find( iStrip );
    if ( iter != m_stripStatus.end() ) {
      vec[iStrip - 1] = iter->second;
    } else {
      vec[iStrip - 1] = DeUTSector::OK;
    }
  } // nStrip
  return vec;
}

inline LHCb::UTChannelID DeUTSector::stripToChan( const unsigned int strip ) const {
  return isStrip( strip ) ? LHCb::UTChannelID( elementID().type(), elementID().station(), elementID().layer(),
                                               elementID().detRegion(), elementID().sector(), strip )
                          : LHCb::UTChannelID( 0 );
}

inline const DeUTSector::Sensors& DeUTSector::sensors() const { return m_sensors; }

inline DeUTSensor* DeUTSector::middleSensor() const { return m_sensors[m_sensors.size() / 2u]; }

inline bool DeUTSector::xInverted() const { return middleSensor()->xInverted(); }

inline bool DeUTSector::yInverted() const { return middleSensor()->yInverted(); }

/** ouput operator for class DeUTSector
 *  @see DeUTSector
 *  @see MsgStream
 *  @param os      reference to STL output stream
 *  @param aSector reference to DeUTSector object
 */
inline std::ostream& operator<<( std::ostream& os, const DeUTSector* aSector ) { return aSector->printOut( os ); }

/** ouput operator for class DeUTSector
 *  @see DeUTSector
 *  @see MsgStream
 *  @param os      reference to MsgStream output stream
 *  @param aSector reference to DeUTSector object
 */
inline MsgStream& operator<<( MsgStream& os, const DeUTSector* aSector ) { return aSector->printOut( os ); }

inline const std::string& DeUTSector::nickname() const { return m_nickname; }

inline const Condition* DeUTSector::statusCondition() const { return condition( m_statusString ); }

inline const Condition* DeUTSector::noiseCondition() const { return condition( m_noiseString ); }

inline double DeUTSector::measEff() const { return m_measEff; }

#include "UTDet/DeUTModule.h"

inline unsigned int DeUTSector::column() const { return m_parent->column(); }

inline unsigned int DeUTSector::row() const { return m_row; }

inline std::string DeUTSector::moduleType() const { return m_parent->type(); }

#endif // _DEUTSECTOR_H
