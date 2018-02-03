
#pragma once

// Gaudi
#include "GaudiKernel/Kernel.h"

// STL
#include <iostream>
#include <cmath>
#include <type_traits>

namespace Rich
{

  /** @namespace RayTracingUtils RichUtils/RayTracingUtils.h
   *
   *  Utility namespace providing basic ray tracing methods for
   *  intersecting and reflecting photons (line) off a sphere and plane.
   *
   *  Provides SIMD vectorised versions.
   *
   *  In part based on http://www.realtimerendering.com/int/
   *
   *  @author Chris Jones
   *  @date   2016-03-02
   */
  namespace RayTracingUtils
  {

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
     *  @return Boolean indicating the status of the ray tracing
     *  @retval true  Ray tracing was successful
     *  @retval false Ray tracing was unsuccessful
     */
    template < typename POSITION, typename DIRECTION, typename COC, typename FTYPE,
               typename = typename std::enable_if< !std::is_arithmetic<typename  POSITION::Scalar>::value && 
                                                   !std::is_arithmetic<typename DIRECTION::Scalar>::value && 
                                                   !std::is_arithmetic<typename       COC::Scalar>::value && 
                                                   !std::is_arithmetic<FTYPE>::value >::type >
    inline decltype(auto) intersectSpherical ( const POSITION& position,
                                               const DIRECTION& direction,
                                               const COC& CoC,
                                               const FTYPE radius,
                                               POSITION& intersection )
    {
      const FTYPE two(2.0), four(4.0), half(0.5);
      // for line sphere intersection look at http://www.realtimerendering.com/int/
      const FTYPE      a = direction.Mag2();
      const auto   delta = position - CoC;
      const FTYPE      b = two * direction.Dot( delta );
      const FTYPE      c = delta.Mag2() - radius*radius;
      FTYPE        discr = b*b - four*a*c;
      // Zero out the negative values in discr, to prevent sqrt(-ve)
      const auto OK = discr > FTYPE::Zero();
      discr.setZeroInverted(OK);
      // distance
      const auto dist = half * ( sqrt(discr) - b ) / a;
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
     *  @return Boolean indicating if the ray tracing was succesful
     *  @retval true  Ray tracing was successful
     *  @retval false Ray tracing was unsuccessful
     */
    template < typename POSITION, typename DIRECTION, typename COC, typename FTYPE,
               typename = typename std::enable_if< !std::is_arithmetic<typename  POSITION::Scalar>::value && 
                                                   !std::is_arithmetic<typename DIRECTION::Scalar>::value && 
                                                   !std::is_arithmetic<typename       COC::Scalar>::value && 
                                                   !std::is_arithmetic<FTYPE>::value >::type > 
    inline decltype(auto) reflectSpherical ( POSITION& position,
                                             DIRECTION& direction,
                                             const COC& CoC,
                                             const FTYPE radius )
    {
      const FTYPE two(2.0), four(4.0), half(0.5);
      const FTYPE      a = direction.Mag2();
      const auto   delta = position - CoC;
      const FTYPE      b = two * direction.Dot( delta );
      const FTYPE      c = delta.Mag2() - radius*radius;
      FTYPE        discr = b*b - four*a*c;
      // Zero out the negative values in discr, to prevent sqrt(-ve)
      const auto OK = discr > FTYPE::Zero();
      discr.setZeroInverted(OK);
      // compute the distance
      const auto dist = half * ( sqrt(discr) - b ) / a;
      // change position to the intersection point
      position += dist * direction;
      // reflect the vector
      // r = u - 2(u.n)n, r=reflection, u=incident, n=normal
      const auto normal = position - CoC;
      direction -= ( two * normal.Dot(direction) / normal.Mag2() ) * normal;
      // return the mask indicating which results should be used
      return OK;
    }

    /** Intersect a given direction, from a given point, with a given plane.
     *
     *  @param[in]  position      The start point to use for the ray tracing
     *  @param[in]  direction     The direction to ray trace from the start point
     *  @param[in]  plane         The plane to intersect
     *  @param[out] intersection  The intersection point of the direction with the plane
     *
     *  @return Boolean indicating the status of the ray tracing
     *  @retval true  Ray tracing was successful
     *  @retval false Ray tracing was unsuccessful
     */
    template < typename POINT, typename VECTOR, typename PLANE,
               typename FTYPE = typename POINT::Scalar,
               typename = typename std::enable_if< !std::is_arithmetic<typename POINT::Scalar>::value && 
                                                   !std::is_arithmetic<typename VECTOR::Scalar>::value && 
                                                   !std::is_arithmetic<typename PLANE::Scalar>::value >::type >
    inline decltype(auto) intersectPlane ( const POINT& position,
                                           const VECTOR& direction,
                                           const PLANE& plane,
                                           POINT& intersection )
    {
      const typename FTYPE::mask_type OK(true);
      const FTYPE scalar   = direction.Dot( plane.Normal() );
      const FTYPE distance = -(plane.Distance(position)) / scalar;
      intersection = position + (distance*direction);
      return OK;
    }

    /** Ray trace from given position in given direction off flat mirrors
     *
     *  @param[in,out] position  On input the start point. 
     *                           On output the reflection point
     *  @param[in,out] direction On input the starting direction. 
     *                           On output the reflected direction.
     *  @param[in]     plane     The plane to reflect off
     *
     *  @return Boolean indicating if the ray tracing was succesful
     *  @retval true  Ray tracing was successful
     *  @retval false Ray tracing was unsuccessful
     */
    template < typename POINT, typename VECTOR, typename PLANE,
               typename FTYPE = typename POINT::Scalar,
               typename = typename std::enable_if< !std::is_arithmetic<typename POINT::Scalar>::value && 
                                                   !std::is_arithmetic<typename VECTOR::Scalar>::value && 
                                                   !std::is_arithmetic<typename PLANE::Scalar>::value >::type >
    inline decltype(auto) reflectPlane ( POINT& position,
                                         VECTOR& direction,
                                         const PLANE& plane )
    {
      const typename POINT::Scalar two(2.0);
      const typename FTYPE::mask_type OK(true);
      // Plane normal
      const VECTOR normal  = plane.Normal();
      // compute distance to the plane
      const FTYPE scalar   = direction.Dot(normal);
      const FTYPE distance = -(plane.Distance(position)) / scalar;
      // change position to reflection point and update direction
      position  += distance * direction;
      direction -= two * scalar * normal;
      return OK;
    }

  }

}
