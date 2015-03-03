#include "L0MuonKernel/MuonTriggerUnit.h"

L0Muon::MuonTriggerUnit::MuonTriggerUnit() {}

L0Muon::MuonTriggerUnit::MuonTriggerUnit(DOMNode* pNode):L0MUnit(pNode){
}

L0Muon::MuonTriggerUnit::~MuonTriggerUnit() {}

void L0Muon::MuonTriggerUnit::initialize() {

  m_ignoreM1     =  m_properties["ignoreM1"];
  m_ignoreM2 = false;
  std::map<std::string,L0Muon::Property>::iterator ip= m_properties.find("ignoreM2");
  if ( ip!=m_properties.end()) m_ignoreM2 = m_properties["ignoreM2"];
  m_ptparameters =  m_properties["ptParameters"]; 
  m_xfoi         =  m_properties["foiXSize"]; 
  m_yfoi         =  m_properties["foiYSize"];
  m_procVersion  =  m_properties["procVersion"];

  L0Muon::Unit::initialize();
}


int L0Muon::MuonTriggerUnit::xFoi(int sta)
{
  int xfoi= m_xfoi[sta];
  return xfoi;  
}

int L0Muon::MuonTriggerUnit::yFoi(int sta)
{
  int yfoi= m_yfoi[sta];
  return yfoi;
}

