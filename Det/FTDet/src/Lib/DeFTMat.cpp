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
  m_airGap = (double)param<double>("airGap");
  m_deadRegion = (double)param<double>("deadRegion");
  m_channelPitch = (double)param<double>("channelPitch");
  m_dieGap = (double)param<double>("dieGap");
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

  // Make the plane for the mat
  const Gaudi::XYZPoint g1 = geometry() -> toGlobal( Gaudi::XYZPoint(0.,0.,0.) );
  const Gaudi::XYZPoint g2 = geometry() -> toGlobal( Gaudi::XYZPoint(1.,0.,0.) );
  const Gaudi::XYZPoint g3 = geometry() -> toGlobal( Gaudi::XYZPoint(0.,1.,0.) );
  m_plane = Gaudi::Plane3D(g1,g2,g3 );

  // Get the slopes in units of local delta x
  m_ddx = Gaudi::XYZVector(g2 - g1);

  // Get the slopes in units of delta y (needed by PrFTHit, mind the sign)
  Gaudi::XYZVector deltaY( g1 - g3 );
  m_dxdy = deltaY.x() / deltaY.y();
  m_dzdy = deltaY.z() / deltaY.y();

  return StatusCode::SUCCESS;
}


// determine which channel+fraction belongs to a local x-position
// (used in the digitization)
LHCb::FTChannelID DeFTMat::calculateChannelAndFrac(double localX,
    double& frac) const {

  // Correct for the starting point of the sensitive area
  double xInMat = localX - m_uBegin;

  // Find the sipm that is hit and the local position within the sipm
  int hitSiPM = std::min(std::max(0, int(xInMat / m_sipmPitch)), m_nSiPMsInMat-1);
  double xInSiPM = xInMat - (hitSiPM * m_sipmPitch);

  // Find the die that is hit and the local position within the die
  int hitDie = std::min(std::max(0, int(xInSiPM / m_diePitch)), m_nDiesInSiPM - 1);
  double chanInDie = (xInSiPM - (hitDie * m_diePitch)) / m_channelPitch;

  // Find the channel that is hit and the local position within the channel
  int hitChan = std::min( std::max(0, int(chanInDie)), m_nChannelsInDie-1);
  frac = chanInDie - hitChan - 0.5;

  // Construct channelID
  return LHCb::FTChannelID(stationID(), layerID(), quarterID(), moduleID(), matID(),
      hitSiPM, hitChan+(hitDie*m_nChannelsInDie));
}

// Get the relevant channel boundaries
std::vector<std::pair<LHCb::FTChannelID, double>> DeFTMat::calculateChannels(
    LHCb::FTChannelID thisChannel, LHCb::FTChannelID endChannel) const
{
  // Reserve memory
  std::vector<std::pair<LHCb::FTChannelID, double>> channelsAndLeftEdges;
  channelsAndLeftEdges.reserve(endChannel - thisChannel);

  // Loop over the intermediate channels
  bool keepAdding = true;
  while(keepAdding) {
    double channelLeftEdge = localXfromChannel(thisChannel, -0.5);
    // Add channel and left edge to output vector.
    channelsAndLeftEdges.emplace_back(thisChannel, channelLeftEdge);
    if( thisChannel == endChannel) keepAdding = false;
    thisChannel.next();
  }
  return channelsAndLeftEdges;
}

// Get the relevant channel boundaries
std::vector<std::pair<LHCb::FTChannelID, double>> DeFTMat::calculateChannels(
    const double localEntry, const double localExit,
    const unsigned int numOfAdditionalChannels ) const
{
  // set ordering in increasing local x
  double xBegin = std::min(localEntry, localExit);
  double xEnd   = std::max(localEntry, localExit);

  // Find the first and last channels that are involved
  double xOffset = numOfAdditionalChannels * m_channelPitch;
  double fracBegin=0.0, fracEnd=0.0;
  LHCb::FTChannelID thisChannel = calculateChannelAndFrac(xBegin-xOffset, fracBegin);
  LHCb::FTChannelID endChannel  = calculateChannelAndFrac(xEnd  +xOffset, fracEnd);

  // return empty vector when both channels are the same gap
  if( thisChannel.channelID() == endChannel.channelID() &&
      std::abs(fracBegin) > 0.5 && std::abs(fracEnd) > 0.5 && fracBegin*fracEnd > 0.25 )
    return std::vector<std::pair<LHCb::FTChannelID, double>>();

  return DeFTMat::calculateChannels( thisChannel, endChannel);
}

// Get all the relevant channel boundaries in a mat
std::vector<std::pair<LHCb::FTChannelID, double>> DeFTMat::calculateChannels() const
{
  LHCb::FTChannelID thisChannel = (stationID(), layerID(), quarterID(), moduleID(), matID(), 0u);
  LHCb::FTChannelID endChannel  = (stationID(), layerID(), quarterID(), moduleID(), matID(),
      m_nChannelsInSiPM * m_nSiPMsInMat - 1);

  return DeFTMat::calculateChannels( thisChannel, endChannel);
}


// Find the local x-position for a given channel+fraction
double DeFTMat::localXfromChannel(const LHCb::FTChannelID channelID,
    const double frac) const {
  double uFromChannel = m_uBegin + (double(channelID.channel())+0.5+frac)*m_channelPitch;
  if( int(channelID.channel()) >= m_nChannelsInDie ) uFromChannel += m_dieGap;
  uFromChannel += channelID.sipm() * m_sipmPitch;
  return uFromChannel;
}

// Get the distance between a 3D global point and a channel+fraction
double DeFTMat::distancePointToChannel(const Gaudi::XYZPoint& globalPoint,
    const LHCb::FTChannelID channelID, const double frac ) const {
  Gaudi::XYZPoint localPoint = geometry()->toLocal( globalPoint );
  return localXfromChannel(channelID, frac) - localPoint.x() ;
}

// Get the begin and end positions of a fibre
std::unique_ptr<LHCb::Trajectory> DeFTMat::trajectory(const LHCb::FTChannelID channelID,
    const double frac) const {
  double localX = localXfromChannel( channelID, frac );

  Gaudi::XYZVector delta(m_ddx*localX);
  return std::unique_ptr<LHCb::Trajectory>(new LHCb::LineTraj(m_mirrorPoint+delta,
                                                              m_sipmPoint+delta ));

}

// Get the endpoints of the line defined by the hit
std::pair<Gaudi::XYZPoint,Gaudi::XYZPoint> DeFTMat::endPoints(
    const LHCb::FTChannelID channelID, const double frac) const{
  double localX = localXfromChannel( channelID, frac );

  Gaudi::XYZVector delta(m_ddx*localX);
  return std::make_pair<Gaudi::XYZPoint>( m_mirrorPoint+delta,
                                          m_sipmPoint+delta );
}
