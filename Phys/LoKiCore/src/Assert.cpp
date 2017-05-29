// $Id$
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
