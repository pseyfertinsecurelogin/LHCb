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

#pragma once

// Gaudi
#include "GaudiKernel/Kernel.h"

// STL
#include <cmath>
#include <iostream>
#include <type_traits>

namespace Rich {

  /** @namespace RayTracingUtils RichUtils/RayTracingUtils.h
   *
   *  Utility namespace providing basic ray tracing methods for
   *  intersecting and reflecting photons (line) off a sphere and plane.
   *
   *  In part based on http://www.realtimerendering.com/int/
   *
   *  @author Chris Jones
   *  @date   2016-03-02
   */
  namespace RayTracingUtils {

    /** Intersect a given direction, from a given point, with a given spherical shell.
     *
     *  @attention This method is specifically optimised for a spherical shell from
     *             the inside out, as is required for the RICH mirrors. It will *not*
     *             give the correct results if used in the other direction...
     *
     *  @param[in]  position      The start point to use for the ray tracing
     *  @param[in]  direction     The direction to ray trace from the start point
     *  @param[in] CoC            The centre of curvature of the spherical mirror
     *  @param[in] radius         The radius of curvature of the spherical mirror
     *  @param[out] intersection  The intersection point of the direction with the plane
     *
     *  @return Boolean/mask indicating the status of the ray tracing
     *  @retval true  Ray tracing was successful
     *  @retval false Ray tracing was unsuccessful
     */
    template <typename POSITION, typename DIRECTION, typename COC, typename FTYPE>
    inline decltype( auto )                               //
        __attribute__( ( always_inline ) )                //
        intersectSpherical( const POSITION&  position,    //
                            const DIRECTION& direction,   //
                            const COC&       CoC,         //
                            const FTYPE      radius,      //
                            POSITION&        intersection //
                            ) noexcept {
      const FTYPE zero( 0.0f ), two( 2.0f );
      // for line sphere intersection look at http://www.realtimerendering.com/int/
      const FTYPE a     = direction.Mag2();
      const auto  delta = position - CoC;
      const FTYPE b     = two * direction.Dot( delta );
      const FTYPE c     = delta.Mag2() - radius * radius;
      FTYPE       discr = ( b * b ) - ( FTYPE( 4.0f ) * a * c );
      const auto  OK    = discr > zero;
      // Zero out the negative values in discr, to prevent sqrt(-ve)
      if constexpr ( std::is_arithmetic<typename POSITION::Scalar>::value &&  //
                     std::is_arithmetic<typename DIRECTION::Scalar>::value && //
                     std::is_arithmetic<typename COC::Scalar>::value &&       //
                     std::is_arithmetic<FTYPE>::value ) {
        // scalar
        if ( UNLIKELY( !OK ) ) { discr = zero; }
      } else {
        // SIMD
        if ( UNLIKELY( !all_of( OK ) ) ) { discr.setZeroInverted( OK ); }
      }
      // distance
      const auto dist = FTYPE( 0.5f ) * ( std::sqrt( discr ) - b ) / a;
      // set intersection point
      intersection = position + ( dist * direction );
      // return the mask
      return OK;
    }

