/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
/// Kernel
#include "Kernel/LHCbID.h"
#include "Kernel/OTChannelID.h"
#include "Kernel/Trajectory.h"

// GaudiKernel
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/Point3DTypes.h"

/// OTDet
#include "OTDet/DeOTDetector.h"
#include "OTDet/DeOTLayer.h"
#include "OTDet/DeOTModule.h"
#include "OTDet/DeOTQuarter.h"
#include "OTDet/DeOTStation.h"

// Kernel/LHCbKernel

/** @file DeOTDetector.cpp
 *
 *  Implementation of class :  DeOTDetector
 *
 *  @author Jeroen van Tilburg jtilburg@nikhef.nl
 */

using namespace LHCb;

DeOTDetector::DeOTDetector( const std::string& name ) : DetectorElement( name ) {
  /// Constructor
  m_stations.reserve( 3 );
  m_layers.reserve( 4 * 3 );
  m_quarters.reserve( 4 * 4 * 3 );
  m_modules.reserve( 9 * 4 * 4 * 3 );
}

const CLID& DeOTDetector::clID() const { return DeOTDetector::classID(); }

/// Initialise
StatusCode DeOTDetector::initialize() {
  StatusCode sc = DetectorElement::initialize();

  if ( sc.isFailure() ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "Failed to initialize DetectorElement" << endmsg;
    return sc;
  }

  if ( hasCondition( "Calibration" ) ) {
    m_calibration.emplace( this->condition( "Calibration" ) );
    updMgrSvc()->registerCondition( &m_calibration.value(), m_calibration->m_condition.path(),
                                    &DeOTDetector::Calibration::callback );
  } else {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::DEBUG << "Cannot find condition for global OT calibration" << endmsg;
  }

  // loop over stations
  for ( auto iS = this->childBegin(); iS != this->childEnd(); ++iS ) {
    auto* station = dynamic_cast<DeOTStation*>( *iS );
    if ( station ) {
      /// fill sation vector
      m_stations.push_back( station );
      m_mapStations.insert( ( station->elementID() ).station(), station );
    }
    // loop over layers
    for ( auto iL = ( *iS )->childBegin(); iL != ( *iS )->childEnd(); ++iL ) {
      DeOTLayer* layer = dynamic_cast<DeOTLayer*>( *iL );
      if ( layer ) {
        /// fill layer vector
        m_layers.push_back( layer );
      }
      // loop over quarters
      for ( auto iQ = ( *iL )->childBegin(); iQ != ( *iL )->childEnd(); ++iQ ) {
        DeOTQuarter* quarter = dynamic_cast<DeOTQuarter*>( *iQ );
        if ( quarter ) {
          /// fill quarter vector
          m_quarters.push_back( quarter );
        }
        // loop over modules
        for ( auto iM = ( *iQ )->childBegin(); iM != ( *iQ )->childEnd(); ++iM ) {
          DeOTModule* module = dynamic_cast<DeOTModule*>( *iM );
          if ( module ) {
            // get # of channels
            unsigned int channels = module->nChannels();
            m_nChannels += channels;
            if ( channels > m_nMaxChanInModule ) m_nMaxChanInModule = channels;
            // fill mod vector
            m_modules.push_back( module );
          }
        } // modules
      }   // quarters
    }     // layers
  }       // stations

  /// Set the first station
  setFirstStation( m_stations.front()->stationID() );

  /// Get OT Detector parameters
  m_resolution       = param<double>( "resolution" );
  m_propagationDelay = param<double>( "propagationDelay" );
  m_maxDriftTime     = param<double>( "maxDriftTime" );
  m_maxDriftTimeCor  = param<double>( "maxDriftTimeCor" );
  m_deadTime         = param<double>( "deadTime" );
  m_cellRadius       = param<double>( "cellRadius" );

  return sc;
}

void DeOTDetector::setFirstStation( const unsigned int iStation ) { m_firstStation = iStation; }

/// Find the station for a given XYZ point
const DeOTStation* DeOTDetector::findStation( const Gaudi::XYZPoint& aPoint ) const {
  /// Find the station and return a pointer to the station from XYZ point
  auto iS = std::find_if( m_stations.begin(), m_stations.end(),
                          [&]( const DetectorElement* e ) { return e->isInside( aPoint ); } );
  return iS != m_stations.end() ? *iS : nullptr;
}

/// Find the layer for a given XYZ point
const DeOTLayer* DeOTDetector::findLayer( const Gaudi::XYZPoint& aPoint ) const {
  const DeOTStation* s = findStation( aPoint );
  return s ? s->findLayer( aPoint ) : nullptr;
}

/// Find the quarter for a given XYZ point
const DeOTQuarter* DeOTDetector::findQuarter( const Gaudi::XYZPoint& aPoint ) const {
  const DeOTLayer* l = findLayer( aPoint );
  return l ? l->findQuarter( aPoint ) : nullptr;
  ;
}

