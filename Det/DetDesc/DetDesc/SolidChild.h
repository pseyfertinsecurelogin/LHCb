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
#ifndef       DETDESC_SOLIDCHILD_H
#define       DETDESC_SOLIDCHILD_H 1
/// STD and STL
#include <functional>
#include <optional>
// Geometry
#include "GaudiKernel/Transform3DTypes.h"
/// Detdesc
#include "DetDesc/SolidException.h"
#include "DetDesc/SolidBase.h"
/// forward declarations
class MsgStream;        ///< GaudiKernel
class SolidBoolean;     ///< DetDesc
template <class TYPE>
class SolidFactory;     ///< DetDesc


/** @class SolidChild SolidChild.h DetDesc/SolidChild.h
 *
 *  Simple helper class for implementation of "complex" boolean solids
 *  represent a "solid with position" (e.g. hole in the "mother" solid)
 *
 *  @bug cover and coverTop methods are wrong!
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date xx/xx/xxxx
 */

class SolidChild : public virtual SolidBase
{
  ///
  friend class SolidBoolean;
  friend class SolidFactory<SolidChild>;
  ///
public:

  /** constructor
   *  @param Name name of this solid
   */
  SolidChild( const std::string& Name = "" );

  /** constructor
   *  @param solid pointer to ISolid object
   *  @param mtrx  pointer to transformation
   *  @param Name name of this solid
   */
  SolidChild( std::unique_ptr<ISolid>   solid      ,
              const Gaudi::Transform3D* mtrx  = 0  ,
              const std::string&    Name  = "" );

  /** constructor
   *  @param solid pointer ot ISolid object
   *  @param pos   position
   *  @param rot   rotation
   *  @param Name name of this solid
   */
  SolidChild( std::unique_ptr<ISolid>    solid                 ,
              const Gaudi::XYZPoint&     pos                   ,
              const Gaudi::Rotation3D&    rot   = Gaudi::Rotation3D() ,
              const std::string&    Name  = ""            );



  /** retrieve the specific type of the solid
   *  @return specific type of the solid
   */
  inline std::string typeName   () const override { return "SolidChild" ; }

  /** retrieve the pointer to "simplified" solid - "cover"
   *  @return pointer to "simplified" solid - "cover"
   */
  inline const ISolid* cover () const override { return solid()->cover() ; }

  /** retrieve the pointer to "the most simplified cover"
   *  probably, something like "gabarite box"
   *  @return pointer to the most simplified cover
   */
  inline const ISolid* coverTop () const override
  { return solid()->coverTop() ; }

  /** printout to STD/STL stream
   *  @param os STD/STL stream
   *  @return reference to the stream
   */
  std::ostream& printOut ( std::ostream& os = std::cout ) const override;

  /** printout to Gaudi  stream
   *  @param os Gaudi stream
   *  @return reference to the stream
   */
  MsgStream&    printOut ( MsgStream&    os             ) const override;

  /** - check for the given 3D-point.
   *    Point coordinated are in the local reference
   *    frame of the solid.
   *  - implementation of ISolid absstract interface
   *  @see ISolid
   *  @param point point (in local reference system of the solid)
   *  @return true if the point is inside the solid
   */
  bool isInside ( const Gaudi::XYZPoint& point) const override;
  bool isInside ( const Gaudi::Polar3DPoint   & point ) const override;
  bool isInside ( const Gaudi::RhoZPhiPoint   & point ) const override;
  /** reset to the initial ("after constructor") state
   */
  ISolid*  reset() override;

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
  unsigned int
  intersectionTicks ( const Gaudi::XYZPoint&  Point  ,
                      const Gaudi::XYZVector& Vector ,
                      ISolid::Ticks&          ticks  ) const override;
  unsigned int
  intersectionTicks  ( const Gaudi::Polar3DPoint&  Point  ,
                       const Gaudi::Polar3DVector& Vector ,
                       ISolid::Ticks&              ticks  ) const override;

  unsigned int
  intersectionTicks  ( const Gaudi::RhoZPhiPoint  & Point  ,
                       const Gaudi::RhoZPhiVector & Vector ,
                       ISolid::Ticks              & ticks  ) const override;

  /** Calculate the maximum number of ticks that a straight line could
      make with this solid
  *  @return maximum number of ticks
  */
  Ticks::size_type maxNumberOfTicks() const override { return solid() ? solid()->maxNumberOfTicks() : 0 ; }

public:

  /// "new" method - return solid itself
  inline const ISolid* solid () const { return m_sc_solid.get(); }
  // is solid transformed?
  inline       bool    simple() const { return m_sc_simple; }
  // transfromation
  inline const Gaudi::Transform3D& matrix () const
  {
    if( !m_sc_matrix ) { m_sc_matrix = Gaudi::Transform3D{}; }
    return *m_sc_matrix;
  }

protected:

  /** set boundary parameters
   */
  void setBP();

private:

  /** copy constructor
   *  @param solid reference to SolidChild object
   */
  SolidChild           ( const SolidChild& solid );

  /** assignment
   *  @param solid reference to SolidChild object
   *  @return self reference
   */
  SolidChild& operator=( const SolidChild& solid );

  /**
   * implementation of isInside
   * @param  point reference to any kind of point with x(), y(), z()
   * @return bool
   */
  template<class aPoint>
  bool isInsideImpl(const aPoint& point) const;

  template<class aPoint, class aVector>
  unsigned int intersectionTicksImpl ( const aPoint&  Point  ,
                                       const aVector& Vector ,
                                       ISolid::Ticks& ticks  ) const;

private:
  ///
  std::unique_ptr<ISolid>                      m_sc_solid   ; ///< own solid
  mutable std::optional<Gaudi::Transform3D>  m_sc_matrix  ; ///< position/rotation
  bool                     m_sc_simple  ; ///< flag if "simple"
  ///
};

/// ===========================================================================
#endif   ///<   DETDESC_SOLIDCHILD_H
/// ===========================================================================
