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
#pragma once
#include <stdexcept>

namespace Calo::Future::Enum {

  inline constexpr double Default = -1.e+06; // default value

  enum class DataType {
    HypoE = 0,     // 0 hypo energy
    HypoEt,        // 1 hypo Et
    HypoM,         // 2 hypo Mass
    HypoPrsE,      // 3 Prs extra-digit E
    HypoPrsM,      // 4 Prs extra-digit Multiplicity
    HypoSpdM,      // 5 Spd extra-digit Multiplicity
    ClusterE,      // 6 cluster energy  ( = E9 for 3x3 cluster evaluation)
    ToPrsE,        // 7 Prs deposit in front (using local setting of CaloFutureHypo2CaloFuture)
    ToHcalE,       // 8 Hcal deposit in front (using local setting of CaloFutureHypo2CaloFuture)
    ToPrsM,        // 9 Prs multiplicity
    ToSpdM,        // 10 Spd multiplicity
    E1,            // cluster seed energy
    E9,            // 3x3 area cluster
    E19,           // E1/E9
    E1Hypo,        // E1/HypoE
    E4,            // max (2x2) energy within 3x3
    E49,           // E4/E9
    CellID,        // cluster-seed id
    Hcal2Ecal,     // ToHcalE/ClusterE
    ClusterMatch,  // chi2(2D)
    ElectronMatch, // 20 chi2(e)
    BremMatch,     // chi2(brem)
    NeutralID,     // neutral pid
    Spread,        // cluster spread
    TrajectoryL,
    PrsE19,
    PrsE49,
    PrsE4Max,
    PrsE1,
    PrsE2,
    PrsE3, // 30
    PrsE4,
    PrsE5,
    PrsE6,
    PrsE7,
    PrsE8,
    PrsE9,
    isPhoton,
    isPhotonEcl,
    isPhotonFr2,
    isPhotonFr2r4,
    isPhotonAsym, // 40
    isPhotonKappa,
    isPhotonEseed,
    isPhotonE2,
    isPhotonPrsFr2,
    isPhotonPrsAsym,
    isPhotonPrsM,
    isPhotonPrsM15,
    isPhotonPrsM30,
    isPhotonPrsM45,
    isPhotonPrsEmax, // 50
    isPhotonPrsE2,
    isNotH,
    isNotE,
    ClusterCode,
    ClusterFrac,
    Saturation,
    ClusterAsX,
    ClusterAsY,
    isPhotonXGB,
  };

  template <typename T>
  constexpr int count();
  template <>
  inline constexpr int count<DataType>() {
    return 61;
  }

