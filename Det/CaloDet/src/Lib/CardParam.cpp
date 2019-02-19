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
// $Id: $
// ============================================================================
#define CALODET_CARDPARAM_CPP 1
// ============================================================================
/// Include files
/// local
#include "CaloDet/CardParam.h"
#include "CaloDet/DeCalorimeter.h"

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
    , m_DownNumber( -1 )
    , m_LeftNumber( -1 )
    , m_CornerNumber( -1 )
    , m_PreviousNumber( -1 )
    , m_validationNumber( -1 )
    , m_isPin( false )
    , m_number( iNum )
    , m_crate( iCrate )
    , m_slot( iSlot )
    , m_tell1( -1 )
    , m_selectionType( -1 )
    , m_mapping( CardParam::None ) {
  m_ids.reserve( 64 );
  m_ids.clear();
}

// ============================================================================
// Destructor
// ============================================================================
CardParam::~CardParam() {}

// ============================================================================
