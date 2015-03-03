// $Id: IDecay.cpp,v 1.1 2009-05-27 18:45:23 ibelyaev Exp $
// ============================================================================
// Include files 
// ============================================================================
// Event 
// ============================================================================
#include "Event/Particle.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/IDecay.h"
// ============================================================================
/** @file 
 *  Implementation file for class Decays::IDecay
 *  @author Ivan BELYAEV
 *  @date   2009-05-22
 */
// ============================================================================
// virtual & protected destructor 
// ============================================================================
Decays::IDecay::~IDecay () {}                 // virtual & protected destructor 
// ============================================================================
// unique interface ID
// ============================================================================
const InterfaceID& Decays::IDecay::interfaceID()     // the unique interface ID
{
  static const InterfaceID s_ID ( "Decays::IDecay" , 1 , 0 ) ;
  return s_ID ;
}
// ============================================================================


// ============================================================================
// The END 
// ============================================================================
