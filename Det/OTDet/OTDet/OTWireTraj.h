#ifndef OTDET_OTWIRETRAJ
#define OTDET_OTWIRETRAJ

#include "Kernel/Trajectory.h"
#include "GaudiKernel/SystemOfUnits.h"
#include <boost/pool/poolfwd.hpp>

/*****************************/
/* class representing a piecewise trajectory, with pieces of fixed length, parameterized along y */
/*****************************/

namespace LHCb
{

  /// Interface class
  class OTWireTraj : public Trajectory<double>
  {
  public:
    typedef unsigned int Index ;
    OTWireTraj(const double beginyrange, const double endyrange) : Trajectory<double>(beginyrange,endyrange) {}
    virtual void applyTranslation( const Vector& vec ) = 0 ;
    virtual std::unique_ptr<OTWireTraj> cloneOTWireTraj() const = 0 ;
    virtual void indexAndMu(double y, unsigned int& index, double& localmu) const = 0 ;
    virtual Index numSegments() const = 0 ;
  } ;

  /// Templated implementation
  template<unsigned int N>
  class OTWireTrajImp : public OTWireTraj
  {
  private:
      // helper class to parameterize locally a line
  class Segment final
  {
    Point  m_begin ;
    Vector m_dir ;
  public:
    Segment() {}
    Segment( const Point& begin,
             const Point& end ) : m_begin(begin) {
        m_dir = (end - begin) / (end.y()-begin.y() ) ;
    }

    const Point& begin() const { return m_begin ; }

    Point position( double mu ) const {
      return m_begin + (mu-m_begin.y()) * m_dir ;
    }

	Vector direction() const {
	  // do we really need unit here?
	  return m_dir.Unit();
	}

	void expansion( double mu, Point& p,
			Vector& dp, Vector& ddp ) const
	{
	  ddp = Vector(0,0,0);
	  dp  = m_dir;
	  p   = m_begin + (mu-m_begin.y()) * m_dir;
	}

	void applyTranslation( const Vector& translation ) {
	  m_begin += translation ;
	}
      } ;

      double yBegin() const { return m_segments.front().begin().y() ; }

    private:
      std::array<Segment,N> m_segments ; /// subsegments
      double m_deltay ; /// Length of 3 segments (not equal to range)
      double m_dndy ;   /// normalization for arclength

    public:
      /// Default constructor. The list of points must be even: each
      /// specifies the begin and end of a section between two wire
      /// locators. Effectively, all segments have the same length
      /// (808mm). The range is specified independent of these points
      /// because for short modules, the actual length of the wire is
      /// not 3 times the space between the wire locators.


    OTWireTrajImp( const std::array<Point,2*N>& points, const double beginyrange, const double endyrange)
      : OTWireTraj(beginyrange, endyrange)
	{
	  for( size_t i=0; i<N; ++i)
	    m_segments[i] = Segment( points[2*i], points[2*i+1] ) ;
	  Vector delta = points.back() - points.front() ;
	  m_deltay = delta.y() ;
	  m_dndy = delta.R() / delta.y() ;
	}


    // We create this one with template specialization to speed things up
    inline Index index(double y) const ;

    /// apply a translation in space to entire trajectory
    void applyTranslation( const Vector& vec ) override
    {
      // move the segments
      for( Segment& iseg : m_segments ) iseg.applyTranslation( vec ) ;
      // change the range
      Range r = range() ;
      setRange( r.first+vec.y(), r.second+vec.y() ) ;
    }

    /// returns a mu in units of range, within a segment. used in the calibration.
    void indexAndMu(double y, Index& index, double& localmu) const override
    {
      double frac = N*(y - yBegin()) / m_deltay ;
      int i = int(frac) ;
      index = i<=0 ? 0 : (i>=int(N) ? N-1 : i ) ;
      localmu = frac - index ;
    }

    std::unique_ptr<OTWireTraj> cloneOTWireTraj() const override {
	  return std::make_unique<OTWireTrajImp<N>>(*this);
    }

    Index numSegments() const override { return N ; }

    public: // implementation for member functions of the interface

    std::unique_ptr<Trajectory<double>> clone() const override {
      return std::make_unique<OTWireTrajImp<N>>(*this);
    }

    /// trajectory is parameterized along y
    double muEstimate( const Point& p) const override {
      return p.y() ;
    }

    /// Point on the trajectory
    Point position( double mu ) const override {
      return m_segments[index(mu)].position( mu ) ;
    }

    Vector direction( double mu) const override {
      return m_segments[index(mu)].direction() ;
    }

    Vector curvature( double ) const override {
      return Vector(0.0,0.0,0.0) ;
    }

    /// Expansion ... all that matters for TrajPoca
    void expansion( double mu, Point& p, Vector& dp, Vector& ddp ) const override {
      m_segments[index(mu)].expansion(mu,p,dp,ddp ) ;
    }

    double distTo1stError( double, double, int) const override {
      return 10*Gaudi::Units::km;
    }

    double distTo2ndError( double, double, int) const override {
      return 10*Gaudi::Units::km;
    }

      /// arclength for given range
    double arclength(double mu1, double mu2) const override {
      return (mu2 - mu1)*m_dndy;
    }

    /// arclength
    double arclength() const override {
      return arclength(beginRange(),endRange()) ;
    }

  } ;

  template<unsigned int N>
  inline OTWireTraj::Index OTWireTrajImp<N>::index( double y) const {
    double frac = (y - yBegin())/m_deltay ;
    int i = int(N*frac) ;
    return i<=0 ? 0 : ( i>=int(N) ? N-1 : i ) ;
  }

  template<>
  inline OTWireTraj::Index OTWireTrajImp<1>::index(double) const {
    return 0 ;
  }
}
#endif
