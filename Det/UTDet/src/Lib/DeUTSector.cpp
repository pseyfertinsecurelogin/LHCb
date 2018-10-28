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
#include "UTDet/DeUTSector.h"
#include "UTDet/DeUTSensor.h"
#include "UTDet/DeUTModule.h"
#include "UTDet/UTDetFun.h"
#include "UTDet/StatusMap.h"

#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/SolidBox.h"

#include <algorithm>

// Kernel
#include "Kernel/LineTraj.h"
#include "Kernel/LHCbID.h"
#include "Kernel/PiecewiseTrajectory.h"
#include "Kernel/UTNames.h"
#include "Kernel/LHCbConstants.h"

#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IUpdateManagerSvc.h"

#include "boost/lexical_cast.hpp"

using namespace LHCb;

/** @file DeUTSector.cpp
 *
 *  Implementation of class :  DeUTSector
 *
 *  @author Andy Beiter (based on code by Jianchun Wang, Matt Needham)
 *  @date   2018-09-04
 *
 */

DeUTSector::DeUTSector( const std::string& name ) :
  DeUTBaseElement( name )
{
}


const CLID& DeUTSector::clID () const
{
  return DeUTSector::classID() ;
}


std::string DeUTSector::hybridType() const
{
  return m_hybridType;
}

std::ostream& DeUTSector::printOut( std::ostream& os ) const{

  // stream to cout
  os << " Sector :  "  << name() << std::endl;
  os << " Nickname: " << m_nickname
     << "\n ID " << id()
     << "\n type  " << type()
     << "\n pitch " << m_pitch
     << "\n strip " << m_nStrip
     << "\n capacitance " << m_capacitance/Gaudi::Units::picofarad
     << "\n dead width " << m_deadWidth
     << "\n center " << globalCentre()
     << "\n Sector status " << sectorStatus()
     << "\n fraction active " << fractionActive()
     << "\n version " << m_versionString
     << std::endl;
  return os;
}

MsgStream& DeUTSector::printOut( MsgStream& os ) const{

  // stream to Msg service
  os << " Sector : \n "  << name() << std::endl;
  os   << " Nickname: " << m_nickname
       << "\n ID " << id()
       << "type \n " << type()
       << " pitch \n " << m_pitch
       << "n strip \n " << m_nStrip
       << " capacitance \n " << m_capacitance/Gaudi::Units::picofarad
       << "dead width \n " << m_deadWidth
       << "\n center " << globalCentre()
       << "\n fraction active " << fractionActive()
       << "\n version " << m_versionString
       << std::endl;

  return os;
}

