// ============================================================================
// Include files
// ============================================================================
// LHCbKernel
// ============================================================================
#include "Kernel/IInAcceptance.h"
// ============================================================================

// ============================================================================
/** @file
 *  Implementation file for class IInAcceptance
 *
 *  @date 2006-04-05
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 */
// ============================================================================

// ============================================================================
/// anonynous namespace to hide local variable
// ============================================================================
namespace
{
  // ==========================================================================
  /** The unique identifier for IInAcceptance interface
   *  @see IInterface
   *  @see InterfaceID
   */
  // ==========================================================================
  const InterfaceID IID_IInAcceptance ( "IInAcceptance", 1, 0 ) ;
  // ==========================================================================
}
// ============================================================================
/// Return the unique interface identifier
// ============================================================================
const InterfaceID& IInAcceptance::interfaceID() { return IID_IInAcceptance ; }
// ============================================================================
/// virtual and protected destructor
// ============================================================================
IInAcceptance::~IInAcceptance(){}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
