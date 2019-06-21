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
#pragma once
/// STD and STL
#include <array>
#include <cmath>
#include <iostream>
/// Geometry Definitions
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
/// DetDesc
#include "DetDesc/SolidBase.h"
/// forward declarations
template <class TYPE>
class SolidFactory; ///< DetDesc

/** @class SolidBox SolidBox.h "DetDesc/SolidBox.h"
 *
 *  A simple implementation of BOX
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date xx/xx/xxx
 */

class SolidBox final : public virtual SolidBase {
  /// friend factory
  friend class SolidFactory<SolidBox>;

public:
  /** standard(public)  constructor
   *  @param      Name        name of this box
   *  @param      xHalf half-size of the box in x-direction
   *  @param      yHalf half-size of the box in y-direction
   *  @param      zHalf half-size of the box in z-direction
   *  @exception  SolidException wrong parameters range
   */
  SolidBox( const std::string& Name, const double xHalf, //
            const double yHalf, const double zHalf );

  SolidBox( const SolidBox& ) = delete;            ///< no copy-constructor
  SolidBox& operator=( const SolidBox& ) = delete; ///< no assignment

  /** - retrieve the box type
   *  - implementation of ISolid abstract interface
   *  @see ISolid
   *  return box type
   */
  inline std::string typeName() const override { return "SolidBox"; }

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
  /** - retrieve the pointer to "simplified" solid - "cover"
   *  - implementation of ISolid abstract interface
   *  The Box is the most simple shape
   *  @see ISolid
   *  @return pointer to "simplified" solid - "cover"
   */
  const ISolid* cover() const override;

  /** - retrieve the pointer to "the most simplified cover"
   *  - implementation  of ISolid abstract interface
   *  - reimplementation of SolidBase::coverTop()
   *  The Box is the most simple shape
   *  @see ISolid
   *  @see SolidBase
   *  @return pointer to the most simplified cover
   */
  const ISolid* coverTop() const override;

  /** - printout to STD/STL stream
   *  - implementation  of ISolid abstract interface
   *  - reimplementation of SolidBase::printOut( std::ostream& )
   *  @see SolidBase
   *  @see ISolid
   *  @param os STD/STL stream
   *  @return reference to the stream
   */
  std::ostream& printOut( std::ostream& os ) const override;

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

  /**  return the full x-size of the box
   *  @return the full x-size of the box
   */
  inline double xsize() const noexcept { return m_box_xHalfLength * 2; }

  /**  return the full y-size of the box
   *  @return the full y-size of the box
   */
  inline double ysize() const noexcept { return m_box_yHalfLength * 2; }

  /**  return the full z-size of the box
   *  @return the full z-size of the box
   */
  inline double zsize() const noexcept { return m_box_zHalfLength * 2; }

  /**  return the half x-size of the box
   *  @return the half x-size of the box
   */
  inline double xHalfLength() const noexcept { return m_box_xHalfLength; }

  /**  return the half y-size of the box
   *  @return the half y-size of the box
   */
  inline double yHalfLength() const noexcept { return m_box_yHalfLength; }

  /**  return the half z-size of the box
   *  @return the half z-size of the box
   */
  inline double zHalfLength() const noexcept { return m_box_zHalfLength; }

  /** Calculate the maximum number of ticks that a straight line could
      make with this solid
  *  @return maximum number of ticks
  */
  Ticks::size_type maxNumberOfTicks() const override { return 2; }

protected:
  /** set parameters for bounding solids (box, sphere and cylinder)
   */
  void setBP();

private:
  /**
   * implementation of isInside
   * @param  point reference to any kind of point with x(), y(), z()
   * @return bool
   */
  template <class aPoint>
  inline bool isInsideImpl( const aPoint& point ) const {
    return ( fabs( point.z() ) < zHalfLength() && //
             fabs( point.y() ) < yHalfLength() && //
             fabs( point.x() ) < xHalfLength() );
  }

  template <class aPoint, class aVector>
  inline unsigned int intersectionTicksImpl( const aPoint&       Point,   //
                                             const aVector&      Vector,  //
                                             const ISolid::Tick& tickMin, //
                                             const ISolid::Tick& tickMax, //
                                             ISolid::Ticks&      ticks ) const {
    unsigned int rc = 0;
    if ( !isOutBBox( Point, Vector, tickMin, tickMax ) ) {
      rc = SolidBox::intersectionTicksImpl( Point, Vector, ticks );
      // we can optimize this because in a box there are at 0 or 2 ticks.
      if ( rc != 0 ) {
        // assert( rc==2 ) ;
        auto& first  = ticks[0];
        auto& second = ticks[1];
        if ( first > tickMax || second < tickMin ) {
          ticks.clear();
          rc = 0;
        } else {
          if ( first < tickMin ) { first = tickMin; }
          if ( second > tickMax ) { second = tickMax; }
        }
      }
    }
    return rc;
  }

  template <class aPoint, class aVector>
  unsigned int intersectionTicksImpl( const aPoint&  Point,  //
                                      const aVector& Vector, //
                                      ISolid::Ticks& ticks ) const;

private:
  double m_box_xHalfLength{0}; ///< x/2
  double m_box_yHalfLength{0}; ///< y/2
  double m_box_zHalfLength{0}; ///< z/2
};

/// ===========================================================================
