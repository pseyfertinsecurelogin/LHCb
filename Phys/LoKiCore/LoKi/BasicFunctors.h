// $Id: BasicFunctors.h 120050 2011-03-10 17:10:51Z ibelyaev $
// ============================================================================
#ifndef LOKI_BASICFUNCTORS_H 
#define LOKI_BASICFUNCTORS_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Functor.h"
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
 *  By usage of this code one clearly states the disagreement 
 *  with the campain of Dr.O.Callot et al.: 
 *  ``No Vanya's lines are allowed in LHCb/Gaudi software.''
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-10-31 
 *
 *                    $Revision: 120050 $
 *  Last modification $Date: 2011-03-10 18:10:51 +0100 (Thu, 10 Mar 2011) $
 *                 by $Author: ibelyaev $
 */
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  template <class TYPE>
  struct Assignable ;
  // ==========================================================================
  /** @struct Assignable 
   *  helper structire to defien the correspondig assignable functors 
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2007-10-31   
   */ 
  template <class TYPE1, class TYPE2>
  struct Assignable<LoKi::Functor<TYPE1,TYPE2> > 
  {
    typedef LoKi::FunctorFromFunctor<TYPE1,TYPE2>  Type ;
  };
  // ==========================================================================
  /** @struct BasicFunctors 
   *
   *  The helper structure to propagate the actual types for the basic functors 
   *
   *  The idea comes from Gerhard "The Great" Raven
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2007-10-31   
   */
  template <class TYPE>
  struct BasicFunctors 
  {
  public:
    // the basic type for "function"  (interface)
    typedef LoKi::Functor<TYPE,double>               Function               ;
    // the basic type for "predicate" (interface)
    typedef LoKi::Functor<TYPE,bool>                 Predicate              ;
    // the basic type for "function"  (assignable)
    typedef typename Assignable<Function>::Type      FunctionFromFunction   ;
    // the basic type for "predicate" (assignable)
    typedef typename Assignable<Predicate>::Type     PredicateFromPredicate ;
    //
    typedef LoKi::Constant<TYPE,double>              Constant               ;
    typedef LoKi::Constant<TYPE,bool>                BooleanConstant        ;
    //
    // ========================================================================
    // for functional programing:
    // ========================================================================
    //
    // map/yield:  
    typedef LoKi::Functor<std::vector<TYPE>,std::vector<double> > Map        ;
    // filter:
    typedef LoKi::Functor<std::vector<TYPE>,std::vector<TYPE> >   Pipe       ;
    // reduce 
    typedef LoKi::Functor<std::vector<TYPE>,double>               FunVal     ;
    // Source
    typedef LoKi::Functor<void,std::vector<TYPE> >                Source     ;
    // reduce 
    typedef LoKi::Functor<std::vector<TYPE>,bool>                 CutVal     ;
    //
  } ;
  // ==========================================================================
} //                                                      end of namespace LoKi 
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // LOKI_BASICFUNCTORS_H
// ============================================================================
