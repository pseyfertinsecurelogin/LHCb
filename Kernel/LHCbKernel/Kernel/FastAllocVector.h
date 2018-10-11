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
/** @file FastAllocVector.h
 *
 *  Header file for vector class LHCb::FastAllocVector
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   29/03/2007
 */
//--------------------------------------------------------------------------------

#ifndef KERNEL_FastAllocVector_H
#define KERNEL_FastAllocVector_H 1

// STL
#include <vector>
#include <ostream>

#ifndef GOD_NOALLOC
// Memory pool allocator from boost
#include "GaudiKernel/boost_allocator.h"
#include <boost/pool/pool_alloc.hpp>
// STL MT allocator
#include <ext/mt_allocator.h>
// STL Pool allocator
#include <ext/pool_allocator.h>
#endif

namespace LHCb
{

  //--------------------------------------------------------------------------------
  /** @class FastAllocVector Kernel/FastAllocVector.h
   *
   *  Vector with custom allocator
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   23/09/2015
   */
  //--------------------------------------------------------------------------------

  template < typename TYPE, typename ALLOC >
  class FastAllocVector : public std::vector< TYPE, ALLOC >
  {

  private:

    /// Shortcut to the base class type
    typedef std::vector< TYPE, ALLOC > BaseClass;

  public:

    /// Default constructor
    constexpr FastAllocVector( ) { }

    /** Constructor with initial size
     *  @param size Initialisation size for vector
     */
    constexpr FastAllocVector( const typename BaseClass::size_type size )
      : BaseClass(size) { }

    /** Constructor with initial size and initialisation value
     *  @param size Initialisation size for vector
     *  @param init Initialisation value
     */
    constexpr FastAllocVector( const typename BaseClass::size_type size,
                               const TYPE & init )
      : BaseClass(size,init) { }

  public:

    /// Operator overloading for ostream
    friend inline std::ostream& operator << ( std::ostream& str ,
                                              const FastAllocVector<TYPE,ALLOC> & v )
    {
      str << "[ ";
      for ( const auto& i : v ) { str << i << " "; }
      return str << "]";
    }

  };

  namespace Boost
  {

    //--------------------------------------------------------------------------------
    /** @typedef PoolAllocVector Kernel/FastAllocVector.h
     *
     *  Vector with pool allocator from boost
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   23/09/2015
     */
    //--------------------------------------------------------------------------------

#ifndef GOD_NOALLOC
    template < typename TYPE,
               typename USERALLOC = boost::default_user_allocator_new_delete,
               typename MUTEX     = boost::details::pool::default_mutex,
               unsigned NEXTSIZE  = 32 >
    using VecPoolAlloc = boost::pool_allocator< TYPE, USERALLOC, MUTEX, NEXTSIZE >;
#else
    template < typename TYPE >
    using VecPoolAlloc = std::allocator< TYPE >;
#endif

    template < typename TYPE,
               typename ALLOC = VecPoolAlloc< TYPE > >
    using PoolAllocVector = FastAllocVector< TYPE, ALLOC >;

  }

  namespace STL
  {

    //--------------------------------------------------------------------------------
    /** @typedef MTAllocVector Kernel/FastAllocVector.h
     *
     *  Vector with MT allocator
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   23/09/2015
     */
    //--------------------------------------------------------------------------------

    template < typename TYPE,
#ifndef GOD_NOALLOC
               typename ALLOC = __gnu_cxx::__mt_alloc< TYPE >
#else
               typename ALLOC = std::allocator< TYPE >
#endif
               >
    using MTAllocVector = FastAllocVector< TYPE, ALLOC >;

    //--------------------------------------------------------------------------------
    /** @typedef PoolAllocVector Kernel/FastAllocVector.h
     *
     *  Vector with STL Pool allocator
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   23/09/2015
     */
    //--------------------------------------------------------------------------------

    template < typename TYPE,
#ifndef GOD_NOALLOC
               typename ALLOC = __gnu_cxx::__pool_alloc< TYPE >
#else
               typename ALLOC = std::allocator< TYPE >
#endif
               >
    using PoolAllocVector = FastAllocVector< TYPE, ALLOC >;

    //--------------------------------------------------------------------------------
    /** @typedef Vector Kernel/FastAllocVector.h
     *
     *  Standard STL vector, with default allocator
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   23/09/2015
     */
    //--------------------------------------------------------------------------------

    template < typename TYPE,
               typename ALLOC = std::allocator< TYPE > >
    using Vector = FastAllocVector< TYPE, ALLOC >;

  }

}

#endif // KERNEL_FastAllocVector_H
