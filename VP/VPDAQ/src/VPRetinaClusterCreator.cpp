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
#include "DetDesc/ConditionAccessorHolder.h"
#include "Event/RawBank.h"
#include "Event/RawEvent.h"
#include "Event/VPLightCluster.h"
#include "GaudiAlg/Transformer.h"
#include "GaudiKernel/Transform3DTypes.h"
#include "Kernel/VPChannelID.h"
#include "Kernel/VPConstants.h"
#include "VPDet/DeVP.h"
#include "VPKernel/PixelUtils.h"
#include "VPKernel/VeloPixelInfo.h"
#include "VPRetinaCluster.h"
#include "VPRetinaMatrix.h"
#include <array>
#include <iomanip>
#include <iterator>
#include <tuple>
#include <vector>

/** @class VPRetinaClusterCreator VPRetinaClusterCreator.h
 * Algorithm to create RetinaCluster Raw Bank from SuperPixel.
 *
 * There is one raw bank per sensor, that is the sensor number (0-207)
 * is the source ID of the bank. Note that this means there is no
 * need to encode the sensor in the RetinaCluster addresses.
 *
 * Each bank has a four byte word header, followed by a four byte
 * RetinaCluster word for each RetinaCluster on the sensor.
 *
 * The header word is currently simply the number of RetinaCluster
 * on the sensor.
 *
 * The RetinaCluster word encoding is the following:
 *
 * bit 0-2    RetinaCluster Fraction Row (0-1 by step of 0.125)
 * bit 3-10   RetinaCluster Row (0-255)
 * bit 11-13  RetinaCluster Fraction Column (0-1 by step of 0.125)
 * bit 14-23  RetinaCluster Column (0-767)
 * bit 24-25  SensorID in the module
 * bit 26-31  UNUSED
 *
 * @author Federico Lazzari
 * @date   2018-06-20
 */

namespace LHCb::VP::Retina {
  struct GeomCache {
    GeomCache( const DeVP& det );
    std::array<std::array<float, 9>, ::VP::NSensors> m_ltg;
    LHCb::span<const double>                         m_local_x;
    LHCb::span<const double>                         m_x_pitch;
    float                                            m_pixel_size;
  };
} // namespace LHCb::VP::Retina

class VPRetinaClusterCreator
    : public Gaudi::Functional::Transformer<LHCb::RawEvent( const LHCb::RawEvent&, const LHCb::VP::Retina::GeomCache& ),
                                            LHCb::DetDesc::usesConditions<LHCb::VP::Retina::GeomCache>> {

public:
  /// Standard constructor
  VPRetinaClusterCreator( const std::string& name, ISvcLocator* pSvcLocator );

  /// Algorithm initialization
  StatusCode initialize() override;

  /// Algorithm execution
  LHCb::RawEvent operator()( const LHCb::RawEvent&, const LHCb::VP::Retina::GeomCache& ) const override;

private:
  /// bank version. (change this every time semantics change!)
  static constexpr unsigned int m_bankVersion = 1;

  /// make RetinaClusters from bank
  std::vector<uint32_t> makeRetinaClusters( LHCb::span<const uint32_t> bank ) const;
};

using namespace LHCb;

DECLARE_COMPONENT( VPRetinaClusterCreator )

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
    : Transformer( name, pSvcLocator,
                   {KeyValue{"RawEventLocation", LHCb::RawEventLocation::Default},
                    KeyValue{"GeomCacheLocation", "VPRetinaClusterCreator-" + name + "-GeomCache"}},
                   KeyValue{"RetinaClusterLocation", LHCb::RawEventLocation::VeloCluster} ) {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode VPRetinaClusterCreator::initialize() {
  return Transformer::initialize().andThen(
      [&] { addConditionDerivation<LHCb::VP::Retina::GeomCache( const DeVP& )>( *this, DeVPLocation::Default ); } );
}

//============================================================================
// Rebuild the geometry (in case something changes in the Velo during the run)
//============================================================================
LHCb::VP::Retina::GeomCache::GeomCache( const DeVP& det ) {

  m_local_x    = det.sensor( 0 ).xLocal();
  m_x_pitch    = det.sensor( 0 ).xPitch();
  m_pixel_size = det.sensor( 0 ).pixelSize();

  det.runOnAllSensors( [this]( const DeVPSensor& sensor ) {
    // get the local to global transformation matrix and
    // store it in a flat float array of sixe 12.
    Gaudi::Rotation3D     ltg_rot;
    Gaudi::TranslationXYZ ltg_trans;
    sensor.getGlobalMatrixDecomposition( ltg_rot, ltg_trans );
    auto& ltg = m_ltg[sensor.sensorNumber()];
    ltg_rot.GetComponents( ltg.data() );
    ltg[2] = ltg_trans.X();
    ltg[5] = ltg_trans.Y();
    ltg[8] = ltg_trans.Z();
  } );
}

//=============================================================================
// Main execution
//=============================================================================
LHCb::RawEvent VPRetinaClusterCreator::operator()( const LHCb::RawEvent&              rawEvent,
                                                   const LHCb::VP::Retina::GeomCache& cache ) const {
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
    const uint32_t     sensorID_in_module = sensor % ::VP::NSensorsPerModule;
    const unsigned int module             = 1 + ( sensor / ::VP::NSensorsPerModule );

    const auto& ltg = cache.m_ltg[sensor];

    for ( auto iterCluster : makeRetinaClusters( iterBank->range<uint32_t>() ) ) {
      const uint32_t cx = iterCluster >> 14 & 0x3FF;
      const float    fx = ( ( iterCluster >> 11 ) & 0x7 ) / 8.;
      const uint32_t cy = ( iterCluster >> 3 ) & 0xFF;
      const float    fy = ( iterCluster & 0x7 ) / 8.;

      const float local_x = cache.m_local_x[cx] + fx * cache.m_x_pitch[cx];
      const float local_y = ( cy + 0.5 + fy ) * cache.m_pixel_size;

      const float gx = ltg[0] * local_x + ltg[1] * local_y + ltg[2];
      const float gy = ltg[3] * local_x + ltg[4] * local_y + ltg[5];
      // phi
      const float tan = gy / gx;

      toSortClusters.emplace_back( iterCluster + ( sensorID_in_module << 24 ), gx, gy, tan );
    }

    if ( sensorID_in_module == ::VP::NSensorsPerModule - 1 ) {

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
      std::transform( toSortClusters.begin(), toSortClusters.end(), std::back_inserter( sortedClusters ),
                      []( const auto& c ) { return c.word(); } );

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
std::vector<uint32_t> VPRetinaClusterCreator::makeRetinaClusters( LHCb::span<const uint32_t> bank ) const {

  assert( bank.size() == 1 + bank[0] );

  std::vector<VPRetinaMatrix> RetinaMatrixVector;
  RetinaMatrixVector.reserve( 20 );
  std::vector<uint32_t> RetinaCluster;

  // Read super pixel
  for ( const uint32_t sp_word : bank.subspan( 1 ) ) {

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
      iterRetina->AddSP( sp_row, sp_col, sp );
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
