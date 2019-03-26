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
#ifndef _DEUTSENSOR_H_
#define _DEUTSENSOR_H_

#include <memory>
#include <string>

#include "GaudiKernel/Plane3DTypes.h"
#include "GaudiKernel/SystemOfUnits.h"
#include "Kernel/LineTraj.h"
#include "Kernel/UTChannelID.h"
#include "LHCbMath/LineTypes.h"
#include "UTDet/DeUTBaseElement.h"

/** @class DeUTSensor DeUTSensor.h UTDet/DeUTSensor.h
 *
 *  Class representing a UT Sensor
 *
 *  @author Andy Beiter (based on code by Jianchun Wang, Matt Needham)
 *  @date   2018-09-04
 *
 */

class DeUTSector;

static const CLID CLID_DeUTSensor = 9330;

class DeUTSensor : public DeUTBaseElement {

public:
  /** parent type */
  typedef UTDetTraits<DeUTSensor>::parent parent_type;

  /** Constructor */
  DeUTSensor( const std::string& name = "" );

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID& classID() { return CLID_DeUTSensor; }

  /**
   * another reference to class identifier
   * @return the class identifier for this class
   */
  const CLID& clID() const override;

  /** initialization method
   * @return Status of initialisation
   */
  StatusCode initialize() override;

  /** check whether contains
   *  @param  aChannel channel
   *  @return bool
   */
  bool contains( const LHCb::UTChannelID aChannel ) const override;

  /** sector identfier
   * @return id
   */
  unsigned int id() const;

  /** set sector id */
  void setID( const unsigned int id );

  /** detector pitch
   * @return pitch
   */
  float pitch() const;

  /** number of strips
   * @return number of strips
   */
  unsigned int nStrip() const;

  /**
   * check if valid strip number
   *
   */
  bool isStrip( const unsigned int strip ) const;

  /** convert a local u to a strip
   * @param  u local u
   * @return bool strip
   **/
  unsigned int localUToStrip( double u ) const;

  /** convert strip to local U
   * @param strip
   * @param offset
   * @return local u
   */
  double localU( unsigned int strip, double offset = 0. ) const;

  /** trajectory
   * @return trajectory for the fit
   */
  LHCb::LineTraj<double> trajectory( unsigned int strip, double offset ) const;

  /** plane corresponding to the sector
   * @return the plane
   */
  Gaudi::Plane3D plane() const;

  /** plane corresponding to the module entrance
   * @return the plane
   */
  Gaudi::Plane3D entryPlane() const;

  /** plane corresponding to the module exit
   * @return the plane
   */
  Gaudi::Plane3D exitPlane() const;

  /** localInActive
   * @param  point point in local frame
   * @param  tol   tolerance
   * @return in active region
   */
  bool localInActive( const Gaudi::XYZPoint& point, Gaudi::XYZPoint tol = Gaudi::XYZPoint( 0., 0., 0. ) ) const;

  /** globalInActive
   * @param  point point in global frame
   * @param  tol   tolerance
   * @return bool in active region
   */
  bool globalInActive( const Gaudi::XYZPoint& point, Gaudi::XYZPoint tol = Gaudi::XYZPoint( 0., 0., 0. ) ) const;

  /** localInBondGap
   * @param  v     coordinate in local frame
   * @param  tol   tolerance
   * @return bool if in bond gap
   */
  bool localInBondGap( const double v, const double tol ) const;

  /** globalInActive
   * @param  point point in global frame
   * @param  tol   tolerance
   * @return bool in bondgap
   */
  bool globalInBondGap( const Gaudi::XYZPoint& point, double tol = 0 ) const;

  /** thickness
   * @return float thickness
   */
  float thickness() const;

  /** active height
   * @return float activeHeight
   **/
  float activeHeight() const;

  /** active height
   * @return float activeWidth
   **/
  float activeWidth() const;

  /**  sensor capacitance **/
  double capacitance() const;

  /** x sense of local frame relative to global */
  bool xInverted() const;

  /** y sense of local frame relative to global */
  bool yInverted() const;

  /** print to stream */
  std::ostream& printOut( std::ostream& os ) const override;

