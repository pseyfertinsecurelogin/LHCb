// $Id: DeOTStation.cpp,v 1.8 2007-06-09 13:56:49 janos Exp $

#include <algorithm>
#include <string>
#include <typeinfo>
#include <vector>

#include "DetDesc/DetectorElement.h"
#include "DetDesc/IDetectorElement.h"
#include "GaudiKernel/ClassID.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/StatusCode.h"
#include "Kernel/OTChannelID.h"
#include "OTDet/DeOTLayer.h"
/// OTDet
#include "OTDet/DeOTStation.h"

/** @file DeOTStation.cpp
 *
 *  Implementation of class :  DeOTStation
 *
 *  @author Jeroen van Tilburg jtilburg@nikhef.nl
 */

using namespace LHCb;

DeOTStation::DeOTStation(const std::string& name) :
  DetectorElement( name )
{ /// constructor
  m_layers.reserve(4);
}

const CLID& DeOTStation::clID() const {
  return DeOTStation::classID() ;
}

StatusCode DeOTStation::initialize() {
  /// Loop over layers
  for (auto iL = this->childBegin(); iL != this->childEnd(); ++iL) {
    DeOTLayer* layer = dynamic_cast<DeOTLayer*>(*iL);
    if (layer) {
      m_layers.push_back(layer);
      m_mapLayers.insert(layer->elementID().layer(), layer);
    }
  }/// iLayer

  m_stationID = param<int>("stationID");
  OTChannelID aChan(m_stationID, 0u, 0u, 0u, 0u, 0u);
  setElementID(aChan);

  return StatusCode::SUCCESS;
}

/// Find the layer for a given XYZ point
const DeOTLayer* DeOTStation::findLayer(const Gaudi::XYZPoint& aPoint) const {
  /// Find the layer and return a pointer to the layer from XYZ point
  auto iter = std::find_if(m_layers.begin(), m_layers.end(),
                           [&](const DetectorElement* e)
                           { return e->isInside(aPoint); } );
  return iter != m_layers.end() ? *iter : nullptr;
}
