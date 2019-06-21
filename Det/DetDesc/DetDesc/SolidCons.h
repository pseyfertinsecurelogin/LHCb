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
// STD and STL
#include <cmath>
#include <iostream>
#include <memory>
// Units
#include "GaudiKernel/SystemOfUnits.h"
// Geometry Definitions
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
// DetDesc
#include "DetDesc/SolidBase.h"
// forward declarations
template <class TYPE>
class SolidFactory;

/** @class SolidCons SolidCons.h DetDesc/SolidCons.h
 *
 *  A simple implementation segment of conical tube.
 *  The shape corresponds to Geant4 shape G4Cons
 *  and to Geant3 shape CONS
 *
 *  @author Vanya Belyaev
 */

class SolidCons final : public virtual SolidBase {
  /// friend factory for instantiation
  friend class SolidFactory<SolidCons>;

public:
  /** standard constructor
   *  @param name                  name of conical tube segment
   *  @param ZHalfLength           half-length
   *  @param OuterRadiusAtMinusZ   outer radius at minus Z
   *  @param OuterRadiusAtPlusZ    outer radius at plus  Z
   *  @param InnerRadiusAtMinusZ   inner radius at minus Z
   *  @param InnerRadiusAtPlusZ    inner radius at plus  Z
   *  @param StartPhiAngle         start phi angle
   *  @param DeltaPhiAngle         delta phi angle
   *  @param CoverModel            covering model
   *  @exception SolidException    wrong parameters range
   */
  SolidCons( const std::string& name,                                               //
             const double       ZHalfLength,                                        //
             const double       OuterRadiusAtMinusZ,                                //
             const double       OuterRadiusAtPlusZ,                                 //
             const double       InnerRadiusAtMinusZ = 0.0,                          //
             const double       InnerRadiusAtPlusZ  = 0.0,                          //
             const double       StartPhiAngle       = 0.0 * Gaudi::Units::degree,   //
             const double       DeltaPhiAngle       = 360.0 * Gaudi::Units::degree, //
             const int          CoverModel          = 0 );

  /** - retrieve the conical tube segment  type
   *  - implementation of ISolid abstract interface
   *  @see ISolid
   *  return box type
   */
  inline std::string typeName() const override { return "SolidCons"; }

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
  /** -\# retrieve the pointer to "simplified" solid - "cover"
   *    - for Model = 0
   *        -\# the cover for the general conical tube segment is
   *            the conical tube
   *        -\# the cover for the conical tube is conical cylinder
   *        -\# the cover for the conical cylinder is TRD
   *    - for Model != 0
   *        -\# the cover for conical tube serment is conical cylinder segment
   *        -\# the cover for conical cylinder segment is conical cylinder
   *        -\# the cover for conical cylinder is TRD
   *  -\# implementation of ISolid abstract interface
   *  @see ISolid
   *  @return pointer to "simplified" solid - "cover"
   */
  inline const ISolid* cover() const override { return m_cover.get(); }

  /** - printout to STD/STL stream
   *  - implementation  of ISolid abstract interface
   *  - reimplementation of SolidBase::printOut( std::ostream& )
   *  @see SolidBase
   *  @see ISolid
   *  @param os STD/STL stream
   *  @return reference to the stream
   */
  std::ostream& printOut( std::ostream& os = std::cout ) const override;

  /** - printout to Gaudi MsgStream stream
   *  - implementation  of ISolid abstract interface
   *  - reimplementation of SolidBase::printOut( MsgStream& )
   *  @see SolidBase
   *  @see ISolid
   *  @param "" STD/STL stream
   *  @return reference to the stream
   */
  MsgStream& printOut( MsgStream& ) const override;

