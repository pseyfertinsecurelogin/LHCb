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
// ============================================================================
// Include files
#include "GaudiKernel/GaudiException.h"
// local
#include "Kernel/CaloCellIDKeyTraits.h"


// ============================================================================
/** @file CaloCellIDKeyTraits.cpp
 *
 *  Implementation file for methods from Containers namespace
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date 20/03/2002
 */
// ============================================================================

// ============================================================================
/**
 *  function to be called at each attempt of automatic creation of CaloCellID
 *  object as a KEY for KeyedObjects
 *
 *  @exception GaudiException
 *  @see GaudiException
 *  @see Containers
 *  @see CaloCellID
 *  @see CaloCellIDKeyTraits
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date 20/03/2002
 */
// ============================================================================
void Containers::errorMakeCaloCellIDKey()
{
  throw
    GaudiException("No automatic key creation for KEY=LHCb::CaloCellID !",
                   "KeyedObject<LHCb::CaloCellID>" , StatusCode::FAILURE );
}

// ============================================================================
// The End
// ============================================================================
