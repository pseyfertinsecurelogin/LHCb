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
#ifndef LOKI_SAME_H
#define LOKI_SAME_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/SmartRef.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Reference.h"
// ============================================================================
namespace LoKi {
  // ==========================================================================
  template <class TYPE>
  bool same( const TYPE* o1, const TYPE* o2 ) {
    return o1 == o2;
  }
  // ==========================================================================
  template <class TYPE>
  bool same( const TYPE* o1, const SmartRef<TYPE>& o2 ) {
    return o1 == o2;
  }
  // ==========================================================================
  template <class TYPE>
  bool same( const SmartRef<TYPE>& o1, const TYPE* o2 ) {
    return o1 == o2;
  }
  // ==========================================================================
  template <class TYPE>
  bool same( const SmartRef<TYPE>& o1, const SmartRef<TYPE>& o2 ) {
    return o1 == o2.target();
  }
  // ==========================================================================
  template <class TYPE>
  bool same( const LoKi::Reference<TYPE>& o1, const TYPE& o2 ) {
    return o1 == o2;
  }
  // ==========================================================================
  template <class TYPE>
  bool same( const TYPE& o1, const LoKi::Reference<TYPE>& o2 ) {
    return o2 == o1;
  }
  // ==========================================================================
  template <class TYPE>
  bool same( const LoKi::Reference<TYPE>& o1, const LoKi::Reference<TYPE>& o2 ) {
    return o1 == o2;
  }
  // ==========================================================================
  template <class TYPE>
  bool same( const TYPE& o1, const TYPE& o2 ) {
    return same( &o1, &o2 );
  }
  // ==========================================================================
} // namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_SAME_H
