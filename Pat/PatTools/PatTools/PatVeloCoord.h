// $Id: PatVeloCoord.h,v 1.5 2006-10-10 15:48:03 ocallot Exp $
#ifndef PATTOOLS_PATVELOCOORD_H 
#define PATTOOLS_PATVELOCOORD_H 1

// Include files
#include <math.h>
#include "Kernel/LHCbID.h"

/** @class PatVeloCoord PatVeloCoord.h PatTools/PatVeloCoord.h
 *  Holds the information of one Velo measurement, sort of cluster
 *  This is the representation of the information for Pattern.
 *
 *  @author Olivier Callot
 *  @date   2005-05-26
 */
class PatVeloCoord {
public: 
  /// Standard constructor
  PatVeloCoord( ) { };

  virtual ~PatVeloCoord( ) {}; ///< Destructor

  void setCoord( int sect, double z, double signal,
                 double coord, double error, LHCb::VeloChannelID channelID,
                 int size ) {
    m_sensor    = channelID.sensor();
    m_sector    = sect;
    m_z         = z;
    m_signal    = signal;
    m_coord     = coord;
    m_error     = error;
    m_channelID = LHCb::LHCbID( channelID);
    m_used      = false;
    m_size = size;
    m_keys.clear();
  }

  int    sensor ()            const { return m_sensor;    }
  int    sector ()            const { return m_sector;    }
  double z()                  const { return m_z; }
  double signal()             const { return m_signal; }
  double coord ()             const { return m_coord;     }
  double error ()             const { return m_error;     }
  LHCb::LHCbID channelID ()         const { return m_channelID; }
  LHCb::VeloChannelID veloID ()     const { return m_channelID.veloID(); }
  int    size()               const { return m_size;      }
  bool   used ()              const { return m_used;      }

  void setUsed( bool flag )         { m_used = flag; }

  void setRadiusAndPhi( double r, double phi ) {
    m_radius = r;
    m_phi    = phi;
    m_cosPhi = cos( phi );
    m_sinPhi = sin( phi );
  }
  double radius() const { return m_radius; }
  double phi()    const { return m_phi; }
  double cosPhi() const { return m_cosPhi; }
  double sinPhi() const { return m_sinPhi; }

  std::vector<int>& keys() { return m_keys; }

  //== Auxilliary for sorting, e.g. inside a sector by coordinate
  class increasingByCoordinate  {
  public:
    bool operator() (const PatVeloCoord* first, const PatVeloCoord* second ) const {
      if ( 0==first  ) return true;
      if ( 0==second ) return false;
      return first->coord() < second->coord() ;
    }
  };

  //== Auxilliary for sorting,e.g.  along a track by increasing z.
  class decreasingByZ  {
  public:
    bool operator() (const PatVeloCoord* first, const PatVeloCoord* second ) const {
      if ( 0==first  ) return true;
      if ( 0==second ) return false;
      return first->z() > second->z() ;
    }
  };


protected:

private:
  int    m_sensor;    ///< Sensor of the coordinate.
  int    m_sector;    ///< Sector in the sensor the coordinate is
  double m_z;         ///< Z coordinate of the sensor
  double m_signal;    ///< Total signal, in ADC count, of this cluster
  double m_coord;     ///< Coordinate, strip number
  double m_error;     ///< Error on the coordinate
  LHCb::LHCbID m_channelID; ///< ID of the first strip+size used for the coordinate
  bool   m_used;      ///< Flag to tag if already used in a track.
  double m_radius;    ///< Radius at which phi is defined 
  double m_phi;       ///< Phi computed for the current track
  double m_cosPhi;    ///< cos( m_phi )
  double m_sinPhi;    ///< sin( m_phi )
  unsigned int m_size;
  std::vector<int> m_keys;  ///< MC particles associated
};
#endif // PATTOOLS_PATVELOCOORD_H
