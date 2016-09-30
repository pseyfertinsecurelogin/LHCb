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
    if (quarter) m_quarters.push_back(quarter);
  }
  return StatusCode::SUCCESS;
}

/// Find the quarter for a given XYZ point
const DeFTQuarter* DeFTLayer::findQuarter(const Gaudi::XYZPoint& aPoint) const {
  auto  iQ = std::find_if(m_quarters.begin(), m_quarters.end(),
      [&aPoint](const DeFTQuarter* q){return q->isInside(aPoint); } );
  return iQ != m_quarters.end() ? *iQ : nullptr ;
}
