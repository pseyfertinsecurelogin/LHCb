#ifndef DICT_LHCBKERNELDICT_H
#define DICT_LHCBKERNELDICT_H 1

// Additional classes to be added to automatically generated lcgdict

// begin include files
#include <map>
#include <vector>
#include <utility>
#include "GaudiKernel/VectorMap.h"
#include "Kernel/VeloChannelID.h"
#include "Kernel/VeloPixChannelID.h"
#include "Kernel/STChannelID.h"
#include "Kernel/OTChannelID.h"
#include "Kernel/CaloCellID.h"
#include "Kernel/MuonTileID.h"
#include "Kernel/MuonLayout.h"
#include "Kernel/Trajectory.h"
#include "Kernel/AlignTraj.h"
#include "Kernel/CircleTraj.h"
#include "Kernel/LineTraj.h"
#include "Kernel/ParabolaTraj.h"
#include "Kernel/SiPositionInfo.h"
#include "Kernel/PixelPositionInfo.h"
#include "Kernel/RichSmartID.h"

#include "Kernel/IAlgorithmCorrelations.h"
#include "Kernel/IBdlTool.h"
#include "Kernel/IBIntegrator.h"
#include "Kernel/ICheckTool.h"
#include "Kernel/ICondDBInfo.h"
#include "Kernel/ICountContainedObjects.h"
#include "Kernel/IEvtTypeSvc.h"
#include "Kernel/IInAcceptance.h"
#include "Kernel/ILHCbMagnetSvc.h"
#include "Kernel/IMCVeloFEType.h"
#include "Kernel/INormalizeTool.h"
#include "Kernel/ISiAmplifierResponse.h"
#include "Kernel/ITrajPoca.h"
#include "Kernel/IUsedLHCbID.h"
#include "Kernel/TTNames.h"
#include "Kernel/ITNames.h"
#include "Kernel/Inspectors.h"
#include "Kernel/HitPattern.h"
#include "Kernel/IReferenceRate.h"
#include "Kernel/IAccept.h"
#include "Kernel/Counters.h"

#include "Kernel/RichParticleIDType.h"
#include "Kernel/RichRadiatorType.h"
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichSide.h"
#include "Kernel/RichRadIntersection.h"

// end include files

namespace {
  struct _Instantiations {
    // begin instantiations
    GaudiUtils::VectorMap<int,double>                   m_vector_map_int_double;
    std::pair<LHCb::VeloChannelID,int>                  m_std_pair_VeloChannelID_int;
    std::pair<LHCb::VeloPixChannelID,int>               m_std_pair_VeloPixChannelID_int;
    std::pair<LHCb::STChannelID,int>                    m_std_pair_STChannelID_int;
    std::pair<LHCb::OTChannelID,int>                    m_std_pair_OTChannelID_int;
    std::pair<double,LHCb::CaloCellID>                  m_std_pair_double_CaloCellID;
    std::pair<LHCb::MuonTileID,int>                     m_std_pair_MuonTileID_int;
    std::vector<std::pair<LHCb::VeloChannelID,int> >    m_std_vector_std_pair_VeloChannelID_int;
    std::vector<std::pair<LHCb::VeloPixChannelID,int> > m_std_vector_std_pair_VeloPixChannelID_int;
    std::vector<std::pair<LHCb::STChannelID,int> >      m_std_vector_std_pair_STChannelID_int;
    std::vector<std::pair<LHCb::OTChannelID,int> >      m_std_vector_std_pair_OTChannelID_int;
    std::vector<std::pair<double,LHCb::CaloCellID> >    m_std_vector_std_pair_double_CaloCellID;
    std::vector<std::pair<LHCb::MuonTileID,int> >       m_std_vector_std_pair_MuonTileID_int;
    std::auto_ptr<LHCb::Trajectory>                     m_auto_ptr_Trajectory;
    LHCb::SiPositionInfo<LHCb::STChannelID>             m_SiPositionInfo_STChannelID;
    LHCb::SiPositionInfo<LHCb::VeloChannelID>           m_SiPositionInfo_VeloChannelID;
    LHCb::PixelPositionInfo<LHCb::VeloPixChannelID>     m_PixelPositionInfo_VeloPixChannelID;
    std::pair<Gaudi::XYZPoint,Gaudi::XYZPoint>          m_std_pair_XYZPoint_XYZPoint;
    std::vector<LHCb::VeloChannelID>                    m_std_vector_VeloChannelID;
    std::vector<LHCb::VeloPixChannelID>                 m_std_vector_VeloPixChannelID;
    std::vector<LHCb::LHCbID>::const_iterator           m_std_vector_LHCbID_const_iterator;
    std::map<std::pair<int,unsigned int>,unsigned int>  m_L0DUReport_Summaries;
    Rich::Radiators                                     m_RICH_Radiators;
    Rich::RadiatorTypeVector                            m_RICH_Rad_V;
    Rich::Detectors                                     m_RICH_Detectors;
    Rich::DetectorVector                                m_RICH_Det_V;
    Rich::ParticleIDTypeVector                          m_RICH_PID_V;
    Rich::Particles                                     m_RICH_Particles;
    Rich::RichSideVector                                m_RICH_Side_V;
    Rich::Sides                                         m_RICH_Sides;
    //
    std::pair<std::string,double>     m_pair_1 ;
    std::pair<std::string,StatEntity> m_pair_2 ;
    // end instantiations
  };
}

#endif // DICT_LHCBKERNELDICT_H
