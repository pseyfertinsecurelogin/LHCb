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
#include "FTDet/DeFTMat.h"

#include "DetDesc/SolidBox.h"

/** @file DeFTMat.cpp
 *
 *  Implementation of class : DeFTMat
 *
 *  @author Jeroen van Tilburg
 *  @date   2016-07-18
 */

//=============================================================================
// classID function
//=============================================================================
const CLID& DeFTMat::clID () const {
  return DeFTMat::classID();
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode DeFTMat::initialize(){
  IDetectorElement* module = this->parentIDetectorElement();
  IDetectorElement* quarter = module->parentIDetectorElement();
  IDetectorElement* layer = quarter->parentIDetectorElement();
  IDetectorElement* station = layer->parentIDetectorElement();

  // Get specific parameters from the module
  unsigned int matID = param<int>("matID");
  unsigned int moduleID = module->params()->param<int>("moduleID");
  unsigned int quarterID = quarter->params()->param<int>("quarterID");
  unsigned int layerID = layer->params()->param<int>("layerID");
  unsigned int stationID = station->params()->param<int>("stationID");
  LHCb::FTChannelID aChan(stationID, layerID, quarterID, moduleID, matID, 0u, 0u);
  setElementID(aChan);

  // Get some useful geometric parameters from the database
  m_airGap = (float)param<double>("airGap");
  m_deadRegion = (float)param<double>("deadRegion");
  m_channelPitch = (float)param<double>("channelPitch");
  m_halfChannelPitch = 0.5*m_channelPitch;
  m_dieGap = (float)param<double>("dieGap");
  m_nChannelsInSiPM = (int)param<int>("nChannelsInSiPM");
  m_nSiPMsInMat = (int)param<int>("nSiPMsInMat");
  m_nDiesInSiPM = (unsigned int)param<int>("nDiesInSiPM");

  m_sipmPitch = m_nChannelsInSiPM*m_channelPitch+m_dieGap+2*m_airGap+2*m_deadRegion;
  m_nChannelsInDie = m_nChannelsInSiPM / m_nDiesInSiPM;
  m_diePitch = m_nChannelsInDie*m_channelPitch+m_dieGap;
  m_uBegin = m_airGap + m_deadRegion - 2.0*m_sipmPitch;

  // Get the boundaries of the layer
  const SolidBox* box = dynamic_cast<const SolidBox*> (geometry()->lvolume()->solid()->coverTop());
  m_sizeX = box->xsize();
  m_sizeY = box->ysize();
  m_sizeZ = box->zsize();

  // Get the central points of the fibres at the mirror and at the SiPM locations
  m_mirrorPoint = geometry()->toGlobal( Gaudi::XYZPoint( 0,-0.5*m_sizeY,0) );
  m_sipmPoint   = geometry()->toGlobal( Gaudi::XYZPoint( 0,+0.5*m_sizeY,0) );

  // Define the global z position to be at the point closest to the mirror
  m_globalZ = m_mirrorPoint.z();

  // Define the global length in y of the mat
  m_globaldy = m_sipmPoint.y()-m_mirrorPoint.y();

  // Make the plane for the mat
  const Gaudi::XYZPoint g1 = geometry() -> toGlobal( Gaudi::XYZPoint(0.,0.,0.) );
  const Gaudi::XYZPoint g2 = geometry() -> toGlobal( Gaudi::XYZPoint(1.,0.,0.) );
  const Gaudi::XYZPoint g3 = geometry() -> toGlobal( Gaudi::XYZPoint(0.,1.,0.) );
  m_plane = Gaudi::Plane3D(g1,g2,g3 );

  // Get the slopes in units of local delta x
  m_ddx = Gaudi::XYZVectorF(g2 - g1);

  // Get the slopes in units of delta y (needed by PrFTHit, mind the sign)
  Gaudi::XYZVectorF deltaY( g1 - g3 );
  m_dxdy = deltaY.x() / deltaY.y();
  m_dzdy = deltaY.z() / deltaY.y();

  return StatusCode::SUCCESS;
}


// determine which channel+fraction belongs to a local x-position
// (used in the digitization)
LHCb::FTChannelID DeFTMat::calculateChannelAndFrac(float localX,
    float& frac) const {

  // Correct for the starting point of the sensitive area
  float xInMat = localX - m_uBegin;

  // Find the sipm that is hit and the local position within the sipm
  int hitSiPM = std::min(std::max(0, int(xInMat / m_sipmPitch)), m_nSiPMsInMat-1);
  float xInSiPM = xInMat - (hitSiPM * m_sipmPitch);

  // Find the die that is hit and the local position within the die
  int hitDie = std::min(std::max(0, int(xInSiPM / m_diePitch)), m_nDiesInSiPM - 1);
  float chanInDie = (xInSiPM - (hitDie * m_diePitch)) / m_channelPitch;

  // Find the channel that is hit and the local position within the channel
  int hitChan = std::min( std::max(0, int(chanInDie)), m_nChannelsInDie-1);
  frac = chanInDie - hitChan - 0.5;

  // Construct channelID
  return LHCb::FTChannelID(stationID(), layerID(), quarterID(), moduleID(), matID(),
      hitSiPM, hitChan+(hitDie*m_nChannelsInDie));
}

// Get the relevant channel boundaries
std::vector<std::pair<LHCb::FTChannelID, float>> DeFTMat::calculateChannels(
    LHCb::FTChannelID thisChannel, LHCb::FTChannelID endChannel) const
{
  // Reserve memory
  std::vector<std::pair<LHCb::FTChannelID, float>> channelsAndLeftEdges;
  channelsAndLeftEdges.reserve(endChannel - thisChannel);

  // Loop over the intermediate channels
  bool keepAdding = true;
  while(keepAdding) {
    float channelLeftEdge = localXfromChannel(thisChannel, -0.5f );
    // Add channel and left edge to output vector.
    channelsAndLeftEdges.emplace_back(thisChannel, channelLeftEdge);
    if( thisChannel == endChannel) keepAdding = false;
    thisChannel.next();
  }
  return channelsAndLeftEdges;
}

// Get the relevant channel boundaries
std::vector<std::pair<LHCb::FTChannelID, float>> DeFTMat::calculateChannels(
    const float localEntry, const float localExit,
    const unsigned int numOfAdditionalChannels ) const
{
  // set ordering in increasing local x
  float xBegin = std::min(localEntry, localExit);
  float xEnd   = std::max(localEntry, localExit);

  // Find the first and last channels that are involved
  float xOffset = numOfAdditionalChannels * m_channelPitch;
  float fracBegin=0.0, fracEnd=0.0;
  LHCb::FTChannelID thisChannel = calculateChannelAndFrac(xBegin-xOffset, fracBegin);
  LHCb::FTChannelID endChannel  = calculateChannelAndFrac(xEnd  +xOffset, fracEnd);

  // return empty vector when both channels are the same gap
  if( thisChannel.channelID() == endChannel.channelID() &&
      std::abs(fracBegin) > 0.5 && std::abs(fracEnd) > 0.5 && fracBegin*fracEnd > 0.25 )
    return std::vector<std::pair<LHCb::FTChannelID, float>>();

  return DeFTMat::calculateChannels( thisChannel, endChannel);
}

// Get all the relevant channel boundaries in a mat
std::vector<std::pair<LHCb::FTChannelID, float>> DeFTMat::calculateChannels() const
{
  LHCb::FTChannelID thisChannel = (stationID(), layerID(), quarterID(), moduleID(), matID(), 0u);
  LHCb::FTChannelID endChannel  = (stationID(), layerID(), quarterID(), moduleID(), matID(),
      m_nChannelsInSiPM * m_nSiPMsInMat - 1);

  return DeFTMat::calculateChannels( thisChannel, endChannel);
}


// Find the local x-position for a given channel+fraction
float DeFTMat::localXfromChannel(const LHCb::FTChannelID channelID,
    const float frac) const {
  float uFromChannel = m_uBegin + (float(channelID.channel())+0.5+frac)*m_channelPitch;
  if( int(channelID.channel()) >= m_nChannelsInDie ) uFromChannel += m_dieGap;
  uFromChannel += channelID.sipm() * m_sipmPitch;
  return uFromChannel;
}

// Get the distance between a 3D global point and a channel+fraction
float DeFTMat::distancePointToChannel(const Gaudi::XYZPoint& globalPoint,
    const LHCb::FTChannelID channelID, const float frac ) const {
  Gaudi::XYZPoint localPoint = geometry()->toLocal( globalPoint );
  return localXfromChannel(channelID, frac) - localPoint.x() ;
}

// Get the begin and end positions of a fibre
std::unique_ptr<LHCb::Trajectory<double>> DeFTMat::trajectory(const LHCb::FTChannelID channelID,
    const float frac) const {
  float localX = localXfromChannel( channelID, frac );
  Gaudi::XYZPoint mirrorPoint(m_mirrorPoint.x() + localX*m_ddx.x(),
                              m_mirrorPoint.y() + localX*m_ddx.y(),
                              m_mirrorPoint.z() + localX*m_ddx.z());
  Gaudi::XYZPoint sipmPoint(m_sipmPoint.x() + localX*m_ddx.x(),
                            m_sipmPoint.y() + localX*m_ddx.y(),
                            m_sipmPoint.z() + localX*m_ddx.z());
  return std::make_unique<LHCb::LineTraj<double>>(mirrorPoint,
                                                  sipmPoint);
}

// Get the endpoints of the line defined by the hit
std::pair<Gaudi::XYZPointF,Gaudi::XYZPointF> DeFTMat::endPoints(
    const LHCb::FTChannelID channelID, const float frac) const{
  float localX = localXfromChannel( channelID, frac );

  Gaudi::XYZVectorF delta(m_ddx*localX);
  return { m_mirrorPoint+delta, m_sipmPoint+delta };
}
