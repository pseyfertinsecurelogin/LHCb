/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// ============================================================================
#ifndef DETDESC_SOLIDPOLYCONE_H
#define DETDESC_SOLIDPOLYCONE_H 1
// Include files
// STD and STL
#include <cmath>
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>
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

/** @class SolidPolycone SolidPolycone.h DetDesc/SolidPolycone.h
 *
 *  A simple implementation of "Polycone" shape
 *  "PCON" in Geant3 (G4Polycone in Geant4)
 *
 *
 *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
 *  @date   23/10/2001
 */

class SolidPolycone final : public virtual SolidBase
{
  /// friend factory for instantiation
  friend class SolidFactory<SolidPolycone>;

public:

  typedef std::pair<double,double>  Pair     ;
  typedef std::pair<double,Pair>    Triplet  ;
  typedef std::vector<Triplet>      Triplets ;
  typedef Triplets::const_iterator  Iterator ;

  /** Standard constructor
   *
   *  @param Name          the name of the polycone
   *  @param Params        vector of "triplets" (z,(rMax,rMin))
   *  @param StartPhiAngle the azimuthal angle phi at which polycone "begins"
   *  @param DeltaPhiAngle the opening angle
   */
  SolidPolycone( const std::string&  Name                       ,
                 const Triplets   &  Params                     ,
                 const double        StartPhiAngle = 0          ,
                 const double        DeltaPhiAngle = 360*Gaudi::Units::degree );

  /** - retrieve the polycone  type
   *  - implementation of ISolid abstract interface
   *  @see ISolid
   *  return box type
   */
  inline std::string typeName () const override { return "SolidPolycone"; }

  /** - check for the given 3D-point.
   *    Point coordinates are in the local reference
   *    frame of the solid.
   *  - implementation of ISolid absstract interface
   *  @see ISolid
   *  @param point point (in local reference system of the solid)
   *  @return true if the point is inside the solid
   */
  bool isInside (  const Gaudi::XYZPoint& point ) const override;
  bool isInside ( const Gaudi::Polar3DPoint& point ) const override;
  bool isInside ( const Gaudi::RhoZPhiPoint& point ) const override;

  /** -# retrieve the pointer to "simplified" solid - "cover"
   *  -# implementation of ISolid abstract interface
   *  @see ISolid
   *  @return pointer to "simplified" solid - "cover"
   */
  inline const ISolid* cover () const override { return m_cover.get(); }

  /** - printout to STD/STL stream
   *  - implementation  of ISolid abstract interface
   *  - reimplementation of SolidBase::printOut( std::ostream& )
   *  @see SolidBase
   *  @see ISolid
   *  @param os STD/STL stream
   *  @return reference to the stream
   */
  std::ostream&  printOut ( std::ostream& os = std::cout )    const override;

  /** - printout to Gaudi MsgStream stream
   *  - implementation  of ISolid abstract interface
   *  - reimplementation of SolidBase::printOut( MsgStream& )
   *  @see SolidBase
   *  @see ISolid
   *  @param "" STD/STL stream
   *  @return reference to the stream
   */
  MsgStream&     printOut ( MsgStream&                   )    const override;

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
  using SolidBase::intersectionTicks;
  unsigned int intersectionTicks( const Gaudi::XYZPoint & Point,
                                  const Gaudi::XYZVector& Vector,
                                  ISolid::Ticks& ticks  ) const override;

  unsigned int intersectionTicks( const Gaudi::Polar3DPoint  & Point,
                                  const Gaudi::Polar3DVector & Vector,
                                  ISolid::Ticks     & ticks) const override;

  unsigned int intersectionTicks( const Gaudi::RhoZPhiPoint  & Point,
                                  const Gaudi::RhoZPhiVector & Vector,
                                  ISolid::Ticks     & ticks) const override;

  /** specific for polycone
   */

  /** accessor to number of triplets
   *  @return number of z-planes
   */
  inline unsigned int number   () const { return triplets().size() ; }

  /** accessor to all triplets
   *  @return reference to all triplets
   */
  inline const Triplets&
  triplets () const { return m_triplets ; }

  /** accessor to triplets begin iterator
   *  @return begin iterator to triplets comntainer
   */
  inline Triplets::const_iterator
  begin    () const { return triplets().begin(); }