    /** Reflect a given direction off a spherical mirror. Can be used for intersection.
     *
     *  @attention This method is specifically optimised for a spherical shell from
     *             the inside out, as is required for the RICH mirrors. It will *not*
     *             give the correct results if used in the other direction...
     *
     *  @param[in,out] position   The start point to use for the ray tracing.
     *                            Afterwards gives the reflection point on the
     *                            spherical mirror.
     *  @param[in,out] direction  The direction to ray trace from the start point.
     *                            Afterwards represents the reflection direction
     *                            from the spherical mirror.
     *  @param[in] CoC        The centre of curvature of the spherical mirror
     *  @param[in] radius     The radius of curvature of the spherical mirror
     *
     *  @return Boolean/mask indicating if the ray tracing was succesful
     *  @retval true  Ray tracing was successful
     *  @retval false Ray tracing was unsuccessful
     */
    template <typename POSITION, typename DIRECTION, typename COC, typename FTYPE>
    inline decltype( auto )                      //
        __attribute__( ( always_inline ) )       //
        reflectSpherical( POSITION&   position,  //
                          DIRECTION&  direction, //
                          const COC&  CoC,       //
                          const FTYPE radius     //
                          ) noexcept {
      // constants
      const FTYPE zero( 0.0f ), two( 2.0f );
      // for line sphere intersection look at http://www.realtimerendering.com/int/
      const FTYPE a     = direction.Mag2();
      const auto  delta = position - CoC;
      const FTYPE b     = two * direction.Dot( delta );
      const FTYPE c     = delta.Mag2() - ( radius * radius );
      FTYPE       discr = ( b * b ) - ( FTYPE( 4.0f ) * a * c );
      const auto  OK    = discr > zero;
      if constexpr ( std::is_arithmetic<typename POSITION::Scalar>::value &&  //
                     std::is_arithmetic<typename DIRECTION::Scalar>::value && //
                     std::is_arithmetic<typename COC::Scalar>::value &&       //
                     std::is_arithmetic<FTYPE>::value ) {
        // scalar
        if ( UNLIKELY( !OK ) ) { discr = zero; }
      } else {
        // SIMD
        // Zero out the negative values in discr, to prevent sqrt(-ve)
        if ( UNLIKELY( !all_of( OK ) ) ) { discr.setZeroInverted( OK ); }
      }
      // compute the distance
      const auto dist = FTYPE( 0.5f ) * ( std::sqrt( discr ) - b ) / a;
      // change position to the intersection point
      position += dist * direction;
      // reflect the vector
      // r = u - 2(u.n)n, r=reflection, u=incident, n=normal
      const auto normal = position - CoC;
      direction -= ( two * normal.Dot( direction ) / normal.Mag2() ) * normal;
      // return the mask indicating which results should be used
      return OK;
    }

    /** Intersect a given direction, from a given point, with a given plane.
     *
     *  @param[in]  position      The start point to use for the ray tracing
     *  @param[in]  direction     The direction to ray trace from the start point
     *  @param[in]  plane         The plane to intersect
     *  @param[out] intersection  The intersection point of the direction with the plane
     */
    template <typename POINT, typename VECTOR, typename PLANE, //
              typename FTYPE = typename POINT::Scalar>
    inline void                                    //
        __attribute__( ( always_inline ) )         //
        intersectPlane( const POINT&  position,    //
                        const VECTOR& direction,   //
                        const PLANE&  plane,       //
                        POINT&        intersection //
                        ) noexcept {
      // compute -1*distance to the plane
      const FTYPE scalar   = direction.Dot( plane.Normal() );
      const FTYPE distance = plane.Distance( position ) / scalar;
      // compute plane intersection
      intersection = position - ( distance * direction );
    }

    /** Ray trace from given position in given direction off flat mirrors
     *
     *  @param[in,out] position  On input the start point.
     *                           On output the reflection point
     *  @param[in,out] direction On input the starting direction.
     *                           On output the reflected direction.
     *  @param[in]     plane     The plane to reflect off
     */
    template <typename POINT, typename VECTOR, typename PLANE, //
              typename FTYPE = typename POINT::Scalar>
    inline void                               //
        __attribute__( ( always_inline ) )    //
        reflectPlane( POINT&       position,  //
                      VECTOR&      direction, //
                      const PLANE& plane      //
                      ) noexcept {
      // plane normal
      const VECTOR normal( plane.Normal() );
      // compute -1*distance to the plane
      const FTYPE scalar   = direction.Dot( normal );
      const FTYPE distance = plane.Distance( position ) / scalar;
      // change position to reflection point and update direction
      position -= distance * direction;
      direction -= FTYPE( 2.0f ) * scalar * normal;
    }

  } // namespace RayTracingUtils

} // namespace Rich
