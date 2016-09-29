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
// Standard constructor, initializes variables
//=============================================================================
DeFTQuarter::DeFTQuarter( const std::string& name ) :
DetectorElement ( std::move(name) )
{
  m_modules.reserve(6);
}

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
  IDetectorElement::IDEContainer::const_iterator iM;
  for (iM = this->childBegin(); iM != this->childEnd(); ++iM) {
    DeFTModule* module = dynamic_cast<DeFTModule*>(*iM);
    if (module) {
      m_modules.push_back(module);
    }
  } /// iM

  return StatusCode::SUCCESS;
}

/// Find the module for a given XYZ point
const DeFTModule* DeFTQuarter::findModule(const Gaudi::XYZPoint& aPoint) const {
  auto  iM = std::find_if(m_modules.begin(), m_modules.end(),
      [&aPoint](const DeFTModule* m){ return m->isInside(aPoint); } );
  return iM != m_modules.end() ? *iM : nullptr ;
}
