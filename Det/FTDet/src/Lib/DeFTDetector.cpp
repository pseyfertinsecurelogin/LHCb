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
#include "FTDet/DeFTDetector.h"

/** @file DeFTDetector.cpp
 *
 *  Implementation of class : DeFTDetector
 *
 *  @author Jeroen van Tilburg
 *  @date   2016-07-18
 */

//=============================================================================
// classID function
//=============================================================================
const CLID& DeFTDetector::clID () const {
  return DeFTDetector::classID();
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode DeFTDetector::initialize(){

  /// Create a MsgStream object with an useful name
  if ( !m_msg ) m_msg.reset( new MsgStream( msgSvc(), "DeFTDetector" ) );

  *m_msg << MSG::DEBUG << "==> Initialize DeFTDetector" << endmsg;

  // Load version and check if version is correct
  m_FTversion = ( exists("FTversion") ? this->params()->param<int>("FTversion") : 0 );
  *m_msg << MSG::INFO << "Current FT geometry version =   " <<  m_FTversion << endmsg;
  if ( m_FTversion < 61 ) {
    *m_msg << MSG::FATAL << "Wrong geometry version! " << endmsg;
    *m_msg << MSG::FATAL << "This version of FTDet requires version 6.1 or higher." << endmsg;
    *m_msg << MSG::FATAL << "--> Current version is " <<  m_FTversion << endmsg;
    return StatusCode::FAILURE;
  }

  // loop over stations and fill stations vector and quarters vector
  unsigned int iQuarter = 0u;
  for (auto  iS = this->childBegin(); iS != this->childEnd(); ++iS) {
    auto* station = dynamic_cast<DeFTStation*>(*iS);
    if (station) {
      unsigned int stationID = station->stationID();
      if( stationID < 4 && stationID > 0) {
        m_stations[stationID-1u] = station;
        m_nStations++;
        for( const auto layer : station->layers() ) {
          for( const auto quarter : layer->quarters() )
            m_quarters[iQuarter++] = quarter;
        }
      }
    }
  }

  m_nModulesT1 = (unsigned int)param<int>("nModulesT1");
  m_nModulesT2 = (unsigned int)param<int>("nModulesT2");
  m_nModulesT3 = (unsigned int)param<int>("nModulesT3");
  m_nLayers    = (unsigned int)param<int>("nLayers");
  m_nQuarters  = (unsigned int)param<int>("nQuarters");
  m_nChannelsInModule = (unsigned int)param<int>("nChannelsInModule");
  m_nTotQuarters = m_nStations * m_nLayers * m_nQuarters;
  m_nTotChannels = (m_nModulesT1 + m_nModulesT2 + m_nModulesT3) *
      m_nLayers * m_nQuarters * m_nChannelsInModule;

  return StatusCode::SUCCESS;// && update) ;
}

//=============================================================================
// Finalization
//=============================================================================
StatusCode DeFTDetector::finalize(){
  // destroy the MsgService object
  m_msg.reset();
  return StatusCode::SUCCESS;
}

/// Find the station for a given XYZ point
const DeFTStation* DeFTDetector::findStation(const Gaudi::XYZPoint& aPoint) const {
  auto iS = std::find_if( std::begin(m_stations), std::end(m_stations),
      [&aPoint] (const DeFTStation* s) {return s ? s->isInside(aPoint) : false ;});
  return iS != m_stations.end() ? *iS : nullptr;
}

/// Find the layer for a given XYZ point
const DeFTLayer* DeFTDetector::findLayer(const Gaudi::XYZPoint& aPoint) const {
  const DeFTStation* s = findStation(aPoint);
  return s ? s->findLayer(aPoint) : nullptr ;
}

/// Find the quarter for a given XYZ point
const DeFTQuarter* DeFTDetector::findQuarter(const Gaudi::XYZPoint& aPoint) const {
  const DeFTLayer* l = findLayer(aPoint);
  return l ? l->findQuarter(aPoint) : nullptr ;
}

/// Find the module for a given XYZ point
const DeFTModule* DeFTDetector::findModule(const Gaudi::XYZPoint& aPoint) const {
  const DeFTLayer* l = findLayer(aPoint); // is faster than via DeFTQuarter
  return l ? l->findModule(aPoint) : nullptr ;
}

/// Find the mat for a given XYZ point
const DeFTMat* DeFTDetector::findMat(const Gaudi::XYZPoint& aPoint) const {
  const DeFTModule* m = findModule(aPoint);
  return m ? m->findMat(aPoint) : nullptr ;
}

/// Get a random FTChannelID (useful for the thermal noise, which is ~flat)
LHCb::FTChannelID DeFTDetector::getRandomChannelFromSeed(const double seed) const {
  if(seed < 0.0 || seed > 1.0 )
    return LHCb::FTChannelID(0u);
  unsigned int flatChannel = int(seed*m_nTotChannels);
  unsigned int channelInModule = flatChannel & (m_nChannelsInModule - 1u) ;
  flatChannel /= m_nChannelsInModule;
  unsigned int quarter = flatChannel & (m_nQuarters - 1u);
  flatChannel /= m_nQuarters;
  unsigned int layer   = flatChannel & (m_nLayers - 1u);
  flatChannel /= m_nLayers;
  unsigned int station = 1;
  unsigned int module = flatChannel;
  if( flatChannel >= m_nModulesT1 + m_nModulesT2 ){
    station = 3; module = flatChannel - m_nModulesT1 - m_nModulesT2;
  } else if( flatChannel >=  m_nModulesT1 ) {
    station = 2; module = flatChannel - m_nModulesT1;
  }
  return LHCb::FTChannelID(station, layer, quarter, module, channelInModule);
}

/// Get a random FTChannelID from a pseudoChannel (useful for the AP noise)
LHCb::FTChannelID DeFTDetector::getRandomChannelFromPseudo(const int pseudoChannel,
    const double seed) const {
  if(seed < 0.0 || seed > 1.0 )
    return LHCb::FTChannelID(0u);
  unsigned int flatQuarter = int(seed*m_nTotQuarters);
  unsigned int quarter = flatQuarter & (m_nQuarters - 1u);
  flatQuarter /= m_nQuarters;
  unsigned int layer   = flatQuarter & (m_nLayers - 1u);
  flatQuarter /= m_nLayers;
  unsigned int station = (flatQuarter & m_nStations) + 1u;

  unsigned int module = pseudoChannel / m_nChannelsInModule ;
  const DeFTModule* moduleDet =
      findModule( LHCb::FTChannelID(station, layer, quarter, module, 0u) );
  if( moduleDet == nullptr ) {
    return LHCb::FTChannelID(0u);
  } else {
    return moduleDet->channelFromPseudo( pseudoChannel & (m_nChannelsInModule -1u));
  }
}

