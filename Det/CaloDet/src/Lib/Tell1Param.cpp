// ============================================================================
// $Id: $ 
// ============================================================================
#define  CALODET_TELL1PARAM_CPP 1 
#include <vector>

#include "CaloDet/Tell1Param.h"


//-----------------------------------------------------------------------------
// Implementation file for class : Tell1Param
//
// 01/30/2007 : Olivier Deschamps
//-----------------------------------------------------------------------------

// ============================================================================
// Standard creator, initializes variables
// ============================================================================
Tell1Param::Tell1Param( int num )
  : m_number ( num )
{
  m_feCards.clear();
  m_feCards.reserve( 24 );
  m_isPin = false;
}

// ============================================================================
// Destructor
// ============================================================================
Tell1Param::~Tell1Param(){}

// ============================================================================