StatusCode DeUTSector::initialize()
{
  // initialize method
  MsgStream msg(msgSvc(), name() );

  StatusCode sc = DeUTBaseElement::initialize();
  if (sc.isFailure() ){
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
    return sc;
  }
  else {
    m_pitch = param<double>("pitch");
    m_nStrip =  param<int>("numStrips");
    m_capacitance = param<double>("capacitance");
    m_type = param<std::string>("type");


    // version
    if (exists("version") == true) m_versionString = param<std::string>("version");

    // guard ring
    m_deadWidth = param<double>("verticalGuardRing");

    m_noiseValues.assign(m_nStrip, 0);
    m_electronsPerADC.assign(m_nStrip, 1.);

    if (m_versionString != "DC06")
    {
      sc = registerCondition(this,m_statusString,
                             &DeUTSector::updateStatusCondition,true);
      if (sc.isFailure() ){
        msg << MSG::ERROR << "Failed to register status conditions" << endmsg;
        return StatusCode::FAILURE;
      }

      // Try to add the noise from the DB as well..
      // Can't test the version string, it's unfortunalety not trustable
      // it exists a DC07 version (why?) that doesn't contain Noise
      if (m_versionString != "DC07")
      {
        sc = registerCondition(this, m_noiseString,
                               &DeUTSector::updateNoiseCondition, true);

        if (sc.isFailure())
        {
          msg << MSG::ERROR << "Failed to register noise conditions" << endmsg;
          return StatusCode::FAILURE;
        }
      }

    } // !DC06

    // get the parent
    m_parent = getParent<DeUTSector>();

    unsigned int tSize = m_parent->numSectorsExpected();

    // sub id
    const unsigned subID = param<int>("subID");

    // sector number needs info from mother
    if (m_parent->moduleRotZ() == "No") {
      setID(m_parent->firstSector() + subID - 1);
    } else {
      setID(m_parent->firstSector() + tSize - subID);
    }

    // row..
    m_row = id() - m_parent->firstSector() + 1;

    // build the id
    const UTChannelID parentID = m_parent->elementID();
    UTChannelID chan(UTChannelID::detType::typeUT,parentID.station(),parentID.layer(),
                     parentID.detRegion(),  id(), 0);
    setElementID(chan);

    // get the nickname
    m_nickname = UTNames().UniqueSectorToString(chan);

    // see if stereo
    const unsigned int layer = chan.layer();
    m_isStereo = false;
    if ( (chan.station() == UTNames::Station::UTa && layer == 2) ||
         (chan.station() == UTNames::Station::UTb && layer == 1))  m_isStereo = true;

    // get the attached sensors
    std::vector<DeUTSensor*> sensors = getChildren<DeUTSector>();
    std::sort(sensors.begin(),sensors.end(),UTDetFun::SortByY());
    m_sensors.reserve(sensors.size());
    m_sensors.insert(m_sensors.begin(), sensors.begin(), sensors.end());
    m_thickness = m_sensors.front()->thickness();

    m_hybridType = "N";
    if ( DeUTSector::type() == "Dual" ) m_hybridType = "D";
    if ( DeUTSector::type() == "Quad" ) m_hybridType = "Q";

    using namespace boost;
    std::string region = lexical_cast<std::string>(chan.detRegion());
    std::string col    = moduleNumber( chan.detRegion(), parentID.station());
    std::string sector = lexical_cast<std::string>(subID);

    m_conditionPathName = UTNames().UniqueLayerToString(chan) + "LayerR"
      + region + "Module" + col + "Sector" + sector;

    sc = registerConditionsCallbacks();
    if (sc.isFailure()){
      msg << MSG::ERROR << "Failed to registerConditions call backs" << endmsg;
      return sc;
    }
    sc = cacheInfo();
    if (sc.isFailure()){
      msg << MSG::ERROR << "Failed to cache geometry" << endmsg;
      return sc;
    }
  }

//   std::cout << " Sector :  "  << name()
//             << "\n Chan " << elementID()
//             << "\n Nickname: " << nickname()
//             << "\n ID " << id()
//             << "\n type  " << type()
//             << "\n pitch " << pitch()
//             << "\n strip " << nStrip()
//             << "\n capacitance " << capacitance()/Gaudi::Units::picofarad
//             << "\n thickness " << thickness()
//             << "\n measEff " << measEff()
//             << "\n dead width " << deadWidth()
//             << "\n center " << globalCentre()
//             << "\n Sector status " << sectorStatus()
//             << "\n fraction active " << fractionActive()
//             << "\n column " << column()
//             << "\n row " << row()
//             << "\n prodID " << prodID()
//             << "\n conditionsPath " << conditionsPathName()
//             << "\n moduleType " << moduleType()
//             << std::endl;

  return StatusCode::SUCCESS;
}

float DeUTSector::noise(const LHCb::UTChannelID& aChannel) const
{
  // check strip is valid
  if (!isStrip(aChannel.strip())) return 999;

  const Status theStatus = stripStatus(aChannel);

  // take ok strips
  if (theStatus == DeUTSector::OK) return m_noiseValues[aChannel.strip()-1u];

  // and pinholes...
  if (theStatus == DeUTSector::Pinhole) return m_noiseValues[aChannel.strip()-1u];

  return 999;
}

float DeUTSector::rawNoise(const LHCb::UTChannelID& aChannel) const{
  return sqrt(noise(aChannel)*noise(aChannel) + cmNoise(aChannel)*cmNoise(aChannel));
}

float DeUTSector::rawSectorNoise() const{
  return sqrt(sectorNoise()*sectorNoise() + cmSectorNoise()*cmSectorNoise());
}

float DeUTSector::rawBeetleNoise(const unsigned int& beetle) const{
  return sqrt(beetleNoise(beetle)*beetleNoise(beetle) + cmBeetleNoise(beetle)*cmBeetleNoise(beetle));
}

float DeUTSector::rawPortNoise(const unsigned int& beetle, const unsigned int& port) const{
  return sqrt(portNoise(beetle,port)*portNoise(beetle,port) + cmPortNoise(beetle,port)*cmPortNoise(beetle,port));
}

