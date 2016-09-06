
//-----------------------------------------------------------------------------
/** @file RichGeomFunctions.h
 *
 *  Header file for RICH geometrical utility functions
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   20/01/2006
 */
//-----------------------------------------------------------------------------

#ifndef RICHUTILS_RICHGEOMFUNCTIONS_H
#define RICHUTILS_RICHGEOMFUNCTIONS_H 1

#include <cmath>

// Gaudi
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// VDT
#include "vdt/asin.h"

namespace Rich
{

  /** @namespace Rich::Geom
   *
   *  Namespace for a collection of geomtrical utilities, such as useful functions 
   *  not available in ROOT's MatchCore or MathMore
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   20/01/2006
   */
  namespace Geom
  {

    /** Calculates the angle between two vectors
     *
     *  @param a First vector
     *  @param b Second vector
     *
     *  @return Angle between vectors (in rad)
     */
    inline double AngleBetween( const Gaudi::XYZVector & a,
                                const Gaudi::XYZVector & b )
    {
      const auto demon   = std::sqrt( a.Mag2() * b.Mag2() );
      const auto dotprod = a.Dot(b);
      //return ( demon>0 && (dotprod/demon)<1 ? std::acos(dotprod/demon) : 0.0 );
      return ( demon>0 && (dotprod/demon)<1 ? vdt::fast_acos(dotprod/demon) : 0.0 );
    }

  }

}

#endif // RICHUTILS_RICHGEOMFUNCTIONS_H