  /** Tests whether or not the given line (defined as a point and
   *  a direction) intersects or not with the given solid.
   *  - Line is parametrized with parameter \a t :
   *     \f$ \vec{x}(t) = \vec{p} + t \times \vec{v} \f$
   *      - \f$ \vec{p} \f$ is a point on the line
   *      - \f$ \vec{v} \f$ is a vector along the line
   *  @param Point initial point for the line
   *  @param Vector vector along the line
   *  @return the number of intersection points
   */
  bool testForIntersection( const Gaudi::XYZPoint& Point, const Gaudi::XYZVector& Vector ) const override;
  bool testForIntersection( const Gaudi::Polar3DPoint& Point, const Gaudi::Polar3DVector& Vector ) const override;
  bool testForIntersection( const Gaudi::RhoZPhiPoint& Point, const Gaudi::RhoZPhiVector& Vector ) const override;

  /** - calculate the intersection points("ticks") of the solid objects
   *    with given line.
   *  -# Line is parametrized with parameter \a t :
   *     \f$ \vec{x}(t) = \vec{p} + t \times \vec{v} \f$
   *      - \f$ \vec{p} \f$ is a point on the line
   *      - \f$ \vec{v} \f$ is a vector along the line
   *  -# \a tick is just a value of parameter \a t, at which the
   *    intersection of the solid and the line occurs
   *  -# both  \a Point  (\f$\vec{p}\f$) and \a Vector
   *    (\f$\vec{v}\f$) are defined in local reference system
   *   of the solid
   *  - implementation of ISolid abstract interface
   *  @see ISolid
   *  @param Point initial point for the line
   *  @param Vector vector along the line
   *  @param ticks output container of "Ticks"
   *  @return the number of intersection points
   */

  unsigned int intersectionTicks( const Gaudi::XYZPoint&  Point,  //
                                  const Gaudi::XYZVector& Vector, //
                                  ISolid::Ticks&          ticks ) const override;

  unsigned int intersectionTicks( const Gaudi::Polar3DPoint&  Point,  //
                                  const Gaudi::Polar3DVector& Vector, //
                                  ISolid::Ticks&              ticks ) const override;

  unsigned int intersectionTicks( const Gaudi::RhoZPhiPoint&  Point,  //
                                  const Gaudi::RhoZPhiVector& Vector, //
                                  ISolid::Ticks&              ticks ) const override;

  /** calculate the intersection points("ticks") of the solid objects
   *  with given line.
   *  - Line is parametrized with parameter \a t :
   *     \f$ \vec{x}(t) = \vec{p} + t \times \vec{v} \f$
   *      - \f$ \vec{p} \f$ is a point on the line
   *      - \f$ \vec{v} \f$ is a vector along the line
   *  - \a tick is just a value of parameter \a t, at which the
   *    intersection of the solid and the line occurs
   *  - both  \a Point  (\f$\vec{p}\f$) and \a Vector
   *    (\f$\vec{v}\f$) are defined in local reference system
   *   of the solid
   *  Only intersection ticks within the range
   *   \a tickMin and \a tickMax are taken into account.
   *  @see ISolid::intersectionTicks()
   *  @param Point initial point for the line
   *  @param Vector vector along the line
   *  @param tickMin minimum value of Tick
   *  @param tickMax maximu value of Tick
   *  @param ticks output container of "Ticks"
   *  @return the number of intersection points
   */

  unsigned int intersectionTicks( const Gaudi::XYZPoint&  Point,   //
                                  const Gaudi::XYZVector& Vector,  //
                                  const ISolid::Tick&     tickMin, //
                                  const ISolid::Tick&     tickMax, //
                                  ISolid::Ticks&          ticks ) const override;

  unsigned int intersectionTicks( const Gaudi::Polar3DPoint&  Point,   //
                                  const Gaudi::Polar3DVector& Vector,  //
                                  const ISolid::Tick&         tickMin, //
                                  const ISolid::Tick&         tickMax, //
                                  ISolid::Ticks&              ticks ) const override;

  unsigned int intersectionTicks( const Gaudi::RhoZPhiPoint&  Point,   //
                                  const Gaudi::RhoZPhiVector& Vector,  //
                                  const ISolid::Tick&         tickMin, //
                                  const ISolid::Tick&         tickMax, //
                                  ISolid::Ticks&              ticks ) const override;