float DeUTSector::sectorNoise() const{
  const std::vector<DeUTSector::Status> statusVector = stripStatus();

  float sum(0.0), number(0.0);

  for (unsigned int chan(0); chan < m_nStrip; ++chan)
  {
    if ( statusVector[chan] == DeUTSector::OK ||
         statusVector[chan] == DeUTSector::Pinhole )
    {
      sum    += m_noiseValues[chan];
      number += 1.;
    }
  }

  if (number < 1.)
    return 999.99;
  else
  {
    MsgStream msg(msgSvc(), name() );
    if( UNLIKELY( msg.level() <= MSG::DEBUG ) )
      msg << MSG::DEBUG << number << " strips out of " << nStrip()
          << " are not taken into account" << endmsg;
    return sum / number;
  }
}

float DeUTSector::beetleNoise(const unsigned int& beetle) const
{
  if (beetle > nBeetle())
   {
     MsgStream msg(msgSvc(), name() );
     msg << MSG::WARNING << "You asked for beetle " << beetle
         << " but there are " << nBeetle() << " of them" << endmsg;
     return 999.99;
   }
  else if (beetle == 0)
  {
    MsgStream msg(msgSvc(), name() );
    msg << MSG::WARNING << "You asked for beetle 0 but is starts at 1"
        << endmsg;
     return 999.99;
  }

  const std::vector<DeUTSector::Status> statusVector = stripStatus();

  float sum(0.), number(0.);

  for (unsigned int chan((beetle - 1) * LHCbConstants::nStripsInBeetle);
       chan < beetle * LHCbConstants::nStripsInBeetle; chan++)
  {
    if ( statusVector[chan] == DeUTSector::OK ||
         statusVector[chan] == DeUTSector::Pinhole )
    {
      sum    += m_noiseValues[chan];
      number += 1.;
    }
  }

  if (number < 1.)
    return 999.99;
  else
  {
    MsgStream msg(msgSvc(), name() );
    if( UNLIKELY( msg.level() <= MSG::DEBUG ) )
      msg << MSG::DEBUG << number << " strips out of "
          << LHCbConstants::nStripsInBeetle
          << " are not taken into account" << endmsg;
    return sum / number;
  }
}

float DeUTSector::portNoise(const unsigned int& beetle,
                             const unsigned int& port) const
{
  if (beetle > nBeetle())
  {
    MsgStream msg(msgSvc(), name() );
    msg << MSG::WARNING << "You asked for beetle " << beetle
        << " but there are " << nBeetle() << " of them" << endmsg;
    return 999.99;
  }
  else if (beetle == 0)
  {
    MsgStream msg(msgSvc(), name() );
    msg << MSG::WARNING << "You asked for beetle 0 but is starts at 1"
        << endmsg;
    return 999.99;
  }

  if (port > 4)
  {
    MsgStream msg(msgSvc(), name() );
    msg << MSG::WARNING << "You asked for port " << port
        << " but there are 4 of them" << endmsg;
    return 999.99;
  }
  else if (port == 0)
  {
    MsgStream msg(msgSvc(), name() );
    msg << MSG::WARNING << "You asked for port 0 but is starts at 1"
        << endmsg;
    return 0.;
  }

  const std::vector<DeUTSector::Status> statusVector = stripStatus();

  float sum(0.), number(0.);

  for (unsigned int chan((beetle - 1) * LHCbConstants::nStripsInBeetle +
                         (port - 1) * LHCbConstants::nStripsInPort);
       chan < (beetle - 1) * LHCbConstants::nStripsInBeetle +
         port * LHCbConstants::nStripsInPort; chan++)
  {
    if ( statusVector[chan] == DeUTSector::OK ||
         statusVector[chan] == DeUTSector::Pinhole )
    {
      sum    += m_noiseValues[chan];
      number += 1.;
    }
  }

  if (number < 1.)
    return 999.99;
  else
  {
    MsgStream msg(msgSvc(), name() );
    if( UNLIKELY( msg.level() <= MSG::DEBUG ) )
      msg << MSG::DEBUG << number << " strips out of "
          << LHCbConstants::nStripsInPort
          << " are not taken into account" << endmsg;
    return sum / number;
  }
}

void DeUTSector::setNoise(const unsigned int& strip, const float& value)
{
  Condition* aCon = condition(m_noiseString);
  if (aCon == nullptr){
    MsgStream msg(msgSvc(), name());
    msg << MSG::ERROR << "Failed to find status condition" << endmsg;
  }
  else
  {
    std::vector<float>& reference =
      aCon -> param< std::vector< float > >( "SectorNoise" );
    reference[strip-1u] = value;
    m_noiseValues[strip-1u] = value;
  }
}

