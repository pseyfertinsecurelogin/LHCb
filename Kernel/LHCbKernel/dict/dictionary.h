/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

// begin include files
#include "GaudiKernel/VectorMap.h"
#include "Kernel/AlignTraj.h"
#include "Kernel/CaloCellID.h"
#include "Kernel/CircleTraj.h"
#include "Kernel/Counters.h"
#include "Kernel/FTChannelID.h"
#include "Kernel/HCCellID.h"
#include "Kernel/HitPattern.h"
#include "Kernel/IAccept.h"
#include "Kernel/IAlgorithmCorrelations.h"
#include "Kernel/IBIntegrator.h"
#include "Kernel/ICheckTool.h"
#include "Kernel/ICondDBInfo.h"
#include "Kernel/ICountContainedObjects.h"
#include "Kernel/IDQFilter.h"
#include "Kernel/IDQScanner.h"
#include "Kernel/IEventCounter.h"
#include "Kernel/IEventTupleTool.h"
#include "Kernel/IEvtTypeSvc.h"
#include "Kernel/ILHCbMagnetSvc.h"
#include "Kernel/IMCVeloFEType.h"
#include "Kernel/INormalizeTool.h"
#include "Kernel/IReferenceRate.h"
#include "Kernel/ISiAmplifierResponse.h"
#include "Kernel/ITNames.h"
#include "Kernel/ITrajPoca.h"
#include "Kernel/IUsedLHCbID.h"
#include "Kernel/IVeloCCEConfigTool.h"
#include "Kernel/Inspectors.h"
#include "Kernel/LHCbID.h"
#include "Kernel/LineTraj.h"
#include "Kernel/MuonLayout.h"
#include "Kernel/MuonTileID.h"
#include "Kernel/OTChannelID.h"
#include "Kernel/Packer.h"
#include "Kernel/ParabolaTraj.h"
#include "Kernel/PixelPositionInfo.h"
#include "Kernel/PlatformInfo.h"
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichParticleIDType.h"
#include "Kernel/RichRadIntersection.h"
#include "Kernel/RichRadiatorType.h"
#include "Kernel/RichSide.h"
#include "Kernel/RichSmartID.h"
#include "Kernel/RichTraceMode.h"
#include "Kernel/STChannelID.h"
#include "Kernel/SiPositionInfo.h"
#include "Kernel/TTNames.h"
#include "Kernel/Trajectory.h"
#include "Kernel/UTChannelID.h"
#include "Kernel/UTNames.h"
#include "Kernel/VPChannelID.h"
#include "Kernel/VeloChannelID.h"
#include <map>
#include <utility>
#include <vector>

// end include files

namespace {
  struct LHCbKernel_Instantiations {
    // begin instantiations
    // Rich::DetectorVector                              m_RICH_Det_V;
    // Rich::ParticleIDTypeVector                        m_RICH_PID_V;
    // Rich::RadiatorTypeVector                          m_RICH_Rad_V;
    // Rich::RichSideVector                              m_RICH_Side_V;
    GaudiUtils::VectorMap<int, double>                   m_vector_map_int_double;
    LHCb::PixelPositionInfo<LHCb::VPChannelID>           m_PixelPositionInfo_VPChannelID;
    LHCb::RichSmartID::Vector                            m_RICH_SmartID_V;
    LHCb::SiPositionInfo<LHCb::STChannelID>              m_SiPositionInfo_STChannelID;
    LHCb::SiPositionInfo<LHCb::UTChannelID>              m_SiPositionInfo_UTChannelID;
    LHCb::SiPositionInfo<LHCb::VeloChannelID>            m_SiPositionInfo_VeloChannelID;
    Rich::Detectors                                      m_RICH_Detectors;
    Rich::Particles                                      m_RICH_Particles;
    Rich::Radiators                                      m_RICH_Radiators;
    Rich::Sides                                          m_RICH_Sides;
    std::map<std::pair<int, unsigned int>, unsigned int> m_L0DUReport_Summaries;
    std::pair<Gaudi::XYZPoint, Gaudi::XYZPoint>          m_std_pair_XYZPoint_XYZPoint;
    std::pair<LHCb::HCCellID, int>                       m_std_pair_HCCellID_int;
    std::pair<LHCb::MuonTileID, int>                     m_std_pair_MuonTileID_int;
    std::pair<LHCb::OTChannelID, int>                    m_std_pair_OTChannelID_int;
    std::pair<LHCb::STChannelID, int>                    m_std_pair_STChannelID_int;
    std::pair<LHCb::UTChannelID, int>                    m_std_pair_UTChannelID_int;
    std::pair<LHCb::VPChannelID, int>                    m_std_pair_VPChannelID_int;
    std::pair<LHCb::VeloChannelID, int>                  m_std_pair_VeloChannelID_int;
    std::pair<double, LHCb::CaloCellID>                  m_std_pair_double_CaloCellID;
    std::pair<std::string, StatEntity>                   m_pair_2;
    std::pair<std::string, double>                       m_pair_1;
    std::vector<LHCb::CaloCellID>                        m_std_vector_LHCb__CaloCellID;
    std::vector<LHCb::FTChannelID>                       m_std_vector_LHCb__FTChannelID;
    std::vector<LHCb::HCCellID>                          m_std_vector_HCCellID;
    std::vector<LHCb::LHCbID>                            m_std_vector_LHCb__LHCbID;
    std::vector<LHCb::LHCbID>::const_iterator            m_std_vector_LHCbID_const_iterator;
    std::vector<LHCb::MuonTileID>                        m_std_vector_LHCb__MuonTileID;
    std::vector<LHCb::OTChannelID>                       m_std_vector_LHCb__OTChannelID;
    std::vector<LHCb::STChannelID>                       m_std_vector_LHCb__STChannelID;
    std::vector<LHCb::UTChannelID>                       m_std_vector_LHCb__UTChannelID;
    std::vector<LHCb::VPChannelID>                       m_std_vector_VPChannelID;
    std::vector<LHCb::VeloChannelID>                     m_std_vector_VeloChannelID;
    std::vector<std::pair<LHCb::HCCellID, int>>          m_std_vector_std_pair_HCCellID_int;
    std::vector<std::pair<LHCb::MuonTileID, int>>        m_std_vector_std_pair_MuonTileID_int;
    std::vector<std::pair<LHCb::OTChannelID, int>>       m_std_vector_std_pair_OTChannelID_int;
    std::vector<std::pair<LHCb::STChannelID, int>>       m_std_vector_std_pair_STChannelID_int;
    std::vector<std::pair<LHCb::UTChannelID, int>>       m_std_vector_std_pair_UTChannelID_int;
    std::vector<std::pair<LHCb::VPChannelID, int>>       m_std_vector_std_pair_VPChannelID_int;
    std::vector<std::pair<LHCb::VeloChannelID, int>>     m_std_vector_std_pair_VeloChannelID_int;
    std::vector<std::pair<double, LHCb::CaloCellID>>     m_std_vector_std_pair_double_CaloCellID;
    // end instantiations
  };
} // namespace
