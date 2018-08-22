
//-----------------------------------------------------------------------------
/** @file RichSmartIDSorter.h
 *
 *  Header file for RichSmartID utility class : RichSmartIDSorter
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2005-01-10
 */
//-----------------------------------------------------------------------------

#pragma once

// STL
#include <algorithm>

// Kernel
#include "Kernel/RichSmartID.h"

namespace Rich
{

  //-----------------------------------------------------------------------------
  /** @class SmartIDSorter RichSmartIDSorter.h RichUtils/RichSmartIDSorter.h
   *
   *  Utility class to sort RichSmartIDs according to various criteria
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   2005-01-10
   */
  //-----------------------------------------------------------------------------

  class SmartIDSorter final
  {

  public:

    /// Sort the list by detector region
    inline static void sortByRegion( LHCb::RichSmartID::Vector &smartIDs )
    {
      std::sort( smartIDs.begin(), smartIDs.end(), []( const auto p1, const auto p2 ) {
        // use full class, including data set bits
        // return p1.key() < p2.key();
        // Use internal bit packing to sort
        return p1.dataBitsOnly().key() < p2.dataBitsOnly().key();
      } );
    }
  };

} // namespace Rich
