// FTDet
#include "FTDet/DeFTModule.h"

/** @file DeFTModule.cpp
 *
 *  Implementation of class : DeFTModule
 *
 *  @author Jeroen van Tilburg
 *  @date   2016-07-18
 */

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
DeFTModule::DeFTModule( const std::string& name ) :
DetectorElement ( std::move(name) ) {
}

//=============================================================================
// classID function
//=============================================================================
const CLID& DeFTModule::clID () const {
  return DeFTModule::classID();
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode DeFTModule::initialize(){
  IDetectorElement* quarter = this->parentIDetectorElement();
  IDetectorElement* layer = quarter->parentIDetectorElement();
  IDetectorElement* station = layer->parentIDetectorElement();

  // Get specific parameters from the module
  m_moduleID = (unsigned int)param<int>("moduleID");
  m_quarterID = (unsigned int)quarter->params()->param<int>("quarterID");
  m_layerID = (unsigned int)layer->params()->param<int>("layerID");
  m_stationID = (unsigned int)station->params()->param<int>("stationID");
  LHCb::FTChannelID aChan(m_stationID, m_layerID, m_quarterID, m_moduleID, 0u, 0u);
  setElementID(aChan);

  // Get some useful geometric parameters from the database
  m_uBegin = (double)param<double>("uBegin");
  m_airGap = (double)param<double>("airGap");
  m_deadRegion = (double)param<double>("deadRegion");
  m_channelPitch = (double)param<double>("channelPitch");
  m_dieGap = (double)param<double>("dieGap");
  m_nChannelsInSiPM = (int)param<int>("nChannelsInSiPM");
  m_nChannelsInDie = (int)param<int>("nChannelsInDie");
  m_nSiPMsInMat = (int)param<int>("nSiPMsInMat");
  m_nMats = (int)param<int>("nMats");
  m_nDiesInSiPM = (unsigned int)param<int>("nDiesInSiPM");
  m_sipmPitch = (double)param<double>("sipmPitch");
  m_matPitch = (double)param<double>("matPitch");
  m_diePitch = (double)param<double>("diePitch");

  m_nSiPMsInModule = m_nSiPMsInMat * m_nMats;
  m_nChannelsInModule = m_nChannelsInSiPM * m_nSiPMsInModule;

  m_fibreSizeX = (double)param<double>("fibreSizeX");
  m_fibreSizeY = (double)param<double>("fibreSizeY");
  m_fibreSizeZ = (double)param<double>("fibreSizeZ");
  m_holeSizeX  = (double)param<double>("holeSizeX");   ///< Only sensible for special modules
  m_holeSizeY  = (double)param<double>("holeSizeY");   ///< Only sensible for special modules

  // Determine if the pseudoChannel numbering is reversed.
  // (pseudoChannels have increasing x)
  Gaudi::XYZPoint firstPoint = geometry()->toGlobal( Gaudi::XYZPoint( m_uBegin,0,0) );
  Gaudi::XYZPoint lastPoint  = geometry()->toGlobal( Gaudi::XYZPoint(-m_uBegin,0,0) );
  m_reversed = std::abs(firstPoint.x()) > fabs(lastPoint.x());

  // Get the global z position of the module at the point closest to the mirror
  Gaudi::XYZPoint mirrorPoint = geometry()->toGlobal( Gaudi::XYZPoint( 0,-0.5*m_fibreSizeY,0) );
  m_globalZ = mirrorPoint.z();

  // Make the plane for the module
  const Gaudi::XYZPoint g1 = geometry() -> toGlobal( Gaudi::XYZPoint(0.,0.,0.) );
  const Gaudi::XYZPoint g2 = geometry() -> toGlobal( Gaudi::XYZPoint(1.,0.,0.) );
  const Gaudi::XYZPoint g3 = geometry() -> toGlobal( Gaudi::XYZPoint(0.,1.,0.) );
  m_plane = Gaudi::Plane3D(g1,g2,g3 );

  // Find the sensitive daughter volume
  // Ugly for now, remove when we have detector elements for the mats
  for( auto pv : geometry()->lvolume()->pvolumes() ) {
    if( lvolume().sdName() != "" ) m_sensitiveGeometry = pv;
  }

  return StatusCode::SUCCESS;
}

// determine which channel+fraction belongs to a local x-position
// (used in the digitization)
LHCb::FTChannelID DeFTModule::calculateChannelAndFrac(double localX,
    double& frac) const {

  // Correct for the starting point of the sensitive area
  double xLocal = localX - m_uBegin - m_airGap - m_deadRegion;

  // Find the mat that is hit and the local position within the mat
  int hitMat = std::min( std::max(0, int(xLocal / m_matPitch)), m_nMats-1);
  double xInMat = xLocal - (hitMat * m_matPitch);

  // Find the sipm that is hit and the local position within the sipm
  int hitSiPM = std::min(std::max(0, int(xInMat / m_sipmPitch)), m_nSiPMsInMat-1);
  double xInSiPM = xInMat - (hitSiPM * m_sipmPitch);

  // Find the die that is hit and the local position within the die
  int hitDie = std::min(std::max(0, int(xInSiPM / m_diePitch)), m_nDiesInSiPM - 1);
  double xInDie = xInSiPM - (hitDie * m_diePitch);

  // Find the channel that is hit and the local position within the channel
  int hitChan = std::min( std::max(0, int(xInDie / m_channelPitch)), m_nChannelsInDie-1);
  double xInChan = xInDie - (hitChan * m_channelPitch);
  frac = xInChan/m_channelPitch - 0.5;

  // Construct channelID
  return LHCb::FTChannelID(m_stationID, m_layerID, m_quarterID, m_moduleID,
      hitSiPM+(hitMat*m_nSiPMsInMat), hitChan+(hitDie*m_nChannelsInDie));
}

// Get the relevant channel boundaries
std::vector<std::pair<LHCb::FTChannelID, double>> DeFTModule::calculateChannels(
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

  // Estimate of the size of the vector and reserve memory
  std::vector<std::pair<LHCb::FTChannelID, double>> channelsAndLeftEdges;
  int vectorSize = int((xEnd - xBegin)/m_channelPitch)+1+2*numOfAdditionalChannels;
  channelsAndLeftEdges.reserve(vectorSize);

  // return empty vector when both channels are the same gap
  if( thisChannel.channelID() == endChannel.channelID() &&
      std::abs(fracBegin) > 0.5 && std::abs(fracEnd) > 0.5 && fracBegin*fracEnd > 0.25 )
    return channelsAndLeftEdges;

  // Loop over the intermediate channels
  bool keepAdding = true;
  while(keepAdding) {
    double channelLeftEdge = localXfromChannel(thisChannel, -0.5);
    // Add channel and left edge to output vector.
    channelsAndLeftEdges.emplace_back(thisChannel, channelLeftEdge);
    if( thisChannel == endChannel) keepAdding = false;
    else thisChannel.next();
  }
  return channelsAndLeftEdges;
}

// Find the local x-position for a given channel+fraction
double DeFTModule::localXfromChannel(const LHCb::FTChannelID channelID,
    const double frac) const {
  double uFromChannel = m_uBegin + m_airGap + m_deadRegion;
  uFromChannel += (double(channelID.channel())+0.5+frac)*m_channelPitch;
  if( int(channelID.channel()) >= m_nChannelsInDie ) uFromChannel += m_dieGap;
  uFromChannel += (channelID.sipm() / m_nMats) * m_matPitch;
  uFromChannel += (channelID.sipm() % m_nMats) * m_sipmPitch;
  return uFromChannel;
}

// Get the distance between a 3D global point and a channel+fraction
double DeFTModule::distancePointToChannel(const Gaudi::XYZPoint& globalPoint,
    const LHCb::FTChannelID channelID, const double frac ) const {
  Gaudi::XYZPoint localPoint = geometry()->toLocal( globalPoint );
  return localXfromChannel(channelID, frac) - localPoint.x() ;
}

// Get the begin and end positions of a fibre
std::unique_ptr<LHCb::Trajectory> DeFTModule::trajectory(const LHCb::FTChannelID channelID,
    const double frac) const {
  double localX = localXfromChannel( channelID, frac );
  double localY1 = (inHole(localX)) ? -0.5*m_fibreSizeY + m_holeSizeY : -0.5*m_fibreSizeY;
  auto to_global = [g=geometry()](double x, double y, double z) { return g->toGlobal(Gaudi::XYZPoint(x,y,z));};
  return std::make_unique<LHCb::LineTraj>( to_global(localX,           localY1, 0.),
                                           to_global(localX, +0.5*m_fibreSizeY, 0.) );
}

// Get the pseudo-channel for a FTChannelID (useful in the monitoring)
int DeFTModule::pseudoChannel( const LHCb::FTChannelID channelID ) const {
  int channelInModule = channelID.channelID() & (m_nChannelsInModule-1u);
  if( m_reversed ) {
    channelInModule = m_nChannelsInModule - 1 - channelInModule;
  }
  int pseudoChannel = channelInModule + m_moduleID * m_nChannelsInModule;
  return pseudoChannel;
}

LHCb::FTChannelID DeFTModule::channelFromPseudo( const int pseudoChannel ) const {
  int channelInModule = pseudoChannel & (m_nChannelsInModule - 1u);
  if( m_reversed ) {
    channelInModule = m_nChannelsInModule - 1 - channelInModule;
  }
  unsigned int channel = channelInModule & (m_nChannelsInSiPM - 1u);
  unsigned int sipm    = channelInModule / m_nChannelsInSiPM;
  return LHCb::FTChannelID(m_stationID, m_layerID, m_quarterID,
      m_moduleID, sipm, channel);
}
