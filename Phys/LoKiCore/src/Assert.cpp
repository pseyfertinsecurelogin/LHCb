// $Id: Assert.cpp,v 1.1 2007-08-11 20:17:01 ibelyaev Exp $
// ============================================================================
// Include files 
// ============================================================================
// STD &STL
// ============================================================================
#include <string>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Exception.h"
#include "LoKi/Assert.h"
// ============================================================================
/** @file
 *  Implementation file for function LoKi::throwException 
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-08-11
 */
// ============================================================================
void LoKi::throwException ( const std::string& message ) 
{ throw LoKi::Exception ( message ) ; }
// ============================================================================

// ============================================================================
// The END 
// ============================================================================
