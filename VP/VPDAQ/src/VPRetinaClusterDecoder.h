#ifndef VPRETINACLUSTERDECODER_H
#define VPRETINACLUSTERDECODER_H 1

#include <array>
#include <tuple>
#include <vector>

// Gaudi
#include "GaudiAlg/Transformer.h"

// LHCb
#include "Event/RawEvent.h"
#include "Event/RawBank.h"
#include "Event/VPLightCluster.h"
#include "Kernel/VPChannelID.h"
#include "Kernel/VPConstants.h"
#include "VPDet/DeVP.h"
#include "VPKernel/PixelModule.h"
#include "VPKernel/VeloPixelInfo.h"


// Namespace for locations in TDS
namespace LHCb
{
  namespace VPClusterLocation
  {
    static const std::string Offsets = "Raw/VP/LightClustersOffsets";
  }
}

class VPRetinaClusterDecoder
  : public Gaudi::Functional::MultiTransformer<std::tuple<std::vector<LHCb::VPLightCluster>,
                                                          std::array<unsigned, VeloInfo::Numbers::NOffsets>>(
           const LHCb::RawEvent& )>
{

public:
  /// Standard constructor
  VPRetinaClusterDecoder( const std::string& name, ISvcLocator* pSvcLocator );

  /// Algorithm initialization
  StatusCode initialize() override;

  /// Algorithm execution
  std::tuple<std::vector<LHCb::VPLightCluster>, std::array<unsigned,VeloInfo::Numbers::NOffsets >>
  operator()( const LHCb::RawEvent& ) const override;

private:

  /// Recompute the geometry in case of change
  StatusCode rebuildGeometry();
  //
  /// Detector element
  DeVP* m_vp{nullptr};

  /// List of pointers to modules (which contain pointers to their hits)
  std::vector<PixelModule*> m_modules;
  std::vector<PixelModule> m_module_pool;

  /// Indices of first and last module
  unsigned int m_firstModule;
  unsigned int m_lastModule;

  float m_ltg[16 * VP::NSensors]; // 16*208 = 16*number of sensors
  LHCb::span<const double> m_local_x;
  LHCb::span<const double> m_x_pitch;
  float m_pixel_size;

  Gaudi::Property<std::vector<unsigned int>> m_modulesToSkip{ this, "ModulesToSkip",{}, 
      "List of modules that should be skipped in decoding"};
  std::bitset<VP::NModules> m_modulesToSkipMask;

};

#endif // VPRETINACLUSTERDECODER_H

