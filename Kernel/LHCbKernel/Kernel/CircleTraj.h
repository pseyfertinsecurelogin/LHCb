#ifndef LHCbKernel_CircleTraj_H
#define LHCbKernel_CircleTraj_H 1

// Include files
#include "Kernel/DifTraj.h"
#ifdef _WIN32
// Avoid conflict of Windows macro with std::max
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
#endif

namespace LHCb
{

  /** @class CircleTraj CircleTraj.h Kernel/CircleTraj.h
   *
   * Trajectory parameterized as a circle
   *
   * @author Edwin Bos, Eduardo Rodrigues
   * @date   30/01/2006
   *
   */

  class CircleTraj : public Trajectory<double> {

  public:

    // clone thyself...
    std::unique_ptr<Trajectory<double>> clone() const override;

    using Trajectory<double>::Trajectory;
    using Vector = typename Trajectory<double>::Vector;
    using Point  = typename Trajectory<double>::Point;
    using Range  = typename Trajectory<double>::Range;

    CircleTraj() = default;

    /// Constructor from a center, the normal which defines the plane
    /// of the circle, a vector from the center to a point on the circle,
    /// and the range of the circle (radius*phirange, phi=0 corresponds to
    /// the above point on the circle, phi is the angle which is used to
    /// rotate this point around the normal)
    /// Only the component of origin2point perpendicular to normal
    /// is considered!!!...
    CircleTraj( const Point& origin,// center of circle
                const Vector& normal, // direction of end
                const Vector& origin2point, // direction of start
                const Trajectory<double>::Range& range); // valid range, in radius*deltaphi

    /// Constructor from a center, the directions of the
    /// start and end of the traj wrt. the center, and the radius.
    /// The Traj goes along the 'short' arc from origin+radius*dir1.unit()
    /// at arclength=0 to origin+radius*dir2.unit() at the arclen=length().
    CircleTraj( const Point& origin,// center of circle
                const Vector& dir1, // direction of start
                const Vector& dir2, // direction of end
                double radius);

    /// Point on the trajectory at arclength from the starting point
    Point position( double arclength ) const override;

    /// First derivative of the trajectory at arclength from the starting point
    Vector direction( double arclength ) const override;

    /// Second derivative of the trajectory at arclength from the starting point
    Vector curvature( double arclength ) const override;

    /// Create a parabolic approximation to the trajectory
    /// at arclength from the starting point
    void expansion( double arclength,
                    Point& p,
                    Vector& dp,
                    Vector& ddp ) const override;

    /// Return arclen at which the trajectory is
    /// closest to the specified point
    double muEstimate( const Point& point ) const override;

    /// distance along the trajectory until deviation from the
    /// 1st order expansion reaches the given tolerance.
    double distTo1stError( double arclength,
                           double tolerance,
                           int pathDirection = +1 ) const override;

    /// distance along trajectory until deviation from the
    /// 2nd order expansion reaches the given tolerance.
    double distTo2ndError( double arclength,
                           double tolerance,
                           int pathDirection = +1 ) const override;

    /// Distance, along the Trajectory, between position(mu1) and
    /// position(mu2). Trivial because CircleTraj is parameterized in
    /// arclength.
    using Trajectory<double>::arclength;
    double arclength(double mu1, double mu2) const override { return mu2 - mu1 ; }

  private :

    Point  m_origin;
    Vector m_normal;
    Vector m_dirStart;
    double m_radius = 0.;
    double m_cbrt6radius2 = 0.;

  }; // class CircleTraj

} // namespace LHCb

#endif /// LHCbKernel_CircleTraj_H
