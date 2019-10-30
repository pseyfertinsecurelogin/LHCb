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
#ifndef RELATIONS_ObjectTypeTraits_H
#define RELATIONS_ObjectTypeTraits_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <functional>
// ============================================================================
// Boost
// ============================================================================
#include "boost/call_traits.hpp"
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/SmartRef.h"
// ============================================================================
// Relations
// ============================================================================
#include "Relations/IsConvertible.h"
#include "Relations/Pointer.h"
#include "Relations/Reference.h"
// ============================================================================
namespace Relations {
  // ==========================================================================
  /// forward declarations
  // ==========================================================================
  template <class OBJECT>
  class Pointer;
  template <class OBJECT>
  class Reference;
  template <class OBJECT>
  struct Types;
  template <class T1, class T2>
  struct InConvertible;
  // ==========================================================================
  namespace detail {
    // ========================================================================
    template <class T1, class T2>
    struct Conversion;
    // ========================================================================
    template <bool, class TYPE>
    struct _Types {
      using Input  = typename boost::call_traits<TYPE>::param_type;
      using Output = const TYPE&;
      using Inner  = TYPE;
    };
    // ========================================================================
    template <bool value, class TYPE>
    struct _Types<value, TYPE*> {
      using Input  = const TYPE* const;
      using Output = TYPE*;
      using Inner  = Pointer<TYPE>;
    };
    // ========================================================================
    template <bool value, class TYPE>
    struct _Types<value, const TYPE*> {
      using Input  = const TYPE* const;
      using Output = TYPE*;
      using Inner  = Pointer<TYPE>;
    };
    // ========================================================================
    template <bool value, class TYPE>
    struct _Types<value, TYPE&> {
      using Input  = const TYPE&;
      using Output = TYPE&;
      using Inner  = Reference<TYPE>;
    };
    // ========================================================================
    template <bool value, class TYPE>
    struct _Types<value, const TYPE&> {
      using Input  = const TYPE&;
      using Output = const TYPE&;
      using Inner  = Reference<TYPE>;
    };
    // ========================================================================
    template <class TYPE>
    struct _Types<true, TYPE> {
      using Input  = const TYPE* const;
      using Output = TYPE*;
      using Inner  = SmartRef<TYPE>;
    };
    // ========================================================================
    template <class TYPE>
    struct _Types<true, TYPE*> {
      static_assert( sizeof( TYPE ) == 0 );
    };
    // ========================================================================
    template <class TYPE>
    struct _Types<true, TYPE&> {
      static_assert( sizeof( TYPE ) == 0 );
    };
    // ========================================================================
  } // namespace detail
  // ==========================================================================
  template <class TYPE>
  struct Types {
    // ========================================================================
    enum {
      value = Relations::IsConvertible<const TYPE*, const DataObject*>::value ||
              Relations::IsConvertible<const TYPE*, const ContainedObject*>::value
    };
    // ========================================================================
    typedef typename detail::_Types<value, TYPE>::Input  Input;
    typedef typename detail::_Types<value, TYPE>::Output Output;
    typedef typename detail::_Types<value, TYPE>::Inner  Inner;
    // ========================================================================
  };
  // ==========================================================================
  /** @struct ObjectTypeTraits
   *
   *  A helper structure to define the Object Type Traits
   *
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   06/02/2002
   */
  template <class OBJECT>
  struct ObjectTypeTraits {
    // ========================================================================
    /// true type ( almost never used)
    using Type = OBJECT;
    /// 'type'-provider, here it is own type
    using Traits = ObjectTypeTraits<OBJECT>;
    /// 'in'-type
    using Input = typename Types<Type>::Input;
    /// 'out'-type
    using Output = typename Types<Type>::Output;
    /// 'storable' type
    using Inner = typename Types<Type>::Inner;
    /// comparison (strict ordering criteria)
    using Less = std::less<>;
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace Relations
// ============================================================================
//  The End
// ============================================================================
#endif // RELATIONS_ObjectTypeTraits_H
