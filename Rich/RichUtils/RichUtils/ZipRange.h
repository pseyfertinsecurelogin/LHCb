
#ifndef RICHUTILS_ZipRange_H
#define RICHUTILS_ZipRange_H 1

// STL
#include <cassert>

// Boost implementation
//#include "boost/iterator/zip_iterator.hpp"

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

    // namespace Boost
    // {
    //   /// Zips multiple containers together to form a single range
    //   template<class... Conts>
    //   decltype(auto) ZipRange( Conts&... conts )
    //   {
    //     return boost::make_iterator_range
    //       ( boost::make_zip_iterator(boost::make_tuple(conts.begin()...)),
    //         boost::make_zip_iterator(boost::make_tuple(conts.end()...  )) );
    //   }
    // }

  }
}

#endif // RICHUTILS_ZipRange_H
