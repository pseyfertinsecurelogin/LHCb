// $Id: Line.h,v 1.9 2009-07-09 08:02:11 cattanem Exp $
// ============================================================================
#ifndef LHCBMATH_LINE_H 
#define LHCBMATH_LINE_H 1
// ============================================================================
// Include files
// ============================================================================
#include <iostream>
#ifdef __INTEL_COMPILER         // Disable ICC remark from ROOT GenVector classes
  #pragma warning(disable:1572) // floating-point equality and inequality comparisons are unreliable
  #pragma warning(push)
#endif
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
// ============================================================================
namespace Gaudi
{
  namespace Math
  {
    
    // ==========================================================================
    /** @class Line Line.h Kernel/Line.h
     *  
     *  A very simple class to describe a 3D-line.
     *  Based on line equation 
     *  \f$ \vec{\mathbf{P}}\left(\mu\right) = 
     *      \vec{\mathbf{P}}_0 + \mu \vec{\mathbf{V}}_0 \f$.
     *  where \f$ \vec{\mathbf{V}}_0\f$ is a direction vector of the 
     *  line, \f$ \vec{\mathbf{V}}_0=\vec{\mathbf{P}}_1-\vec{\mathbf{P}}_1 \f$,
     *  \f$ \vec{\mathbf{P}}_1\f$ and \f$\vec{\mathbf{P}}_0 \f$
     *   being two points on the line.
     *  
     *  The class can be used as a very general abstraction for a 3D line.
     *  However, as there is only const access to the first point given in the 
     *  constructor, this can take the meaning of the origin of a ray. The
     *  direction vector also only has const access. It can therefore be safely
     *  used to define the scale of steps along the line. Users requiring a 
     *  normalised direction vector should construct the line using one.
     *
     *  @author Juan PALACIOS
     *  @date   2006-04-19
     */
    template<typename aPoint, typename aVector>
    class Line 
    {
    public:
      typedef aPoint  Point;
      typedef aVector Vector;   
    public:
      /// the default constructor 
      Line() {}
      /// the constructor from the point and direction vector 
      Line ( const aPoint& p0 , const aVector& v0 ) : m_p0 ( p0 ) , m_v0 ( v0      ) {}
      /// the constructor from two points:
      Line ( const aPoint& p0 , const aPoint&  p1 ) : m_p0 ( p0 ) , m_v0 ( p1 - p0 ) {}
      /// Return the point of origin
      const aPoint&  beginPoint() const { return m_p0 ; }
      /// Return the direction vector of the line
      const aVector& direction()  const { return m_v0 ; }
      /** Return a point on the line tick direction vectors away
       *  from point of origin:
       *  \f$ \vec{\mathbf{P}}\left(\mu\right) = 
       *      \vec{\mathbf{P}}_0 + \mu\vec{\mathbf{V}} \f$ 
       */
      aPoint position ( const double mu ) const 
      { return beginPoint() + direction() * (float)mu ; }
      /** Return a point on the line tick direction vectors away
       *  from point of origin:
       *  \f$ \vec{\mathbf{P}}\left(\mu\right) = 
       *      \vec{\mathbf{P}}_0 + \mu\vec{\mathbf{V}} \f$ 
       */    
      aPoint operator() ( const double mu ) const 
      { return beginPoint() + direction() * (float)mu ; }
    public:
      inline std::ostream& fillStream ( std::ostream& os ) const 
      {
        os << "\np0 (" 
           << m_p0.x() << " " << m_p0.y() << " " << m_p0.z()
           << ") direction (" 
           << m_v0.x()<< " " << m_v0.y() << " " << m_v0.z() << ")\n"
           << std::endl;
        return os;
      }
    private:
      /// the start point on the line 
      aPoint  m_p0; // the start point on the line 
      /// the direction vector of the line  
      aVector m_v0; // the direction vector of the line
    };

    // ========================================================================
  } // end of namespace Math

  // ==========================================================================
} // end of namespace Gaudi

// ============================================================================
template<typename aPoint, typename aVector>
inline std::ostream& operator<<
  (std::ostream&                      os  , 
   const Gaudi::Math::Line<aPoint,aVector>& rhs ) 
{ 
  return rhs.fillStream(os); 
}
#ifdef __INTEL_COMPILER // End disable ICC remark from ROOT GenVector classes
  #pragma warning(pop)
#endif
// ==========================================================================
// The END 
// ==========================================================================
#endif // LHCBMATH_LINE_H
// ==========================================================================

