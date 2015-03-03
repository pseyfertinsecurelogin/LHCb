// $Id: PatTCoord.h,v 1.8 2006-11-09 07:56:58 ocallot Exp $
#ifndef PATTCOORD_H 
#define PATTCOORD_H 1

// Include files
#include "Kernel/LHCbID.h"
#include "Kernel/Trajectory.h"
#include "PatTools/PatTRegionGeom.h"

/** @class PatTCoord PatTCoord.h
 *  Store a T station coordinate, IT or OT, appropriate for pattern
 *
 *  @author Olivier Callot
 *  @date   2005-06-27
 */
class PatTCoord {
public: 

  /// Standard constructor
  PatTCoord( ) {}; 

  /// Constructor with only ordinate, for comparison (PatTRegion.h in PatForward)
  PatTCoord( double ordinate ) { m_x0 = ordinate; }; 

  virtual ~PatTCoord( ) {}; ///< Destructor

  //== Simple accessors to internal data members

  double  z ()            const { return m_z;   }
  double  measure ()      const { return m_measure;  }
  double  projection ()   const { return m_projection;  }
  double  weight()        const { return m_weight;  }
  double  driftDistance() const { return m_driftDistance; }
  LHCb::LHCbID lhcbID()   const { return m_lhcbID; }
  int     size()          const { return m_size; }
  double  yMin ()         const { return m_yMin;  }
  double  yMax ()         const { return m_yMax;  }
  double  dxDy ()         const { return m_dxDy;  }
  double  dzDy ()         const { return m_dzDy;  }
  bool    isUsed()        const { return m_used; }
  bool    isSelected()    const { return m_selected; }
  bool    isIgnored()     const { return m_ignored; }
  bool    hasNext()       const { return m_hasNext; }
  bool    hasPrevious()   const { return m_hasPrevious; }
  int     rl()            const { return m_rl; }
  bool    highThreshold() const { return m_highThreshold; }

  //== Indirect accessors
  int     station()       const { return m_geom->station();  }
  int     layer()         const { return m_geom->layer();  }
  int     type ()         const { return m_geom->type();  }
  double  sinT()          const { return m_geom->sinT(); }
  bool    isIT()          const { return 1 < type();      }
  bool    isX()           const { return (0 == m_geom->layer()) || ( 3 == m_geom->layer() ); }
  int     planeCode ()    const { return m_geom->planeCode(); }
  bool    isYCompatible ( double y, double tol ) {
    if ( m_yMin - tol > y ) return false;
    if ( m_yMax + tol < y ) return false;
    return true;
  }

  //== Main setters
  void updateHit( PatTRegionGeom* geom, 
                  double x0, double z0, double yMin, double yMax, double dxDy, double dzDy,
                  double error, int size, LHCb::LHCbID id, 
                  double drift=0., double wireLength=0. ) {
    m_geom        = geom;
    m_x0          = x0;
    m_z0          = z0;
    m_yMin        = yMin;
    m_yMax        = yMax;
    m_dxDy        = dxDy;
    m_dzDy        = dzDy;

    m_z           = m_z0;  // default until updated
    m_measure     = m_x0;
    m_projection  = m_x0;

    m_weight      = 1. / error / error;
    m_size        = size;
    m_lhcbID      = id;
    m_driftTime   = drift;
    m_driftDistance = drift;
    m_wireLength  = wireLength;

    m_used        = false;
    m_selected    = true;
    m_ignored     = false;
    m_hasNext     = false;
    m_hasPrevious = false;
    m_rl          = 0;
    m_highThreshold = false;
    m_keys.clear();
  };

  //== Set the measures at the Y of the track. Compute y, then x and z
  void updateForTrack( double y0, double dyDz, double wireVelocity, double driftVelocity ) {
    double y  = ( y0 + dyDz * m_z0 ) / ( 1. - m_dzDy * dyDz );
    m_z       = m_z0 + y * m_dzDy;
    m_measure = m_x0 + y * m_dxDy;
    if ( 1 < type() ) return;
    double dy = m_wireLength - fabs( y );      
    m_driftDistance = ( m_driftTime - dy * wireVelocity ) * driftVelocity;
  };  

  void setHighThreshold( bool threshold )    { m_highThreshold = threshold; }

  void saveDefaultMeasure() {
    m_zSave       = m_z;
    m_measureSave = m_measure;
    m_driftSave   = m_driftDistance;
  }
  void restoreDefaultMeasure() {
    m_z             = m_zSave;
    m_measure       = m_measureSave;
    m_driftDistance = m_driftSave;
  }
  

  double ordinate() const { return m_x0; }

  void setProjection( double projection )  { m_projection = projection;  m_rl = 0; }

  //== Setters for simple properties

  void setUsed( bool flag )              { m_used        = flag; }
  void setSelected( bool flag )          { m_selected    = flag; }
  void setIgnored( bool flag )           { m_ignored     = flag; }
  void setHasNext( bool flag )           { m_hasNext     = flag; }
  void setHasPrevious( bool flag )       { m_hasPrevious = flag; }
  void setRl( int rl )                   { m_rl = rl;            }

  std::vector<int>& MCKeys()   { return m_keys; }

  //== Auxilliary, for sorting

  class increasingByOrdinate  {
  public:
    bool operator() (const PatTCoord* firstHit, const PatTCoord* secondHit ) const {
      if ( 0==firstHit  ) return true;
      if ( 0==secondHit ) return false;
      return firstHit->ordinate() < secondHit->ordinate() ;
    };
  };

  class increasingByProjection{
  public:
    bool operator() ( const PatTCoord* first, 
                      const PatTCoord* second ) const {
      return first->projection() < second->projection();
    }
  };
 
  class increasingByZ{
  public:
    bool operator() ( const PatTCoord* first, 
                      const PatTCoord* second ) const {
      return first->z() < second->z();
    }
  };
 
protected:

private:
  PatTRegionGeom* m_geom;
  double m_z0;
  double m_x0;
  double m_dxDy;
  double m_dzDy;

  double m_z;
  double m_measure;
  double m_projection;
  double m_weight;
  double m_driftTime;
  double m_driftDistance;
  double m_wireLength;
  LHCb::LHCbID m_lhcbID;
  int    m_size;
  double m_yMin;
  double m_yMax;
  bool   m_used;
  bool   m_selected;
  bool   m_ignored;
  bool   m_hasNext;
  bool   m_hasPrevious;
  bool   m_highThreshold;
  int    m_rl;
  double m_zSave;
  double m_measureSave;
  double m_driftSave;

  std::vector<int> m_keys;
};

//== To be used as a vector
typedef std::vector<PatTCoord*> PatTCoords;

#endif // PATTCOORD_H
