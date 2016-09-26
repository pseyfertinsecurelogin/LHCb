
//--------------------------------------------------------------------------------
/** @file RichHashMap.h
 *
 *  Header file for utility hash map for the RICH : Rich::HashMap
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   2005-01-11
 */
//--------------------------------------------------------------------------------

#ifndef RICHUTILS_RICHHASHMAP_H
#define RICHUTILS_RICHHASHMAP_H 1

// GaudiKernel
#include "GaudiKernel/HashMap.h"

// STD
#include <ostream>

// Include here to make sure always use defined hash functions for RichSmartIDs
// should be included from RichSmartID.h head file, but not possible with G.O.D.
#include "Kernel/RichSmartIDHashFuncs.h"

namespace Rich
{

  //--------------------------------------------------------------------------------
  /** @class HashMap RichHashMap.h RichUtils/RichHashMap.h
   *
   *  A utility class providing a templated HashMap for fast
   *  loop up table like access.
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   2005-01-11
   */
  //--------------------------------------------------------------------------------

  template < class KEY, class VALUE >
  class HashMap : public GaudiUtils::HashMap < KEY, VALUE >
  {
  public:
    /// Operator overloading for ostream
    friend inline std::ostream& operator << ( std::ostream& str ,
                                              const HashMap<KEY,VALUE> & m )
    {
      str << "[";
      for ( const auto& i : m ) { str << " (" << i.first << "," << i.second << ")"; }
      return str << " ]";
    }
  };

}

#endif // RICHUTILS_RICHHASHMAP_H