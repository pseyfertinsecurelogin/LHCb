// $Id: PatTTCoord.h,v 1.8 2006-09-15 11:55:48 ocallot Exp $
#ifndef PATTTCOORD_H 
#define PATTTCOORD_H 1

// Include files
// from Gaudi
#include "Kernel/LHCbID.h"
#include "Kernel/Trajectory.h"
#include "PatTools/PatTTRegionGeom.h"

/** @class PatTTCoord PatTTCoord.h
 *  This contains a TT hit 
 *
 *  @author Olivier Callot
 *  @date   2003-12-16
 */
class PatTTCoord {
  
public:
  /// Standard constructor
  PatTTCoord( ) {
    m_z          = 0. ;
    m_measure    = 0. ;
    m_weight     = 0. ;
    m_size       = 0 ;
    m_highThr    = false;
  }

  PatTTCoord(double ordinate ) { m_measure = ordinate;};

  /** 
   * set the main parameters 
   */

  void updateForTrack( double yTr, double tyTr ) {
    m_z       = m_z0 + (yTr - m_y0) * m_dzDy;
    double y  = yTr + tyTr*(m_z-m_z0); 
    m_measure = m_x0 + (y - m_y0) * m_dxDy;
  }

  void updateHit( PatTTRegionGeom* geom, LHCb::Trajectory* traj, 
                  double error, int size, LHCb::LHCbID id, bool highThr) {
    m_geom        = geom;
    m_lhcbId      = id ;

    //== Measurement (trajectory) as straight line
    //== Store x0, z0, dx/dy, dz/dy and range in y.
    double lBeg = traj->beginRange();
    double lEnd = traj->endRange();
    double lMid = .5 * (lBeg + lEnd);
    Gaudi::XYZPoint  cent = traj->position( lMid );
    Gaudi::XYZVector slop = traj->direction( lMid );
    m_dxDy        = slop.X() / slop.Y(); 
    m_x0          = cent.X();
    m_dzDy        = slop.Z() / slop.Y();
    m_y0          = cent.Y();
    m_z0          = cent.Z();
    //== Assumes linear trajectory,i.e. position = a + slope * length
    double halfYLength = fabs( .5 * ( lEnd - lBeg) * slop.y() );
    m_yMin        = m_y0 - halfYLength;
    m_yMax        = m_y0 + halfYLength;
    m_z           = m_z0;  // default until updated
    m_measure     = cent.X() ;
    m_projection  = cent.X() ;
    m_weight      = 1./(error*error);
    m_size        = size;
    m_used        = -1;
    m_highThr     = highThr;
  };

  void setHit( PatTTRegionGeom* geom, LHCb::LHCbID id, 
               double z, double measure, double error, 
               int size, double yMin, double yMax ) {
    m_geom        = geom;
    m_lhcbId      = id ;
    m_z           = z ;
    m_measure     = measure ;
    m_weight      = 1./(error*error);
    m_size        = size;
    m_yMin        = yMin;
    m_yMax        = yMax;
    m_used        = -1;
  };

  virtual ~PatTTCoord( ) {}; ///< Destructor

  bool highThreshold() const { return m_highThr; }

  /**
   * Get accessor to member m_z
   * @return the current value of m_z
   */
  double z () const {     return m_z;   }

  double z0 () const {     return m_z0;   }

  /**
   * Get accessor to member m_measure
   * @return the current value of m_measure
   */
  double measure () const {    return m_measure;  }

  /**
   * Get the LHCbID of the coordinate
   * @return the current value of m_lhcbId
   */
  LHCb::LHCbID lhcbID () const {    return m_lhcbId;  }


  /**
   * Get accessor to member m_size
   * @return the current value of m_size
   */
  int    size()   { return m_size;  }

  /**
   * Get accessor to member m_weight
   * @return the current value of m_weight
   */
  double weight() { return m_weight;  }

  void setProjection( double proj )      { m_projection = proj; }
  double projection()              const { return m_projection;  }

  //== Indirect accessors
  int     station()       const { return m_geom->station();  }
  int     layer()         const { return m_geom->layer();  }
  int     type ()         const { return m_geom->type();  }
  double  cosT ()         const { return m_geom->cosT();  }
  double  sinT ()         const { return m_geom->sinT();  }
  int     planeCode ()    const { return m_geom->planeCode(); }

  /**
   * Get accessor to member m_yMin
   * @return the current value of m_yMin
   */
  double yMin () {
    return m_yMin;
  }

  /**
   * Get accessor to member m_yMax
   * @return the current value of m_yMax
   */
  double yMax () {
    return m_yMax;
  }

  void setUsed( int flag )       { m_used = flag; }
  int isUsed( )                  { return m_used; }

  std::vector<int>& MCKeys()   { return m_keys; }

  class increasingByOrdinate  {
  public:
    bool operator() (const PatTTCoord* firstHit, const PatTTCoord* secondHit ) const {
      if ( 0==firstHit  ) return true;
      if ( 0==secondHit ) return false;
      return firstHit->measure() < secondHit->measure() ;
    }     
  };

  class increasingByProjection  {
  public:
    bool operator() (const PatTTCoord* firstHit, const PatTTCoord* secondHit ) const {
      if ( 0==firstHit  ) return true;
      if ( 0==secondHit ) return false;
      return firstHit->projection() < secondHit->projection() ;
    }     
  };

  class increasingByZ  {
  public:
    bool operator() (const PatTTCoord* firstHit, const PatTTCoord* secondHit ) const {
      if ( 0==firstHit  ) return true;
      if ( 0==secondHit ) return false;
      return firstHit->z() < secondHit->z() ;
    }     
  };

protected:

private:
  PatTTRegionGeom* m_geom;
  double m_z0;
  double m_x0;
  double m_y0;
  double m_dxDy;
  double m_dzDy;

  double  m_z;
  double  m_measure;
  double  m_weight;
  LHCb::LHCbID  m_lhcbId;
  int     m_size;
  double  m_yMin;
  double  m_yMax;
  int     m_used;
  double  m_projection;
  bool    m_highThr;
  std::vector<int> m_keys;
};

typedef std::vector<PatTTCoord*> PatTTCoords;

#endif // PATTTCOORD_H
