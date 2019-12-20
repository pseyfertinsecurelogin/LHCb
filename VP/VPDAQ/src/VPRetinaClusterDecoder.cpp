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

#include "VPRetinaClusterDecoder.h"

#include "Event/StateParameters.h"
#include "Event/Track.h"
#include "Kernel/VPConstants.h"
#include "VPKernel/PixelUtils.h"

#include "GaudiKernel/Transform3DTypes.h"

#include <iomanip>

DECLARE_COMPONENT( VPRetinaClusterDecoder )

namespace {
  using namespace Pixel;
}

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
VPRetinaClusterDecoder::VPRetinaClusterDecoder( const std::string& name, ISvcLocator* pSvcLocator )
    : MultiTransformer( name, pSvcLocator, KeyValue{"RawEventLocation", LHCb::RawEventLocation::Default},
                        {KeyValue{"ClusterLocation", LHCb::VPClusterLocation::Light},
                         KeyValue{"ClusterOffsets", LHCb::VPClusterLocation::Offsets}} ) {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode VPRetinaClusterDecoder::initialize() {

  StatusCode sc = MultiTransformer::initialize();
  if ( sc.isFailure() ) return sc;

  m_vp = getDet<DeVP>( DeVPLocation::Default );
  rebuildGeometry();

  sc = updMgrSvc()->update( this );
  if ( !sc.isSuccess() ) return Error( "Failed to update station structure." );

  return StatusCode::SUCCESS;
}

//============================================================================
// Rebuild the geometry (in case something changes in the Velo during the run)
//============================================================================
StatusCode VPRetinaClusterDecoder::rebuildGeometry() {
  // Delete the existing modules.
  m_modules.clear();
  m_firstModule = 999;
  m_lastModule  = 0;

  int previousLeft  = -1;
  int previousRight = -1;

  m_local_x    = m_vp->sensor( 0 ).xLocal();
  m_x_pitch    = m_vp->sensor( 0 ).xPitch();
  m_pixel_size = m_vp->sensor( 0 ).pixelSize();

  m_vp->runOnAllSensors( [this, &previousLeft, &previousRight]( const DeVPSensor& sensor ) {
    // get the local to global transformation matrix and
    // store it in a flat float array of sixe 12.
    Gaudi::Rotation3D     ltg_rot;
    Gaudi::TranslationXYZ ltg_trans;
    sensor.getGlobalMatrixDecomposition( ltg_rot, ltg_trans );
    assert( sensor.sensorNumber() < m_ltg.size() );
    auto& ltg = m_ltg[sensor.sensorNumber()];
    ltg_rot.GetComponents( begin( ltg ) );
    ltg_trans.GetCoordinates( ltg.data() + 9 );

    // Get the number of the module this sensor is on.
    const unsigned int number = sensor.module();
    if ( number < m_modules.size() ) {
      // Check if this module has already been setup.
      if ( m_modules[number] ) return;
    } else {
      m_modules.resize( number + 1, 0 );
    }

    // Create a new module and add it to the list.
    m_module_pool.emplace_back( number, sensor.isRight() );
    PixelModule* module = &m_module_pool.back();
    module->setZ( sensor.z() );
    if ( sensor.isRight() ) {
      module->setPrevious( previousRight );
      previousRight = number;
    } else {
      module->setPrevious( previousLeft );
      previousLeft = number;
    }
    m_modules[number] = module;
    if ( m_firstModule > number ) m_firstModule = number;
    if ( m_lastModule < number ) m_lastModule = number;
  } );
  // the module pool might have been resized -- make sure
  // all module pointers are valid.
  for ( unsigned int i = 0; i < m_module_pool.size(); ++i ) {
    PixelModule* module         = &m_module_pool[i];
    m_modules[module->number()] = module;
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
std::tuple<std::vector<LHCb::VPLightCluster>, std::array<unsigned, VeloInfo::Numbers::NOffsets>>
VPRetinaClusterDecoder::operator()( const LHCb::RawEvent& rawEvent ) const {
  auto result = std::tuple<std::vector<LHCb::VPLightCluster>, std::array<unsigned, VeloInfo::Numbers::NOffsets>>{};
  auto& [pool, offsets] = result;

  const auto& tBanks = rawEvent.banks( LHCb::RawBank::VPRetinaCluster );
  debug() << "Number of retina cluster raw banks: " << tBanks.size() << "." << endmsg;
  if ( tBanks.empty() ) return result;

  const unsigned int version = tBanks[0]->version();
  if ( version != 1 ) {
    warning() << "Unsupported raw bank version (" << version << ")" << endmsg;
    return result;
  }

  // Since the pool is local, to first preallocate the pool, then count hits per module,
  // and then preallocate per module and move hits might not be faster than adding
  // directly to the PixelModuleHits (which would require more allocations, but
  // not many if we start with a sensible default)
  constexpr unsigned int startSize = 10000U;
  pool.reserve( startSize );

  // Loop over VP RawBanks
  for ( auto iterBank : tBanks ) {

    const unsigned int module = iterBank->sourceID();
    if ( m_modulesToSkipMask[module - 1] ) continue;

    const unsigned int sensor0 = ( module - 1 ) * VP::NSensorsPerModule;

    const uint32_t* bank = iterBank->data();

    const uint32_t ncluster = *bank++;

    // Read clusters
    for ( unsigned int i = 0; i < ncluster; ++i ) {
      const uint32_t cluster_word = *bank++;

      const unsigned int sensor = sensor0 + ( cluster_word >> 24 );

      const uint32_t cx = ( cluster_word >> 14 ) & 0x3FF;
      const float    fx = ( ( cluster_word >> 11 ) & 0x7 ) / 8.f;
      const uint32_t cy = ( cluster_word >> 3 ) & 0xFF;
      const float    fy = ( cluster_word & 0x7 ) / 8.f;

      const uint32_t chip = cx / CHIP_COLUMNS;
      const uint32_t ccol = cx % CHIP_COLUMNS;

      LHCb::VPChannelID cid( sensor, chip, ccol, cy );

      const float local_x = m_local_x[cx] + fx * m_x_pitch[cx];
      const float local_y = ( cy + 0.5 + fy ) * m_pixel_size;

      const auto& ltg = m_ltg[sensor];
      const float gx  = ( ltg[0] * local_x + ltg[1] * local_y + ltg[2] );
      const float gy  = ( ltg[3] * local_x + ltg[4] * local_y + ltg[5] );
      const float gz  = ( ltg[6] * local_x + ltg[7] * local_y + ltg[8] );

      pool.emplace_back( 1, 1, gx, gy, gz, cid );

      ++offsets[module];
    } // loop over clusters in raw bank
  }   // loop over all banks

  std::partial_sum( offsets.begin(), offsets.end(), offsets.begin() );

  return result;
}
