// $Id: GenDump.h 124235 2011-06-05 12:25:53Z ibelyaev $
// ============================================================================
#ifndef LOKI_GENDUMP_H 
#define LOKI_GENDUMP_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Filters.h"
#include "LoKi/GenTypes.h"
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
 *  with the smear campaign of Dr.O.Callot et al.: 
 *  ``No Vanya's lines are allowed in LHCb/Gaudi software.''
 *
 *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
 *  @date   2011-06-03
 * 
 *                    $Revision: 124235 $
 *  Last modification $Date: 2011-06-05 14:25:53 +0200 (Sun, 05 Jun 2011) $
 *                 by $Author: ibelyaev $
 */
// ============================================================================
namespace LoKi 
{
  // ==========================================================================  
  namespace Functors 
  {
    // ========================================================================
    /** template specialization of ``dumper''
     *  @see LoKi::Functors::Dump_
     *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
     *  @date   2011-06-03
     */  
    template <>
    Dump_<const HepMC::GenParticle*>::result_type 
    Dump_<const HepMC::GenParticle*>::operator() 
      ( Dump_<const HepMC::GenParticle*>::argument a ) const ;
    // ========================================================================
  } //                                          end of namesapce LoKi::Functors 
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // LOKI_GENDUMP_H
// ============================================================================
