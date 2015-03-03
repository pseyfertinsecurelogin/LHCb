// $Id: GenDump.h 134319 2012-01-29 17:47:25Z ibelyaev $
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
 *  By usage of this code one clearly states the disagreement 
 *  with the smear campaign of Dr.O.Callot et al.: 
 *  ``No Vanya's lines are allowed in LHCb/Gaudi software.''
 *
 *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
 *  @date   2011-06-03
 * 
 *                    $Revision: 134319 $
 *  Last modification $Date: 2012-01-29 18:47:25 +0100 (Sun, 29 Jan 2012) $
 *                 by $Author: ibelyaev $
 */
// ============================================================================
namespace LoKi 
{
  // ==========================================================================  
  namespace Functors 
  {
    // ========================================================================
    /** template specialization of ``dumpers''
     *  @see LoKi::Functors::Dump_
     *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
     *  @date   2011-06-03
     */  
    template <>
    Dump_<const HepMC::GenParticle*>::result_type 
    Dump_<const HepMC::GenParticle*>::operator() 
      ( Dump_<const HepMC::GenParticle*>::argument a ) const ;
    // ========================================================================
    /** template specialization of ``dumpers''
     *  @see LoKi::Functors::Dump1_
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2012-01-28
     */  
    template <>
    Dump1_<const HepMC::GenParticle*,bool>::result_type 
    Dump1_<const HepMC::GenParticle*,bool>::operator() 
      ( Dump1_<const HepMC::GenParticle*,bool>::argument a ) const ;
    // ========================================================================
    /** template specialization of ``dumpers''
     *  @see LoKi::Functors::Dump1_
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2012-01-28
     */  
    template <>
    Dump1_<const HepMC::GenParticle*,double>::result_type 
    Dump1_<const HepMC::GenParticle*,double>::operator() 
      ( Dump1_<const HepMC::GenParticle*,double>::argument a ) const ;
    // ========================================================================
  } //                                          end of namesapce LoKi::Functors
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // LOKI_GENDUMP_H
// ============================================================================