void DeUTSector::setNoise(const std::vector<float>& values)
{
  Condition* aCon( condition(m_noiseString) );
  if (aCon == nullptr)
  {
    MsgStream msg(msgSvc(), name());
    msg << MSG::ERROR << "Failed to find status condition" << endmsg;
  }
  else
  {
    std::vector<float>& reference =
      aCon -> param< std::vector< float > >( "SectorNoise" );
    reference.assign( values.begin(), values.end() );
    m_noiseValues = values;
  }
}

void DeUTSector::setCMNoise(const unsigned int& strip, const float& value)
{
  Condition* aCon = condition(m_noiseString);
  if (aCon == nullptr){
    MsgStream msg(msgSvc(), name());
    msg << MSG::ERROR << "Failed to find status condition" << endmsg;
  }
  else
  {
    std::vector<float>& reference =
      aCon -> param< std::vector< float > >( "cmNoise" );
    reference[strip-1u] = value;
    m_cmModeValues[strip-1u] = value;
  }
}

void DeUTSector::setCMNoise(const std::vector<float>& values)
{
  Condition* aCon( condition(m_noiseString) );
  if (aCon == nullptr)
  {
    MsgStream msg(msgSvc(), name());
    msg << MSG::ERROR << "Failed to find status condition" << endmsg;
  }
  else
  {
    std::vector<float>& reference =
      aCon -> param< std::vector< float > >( "cmNoise" );
    reference.assign( values.begin(), values.end() );
    m_cmModeValues = values;
  }
}

void DeUTSector::setADCConversion(const std::vector<double>& values)
{
  Condition* aCon( condition(m_noiseString) );
  if (aCon == nullptr)
  {
    MsgStream msg(msgSvc(), name());
    msg << MSG::ERROR << "Failed to find status condition" << endmsg;
  }
  else
  {
    std::vector<double>& reference =
      aCon -> param< std::vector< double > >( "electronsPerADC" );
    reference.assign( values.begin(), values.end() );
    m_electronsPerADC = values;
  }
}

float DeUTSector::cmNoise(const LHCb::UTChannelID& aChannel) const
{
  // check strip is valid
  if (!isStrip(aChannel.strip())) return 999;

  const Status theStatus = stripStatus(aChannel);

  // take ok strips
  if (theStatus == DeUTSector::OK) return m_cmModeValues[aChannel.strip()-1u];

  // and pinholes...
  if (theStatus == DeUTSector::Pinhole) return m_cmModeValues[aChannel.strip()-1u];

  return 999;
}


float DeUTSector::cmSectorNoise() const
{
  const std::vector<DeUTSector::Status> statusVector = stripStatus();

  float sum(0.0), number(0.0);

  for (unsigned int chan(0); chan < m_nStrip; ++chan)
  {
    if ( statusVector[chan] == DeUTSector::OK ||
         statusVector[chan] == DeUTSector::Pinhole )
    {
      sum    += m_cmModeValues[chan];
      number += 1.;
    }
  }

  if (number < 1.)
    return 999.99;
  else
  {
    MsgStream msg(msgSvc(), name() );
    if( UNLIKELY( msg.level() <= MSG::DEBUG ) )
      msg << MSG::DEBUG << number << " strips out of " << nStrip()
          << " are not taken into account" << endmsg;
    return sum / number;
  }
}

float DeUTSector::cmBeetleNoise(const unsigned int& beetle) const
{
  if (beetle > nBeetle())
   {
     MsgStream msg(msgSvc(), name() );
     msg << MSG::WARNING << "You asked for beetle " << beetle
         << " but there are " << nBeetle() << " of them" << endmsg;
     return 999.99;
   }
  else if (beetle == 0)
  {
    MsgStream msg(msgSvc(), name() );
    msg << MSG::WARNING << "You asked for beetle 0 but is starts at 1"
        << endmsg;
     return 999.99;
  }

  const std::vector<DeUTSector::Status> statusVector = stripStatus();

  float sum(0.), number(0.);

  for (unsigned int chan((beetle - 1) * LHCbConstants::nStripsInBeetle);
       chan < beetle * LHCbConstants::nStripsInBeetle; chan++)
  {
    if ( statusVector[chan] == DeUTSector::OK ||
         statusVector[chan] == DeUTSector::Pinhole )
    {
      sum    += m_cmModeValues[chan];
      number += 1.;
    }
  }

  if (number < 1.)
    return 999.99;
  else
  {
    MsgStream msg(msgSvc(), name() );
    if( UNLIKELY( msg.level() <= MSG::DEBUG ) )
      msg << MSG::DEBUG << number << " strips out of "
          << LHCbConstants::nStripsInBeetle
          << " are not taken into account" << endmsg;
    return sum / number;
  }
}