  /** inner radius squared at minus Z
   *  @return inner radius squared at minus Z
   */
  inline double innerRadiusSqrAtMinusZ() const noexcept { return m_cons_innerRadiusMinusZSq; }

  /** outer radius squared at minus Z
   *  @return outer radius squared at minus Z
   */
  inline double outerRadiusSqrAtMinusZ() const noexcept { return m_cons_outerRadiusMinusZSq; }

  /** inner radius at minus Z
   *  @return inner radius at minus Z
   */
  inline double innerRadiusAtMinusZ() const noexcept { return m_cons_innerRadiusMinusZ; }

  /** outer radius at minus Z
   *  @return outer radius at minus Z
   */
  inline double outerRadiusAtMinusZ() const noexcept { return m_cons_outerRadiusMinusZ; }

  /** inner radius squared at plus Z
   *  @return inner radius squared at plus Z
   */
  inline double innerRadiusSqrAtPlusZ() const noexcept { return m_cons_innerRadiusPlusZSq; }

  /** outer radius squared at plus Z
   *  @return outer radius squared at plus Z
   */
  inline double outerRadiusSqrAtPlusZ() const noexcept { return m_cons_outerRadiusPlusZSq; }

  /** inner radius at plus Z
   *  @return inner radius at plus Z
   */
  inline double innerRadiusAtPlusZ() const noexcept { return m_cons_innerRadiusPlusZ; }

  /** outer radius at plus Z
   *  @return outer radius at plus Z
   */
  inline double outerRadiusAtPlusZ() const noexcept { return m_cons_outerRadiusPlusZ; }

  /** half length
   *  @return half length
   */
  inline double zHalfLength() const noexcept { return m_cons_zHalfLength; }

  /** start phi angle
   *  @return  start phi angle
   */
  inline double startPhiAngle() const noexcept { return m_cons_startPhiAngle; }

  /** delta phi
   *  @return delta phi
   */
  inline double deltaPhiAngle() const noexcept { return m_cons_deltaPhiAngle; }

  /** inner diameter at minus Z
   *  @return inner diameter at minus Z
   */
  inline double innerDiameterAtMinusZ() const noexcept { return m_cons_innerRadiusMinusZ * 2; }

  /** outer radius at minus Z
   *  @return outer radius at minus Z
   */
  inline double outerDiameterAtMinusZ() const noexcept { return m_cons_outerRadiusMinusZ * 2; }

  /**inner radius at plus Z
   * @return inner radius at plus Z
   */
  inline double innerDiameterAtPlusZ() const noexcept { return m_cons_innerRadiusPlusZ * 2; }

  /** outer radius at plus Z
   *  @return outer radius at plus Z
   */
  inline double outerDiameterAtPlusZ() const noexcept { return m_cons_outerRadiusPlusZ * 2; }

  /** full length
   *  @return full length
   */
  inline double zLength() const noexcept { return m_cons_zHalfLength * 2; }

  /** end phi angle
   *  @return end phi angle
   */
  inline double endPhiAngle() const noexcept { return m_cons_startPhiAngle + m_cons_deltaPhiAngle; }

protected:
  /** inner radius at given z
   *  @param z z position
   *  @return inner ragius at given z
   */
  inline double iR_z( const double z ) const;

  /** outer radius at given z
   *  @param z z position
   *  @return outer ragius at given z
   */
  inline double oR_z( const double z ) const;

  /// gap in phi ?
  bool noPhiGap() const noexcept { return m_noPhiGap; }

  /// check if phi is in phi range
  inline bool insidePhi( const double phi /* [-pi,pi] */ ) const;

  /** check for phi
   *  @param point to be checked
   *  @return true if point is "inside phi"
   */
  template <class aPoint>
  inline bool insidePhi( const aPoint& point ) const;

  /** Calculate the maximum number of ticks that a straight line could
      make with this solid
  *  @return maximum number of ticks
  */
  Ticks::size_type maxNumberOfTicks() const noexcept override { return 4; }

protected:
  /** set bounding parameters
   */
  void setBP();

private:
  SolidCons( const SolidCons& );            ///< no copy-constructor
  SolidCons& operator=( const SolidCons& ); ///< no assignment

