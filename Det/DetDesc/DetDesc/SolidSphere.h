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
// ===========================================================================
#pragma once

#include <memory>
// DetDesc
#include "DetDesc/SolidBase.h"
// Units
#include "GaudiKernel/SystemOfUnits.h"

template <class TYPE>
class SolidFactory;

/** @class SolidSphere SolidSphere.h "DetDesc/SolidSphere.h"
 *
 *  A simple implementation of SPHERE
 *
 *  @author Vanya Belyaev
 */

class SolidSphere final : public virtual SolidBase {
  /// friend factory fo instantiation
  friend class SolidFactory<SolidSphere>;

public:
  /** constructor
   *  @param name             name of sphere segment
   *  @param OuterRadius      outer radius of sphere segement
   *  @param InsideRadius     inner  radius of sphere segement
   *  @param StartPhiAngle    start phi angle
   *  @param DeltaPhiAngle    delta phi angle
   *  @param StartThetaAngle  start theta angle
   *  @param DeltaThetaAngle  delta theta angle
   *  @param CoverModel       covering model
   */
  SolidSphere( const std::string& name,                                           //
               const double       OuterRadius,                                    //
               const double       InsideRadius    = 0.0,                          //
               const double       StartPhiAngle   = 0.0 * Gaudi::Units::degree,   //
               const double       DeltaPhiAngle   = 360.0 * Gaudi::Units::degree, //
               const double       StartThetaAngle = 0.0 * Gaudi::Units::degree,   //
               const double       DeltaThetaAngle = 180.0 * Gaudi::Units::degree, //
               const int          CoverModel      = 0 );

  /** - retrieve the specific type of the solid
   *  - implementation of ISolid abstract interface
   *  @see ISolid
   *  @return specific type of the solid
   */
  inline std::string typeName() const override { return "SolidSphere"; }

  /** - check for the given 3D-point.
   *    Point coordinated are in the local reference
   *    frame of the solid.
   *  - implementation of ISolid absstract interface
   *  @see ISolid
   *  @param point point (in local reference system of the solid)
   *  @return true if the point is inside the solid
   */
  bool isInside( const Gaudi::XYZPoint& point ) const override;
  bool isInside( const Gaudi::Polar3DPoint& point ) const override;
  bool isInside( const Gaudi::RhoZPhiPoint& point ) const override;

  /** -# retrieve the pointer to "simplified" solid - "cover"
   *  -# implementation of ISolid abstract interface
   *    The simplification scheme:
   *  - for Model == 0
   *     -# The cover for the general sphere segment is the clove
   *         (remove gap in theta)
   *     -# The cover for the clove is the sphere
   *         (remove gap in phi)
   *     -# The cover for the sphere is the ball
   *         (set the inner radius to zero)
   *     -# The cover for the ball is the box
   *  - alternative model
   *     -# The cover for general sphere segment is
   *          the segment with inner radius equal to zero
   *     -# The cover for the general sphere segment with
   *        inner radius equal to zero is the
   *        the sphere segemnt with no phi gap
   *     -# The cover for the general sphere segment with
   *         inner radius equal to zero and nophi gap
   *         is the ball
   *     -# the cover for the ball is the box
   *  @see ISolid
   *  @see SolidSphere::m_sphere_coverModel
   *  @return pointer to "simplified" solid - "cover"
   */
  inline const ISolid* cover() const override { return m_cover.get(); }

  /** - printout to STD/STL stream
   *  - implementation of ISolid abstract interface
   *  - reimplementation of SolidBase::printOut( std::ostream& )
   *  @see ISolid
   *  @see SolidBase
   *  @param os STD/STL stream
   *  @return reference to the stream
   */
  std::ostream& printOut( std::ostream& os = std::cout ) const override;

  /** - printout to Gaudi  stream
   *  - implementation of ISolid abstract interface
   *  - reimplementation of SolidBase::printOut( MsgStream& )
   *  @see ISolid
   *  @see SolidBase
   *  @param os Gaudi stream
   *  @return reference to the stream
   */
  MsgStream& printOut( MsgStream& os ) const override;

