// Include files

// local
#include "MuonDet/MuonStationCabling.h"
#include <iostream>

//-----------------------------------------------------------------------------
// Implementation file for class : MuonStationCabling
//
// 2004-01-07 : Alessia Satta
//-----------------------------------------------------------------------------
//=============================================================================
 StatusCode MuonStationCabling::update(long l1numb){
  m_numberOfL1Board=l1numb;
  return StatusCode::SUCCESS;
}

StatusCode MuonStationCabling::addL1Name(std::string name)
{
  if ( static_cast<int>(m_listOfL1.size())>m_numberOfL1Board ) return StatusCode::FAILURE;
  m_listOfL1.push_back(name);
  return StatusCode::SUCCESS;
}

/// update constructor, do a deep copy of all
/// except for the properties of a generic DataObject
void MuonStationCabling::update( Condition& obj ){
  Condition::update(obj);
  const MuonStationCabling& obj1=static_cast<const MuonStationCabling&>(obj);
  m_numberOfL1Board=obj1.getNumberOfL1Board();
  m_listOfL1=obj1.getAllL1Names();
}

/// update constructor, do a deep copy of all
/// except for the properties of a generic DataObject
void MuonStationCabling::update( ValidDataObject& obj ){
Condition& cc=dynamic_cast<Condition&>(obj);
 Condition::update(cc);
  MuonStationCabling& obj1=static_cast<MuonStationCabling&> (obj);
  m_numberOfL1Board=obj1.getNumberOfL1Board();
  m_listOfL1=obj1.getAllL1Names();
}
