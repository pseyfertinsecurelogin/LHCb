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
#include "CaloFutureUtils/CaloFutureAlgUtils.h"
#include "CaloDet/DeCalorimeter.h"
#include "CaloFutureUtils/CaloFuture2Track.h"
#include "Event/CaloAdc.h"
#include "Event/CaloCluster.h"
#include "Event/CaloDigit.h"
#include "Event/CaloHypo.h"
#include "Event/ProtoParticle.h"
#include "Event/RawBankReadoutStatus.h"
#include "GaudiKernel/IRegistry.h"
#include <algorithm>

using LHCb::CaloFutureAlgUtils::details::contains_ci;
namespace {
  std::string pathFromContext( const std::initializer_list<std::pair<std::string_view, std::string_view>>& table,
                               std::string_view                                                            val ) {
    auto i = std::find_if(
        std::begin( table ), std::end( table ),
        [&]( const std::pair<std::string_view, std::string_view>& item ) { return contains_ci( val, item.first ); } );
    return i != std::end( table ) ? LHCb::CaloFutureAlgUtils::details::to_string( i->second ) : std::string{};
  }
} // namespace

// Default location for CaloFutureObject as function of detector

// DetectorElement location from name
std::string LHCb::CaloFutureAlgUtils::DeCaloFutureLocation( const std::string& name ) {
  std::string det = CaloFutureNameFromAlg( name );
  return det == "Ecal" ? DeCalorimeterLocation::Ecal
                       : det == "Hcal" ? DeCalorimeterLocation::Hcal
                                       : det == "Prs" ? DeCalorimeterLocation::Prs
                                                      : det == "Spd" ? DeCalorimeterLocation::Spd : "";
}

// ADC location from name
std::string LHCb::CaloFutureAlgUtils::CaloFutureRawBankReadoutStatusLocation( const std::string& name ) {
  std::string det = CaloFutureNameFromAlg( name );
  using namespace LHCb::RawBankReadoutStatusLocation;
  return det == "Ecal" ? LHCb::RawBankReadoutStatusLocation::Ecal
                       : det == "Hcal" ? LHCb::RawBankReadoutStatusLocation::Hcal : "";
}

// ADC location from name
std::string LHCb::CaloFutureAlgUtils::CaloFutureAdcLocation( const std::string& name ) {
  std::string det = CaloFutureNameFromAlg( name );
  using namespace LHCb::CaloAdcLocation;
  return det == "Ecal" ? Ecal : det == "Hcal" ? Hcal : "";
}

// Digit location from name
std::string LHCb::CaloFutureAlgUtils::CaloFutureDigitLocation( const std::string& name ) {
  std::string det = CaloFutureNameFromAlg( name );
  using namespace LHCb::CaloDigitLocation;
  return det == "Ecal"
             ? Ecal
             : det == "Hcal" ? Hcal : det == "Prs" ? Prs : det == "Spd" ? Spd : Ecal; // default is Ecal in offline mode
}

// Digit location from name
std::string LHCb::CaloFutureAlgUtils::CaloFutureUnfilteredDigitLocation( const std::string& name ) {
  std::string det = CaloFutureNameFromAlg( name );
  using namespace LHCb::CaloDigitLocation;
  return det == "Ecal" ? UnfilteredEcal
                       : det == "Hcal" ? UnfilteredHcal : UnfilteredEcal; // default is Ecal in offline mode
}

// Cluster location from context
std::string LHCb::CaloFutureAlgUtils::CaloFutureSplitClusterLocation() { return LHCb::CaloClusterLocation::EcalSplit; }

std::string LHCb::CaloFutureAlgUtils::CaloFutureClusterLocation( const std::string& name, const std::string& type ) {
  //##  splitClusters
  if ( contains_ci( name, "SPLITCLUSTER" ) ) return CaloFutureSplitClusterLocation();

  // ## standard clusters
  std::string det = CaloFutureNameFromAlg( name );

  using namespace LHCb::CaloClusterLocation;
  std::string clusters;
  if ( det == "Hcal" ) {
    clusters = Hcal;
  } else {
    if ( type == "EcalRaw" ) {
      clusters = EcalRaw;
    } else if ( type == "EcalOverlap" ) {
      clusters = EcalOverlap;
    } else {
      clusters = Ecal;
    }
  }
  return clusters;
}

// Hypo location from type
std::string LHCb::CaloFutureAlgUtils::CaloFutureHypoLocation( const std::string& type ) {
  using namespace LHCb::CaloHypoLocation;
  static const std::initializer_list<std::pair<std::string_view, std::string_view>> table = {
      {"ELECTRON", Electrons},
      {"SPLITPHOTON", SplitPhotons},
      {"BREM", Photons},   // Brem=photon
      {"PHOTON", Photons}, // after split
      {"MERGED", MergedPi0s}};
  return pathFromContext( table, type );
}

