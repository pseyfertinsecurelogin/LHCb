
#ifndef RICHUTILS_ZipRange_H
#define RICHUTILS_ZipRange_H 1

// Gaudi
#include "GaudiAlg/FunctionalDetails.h"

// Range V3
#include <range/v3/all.hpp>

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

    /// Zips multiple containers together to form a single range
    template< typename... Args >
    inline decltype(auto) Zip( Args&&... args )
    {
      return Gaudi::Functional::details::zip::range( std::forward<Args>(args)... );
    }

    /// Zips multiple containers together to form a single const range
    template< typename... Args >
    inline decltype(auto) ConstZip( Args&&... args )
    {
      return Gaudi::Functional::details::zip::const_range( std::forward<Args>(args)... );
    }

  }
}

#endif // RICHUTILS_ZipRange_H
