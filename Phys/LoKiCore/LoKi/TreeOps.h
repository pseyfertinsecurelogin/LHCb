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
#ifndef LOKI_TREEOPS_H
#define LOKI_TREEOPS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <sstream>
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleID.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Trees.h"
#include "LoKi/iTree.h"
// ============================================================================
namespace LoKi {
  namespace Dicts {
    // ========================================================================
    template <class PARTICLE>
    struct TreeOps {
      // ======================================================================
      using iTree = Decays::iTree_<PARTICLE>;
      using Tree  = Decays::Tree_<PARTICLE>;
      // ======================================================================
    public:
      // ======================================================================
      // call
      static bool __call__( const iTree& t, typename iTree::argument a ) { return t( a ); }
      // ======================================================================
    public:
      // ======================================================================
      // mark
      static Tree __mark__( const iTree& tree ) { return Decays::Trees::Marked_<PARTICLE>( tree ); }
      // ======================================================================
    public:
      // ======================================================================
      // and
      static Tree __and__( const iTree& t1, const iTree& t2 ) { return t1 && t2; }
      // or
      static Tree __or__( const iTree& t1, const iTree& t2 ) { return t1 || t2; }
      // not
      static Tree __invert__( const iTree& t1 ) { return ~t1; }
      // ======================================================================
    public:
      // ======================================================================
      // __rrshift__
      static bool __rrshift__( const iTree& t, typename iTree::argument a ) { return t( a ); }
      // ======================================================================
      // __rshift__
      static Tree __rshift__( const iTree& t1, const iTree& t2 ) { return t1 && t2; }
      // ======================================================================
      // __rshift__
      static size_t __rshift__( const iTree& t, typename iTree::Collection& c ) { return t.collect( c ); }
      // ======================================================================
    public:
      // ======================================================================
      // __str__
      static std::string __toString__( const iTree& t ) {
        std::ostringstream s;
        t.fillStream( s );
        return s.str();
      }
      // ======================================================================
    };
    // ========================================================================
  } // namespace Dicts
} // namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_TREEOPS_H
