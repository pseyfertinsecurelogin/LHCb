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
#ifndef LOKI_PIDOPS_H
#define LOKI_PIDOPS_H 1
// ============================================================================
// Include files
// ============================================================================
// Kernel
// ============================================================================
#include "Kernel/ParticleID.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BasicFunctors.h"
#include "LoKi/Functions.h"
#include "LoKi/Operators.h"
// ============================================================================
namespace LoKi {
  // ==========================================================================
  namespace Dicts {
    // ========================================================================
    /** @class PIDOps PIDOps.h LoKi/PIDOps.h
     *  @author Vanya BELYAEV
     *  @date   2007-05-29
     */
    template <class FUNCTION>
    class PIDOps {
    private:
      // ======================================================================
      typedef FUNCTION                                                       Function;
      typedef typename FUNCTION::argument_type                               Argument;
      typedef typename LoKi::BasicFunctors<Argument>::PredicateFromPredicate Result;
      typedef typename LoKi::BasicFunctors<Argument>::Function               Other;
      // ======================================================================
    public:
      // ======================================================================
      /// ==
      // ======================================================================
      static Result __eq__( const Function& self, const long other ) { return self == other; }
      static Result __eq__( const Function& self, const LHCb::ParticleID& other ) { return self == other; }
      static Result __eq__( const Function& self, const std::string& other ) { return self == other; }
      static Result __eq__( const Function& self, const Other& other ) { return self == other; }
      //
      static Result __eq__( const Function& self, const std::vector<int>& other ) { return self == other; }
      static Result __eq__( const Function& self, const std::vector<unsigned int>& other ) { return self == other; }
      static Result __eq__( const Function& self, const std::vector<std::string>& other ) { return self == other; }
      static Result __eq__( const Function& self, const std::vector<LHCb::ParticleID>& other ) { return self == other; }
      // ======================================================================
      // !=
      // ======================================================================
      static Result __ne__( const Function& self, const long other ) { return self != other; }
      static Result __ne__( const Function& self, const LHCb::ParticleID& other ) { return self != other; }
      static Result __ne__( const Function& self, const std::string& other ) { return self != other; }
      static Result __ne__( const Function& self, const Other& other ) { return self != other; }
      //
      static Result __ne__( const Function& self, const std::vector<int>& other ) { return self != other; }
      static Result __ne__( const Function& self, const std::vector<unsigned int>& other ) { return self != other; }
      static Result __ne__( const Function& self, const std::vector<std::string>& other ) { return self != other; }
      static Result __ne__( const Function& self, const std::vector<LHCb::ParticleID>& other ) { return self != other; }
      // ======================================================================
      // in_list
      // ======================================================================
      static Result __in_list__( const Function& self, const std::vector<int>& other ) { return self == other; }
      static Result __in_list__( const Function& self, const std::vector<unsigned int>& other ) {
        return self == other;
      }
      static Result __in_list__( const Function& self, const std::vector<std::string>& other ) { return self == other; }
      static Result __in_list__( const Function& self, const std::vector<LHCb::ParticleID>& other ) {
        return self == other;
      }
    };
    // ========================================================================
  } // namespace Dicts
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_PIDOPS_H
