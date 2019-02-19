/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// Include files

#include <iostream>

#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IRegistry.h"

#include "DetDesc/IDetectorElement.h"

// local
#include "DetDesc/DataStoreLoadAgent.h"

//-----------------------------------------------------------------------------
// Implementation file for class : DataStoreLoadAgent
//
// 2005-02-24 : Marco CLEMENCIC
//-----------------------------------------------------------------------------

//=============================================================================
// Analyze
//=============================================================================
bool DataStoreLoadAgent::analyse( IRegistry* pObject, int level ) {
  DataObject* tmp;

  // I couldn't find a better way to load an object.
  StatusCode sc = pObject->dataSvc()->retrieveObject( pObject->identifier(), tmp );

  if ( level > m_maxDepth ) m_maxDepth = level;

  if ( sc.isSuccess() ) { // object was loaded successfully
    ++m_loadedObjects;
  } else { // obj. was not loaded: stop the recursion (there shouldn't be any level downstream)
    return false;
  }

  return true;
}
//=============================================================================
