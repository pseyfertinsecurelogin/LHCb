// FTDet
#include "FTDet/DeFTQuarter.h"

/** @file DeFTQuarter.cpp
 *
 *  Implementation of class : DeFTQuarter
 *
 *  @author Jeroen van Tilburg
 *  @date   2016-07-18
 */

//=============================================================================
// classID function
//=============================================================================
const CLID& DeFTQuarter::clID () const {
  return DeFTQuarter::classID();
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode DeFTQuarter::initialize() {
  /// Loop over modules
  m_modules.resize( this->childIDetectorElements().size(), nullptr);
  IDetectorElement::IDEContainer::const_iterator iM;
  for (iM = this->childBegin(); iM != this->childEnd(); ++iM) {
    DeFTModule* module = dynamic_cast<DeFTModule*>(*iM);
    if (module) {
      unsigned int moduleID = module->moduleID();
       if( moduleID < m_modules.size() ) {
         m_modules[moduleID] = module;
       }
    }
  } /// iM

  m_quarterID = (unsigned int)param<int>("quarterID");

  return StatusCode::SUCCESS;
}

/// Find the module for a given XYZ point
const DeFTModule* DeFTQuarter::findModule(const Gaudi::XYZPoint& aPoint) const {
  auto  iM = std::find_if(m_modules.begin(), m_modules.end(),
      [&aPoint](const DeFTModule* m){ return m ? m->isInside(aPoint) : false; } );
  return iM != m_modules.end() ? *iM : nullptr ;
}