// CaloFutureId location from type
std::string LHCb::CaloFutureAlgUtils::CaloFutureIdLocation( const std::string& type ) {
  using namespace LHCb::CaloFutureIdLocation;
  static const std::initializer_list<std::pair<std::string_view, std::string_view>> table = {
      {"BREMMATCH", BremMatch},
      {"ELECTRONMATCH", ElectronMatch},
      {"PHOTONMATCH", ClusterMatch}, //=ClusterMatch
      {"CLUSTERMATCH", ClusterMatch},
      {"BREMCHI2", BremChi2},
      {"ECALCHI2", EcalChi2},
      {"CLUSCHI2", ClusChi2},
      {"ECALE", EcalE},
      {"HCALE", HcalE},
      {"PRSE", PrsE},
      {"SPDE", SpdE},
      {"PRSPIDE", PrsPIDe},
      {"BREMPIDE", BremPIDe},
      {"ECALPIDE", EcalPIDe},
      {"HCALPIDE", HcalPIDe},
      {"ECALPIDMU", EcalPIDmu},
      {"HCALPIDMU", HcalPIDmu},
      {"INBREM", InBrem},
      {"INECAL", InEcal},
      {"INHCAL", InHcal},
      {"INPRS", InPrs},
      {"INSPD", InSpd},
      {"PHOTONFROMMERGEDID", PhotonFromMergedID},
      {"SPLITPHOTONID", PhotonFromMergedID}, // same
      {"PHOTONID", PhotonID},                // after SplitPhotonID
      {"MERGEDID", MergedID},                // after PhotonFromMerged
  };
  return pathFromContext( table, type );
}

// Track location
std::vector<std::string> LHCb::CaloFutureAlgUtils::TrackLocations() {
  return {LHCb::TrackLocation::Default}; // default is offline
}

const LHCb::CaloCluster* LHCb::CaloFutureAlgUtils::ClusterFromHypo( const LHCb::CaloHypo* hypo, bool split ) {
  if ( !hypo ) return nullptr;

  if ( 1 == hypo->clusters().size() ) return hypo->clusters().front(); // single hypo

  const auto& clusters = hypo->clusters();
  // split hypo
  if ( hypo->parent() && hypo->parent()->registry() ) {
    bool hsplit =
        ( contains_ci( hypo->parent()->registry()->identifier(), "SPLIT" ) && 1 < clusters.size() ); // split hypo
    if ( !hsplit ) return clusters.front(); // SHOULD NEVER OCCUR
    const LHCb::CaloCluster* cluster = nullptr;
    for ( const auto& cl : clusters ) {
      bool split_container = contains_ci( cl->parent()->registry()->identifier(), "SPLIT" );
      if ( split && split_container )
        cluster = cl; // return splitCluster
      else if ( !split && !split_container )
        cluster = cl; // return mainCluster
    }
    return cluster;
  }
  // unregistered caloHypo -> return the smallest/largest cluster according to getsplit
  auto smallest = []( const LHCb::CaloCluster* lhs, const LHCb::CaloCluster* rhs ) {
    return lhs->entries().size() < rhs->entries().size();
  };
  auto largest = []( const LHCb::CaloCluster* lhs, const LHCb::CaloCluster* rhs ) {
    return lhs->entries().size() > rhs->entries().size();
  };
  auto i = std::min_element( clusters.begin(), clusters.end(),
                             split ? smallest : largest ); // only works because non-capturing lambda can be converted
                                                           // to pointer to function
  return i != clusters.end() ? i->target() : nullptr;
}

bool LHCb::CaloFutureAlgUtils::StringMatcher( const std::vector<std::string>& refs, const std::string& name ) {

  bool ok = false;
  bool ko = true;

  if ( refs.empty() ) return true;
  for ( const auto& ref : refs ) {
    if ( toUpper( ref ) == "ALL" ) ok = true;
    if ( toUpper( ref ) == "NONE" ) return false;
    bool match = StringMatcher( ref, name );
    if ( contains_ci( ref, "!" ) && !match ) ko = false;
    if ( !contains_ci( ref, "!" ) && match ) ok = true;
  }
  if ( !ko ) return false; // rejection wins
  return ok;
}

bool LHCb::CaloFutureAlgUtils::StringMatcher( const std::string& ref, const std::string& name ) {
  std::string uref = toUpper( ref );
  if ( uref == "ALL" ) return true;
  if ( uref == "NONE" ) return false;
  bool rev = false;
  if ( contains_ci( uref, "!" ) ) {
    rev     = true;
    int ind = uref.find_first_of( "!" );
    uref    = uref.erase( ind, 1 );
  }
  bool exact = true;
  if ( contains_ci( uref, "*" ) ) {
    exact = false;
    if ( uref.find_first_of( "*" ) != std::string::npos ) {
      int i1 = uref.find_first_of( "*" );
      uref.erase( i1, 1 );
    }
    if ( uref.find_last_of( "*" ) != std::string::npos ) {
      int i2 = uref.find_last_of( "*" );
      uref.erase( i2, 1 );
    }
  }
  std::string uname = toUpper( name );
  bool        match = ( exact ? uref == uname : contains_ci( uname, uref ) );
  return rev ? !match : match;
}
