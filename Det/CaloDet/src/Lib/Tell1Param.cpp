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
// ============================================================================
#define CALODET_TELL1PARAM_CPP 1
// ============================================================================
/// Include files
/// local
#include "CaloDet/Tell1Param.h"
#include "CaloDet/DeCalorimeter.h"

//-----------------------------------------------------------------------------
// Implementation file for class : Tell1Param
//
// 01/30/2007 : Olivier Deschamps
//-----------------------------------------------------------------------------

// ============================================================================
// Standard creator, initializes variables
// ============================================================================
Tell1Param::Tell1Param( int num ) : m_number( num ) {
  m_feCards.clear();
  m_feCards.reserve( 24 );
  m_isPin = false;
}

// ============================================================================
// Destructor
// ============================================================================
Tell1Param::~Tell1Param() {}

// ============================================================================
