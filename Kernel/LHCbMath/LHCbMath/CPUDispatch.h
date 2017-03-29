
#pragma once

// STL
#include <string>
#include <iterator>
#include <algorithm>

// Gaudi
#include "GaudiKernel/GaudiException.h"

// Vector Class
#include "VectorClass/instrset.h"

namespace LHCb
{

  /// Namespace for CPU dispatch helper methods
  namespace CPU
  {
    
    /// Enum for CPU capabilities
    enum ID { GENERIC = 0, SSE3 = 3, SSE4 = 6, AVX = 7, AVX2 = 8, UNKNOWN = 999999 };
    
    /** @brief Method to set a dispatch function from a list of options based on CPU ID.
     *
     *  Expects to be passed an initializer list of the form of list of std::pairs where the
     *  first element is the CPU ID enum and the second element is the dispatch function
     *  to use for that instruction set.
     *
     *  Entries in the initializer list must be sorted in order of decreasing CPU ID 
     *  (i.e. fastest first). A check is applied to enforce this.
     *
     *  @return The selected dispatch function.
     */
    template< typename Vtbl >
    decltype(auto) dispatch( const Vtbl& vtbl )
    {
      // make sure list is sorted in strict decreasing CPU ID
      // needed for the following search to find the right implementation
      {
        ID lastID = UNKNOWN;
        for ( const auto & i : vtbl ) 
        {
          if ( i.first >= lastID ) 
          {
            throw GaudiException( "Dispatch table must be strictly in decreasing ID order",
                                  "Rich::CPU::dispatch", StatusCode::FAILURE );
          }
          lastID = i.first;
        }
      }
      
      // Get supported instruction set
      const auto level = instrset_detect();
      
      // find pointer to the appropriate version
      const auto impl = std::find_if( std::begin(vtbl), std::end(vtbl),
                                      [&level]( const auto j )
                                      { return level >= j.first; } );
      if ( impl == std::end(vtbl) )
      {
        throw GaudiException( "No implementation for instruction set level " + std::to_string(level),
                              "Rich::CPU::dispatch", StatusCode::FAILURE );
      }
      
      // return the impl
      return impl->second;
    }
    
  }
}