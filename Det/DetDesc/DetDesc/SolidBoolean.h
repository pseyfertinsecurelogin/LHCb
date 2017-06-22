#ifndef      DETDESC_SOLIDBOOLEAN_H
#define      DETDESC_SOLIDBOOLEAN_H  1
/// STL & STD
#include <string>
#include <vector>
#include <algorithm>
#include "range/v3/view/indirect.hpp"
#include "range/v3/view/const.hpp"

/// DetDesc
#include "DetDesc/SolidChild.h"

// Forward declarations
class StatusCode;
class MsgStream;


/** @class SolidBoolean SolidBoolean.h "DetDesc/SolidBoolean.h"
 *
 *  A simple helper class (common base) for implementation of
 *  complex "boolean" solids.
 *  represents 1 "main"("first") solid and a container of
 *  "children"  placed inside of "main"
 *
 *  @author Vanya Belyaev
 */

class SolidBoolean: public virtual SolidBase
{
 public:
  //
  typedef std::vector<std::unique_ptr<SolidChild>> SolidChildrens;
  //

  /** retrieve the specific type of the solid
   *  @return specific type of the solid
   */
  std::string   typeName () const override { return "SolidBoolean"; }

  /** retrieve the pointer to "simplified" solid - "cover"
   *  @return pointer to "simplified" solid - "cover"
   */
  const ISolid* cover () const override { return first()->cover() ; }

  /** retrieve the pointer to "the most simplified cover"
   *  probably, something like "gabarite box"
   *  @return pointer to the most simplified cover
   */
  const ISolid* coverTop () const override { return first()->coverTop() ; }

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

  /** reset to the initial ("after constructor") state
   */
  ISolid* reset() override;

  /** calculate the intersection points("ticks") with a given line.
   *  Input - line, paramterised by  x_vect = Point + Vector * T
   *  "tick" is just a value of T, at which the intersection occurs
   *  @param Point initial point for the line
   *  @param Vector vector along the line
   *  @param ticks output container of "Ticks"
   *  @return the number of intersection points (=size of Ticks container)
   */
  unsigned int intersectionTicks( const Gaudi::XYZPoint & Point,
                                  const Gaudi::XYZVector& Vector,
                                  ISolid::Ticks& ticks  ) const override;

  unsigned int intersectionTicks( const Gaudi::Polar3DPoint  & Point,
                                  const Gaudi::Polar3DVector & Vector,
                                  ISolid::Ticks     & ticks) const override;

  unsigned int intersectionTicks( const Gaudi::RhoZPhiPoint  & Point,
                                  const Gaudi::RhoZPhiVector & Vector,
                                  ISolid::Ticks     & ticks) const override;

  /** calculate the intersection points("ticks") with a given line.
   *  Input - line, paramterised by  x_vect = Point + Vector * T
   *  "tick" is just a value of T, at which the intersection occurs
   *  @param Point initial point for the line
   *  @param Vector vector along the line
   *  @param tickMin minimal value of "Tick"
   *  @param tickMax maximal value of "Tick"
   *  @param ticks output container of "Ticks"
   *  @return the number of intersection points (=size of Ticks container)
   *  between tickMin and tickMax
   */

  unsigned int intersectionTicks( const Gaudi::XYZPoint& Point,
                                  const Gaudi::XYZVector & Vector,
                                  const ISolid::Tick& tickMin,
                                  const ISolid::Tick& tickMax,
                                  ISolid::Ticks& ticks   ) const override;

  unsigned int intersectionTicks( const Gaudi::Polar3DPoint& Point,
                                  const Gaudi::Polar3DVector & Vector,
                                  const ISolid::Tick& tickMin,
                                  const ISolid::Tick& tickMax,
                                  ISolid::Ticks& ticks   ) const override;

  unsigned int intersectionTicks( const Gaudi::RhoZPhiPoint& Point,
                                  const Gaudi::RhoZPhiVector & Vector,
                                  const ISolid::Tick& tickMin,
                                  const ISolid::Tick& tickMax,
                                  ISolid::Ticks& ticks   ) const override;

  /** poiter to the "main"/"first" boolean
   *  @return poiter to the "main"/"first" boolean
   */
  const  ISolid* first () const { return m_sb_first.get(); }

  /** acess to range of const children
   *  @return "range"
   */
  auto children() const { return m_sb_childrens | ranges::view::indirect | ranges::view::const_; }

  /** access to the childrens by index
   *  @param index index of child solid
   *  @return pointer to child solid
   */
  const ISolid* operator[]  ( unsigned int index ) const
  {   auto c = children();
      return  index < c.size() ? &c[index] : nullptr ;
  }

  /** acess to constant iterator
   *  @return "begin" iterator
   */
  [[deprecated("please use begin(children()) instead")]]
  SolidChildrens::const_iterator
  childBegin () const { return m_sb_childrens.begin(); }

  /** acess to constant iterator
   *  @return "end" iterator
   */
  [[deprecated("please use end(children()) instead")]]
  SolidChildrens::const_iterator
  childEnd   () const { return m_sb_childrens.end  (); }

  ///

  /** constructor - "main"("first") solid is mandatory!
   *  @param name name of the solid
   *  @param solid pointer to teh "first"/"main" solid
   *  @exception SolidException NULL pointer to ISolid
   */
  SolidBoolean( const std::string& name  ,
                std::unique_ptr<ISolid> solid );

protected:

  /** constructor - "main"("first") solid is mandatory!
   *  @param name name of the solid
   */
  SolidBoolean ( const std::string& name="Undefined" );

  /** add child to daughter container
   *  @param child pointer to solid
   *  @param mtrx  pointer to transformation
   *  @return status code
   */
  StatusCode addChild
  ( std::unique_ptr<ISolid>   child ,
    const Gaudi::Transform3D* mtrx  );

  /** add child to daughter container
   *  @param child    pointer to solid
   *  @param position position
   *  @param rotation rotation
   */
  StatusCode addChild
  ( std::unique_ptr<ISolid>   child    ,
    const Gaudi::XYZPoint&     position ,
    const Gaudi::Rotation3D&    rotation );

  /** acess to range of children
   *  @return "range"
   */
  auto children() { return m_sb_childrens | ranges::view::indirect; }

  /** Calculate the maximum number of ticks that a straight line could
      make with this solid
  *  @return maximum number of ticks
  */
  Ticks::size_type maxNumberOfTicks() const override;

private:

  // default constructor is disabled
  // SolidBoolean() ;
  // assignement operator is disabled
  SolidBoolean& operator=(SolidBoolean & ) ;

  template<class aPoint, class aVector>
  unsigned int intersectionTicksImpl( const aPoint  & Point,
                                      const aVector & Vector,
                                      const ISolid::Tick& tickMin,
                                      const ISolid::Tick& tickMax,
                                      ISolid::Ticks&  ticks) const;

  template<class aPoint, class aVector>
  unsigned int intersectionTicksImpl( const aPoint  & Point,
                                      const aVector & Vector,
                                      ISolid::Ticks& ticks ) const;

protected:

  /** set bounding parameters
   */
  void setBP();

private:
  ///
  std::string                    m_sb_name     ;
  mutable     std::unique_ptr<ISolid> m_sb_first    ;
  mutable     SolidChildrens     m_sb_childrens;
  //
};

/// ===========================================================================
#endif   ///< DETDESC_SOLIDBOOLEAN_H
/// ===========================================================================
