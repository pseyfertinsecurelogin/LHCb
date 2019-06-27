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
// ===========================================================================

#pragma once

/// STD & STL includes
#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>

#ifdef __INTEL_COMPILER             // Disable ICC remark from Math headers
#  pragma warning( disable : 1572 ) // Floating-point equality and inequality comparisons are unreliable
#endif

// Geometry includes
#include "GaudiKernel/Plane3DTypes.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// DetDesc includes
#include "DetDesc/ISolid.h"
#include "DetDesc/SolidMath.h"

/** @file
 *
 *  Collection of technical methods for manipulation of
 *  ISolid::Tick ans ISolid::Ticks. Also some additional
 *  mathematical utilities are supplied.
 *
 *  @author      Vanya Belyaev   Ivan.Belyaev@itep.ru
 *  @date        10.02.2000
 */

/** @namespace SolidTicks SolidTicks.h "DetDesc/SolidTicks.h"
 *
 *  SolidTicks - a collection of technical methods for manipulation of
 *  ISolid::Tick ans ISolid::Ticks. Also some additional
 *  mathematical utilities are supplied.
 *
 *  @author      Vanya Belyaev   Ivan.Belyaev@itep.ru
 *  @date        10.02.2000
 */
namespace SolidTicks {

  /** Remove all adjacent ticks
   *  @author      Vanya Belyaev   Ivan.Belyaev@itep.ru
   *  @date        10.02.2000
   *  @see ISolid
   *  @param ticks   container of "ticks" (sorted!)
   *  @param point   point for line perametrisation
   *  @param vect    vector along the line
   *  @param solid   reference to SOLID
   *  @return number of ticks
   */
  template <class SOLID, class aPoint, class aVector>
  inline unsigned int RemoveAdjacent( ISolid::Ticks& ticks, //
                                      const aPoint&  point, //
                                      const aVector& vect,  //
                                      const SOLID&   solid ) {
    unsigned int ret = 0;

    // no adjacent ?
    if ( ticks.size() < 2 ) {
      ticks.clear();
    } else if ( ticks.size() == 2 ) {
      const auto tick1 = ticks.front();           // first tick
      const auto tick2 = ticks.back();            // last  tick
      const auto tick  = 0.5 * ( tick1 + tick2 ); // middle tick
      if ( solid.isInside( point + ( vect * tick ) ) ) {
        ret = 2;
      } else {
        ticks.clear();
      }
    } else {
      // perform removing of adjacent  ticks
      boost::container::static_vector<size_t, ISolid::MaxTicks> tmp;
      //
      bool boolPrev = true;
      bool boolNext = true;
      for ( auto it = ticks.begin(); it != ticks.end(); ++it ) {
        // the last point is to be treated in a specific way
        if ( ticks.end() != it + 1 ) {
          auto tickNext = 0.5 * ( ( *it ) + *( it + 1 ) );
          boolNext      = solid.isInside( point + vect * tickNext );
        }
        // get the index
        const unsigned int index = it - ticks.begin();
        /**  to write the last  tick it is enought
         *   to have the previous interval "inside"
         */
        if ( ticks.end() == it + 1 ) {
          if ( !boolPrev ) { tmp.push_back( index ); }
        }
        /** to write the first tick it is enought
         *  to have the first    interval "inside"
         */
        else if ( ticks.begin() == it ) {
          if ( !boolNext ) { tmp.push_back( index ); }
        }
        /**  to write the "regular" tick, it should
         *   separate 2 different zones!
         */
        else {
          if ( boolPrev == boolNext ) { tmp.push_back( index ); }
        }
        ///
        boolPrev = boolNext;
      }
      // remove ticks (from the end!)
      auto cri = tmp.rbegin();
      while ( cri != tmp.rend() ) { ticks.erase( ticks.begin() + *cri++ ); }
      // get the final answer
      ret = ticks.size();
    }
    // return final number
    return ret;
  }

  /** Sort Ticks, eliminate duplicates and remove all adjacent ticks
   *  @author      Vanya Belyaev   Ivan.Belyaev@itep.ru
   *  @date        10.02.2000
   *  @see ISolid
   *  @param ticks   container of "ticks" (unsorted!)
   *  @param point   point for line perametrisation
   *  @param vect    vector along the line
   *  @param solid   reference to SOLID
   *  @return number of ticks
   */
  template <class SOLID, class aPoint, class aVector>
  inline unsigned int RemoveAdjacentTicks( ISolid::Ticks& ticks, //
                                           const aPoint&  point, //
                                           const aVector& vect,  //
                                           const SOLID&   solid ) {
    //     useful local typedefs
    //    typedef ISolid::Tick            Tick     ;
    //    typedef ISolid::Ticks::iterator iterator ;
    // (1) sort container
    std::sort( ticks.begin(), ticks.end() );
    // (2) eliminate duplicates and (3) shrink container
    ticks.erase( std::unique( ticks.begin(), ticks.end() ), ticks.end() );
    // remove adjacent
    return RemoveAdjacent( ticks, point, vect, solid );
  }

