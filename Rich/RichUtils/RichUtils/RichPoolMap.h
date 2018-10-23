/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

//--------------------------------------------------------------------------------
/** @file RichPoolMap.h
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

// Traits
#include <type_traits>

#ifndef GOD_NOALLOC
// boost
#include "GaudiKernel/boost_allocator.h"
#include <boost/pool/pool_alloc.hpp>
#endif

namespace Rich
{

  //--------------------------------------------------------------------------------
  /** @class PoolMap RichUtils/RichPoolMap.h
   *
   *  A utility class providing a standard std::map like object using a boost
   *  pool allocator.
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   2005-01-11
   */
  //--------------------------------------------------------------------------------

#ifndef GOD_NOALLOC
  template < typename KEY,
             typename VALUE,
             typename USERALLOC = boost::default_user_allocator_new_delete,
             typename MUTEX     = boost::details::pool::default_mutex,
             unsigned NEXTSIZE  = 32 >
  using MapPoolAlloc =
    boost::fast_pool_allocator< std::pair< const KEY, VALUE >, USERALLOC, MUTEX, NEXTSIZE >;
#else
  template < typename KEY, typename VALUE >
  using MapPoolAlloc = std::allocator< std::pair< const KEY, VALUE > >;
#endif

  template < typename KEY,
             typename VALUE,
             typename COMPARE = std::less< KEY >,
             typename ALLOC   = MapPoolAlloc< KEY, VALUE > >
  class PoolMap : public std::map< KEY, VALUE, COMPARE, ALLOC >
  {
  public:

    /// Operator overloading for ostream
    friend inline std::ostream &operator<<( std::ostream &                               str,
                                            const PoolMap< KEY, VALUE, COMPARE, ALLOC > &m )
    {
      str << "[";
      for ( const auto &i : m ) { str << " (" << i.first << "," << i.second << ")"; }
      return str << " ]";
    }
  };

} // namespace Rich
