// $Id: PatTRegionGeom.h,v 1.2 2005-09-23 07:55:15 cattanem Exp $
#ifndef PATTREGIONGEOM_H 
#define PATTREGIONGEOM_H 1

// Include files
#include <math.h>

/** @class PatTRegionGeom PatTRegionGeom.h
 *  Simple class to hold the geometry of a region. 
 *
 *  @author Olivier Callot
 *  @date   2005-04-01
 */
class PatTRegionGeom {
public: 
  /// Standard constructor
  PatTRegionGeom() {};

  PatTRegionGeom( int station, int layer, int type ) {
    m_station   = station;
    m_layer     = layer;
    m_type      = type;
    m_planeCode = 4 * station + layer;
  }

  ~PatTRegionGeom( ){}; ///< Destructor

  void setAngle( double ang ) {
    m_cosT = cos( ang );
    m_sinT = sin( ang );
    m_tanT = m_sinT / m_cosT;
  }
  
  int      station ()   const { return m_station;    }
  int      layer ()     const { return m_layer;    }
  int      type ()      const { return m_type;    }
  int      planeCode () const { return m_planeCode; }
  double   cosT ()      const { return m_cosT;    }
  double   sinT ()      const { return m_sinT;    }
  double   tanT ()      const { return m_tanT;    }

private:
  int    m_station;
  int    m_layer;
  int    m_type;
  int    m_planeCode;
  double m_cosT;
  double m_sinT;
  double m_tanT;
};
#endif // PATTREGIONGEOM_H
