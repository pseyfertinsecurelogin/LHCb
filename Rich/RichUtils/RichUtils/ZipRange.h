
#ifndef RICHUTILS_ZipRange_H
#define RICHUTILS_ZipRange_H 1

// STL
#include <cassert>
#include <algorithm>

// Range V3
#include <range/v3/all.hpp>
//#include "range/v3/view/zip.hpp"

namespace Rich
{

  //-----------------------------------------------------------------------------
  /** @namespace Rich::Range
   *
   *  Namespace for RICH Range utities
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   16/10/2016
   */
  //-----------------------------------------------------------------------------

  namespace Ranges
  {

    /// Implementation details
    namespace
    {
      /// Compare sizes of a list of containers
      //template < typename... C >
      //inline bool check_sizes( const C& ... c ) noexcept
      //{ 
      //  const auto sizes = { c.size() ... };
      // return ( sizes.size() < 2 || 
      //           std::mismatch( std::next( std::begin(sizes) ), std::end(sizes),
      //                          std::begin(sizes) ).first == std::end(sizes) );
      //}

      /// Compare sizes of two containers
      template < typename A, typename B >
      inline bool check_sizes( const A& a, const B& b ) noexcept
      {
        return a.size() == b.size();
      }
      
      /// Compare sizes of 3 or more containers
      template < typename A, typename B, typename... C >
      inline bool check_sizes( const A& a, const B& b, const C& ... c ) noexcept
      {
        return ( check_sizes(a,b) && check_sizes(b,c...) );
      }
    }

    /// Zips multiple containers together to form a single range
    template< typename... Args >
    inline decltype(auto) Zip( const Args&... args ) noexcept
    {
      assert( check_sizes( args... ) );
      return ranges::view::zip( args... );
    }

  }
}

#endif // RICHUTILS_ZipRange_H
