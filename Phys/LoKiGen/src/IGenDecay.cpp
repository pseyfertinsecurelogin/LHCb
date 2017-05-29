// ============================================================================
// Include files
// ============================================================================
// HepMC
// ============================================================================
#ifdef __INTEL_COMPILER
  #pragma warning (disable:1572) // floating-point equality and inequality comparisons are unreliable
  #pragma warning(push)
#endif
#ifdef __INTEL_COMPILER
  #pragma warning(pop)
#endif
#include "GaudiKernel/IInterface.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/IGenDecay.h"

// ============================================================================
/** @file
 *  Implementation file for class Decays::IGenDecay
 *  @author Ivan BELYAEV
 *  @date   2009-05-22
 */
// ============================================================================
// unique interface ID
// ============================================================================
const InterfaceID& Decays::IGenDecay::interfaceID()  // the unique interface ID
{
  static const InterfaceID s_ID ( "Decays::IGenDecay" , 1 , 0 ) ;
  return s_ID ;
}
// ============================================================================


// ============================================================================
// The END
// ============================================================================