float DeUTSector::cmPortNoise(const unsigned int& beetle,
                               const unsigned int& port) const
{
  if (beetle > nBeetle())
  {
    MsgStream msg(msgSvc(), name() );
    msg << MSG::WARNING << "You asked for beetle " << beetle
        << " but there are " << nBeetle() << " of them" << endmsg;
    return 999.99;
  }
  else if (beetle == 0)
  {
    MsgStream msg(msgSvc(), name() );
    msg << MSG::WARNING << "You asked for beetle 0 but is starts at 1"
        << endmsg;
    return 999.99;
  }

  if (port > 4)
  {
    MsgStream msg(msgSvc(), name() );
    msg << MSG::WARNING << "You asked for port " << port
        << " but there are 4 of them" << endmsg;
    return 999.99;
  }
  else if (port == 0)
  {
    MsgStream msg(msgSvc(), name() );
    msg << MSG::WARNING << "You asked for port 0 but is starts at 1"
        << endmsg;
    return 0.;
  }

  const std::vector<DeUTSector::Status> statusVector = stripStatus();

  float sum(0.), number(0.);

  for (unsigned int chan((beetle - 1) * LHCbConstants::nStripsInBeetle +
                         (port - 1) * LHCbConstants::nStripsInPort);
       chan < (beetle - 1) * LHCbConstants::nStripsInBeetle +
         port * LHCbConstants::nStripsInPort; chan++)
  {
    if ( statusVector[chan] == DeUTSector::OK ||
         statusVector[chan] == DeUTSector::Pinhole )
    {
      sum    += m_cmModeValues[chan];
      number += 1.;
    }
  }

  if (number < 1.)
    return 999.99;
  else
  {
    MsgStream msg(msgSvc(), name() );
    if( UNLIKELY( msg.level() <= MSG::DEBUG ) )
      msg << MSG::DEBUG << number << " strips out of "
          << LHCbConstants::nStripsInPort
          << " are not taken into account" << endmsg;
    return sum / number;
  }
}


double DeUTSector::toADC(const double& e,
                         const LHCb::UTChannelID& aChannel) const
{
  return toADC(e, aChannel.strip());
}

double DeUTSector::toADC(const double& e, const unsigned int& aStrip) const
{
  return e / m_electronsPerADC[aStrip-1];
}

double DeUTSector::toElectron(const double& val,
                              const LHCb::UTChannelID& aChannel) const
{
  return toElectron(val, aChannel.strip());
}

double DeUTSector::toElectron(const double& val,
                              const unsigned int& aStrip) const
{
  return val * m_electronsPerADC[aStrip-1];
}

std::unique_ptr<LHCb::Trajectory<double>>
DeUTSector::trajectory(const UTChannelID& aChan, const double offset) const
{

  if (!contains(aChan)){
    MsgStream msg(msgSvc(), name() );
    msg << MSG::ERROR << "Failed to link to sector " << nickname()
        << " test strip  number "  << aChan.strip() << " strip "  << endmsg;
    throw GaudiException( "Failed to make trajectory",
                          "DeUTSector.cpp", StatusCode::FAILURE );
  }

  return createTraj(aChan.strip(), offset);
}

std::unique_ptr<LHCb::Trajectory<double>> DeUTSector::trajectoryFirstStrip() const
{
  return createTraj(m_firstStrip,0.);
}

std::unique_ptr<LHCb::Trajectory<double>> DeUTSector::trajectoryLastStrip() const
{
  return createTraj(nStrip(), 0.);
}

std::unique_ptr<LHCb::Trajectory<double>> DeUTSector::createTraj(const unsigned int strip,
                                                                 const double offset) const{
  // collect the individual traj
  const Sensors& theSensors = sensors();

  if (theSensors.size() == 1){ // can just return a line traj
    return theSensors.front()->trajectory(strip,offset);
  }
  // return a piecewise traj
  auto traj = std::make_unique<UTTraj>();
  for (auto iterS = theSensors.begin(); iterS != theSensors.end(); ++iterS) {
    auto sensTraj = (*iterS)->trajectory(strip,offset);
    if (traj->numberOfPieces() == 0) {
      traj->append(std::move(sensTraj));
    } else {
      const double d1 = (sensTraj->beginPoint()-traj->endPoint()).mag2();
      const double d2 = (sensTraj->endPoint()-traj->beginPoint()).mag2();
      if (d1 < d2) {
        double mu = sensTraj->muEstimate(traj->endPoint());
        sensTraj->setRange(mu,sensTraj->endRange());
        traj->append(std::move(sensTraj));
      } else {
        const double mu = sensTraj->muEstimate(traj->beginPoint());
        sensTraj->setRange(sensTraj->beginRange(),mu);
        traj->prepend(std::move(sensTraj));
      }
    }
  } // loop
  return traj;
}

