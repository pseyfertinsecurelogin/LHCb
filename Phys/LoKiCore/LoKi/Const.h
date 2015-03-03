// $Id: Const.h 133878 2012-01-18 19:33:38Z ibelyaev $
// ============================================================================
#ifndef LOKI_CONST_H 
#define LOKI_CONST_H 1
// ============================================================================
// Include files
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
 *   By usage of this code one clearly states the disagreement 
 *    with the smear campaign of Dr.O.Callot et al.: 
 *  ``No Vanya's lines are allowed in LHCb/Gaudi software.''
 * 
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-03-29
 */
// ============================================================================
namespace LoKi 
{
  // ==========================================================================
  /** @struct Const LoKi/Const.h
   *  Auxillary structure to define "const-traits"
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2006-03-29
   */
  template <class TYPE>
  struct  Const 
  { typedef           const TYPE                       Value ; } ;
  // ==========================================================================
  template <class TYPE>
  struct  Const<const TYPE>
  { typedef typename  Const<TYPE>::Value const         Value ; } ;
  // ==========================================================================
  template <class TYPE>
  struct  Const<TYPE*>
  { typedef typename  Const<TYPE>::Value const * const Value ; } ;
  // ==========================================================================
  template <class TYPE>
  struct  Const<TYPE&>
  { typedef typename  Const<TYPE>::Value const &       Value ; } ;
  // ==========================================================================
  /** @struct NonConst LoKi/Const.h
   *  Auxillary structure to define "const-traits"
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date   2010-07-30
   */
  template <class TYPE>
  struct  NonConst 
  { typedef           TYPE                             Value ; } ;
  // ==========================================================================
  template <class TYPE>
  struct  NonConst<const TYPE>
  { typedef typename  NonConst<TYPE>::Value            Value ; } ;
  // ==========================================================================
  template <class TYPE>
  struct  NonConst<TYPE*>
  { typedef typename  NonConst<TYPE>::Value*           Value ; } ;
  // ==========================================================================
  template <class TYPE>
  struct  NonConst<TYPE&>
  { typedef typename  NonConst<TYPE>::Value&           Value ; } ;
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // LOKI_CONST_H
// ============================================================================