/// Find the module for a given XYZ point
const DeOTModule* DeOTDetector::findModule( const Gaudi::XYZPoint& aPoint ) const {
  const DeOTQuarter* q = findQuarter( aPoint );
  return q ? q->findModule( aPoint ) : nullptr;
  ;
}

int DeOTDetector::sensitiveVolumeID( const Gaudi::XYZPoint& aPoint ) const {
  const DeOTModule* m = findModule( aPoint );
  return m ? static_cast<int>( m->elementID() ) : -1;
}

double DeOTDetector::distanceAlongWire( const OTChannelID aChannel, double xHit, double yHit ) const {
  const DeOTModule* aModule = findModule( aChannel );
  if ( UNLIKELY( !aModule ) ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::WARNING << "DistanceAlongWire requested for module that does not exist " << endmsg;
    return 0.0;
  }

  return aModule->distanceAlongWire( xHit, yHit );
}

double DeOTDetector::propagationTime( const OTChannelID aChannel, const double x, const double y ) const {
  /// Calculate the propagation delay along the wire
  return ( distanceAlongWire( aChannel, x, y ) * propagationDelay() );
}

OTChannelID DeOTDetector::nextChannelLeft( const OTChannelID aChannel ) const {
  const DeOTModule* aModule  = findModule( aChannel );
  int               nextLeft = aModule->nextLeftStraw( aChannel.straw() );
  return ( nextLeft == 0u )
             ? OTChannelID( 0u )
             : OTChannelID( aChannel.station(), aChannel.layer(), aChannel.quarter(), aChannel.module(), nextLeft );
}

OTChannelID DeOTDetector::nextChannelRight( const OTChannelID aChannel ) const {
  const DeOTModule* aModule   = findModule( aChannel );
  int               nextRight = aModule->nextRightStraw( aChannel.straw() );
  return ( nextRight == 0u )
             ? OTChannelID( 0u )
             : OTChannelID( aChannel.station(), aChannel.layer(), aChannel.quarter(), aChannel.module(), nextRight );
}

std::unique_ptr<LHCb::Trajectory<double>> DeOTDetector::trajectoryFirstWire( const LHCb::LHCbID& id,
                                                                             int                 monolayer ) const {
  if ( UNLIKELY( !id.isOT() ) ) {
    throw GaudiException( "The LHCbID is not of OT type!", "DeOTDetector.cpp", StatusCode::FAILURE );
  }

  const DeOTModule* aModule = findModule( id.otID() );
  if ( UNLIKELY( !aModule ) ) {
    throw GaudiException( "Failed to find module", "DeOTDetector.cpp", StatusCode::FAILURE );
  }

  return aModule->trajectoryFirstWire( monolayer );
}

std::unique_ptr<LHCb::Trajectory<double>> DeOTDetector::trajectoryLastWire( const LHCb::LHCbID& id,
                                                                            int                 monolayer ) const {
  if ( UNLIKELY( !id.isOT() ) ) {
    throw GaudiException( "The LHCbID is not of OT type!", "DeOTDetector.cpp", StatusCode::FAILURE );
  }

  const DeOTModule* aModule = findModule( id.otID() );
  if ( UNLIKELY( !aModule ) ) {
    throw GaudiException( "Failed to find module", "DeOTDetector.cpp", StatusCode::FAILURE );
  }

  return aModule->trajectoryLastWire( monolayer );
}

/// Returns a Trajectory representing the wire identified by the LHCbID
/// The offset is zero for all OT Trajectories
std::unique_ptr<LHCb::Trajectory<double>> DeOTDetector::trajectory( const LHCb::LHCbID& id,
                                                                    const double /*offset*/ ) const {
  if ( UNLIKELY( !id.isOT() ) ) {
    throw GaudiException( "The LHCbID is not of OT type!", "DeOTDetector.cpp", StatusCode::FAILURE );
  }

  const DeOTModule* aModule = findModule( id.otID() );
  if ( UNLIKELY( !aModule ) ) {
    throw GaudiException( "Failed to find module", "DeOTDetector.cpp", StatusCode::FAILURE );
  }

  // Offset hardcoded to 0. to eliminate warning about unused parameter
  return aModule->trajectory( id.otID(), 0 );
}

StatusCode DeOTDetector::Calibration::callback() {
  // find the t0 parameter in the calibration condition
  if ( UNLIKELY( !m_condition->exists( "TZero" ) ) ) {
    throw GaudiException( "No parameter 'TZero' in Calibration condition", "DeOTDetector.cpp", StatusCode::FAILURE );
  }
  m_globalT0 = m_condition->param<double>( "TZero" );
  return StatusCode::SUCCESS;
}

void DeOTDetector::setGlobalT0( double t0 ) {
  if ( UNLIKELY( !m_calibration ) ) {
    throw GaudiException( "Cannot set TZero parameter since condition does not exist.", "DeOTDetector.cpp",
                          StatusCode::FAILURE );
  }
  m_calibration->m_condition->param<double>( "TZero" ) = t0;
  updMgrSvc()->invalidate( m_calibration->m_condition.target() );
}
