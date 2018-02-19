#ifndef LHCbKernel_ParabolaTraj_H
#define LHCbKernel_ParabolaTraj_H 1

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

  /** @class ParabolaTraj ParabolaTraj.h Kernel/ParabolaTraj.h
   *
   * Trajectory parameterized as a parabola
   *
   * @author Edwin Bos, Jeroen van Tilburg, Eduardo Rodrigues
   * @date   09/01/2006
   */

  class ParabolaTraj: public Trajectory<double> {

  public:

    // clone thyself...
    std::unique_ptr<Trajectory<double>> clone() const override;

    /// Constructor from a (middle) point, a (unit) direction vector,
    /// a curvature vector and the pair of begin- and endpoints.
    ParabolaTraj( const Point& middle,
                  const Vector& dir,
                  const Vector& curv,
                  const Trajectory<double>::Range& range );

    /// Point on the trajectory at arclength from the starting point
    Point position( double arclength ) const override;

    /// First derivative of the trajectory at arclength from the starting point
    Vector direction( double arclength ) const override;

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
                           int pathDirection ) const override;

    /// Number of arclengths until deviation of the trajectory from the
    /// expansion reaches the given tolerance.
    double distTo2ndError( double arclength,
                           double tolerance,
                           int pathDirection ) const override;

    /// Distance, along the Trajectory, between position(mu1) and
    /// position(mu2). Trivial because ParabolaTraj is parameterized in
    /// arclength.
    using Trajectory<double>::arclength;
    double arclength(double mu1, double mu2) const override { return mu2 - mu1 ; }

  private:

    Point  m_pos;
    Vector m_dir;
    Vector m_curv;

  }; // class ParabolaTraj

} // namespace LHCb

#endif /// LHCbKernel_ParabolaTraj_H
