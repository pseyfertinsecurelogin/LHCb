// ============================================================================
#ifndef LOKI_IMCDECAY_H
#define LOKI_IMCDECAY_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/iTree.h"
#include "LoKi/DecayFinder.h"
#include "LoKi/IDecayNode.h"
// ============================================================================
// forward declarations
// ============================================================================
namespace LHCb { class MCParticle ; }
// ============================================================================
namespace Decays
{
  // ==========================================================================
  /** @class IMCDecay LoKi/IMCDecay.h
   *  New "Decay Finder"
   *  @author Ivan BELYAEV
   *  @date   2009-05-22
   */
  struct GAUDI_API IMCDecay : extend_interfaces<IDecayNode>
  {
    // ========================================================================
    /// the actual type of the tree
    typedef Decays::Tree_<const LHCb::MCParticle*>                       Tree ;
    /// the actual type of the tree
    typedef Decays::iTree_<const LHCb::MCParticle*>                     iTree ;
    /// the actual type of decay finder:
    typedef Decays::Finder_<const LHCb::MCParticle*>                   Finder ;
    // ========================================================================
    /** create the decay tree from the descriptor
     *  @param descriptor (INPUT)  the decay descriptor
     *  @param tree       (OUTPUT) the decay tree
     */
    virtual Tree tree ( const std::string& decay = "" ) const = 0 ;
    // ========================================================================
    /// the unique interface ID
    DeclareInterfaceID( IMCDecay , 2 , 0 );
    // ========================================================================
  } ;
  // ==========================================================================
} // end of namespace Decays
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_IMCDECAY_H
// ============================================================================
