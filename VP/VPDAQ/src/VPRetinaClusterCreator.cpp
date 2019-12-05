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

#include "VPRetinaClusterCreator.h"

#include "Event/StateParameters.h"
#include "Kernel/VPConstants.h"
#include "VPKernel/PixelUtils.h"

#include "GaudiKernel/Transform3DTypes.h"

#include <iomanip>
#include <iterator>

using namespace LHCb;

DECLARE_COMPONENT( VPRetinaClusterCreator )

namespace {
  using namespace Pixel;
}

namespace {
  struct SPCache {
    std::array<float, 4> fxy;
    unsigned char        pattern;
    unsigned char        nx1;
    unsigned char        nx2;
    unsigned char        ny1;
    unsigned char        ny2;
  };
  //=========================================================================
  // Cache Super Pixel cluster patterns.
  //=========================================================================
  auto create_SPPatterns() {
    std::array<SPCache, 256> SPCaches;
    // create a cache for all super pixel cluster patterns.
    // this is an unoptimized 8-way flood fill on the 8 pixels
    // in the super pixel.
    // no point in optimizing as this is called once in
    // initialize() and only takes about 20 us.

    // define deltas to 8-connectivity neighbours
    const int dx[] = {-1, 0, 1, -1, 0, 1, -1, 1};
    const int dy[] = {-1, -1, -1, 1, 1, 1, 0, 0};

    // clustering buffer for isolated superpixels.
    unsigned char sp_buffer[8];

    // SP index buffer and its size for single SP clustering
    unsigned char sp_idx[8];
    unsigned char sp_idx_size = 0;

    // stack and stack pointer for single SP clustering
    unsigned char sp_stack[8];
    unsigned char sp_stack_ptr = 0;

    // loop over all possible SP patterns
    for ( unsigned int sp = 0; sp < 256; ++sp ) {
      sp_idx_size = 0;
      for ( unsigned int shift = 0; shift < 8; ++shift ) {
        const unsigned char p = sp & ( 1 << shift );
        sp_buffer[shift]      = p;
        if ( p ) { sp_idx[sp_idx_size++] = shift; }
      }

      // loop over pixels in this SP and use them as
      // cluster seeds.
      // note that there are at most two clusters
      // in a single super pixel!
      unsigned char clu_idx = 0;
      for ( unsigned int ip = 0; ip < sp_idx_size; ++ip ) {
        unsigned char idx = sp_idx[ip];

        if ( 0 == sp_buffer[idx] ) { continue; } // pixel is used

        sp_stack_ptr             = 0;
        sp_stack[sp_stack_ptr++] = idx;
        sp_buffer[idx]           = 0;
        unsigned char x          = 0;
        unsigned char y          = 0;
        unsigned char n          = 0;

        while ( sp_stack_ptr ) {
          idx                     = sp_stack[--sp_stack_ptr];
          const unsigned char row = idx % 4;
          const unsigned char col = idx / 4;
          x += col;
          y += row;
          ++n;

          for ( unsigned int ni = 0; ni < 8; ++ni ) {
            const char ncol = col + dx[ni];
            if ( ncol < 0 || ncol > 1 ) continue;
            const char nrow = row + dy[ni];
            if ( nrow < 0 || nrow > 3 ) continue;
            const unsigned char nidx = ( ncol << 2 ) | nrow;
            if ( 0 == sp_buffer[nidx] ) continue;
            sp_stack[sp_stack_ptr++] = nidx;
            sp_buffer[nidx]          = 0;
          }
        }

        const uint32_t cx = x / n;
        const uint32_t cy = y / n;
        const float    fx = x / static_cast<float>( n ) - cx;
        const float    fy = y / static_cast<float>( n ) - cy;

        // store the centroid pixel
        SPCaches[sp].pattern |= ( ( cx << 2 ) | cy ) << 4 * clu_idx;

        // set the two cluster flag if this is the second cluster
        SPCaches[sp].pattern |= clu_idx << 3;

        // set the pixel fractions
        SPCaches[sp].fxy[2 * clu_idx]     = fx;
        SPCaches[sp].fxy[2 * clu_idx + 1] = fy;

        // increment cluster count. note that this can only become 0 or 1!
        ++clu_idx;
      }
    }
    return SPCaches;
  }
  // SP pattern buffers for clustering, cached once.
  // There are 256 patterns and there can be at most two
  // distinct clusters in an SP.
  static const std::array<SPCache, 256> s_SPCaches = create_SPPatterns();
} // namespace

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
VPRetinaClusterCreator::VPRetinaClusterCreator( const std::string& name, ISvcLocator* pSvcLocator )
    : Transformer( name, pSvcLocator, KeyValue{"RawEventLocation", LHCb::RawEventLocation::Default},
                   KeyValue{"RetinaClusterLocation", LHCb::RawEventLocation::VeloCluster} ) {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode VPRetinaClusterCreator::initialize() {

  StatusCode sc = Transformer::initialize();
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
StatusCode VPRetinaClusterCreator::rebuildGeometry() {

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
    auto ltg = m_ltg + sensor.sensorNumber();
    ltg_rot.GetComponents( ltg );
    ltg_trans.GetCoordinates( ltg + 9 );

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
LHCb::RawEvent VPRetinaClusterCreator::operator()( const LHCb::RawEvent& rawEvent ) const {
  RawEvent result;

  const auto& tBanks = rawEvent.banks( LHCb::RawBank::VP );
  if ( tBanks.empty() ) return result;

  const unsigned int version = tBanks[0]->version();
  if ( version != 2 ) {
    warning() << "Unsupported raw bank version (" << version << ")" << endmsg;
    return result;
  }

  LHCb::VPRetinaClusters toSortClusters;

  debug() << "Read " << tBanks.size() << " raw banks from TES" << endmsg;

  unsigned int nBanks = 0;

  // Loop over VP RawBanks
  for ( auto iterBank : tBanks ) {

    const unsigned int sensor             = iterBank->sourceID();
    const uint32_t     sensorID_in_module = sensor % VP::NSensorsPerModule;
    const unsigned int module             = 1 + ( sensor / VP::NSensorsPerModule );

    const float* ltg = m_ltg + 16 * sensor;

    const uint32_t* bank = iterBank->data();

    auto retinaClusters = makeRetinaClusters( bank );

    for ( auto iterCluster : retinaClusters ) {
      const uint32_t cx = iterCluster >> 14 & 0x3FF;
      const float    fx = ( ( iterCluster >> 11 ) & 0x7 ) / 8.;
      const uint32_t cy = ( iterCluster >> 3 ) & 0xFF;
      const float    fy = ( iterCluster & 0x7 ) / 8.;

      const float local_x = m_local_x[cx] + fx * m_x_pitch[cx];
      const float local_y = ( cy + 0.5 + fy ) * m_pixel_size;

      // gx
      const float gx = ( ltg[0] * local_x + ltg[1] * local_y + ltg[9] );
      // gy
      const float gy = ( ltg[3] * local_x + ltg[4] * local_y + ltg[10] );
      // phi
      float tan = gy / gx;

      toSortClusters.push_back( LHCb::VPRetinaCluster( iterCluster + ( sensorID_in_module << 24 ), gx, gy, tan ) );
    }

    if ( sensorID_in_module == VP::NSensorsPerModule - 1 ) {

      const bool odd = ( module - 1 ) % 2 == 1;

      std::stable_sort( toSortClusters.begin(), toSortClusters.end(),
                        [&odd]( const LHCb::VPRetinaCluster& a, const LHCb::VPRetinaCluster& b ) {
                          // sorting in phi for even modules
                          return (
                              // odd modules, change in y value
                              ( odd && ( a.gy() < 0.f && b.gy() > 0.f ) ) ||
                              // or even modules, change in y value, but swap logic
                              ( !odd && ( a.gy() > 0.f && b.gy() < 0.f ) ) ||
                              // same y side even and odd modules, check y1/x1 < y2/x2
                              ( ( a.gy() * b.gy() ) > 0.f && ( a.gy() * b.gx() < b.gy() * a.gx() ) ) );
                        } );

      std::vector<uint32_t> sortedClusters;
      sortedClusters.reserve( toSortClusters.size() + 1 );
      sortedClusters.push_back( toSortClusters.size() );
      for ( auto iterCluster : toSortClusters ) sortedClusters.push_back( iterCluster.word() );

      result.addBank( module, LHCb::RawBank::VPRetinaCluster, m_bankVersion, sortedClusters );

      ++nBanks;

      toSortClusters.clear();
    }

  } // loop over all banks

  debug() << "Added " << nBanks << " raw banks of retina clusters to TES" << endmsg;

  return result;
}

//=============================================================================
// make RetinaClusters from bank
//=============================================================================
std::vector<uint32_t> VPRetinaClusterCreator::makeRetinaClusters( const uint32_t* bank ) const {
  const uint32_t nsp = *bank++;

  std::vector<VPRetinaMatrix> RetinaMatrixVector;
  RetinaMatrixVector.reserve( 20 );
  std::vector<uint32_t> RetinaCluster;

  // Read super pixel
  for ( unsigned int i = 0; i < nsp; ++i ) {
    const uint32_t sp_word = *bank++;

    uint8_t sp = sp_word & 0xFFU;

    if ( 0 == sp ) continue; // protect against zero super pixels.

    const uint32_t sp_addr          = ( sp_word & 0x007FFF00U ) >> 8;
    const uint32_t sp_row           = sp_addr & 0x3FU;
    const uint32_t sp_col           = ( sp_addr >> 6 );
    const uint32_t no_sp_neighbours = sp_word & 0x80000000U;

    // if a super pixel is isolated the clustering boils
    // down to a simple pattern look up.
    if ( no_sp_neighbours ) {

      // there is always at least one cluster in the super
      // pixel. look up the pattern and add it.

      // remove after caches rewrite
      const auto&    spcache = s_SPCaches[sp];
      const uint32_t idx     = spcache.pattern;

      const uint32_t row = idx & 0x03U;
      const uint32_t col = ( idx >> 2 ) & 1;

      const uint64_t cX = ( ( sp_col * 2 + col ) << 3 ) + ( uint64_t )( spcache.fxy[0] * 8 );
      const uint64_t cY = ( ( sp_row * 4 + row ) << 3 ) + ( uint64_t )( spcache.fxy[1] * 8 );

      RetinaCluster.push_back( cX << 11 | cY );
      if ( idx & 8 ) {
        const uint32_t row = ( idx >> 4 ) & 3;
        const uint32_t col = ( idx >> 6 ) & 1;

        const uint64_t cX = ( ( sp_col * 2 + col ) << 3 ) + ( uint64_t )( spcache.fxy[2] * 8 );
        const uint64_t cY = ( ( sp_row * 4 + row ) << 3 ) + ( uint64_t )( spcache.fxy[3] * 8 );

        RetinaCluster.push_back( cX << 11 | cY );
      }

      continue;
    }

    // this one is not isolated, we fill a Retina

    // we look for already created Retina
    auto iterRetina = std::find_if( RetinaMatrixVector.begin(), RetinaMatrixVector.end(),
                                    [&]( const VPRetinaMatrix& m ) { return m.IsInRetina( sp_row, sp_col ); } );
    if ( iterRetina != RetinaMatrixVector.end() ) {
      ( *iterRetina ).AddSP( sp_row, sp_col, sp );
    } else {
      RetinaMatrixVector.emplace_back( sp_row, sp_col, sp );
    }
  } // loop over super pixels in raw bank

  // searchRetinaCluster
  for ( auto& m : RetinaMatrixVector ) {
    const auto& clusters = m.SearchCluster();
    RetinaCluster.insert( end( RetinaCluster ), begin( clusters ), end( clusters ) );
  };

  return RetinaCluster;
}