  /** -# calculate the intersection points("ticks") of the solid objects
   *    with given line.
   *  - Line is parametrized with parameter \a t :
   *     \f$ \vec{x}(t) = \vec{p} + t \times \vec{v} \f$
   *      - \f$ \vec{p} \f$ is a point on the line
   *      - \f$ \vec{v} \f$ is a vector along the line
   *  - \a tick is just a value of parameter \a t, at which the
   *    intersection of the solid and the line occurs
   *  - both  \a Point  (\f$\vec{p}\f$) and \a Vector
   *    (\f$\vec{v}\f$) are defined in local reference system
   *   of the solid
   *  -# implementation of ISolid abstract interface
   *  @see ISolid
   *  @param Point initial point for the line
   *  @param Vector vector along the line
   *  @param ticks output container of "Ticks"
   *  @return the number of intersection points
   */

  using SolidBase::intersectionTicks;
  unsigned int intersectionTicks( const Gaudi::XYZPoint&  Point,  //
                                  const Gaudi::XYZVector& Vector, //
                                  ISolid::Ticks&          ticks ) const override;

  unsigned int intersectionTicks( const Gaudi::Polar3DPoint&  Point,  //
                                  const Gaudi::Polar3DVector& Vector, //
                                  ISolid::Ticks&              ticks ) const override;

  unsigned int intersectionTicks( const Gaudi::RhoZPhiPoint&  Point,  //
                                  const Gaudi::RhoZPhiVector& Vector, //
                                  ISolid::Ticks&              ticks ) const override;

  /**  return the inner radius of sphere segment
   *  @return the inner radius of sphere segment
   */
  inline double insideRadius() const noexcept { return m_sphere_insideR; }

  /**  return the squared inner radius of sphere segment
   *  @return the squared inner radius of sphere segment
   */
  inline double insideR2() const noexcept { return m_sphere_insideR2; }

  /**  return the outer radius of sphere segment
   *  @return the outer radius of sphere segment
   */
  inline double outerRadius() const noexcept { return m_sphere_outerR; }

  /**  return the squared outer radius of sphere segment
   *  @return the squared outer radius of sphere segment
   */
  inline double outerR2() const noexcept { return m_sphere_outerR2; }

  /**  return the start of phi angle of sphere segment
   *  @return the start of phi angle of sphere segment
   */
  inline double startPhiAngle() const noexcept { return m_sphere_startPhiAngle; }

  /**  return the delta of phi angle of sphere segment
   *  @return the delta of phi angle of sphere segment
   */
  inline double deltaPhiAngle() const noexcept { return m_sphere_deltaPhiAngle; }

  /**  return the start of theta angle of sphere segment
   *  @return the start of theta angle of sphere segment
   */
  inline double startThetaAngle() const noexcept { return m_sphere_startThetaAngle; }

  /**  return the delta of theta angle of sphere segment
   *  @return the delta of theta angle of sphere segment
   */
  inline double deltaThetaAngle() const noexcept { return m_sphere_deltaThetaAngle; }

  /**  return the inner diameter of sphere segment
   *  @return the inner diameter of sphere segment
   */
  inline double insideDiameter() const noexcept { return insideRadius() * 2; }

  /**  return the outer diameter of sphere segment
   *  @return the outer diameter of sphere segment
   */
  inline double outerDiameter() const noexcept { return outerRadius() * 2; }

  /**  return the end of phi angle of sphere segment
   *  @return the end of phi angle of sphere segment
   */
  inline double endPhiAngle() const noexcept { return m_sphere_startPhiAngle + m_sphere_deltaPhiAngle; }

  /**  return the end of theta angle of sphere segment
   *  @return the end of theta angle of sphere segment
   */
  inline double endThetaAngle() const noexcept { return m_sphere_startThetaAngle + m_sphere_deltaThetaAngle; }

