// $Id: MCDump.h 155779 2013-04-29 11:00:51Z cattanem $
// ============================================================================
#ifndef LOKI_MCDUMP_H 
#define LOKI_MCDUMP_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Filters.h"
#include "LoKi/MCTypes.h"
#include "LoKi/Dumper.h"
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
 *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
 *  @date   2011-06-03
 * 
 *                    $Revision: 155779 $
 *  Last modification $Date: 2013-04-29 13:00:51 +0200 (Mon, 29 Apr 2013) $
 *                 by $Author: cattanem $
 */
// ============================================================================
namespace LoKi 
{
  // ==========================================================================  
  namespace Functors 
  {
    // ========================================================================
    /** template specialization of ``dumper''
     *  @see LoKi::Fuctors::Dump_
     *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
     *  @date   2011-06-03
     */  
    template <>
    Dump_<const LHCb::MCParticle*>::result_type 
    Dump_<const LHCb::MCParticle*>::operator() 
      ( Dump_<const LHCb::MCParticle*>::argument a ) const ;
    // ========================================================================
    /** template specialization of ``dumper''
     *  @see LoKi::Fuctors::Dump1_
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2012-01-28
     */  
    template <>
    Dump1_<const LHCb::MCParticle*,bool>::result_type 
    Dump1_<const LHCb::MCParticle*,bool>::operator() 
      ( Dump1_<const LHCb::MCParticle*,bool>::argument a ) const ;
    // ========================================================================
    /** template specialization of ``dumper''
     *  @see LoKi::Fuctors::Dump1_
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2012-01-28
     */  
    template <>
    Dump1_<const LHCb::MCParticle*,double>::result_type 
    Dump1_<const LHCb::MCParticle*,double>::operator() 
      ( Dump1_<const LHCb::MCParticle*,double>::argument a ) const ;
    // ========================================================================
  } //                                          end of namespace LoKi::Functors 
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_MCDUMP_H
// ============================================================================
