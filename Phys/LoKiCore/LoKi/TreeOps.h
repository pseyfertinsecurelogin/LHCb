// $Id$
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
#include "Kernel/ParticleID.h"
#include "Kernel/IParticlePropertySvc.h"
// ============================================================================
// LoKi 
// ============================================================================
#include "LoKi/iTree.h"
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace Dicts
  {
    // ========================================================================
    template <class PARTICLE>
    struct TreeOps 
    {
      // ======================================================================
      typedef Decays::iTree_<PARTICLE>  iTree ;
      typedef Decays::Tree_<PARTICLE>    Tree ;
      // ======================================================================
    public:
      // ======================================================================
      // call 
      static bool __call__ 
      ( const iTree& t , typename iTree::argument a ) { return t ( a ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // mark 
      static Tree __mark__ 
      ( const iTree& tree ) 
      { return Decays::Trees::Marked_<PARTICLE> ( tree ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // and 
      static Tree __and__ 
      ( const iTree& t1 , const iTree& t2 ) { return t1 && t2 ; }
      // or  
      static Tree __or__ 
      ( const iTree& t1 , const iTree& t2 ) { return t1 || t2 ; }
      // not 
      static Tree __invert__ ( const iTree& t1 ) { return ~t1 ; }
      // ======================================================================
    public:
      // ======================================================================
      // __rrshift__
      static bool __rrshift__ 
      ( const iTree& t , typename iTree::argument a ) { return t ( a ) ; }
      // ======================================================================
      // __rshift__ 
      static Tree __rshift__ 
      ( const iTree& t1 , const iTree& t2 ) { return t1 && t2 ; }
      // ======================================================================
      // __rshift__ 
      static size_t __rshift__ 
      ( const iTree& t , typename iTree::Collection& c ) 
      { return t.collect ( c ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // __str__ 
      static std::string __toString__ ( const iTree& t ) 
      {
        std::ostringstream s ;
        t.fillStream ( s ) ;
        return s.str() ;
      }  
      // ======================================================================      
    };
    // ========================================================================
  } // end of namespace LoKi::Dicts 
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_TREEOPS_H
// ============================================================================
