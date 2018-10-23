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
#include "STDet/DeUTDetector.h"
#include "DetDesc/IGeometryInfo.h"

#include "STDet/DeUTStation.h"
#include "STDet/DeUTLayer.h"
#include "STDet/DeUTModule.h"
#include "STDet/DeUTSector.h"

#include "Kernel/UTNames.h"
#include <cassert>

using namespace LHCb;

/** @file DeUTDetector.cpp
 *
 *  Implementation of class: DeUTDetector
 *
 *  All UT elements are modification for TT element classes
 *  that were originally written by Matt Needham)
 *
 *  @author Jianchun Wang, Matt Needham
 *  @date   2012-07-10
 *
 */


DeUTDetector::DeUTDetector( const std::string& name ) :
  DeSTDetector( name )
{
  m_sectors.clear();
}

const CLID& DeUTDetector::clID () const
{
  return DeUTDetector::classID() ;
}


StatusCode DeUTDetector::initialize()
{
  // init the base class
  StatusCode sc = DeSTDetector::initialize();
  if ( sc.isFailure() ){
    MsgStream msg(msgSvc(), name());
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
  }
  else {
    // make a flat list of sectors
    flatten();
    setOffset();
    if ( !m_sectors.empty() ) {
      setNstrip(m_sectors.front()->nStrip()* m_sectors.size());
      m_sMap.reserve(m_sectors.size());
      for (Sectors::const_iterator iter = m_sectors.begin(); iter != m_sectors.end(); ++iter){
        m_sMap.insert((*iter)->elementID().uniqueSector(), *iter);
      }
    }
  }
  return sc;

}


DeSTSector* DeUTDetector::findSector(const Gaudi::XYZPoint& aPoint) const
{
  DeSTSector* aSector = nullptr;
  DeSTStation* tStation = findStation(aPoint);
  if ( tStation ){
    DeUTStation* aStation = dynamic_cast<DeUTStation*>(tStation);
    DeUTLayer* aLayer = aStation->findLayer(aPoint);
    if ( aLayer ){
      DeUTModule* aModule = aLayer->findModule(aPoint);
      if ( aModule ){
        aSector = aModule->findSector(aPoint);
      }
    }
  }
  return aSector;
}


DeSTBaseElement* DeUTDetector::findTopLevelElement(const std::string& nickname) const
{
  if ( nickname.find("UT") == std::string::npos ) return nullptr;

  const STChannelID chan = UTNames().stringToChannel(nickname);
  if ( chan.sector() != 0 )  return this->DeSTDetector::findSector(chan);
  if ( chan.layer() != 0 )   return findLayer(chan);
  if ( chan.station() != 0u) return findStation(chan);

  return nullptr;
}


void DeUTDetector::flatten()
{
  m_sectors.reserve(1200);
  for (auto ptrstation: m_stations) {
    for (auto ptrlayer: dynamic_cast<DeUTStation*>(ptrstation)->layers()) {
      m_layers.push_back(ptrlayer);
      const auto& vecptrsectors = ptrlayer->sectors();
      m_sectors.insert(m_sectors.end(), vecptrsectors.begin(), vecptrsectors.end());
    }
  }
}

void DeUTDetector::setOffset() {

  bool beginit = true;
  uint curr_region = 0;
  uint topoffset = 0;

  assert(m_stations.size() == NBSTATION
         && "setOffset of UTDetector assumes a wrong number of station");

  // add one offset for each region of each module of each layer of each station
  int ir = 0;
  for (auto ptrstation: m_stations) {
    for (auto ptrlayer: dynamic_cast<DeUTStation*>(ptrstation)->layers()) {

      assert(dynamic_cast<DeUTStation*>(ptrstation)->layers().size() == NBLAYER
             && "setOffset of UTDetector assumes a wrong number of layers per station");
#ifndef NDEBUG
      int curr_nbreg = ir;
#endif
      for(const auto& ptrmodule: ptrlayer->modules())
      {
        const auto& vecptrsectors = ptrmodule->sectors();
        // add an offset only if we are on a new region
        if(beginit || curr_region != ptrmodule->detRegion()) {
          curr_region = ptrmodule->detRegion();
          beginit = false;
          m_offset[ir] = topoffset;
          ir++;
        }

        // move the total to the last offset
        topoffset += vecptrsectors.size();
      }
      assert((ir - curr_nbreg) == NBREGION && "setOffset of UTDetector assumes a wrong number of region per layer");
    }
  }
}

DeSTSector* DeUTDetector::getSector(const LHCb::STChannelID chan) const {
  return getSector(chan.station(), chan.layer(), chan.detRegion(), chan.sector(), chan.uniqueSector());
}

DeSTSector* DeUTDetector::getSector(unsigned int station,
                                    unsigned int layer,
                                    unsigned int region,
                                    unsigned int sector,
                                    unsigned int uniqueSector) const {

  // helper to get index according to station/layer/region
  constexpr std::array<std::array<std::array<uint, NBREGION>, NBLAYER>, NBSTATION> get_idx_offset {{
      {{ {0, 1, 2}, {3,  4,  5} }},
      {{ {6, 7, 8}, {9, 10, 11} }}
    }};

  // get index offset corresponding to the station/layer/region we want
  auto idx_offset = get_idx_offset[station-1][layer-1][region-1];

  DeSTSector* res = m_sectors[m_offset[idx_offset] + sector-1];

  // debug check to be sure we find the same sector as findSector
  assert( [&] () {
      auto goodsector = m_sMap.find(uniqueSector)->second;
      return goodsector == res;
    }() && "getSector was not able to find the same UT Sector as findSector" );
  // avoid warning in non debug mode
  (void)uniqueSector;

  return res;
}
