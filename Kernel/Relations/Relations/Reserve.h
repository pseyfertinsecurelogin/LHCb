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
#ifndef RELATIONS_RESERVE_H
#define RELATIONS_RESERVE_H 1
// ============================================================================
// include
// ============================================================================
// STD & STL
// ============================================================================
#include <cstddef>
#include <vector>
// ============================================================================

namespace Relations {
  /**
   *  Helper function to reserve the size of container
   *
   *  @warning the implementation  is platform dependent!
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   07/02/2002
   */
  template <class TYPE>
  void reserve( TYPE&, size_t ) {}

  template <class TYPE>
  void reserve( std::vector<TYPE>& object, size_t size ) {
    object.reserve( size );
  }

} // namespace Relations

// ============================================================================
// The End
// ============================================================================
#endif // RELATIONS_RESERVE_H
