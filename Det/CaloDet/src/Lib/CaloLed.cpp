#define  CALODET_CALOLED_CPP 1 
#include <iosfwd>
#include <string>
#include <vector>

#include "CaloDet/CaloLed.h"
#include "Kernel/CaloCellID.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CaloLed 
//
// 15/11/2006 : Olivier Deschamps
//-----------------------------------------------------------------------------


//=============================================================================
// Standard creator, initializes variables
//=============================================================================
CaloLed::CaloLed(int id )
  : m_id(id),
    m_pin(),
    m_area(),
    m_fCol(),
    m_fRow(),
    m_lCol(),
    m_lRow(),
    m_index(-1),
    m_cells()
{
  m_cells.clear();
  std::stringstream reg("Region ") ;
  m_region = reg.str();
}

//=============================================================================
// destructor 
//=============================================================================
CaloLed::~CaloLed() {}

//=============================================================================
