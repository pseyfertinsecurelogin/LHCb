
//-----------------------------------------------------------------------------
/** @file FastMaths.h
 *
 * Implementation of vaious fast mathematical functions for the Rich
 *
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date 02/09/2007
 */
//-----------------------------------------------------------------------------

#pragma once

// STL
#include <cmath>

// Local
#include "RichUtils/RichSIMDTypes.h"

namespace Rich
{

  /** @namespace Rich::SIMD
   *
   *  Namespace for RICH SIMD types and utilities
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   17/10/2017
   */
  namespace SIMD
  {

    namespace Maths
    {

      /** fast atan2 for SIMD types
       *  Based on VDT fast_atan2f */
      template < typename TYPE,
                 typename std::enable_if<!std::is_arithmetic<TYPE>::value>::type * = nullptr >
      inline TYPE fast_atan2f( const TYPE y, const TYPE x )
      {

        // move in first octant
        auto xx = abs(x);
        auto yy = abs(y);
        auto tmp ( TYPE::Zero() );
        auto m = ( yy > xx );
        if ( any_of(m) )
        {
          tmp(m) = yy;
          yy(m)  = xx; 
          xx(m)  = tmp;
          tmp(m) = TYPE::One();
        }

        const auto t = yy / xx;
        auto       z = t;
        m            = t > TYPE(0.4142135623730950);
        if ( any_of(m) ) { z(m) = ( t - TYPE::One() ) / ( t + TYPE::One() ); }

        const auto z2 = z * z;

        auto ret = ((((   TYPE(8.05374449538e-2f)  * z2
                        - TYPE(1.38776856032E-1f)) * z2
                        + TYPE(1.99777106478E-1f)) * z2
                        - TYPE(3.33329491539E-1f)) * z2 * z
                        + z );

        // move back in place
        ret.setZero( xx == TYPE::Zero() ||
                     y  == TYPE::Zero() );
        ret( m                   ) += TYPE(M_PI_4);
        ret( tmp != TYPE::Zero() )  = TYPE(M_PI_2) - ret;
        ret( x    < TYPE::Zero() )  = TYPE(M_PI)   - ret;
        ret( y    < TYPE::Zero() )  =              - ret;

        return ret;
      }
      
    }
  }
}
