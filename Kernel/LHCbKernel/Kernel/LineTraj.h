#ifndef LHCbKernel_LineTraj_H
#define LHCbKernel_LineTraj_H 1

#include <boost/pool/singleton_pool.hpp>
#include <stddef.h>
#include <memory>
#include <new>

// Include files
#include "Kernel/DifTraj.h"
#include "Kernel/Trajectory.h"
#ifdef _WIN32
// Avoid conflict of Windows macro with std::max
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
#endif
#include "GaudiKernel/boost_allocator.h"

namespace LHCb
{

  /** @class LineTraj LineTraj.h Kernel/LineTraj.h
   *
   * Trajectory parameterized as a straight line
   *
   * @author Edwin Bos, Jeroen van Tilburg, Eduardo Rodrigues
   * @date   17/01/2005
   *
   */

  class LineTraj: public Trajectory {

  public:

    // clone thyself...
    std::unique_ptr<Trajectory> clone() const override;

    /// Constructor from the middle point and a direction vector
    LineTraj( const Point& middle,
              const Vector& dir,
              const Range& range );

    // constructor assuming a normalized direction vector.
    // value of 'normalized' is NOT checked, only used to disambiguate from the
    // above constructor...
    LineTraj( const Point& middle,
              const Vector& dir,
              const Range& range,
              bool normalized );

    // constructor which takes either a normalized direction vector or not....
    enum isNormalized {yes,no} ;
    LineTraj( const Point& middle,
              const Vector& dir,
              const Range& range,
              isNormalized no );



    /// Constructor from a begin and an end point
    LineTraj( const Point& begPoint,
              const Point& endPoint );

    /// Point on the trajectory at arclength from the starting point
    Point position( double arclength ) const override;

    /// First derivative of the trajectory at arclength from the starting point
    Vector direction( double arclength=0 ) const override;

    /// Second derivative of the trajectory at arclength from the starting point
    Vector curvature( double arclength=0 ) const override;

    /// Create a parabolic approximation to the trajectory
    /// at arclength from the starting point
    void expansion( double arclength,
                    Point& p,
                    Vector& dp,
                    Vector& ddp ) const override;

    /// Determine the distance in arclenghts to the
    /// closest point on the trajectory to a given point
    double muEstimate( const Point& point ) const override;

    /// Number of arclengths until deviation of the trajectory from the
    /// expansion reaches the given tolerance.
    double distTo1stError( double arclength,
                           double tolerance,
                           int pathDirection = +1 ) const override;

    /// Number of arclengths until deviation of the trajectory from the
    /// expansion reaches the given tolerance.
    double distTo2ndError( double arclength,
                           double tolerance,
                           int pathDirection = +1 ) const override;

    /// Distance, along the Trajectory, between position(mu1) and
    /// position(mu2). Trivial because LineTraj is parameterized in
    /// arclength.
    using Trajectory::arclength;
    double arclength(double mu1, double mu2) const override { return mu2 - mu1 ; }

#ifndef GOD_NOALLOC
    /// operator new
    static void* operator new ( size_t size )
    {
      return ( sizeof(LineTraj) == size ?
               boost::singleton_pool<LineTraj, sizeof(LineTraj)>::malloc() :
               ::operator new(size) );
    }

    /// placement operator new
    /// it is needed by libstdc++ 3.2.3 (e.g. in std::vector)
    /// it is not needed in libstdc++ >= 3.4
    static void* operator new ( size_t size, void* pObj )
    {
      return ::operator new (size,pObj);
    }

    /// operator delete
    static void operator delete ( void* p )
    {
      boost::singleton_pool<LineTraj, sizeof(LineTraj)>::is_from(p) ?
        boost::singleton_pool<LineTraj, sizeof(LineTraj)>::free(p) :
        ::operator delete(p);
    }

    /// placement operator delete
    /// not sure if really needed, but it does not harm
    static void operator delete ( void* p, void* pObj )
    {
      ::operator delete (p, pObj);
    }
#endif


  private:

    Vector m_dir;
    Point  m_pos;

  }; // class LineTraj

} // namespace LHCb

#ifdef __INTEL_COMPILER         // Disable ICC remark from ROOT
 #pragma warning(disable:1572) // Floating-point equality and inequality comparisons are unreliable
#endif

/// Constructor from the middle point and a direction vector
inline LHCb::LineTraj::LineTraj( const Point& middle,
                                 const Vector& dir,
                                 const Range& range )
  : Trajectory(range),
    m_dir(dir.Unit()),
    m_pos(middle)
{}

/// Constructor from the middle point and a unit direction vector
inline LHCb::LineTraj::LineTraj( const Point& middle,
                                 const Vector& dir,
                                 const Range& range, bool /* normalized */ )
  : Trajectory(range),
    m_dir(dir),
    m_pos(middle)
{}

/// Constructor from the middle point and a possibly normalized direction vector
inline LHCb::LineTraj::LineTraj( const Point& middle,
                                 const Vector& dir,
                                 const Range& range, isNormalized normalized)
  : Trajectory(range),
    m_dir(normalized==yes?dir:dir.Unit()),
    m_pos(middle)
{}

#endif /// LHCbKernel_LineTraj_H