  inline int toTypeMask( DataType dt ) { // 0x1 : neutral ; 0x2 : charged ; 0x3 : both
    switch ( dt ) {
    case DataType::HypoE:
    case DataType::HypoEt:
    case DataType::HypoM:
    case DataType::HypoPrsE:
    case DataType::HypoPrsM:

    case DataType::HypoSpdM:
    case DataType::ClusterE:
    case DataType::ToPrsE:
    case DataType::ToHcalE:
    case DataType::ToPrsM:

    case DataType::ToSpdM:
    case DataType::E1:
    case DataType::E9:
    case DataType::E19:
    case DataType::E1Hypo:

    case DataType::E4:
    case DataType::E49:
    case DataType::CellID:
    case DataType::Hcal2Ecal:
    case DataType::ClusterMatch:
      return 0x3;

    case DataType::ElectronMatch:
    case DataType::BremMatch:
      return 0x2;
    case DataType::NeutralID:
      return 0x1;
    case DataType::Spread:
      return 0x3;
    case DataType::TrajectoryL:
      return 0x2;

    case DataType::PrsE19:
    case DataType::PrsE49:
    case DataType::PrsE4Max:
    case DataType::PrsE1:
    case DataType::PrsE2:

    case DataType::PrsE3:
    case DataType::PrsE4:
    case DataType::PrsE5:
    case DataType::PrsE6:
    case DataType::PrsE7:

    case DataType::PrsE8:
    case DataType::PrsE9:
      return 0x3;
    case DataType::isPhoton:
    case DataType::isPhotonEcl:
    case DataType::isPhotonFr2:

    case DataType::isPhotonFr2r4:
    case DataType::isPhotonAsym:
    case DataType::isPhotonKappa:
    case DataType::isPhotonEseed:
    case DataType::isPhotonE2:

    case DataType::isPhotonPrsFr2:
    case DataType::isPhotonPrsAsym:
    case DataType::isPhotonPrsM:
    case DataType::isPhotonPrsM15:
    case DataType::isPhotonPrsM30:

    case DataType::isPhotonPrsM45:
    case DataType::isPhotonPrsEmax:
    case DataType::isPhotonPrsE2:
      return 0x1;
    case DataType::isNotH:
    case DataType::isNotE:
      return 0x1;

    case DataType::ClusterCode:
    case DataType::ClusterFrac:
      return 0x3;
    case DataType::Saturation:
    case DataType::ClusterAsX:
    case DataType::ClusterAsY:

    case DataType::isPhotonXGB:
      return 0x1;
    }
  }
  inline const char* toString( DataType dt ) {
    switch ( dt ) {
    case DataType::HypoE:
      return "HypoE"; // 0
    case DataType::HypoEt:
      return "HypoEt";
    case DataType::HypoM:
      return "HypoM";
    case DataType::HypoPrsE:
      return "HypoPrsE";
    case DataType::HypoPrsM:
      return "HypoPrsM";
    case DataType::HypoSpdM:
      return "HypoSpdM";
    case DataType::ClusterE:
      return "ClusterE";
    case DataType::ToPrsE:
      return "ToPrsE";
    case DataType::ToHcalE:
      return "ToHcalE";
    case DataType::ToPrsM:
      return "ToPrsM";
    case DataType::ToSpdM:
      return "ToSpdM"; // 10
    case DataType::E1:
      return "E1";
    case DataType::E9:
      return "E9";
    case DataType::E19:
      return "E19";
    case DataType::E1Hypo:
      return "E1Hypo";
    case DataType::E4:
      return "E4";
    case DataType::E49:
      return "E49";
    case DataType::CellID:
      return "CellID";
    case DataType::Hcal2Ecal:
      return "Hcal2Ecal";
    case DataType::ClusterMatch:
      return "ClusterMatch";
    case DataType::ElectronMatch:
      return "ElectronMatch"; // 20
    case DataType::BremMatch:
      return "BremMatch";
    case DataType::NeutralID:
      return "NeutralID";
    case DataType::Spread:
      return "Spread";
    case DataType::TrajectoryL:
      return "TrajectoryL";
    case DataType::PrsE19:
      return "PrsE19";
    case DataType::PrsE49:
      return "PrsE49";
    case DataType::PrsE4Max:
      return "PrsE4Max";
    case DataType::PrsE1:
      return "PrsE1";
    case DataType::PrsE2:
      return "PrsE2";
    case DataType::PrsE3:
      return "PrsE3"; // 30
    case DataType::PrsE4:
      return "PrsE4";
    case DataType::PrsE5:
      return "PrsE5";
    case DataType::PrsE6:
      return "PrsE6";
    case DataType::PrsE7:
      return "PrsE7";
    case DataType::PrsE8:
      return "PrsE8";
    case DataType::PrsE9:
      return "PrsE9";
    case DataType::isPhoton:
      return "isPhoton";
    case DataType::isPhotonEcl:
      return "isPhoton_Ecl";
    case DataType::isPhotonFr2:
      return "isPhoton_Fr2";
    case DataType::isPhotonFr2r4:
      return "isPhoton_Fr2r4"; // 40
    case DataType::isPhotonAsym:
      return "isPhoton_Asym";
    case DataType::isPhotonKappa:
      return "isPhoton_Kappa";
    case DataType::isPhotonEseed:
      return "isPhoton_Eseed";
    case DataType::isPhotonE2:
      return "isPhoton_E2";
    case DataType::isPhotonPrsFr2:
      return "isPhoton_PrsFr2";
    case DataType::isPhotonPrsAsym:
      return "isPhoton_PrsAsym";
    case DataType::isPhotonPrsM:
      return "isPhoton_PrsM";
    case DataType::isPhotonPrsM15:
      return "isPhoton_PrsM15";
    case DataType::isPhotonPrsM30:
      return "isPhoton_PrsM30";
    case DataType::isPhotonPrsM45:
      return "isPhoton_PrsM45"; // 50
    case DataType::isPhotonPrsEmax:
      return "isPhoton_PrsEmax";
    case DataType::isPhotonPrsE2:
      return "isPhoton_PrsE2";
    case DataType::isNotH:
      return "isNotH";
    case DataType::isNotE:
      return "isNotE";
    case DataType::ClusterCode:
      return "ClusterCode";
    case DataType::ClusterFrac:
      return "ClusterFrac";
    case DataType::Saturation:
      return "Saturation";
    case DataType::ClusterAsX:
      return "ClusterAsX";
    case DataType::ClusterAsY:
      return "ClusterAsY";
    case DataType::isPhotonXGB:
      return "isPhotonXGB";
    };
    throw std::runtime_error( "Unknown DataType" );
  }

  enum class MatchType { ClusterMatch = 0, ElectronMatch, BremMatch };
  inline const char* toString( MatchType mt ) {
    switch ( mt ) {
    case MatchType::ClusterMatch:
      return "ClusterMatch";
    case MatchType::ElectronMatch:
      return "ElectronMatch";
    case MatchType::BremMatch:
      return "BremMatch";
    }
    throw std::runtime_error( "Unknown MatchType" );
  }

  enum class ClusterType {
    Main = 0,
    SplitOrMain,
  };
  inline const char* toString( ClusterType ct ) {
    switch ( ct ) {
    case ClusterType::Main:
      return "Main";
    case ClusterType::SplitOrMain:
      return "SplitOrMain";
    }
    throw std::runtime_error( "Unknown ClusterType" );
  }
} // namespace Calo::Future::Enum
