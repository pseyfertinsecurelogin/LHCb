// $Id: Child.h,v 1.8 2007-06-10 20:07:17 ibelyaev Exp $
// ============================================================================
#ifndef LOKI_CHILD_H 
#define LOKI_CHILD_H 1
// ============================================================================
// Include files
// ============================================================================
// Event 
// ============================================================================
#include "Event/Particle.h"
// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project - 
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
 *  contributions and advices from G.Raven, J.van Tilburg, 
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-02-10 
 */
namespace LoKi 
{
  /** @namespace LoKi::Child Child.h LoKi/Child.h
   *  Namespace with collection of accessr to children  
   *  @see LHCb::Particle
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2006-02-11
   */
  namespace Child
  { 
    // ========================================================================
    /** get the number of children 
     *  @param mother poiunter to mother particle 
     *  @return number of daugthter particles 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-02-11
     */
    std::size_t nChildren ( const LHCb::Particle* mother ) ;
    // ========================================================================
    /** Trivial accessor to the daughter particles for the given particle.
     *
     *  @attention index starts with 1 , null index corresponds 
     *             to the original particle 
     *
     *  @param  particle (const) pointer to mother particle 
     *  @param  index    index   index of the child particle 
     *  @return daughter particle with given index 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-02-11
     */
    LHCb::Particle* child
    ( const LHCb::Particle*  particle , 
      const size_t           index    ) ;
    // ========================================================================
    /** Trivial accessor to the daughter particles for the given particle.
     *
     *  @attention index starts with 1 , null index corresponds 
     *             to the original particle 
     *
     *  @param  particle (const) pointer to mother particle 
     *  @param  index1   index   index of the child particle 
     *  @param  index2   index   index of the child particle 
     *  @return daughter particle with given index 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-02-11
     */    
    inline 
    LHCb::Particle* child
    ( const LHCb::Particle*  particle , 
      const size_t           index1   , 
      const size_t           index2   )
    { return child ( child ( particle , index1 ) , index2 ) ; }
    // ========================================================================
    /** Trivial accessor to the daughter particles for the given particle.
     *
     *  @attention index starts with 1 , null index corresponds 
     *             to the original particle 
     *
     *  @param  particle (const) pointer to mother particle 
     *  @param  index1   index   index of the child particle 
     *  @param  index2   index   index of the child particle 
     *  @param  index3   index   index of the child particle 
     *  @return daughter particle with given index 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-02-11
     */    
    inline
    LHCb::Particle* child
    ( const LHCb::Particle*  particle , 
      const size_t           index1   , 
      const size_t           index2   ,
      const size_t           index3   )
    { return child ( child ( particle , index1 ) , index2 , index3 ) ; }
    // ========================================================================
    /** Trivial accessor to the daughter particles for the given particle.
     *
     *  @attention index starts with 1 , null index corresponds 
     *             to the original particle 
     *
     *  @param  particle (const) pointer to mother particle 
     *  @param  index1   index   index of the child particle 
     *  @param  index2   index   index of the child particle 
     *  @param  index3   index   index of the child particle 
     *  @param  index4   index   index of the child particle 
     *  @return daughter particle with given index 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-02-11
     */    
    inline 
    LHCb::Particle* child
    ( const LHCb::Particle*  particle , 
      const size_t           index1   , 
      const size_t           index2   ,
      const size_t           index3   ,
      const size_t           index4   )
    { return child ( child ( particle , index1 ) , index2 , index3 , index4 ) ; }
    // ========================================================================
    /** trivial function to access all children particles 
     *  @see LHCb::Particle::daughters
     *  @param particle pointer to particle 
     *  @param return vector of children
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-06-04
     */
    LHCb::Particle::ConstVector 
    children 
    ( const LHCb::Particle* particle ) ;
    // ========================================================================
    /** trivial function to access all descendants particles 
     *  @param particle pointer to particle 
     *  @param return vector of descendants 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-06-04
     */
    LHCb::Particle::ConstVector 
    descendants
    ( const LHCb::Particle* particle ) ;
    // ========================================================================
    ///    helper structure to force the loading of functions 
    struct PhysChild     
    {
      /// ficvtive contructor
      PhysChild() ; ///< fictive  constructor
    } ;
    // ========================================================================
  }  // end of the namespace LoKi:Child 
}  // end of the namespace LoKi
// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_CHILD_H
// ============================================================================

