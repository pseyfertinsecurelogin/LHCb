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
#ifndef VPRETINACLUSTERDECODER_H
#define VPRETINACLUSTERDECODER_H 1

#include <array>
#include <tuple>
#include <vector>

// Gaudi
#include "GaudiAlg/Transformer.h"

// LHCb
#include "Event/RawBank.h"
#include "Event/RawEvent.h"
#include "Event/VPLightCluster.h"
#include "Kernel/VPChannelID.h"
#include "Kernel/VPConstants.h"
#include "VPDet/DeVP.h"
#include "VPKernel/PixelModule.h"
#include "VPKernel/VeloPixelInfo.h"

// Namespace for locations in TDS
namespace LHCb {
  namespace VPClusterLocation {
    inline const std::string Offsets = "Raw/VP/LightClustersOffsets";
  }
} // namespace LHCb

class VPRetinaClusterDecoder
    : public Gaudi::Functional::MultiTransformer<
          std::tuple<std::vector<LHCb::VPLightCluster>, std::array<unsigned, VeloInfo::Numbers::NOffsets>>(
              const LHCb::RawEvent& )> {

public:
  /// Standard constructor
  VPRetinaClusterDecoder( const std::string& name, ISvcLocator* pSvcLocator );

  /// Algorithm initialization
  StatusCode initialize() override;

  /// Algorithm execution
  std::tuple<std::vector<LHCb::VPLightCluster>, std::array<unsigned, VeloInfo::Numbers::NOffsets>>
  operator()( const LHCb::RawEvent& ) const override;

private:
  /// Recompute the geometry in case of change
  StatusCode rebuildGeometry();
  //
  /// Detector element
  DeVP* m_vp{nullptr};

  /// List of pointers to modules (which contain pointers to their hits)
  std::vector<PixelModule*> m_modules;
  std::vector<PixelModule>  m_module_pool;

  /// Indices of first and last module
  unsigned int m_firstModule;
  unsigned int m_lastModule;

  std::array<std::array<float, 9>, VP::NSensors> m_ltg; // 9*208 = 9*number of sensors
  LHCb::span<const double>                       m_local_x;
  LHCb::span<const double>                       m_x_pitch;
  float                                          m_pixel_size;

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

#endif // VPRETINACLUSTERDECODER_H
