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

//--------------------------------------------------------------------------------
/** @file RichMap.h
 *
 *  Header file for utility has map for the RICH : Rich::Map
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   2005-01-11
 */
//--------------------------------------------------------------------------------

#pragma once

// STL
#include <map>
#include <ostream>

namespace Rich
{

  //--------------------------------------------------------------------------------
  /** @class Map RichMap.h RichUtils/RichMap.h
   *
   *  A utility class providing a standard std::map like object.
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   2005-01-11
   */
  //--------------------------------------------------------------------------------

  template < typename KEY,
             typename VALUE,
             typename COMPARE = std::less< KEY >,
             typename ALLOC   = std::allocator< std::pair< const KEY, VALUE > > >
  class Map : public std::map< KEY, VALUE, COMPARE, ALLOC >
  {
  public:

    /// Operator overloading for ostream
    friend inline std::ostream &operator<<( std::ostream &                           str,
                                            const Map< KEY, VALUE, COMPARE, ALLOC > &m )
    {
      str << "[";
      for ( const auto &i : m ) { str << " (" << i.first << "," << i.second << ")"; }
      return str << " ]";
    }
  };

} // namespace Rich
