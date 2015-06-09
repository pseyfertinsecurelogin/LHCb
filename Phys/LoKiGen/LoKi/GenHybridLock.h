// $Id: GenHybridLock.h 155778 2013-04-29 10:03:16Z cattanem $
// ============================================================================
#ifndef LOKI_GENHYBRIDLOCK_H 
#define LOKI_GENHYBRIDLOCK_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Interface.h"
#include "LoKi/IGenHybridTool.h"
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
 *  @date 2007-10-31 
 *
 *                    $Revision: 155778 $
 *  Last modification $Date: 2013-04-29 12:03:16 +0200 (Mon, 29 Apr 2013) $
 *                 by $Author: cattanem $
 */
namespace LoKi
{
  // ==========================================================================
  namespace Hybrid
  {
    // ========================================================================
    /** @class GenLock  LoKi/GenHybridLock.h 
     *  Helper class (sentry) to connent IGenHybridTool to GenEngine  
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-09
     */
    class GenLock 
    {
    public:
      // ======================================================================
      /// constructor : Lock 
      GenLock  ( IGenHybridTool* tool ) ; // contructor : Lock 
      /// destructor : UnLock
      virtual ~GenLock () ;               // destrcutor : UnLock
      // ======================================================================
    private:
      // ======================================================================
      /// no default constructor 
      GenLock () ;                     // no default constructor 
      /// no copy constructor 
      GenLock ( const GenLock& ) ;     // no copy constructor 
      /// no assignement opeartor 
      GenLock& operator = ( const GenLock& ) ; // no assignement operator
      // ======================================================================
    private:
      // ======================================================================
      /// the tool itself 
      LoKi::Interface<LoKi::IGenHybridTool> m_tool ;  // the tool itself 
      // ======================================================================
    } ;
    // ========================================================================
  } //                                            end of namespace LoKi::Hybrid 
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_MCHYBRIDLOCK_H
// ============================================================================
