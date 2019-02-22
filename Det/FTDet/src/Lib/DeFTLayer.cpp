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
#include "FTDet/DeFTLayer.h"

#include "DetDesc/SolidBox.h"

/** @file DeFTLayer.cpp
 *
 *  Implementation of class : DeFTLayer
 *
 *  @author Jeroen van Tilburg
 *  @date   2016-07-18
 */

//=============================================================================
// classID function
//=============================================================================
const CLID& DeFTLayer::clID() const { return DeFTLayer::classID(); }

//=============================================================================
// Initialization
//=============================================================================
StatusCode DeFTLayer::initialize() {
  /// Loop over quarters and add to vector of quarters
  for ( auto iQ = this->childBegin(); iQ != this->childEnd(); ++iQ ) {
    DeFTQuarter* quarter = dynamic_cast<DeFTQuarter*>( *iQ );
    if ( quarter ) {
      unsigned int quarterID = quarter->quarterID();
      if ( quarterID < 4 ) {
        m_quarters[quarterID] = quarter;
        // Create the flat list of modules
        auto modules = quarter->modules();
        m_modules.insert( m_modules.end(), modules.begin(), modules.end() );
      }
    }
  }

  m_layerID = (unsigned int)param<int>( "layerID" );

  // Get the global z position of the layer
  Gaudi::XYZPoint globalPoint = geometry()->toGlobal( Gaudi::XYZPoint( 0., 0., 0. ) );
  m_globalZ                   = globalPoint.z();

  // Get the boundaries of the layer
  const SolidBox* box = dynamic_cast<const SolidBox*>( geometry()->lvolume()->solid()->coverTop() );
  m_sizeX             = box->xsize();
  m_sizeY             = box->ysize();

  // Make the plane for the layer
  const Gaudi::XYZPoint g1 = geometry()->toGlobal( Gaudi::XYZPoint( 0., 0., 0. ) );
  const Gaudi::XYZPoint g2 = geometry()->toGlobal( Gaudi::XYZPoint( 1., 0., 0. ) );
  const Gaudi::XYZPoint g3 = geometry()->toGlobal( Gaudi::XYZPoint( 0., 1., 0. ) );
  m_plane                  = Gaudi::Plane3D( g1, g2, g3 );
  m_dzdy                   = ( g3.z() - g1.z() ) / ( g3.y() - g1.y() );
  m_stereoAngle            = (float)param<double>( "stereoAngle" );
  m_dxdy                   = -tan( m_stereoAngle );

  return StatusCode::SUCCESS;
}

/// Find the quarter for a given XYZ point
const DeFTQuarter* DeFTLayer::findQuarter( const Gaudi::XYZPoint& aPoint ) const {
  auto iQ = std::find_if( m_quarters.begin(), m_quarters.end(),
                          [&aPoint]( const DeFTQuarter* q ) { return q ? q->isInside( aPoint ) : false; } );
  return iQ != m_quarters.end() ? *iQ : nullptr;
}

/// Find the module for a given XYZ point
const DeFTModule* DeFTLayer::findModule( const Gaudi::XYZPoint& aPoint ) const {
  auto iM = std::find_if( m_modules.begin(), m_modules.end(),
                          [&aPoint]( const DeFTModule* m ) { return m ? m->isInside( aPoint ) : false; } );
  return iM != m_modules.end() ? *iM : nullptr;
}
