// $Id: $
// ============================================================================
// Include files 
// ============================================================================
// STD & STL 
// ============================================================================
#include <iostream>
// ============================================================================
// local
// ============================================================================
#include "GSL_sentry.h"
// ============================================================================
/** @file 
 *  Implementation file for class Gaudi::Math::GSL::GSL_Error_Handler
 *  
 *  @see Gaudi::Math::GSL::GSL_Error_Handler
 *  @date 2012-05-27 
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *
 *                    $Revision$
 *  Last modification $Date$
 *                 by $Author$
 */
// ============================================================================
#ifdef __INTEL_COMPILER        // Disable ICC warnings and remarks
#pragma warning(disable:177) // boost::lambda declared but never referenced
#pragma warning(push)
#endif
// ============================================================================
namespace
{
  // ==========================================================================
  // GSL 
  // ==========================================================================
  void GSL_local_error
  ( const char * reason    ,
    const char * file      ,
    int          line      ,
    int          gsl_errno ) 
  {
    std::cerr 
      << " GSL_ERROR : "   
      << gsl_errno << "/'" << gsl_strerror ( gsl_errno ) << "'"
      << "\t reason '"     
      << reason    << "' "
      << "\t file/line '"  
      << file      << "'/" << line 
      << std::endl ;  
  }
  // ==========================================================================
}
// ============================================================================
// constructor: loc
// ============================================================================
Gaudi::Math::GSL::GSL_Error_Handler::GSL_Error_Handler () 
  : m_old ( 0 ) 
{ 
  // m_old = gsl_set_error_handler ( &GSL_local_error ) ; 
  m_old = gsl_set_error_handler_off () ; 
}
// ============================================================================
// destructor/ unlock 
// ============================================================================
Gaudi::Math::GSL::GSL_Error_Handler::~GSL_Error_Handler () 
{ gsl_set_error_handler ( m_old ) ; }
// ============================================================================
#ifdef __INTEL_COMPILER
#pragma warning(pop)        // Reenable ICC remark 2259
#pragma warning(pop)        // Reenable ICC remark 177
#endif
// ============================================================================
// the END 
// ============================================================================
