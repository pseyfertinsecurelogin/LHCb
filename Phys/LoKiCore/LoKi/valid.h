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
// ============================================================================
#ifndef LOKI_VALID_H
#define LOKI_VALID_H 1
// ============================================================================
// GaudiKernel
// ============================================================================
// STD & STL
// ============================================================================
#include <vector>
// ============================================================================
#include "GaudiKernel/SmartRef.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Holder.h"
#include "LoKi/Interface.h"
// ============================================================================
namespace LoKi {
  // ==========================================================================
  template <class TYPE1, class TYPE2>
  bool valid( const std::pair<TYPE1, TYPE2>& p );
  // ==========================================================================
  template <class TYPE1, class TYPE2>
  bool valid( const LoKi::Holder<TYPE1, TYPE2>& p );
  // ==========================================================================
  template <class TYPE, class ALLOCATOR>
  inline bool valid( const std::vector<TYPE, ALLOCATOR>& p );
  // ==========================================================================
  template <class TYPE>
  inline bool valid( const TYPE* ptr ) {
    return 0 != ptr;
  }
  template <class TYPE>
  inline bool valid( TYPE* ptr ) {
    return 0 != ptr;
  }
  // ==========================================================================
  template <class TYPE>
  inline bool valid( const TYPE*& ptr ) {
    return 0 != ptr;
  }
  // ==========================================================================
  template <class TYPE>
  inline bool valid( TYPE*& ptr ) {
    return 0 != ptr;
  }
  // ==========================================================================
  template <class TYPE>
  inline bool valid( const SmartRef<TYPE>& ptr ) {
    return ptr;
  }
  // ==========================================================================
  template <class TYPE>
  inline bool valid( const LoKi::Reference<TYPE>& /* ptr */ ) {
    return true;
  }
  // ==========================================================================
  template <class TYPE>
  inline bool valid( const LoKi::Interface<TYPE>& ptr ) {
    return ptr.validPointer();
  }
  // ==========================================================================
  template <class TYPE, class ALLOCATOR>
  inline bool valid( const std::vector<TYPE, ALLOCATOR>& p ) {
    return !p.empty();
  }
  // ==========================================================================
  template <class TYPE1, class TYPE2>
  bool valid( const std::pair<TYPE1, TYPE2>& p ) {
    return valid( p.first ) && valid( p.second );
  }
  // ==========================================================================
  template <class TYPE1, class TYPE2>
  bool valid( const LoKi::Holder<TYPE1, TYPE2>& p ) {
    return valid( p.first ) && valid( p.second );
  }
  // ==========================================================================
  template <class TYPE>
  inline bool valid( const TYPE& /* ref */ ) {
    return true;
  }
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_VALID_H