  /**
   * implementation of isInside
   * @param  point reference to any kind of point with x(), y(), z()
   * @return bool
   */
  template <class aPoint>
  inline bool isInsideImpl( const aPoint& point ) const {
    if ( isOutBBox( point ) ) { return false; }
    // check for phi
    if ( !insidePhi( point ) ) { return false; }
    // check for radius
    const auto rho2 = point.perp2();
    const auto oR   = oR_z( point.z() );
    if ( rho2 > oR * oR ) { return false; }
    const auto iR = iR_z( point.z() );
    if ( rho2 < iR * iR ) { return false; }
    //
    return true;
  }

  template <class aPoint, class aVector>
  bool testForIntersectionImpl( const aPoint& Point, const aVector& Vector ) const;

  template <class aPoint, class aVector>
  unsigned int intersectionTicksImpl( const aPoint&       Point,   //
                                      const aVector&      Vector,  //
                                      const ISolid::Tick& tickMin, //
                                      const ISolid::Tick& tickMax, //
                                      ISolid::Ticks&      ticks ) const;

  template <class aPoint, class aVector>
  unsigned int intersectionTicksImpl( const aPoint&  Point,  //
                                      const aVector& Vector, //
                                      ISolid::Ticks& ticks ) const;

  void createCover();

private:
  double m_cons_zHalfLength{0};
  double m_cons_outerRadiusMinusZ{0};
  double m_cons_outerRadiusPlusZ{0};
  double m_cons_innerRadiusMinusZ{0};
  double m_cons_innerRadiusPlusZ{0};
  double m_cons_startPhiAngle{0};
  double m_cons_deltaPhiAngle{0};
  ///
  int  m_cons_coverModel{0};
  bool m_noPhiGap{0};
  // cache squares
  double m_cons_outerRadiusMinusZSq{0};
  double m_cons_outerRadiusPlusZSq{0};
  double m_cons_innerRadiusMinusZSq{0};
  double m_cons_innerRadiusPlusZSq{0};

  std::unique_ptr<ISolid> m_cover;
};
// ===========================================================================

// ===========================================================================
/** inner radius at given z
 *  @param z z position
 *  @return inner ragius at given z
 */
// ===========================================================================
inline double SolidCons::iR_z( const double z ) const {
  const double a = ( innerRadiusAtPlusZ() - innerRadiusAtMinusZ() ) / zHalfLength();
  const double b = ( innerRadiusAtPlusZ() + innerRadiusAtMinusZ() );
  return 0.5 * ( a * z + b );
}
// ===========================================================================

// ===========================================================================
/** outer radius at given z
 *  @param z z position
 *  @return outer ragius at given z
 */
// ===========================================================================
inline double SolidCons::oR_z( const double z ) const {
  const double a = ( outerRadiusAtPlusZ() - outerRadiusAtMinusZ() ) / zHalfLength();
  const double b = ( outerRadiusAtPlusZ() + outerRadiusAtMinusZ() );
  return 0.5 * ( a * z + b );
}
// ===========================================================================

// ===========================================================================
/** check for phi
 *  @param phi point to be checked [-pi,pi]
 *  @return true if point is "inside phi"
 */
// ===========================================================================
inline bool SolidCons::insidePhi( const double phi /* [-pi,pi] */ ) const {
  return ( noPhiGap() ||                                                             //
           ( startPhiAngle() <= phi && startPhiAngle() + deltaPhiAngle() >= phi ) || //
           ( startPhiAngle() <= phi + 2 * M_PI && startPhiAngle() + deltaPhiAngle() >= phi + 2 * M_PI ) );
}

// ===========================================================================
/** check for phi
 *  @param point point to be checked
 *  @return true if point is "inside phi"
 */
// ===========================================================================
template <class aPoint>
inline bool SolidCons::insidePhi( const aPoint& point ) const {
  return noPhiGap() || insidePhi( point.phi() );
}
// ===========================================================================
