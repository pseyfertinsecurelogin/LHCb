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
/** @file FastAllocList.h
 *
 *  Header file for list class LHCb::FastAllocList
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   29/03/2007
 */
//--------------------------------------------------------------------------------

#ifndef KERNEL_FastAllocList_H
#define KERNEL_FastAllocList_H 1

// STL
#include <list>
#include <ostream>

#ifndef GOD_NOALLOC
// Memory pool allocator from boost
#  include "GaudiKernel/boost_allocator.h"
#  include <boost/pool/pool_alloc.hpp>
// STL MT allocator
#  include <ext/mt_allocator.h>
// STL Pool allocator
#  include <ext/pool_allocator.h>
#endif

namespace LHCb {

  //--------------------------------------------------------------------------------
  /** @class FastAllocList Kernel/FastAllocList.h
   *
   *  List with custom allocator
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   23/09/2015
   */
  //--------------------------------------------------------------------------------

  template <typename TYPE, typename ALLOC>
  class FastAllocList : public std::list<TYPE, ALLOC> {

  private:
    /// Shortcut to the base class type
    typedef std::list<TYPE, ALLOC> BaseClass;

  public:
    /// Default constructor
    constexpr FastAllocList() = default;

    /** Constructor with initial size
     *  @param size Initialisation size for list
     */
    constexpr FastAllocList( const typename BaseClass::size_type size ) : BaseClass( size ) {}

    /** Constructor with initial size and initialisation value
     *  @param size Initialisation size for list
     *  @param init Initialisation value
     */
    constexpr FastAllocList( const typename BaseClass::size_type size, const TYPE& init ) : BaseClass( size, init ) {}

  public:
    /// Operator overloading for ostream
    friend inline std::ostream& operator<<( std::ostream& str, const FastAllocList<TYPE, ALLOC>& v ) {
      str << "[ ";
      for ( const auto& i : v ) { str << i << " "; }
      return str << "]";
    }
  };

  namespace Boost {

    //--------------------------------------------------------------------------------
    /** @typedef PoolAllocList Kernel/FastAllocList.h
     *
     *  List with pool allocator from boost
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   23/09/2015
     */
    //--------------------------------------------------------------------------------

#ifndef GOD_NOALLOC
    template <typename TYPE, typename USERALLOC = boost::default_user_allocator_new_delete,
              typename MUTEX = boost::details::pool::default_mutex, unsigned NEXTSIZE = 32>
    using ListPoolAlloc = boost::fast_pool_allocator<TYPE, USERALLOC, MUTEX, NEXTSIZE>;
#else
    template <typename TYPE>
    using ListPoolAlloc = std::allocator<TYPE>;
#endif

    template <typename TYPE, typename ALLOC = ListPoolAlloc<TYPE>>
    using PoolAllocList = FastAllocList<TYPE, ALLOC>;

  } // namespace Boost

  namespace STL {

    //--------------------------------------------------------------------------------
    /** @typedef MTAllocList Kernel/FastAllocList.h
     *
     *  List with MT allocator
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   23/09/2015
     */
    //--------------------------------------------------------------------------------

    template <typename TYPE,
#ifndef GOD_NOALLOC
              typename ALLOC = __gnu_cxx::__mt_alloc<TYPE>
#else
              typename ALLOC = std::allocator<TYPE>
#endif
              >
    using MTAllocList = FastAllocList<TYPE, ALLOC>;

    //--------------------------------------------------------------------------------
    /** @typedef PoolAllocList Kernel/FastAllocList.h
     *
     *  List with STL Pool allocator
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   23/09/2015
     */
    //--------------------------------------------------------------------------------

    template <typename TYPE,
#ifndef GOD_NOALLOC
              typename ALLOC = __gnu_cxx::__pool_alloc<TYPE>
#else
              typename ALLOC = std::allocator<TYPE>
#endif
              >
    using PoolAllocList = FastAllocList<TYPE, ALLOC>;

    //--------------------------------------------------------------------------------
    /** @typedef List Kernel/FastAllocList.h
     *
     *  Standard STL list, with default allocator
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   23/09/2015
     */
    //--------------------------------------------------------------------------------

    template <typename TYPE, typename ALLOC = std::allocator<TYPE>>
    using List = FastAllocList<TYPE, ALLOC>;

  } // namespace STL

} // namespace LHCb

#endif // KERNEL_FastAllocList_H
