/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// $Id: IRelationBases.cpp,v 1.5 2008-02-19 15:26:09 ibelyaev Exp $
// ============================================================================
// local
// ============================================================================
#include "Relations/IRelationBase.h"
#include "Relations/IRelationWeightedBase.h"
// ============================================================================
/** @file 
 *  Implementation file for class IRelationBase and class IRelationWeighedBase
 * 
 *  @author Vanya  BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2004-01-14 
 */
// ============================================================================
namespace 
{
  const InterfaceID IID_IRelationBase ( "IRelationBase", 1, 0 );
  //  the unique identifier of interface IRelationWeightedBase
  const InterfaceID IID_IRelationWeightedBase 
  ( "IRelationWeightedBase", 1, 0 );
}
// ============================================================================
/*  the unique interface ID (static)
 *  @return the unique interface identifier 
 */
// ============================================================================
const InterfaceID& IRelationBase::interfaceID() 
{ return IID_IRelationBase; } 
// ============================================================================
// destructor 
// ============================================================================
IRelationBase::~IRelationBase(){} 
// ============================================================================
/*  the unique interface ID (static)
 *  @return the unique interface identifier 
 */
// ============================================================================
const InterfaceID& IRelationWeightedBase::interfaceID() 
{ return IID_IRelationWeightedBase; }
// ============================================================================
// destructor 
// ============================================================================
IRelationWeightedBase::~IRelationWeightedBase(){} 
// ============================================================================

// ============================================================================
// The END 
// ============================================================================

