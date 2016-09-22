// FTDet
#include "FTDet/DeFTStation.h"

/** @file DeFTStation.cpp
 *
 *  Implementation of class : DeFTStation
 *
 *  @author Jeroen van Tilburg
 *  @date   2016-07-18
 */

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
DeFTStation::DeFTStation( const std::string& name ) :
DetectorElement ( std::move(name) )
{   // constructor
  m_layers.reserve(4);
}

//=============================================================================
// classID function
//=============================================================================
const CLID& DeFTStation::clID () const {
  return DeFTStation::classID();
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode DeFTStation::initialize(){
  /// Loop over layers and add to layer vector
  for (auto iL = this->childBegin(); iL != this->childEnd(); ++iL) {
    DeFTLayer* layer = dynamic_cast<DeFTLayer*>(*iL);
    if (layer) m_layers.push_back(layer);
  }
  return StatusCode::SUCCESS;
}

/// Find the layer for a given XYZ point
const DeFTLayer* DeFTStation::findLayer(const Gaudi::XYZPoint& aPoint) const {
  /// Find the layer and return a pointer to the layer from XYZ point
  auto iter = std::find_if(m_layers.begin(), m_layers.end(),
      [&aPoint](const DeFTLayer* l){return l->isInside(aPoint); } );
  return iter != m_layers.end() ? *iter : nullptr;
}
