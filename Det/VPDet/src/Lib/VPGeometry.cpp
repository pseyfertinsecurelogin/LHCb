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
#include <VPDet/VPDetPaths.h>
#include <VPDet/VPGeometry.h>

using namespace LHCb::DetDesc;

IConditionDerivationMgr::DerivationId VPGeometry::registerDerivation( IConditionDerivationMgr& cdm, ConditionKey key,
                                                                      bool withAlignment ) {

  // in case of withAlignment, we declare a dependency on the detector and the two additional conditions
  // to make sure the call back is called when there is a change we care about (even if in the code we
  // seem to look only at the detector)
  //
  // Please note that the number of arguments to the 'transformer' must match the inputkeys
  return withAlignment
             ? LHCb::DetDesc::addConditionDerivation( cdm, LHCb::Det::VP::paths, std::move( key ),
                                                      []( const DeVP& vp, const ParamValidDataObject&,
                                                          const ParamValidDataObject& ) -> VPGeometry { return {vp}; } )
             : LHCb::DetDesc::addConditionDerivation( cdm, LHCb::Det::VP::det_path, std::move( key ),
                                                      []( const DeVP& vp ) -> VPGeometry { return {vp}; } );
}

//============================================================================
// Rebuild the geometry (in case something changes in the Velo during the run)
//============================================================================
VPGeometry::VPGeometry( const DeVP& vp ) {
  const auto sensors = vp.sensors();

  // we copy the data from the geometry object which holds doubles intor a local float array
  std::copy( sensors.front()->xLocal().begin(), sensors.front()->xLocal().end(), m_local_x.begin() );
  std::copy( sensors.front()->xPitch().begin(), sensors.front()->xPitch().end(), m_x_pitch.begin() );
  m_pixel_size = sensors.front()->pixelSize( LHCb::VPChannelID( 0, 0, 0, 0 ) ).second;

  for ( const auto& sensor : sensors ) {
    // TODO:
    // if (!sensor->isReadOut()) continue;

    // get the local to global transformation matrix and
    // store it in a flat float array of sixe 12.
    Gaudi::Rotation3D     ltg_rot;
    Gaudi::TranslationXYZ ltg_trans;
    sensor->geometry()->toGlobalMatrix().GetDecomposition( ltg_rot, ltg_trans );
    assert( sensor->sensorNumber() < m_ltg.size() );
    auto& ltg = m_ltg[sensor->sensorNumber()];
    ltg_rot.GetComponents( ltg.data() ); // writes to [ ltg[0], ltg[8] ]
    ltg[9]  = ltg_trans.X();
    ltg[10] = ltg_trans.Y();
    ltg[11] = ltg_trans.Z();
  }
}
