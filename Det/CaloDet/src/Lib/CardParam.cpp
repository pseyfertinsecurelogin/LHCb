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
// ============================================================================
/// Include files
/// local
#include "CaloDet/CardParam.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CardParam
//
// 02/07/2001 : Ivan Belyaev
//-----------------------------------------------------------------------------

// ============================================================================
// Standard creator, initializes variables
// ============================================================================
CardParam::CardParam( int iArea, int iRow, int iCol, int ilRow, int ilCol, int iNum, int iCrate, int iSlot )
    : m_Area( iArea )
    , m_Row( iRow )
    , m_Col( iCol )
    , m_lRow( ilRow )
    , m_lCol( ilCol )
    , m_number( iNum )
    , m_crate( iCrate )
    , m_slot( iSlot ) {
  m_ids.reserve( 64 );
  m_ids.clear();
}

// ============================================================================
