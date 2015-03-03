// $Id: GetType.h 133878 2012-01-18 19:33:38Z ibelyaev $
// ============================================================================
#ifndef LOKI_GETTYPE_H 
#define LOKI_GETTYPE_H 1
// ============================================================================
// Include files
// ============================================================================
/** @file LoKi/GetType.h
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
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 */
namespace LoKi 
{
  // ==========================================================================
  /** @struct GetType GetType.h LoKi/GetType.h
   *  
   *  Helper utility to get "raw" type 
   *
   *  @author Ivan BELYAEV
   *  @date   2006-10-07
   */
  template <class TYPE>
  struct GetType 
  {
    typedef TYPE Type ;
  } ;
  // ==========================================================================  
  template <class TYPE>
  struct GetType<const TYPE> 
  {
    typedef typename GetType<TYPE>::Type Type ;
  } ; 
  // ==========================================================================  
  template <class TYPE>
  struct GetType<TYPE&> 
  {
    typedef typename GetType<TYPE>::Type Type ;
  } ;
  // ==========================================================================  
  template <class TYPE>
  struct GetType<TYPE*> 
  {
    typedef typename GetType<TYPE>::Type Type ;
  } ;
  // ==========================================================================  
} // end of namespace LoKi
// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_GETTYPE_H
// ============================================================================
