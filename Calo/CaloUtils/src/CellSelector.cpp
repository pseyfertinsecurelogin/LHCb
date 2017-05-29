#include <string>

#include "CaloUtils/CellMatrix.h"
#include "CaloUtils/CellMatrix2x2.h"
#include "CaloUtils/CellMatrix3x3.h"
#include "CaloUtils/CellNeighbour.h"
#include "CaloUtils/CellSelector.h"
#include "CaloUtils/CellSwissCross.h"

class DeCalorimeter;

// constructor
CellSelector::CellSelector( const DeCalorimeter* det , std::string selector )
  : CellMatrix( det ), m_selector( selector ){
  m_cell3x3.setDet( det );
  m_cell2x2.setDet( det );
  m_cellSwissCross.setDet( det );
  m_cellNeighbour.setDet( det );
}
void CellSelector::setDet(const DeCalorimeter* det ){
  CellMatrix::setDet( det );
  m_cell3x3.setDet( det );
  m_cell2x2.setDet( det );
  m_cellSwissCross.setDet( det );
  m_cellNeighbour.setDet( det );
}
