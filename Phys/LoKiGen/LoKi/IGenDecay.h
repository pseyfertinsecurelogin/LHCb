// $Id: IGenDecay.h 134316 2012-01-29 13:55:54Z ibelyaev $
// ============================================================================
#ifndef LOKI_IGENDECAY_H 
#define LOKI_IGENDECAY_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/iTree.h"
#include "LoKi/DecayFinder.h"
#include "LoKi/IDecayNode.h"
// ============================================================================
// forward declaration
// ============================================================================
namespace HepMC { class GenParticle ; }
// ============================================================================
/** @file LoKi/IGenDecay.h
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
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23 
 *
 *                    $Revision: 134316 $
 *  Last modification $Date: 2012-01-29 14:55:54 +0100 (Sun, 29 Jan 2012) $
 *                 by $Author: ibelyaev $
 */
// ============================================================================
namespace Decays 
{
  // ==========================================================================
  /** @class IGenDecay LoKi/IGenDecay.h
   *  New "Decay Finder"
   *  @author Ivan BELYAEV
   *  @date   2009-05-22
   */
  class GAUDI_API IGenDecay : public virtual IDecayNode
  {
    // ========================================================================
  public:
    // ========================================================================
    /// the actual type of the tree 
    typedef Decays::Tree_<const HepMC::GenParticle*>                     Tree ;
    /// the actual type of the tree 
    typedef Decays::iTree_<const HepMC::GenParticle*>                   iTree ;
    /// the actual type of decay finder:
    typedef Decays::Finder_<const HepMC::GenParticle*>                 Finder ;
    // ========================================================================
  public:
    // ========================================================================
    /** create the decay tree from the descriptor
     *  @param descriptor (INPUT)  the decay descriptor 
     *  @param tree       (OUTPUT) the decay tree 
     */
    virtual Tree tree ( const std::string& decay = "" ) const = 0 ;
    // ========================================================================
  public:
    // ========================================================================
    /// the unique interface ID 
    static const InterfaceID& interfaceID() ;        // the unique interface ID 
    // ========================================================================
  protected:
    // ========================================================================
    /// virtual & protected destructor 
    virtual ~IGenDecay () ;                   // virtual & protected destructor 
    // ========================================================================
  } ;
  // ==========================================================================
} //                                                    end of namespace Decays 
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // LOKI_IGENDECAY_H
// ============================================================================
