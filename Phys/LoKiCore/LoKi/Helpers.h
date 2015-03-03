// $Id: Helpers.h 53291 2010-08-05 14:35:53Z ibelyaev $
// ============================================================================
#ifndef LOKI_HELPERS_H 
#define LOKI_HELPERS_H 1
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
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-02-19 
 */
// ============================================================================
namespace LoKi 
{
  /** @namespace LoKi::Helpers Helpers.h LoKi/Helpers.h
   *  Namespace with pure technical ("private") helper functions 
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2006-02-19
   */
  namespace Helpers 
  {
    // ========================================================================
    /** Trivial helper which returns either the first element 
     *  of the container, or its default" value (for empty 
     *  container)
     *  @param cnt the container itself 
     *  @return the front element (or its defualt value)
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-19
     */
    template <class CONTAINER> 
    inline typename CONTAINER::value_type _First
    ( const CONTAINER& cnt ) 
    {
      if ( !cnt.empty() ) { return *(cnt.begin()) ; }
      return typename CONTAINER::value_type() ;
    } 
    // ========================================================================    
    /** Trivial helper which returns either the first element 
     *  of the container, or its default" value (for empty 
     *  or invalid container)
     *  @param cnt pointer to the the container
     *  @return the front element (or its defualt value)
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-19
     */
    template <class CONTAINER> 
    inline typename CONTAINER::value_type _First
    ( const CONTAINER* cnt ) 
    { 
      if ( 0 != cnt ) { return _First(*cnt) ; }
      return typename CONTAINER::value_type() ; 
    }
    // ========================================================================    
  } // end of namespace LoKi::Helpers 
} // end of namespace LoKi
// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_HELPERS_H
// ============================================================================
