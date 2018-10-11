/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
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
    if (layer) {
      unsigned int layerID = layer->layerID();
       if( layerID < 4 ) {
         m_layers[layerID] = layer;
       }
    }
  }

  m_stationID = (unsigned int)param<int>("stationID");

  return StatusCode::SUCCESS;
}

/// Find the layer for a given XYZ point
const DeFTLayer* DeFTStation::findLayer(const Gaudi::XYZPoint& aPoint) const {
  /// Find the layer and return a pointer to the layer from XYZ point
  auto iter = std::find_if(m_layers.begin(), m_layers.end(),
      [&aPoint](const DeFTLayer* l){return l ? l->isInside(aPoint) : false ; } );
  return iter != m_layers.end() ? *iter : nullptr;
}
