// Include files

// local
#include "DetDesc/DataStoreLoadAgent.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/StatusCode.h"

class DataObject;

//-----------------------------------------------------------------------------
// Implementation file for class : DataStoreLoadAgent
//
// 2005-02-24 : Marco CLEMENCIC
//-----------------------------------------------------------------------------

//=============================================================================
// Analyze
//=============================================================================
bool DataStoreLoadAgent::analyse (IRegistry *pObject, int level){
  DataObject *tmp;

   // I couldn't find a better way to load an object.
  StatusCode sc = pObject->dataSvc()->retrieveObject(pObject->identifier(),tmp);

  if (level > m_maxDepth) m_maxDepth = level;

  if ( sc.isSuccess() ){ // object was loaded successfully
    ++m_loadedObjects;
  } else { // obj. was not loaded: stop the recursion (there shouldn't be any level downstream)
    return false;
  }

  return true;
}
//=============================================================================
