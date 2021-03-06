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
#ifndef LOKI_L0TYPES_H
#define LOKI_L0TYPES_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/CoreTypes.h"
#include "LoKi/Functions.h"
// ============================================================================
// forward declarations
// ============================================================================
namespace LHCb {
  class L0DUReport;
}
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @class L0Types LoKi/L0Types.h
   *
   *  The basic types defined for LoKiHlt package
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date   2008-09-18
   */
  namespace L0Types {
    // ========================================================================
    // L0
    // ========================================================================
    /// type of 'cuts' for LHCb::L0DUReport (interface)
    typedef LoKi::BasicFunctors<const LHCb::L0DUReport*>::Predicate L0_Cuts;
    // ========================================================================
    /// type of 'functions' for LHCb::L0DUReport (interface)
    typedef LoKi::BasicFunctors<const LHCb::L0DUReport*>::Function L0_Func;
    // ========================================================================
    /// type of 'cuts' for LHCb::L0DUReport      (assignable)
    typedef LoKi::BasicFunctors<const LHCb::L0DUReport*>::PredicateFromPredicate L0_Cut;
    // ========================================================================
    /// type of 'functions' for LHCb::L0DUReport (assignable)
    typedef LoKi::BasicFunctors<const LHCb::L0DUReport*>::FunctionFromFunction L0_Fun;
    // ========================================================================
  } // namespace L0Types
  // ==========================================================================
  namespace Types {
    // ========================================================================
    // L0
    // ========================================================================
    /// type of 'cuts' for LHCb::L0DUReport      (interface)
    typedef LoKi::L0Types::L0_Cuts L0_Cuts;
    // ========================================================================
    /// type of 'functions' LHCb::L0DUReport     (interface)
    typedef LoKi::L0Types::L0_Func L0_Func;
    // ========================================================================
    /// type of 'cuts' for LHCb::L0DUReport      (assignable)
    typedef LoKi::L0Types::L0_Cut L0_Cut;
    // ========================================================================
    /// type of 'functions' for LHCb::L0DUReport (assignable)
    typedef LoKi::L0Types::L0_Fun L0_Fun;
    // ========================================================================
  } // namespace Types
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
namespace LoKi {
  // ==========================================================================
  // the specialized printout
  // ==========================================================================
  template <>
  inline std::ostream& Constant<const LHCb::L0DUReport*, bool>::fillStream( std::ostream& s ) const {
    return s << ( this->m_value ? "L0_ALL" : "L0_NONE" );
  }
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_L0TYPES_H
