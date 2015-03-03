// $Id: IHltFactory.cpp,v 1.1.1.1 2008-09-21 14:41:20 ibelyaev Exp $
// ============================================================================
// Include files 
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/IHltFactory.h"
// ============================================================================
/** @file 
 *  Implementation file for class LoKi::Hybrid::IHltFactory
 * 
 *  @date 2008-09-18 
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 */
// ============================================================================
// unique interface identifier 
// ============================================================================
const InterfaceID& LoKi::Hybrid::IHltFactory::interfaceID() 
{
  static const InterfaceID s_ID ( "LoKi::Hybryd::IHltFactory" , 1 , 1 ) ;
  return s_ID ;
}
// ============================================================================
// virtual & protected destructor 
// ============================================================================
LoKi::Hybrid::IHltFactory::~IHltFactory(){}
// ============================================================================

 

// ============================================================================
// The END 
// ============================================================================
