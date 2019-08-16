/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "VPDet/VPTrackingInfo.h"
#include "VPDet/VPDetPaths.h"

#include <GaudiKernel/SystemOfUnits.h>

using namespace LHCb::DetDesc;

IConditionDerivationMgr::DerivationId VPTrackingInfo::registerDerivation( IConditionDerivationMgr& cdm,
                                                                          ConditionKey key, bool withAlignment,
                                                                          float minZ_forward, float maxZ_backward ) {
  // check if the derivation was already registered
  auto dId = cdm.derivationFor( key );
  if ( dId != IConditionDerivationMgr::NoDerivation ) { return dId; }
  // it was not, so we have to register it now.
  auto adapter = [minZ_forward, maxZ_backward]( const ConditionKey& /* target */, ConditionUpdateContext& ctx,
                                                Condition& output ) {
    const auto vp = dynamic_cast<const DeVP*>( ctx[LHCb::Det::VP::det_path] );
    if ( !vp )
      throw GaudiException( "The object at " + LHCb::Det::VP::det_path + " is either not a DeVP or not present", "VPTrackingInfo", StatusCode::FAILURE );
    output.payload = VPTrackingInfo{*vp, minZ_forward, maxZ_backward};
  };
  // we declare a dependency on the detector and the conditions to make sure the call back
  // is called when there is a change we care about (even if in the code we seem to look
  // only at the detector)
  return withAlignment ? cdm.add( LHCb::Det::VP::paths, std::move( key ), std::move( adapter ) )
                       : cdm.add( LHCb::Det::VP::det_path, std::move( key ), std::move( adapter ) );
}

VPTrackingInfo::VPTrackingInfo( const DeVP& vp, float minZ_forward, float maxZ_backward ) : m_vp{vp} {

  ///////////////////////////////////////////////////////////////////////////////
  // from PrPixelTracking::recomputeModuleZPositions

  // Note that we are taking the position of the first sensor for that module
  // This is what was done in the original PrPixelHitManager but is not
  // necessary correct.
  m_firstModule = 999;
  m_lastModule  = 0;
  for ( auto sensor : vp.sensors() ) {
    // Get the number of the module this sensor is on.
    const unsigned int number = sensor->module();
    // 4 sensors per module, we want the average position
    // use cast to avoid double arithmetic
    m_moduleZPositions[number] += static_cast<float>( sensor->z() ) / 4.0f;
    if ( m_firstModule > number ) m_firstModule = number;
    if ( m_lastModule < number ) m_lastModule = number;
  }
  unsigned int mID = m_lastModule;
  // will do 51|50 -> 49|48 -> 47|46
  while ( mID <= m_lastModule ) {
    if ( mID > 3 ) { // the algorithm uses forward modules, at most make track with module 4->2->0
      m_modulesDefaultLoop.push_back( mID );
    }
    // In backward search -> at most uses 4->2->0 for a 3 hit track to find.
    if ( m_moduleZPositions[mID] > minZ_forward && mID > 3 ) { m_modulesForwardLoop.push_back( mID ); }
    // In forward search -> at most uses 47->49->51 for a 3 hit track to find.
    if ( m_moduleZPositions[mID] < maxZ_backward && mID < 48 ) { m_modulesBackwardLoop.push_back( mID ); }
    if ( mID == 0 ) break;
    mID--;
  }
  std::reverse( m_modulesBackwardLoop.begin(), m_modulesBackwardLoop.end() );

  ///////////////////////////////////////////////////////////////////////////////
  // from PrPixelTracking::InitClusterPositioning

  float errorSinglePixel = 12.5f * Gaudi::Units::micrometer;

  // Store the rotations of each sensor.
  const unsigned int nSensors = vp.numberSensors();
  m_errorX.resize( nSensors, 0 );
  m_errorY.resize( nSensors, 0 );
  m_errorXLong.resize( nSensors, 0 );
  m_errorYLong.resize( nSensors, 0 );

  const Gaudi::XYZVector vl( 1., 0., 0. );
  auto                   sensors = vp.sensors();

  for ( auto sensor : sensors ) {
    const unsigned int sensorNumber = sensor->sensorNumber();
    if ( sensorNumber >= m_errorX.size() ) {
      m_errorX.resize( sensorNumber + 1 );
      m_errorY.resize( sensorNumber + 1 );
      m_errorXLong.resize( sensorNumber + 1 );
      m_errorYLong.resize( sensorNumber + 1 );
    }
    const auto  vg          = sensor->geometry()->toGlobal( vl );
    const float cphi        = vg.x();
    const float phi         = acos( cphi );
    const float sphi        = sin( phi );
    float       cphiSquared = cphi * cphi;
    float       sphiSquared = sphi * sphi;

    float dx = errorSinglePixel;
    float dy = errorSinglePixel;

    // Transform the error estimate to the global frame.
    m_errorX[sensorNumber] = sqrt( dx * dx * cphiSquared + dy * dy * sphiSquared );
    m_errorY[sensorNumber] = sqrt( dx * dx * sphiSquared + dy * dy * cphiSquared );

    dx *= 2;
    m_errorXLong[sensorNumber] = sqrt( dx * dx * sphiSquared + dy * dy * cphiSquared );
    m_errorYLong[sensorNumber] = sqrt( dx * dx * sphiSquared + dy * dy * cphiSquared );
  }
}
