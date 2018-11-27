/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

//-----------------------------------------------------------------------------
/** @file RichGeomFunctions.h
 *
 *  Header file for RICH geometrical utility functions
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   20/01/2006
 */
//-----------------------------------------------------------------------------

#pragma once

// STL
#include <cmath>

// LHCbMath
#include "LHCbMath/FastMaths.h"

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
     *  @return Angle between vectors (in radians)
     */
    template < typename VECT >
    inline decltype( auto ) AngleBetween( const VECT &a, const VECT &b )
    {
      const auto dd  = a.Mag2() * b.Mag2();
      const auto arg = ( dd > 0 ? a.Dot( b ) * vdt::fast_isqrt( dd ) : 999.9 );
      return ( fabs( arg ) < 1 ? LHCb::Math::fast_acos( arg ) : 0.0 );
    }

  } // namespace Geom

} // namespace Rich