  /** print to msgstream */
  MsgStream& printOut( MsgStream& os ) const override;

protected:
  void       cacheParentProperties( const DeUTSector* parent );
  StatusCode initGeometryInfo();

private:
  parent_type* m_parent = nullptr;

  void       clear();
  void       determineSense();
  StatusCode cacheInfo();
  StatusCode registerConditionsCallbacks();

  Gaudi::Plane3D m_plane;
  Gaudi::Plane3D m_entryPlane;
  Gaudi::Plane3D m_exitPlane;

  Gaudi::XYZVector          m_direction;
  std::pair<double, double> m_range;

  unsigned int m_firstStrip = 1;
  unsigned int m_id         = 0u;
  float        m_pitch      = 0.0;
  unsigned int m_nStrip     = 0u;
  std::string  m_versionString;

  float m_thickness = 0.0;
  float m_uMinLocal = 0.0;
  float m_uMaxLocal = 0.0;
  float m_vMinLocal = 0.0;
  float m_vMaxLocal = 0.0;

  std::optional<LHCb::LineTraj<double>> m_midTraj;

  bool m_xInverted = false;
  bool m_yInverted = false;

  float m_deadWidth   = 0.0;
  float m_stripLength = 0.0;
};

inline bool DeUTSensor::contains( const LHCb::UTChannelID aChannel ) const {
  return ( aChannel.uniqueSector() == elementID().uniqueSector() );
}

inline unsigned int DeUTSensor::id() const { return m_id; }

inline void DeUTSensor::setID( unsigned int id ) { m_id = id; }

inline float DeUTSensor::pitch() const { return m_pitch; }

inline bool DeUTSensor::isStrip( const unsigned int strip ) const {
  return ( strip >= m_firstStrip && strip < m_firstStrip + m_nStrip );
}

inline unsigned int DeUTSensor::nStrip() const { return m_nStrip; }

inline float DeUTSensor::thickness() const { return m_thickness; }

inline float DeUTSensor::activeWidth() const { return fabs( m_uMinLocal - m_uMaxLocal ); }

inline float DeUTSensor::activeHeight() const { return fabs( m_vMinLocal - m_vMaxLocal ); }

inline double DeUTSensor::capacitance() const {
  // by chance the CMS and LHCb sensors have same capacitance
  static const double rho = 1.4 * Gaudi::Units::picofarad / Gaudi::Units::cm;
  return rho * activeHeight();
}

inline Gaudi::Plane3D DeUTSensor::plane() const { return m_plane; }

inline Gaudi::Plane3D DeUTSensor::entryPlane() const { return m_entryPlane; }

inline Gaudi::Plane3D DeUTSensor::exitPlane() const { return m_exitPlane; }

inline bool DeUTSensor::globalInActive( const Gaudi::XYZPoint& gpoint, Gaudi::XYZPoint tol ) const {
  Gaudi::XYZPoint lPoint = toLocal( gpoint );
  return localInActive( lPoint, tol );
}

inline bool DeUTSensor::globalInBondGap( const Gaudi::XYZPoint& gpoint, double tol ) const {
  Gaudi::XYZPoint lPoint = toLocal( gpoint );
  return localInBondGap( lPoint.Y(), tol );
}

inline bool DeUTSensor::localInBondGap( const double v, double tol ) const {

  return ( ( v + tol > m_uMinLocal ) && ( v - tol < m_uMaxLocal ) );
}

inline bool DeUTSensor::xInverted() const { return m_xInverted; }

inline bool DeUTSensor::yInverted() const { return m_yInverted; }

/** ouput operator for class DeUTSensor
 *  @see DeUTSensor
 *  @see MsgStream
 *  @param os      reference to STL output stream
 *  @param aSensor reference to DeUTSensor object
 */
inline std::ostream& operator<<( std::ostream& os, const DeUTSensor* aSensor ) { return aSensor->printOut( os ); }

/** ouput operator for class DeUTSensor
 *  @see DeUTSensor
 *  @see MsgStream
 *  @param os      reference to MsgStream output stream
 *  @param aSensor reference to DeUTSensor object
 */
inline MsgStream& operator<<( MsgStream& os, const DeUTSensor* aSensor ) { return aSensor->printOut( os ); }

#endif // _DEUTSENSOR_H