StatusCode DeUTSector::cacheInfo()
{
  auto firstTraj = createTraj(m_firstStrip,0);

  // get the start and end point. for piecewise trajectories, we
  // effectively make an approximation by a straight line.
  const Gaudi::XYZPoint g1 = firstTraj->beginPoint();
  const Gaudi::XYZPoint g2 = firstTraj->endPoint();

  const double activeWidth = m_sensors.front()->activeWidth();

  // direction
  Gaudi::XYZVector direction = g2 - g1;
  m_stripLength = sqrt(direction.Mag2());
  direction = direction.Unit();

  // cross with normal along z
  Gaudi::XYZVector zVec(0,0,1);
  Gaudi::XYZVector norm = direction.Cross(zVec);

  // trajectory of middle
  const Gaudi::XYZPoint g3 = g1 + 0.5*(g2 - g1);
  const Gaudi::XYZPoint g4 = g3 + activeWidth*norm ;

  // creating the 'fast' trajectories
  const Gaudi::XYZVector vectorlayer = (g4-g3).unit() * m_pitch ;
  const Gaudi::XYZPoint p0 = g3-0.5*m_stripLength*direction ;
  m_dxdy = direction.x()/direction.y() ;
  m_dzdy = direction.z()/direction.y() ;
  m_dy   = m_stripLength * direction.y() ;
  m_dp0di.SetX( vectorlayer.x() - vectorlayer.y() * m_dxdy ) ;
  m_dp0di.SetY( vectorlayer.y()  ) ;
  m_dp0di.SetZ( vectorlayer.z() - vectorlayer.y() * m_dzdy ) ;
  m_p0.SetX( p0.x() - p0.y() * m_dxdy ) ;
  m_p0.SetY( p0.y() ) ;
  m_p0.SetZ( p0.z() - p0.y() * m_dzdy ) ;

  // Update the stereo angle. We correct by 'pi' if necessary.
  Gaudi::XYZVector dir = direction ;
  if(dir.y()<0) dir *= -1 ;
  m_angle    = atan2(-dir.x(),dir.y()) ;
  m_cosAngle = cos( m_angle ) ;
  m_sinAngle = sin( m_angle ) ;

  return StatusCode::SUCCESS;

}

UTChannelID DeUTSector::nextLeft(const UTChannelID testChan) const
{
  if ((contains(testChan))&& (isStrip(testChan.strip()- 1u) == true)){
    return UTChannelID(testChan.type(),
                       testChan.station(),
                       testChan.layer(),
                       testChan.detRegion(),
                       testChan.sector(),
                       testChan.strip() - 1u);
  }
  else {
    return LHCb::UTChannelID(0u);
  }
}

UTChannelID DeUTSector::nextRight(const LHCb::UTChannelID testChan) const
{
  if ((contains(testChan) == true)&& (isStrip(testChan.strip()+ 1u) == true)){
    return UTChannelID(testChan.type(),
                       testChan.station(),
                       testChan.layer(),
                       testChan.detRegion(),
                       testChan.sector(),
                       testChan.strip() + 1u);
  }
  else {
    return LHCb::UTChannelID(0u);
  }
}


StatusCode DeUTSector::registerConditionsCallbacks(){

  // cache trajectories
  // initialize method
  MsgStream msg(msgSvc(), name() );

  if (sensors().empty()){
    msg << MSG::ERROR << "Sterile detector element ! No conditions registered" << endmsg;
    return StatusCode::FAILURE;
  }

  StatusCode sc = registerCondition(this,sensors().front(),&DeUTSector::cacheInfo,true);
  if (sc.isFailure() ){
    msg << MSG::ERROR << "Failed to register geometry condition for first child" << endmsg;
    return StatusCode::FAILURE;
  }

  sc = registerCondition(this,sensors().back(),&DeUTSector::cacheInfo,true);
  if (sc.isFailure() ){
    msg << MSG::ERROR << "Failed to register geometry condition for first child" << endmsg;
    return StatusCode::FAILURE;
  }

  return StatusCode::SUCCESS;
}

