// $Id: PatTTRegionGeom.h,v 1.1 2005-08-03 07:11:36 ocallot Exp $
#ifndef PATTOOLS_PATTTREGIONGEOM_H 
#define PATTOOLS_PATTTREGIONGEOM_H 1

// Include files

/** @class PatTTRegionGeom PatTTRegionGeom.h PatTools/PatTTRegionGeom.h
 *  Small class to store the basic geometry of TT regions
 *
 *  @author Olivier Callot
 *  @date   2005-08-03
 */
class PatTTRegionGeom {
public: 
  /// Standard constructor
  PatTTRegionGeom( ){}; 

  PatTTRegionGeom( int station, int layer, int type ) {
    m_station   = station;
    m_layer     = layer;
    m_type      = type;
    m_planeCode = 2 * station + (layer%2);
  }

  ~PatTTRegionGeom( ) {}; ///< Destructor

  void setAngle( double ang ) {
    m_cosT = cos( ang );
    m_sinT = sin( ang );
  }
  
  int      station ()   const { return m_station;    }
  int      layer ()     const { return m_layer;    }
  int      type ()      const { return m_type;    }
  int      planeCode () const { return m_planeCode; }
  double   cosT ()      const { return m_cosT;    }
  double   sinT ()      const { return m_sinT;    }

private:
  int    m_station;
  int    m_layer;
  int    m_type;
  int    m_planeCode;
  double m_cosT;
  double m_sinT;
};
#endif // PATTOOLS_PATTTREGIONGEOM_H
