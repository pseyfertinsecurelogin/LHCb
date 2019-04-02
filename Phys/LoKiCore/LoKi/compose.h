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
#ifndef LOKI_COMPOSE_H
#define LOKI_COMPOSE_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Primitives.h"
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** compose two functions (with rather liberal interfaces)
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-29
   */
  template <typename F1, typename F2>
  auto compose( F1&& fun1, F2&& fun2 ) {
    return V2::Compose{std::forward<F1>( fun1 ), std::forward<F2>( fun2 )};
  }
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_COMPOSE_H
