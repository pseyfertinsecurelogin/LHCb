// $Id: CmpBarCode.h 124208 2011-06-02 16:05:48Z ibelyaev $
// ============================================================================
#ifndef LOKI_CMPBARCODE_H 
#define LOKI_CMPBARCODE_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL 
// ============================================================================
#include <set>
// ============================================================================
// HepMC 
// ============================================================================
#include "HepMC/GenParticle.h"
// ============================================================================
/** @file 
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
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *
 *                    $Revision: 124208 $
 *  Last modification $Date: 2011-06-02 18:05:48 +0200 (Thu, 02 Jun 2011) $
 *                 by $Author: ibelyaev $
 */
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace GenParticles 
  {
    // ========================================================================
    /** @struct CmpBarCode CmpBarCode.h LoKi/CmpBarCode.h
     *  "srickt-less" comparisons of HepMC::GenParticle pointers 
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date   2008-06-24
     */
    struct CmpBarCode 
    {
      // ======================================================================
      /// the only one essential method: compare two particles 
      inline bool operator() 
        ( const HepMC::GenParticle* a , 
          const HepMC::GenParticle* b ) 
      {
        return
          a == b ? false :                   // they are equal, not "less" 
          0 == a ? true  :                   // NULL is always less 
          0 == b ? false :                   // NULL is always less  
          a->barcode () < b->barcode () ;    // compare by barcode 
      }
      // ======================================================================
    } ;
    // ========================================================================
  } //                                      end of namespace LoKi::GenParticles
  // ==========================================================================
  namespace GenTypes 
  {
    // ========================================================================
    /// the type of ordered set generator particles 
    typedef std::set<const HepMC::GenParticle*,
                     LoKi::GenParticles::CmpBarCode>                   GenSet ;
    // ========================================================================    
  }
  // ==========================================================================
} //                                                      end of namespace LoKi 
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // LOKI_CMPBARCODE_H
// ============================================================================
