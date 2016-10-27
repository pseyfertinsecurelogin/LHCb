// ============================================================================
#ifndef RELATIONS_IRELATIONBASE_H 
#define RELATIONS_IRELATIONBASE_H 1
// ============================================================================
// Include files
// from STL
#include <string>
// from Gaudi
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/IInterface.h"
#include "GaudiKernel/System.h"

/** @class IRelationBase IRelationBase.h Relations/IRelationBase.h
 *  
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date   2004-01-14
 */
struct IRelationBase : extend_interfaces<IInterface>
{
  
  /** rebuild/resort  existing relations 
   *
   *  @return status code 
   *
   */
  virtual StatusCode rebuild() = 0 ;
  
  /** remove ALL relations from ALL to ALL objects
   *
   *  @return status code
   */
  virtual StatusCode   clear ()  = 0 ;
  
  /** the unique interface ID (static)
   *  @return the unique interface identifier 
   */
  DeclareInterfaceID( IRelationBase, 2, 0 );
  
};

// ============================================================================
// The END 
// ============================================================================
#endif // RELATIONS_IRELATIONBASE_H
// ============================================================================
