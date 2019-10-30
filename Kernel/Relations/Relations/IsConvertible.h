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
#ifndef RELATIONS_ISCONVERTIBLE_H
#define RELATIONS_ISCONVERTIBLE_H 1
// ============================================================================
// Include files
// ============================================================================
// ============================================================================
#include <type_traits>
// ============================================================================
namespace Relations {
  // ==========================================================================
  /** @class IsConvertible IsConvertible.h Relations/IsConvertible.h
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2006-06-11
   */
  template <class T, class U>
  struct IsConvertible {
    constexpr static bool same  = std::is_same_v<T, U>;
    constexpr static bool value = std::is_convertible_v<T, U>;
  };
  // ==========================================================================
} // end of namespace Relations
// ============================================================================
// The END
// ============================================================================
#endif // RELATIONS_ISCONVERTIBLE_H
