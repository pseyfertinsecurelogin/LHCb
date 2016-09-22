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
// Standard constructor, initializes variables
//=============================================================================
DeFTDetector::DeFTDetector( const std::string& name ) :
DetectorElement( name )
{
  m_stations.reserve(3);
}

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
  if ( m_FTversion < 60 ) {
    *m_msg << MSG::FATAL << "Wrong geometry version! " << endmsg;
    *m_msg << MSG::FATAL << "This version of FTDet requires version 60 or higher." << endmsg;
    *m_msg << MSG::FATAL << "--> Current version is " <<  m_FTversion << endmsg;
    return StatusCode::FAILURE;
  }

  // loop over stations and fill stations vector
  for (auto  iS = this->childBegin(); iS != this->childEnd(); ++iS) {
    auto* station = dynamic_cast<DeFTStation*>(*iS);
    if (station) m_stations.push_back(station);
  }

  m_nModulesT1 = (unsigned int)param<int>("nModulesT1");
  m_nModulesT2 = (unsigned int)param<int>("nModulesT2");
  m_nModulesT3 = (unsigned int)param<int>("nModulesT3");
  m_nLayers    = (unsigned int)param<int>("nLayers");
  m_nQuarters  = (unsigned int)param<int>("nQuarters");
  m_nSiPMs     = (unsigned int)param<int>("nSiPMs");
  m_nChannels  = (unsigned int)param<int>("nChannels");
  m_nTotChannels = (m_nModulesT1 + m_nModulesT2 + m_nModulesT3) * m_nLayers *
      m_nQuarters * m_nSiPMs * m_nChannels;

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
      [&aPoint] (const DeFTStation* s) {return s->isInside(aPoint) ;});
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
  const DeFTQuarter* q = findQuarter(aPoint);
  return q  ? q->findModule(aPoint) : nullptr;;
}

/// Get a random FTChannelID (useful for the thermal noise, which is ~flat)
const LHCb::FTChannelID DeFTDetector::getRandomChannelFromSeed(const double seed) const {
  if(seed < 0.0 || seed > 1.0 )
    return LHCb::FTChannelID(0u);
  unsigned int flatChannel = int(seed*m_nTotChannels);
  unsigned int channel = flatChannel & (m_nChannels - 1u) ;
  flatChannel /= m_nChannels;
  unsigned int sipm    = flatChannel & (m_nSiPMs - 1u);
  flatChannel /= m_nSiPMs;
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
  return LHCb::FTChannelID(station, layer, quarter, module, sipm, channel);
}
