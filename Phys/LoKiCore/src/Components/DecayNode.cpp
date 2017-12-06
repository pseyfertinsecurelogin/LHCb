// ============================================================================
// Include files
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/Nodes.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/DecayBase.h"
#include "LoKi/IDecayNode.h"
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @class DecayNode
   *  the simple implementation of Decays::IDecayNode interface
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-05-27
   */
  class DecayNode : public extends<LoKi::DecayBase,Decays::IDecayNode>
  {
  public:
    /** forward to baseclass constructor */
    using base_class::base_class;
    /** build the node form the node descriptor
     *
     *  @code
     *
     *  const Decays::IDecayNode* finder = ... ;
     *
     *  Decays::Node node = finder -> node ( " Beauty & Charm & Baryon " ) ;
     *
     *  if ( !node.valid() ){  .. error here ... }
     *
     *  @endcode
     *  @param descriptor (INPUT)  node deccriptor
     *  @param constructed node
     */
    Decays::Node node ( const std::string& descriptor ) const override
    { return LoKi::DecayBase::node ( descriptor ) ; }
  } ; //                                                 end of class DecayNode
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                the factory for instantiation
// ============================================================================
DECLARE_COMPONENT( LoKi::DecayNode )
// ============================================================================


// ============================================================================
// The END
// ============================================================================