StatusCode DeUTSector::updateStatusCondition(){

  Condition* aCon = const_cast<Condition*>(statusCondition());
  if (aCon == nullptr){
    MsgStream msg(msgSvc(), name());
    msg << "failed to find status condition" << endmsg;
    return StatusCode::FAILURE;
  }

  const int tStatus = aCon->param<int>("SectorStatus");
  m_status = Status(tStatus);

  std::map<int,int> beetleMap = aCon->param<std::map<int,int> >("BeetleStatus");
  toEnumMap(beetleMap,m_beetleStatus);

  std::map<int,int> stripMap = aCon->param<std::map<int,int> >("StripStatus");
  toEnumMap(stripMap,m_stripStatus);

  if (aCon->exists("measuredEff") == true){
    m_measEff = aCon-> param<double >("measuredEff");
  }
  else {
    m_measEff = 1.0;
    aCon->addParam("measuredEff", m_measEff, "Measured sector Eff");
  }

  return StatusCode::SUCCESS;
}

StatusCode DeUTSector::updateNoiseCondition()
{
  Condition* aCon = condition(m_noiseString);
  if (aCon == nullptr){
    MsgStream msg(msgSvc(), name());
    msg << MSG::ERROR << "failed to find noise condition" << endmsg;
    return StatusCode::FAILURE;
  }

  std::vector<double > tmpNoise = aCon -> param<std::vector< double > >("SectorNoise");

  if (tmpNoise.size() == m_nStrip)
    m_noiseValues.assign(tmpNoise.begin(), tmpNoise.end());
  else
  {
    MsgStream msg(msgSvc(), name());
    msg << MSG::ERROR << "Size mismatch for SectorNoise" << endmsg;
    return StatusCode::FAILURE;
  }

  std::vector<double > tmpElectrons = aCon -> param<std::vector< double > >("electronsPerADC");

  if (tmpElectrons.size() == m_nStrip)
    m_electronsPerADC.assign(tmpElectrons.begin(), tmpElectrons.end());
  else
  {
    MsgStream msg(msgSvc(), name());
    msg << MSG::ERROR << "Size mismatch for electronsPerADC" << endmsg;
    return StatusCode::FAILURE;
  }

  if (aCon->exists("cmNoise") == true){
    std::vector <double > tmpCM =  aCon -> param<std::vector< double > >("cmNoise");
    if (tmpCM.size() == m_nStrip)
      m_cmModeValues.assign(tmpCM.begin(), tmpCM.end());
    else {
     MsgStream msg(msgSvc(), name());
     msg << MSG::ERROR << "Size mismatch for cmNoise" << endmsg;
     return StatusCode::FAILURE;
    }
  }
  else {
    // doesn't exists (MC early databases...)
    m_cmModeValues.assign(m_nStrip, 0.);
    //for (unsigned int i = 0; i < m_nStrip; ++i) m_cmModeValues[i] = 0.0;
    aCon->addParam("cmNoise", m_cmModeValues, "Common mode per sector");
  }

  return StatusCode::SUCCESS;
}

void DeUTSector::toEnumMap(const std::map<int,int>& input, DeUTSector::StatusMap& output) {
  output.clear();
  std::map<int,int>::const_iterator iterM = input.begin();
  for (; iterM != input.end(); ++iterM){
    output[iterM->first] = Status(iterM->second);
  } // iterM

}

DeUTSensor* DeUTSector::findSensor(const Gaudi::XYZPoint& point) const{
  // return pointer to the layer from point
  auto iter = std::find_if( m_sensors.begin(), m_sensors.end(),
                            [&](const DeUTSensor* s) {
                      return s->isInside(point);
                  } );
  return iter != m_sensors.end() ? *iter: nullptr;
}

bool DeUTSector::globalInActive(const Gaudi::XYZPoint& point,
                                Gaudi::XYZPoint tol) const{
  return std::any_of( m_sensors.begin(), m_sensors.end(),
                      [&](const DeUTSensor* s) {
                          return s->globalInActive(point,tol);
                      });
}

bool DeUTSector::globalInBondGap(const Gaudi::XYZPoint& point,
                                 double tol) const
{
  const DeUTSensor* aSensor =  findSensor(point);
  return (aSensor ?  aSensor->globalInBondGap(point, tol) : false );
}

double DeUTSector::fractionActive() const {

  // fraction of the sector that works
  unsigned int nActive = 0u;
  const std::vector<DeUTSector::Status> statusVector = stripStatus();
  std::vector<DeUTSector::Status>::const_iterator iter = statusVector.begin();
  for (; iter != statusVector.end(); ++iter){
    if ( *iter == DeUTSector::OK) ++nActive;
  }

  return nActive/double(nStrip());
}

