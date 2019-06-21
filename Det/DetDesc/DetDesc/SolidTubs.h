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

// STD and STL includes
#include <cmath>
#include <iostream>
// Units
#include "GaudiKernel/SystemOfUnits.h"
// Geometry Definitions includes
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
// DetDesc
#include "DetDesc/SolidBase.h"
// forward declarations
template <class TYPE>
class SolidFactory;
class MsgStream;
//

/** @class SolidTubs SolidTubs.h DetDesc/SolidTubs.h
 *
 *  A simple implementation of TUBS - the tube segment
 *
 *  @author Vanya Belyaev  Ivan.Belyaev@itep.ru
 */

class SolidTubs final : public virtual SolidBase {
  /// frined factory for instantiation
  friend class SolidFactory<SolidTubs>;

public:
  /** constructor
   *  @param name           name of tube segment
   *  @param ZHalfLength    half-length of the tube segment
   *  @param OuterRadius    outer radius of tube segment
   *  @param InnerRadius    inner radius of tube segment
   *  @param StartPhiAngle  start phi angle
   *  @param DeltaPhiAngle  delta phi angle
   *  @param CoverModel     covering model
   *  @exception SolidException wrong parameter range
   */
  SolidTubs( const std::string& name,                                         //
             const double       ZHalfLength,                                  //
             const double       OuterRadius,                                  //
             const double       InnerRadius   = 0.0,                          //
             const double       StartPhiAngle = 0.0 * Gaudi::Units::degree,   //
             const double       DeltaPhiAngle = 360.0 * Gaudi::Units::degree, //
             const int          CoverModel    = 0 );

  /** - retrieve the tubs type
   *  - implementation of ISolid abstract interface
   *  @see ISolid
   *  return box type
   */
  inline std::string typeName() const override { return "SolidTubs"; }

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

  /** retrieve the pointer to "the most simplified cover"
   *    -# for Model = 0
   *      - the cover for general tube segment is tube
   *      - the cover for tube  is the cylinder
   *      - the cover for cylinder is the box
   *    -#  for Model = 1
   *      - the cover for general tube segment is teh cylinder segment
   *      - the cover for cylinder segment is cylinder
   *      - the cover for cylinder is the box
   *  implementation  of ISolid abstract interface
   *  @see ISolid
   *  @return pointer to the most simplified cover
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
   *  @param os Gaudi MsgStream  stream
   *  @return reference to the stream
   */
  MsgStream& printOut( MsgStream& os ) const override;

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

  ///@{
  /** accessors to the tube segment parameters */
  inline double innerRadius() const noexcept { return m_tubs_innerRadius; }
  inline double innerRadiusSq() const noexcept { return m_tubs_innerRadius_Sq; }
  inline double outerRadius() const noexcept { return m_tubs_outerRadius; }
  inline double outerRadiusSq() const noexcept { return m_tubs_outerRadius_Sq; }
  inline double zHalfLength() const noexcept { return m_tubs_zHalfLength; }
  inline double startPhiAngle() const noexcept { return m_tubs_startPhiAngle; }
  inline double deltaPhiAngle() const noexcept { return m_tubs_deltaPhiAngle; }
  inline double innerDiameter() const noexcept { return m_tubs_innerRadius * 2; }
  inline double outerDiameter() const noexcept { return m_tubs_outerRadius * 2; }
  inline double zLength() const noexcept { return m_tubs_zHalfLength * 2; }
  inline double endPhiAngle() const noexcept { return m_tubs_startPhiAngle + m_tubs_deltaPhiAngle; }
  ///@}

  /** Calculate the maximum number of ticks that a straight line could
      make with this solid
  *  @return maximum number of ticks
  */
  Ticks::size_type maxNumberOfTicks() const override { return 4; }

protected:
  /** check for R
   *  @param point to be checked
   *  @return true if point is "inside rho"
   */
  template <class aPoint>
  inline bool insideRho( const aPoint& point ) const;

  /// check if phi is in phi range
  inline bool insidePhi( const double phi /* [-pi,pi] */ ) const;

  /** check for phi
   *  @param point to be checked
   *  @return true if point is "inside phi"
   */
  template <class aPoint>
  inline bool insidePhi( const aPoint& point ) const;

  /// gap in phi ?
  inline bool noPhiGap() const { return m_noPhiGap; }

protected:
  /** constructor
   *  @param name name of tube segment
   */
  SolidTubs( const std::string& name = "Anonymous Tubs" );

  /** set bounding parameters dor base class SolidBox
   */
  void setBP();

private:
  SolidTubs( const SolidTubs& );            ///< no copy-constructor
  SolidTubs& operator=( const SolidTubs& ); ///< no assignment

  /**
   * implementation of isInside
   * @param  point reference to any kind of point with x(), y(), z()
   * @return bool
   */
  template <class aPoint>
  inline bool isInsideImpl( const aPoint& point ) const {
    return ( !isOutBBox( point ) && // check Z
             insideRho( point ) &&  // check for rho
             insidePhi( point ) );  //  check for phi
  }

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

  /// fills m_cover member
  void createCover();

private:
  // parameters of tube segment
  double m_tubs_zHalfLength{1e7};
  double m_tubs_outerRadius{1e7};
  double m_tubs_outerRadius_Sq{1e14};
  double m_tubs_innerRadius{0};
  double m_tubs_innerRadius_Sq{0};
  double m_tubs_startPhiAngle{0};
  double m_tubs_deltaPhiAngle{360 * Gaudi::Units::degree};
  // model for constructing the cover solid
  int m_tubs_coverModel{0};
  //
  bool m_noPhiGap{true};
  // cover
  std::unique_ptr<ISolid> m_cover;
};

// ===========================================================================
/** check for R
 *  @param point point to be checked
 *  @return true if point is "inside rho"
 */
// ===========================================================================
template <class aPoint>
inline bool SolidTubs::insideRho( const aPoint& point ) const {
  const auto rho2 = point.perp2();
  return ( !( ( rho2 > outerRadiusSq() ) || //
              ( 0 < innerRadius() && rho2 < innerRadiusSq() ) ) );
}
// ===========================================================================

// ===========================================================================
/** check for phi
 *  @param phi point to be checked [-pi,pi]
 *  @return true if point is "inside phi"
 */
// ===========================================================================
inline bool SolidTubs::insidePhi( const double phi /* [-pi,pi] */ ) const {
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
inline bool SolidTubs::insidePhi( const aPoint& point ) const {
  return noPhiGap() || insidePhi( point.phi() );
}
// ===========================================================================
// The END
// ===========================================================================