  /** Calculate the maximum number of ticks that a straight line could
      make with this solid
  *  @return maximum number of ticks
  */
  Ticks::size_type maxNumberOfTicks() const override { return 4; }

protected:
  /// gap in phi   ?
  bool noPhiGap() const { return m_noPhiGap; }
  /// gap in theta ?
  bool noThetaGap() const { return m_noThetaGap; }

  /** check for R
   *  @param point to be checked
   *  @return true if point is "inside rho"
   */
  template <class aPoint>
  inline bool insideR( const aPoint& point ) const;

  /** check for phi
   *  @param point to be checked
   *  @return true if point is "inside phi"
   */
  template <class aPoint>
  inline bool insidePhi( const aPoint& point ) const;

  /** check for theta
   *  @param point to be checked
   *  @return true if point is "inside theta"
   */
  template <class aPoint>
  inline bool insideTheta( const aPoint& point ) const;

protected:
  /** set bounding parameters
   */
  void setBP();

private:
  //
  SolidSphere( const SolidSphere& );            // no copy-constructor
  SolidSphere& operator=( const SolidSphere& ); // no assignment

  /**
   * implementation of isInside
   * @param  point reference to any kind of point with x(), y(), z()
   * @return bool
   */
  template <class aPoint>
  inline bool isInsideImpl( const aPoint& point ) const {
    return ( !isOutBBox( point ) &&  // check bounding box
             insideR( point ) &&     // check for radius
             insideTheta( point ) && // check for theta
             insidePhi( point ) );   // check for phi
  }

  template <class aPoint, class aVector>
  unsigned int intersectionTicksImpl( const aPoint&  Point,  //
                                      const aVector& Vector, //
                                      ISolid::Ticks& ticks ) const;

  void createCover();
  //
private:
  /// sphere segment parameters
  double m_sphere_outerR2{0};
  double m_sphere_insideR2{0};
  double m_sphere_startPhiAngle{0};
  double m_sphere_deltaPhiAngle{360.0 * Gaudi::Units::degree};
  double m_sphere_startThetaAngle{0};
  double m_sphere_deltaThetaAngle{180.0 * Gaudi::Units::degree};
  /// cover model
  int m_sphere_coverModel{0};
  //
  double m_sphere_outerR{0};
  double m_sphere_insideR{0};
  //
  bool m_noPhiGap{true};
  bool m_noThetaGap{true};
  //
  std::unique_ptr<ISolid> m_cover;
};
// ===========================================================================

// ===========================================================================
/** check for R
 *  @param point to be checked
 *  @return true if point is "inside rho"
 */
// ===========================================================================
template <class aPoint>
inline bool SolidSphere::insideR( const aPoint& point ) const {
  const auto r2 = point.mag2();
  return ( r2 <= outerR2() && r2 >= insideR2() );
}
// ===========================================================================

// ===========================================================================
/** check for phi
 *  @param point to be checked
 *  @return true if point is "inside phi"
 */
// ===========================================================================
template <class aPoint>
inline bool SolidSphere::insidePhi( const aPoint& point ) const {
  bool rc = true;
  if ( UNLIKELY( !noPhiGap() ) ) {
    auto phi = point.phi(); // [-180,180]
    rc       = ( startPhiAngle() <= phi && startPhiAngle() + deltaPhiAngle() >= phi );
    if ( !rc ) {
      phi += 360 * Gaudi::Units::degree;
      rc = ( startPhiAngle() <= phi && startPhiAngle() + deltaPhiAngle() >= phi );
    }
  }
  return rc;
}
// ===========================================================================

// ===========================================================================
/** check for theta
 *  @param point to be checked
 *  @return true if point is "inside theta"
 */
// ===========================================================================
template <class aPoint>
inline bool SolidSphere::insideTheta( const aPoint& point ) const {
  bool rc = true;
  if ( UNLIKELY( !noThetaGap() ) ) {
    const auto theta = point.theta();
    rc               = ( startThetaAngle() <= theta && startThetaAngle() + deltaThetaAngle() >= theta );
  }
  return rc;
}
// ===========================================================================

// ===========================================================================
// The END
// ===========================================================================
