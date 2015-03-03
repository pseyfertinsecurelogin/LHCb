// $Id: GenOscillated.h 131413 2011-11-11 18:10:47Z ibelyaev $
// ============================================================================
#ifndef LOKI_GENOSCILLATED_H 
#define LOKI_GENOSCILLATED_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKerne
// ============================================================================
#include "GaudiKernel/Kernel.h"
// ============================================================================
namespace HepMC { class GenParticle ; }
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
 *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
 *
 *                    $Revision: 131413 $
 *  Last modification $Date: 2011-11-11 19:10:47 +0100 (Fri, 11 Nov 2011) $
 *                 by $Author: ibelyaev $
 */
// ===========================================================================
namespace LoKi 
{
  // ==========================================================================
  namespace GenParticles 
  {  
    // ========================================================================
    /** check the oscillation for the particle 
     *  @param p the particle 
     *  @return the oscillated mother particle 
     *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-03 
     */
    GAUDI_API 
    const HepMC::GenParticle* 
    oscillated1  ( const HepMC::GenParticle* p ) ;
    // ========================================================================
    /** check the oscillation for the particle 
     *  @param p the particle 
     *  @return the oscillated daughter particle 
     *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-03 
     */
    GAUDI_API 
    const HepMC::GenParticle* 
    oscillated2  ( const HepMC::GenParticle* p ) ;
    // ========================================================================
  } //                                      end of namespace LoKi::GenParticles 
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // LOKI_GENOSCILLATED_H
// ============================================================================
