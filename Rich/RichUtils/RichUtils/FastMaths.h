
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
#include <limits>

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

    namespace details
    {
      // Some SIMD constants
      const FPF     HALF ( 0.5 );                  ///< 1/2
      const FPF TAN_PIO8F( 0.4142135623730950 );   ///< tan(pi/8)
      const FPF     PIO4F( M_PI_4 );               ///< pi/4
      const FPF     PIO2F( M_PI_2 );               ///< pi/2
      const FPF     PIF  ( M_PI );                 ///< pi
      // For exp
      const FPF   LOG2EF ( 1.44269504088896341f ); ///< log2(e)
      const FPF C1F      (  0.693359375f    );
      const FPF C2F      ( -2.12194440e-4f  );
      const FPF C1PC2F   ( C1F + C2F        );
      const FPF PX1expf  ( 1.9875691500E-4f );
      const FPF PX2expf  ( 1.3981999507E-3f );
      const FPF PX3expf  ( 8.3334519073E-3f );
      const FPF PX4expf  ( 4.1665795894E-2f );
      const FPF PX5expf  ( 1.6666665459E-1f );
      const FPF PX6expf  ( 5.0000001201E-1f );
      const FPF MAXLOGF  ( 88.72283905206835f );
      const FPF MINLOGF  ( -88.f );
      const FPF INFF     ( std::numeric_limits<float>::infinity() );

      /// Converts a float to an int
      inline UInt32 float2uint32( const FPF& x )
      {
        //return std::floor(x); // Vc implementation
        union { FPF f; UInt32 i; } vp = { x };
        return vp.i;
      }

      /// Converts an int to a float
      inline FPF uint322float( const UInt32& x )
      {
        union { UInt32 i; FPF f; } vp = { x };
        return vp.f;
      }

      /// floor implementation
      inline FPF fpfloor( const FPF& x )
      {
        auto ret = Vc::simd_cast<Int32>(x);
        ret -= ( float2uint32(x) >> 31 );  
        return Vc::simd_cast<FPF>(ret); 
      }
    }

    namespace Maths
    {

      /** fast exp for Vc::Vector<float> type
       *  Based on VDT fast_exp2f */
      inline FPF fast_expf( const FPF& initial_x )
      {

        FPF x = initial_x;

        // floor() truncates toward -infinity.
        FPF z = details::fpfloor( details::LOG2EF * x + details::HALF );

        x -= z * details::C1PC2F;

        const auto n = Vc::simd_cast<Int32>( z );

        z  = x*details::PX1expf;
        z += details::PX2expf;
        z *= x;
        z += details::PX3expf;
        z *= x;
        z += details::PX4expf;
        z *= x;
        z += details::PX5expf;
        z *= x;
        z += details::PX6expf;
        z *= x*x;
        z += x + FPF::One();

        // multiply by power of 2
        z *= details::uint322float( ( n + Int32(0x7f) ) <<23 );

        z        ( initial_x > details::MAXLOGF ) = details::INFF;
        z.setZero( initial_x < details::MINLOGF );

        return z;
      }

      /** fast atan2 for Vc::Vector<float> type
       *  Based on VDT fast_atan2f */
      inline FPF fast_atan2f( const FPF& y, const FPF& x )
      {

        // move in first octant
        auto xx = abs(x);
        auto yy = abs(y);
        auto tmp ( FPF::Zero() );
        auto m = ( yy > xx );
        if ( any_of(m) )
        {
          tmp(m) = yy;
          yy(m)  = xx; 
          xx(m)  = tmp;
          tmp(m) = FPF::One();
        }

        const auto t = yy / xx;
        auto       z = t;
        m            = t > details::TAN_PIO8F;
        if ( any_of(m) ) { z(m) = ( t - FPF::One() ) / ( t + FPF::One() ); }

        const auto z2 = z * z;

        auto ret = ((((   FPF(8.05374449538e-2f)  * z2
                        - FPF(1.38776856032E-1f)) * z2
                        + FPF(1.99777106478E-1f)) * z2
                        - FPF(3.33329491539E-1f)) * z2 * z
                        + z );

        // move back in place
        ret.setZero( xx == FPF::Zero() ||
                     y  == FPF::Zero() );
        ret( m                  ) += details::PIO4F;
        ret( tmp != FPF::Zero() )  = details::PIO2F - ret;
        ret( x    < FPF::Zero() )  = details::PIF   - ret;
        ret( y    < FPF::Zero() )  =                - ret;

        return ret;
      }
      
    }
  }
}
