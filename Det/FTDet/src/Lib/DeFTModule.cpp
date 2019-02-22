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
// classID function
//=============================================================================
const CLID& DeFTModule::clID() const { return DeFTModule::classID(); }

//=============================================================================
// Initialization
//=============================================================================
StatusCode DeFTModule::initialize() {
  IDetectorElement* quarter = this->parentIDetectorElement();
  IDetectorElement* layer   = quarter->parentIDetectorElement();
  IDetectorElement* station = layer->parentIDetectorElement();

  // Get specific parameters from the module
  unsigned int      moduleID  = param<int>( "moduleID" );
  unsigned int      quarterID = quarter->params()->param<int>( "quarterID" );
  unsigned int      layerID   = layer->params()->param<int>( "layerID" );
  unsigned int      stationID = station->params()->param<int>( "stationID" );
  LHCb::FTChannelID aChan( stationID, layerID, quarterID, moduleID, 0u, 0u, 0u );
  setElementID( aChan );

  /// Loop over mats and add to mat vector
  for ( auto iM = this->childBegin(); iM != this->childEnd(); ++iM ) {
    DeFTMat* mat = dynamic_cast<DeFTMat*>( *iM );
    if ( mat ) {
      unsigned int matID = mat->matID();
      if ( matID < 4 ) m_mats[matID] = mat;
    }
  }

  // Get some useful geometric parameters from the database
  m_nChannelsInModule = (int)param<int>( "nChannelsInModule" );

  // Determine if the pseudoChannel numbering is reversed.
  // (pseudoChannels have increasing x)
  Gaudi::XYZPoint firstPoint = geometry()->toGlobal( Gaudi::XYZPoint( -1.0, 0, 0 ) );
  Gaudi::XYZPoint lastPoint  = geometry()->toGlobal( Gaudi::XYZPoint( 1.0, 0, 0 ) );
  m_reversed                 = std::abs( firstPoint.x() ) > std::abs( lastPoint.x() );

  // Make the plane for the module
  const Gaudi::XYZPoint g1 = geometry()->toGlobal( Gaudi::XYZPoint( 0., 0., 0. ) );
  const Gaudi::XYZPoint g2 = geometry()->toGlobal( Gaudi::XYZPoint( 1., 0., 0. ) );
  const Gaudi::XYZPoint g3 = geometry()->toGlobal( Gaudi::XYZPoint( 0., 1., 0. ) );
  m_plane                  = Gaudi::Plane3D( g1, g2, g3 );

  return StatusCode::SUCCESS;
}

/// Find the layer for a given XYZ point
const DeFTMat* DeFTModule::findMat( const Gaudi::XYZPoint& aPoint ) const {
  /// Find the layer and return a pointer to the layer from XYZ point
  auto iter = std::find_if( m_mats.begin(), m_mats.end(),
                            [&aPoint]( const DeFTMat* m ) { return m ? m->isInside( aPoint ) : false; } );
  return iter != m_mats.end() ? *iter : nullptr;
}

/// Get the pseudo-channel for a FTChannelID (useful in the monitoring)
int DeFTModule::pseudoChannel( const LHCb::FTChannelID channelID ) const {
  int channelInModule = channelID.channelID() & ( m_nChannelsInModule - 1u );
  if ( m_reversed ) { channelInModule = m_nChannelsInModule - 1 - channelInModule; }
  int pseudoChannel = channelInModule + moduleID() * m_nChannelsInModule;
  return pseudoChannel;
}

LHCb::FTChannelID DeFTModule::channelFromPseudo( const int pseudoChannel ) const {
  int channelInModule = pseudoChannel & ( m_nChannelsInModule - 1u );
  if ( m_reversed ) { channelInModule = m_nChannelsInModule - 1 - channelInModule; }
  return LHCb::FTChannelID( elementID() + channelInModule );
}
