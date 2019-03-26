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
#ifndef DICT_LHCBKERNELDICT_H
#define DICT_LHCBKERNELDICT_H 1

// Additional classes to be added to automatically generated lcgdict

// begin include files
#include "GaudiKernel/VectorMap.h"
#include "Kernel/AlignTraj.h"
#include "Kernel/CaloCellID.h"
#include "Kernel/CircleTraj.h"
#include "Kernel/HCCellID.h"
#include "Kernel/LineTraj.h"
#include "Kernel/MuonLayout.h"
#include "Kernel/MuonTileID.h"
#include "Kernel/OTChannelID.h"
#include "Kernel/ParabolaTraj.h"
#include "Kernel/PixelPositionInfo.h"
#include "Kernel/RichSmartID.h"
#include "Kernel/STChannelID.h"
#include "Kernel/SiPositionInfo.h"
#include "Kernel/Trajectory.h"
#include "Kernel/UTChannelID.h"
#include "Kernel/VPChannelID.h"
#include "Kernel/VeloChannelID.h"
#include <map>
#include <utility>
#include <vector>

#include "Kernel/PlatformInfo.h"

#include "Kernel/Counters.h"
#include "Kernel/HitPattern.h"
#include "Kernel/IAccept.h"
#include "Kernel/IAlgorithmCorrelations.h"
#include "Kernel/IBIntegrator.h"
#include "Kernel/IBdlTool.h"
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
#include "Kernel/TTNames.h"

#include "Kernel/RichDetectorType.h"
#include "Kernel/RichParticleIDType.h"
#include "Kernel/RichRadIntersection.h"
#include "Kernel/RichRadiatorType.h"
#include "Kernel/RichSide.h"

// end include files

namespace {
  struct _Instantiations {
    // begin instantiations
    GaudiUtils::VectorMap<int, double>                   m_vector_map_int_double;
    std::pair<LHCb::VeloChannelID, int>                  m_std_pair_VeloChannelID_int;
    std::pair<LHCb::VPChannelID, int>                    m_std_pair_VPChannelID_int;
    std::pair<LHCb::STChannelID, int>                    m_std_pair_STChannelID_int;
    std::pair<LHCb::UTChannelID, int>                    m_std_pair_UTChannelID_int;
    std::pair<LHCb::OTChannelID, int>                    m_std_pair_OTChannelID_int;
    std::pair<double, LHCb::CaloCellID>                  m_std_pair_double_CaloCellID;
    std::pair<LHCb::MuonTileID, int>                     m_std_pair_MuonTileID_int;
    std::pair<LHCb::HCCellID, int>                       m_std_pair_HCCellID_int;
    std::vector<std::pair<LHCb::VeloChannelID, int>>     m_std_vector_std_pair_VeloChannelID_int;
    std::vector<std::pair<LHCb::VPChannelID, int>>       m_std_vector_std_pair_VPChannelID_int;
    std::vector<std::pair<LHCb::STChannelID, int>>       m_std_vector_std_pair_STChannelID_int;
    std::vector<std::pair<LHCb::UTChannelID, int>>       m_std_vector_std_pair_UTChannelID_int;
    std::vector<std::pair<LHCb::OTChannelID, int>>       m_std_vector_std_pair_OTChannelID_int;
    std::vector<std::pair<double, LHCb::CaloCellID>>     m_std_vector_std_pair_double_CaloCellID;
    std::vector<std::pair<LHCb::MuonTileID, int>>        m_std_vector_std_pair_MuonTileID_int;
    std::vector<std::pair<LHCb::HCCellID, int>>          m_std_vector_std_pair_HCCellID_int;
    LHCb::SiPositionInfo<LHCb::STChannelID>              m_SiPositionInfo_STChannelID;
    LHCb::SiPositionInfo<LHCb::UTChannelID>              m_SiPositionInfo_UTChannelID;
    LHCb::SiPositionInfo<LHCb::VeloChannelID>            m_SiPositionInfo_VeloChannelID;
    LHCb::PixelPositionInfo<LHCb::VPChannelID>           m_PixelPositionInfo_VPChannelID;
    std::pair<Gaudi::XYZPoint, Gaudi::XYZPoint>          m_std_pair_XYZPoint_XYZPoint;
    std::vector<LHCb::VeloChannelID>                     m_std_vector_VeloChannelID;
    std::vector<LHCb::VPChannelID>                       m_std_vector_VPChannelID;
    std::vector<LHCb::HCCellID>                          m_std_vector_HCCellID;
    std::vector<LHCb::LHCbID>::const_iterator            m_std_vector_LHCbID_const_iterator;
    std::map<std::pair<int, unsigned int>, unsigned int> m_L0DUReport_Summaries;
    Rich::Radiators                                      m_RICH_Radiators;
    // Rich::RadiatorTypeVector                            m_RICH_Rad_V;
    Rich::Detectors m_RICH_Detectors;
    // Rich::DetectorVector                                m_RICH_Det_V;
    // Rich::ParticleIDTypeVector                          m_RICH_PID_V;
    Rich::Particles m_RICH_Particles;
    // Rich::RichSideVector                                m_RICH_Side_V;
    Rich::Sides                        m_RICH_Sides;
    LHCb::RichSmartID::Vector          m_RICH_SmartID_V;
    std::pair<std::string, double>     m_pair_1;
    std::pair<std::string, StatEntity> m_pair_2;
    // end instantiations
  };
} // namespace

#endif // DICT_LHCBKERNELDICT_H
