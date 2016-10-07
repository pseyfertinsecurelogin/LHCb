// FTDet
#include "FTDet/DeFTLayer.h"

/** @file DeFTLayer.cpp
 *
 *  Implementation of class : DeFTLayer
 *
 *  @author Jeroen van Tilburg
 *  @date   2016-07-18
 */


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
DeFTLayer::DeFTLayer( const std::string& name ) :
DetectorElement ( std::move(name) )
{
  m_quarters.reserve(4);
}

//=============================================================================
// classID function
//=============================================================================
const CLID& DeFTLayer::clID () const {
  return DeFTLayer::classID();
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode DeFTLayer::initialize() {
  /// Loop over quarters and add to vector of quarters
  for (auto iQ = this->childBegin(); iQ != this->childEnd(); ++iQ) {
    DeFTQuarter* quarter = dynamic_cast<DeFTQuarter*>(*iQ);
    if (quarter) {
      m_quarters.push_back(quarter);
      // Create the flat list of modules
      DeFTLayer::Modules modules = quarter->modules();
      m_modules.insert(m_modules.end(), modules.begin(), modules.end());
    }
  }

  // Get the global z position of the layer
  Gaudi::XYZPoint globalPoint = geometry()->toGlobal( Gaudi::XYZPoint(0.,0.,0.) );
  m_globalZ = globalPoint.z();

  // Make the plane for the layer
  const Gaudi::XYZPoint g1 = geometry() -> toGlobal( Gaudi::XYZPoint(0.,0.,0.) );
  const Gaudi::XYZPoint g2 = geometry() -> toGlobal( Gaudi::XYZPoint(1.,0.,0.) );
  const Gaudi::XYZPoint g3 = geometry() -> toGlobal( Gaudi::XYZPoint(0.,1.,0.) );
  m_plane = Gaudi::Plane3D(g1,g2,g3 );

  return StatusCode::SUCCESS;
}

/// Find the quarter for a given XYZ point
const DeFTQuarter* DeFTLayer::findQuarter(const Gaudi::XYZPoint& aPoint) const {
  auto  iQ = std::find_if(m_quarters.begin(), m_quarters.end(),
      [&aPoint](const DeFTQuarter* q){return q->isInside(aPoint); } );
  return iQ != m_quarters.end() ? *iQ : nullptr ;
}

/// Find the module for a given XYZ point
const DeFTModule* DeFTLayer::findModule(const Gaudi::XYZPoint& aPoint) const {
  auto  iM = std::find_if(m_modules.begin(), m_modules.end(),
      [&aPoint](const DeFTModule* m){return m->isInside(aPoint); } );
  return iM != m_modules.end() ? *iM : nullptr ;
}