void DeUTSector::setMeasEff(const double value){
  m_measEff = value;
  Condition* aCon = condition(m_statusString);
  if (aCon == nullptr){
    MsgStream msg(msgSvc(), name());
    msg << MSG::ERROR << "Failed to find status condition" << endmsg;
  } else {
    double& tvalue = aCon->param<double>("measuredEff");
    tvalue = double(value);
  }
}

void DeUTSector::setSectorStatus(const DeUTSector::Status& newStatus)
{
  m_status = newStatus;

  // Set the condition
  Condition* aCon = condition(m_statusString);
  if (aCon == nullptr){
    MsgStream msg(msgSvc(), name());
    msg << MSG::ERROR << "Failed to find status condition" << endmsg;
  } else {
    int& value = aCon->param<int>("SectorStatus");
    value = int(newStatus);
  }
}

void DeUTSector::setBeetleStatus(const unsigned int beetle,
                                 const DeUTSector::Status& newStatus){

  // update the beetle status properly...
  MsgStream msg(msgSvc(), name());

  if (sectorStatus() != DeUTSector::OK){
    // if the sector is not ok nothing to be done
    if( UNLIKELY( msg.level() <= MSG::DEBUG ) )
      msg << MSG::DEBUG << "Sector is off anyway: set request ignored " << endmsg;
  }
  else {
    if (newStatus == DeUTSector::OK){
      // Lazarus walks...if we have an entry in the map delete it
      m_beetleStatus.erase(beetle);
      setStatusCondition("BeetleStatus", beetle, newStatus);
    }
    else {
      // death comes to this beetle, update the map
      if (std::find(::Status::validBeetleStates().begin(),
                    ::Status::validBeetleStates().end(),
                    newStatus) != ::Status::validBeetleStates().end() ){
        m_beetleStatus[beetle] = newStatus;
        setStatusCondition("BeetleStatus", beetle, newStatus);
      } // check is valid state
      else {
        msg << "Not a valid Beetle state: set request ignored " << endmsg;
      }
    }
  }
}

void DeUTSector::setStripStatus( const unsigned int strip,
                                 const DeUTSector::Status& newStatus )
{
  // update the strip status properly...
  MsgStream msg(msgSvc(), name());

  if(sectorStatus() != DeUTSector::OK || beetleStatus(strip) != DeUTSector::OK){
    // if the sector is not ok nothing to be done
    if( UNLIKELY( msg.level() <= MSG::DEBUG ) )
      msg << MSG::DEBUG << "Sector/Beetle is off anyway: set request ignored "
          << endmsg;
  }
  else {
    if (newStatus == DeUTSector::OK){
      // Lazarus walks...if we have an entry in the map delete it
      m_stripStatus.erase(strip);
      setStatusCondition("StripStatus", strip, newStatus);
    }
    else {
      // death comes to this strip, update the map
      Status oldStatus = m_stripStatus.find(strip)->second;
      if (std::find(::Status::protectedStates().begin(),
                    ::Status::protectedStates().end(),
                    oldStatus) == ::Status::protectedStates().end() ){
        m_stripStatus[strip] = newStatus;
        setStatusCondition("StripStatus", strip, newStatus);
      }
      else {
        msg << "Strip in protected state: set request ignored " << endmsg;
      }
    }
  }
}

void DeUTSector::setStatusCondition( const std::string& type,
                                     const unsigned int entry,
                                     const DeUTSector::Status& newStatus )
{
  // Set the condition
  Condition* aCon = condition(m_statusString);
  if (aCon == nullptr){
    MsgStream msg(msgSvc(), name());
    msg << MSG::ERROR << "Failed to find status condition" << endmsg;
  } else {
    std::map<int,int>& condMap =
      aCon->param<std::map<int,int> >(type);
    condMap[entry] = int(newStatus);
  }
}

unsigned int DeUTSector::prodID() const
{
  return m_parent->prodID();
}


std::string DeUTSector::conditionsPathName() const
{
  return m_conditionPathName;
}


std::string DeUTSector::moduleNumber(const unsigned int& reg, const unsigned int& station) const
{
  using namespace boost;

  int col = 0;

  // UTaX or UTaU
  if(station == 1) {
    switch( reg ) {
      case 1: col = column();
              break;
      case 2: col = column() - 6;
              break;
      case 3: col = column() - 6 - 6;
              break;
    }
  } else if (station == 2) {// UTbV or UTbX
    switch( reg ) {
      case 1: col = column();
              break;
      case 2: col = column() - 7;
              break;
      case 3: col = column() - 7 - 6;
              break;
    }
  }


  return lexical_cast<std::string>(col);
}
