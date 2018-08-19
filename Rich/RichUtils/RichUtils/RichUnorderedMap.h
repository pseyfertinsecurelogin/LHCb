
//--------------------------------------------------------------------------------
/** @file RichUnorderedMap.h
 *
 *  Header file for utility has map for the RICH : Rich::Map
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   2005-01-11
 */
//--------------------------------------------------------------------------------

#pragma once

// STL
#include <ostream>
#include <unordered_map>

namespace Rich
{

  //--------------------------------------------------------------------------------
  /** @class UnorderedMap RichUtils/RichUnorderedMap.h
   *
   *  A utility class providing a standard std::unordered_map like object.
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   2015-09-22
   */
  //--------------------------------------------------------------------------------

  template < typename KEY,
             typename VALUE,
             typename HASH  = std::hash< KEY >,
             typename EQUAL = std::equal_to< KEY >,
             typename ALLOC = std::allocator< std::pair< const KEY, VALUE > > >
  class UnorderedMap : public std::unordered_map< KEY, VALUE, HASH, EQUAL, ALLOC >
  {
  public:
    /// Operator overloading for ostream
    friend inline std::ostream &
    operator<<( std::ostream &str, const UnorderedMap< KEY, VALUE, HASH, EQUAL, ALLOC > &m )
    {
      str << "[";
      for ( const auto &i : m ) { str << " (" << i.first << "," << i.second << ")"; }
      return str << " ]";
    }
  };

} // namespace Rich
