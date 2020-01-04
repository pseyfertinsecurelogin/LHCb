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
#include <array>
#include <iomanip>
#include <tuple>
#include <vector>

struct VPRetinaClusterGeomCache {

  std::array<std::array<float, 9>, VP::NSensors> m_ltg; // 9*208 = 9*number of sensors
  LHCb::span<const double>                       m_local_x;
  LHCb::span<const double>                       m_x_pitch;
  float                                          m_pixel_size;

  VPRetinaClusterGeomCache( const DeVP& det ) {
    m_local_x    = det.sensor( 0 ).xLocal();
    m_x_pitch    = det.sensor( 0 ).xPitch();
    m_pixel_size = det.sensor( 0 ).pixelSize();
    det.runOnAllSensors( [this]( const DeVPSensor& sensor ) {
      // get the local to global transformation matrix and
      // store it in a flat float array of sixe 12.
      Gaudi::Rotation3D     ltg_rot;
      Gaudi::TranslationXYZ ltg_trans;
      sensor.getGlobalMatrixDecomposition( ltg_rot, ltg_trans );
      assert( sensor.sensorNumber() < m_ltg.size() );
      auto& ltg = m_ltg[sensor.sensorNumber()];
      ltg_rot.GetComponents( ltg.data() );
      ltg[2] = ltg_trans.X();
      ltg[5] = ltg_trans.Y();
      ltg[8] = ltg_trans.Z();
    } );
  }
};

// Namespace for locations in TDS
namespace LHCb {
  namespace VPClusterLocation {
    inline const std::string Offsets = "Raw/VP/LightClustersOffsets";
  }
} // namespace LHCb

class VPRetinaClusterDecoder
    : public Gaudi::Functional::MultiTransformer<
          std::tuple<std::vector<LHCb::VPLightCluster>, std::array<unsigned, VeloInfo::Numbers::NOffsets>>(
              const LHCb::RawEvent&, const VPRetinaClusterGeomCache& ),
          LHCb::DetDesc::usesConditions<VPRetinaClusterGeomCache>> {

public:
  /// Standard constructor
  VPRetinaClusterDecoder( const std::string& name, ISvcLocator* pSvcLocator );

  /// Algorithm initialization
  StatusCode initialize() override;

  /// Algorithm execution
  std::tuple<std::vector<LHCb::VPLightCluster>, std::array<unsigned, VeloInfo::Numbers::NOffsets>>
  operator()( const LHCb::RawEvent&, const VPRetinaClusterGeomCache& ) const override;

private:
  std::bitset<VP::NModules>                  m_modulesToSkipMask;
  Gaudi::Property<std::vector<unsigned int>> m_modulesToSkip{this,
                                                             "ModulesToSkip",
                                                             {},
                                                             [=]( auto& ) {
                                                               m_modulesToSkipMask.reset();
                                                               for ( auto i : m_modulesToSkip )
                                                                 m_modulesToSkipMask.set( i );
                                                             },
                                                             Gaudi::Details::Property::ImmediatelyInvokeHandler{true},
                                                             "List of modules that should be skipped in decoding"};
};

DECLARE_COMPONENT( VPRetinaClusterDecoder )

using namespace Pixel;

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
VPRetinaClusterDecoder::VPRetinaClusterDecoder( const std::string& name, ISvcLocator* pSvcLocator )
    : MultiTransformer( name, pSvcLocator,
                        {KeyValue{"RawEventLocation", LHCb::RawEventLocation::Default},
                         KeyValue{"GeomCacheLocation", "VPRetinaClusterDecoder-" + name + "-GeomCache"}},
                        {KeyValue{"ClusterLocation", LHCb::VPClusterLocation::Light},
                         KeyValue{"ClusterOffsets", LHCb::VPClusterLocation::Offsets}} ) {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode VPRetinaClusterDecoder::initialize() {
  return MultiTransformer::initialize().andThen(
      [&] { addConditionDerivation<VPRetinaClusterGeomCache( const DeVP& )>( *this, DeVPLocation::Default ); } );
}

//=============================================================================
// Main execution
//=============================================================================
std::tuple<std::vector<LHCb::VPLightCluster>, std::array<unsigned, VeloInfo::Numbers::NOffsets>>
VPRetinaClusterDecoder::operator()( const LHCb::RawEvent& rawEvent, const VPRetinaClusterGeomCache& cache ) const {
  auto result = std::tuple<std::vector<LHCb::VPLightCluster>, std::array<unsigned, VeloInfo::Numbers::NOffsets>>{};
  auto& [clusters, offsets] = result;

  const auto& banks = rawEvent.banks( LHCb::RawBank::VPRetinaCluster );
  debug() << "Number of retina cluster raw banks: " << banks.size() << "." << endmsg;
  if ( banks.empty() ) return result;

  const unsigned int version = banks[0]->version();
  if ( version != 1 ) {
    warning() << "Unsupported raw bank version (" << version << ")" << endmsg;
    return result;
  }

  // Since 'clusters` is local, to first preallocate, then count hits per module,
  // and then preallocate per module and move hits might not be faster than adding
  // directly to the PixelModuleHits (which would require more allocations, but
  // not many if we start with a sensible default)
  constexpr unsigned int startSize = 10000U;
  clusters.reserve( startSize );

  // Loop over VP RawBanks
  for ( const auto* bank : banks ) {

    const unsigned int module = bank->sourceID();
    if ( m_modulesToSkipMask[module - 1] ) continue;

    const unsigned int sensor0 = ( module - 1 ) * VP::NSensorsPerModule;

    auto data = bank->range<uint32_t>();
    assert( !data.empty() );

    const uint32_t ncluster = data[0];
    assert( data.size() == ncluster + 1 );
    data = data.subspan( 1 );

    // Read clusters
    std::transform( data.begin(), data.end(), std::back_inserter( clusters ),
                    [&]( const uint32_t cluster_word ) -> LHCb::VPLightCluster {
                      const unsigned int sensor = sensor0 + ( cluster_word >> 24 );
                      const uint32_t     cx     = ( cluster_word >> 14 ) & 0x3FF;
                      const float        fx     = ( ( cluster_word >> 11 ) & 0x7 ) / 8.f;
                      const uint32_t     cy     = ( cluster_word >> 3 ) & 0xFF;
                      const float        fy     = ( cluster_word & 0x7 ) / 8.f;
                      const uint32_t     chip   = cx / CHIP_COLUMNS;
                      const uint32_t     ccol   = cx % CHIP_COLUMNS;

                      const float local_x = cache.m_local_x[cx] + fx * cache.m_x_pitch[cx];
                      const float local_y = ( cy + 0.5 + fy ) * cache.m_pixel_size;

                      const auto& ltg = cache.m_ltg[sensor];
                      const float gx  = ( ltg[0] * local_x + ltg[1] * local_y + ltg[2] );
                      const float gy  = ( ltg[3] * local_x + ltg[4] * local_y + ltg[5] );
                      const float gz  = ( ltg[6] * local_x + ltg[7] * local_y + ltg[8] );

                      return {1, 1, gx, gy, gz, LHCb::VPChannelID{sensor, chip, ccol, cy}};
                    } );
    offsets[module] += ncluster;
  } // loop over all banks

  std::partial_sum( offsets.begin(), offsets.end(), offsets.begin() );

  return result;
}
