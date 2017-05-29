// ============================================================================
// Include files 
// ============================================================================
// DetDesc 
// ============================================================================
#include "DetDesc/IGeometryErrorSvc.h"

#include "GaudiKernel/IInterface.h"

// ============================================================================
/** @file 
 *  Implementation file for class DetDesc::IGeometryErrorSvc
 *  @date 2007-12-14 
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 */
// ============================================================================
// Retrieve the unique interface ID
// ============================================================================
const InterfaceID& DetDesc::IGeometryErrorSvc::interfaceID() 
{
  static const InterfaceID s_IID ( "DetDesc::IGeometryErrorSvc" , 1 , 0 ) ;
  return s_IID  ;
}

// ============================================================================
// The END 
// ============================================================================
