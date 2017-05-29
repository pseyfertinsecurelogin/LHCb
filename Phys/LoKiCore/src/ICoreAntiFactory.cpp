// $Id$
// ============================================================================
// Include files 
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/ICoreAntiFactory.h"

#include "GaudiKernel/IInterface.h"

// ============================================================================
/** @file 
 *  Implementation file for class LoKi::Hybrid::ICoreAntiFactory
 *  @see LoKi::Hybrid::ICoreAntiFactory
 *  @date 2008-10-19 
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 */
// ============================================================================
// unique interface identifier 
// ============================================================================
const InterfaceID& LoKi::Hybrid::ICoreAntiFactory::interfaceID() 
{
  static const InterfaceID s_ID ( "LoKi::Hybryd::ICoreAntiFactory" , 1 , 1 ) ;
  return s_ID ;
}
// ============================================================================
// virtual & protected destructor 
// ============================================================================
LoKi::Hybrid::ICoreAntiFactory::~ICoreAntiFactory(){}
// ============================================================================


// ============================================================================
// The END 
// ============================================================================