  /** accessor to triplets end iterator
   *  @return end iterator to triplets container
   */
  inline Triplets::const_iterator
  end      () const { return triplets().end  (); }

  /** accessor to z-position of plane indexed with some index
   *  @param index   plane index
   *  @return z-position
   */
  inline double  z( Triplets::size_type index ) const
  { return ( begin() + index )->first ; }

  /** accessor to rMax of plane indexed with some index
   *  @param index   plane index
   *  @return rMax
   */
  inline double  RMax( Triplets::size_type index ) const
  { return ( begin() + index )->second.first ; }

  /** accessor to rMin of plane indexed with some index
   *  @param index   plane index
   *  @return rMin
   */
  inline double  RMin( Triplets::size_type index ) const
  { return ( begin() + index )->second.second ; }

  /** accessor to start phi angle
   *  @return start phi angle
   */
  inline double startPhiAngle() const { return m_startPhiAngle;}

  /** accessor to delta phi angle
   *  @return delta phi angle
   */
  inline double deltaPhiAngle() const { return m_deltaPhiAngle;}


  /** find the index from the z position
   *  @param thisz z-position
   *  @return index -- return 'number()-1' if not in z-range
   */
  inline Triplets::size_type index( const double thisz ) const
  {
    Triplets::size_type i = thisz < z(0) || thisz > z(number()-1) ? number()-1 : 0 ;
    for(; i<number()-1 && z(i+1) <= thisz; ++i) {}
    return i ;
  }

  /** Calculate the maximum number of ticks that a straight line could
      make with this solid
  *  @return maximum number of ticks
  */
  Ticks::size_type maxNumberOfTicks() const override { return (number()+1)*2 ; }

protected:
  /** static function to generate triplets for a cone */
  static Triplets makeTriplets(double ZHalfLength        ,
			       double OuterRadiusMinusZ  ,
			       double OuterRadiusPlusZ   ,
			       double InnerRadiusMinusZ  ,
			       double InnerRadiusPlusZ ) ;

  /** static function to generate triplets for a tubs */
  static Triplets makeTriplets(double ZHalfLength        ,
			       double OuterRadius,
			       double InnerRadius) {
    return makeTriplets(ZHalfLength,OuterRadius,OuterRadius,InnerRadius,InnerRadius) ; }

protected:

  /** set bounding parameters
   */
  void setBP();

private:

  SolidPolycone           ( const SolidPolycone & );  ///< no copy-constructor
  SolidPolycone& operator=( const SolidPolycone & );  ///< no assignment

  /**
   * implementation of isInside
   * @param  point reference to any kind of point with x(), y(), z()
   * @return bool
   */
  template <class aPoint>
  bool isInsideImpl(const aPoint& point) const;

  template<class aPoint, class aVector>
  unsigned int intersectionTicksImpl( const aPoint  & Point,
                                      const aVector & Vector,
                                      ISolid::Ticks& ticks ) const;

  /// check if phi is in phi range
  inline bool insidePhi ( const double phi /* [-pi,pi] */ ) const {
    return
      noPhiGap() ||
      ( startPhiAngle ()                   <= phi &&
	startPhiAngle () + deltaPhiAngle() >= phi     ) ||
      ( startPhiAngle ()                   <= phi + 2*M_PI &&
	startPhiAngle () + deltaPhiAngle() >= phi + 2*M_PI ) ;
  }

  /// gap in phi ?
#ifdef __INTEL_COMPILER         // Disable ICC remark
  #pragma warning(disable:1572) // Floating-point equality and inequality comparisons are unreliable
#endif
  bool noPhiGap() const { return m_deltaPhiAngle == 360 * Gaudi::Units::degree ; }
private:

  void createCover();

  Triplets       m_triplets      ; ///< vector of parameters
  double         m_startPhiAngle = 0                          ; ///< start phi angle
  double         m_deltaPhiAngle = 360 * Gaudi::Units::degree ; ///< delta phi angle
  std::unique_ptr<ISolid> m_cover;

};

// ============================================================================
// The END
// ============================================================================
#endif ///< DETDESC_SOLIDPOLYCONE_H
// ============================================================================
