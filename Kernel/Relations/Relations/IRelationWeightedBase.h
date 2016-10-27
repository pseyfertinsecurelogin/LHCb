// ============================================================================
#ifndef RELATIONS_IRELATIONWEIGHTEDBASE_H 
#define RELATIONS_IRELATIONWEIGHTEDBASE_H 1
// ============================================================================
// Include files 
// local
#include "Relations/IRelationBase.h"


/** @class IRelationWeightedBase Relations/IRelationWeightedBase.h
 *  
 *  The helper base class for weighted relations 
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date   2004-01-14
 */
struct IRelationWeightedBase : extend_interfaces<IRelationBase>
{
  
  /** the unique interface ID (static)
   *  @return the unique interface identifier 
   */
  DeclareInterfaceID( IRelationWeightedBase, 2, 0 );
  
};

// ============================================================================
// The END 
// ============================================================================
#endif // RELATIONS_IRELATIONWEIGHTEDBASE_H
// ============================================================================