  /** Eliminate duplicates and remove all adjacent ticks,
   *  Assume that "ticks" are already sorted  and
   *  all adjacent ticks are removed!
   *  @author      Vanya Belyaev   Ivan.Belyaev@itep.ru
   *  @date        10.02.2000
   *  @see ISolid
   *  @param ticks   container of "ticks" (sorted!)
   *  @param point   point for line perametrisation
   *  @param vect    vector along the line
   *  @param tickMin minimal value of tick
   *  @param tickMax maximal value of tick
   *  @param solid   reference to SOLID
   *  @return number of ticks
   */
  template <class SOLID, class aPoint, class aVector>
  inline unsigned int RemoveAdjacentTicks( ISolid::Ticks&      ticks,   //
                                           const aPoint&       point,   //
                                           const aVector&      vect,    //
                                           const ISolid::Tick& tickMin, //
                                           const ISolid::Tick& tickMax, //
                                           const SOLID&        solid ) {
    unsigned int ret = 0;

    // valid arguments?
    if ( UNLIKELY( tickMin >= tickMax ) ) {
      ticks.clear();
    } else {

      // remove all garbage what is  less than 'tickMin' or greater than 'tickMax'
      const auto it = std::remove_if( ticks.begin(), ticks.end(),         //
                                      [tickMin, tickMax]( const auto& i ) //
                                      { return ( i < tickMin ) || ( tickMax < i ); } );
      ticks.erase( it, ticks.end() );

      // empty ticks!
      if ( ticks.empty() ) {
        const auto middle = 0.5 * ( tickMin + tickMax );
        if ( solid.isInside( point + middle * vect ) ) {
          ticks.push_back( tickMin );
          ticks.push_back( tickMax );
        }
        ret = ticks.size();
      } else {
        // first tick
        if ( LIKELY( ticks.front() != tickMin ) ) {
          const auto middle = 0.5 * ( tickMin + ticks.front() );
          if ( solid.isInside( point + middle * vect ) ) { ticks.insert( ticks.begin(), tickMin ); }
        }
        // last tick
        if ( LIKELY( ticks.back() != tickMax ) ) {
          const auto middle = 0.5 * ( ticks.back() + tickMax );
          if ( solid.isInside( point + middle * vect ) ) { ticks.push_back( tickMax ); }
        }
        // adjacent are already removed
        ret = ticks.size();
      }
    }
    return ret;
  }

  /** Eliminate duplicate ticks. Not as safe as the original, but a
   *   bit more efficient: it will not call the 'isInside' stuff
   *   unless it has reason to belief there is actually something
   *   wrong with this vector of ticks. It decides that there is
   *   something wrong if there are an odd number of ticks or if there
   *   are 'double' ticks.
   *  @author      Wouter Hulsbergen
   *  @date        20.09.2007
   *  @see ISolid
   *  @param ticks   container of "ticks" (sorted!)
   *  @param point   point for line perametrisation
   *  @param vect    vector along the line
   *  @param solid   reference to SOLID
   *  @return number of ticks
   */
  template <class SOLID, class aPoint, class aVector, class TickContainer>
  inline unsigned int RemoveAdjacentTicksFast( TickContainer& ticks, //
                                               const aPoint&  point, //
                                               const aVector& vect,  //
                                               const SOLID&   solid ) {
    // only call the expensive method if we find that something is wrong:
    const auto newend = std::unique( ticks.begin(), ticks.end() );
    if ( newend != ticks.end() || ticks.size() % 2 != 0 ) {
      ticks.erase( newend, ticks.end() );
      RemoveAdjacent( ticks, point, vect, solid );
    }
    return ticks.size();
  }

  /** Remove or adjust intervals such that they overlap with tick range
   *  Assume that "ticks" are already sorted, come in pairs and
   *  that adjacent ticks are removed.
   *  @author      Wouter Hulsbergen
   *  @date        10.09.2007
   *  @see ISolid
   *  @param ticks   container of "ticks" (sorted and in pairs)
   *  @param tickMin minimal value of tick
   *  @param tickMax maximal value of tick
   *  @return number of ticks
   */
  template <class TickContainer>
  unsigned int adjustToTickRange( TickContainer&      ticks,   //
                                  const ISolid::Tick& tickMin, //
                                  const ISolid::Tick& tickMax ) {
    assert( ticks.size() % 2 == 0 );

    auto r = std::find_if( ticks.rbegin(), ticks.rend(), //
                           [&]( const auto& tick ) { return tick <= tickMax; } )
                 .base();
    if ( std::distance( ticks.begin(), r ) % 2 != 0 ) { *r++ = tickMax; }
    ticks.erase( r, ticks.end() );

    auto l = std::find_if( ticks.begin(), ticks.end(), //
                           [&]( const auto& tick ) { return tick >= tickMin; } );
    if ( std::distance( ticks.begin(), l ) % 2 != 0 ) { *--l = tickMin; }
    ticks.erase( ticks.begin(), l );

    return ticks.size();
  }

} // namespace SolidTicks

// ============================================================================
// The End
// ============================================================================
