// local
#include "HltDAQ/ReportConvertTool.h"

using LHCb::HltObjectSummary;
using LHCb::HltSelRepRBStdInfo;

namespace {

  template <typename C>
  unsigned int count_info(const C& c) {
    return std::count_if( begin(c), end(c),
                          [](typename C::const_reference elem) {
                          return elem.first.find("#")!=std::string::npos;
  } ); }

  static const std::array<LHCb::RecSummary::DataTypes,23> s_rsum_map = {
       LHCb::RecSummary::nLongTracks,
       LHCb::RecSummary::nDownstreamTracks,
       LHCb::RecSummary::nUpstreamTracks,
       LHCb::RecSummary::nVeloTracks,
       LHCb::RecSummary::nTTracks,
       LHCb::RecSummary::nBackTracks,
       LHCb::RecSummary::nTracks,
       LHCb::RecSummary::nRich1Hits,
       LHCb::RecSummary::nRich2Hits,
       LHCb::RecSummary::nVeloClusters,
       LHCb::RecSummary::nITClusters,
       LHCb::RecSummary::nTTClusters,
       LHCb::RecSummary::nUTClusters,
       LHCb::RecSummary::nOTClusters,
       LHCb::RecSummary::nFTClusters,
       LHCb::RecSummary::nSPDhits,
       LHCb::RecSummary::nMuonCoordsS0,
       LHCb::RecSummary::nMuonCoordsS1,
       LHCb::RecSummary::nMuonCoordsS2,
       LHCb::RecSummary::nMuonCoordsS3,
       LHCb::RecSummary::nMuonCoordsS4,
       LHCb::RecSummary::nMuonTracks,
       LHCb::RecSummary::nPVs
  };
  static const std::array<LHCb::ProtoParticle::additionalInfo,67> s_proto_map = {
      LHCb::ProtoParticle::IsPhoton,//381
      LHCb::ProtoParticle::IsNotE,//382
      LHCb::ProtoParticle::IsNotH,//383
      LHCb::ProtoParticle::EcalPIDe,//360
      LHCb::ProtoParticle::PrsPIDe,//361
      LHCb::ProtoParticle::BremPIDe,//362
      LHCb::ProtoParticle::HcalPIDe,//363
      LHCb::ProtoParticle::HcalPIDmu,//364
      LHCb::ProtoParticle::EcalPIDmu,//365
      LHCb::ProtoParticle::CaloTrMatch,//310
      LHCb::ProtoParticle::CaloElectronMatch,//311
      LHCb::ProtoParticle::CaloBremMatch,//312
      LHCb::ProtoParticle::CaloNeutralSpd,//323
      LHCb::ProtoParticle::CaloNeutralPrs,//324
      LHCb::ProtoParticle::CaloNeutralEcal,//325
      LHCb::ProtoParticle::CaloNeutralHcal2Ecal,//326
      LHCb::ProtoParticle::CaloNeutralE49,//327
      LHCb::ProtoParticle::CaloNeutralID,//328
      LHCb::ProtoParticle::CaloDepositID,//320
      LHCb::ProtoParticle::ShowerShape,//321
      LHCb::ProtoParticle::ClusterMass,//322
      LHCb::ProtoParticle::CaloSpdE,//330
      LHCb::ProtoParticle::CaloPrsE,//331
      LHCb::ProtoParticle::CaloEcalE,//332
      LHCb::ProtoParticle::CaloHcalE,//333
      LHCb::ProtoParticle::CaloEcalChi2,//334
      LHCb::ProtoParticle::CaloBremChi2,//335
      LHCb::ProtoParticle::CaloClusChi2,//336
      LHCb::ProtoParticle::CaloNeutralPrsM,//343
      LHCb::ProtoParticle::CaloShapeFr2r4,//344
      LHCb::ProtoParticle::CaloShapeKappa,//345
      LHCb::ProtoParticle::CaloShapeAsym,//346
      LHCb::ProtoParticle::CaloShapeE1,//347
      LHCb::ProtoParticle::CaloShapeE2,//348
      LHCb::ProtoParticle::CaloPrsShapeE2,//349
      LHCb::ProtoParticle::CaloPrsShapeEmax,//350
      LHCb::ProtoParticle::CaloPrsShapeFr2,//351
      LHCb::ProtoParticle::CaloPrsShapeAsym,//352
      LHCb::ProtoParticle::CaloPrsM,//353
      LHCb::ProtoParticle::CaloPrsM15,//354
      LHCb::ProtoParticle::CaloPrsM30,//355
      LHCb::ProtoParticle::CaloPrsM45,//356
      LHCb::ProtoParticle::CaloClusterCode,//357
      LHCb::ProtoParticle::CaloClusterFrac,//358
      LHCb::ProtoParticle::CombDLLe,//600
      LHCb::ProtoParticle::CombDLLmu,//601
      LHCb::ProtoParticle::CombDLLpi,//602
      LHCb::ProtoParticle::CombDLLk,//603
      LHCb::ProtoParticle::CombDLLp,//604
      LHCb::ProtoParticle::InAccBrem,// Extra for PID group
      LHCb::ProtoParticle::InAccSpd,// ''
      LHCb::ProtoParticle::InAccPrs,// ''
      LHCb::ProtoParticle::InAccEcal,// ''
      LHCb::ProtoParticle::InAccHcal,// ''
      LHCb::ProtoParticle::VeloCharge,// ''
      LHCb::ProtoParticle::RichPIDStatus,// ''
      LHCb::ProtoParticle::CaloChargedID,//
      LHCb::ProtoParticle::CaloChargedEcal,//
      LHCb::ProtoParticle::CaloChargedPrs,//
      LHCb::ProtoParticle::CaloChargedSpd,//
      LHCb::ProtoParticle::ProbNNe,//
      LHCb::ProtoParticle::ProbNNmu,//
      LHCb::ProtoParticle::ProbNNpi,//
      LHCb::ProtoParticle::ProbNNk,//
      LHCb::ProtoParticle::ProbNNp,//
      LHCb::ProtoParticle::ProbNNghost, //
      LHCb::ProtoParticle::CombDLLd//605
  };
}

//-----------------------------------------------------------------------------
// Implementation file for class : ReportConvertTool: ReportConvertTool.cpp
// Author: Sean Benson
// 05/06/2008
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_TOOL_FACTORY( ReportConvertTool )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
ReportConvertTool::ReportConvertTool( const std::string& type,
                                const std::string& name,
                                const IInterface* parent )
    : GaudiTool( type, name , parent )
{
  declareInterface<IReportConvert>(this);

  m_LatestVersion=1;
  for (const auto& map : { m_track_unordered_map2_Turbo,
                           m_particle_unordered_map2_Turbo,
                           m_proto_unordered_map2_Turbo,
                           m_rpid_unordered_map2_Turbo,
                           m_mpid_unordered_map2_Turbo,
                           m_recvertex_unordered_map2_Turbo,
                           m_vertex_unordered_map2_Turbo,
                           m_recsummary_unordered_map2,
                           m_calohypo_unordered_map2_Turbo } ) {
    for(const auto & elem : map) {
        if(elem.first > m_LatestVersion) m_LatestVersion = elem.first;
    }
  }
}

float ReportConvertTool::floatFromInt(unsigned int i) {
  union IntFloat { unsigned int mInt; float mFloat; };
  IntFloat a; a.mInt=i;
  return a.mFloat;
}

void ReportConvertTool::setReportVersion(int version){
  m_version = version;
}

void ReportConvertTool::setReportVersionLatest(){
  m_version = m_LatestVersion;
}

int ReportConvertTool::getReportVersion(){
  return m_version;
}

int ReportConvertTool::getLatestVersion(){
  return m_LatestVersion;
}

int ReportConvertTool::getSizeSelRepParticleLatest(){
  return m_particle_unordered_map2.at(findBestPrevious(m_particle_unordered_map2,m_LatestVersion)).size();
}

int ReportConvertTool::findBestPrevious(const unordered_map<int, unordered_map<string,pair<int,int> > >& map,int in) const{
  int out=in;
  for(int m=in;m>0;--m){
    auto it = map.find(m);
    out = m;
    if (it != map.end()) break;
  }
  return out;
}

void ReportConvertTool::SummaryFromRaw(HltObjectSummary::Info* info, HltSelRepRBStdInfo::StdInfo* subbank, int classID) {

  // Version number that we use to ensure backwards compatibility with Run 1
  // (has nothing to do with header ID).
  int run1version=-999;

  debug() << "SummaryFromRaw call for ID:" << classID << endmsg;

  // Make sure the tool is given a version to use
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  debug() << "Assigning used_map with version:" << m_version << endmsg;

  // Which one of the member maps do we need to use?
  /*
   * Word on versions:
   * - Version 3 and greater for Run 2
   * - Versions 1 and 2 for Run 1
   * v3 is the first version in the header ID to have turbo information.
   * v1 and v2 in the header ID have nothing to do with level of information in the reports.
   * If (v1 || v2), Track, Particle, RecVertex and CaloCluster need to look for the level of information.
   * - Need to protect against someone with v1 or v2 asking for a Turbo map.
   * - Need to protect against v1 or v2 requesting a Turbo object decode (i.e. RichPID, etc.)
   */

  if((m_version<3 && m_version>0) && ( (classID!=LHCb::CLID_Track) && (classID!=LHCb::CLID_Particle) && (classID!=LHCb::CLID_RecVertex) && (classID!=LHCb::CLID_CaloCluster) ) )
      Error( std::string{ "Asked to decode Run 1 style reports with Turbo object (" } + std::to_string(classID) + std::string{ "). Please update your software" }, StatusCode::FAILURE, 100 );

  auto pick_map = [&](const auto& map1, const auto& map2 ) {
        return  subbank->size()< map1.at( findBestPrevious( map1, m_version ) ).size() ?
                 &map2 : &map1;
  };
  const unordered_map<int,unordered_map<string, pair<int,int> > > *used_map = nullptr;
  switch( classID )
  {
    case LHCb::CLID_Track:
      {
        if(m_version<3) {
          // Looking at Run 1 data, need to know which map to use
          if( subbank->size() == (m_track_unordered_map2.at(1)).size() ) run1version = 1;
          else if( subbank->size() == (m_track_unordered_map2.at(2)).size() ) run1version = 2;
          else Error( "Track requested in Run 1 reports, but reports are unknown" , StatusCode::FAILURE, 100 ).ignore();
          used_map = &m_track_unordered_map2;
          m_version=run1version;
        } else {
          used_map = pick_map( m_track_unordered_map2_Turbo, m_track_unordered_map2_Turbo );
        }
      }
      break;
    case LHCb::CLID_RecVertex:
      {
        if(m_version<3) {
          // Looking at Run 1 data, need to know which map to use
          if( subbank->size() == (m_recvertex_unordered_map2.at(1)).size() ) run1version = 1;
          else if( subbank->size() == (m_recvertex_unordered_map2.at(2)).size() ) run1version = 2;
          used_map = &m_recvertex_unordered_map2;
          m_version=run1version;
        } else {
          used_map = pick_map( m_recvertex_unordered_map2_Turbo,m_recvertex_unordered_map2) ;
        }
      }
      break;
    case LHCb::CLID_Vertex:
      used_map = pick_map( m_vertex_unordered_map2_Turbo, m_vertex_unordered_map2);
      break;
    case LHCb::CLID_Particle:
      {
        if(m_version<3) {
          // Looking at Run 1 data, need to know which map to use
          if( subbank->size() == (m_particle_unordered_map2.at(1)).size() ) run1version = 1;
          else if( subbank->size() == (m_particle_unordered_map2.at(2)).size() ) run1version = 2;
          used_map = &m_particle_unordered_map2;
          m_version=run1version;
        } else {
          used_map = pick_map( m_particle_unordered_map2_Turbo, m_particle_unordered_map2 );
        }
      }
      break;
    case LHCb::CLID_ProtoParticle:
      used_map = pick_map(m_proto_unordered_map2_Turbo, m_proto_unordered_map2);
      break;
    case LHCb::CLID_RichPID:
      used_map = pick_map(m_rpid_unordered_map2_Turbo, m_rpid_unordered_map2);
      break;
    case LHCb::CLID_CaloHypo:
      used_map = pick_map(m_calohypo_unordered_map2_Turbo, m_calohypo_unordered_map2);
      break;
    case LHCb::CLID_MuonPID:
      used_map = pick_map(m_mpid_unordered_map2_Turbo,  m_mpid_unordered_map2);
      break;
    case LHCb::CLID_RecSummary:
      {
        used_map = &m_recsummary_unordered_map2;
      }
      break;
    case 40: // This is a special number to deal with the holder of related info, contains only the location enum
      {
        info->insert( "0#LocationID", floatFromInt( (*subbank)[ 0 ]));
        return;
      }
      break;
    case 41: // This is a special number to deal with the actual related info
      {
        unsigned int n=0;
        do {
          // First make map key
          std::stringstream key;
          int len = snprintf(nullptr, 0, "%d", n);
          int padding = 4 - len;
          for(int m=0; m<padding; m++) key << "0";
          key << n << "#GenericKey";
          info->insert(key.str().c_str(),floatFromInt( (*subbank)[ n ]));
          n++;

          // Then do the same for the value
          std::stringstream keykey;
          int len2 = snprintf(nullptr, 0, "%d", n);
          int padding2 = 4 - len2;
          for(int m=0; m<padding2; m++) keykey << "0";
          keykey << n << "#GenericValue";
          info->insert(keykey.str().c_str(),floatFromInt( (*subbank)[ n ]));
          n++;
        }while(n<subbank->size());
        return;
      }
      break;
    case LHCb::CLID_CaloCluster:
      if(m_version<3) {
        // Looking at Run 1 data, need to know which map to use
        if( subbank->size() == (m_calo_unordered_map2.at(1)).size() ) run1version = 1;
        else if( subbank->size() == (m_calo_unordered_map2.at(2)).size() ) run1version = 2;
        used_map = &m_calo_unordered_map2;
        m_version=run1version;
      } else {
        used_map = pick_map(m_calo_unordered_map2_Turbo, m_calo_unordered_map2);
      }
      break;
      // NOTE THE CASE OF 1 IS TAKEN CARE OF INSIDE THE DECODER
    default:
      {

        Warning( std::string{ " StdInfo on unsupported class type "}+ std::to_string(classID),
                  StatusCode::SUCCESS, 20 );
        int e = 0;
        for (const auto& i : (*subbank)) {
           info->insert( std::string { "z#Unknown.unknown" } + std::to_string( e++ ),
                                  floatFromInt(i) );
        }
        return;
      }
    }

  // If version<3 => for Track, Particle, RecVertex, and CaloCluster, need to assign version based on size

  debug() << "Inserting from bank" << endmsg;
  for(const auto& object : used_map->at( findBestPrevious( *used_map, m_version ) ))  {
    info->insert( object.first, floatFromInt( (*subbank)[ object.second.first ]) );
  }
}

// Put the information in to the HltObjectSummary
void ReportConvertTool::ParticleObject2Summary( HltObjectSummary::Info* info, const LHCb::Particle* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map
    = ( turbo ? m_particle_unordered_map2_Turbo :m_particle_unordered_map2 );

  for(const auto&  particle: used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( particle.second.second )
    {
      case 0: info->insert( particle.first, float( object->particleID().pid() ) ); break;
      case 1: info->insert( particle.first, float( object->measuredMass() ) ); break;
      case 2: info->insert( particle.first, float( object->referencePoint().z() ) ); break;
      case 3: info->insert( particle.first, float( object->referencePoint().x() ) ); break;
      case 4: info->insert( particle.first, float( object->referencePoint().y() ) ); break;
      case 5: info->insert( particle.first, float( object->slopes().x() ) ); break;
      case 6: info->insert( particle.first, float( object->slopes().y() ) ); break;
      case 7: info->insert( particle.first, float( 1.0/(object->p()) ) ); break;
      case 8: info->insert( particle.first, float( object->confLevel() ) ); break;
      case 9: info->insert( particle.first, float( object->measuredMassErr() ) ); break;
      case 10: info->insert( particle.first, float( object->momCovMatrix()(0,0) ) ); break;
      case 11: info->insert( particle.first, float( object->momCovMatrix()(1,1) ) ); break;
      case 12: info->insert( particle.first, float( object->momCovMatrix()(2,2) ) ); break;
      case 13: info->insert( particle.first, float( object->momCovMatrix()(3,3) ) ); break;
      case 14: info->insert( particle.first, float( object->momCovMatrix()(1,0) ) ); break;
      case 15: info->insert( particle.first, float( object->momCovMatrix()(2,0) ) ); break;
      case 16: info->insert( particle.first, float( object->momCovMatrix()(2,1) ) ); break;
      case 17: info->insert( particle.first, float( object->momCovMatrix()(3,0) ) ); break;
      case 18: info->insert( particle.first, float( object->momCovMatrix()(3,1) ) ); break;
      case 19: info->insert( particle.first, float( object->momCovMatrix()(3,2) ) ); break;
      case 20: info->insert( particle.first, float( object->posMomCovMatrix()(0,0) ) ); break;
      case 21: info->insert( particle.first, float( object->posMomCovMatrix()(1,1) ) ); break;
      case 22: info->insert( particle.first, float( object->posMomCovMatrix()(2,2) ) ); break;
      case 23: info->insert( particle.first, float( object->posMomCovMatrix()(1,0) ) ); break;
      case 24: info->insert( particle.first, float( object->posMomCovMatrix()(0,1) ) ); break;
      case 25: info->insert( particle.first, float( object->posMomCovMatrix()(2,0) ) ); break;
      case 26: info->insert( particle.first, float( object->posMomCovMatrix()(0,2) ) ); break;
      case 27: info->insert( particle.first, float( object->posMomCovMatrix()(2,1) ) ); break;
      case 28: info->insert( particle.first, float( object->posMomCovMatrix()(1,2) ) ); break;
      case 29: info->insert( particle.first, float( object->posMomCovMatrix()(3,0) ) ); break;
      case 30: info->insert( particle.first, float( object->posMomCovMatrix()(3,1) ) ); break;
      case 31: info->insert( particle.first, float( object->posMomCovMatrix()(3,2) ) ); break;
      case 32: info->insert( particle.first, float( object->posCovMatrix()(0,0) ) ); break;
      case 33: info->insert( particle.first, float( object->posCovMatrix()(1,1) ) ); break;
      case 34: info->insert( particle.first, float( object->posCovMatrix()(2,2) ) ); break;
      case 35: info->insert( particle.first, float( object->posCovMatrix()(1,0) ) ); break;
      case 36: info->insert( particle.first, float( object->posCovMatrix()(2,0) ) ); break;
      case 37: info->insert( particle.first, float( object->posCovMatrix()(2,1) ) ); break;
      case 38: info->insert( particle.first, float( object->momentum().M() ) ); break;
      case 39: info->insert( particle.first, float( object->momentum().px() ) ); break;
      case 40: info->insert( particle.first, float( object->momentum().py() ) ); break;
      case 41: info->insert( particle.first, float( object->momentum().pz() ) ); break;
    }
  }

}

void ReportConvertTool::ProtoParticleObject2Summary( HltObjectSummary::Info* info, const LHCb::ProtoParticle* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = ( turbo ? m_proto_unordered_map2_Turbo : m_proto_unordered_map2 );

  for(const auto& proto : used_map.at( findBestPrevious( used_map, m_version ) ) ) {
    assert(proto.second.second < static_cast<int>(s_proto_map.size()));
    info->insert( proto.first, float( object->info( s_proto_map[proto.second.second], -1000 ) ) );
  }

}

void ReportConvertTool::TrackObject2Summary( HltObjectSummary::Info* info, const LHCb::Track* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_track_unordered_map2_Turbo
                                : m_track_unordered_map2 );

  LHCb::State first, last;
  if( object->type() == LHCb::Track::Types::Long ){
    if( object->hasStateAt(LHCb::State::ClosestToBeam) ) first = *(object->stateAt(LHCb::State::ClosestToBeam));
    else first = *(object->states().front());
    //
    if( object->hasStateAt(LHCb::State::BegRich2) ) last = *(object->stateAt(LHCb::State::BegRich2));
    else last = *(object->states().back());
  }
  else if( object->type() == LHCb::Track::Types::Downstream ){
    if( object->hasStateAt(LHCb::State::FirstMeasurement) ) first = *(object->stateAt(LHCb::State::FirstMeasurement));
    else first = *(object->states().front());
    //
    if( object->hasStateAt(LHCb::State::BegRich2) ) last = *(object->stateAt(LHCb::State::BegRich2));
    else last = *(object->states().back());
  }
  else{
    first = *(object->states().front());
    last = *(object->states().back());
  }

  for(const auto& track : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( track.second.second )
    {
      case 0: info->insert( track.first, float( first.z() ) ); break;
      case 1: info->insert( track.first, float( first.x() ) ); break;
      case 2: info->insert( track.first, float( first.y() ) ); break;
      case 3: info->insert( track.first, float( first.tx() ) ); break;
      case 4: info->insert( track.first, float( first.ty() ) ); break;
      case 5: info->insert( track.first, float( first.qOverP() ) ); break;
      case 6: info->insert( track.first, float( object->chi2PerDoF() ) ); break;
      case 7: info->insert( track.first, float( object->nDoF() ) ); break;
      case 8: info->insert( track.first, float( object->likelihood() ) ); break;
      case 9: info->insert( track.first, float( object->ghostProbability() ) ); break;
      case 10: info->insert( track.first, float( object->flags() ) ); break;
      case 11: info->insert( track.first, float( last.z() ) ); break;
      case 12: info->insert( track.first, float( last.x() ) ); break;
      case 13: info->insert( track.first, float( last.y() ) ); break;
      case 14: info->insert( track.first, float( last.tx() ) ); break;
      case 15: info->insert( track.first, float( last.ty() ) ); break;
      case 16: info->insert( track.first, float( last.qOverP() ) ); break;
      case 17: info->insert( track.first, float( object->info( LHCb::Track::CloneDist, -1000) ) ); break;
      case 18: info->insert( track.first, float( object->info( LHCb::Track::FitMatchChi2, -1000) ) ); break;
      case 19: info->insert( track.first, float( object->info( LHCb::Track::FitVeloChi2, -1000) ) ); break;
      case 20: info->insert( track.first, float( object->info( LHCb::Track::FitTChi2, -1000) ) ); break;
      case 21: info->insert( track.first, float( object->info( LHCb::Track::FitVeloNDoF, -1000) ) ); break;
      case 22: info->insert( track.first, float( object->info( LHCb::Track::FitTNDoF, -1000) ) ); break;
      case 23: info->insert( track.first, float( first.flags() ) ); break;
      case 24: info->insert( track.first, float( last.flags() ) ); break;
      case 25: info->insert( track.first, float( first.covariance()(0,0) ) ); break;
      case 26: info->insert( track.first, float( first.covariance()(1,1) ) ); break;
      case 27: info->insert( track.first, float( first.covariance()(2,2) ) ); break;
      case 28: info->insert( track.first, float( first.covariance()(3,3) ) ); break;
      case 29: info->insert( track.first, float( first.covariance()(4,4) ) ); break;
      case 30: info->insert( track.first, float( first.covariance()(0,1) ) ); break;
      case 31: info->insert( track.first, float( first.covariance()(0,2) ) ); break;
      case 32: info->insert( track.first, float( first.covariance()(0,3) ) ); break;
      case 33: info->insert( track.first, float( first.covariance()(0,4) ) ); break;
      case 34: info->insert( track.first, float( first.covariance()(1,2) ) ); break;
      case 35: info->insert( track.first, float( first.covariance()(1,3) ) ); break;
      case 36: info->insert( track.first, float( first.covariance()(1,4) ) ); break;
      case 37: info->insert( track.first, float( first.covariance()(2,3) ) ); break;
      case 38: info->insert( track.first, float( first.covariance()(2,4) ) ); break;
      case 39: info->insert( track.first, float( first.covariance()(3,4) ) ); break;
    }
  }
}

void ReportConvertTool::RichPIDObject2Summary( HltObjectSummary::Info* info, const LHCb::RichPID* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = ( turbo ? m_rpid_unordered_map2_Turbo
                                 : m_rpid_unordered_map2 );

  for(const auto& rpid : used_map.at( findBestPrevious( used_map, m_version ) ) ) {
    switch( rpid.second.second )
    {
      case 0: info->insert( rpid.first, float( object->pidResultCode() ) ); break;
      case 1: info->insert( rpid.first, float( object->particleDeltaLL( Rich::ParticleIDType::Electron ) ) ); break;
      case 2: info->insert( rpid.first, float( object->particleDeltaLL( Rich::ParticleIDType::Muon ) ) ); break;
      case 3: info->insert( rpid.first, float( object->particleDeltaLL( Rich::ParticleIDType::Pion ) ) ); break;
      case 4: info->insert( rpid.first, float( object->particleDeltaLL( Rich::ParticleIDType::Kaon ) ) ); break;
      case 5: info->insert( rpid.first, float( object->particleDeltaLL( Rich::ParticleIDType::Proton ) ) ); break;
      case 6: info->insert( rpid.first, float( object->particleDeltaLL( Rich::ParticleIDType::BelowThreshold ) ) ); break;
      case 7: info->insert( rpid.first, float( object->particleDeltaLL( Rich::ParticleIDType::Deuteron ) ) ); break;
    }
  }

}

void ReportConvertTool::MuonPIDObject2Summary( HltObjectSummary::Info* info , const LHCb::MuonPID* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_mpid_unordered_map2_Turbo
                                : m_mpid_unordered_map2 );
  for(const auto& mpid : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( mpid.second.second )
    {
      case 0: info->insert( mpid.first, float( object->MuonLLMu() ) ); break;
      case 1: info->insert( mpid.first, float( object->MuonLLBg() ) ); break;
      case 2: info->insert( mpid.first, float( object->nShared() ) ); break;
      case 3: info->insert( mpid.first, float( object->Status() ) ); break;
      case 4: info->insert( mpid.first, float( object->IsMuon() ) ); break;
      case 5: info->insert( mpid.first, float( object->IsMuonLoose() ) ); break;
      case 6: info->insert( mpid.first, float( object->IsMuonTight() ) ); break;
    }
  }

}

void ReportConvertTool::CaloClusterObject2Summary( HltObjectSummary::Info* info, const LHCb::CaloCluster* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_calo_unordered_map2_Turbo
                                : m_calo_unordered_map2 );
  for(const auto& calo : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( calo.second.second )
    {
      case 0: info->insert( calo.first, float( object->e() ) ); break;
      case 1: info->insert( calo.first, float( object->position().x() ) ); break;
      case 2: info->insert( calo.first, float( object->position().y() ) ); break;
      case 3: info->insert( calo.first, float( object->position().z() ) ); break;
    }
  }
}

void ReportConvertTool::CaloHypoObject2Summary( HltObjectSummary::Info* info, const LHCb::CaloHypo* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_calohypo_unordered_map2_Turbo
                                : m_calohypo_unordered_map2 );
  for(const auto& calo : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( calo.second.second )
    {
      case 0: info->insert( calo.first, float( object->e() ) ); break;
      case 1: info->insert( calo.first, float( (object->position() ? object->position()->x() : -1.0) ) ); break;
      case 2: info->insert( calo.first, float( (object->position() ? object->position()->y() : -1.0) ) ); break;
      case 3: info->insert( calo.first, float( (object->position() ? object->position()->z() : -1.0) ) ); break;
      case 4: info->insert( calo.first, float( object->lh() ) ); break;
      case 5: info->insert( calo.first, float( object->hypothesis() ) ); break;
    }
  }
}

void ReportConvertTool::RecVertexObject2Summary( HltObjectSummary::Info* info, const LHCb::RecVertex* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_recvertex_unordered_map2_Turbo
                                : m_recvertex_unordered_map2 );
  for(const auto&  recvertex : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( recvertex.second.second )
    {
      case 0: info->insert( recvertex.first, float( object->position().x() ) ); break;
      case 1: info->insert( recvertex.first, float( object->position().y() ) ); break;
      case 2: info->insert( recvertex.first, float( object->position().z() ) ); break;
      case 3: info->insert( recvertex.first, float( object->chi2() ) ); break;
      case 4: info->insert( recvertex.first, float( object->nDoF() ) ); break;
      case 5: info->insert( recvertex.first, float( object->technique() ) ); break;
      case 6: info->insert( recvertex.first, float( object->covMatrix()(0,0) ) ); break;
      case 7: info->insert( recvertex.first, float( object->covMatrix()(1,1) ) ); break;
      case 8: info->insert( recvertex.first, float( object->covMatrix()(2,2) ) ); break;
      case 9: info->insert( recvertex.first, float( object->covMatrix()(1,0) ) ); break;
      case 10: info->insert( recvertex.first, float( object->covMatrix()(2,0) ) ); break;
      case 11: info->insert( recvertex.first, float( object->covMatrix()(2,1) ) ); break;
    }
  }

}

void ReportConvertTool::VertexObject2Summary( HltObjectSummary::Info* info, const LHCb::Vertex* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_vertex_unordered_map2_Turbo
                                : m_vertex_unordered_map2 );
  for(const auto&  vertex : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( vertex.second.second )
    {
      case 0: info->insert( vertex.first, float( object->chi2() ) ); break;
      case 1: info->insert( vertex.first, float( object->nDoF() ) ); break;
      case 2: info->insert( vertex.first, float( object->position().x() ) ); break;
      case 3: info->insert( vertex.first, float( object->position().y() ) ); break;
      case 4: info->insert( vertex.first, float( object->position().z() ) ); break;
      case 5: info->insert( vertex.first, float( object->technique() ) ); break;
      case 6: info->insert( vertex.first, float( object->covMatrix()(0,0) ) ); break;
      case 7: info->insert( vertex.first, float( object->covMatrix()(1,1) ) ); break;
      case 8: info->insert( vertex.first, float( object->covMatrix()(2,2) ) ); break;
      case 9: info->insert( vertex.first, float( object->covMatrix()(1,0) ) ); break;
      case 10: info->insert( vertex.first, float( object->covMatrix()(2,0) ) ); break;
      case 11: info->insert( vertex.first, float( object->covMatrix()(2,1) ) ); break;
    }
  }

}

void ReportConvertTool::RecSummaryObject2Summary( HltObjectSummary::Info* info, const LHCb::RecSummary* object ) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = m_recsummary_unordered_map2;
  for(const auto& recsummary : used_map.at( findBestPrevious( used_map, m_version ) )) {
    assert( recsummary.second.second < static_cast<int>(s_rsum_map.size()));
    info->insert( recsummary.first, float( object->info( s_rsum_map[recsummary.second.second],0 ) ) );
  }

}

// Convert generic Gaudi::VectorMap into a summary
void ReportConvertTool::GenericMapObject2Summary( HltObjectSummary::Info* info , const GaudiUtils::VectorMap<short,float>* map) {
  int n=0;
  for( auto i : *map ){
    // First put the map key inside
    std::stringstream key;
    int len = snprintf(nullptr, 0, "%d", n);
    int padding = 4 - len;
    for(int m=0; m<padding; m++) key << "0";
    key << n << "#GenericKey";
    n++;
    info->insert( key.str().c_str() , float( i.first ) );

    // Then do the same for the value
    std::stringstream keykey;
    int len2 = snprintf(nullptr, 0, "%d", n);
    int padding2 = 4 - len2;
    for(int m=0; m<padding2; m++) keykey << "0";
    keykey << n << "#GenericValue";
    n++;
    info->insert( keykey.str().c_str() , float( i.second ) );
  }
}

// Put the information in the summary back in the object
void ReportConvertTool::ParticleObjectFromSummary( const HltObjectSummary::Info* info, LHCb::Particle* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_particle_unordered_map2_Turbo
                                : m_particle_unordered_map2 );
  if( m_version < 3 ){
    int run1version=-999;
    // find size we care about (i.e. make sure extra info not counted)
    auto Isize = count_info(*info);
    // Looking at Run 1 data, need to know which map to use
    if( Isize == (m_particle_unordered_map2.at(1)).size() ) run1version = 1;
    else if( Isize == (m_particle_unordered_map2.at(2)).size() ) run1version = 2;
    else Error( "Particle requested in Run 1 reports, but reports are unknown" , StatusCode::FAILURE, 100 );
    m_version=run1version;
  }

  // Raw
  double raw_m {0.0}, raw_p1 {0.0}, raw_p2 {0.0}, raw_p3 {0.0};

  // Momentum components
  float p=0;
  float slopex=0;
  float slopey=0;
  // Reference point
  Gaudi::XYZPoint xyz;
  // Matrices
  Gaudi::SymMatrix4x4 & momCov = *(const_cast<Gaudi::SymMatrix4x4*>(&object->momCovMatrix()));
  Gaudi::Matrix4x3 & posMomCov = *(const_cast<Gaudi::Matrix4x3*>(&object->posMomCovMatrix()));
  Gaudi::SymMatrix3x3 & posCov = *(const_cast<Gaudi::SymMatrix3x3*>(&object->posCovMatrix()));

  for(const auto& particle : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( std::get<1>( particle.second ) )
    {
      case 0: object->setParticleID( LHCb::ParticleID( (*info)[ particle.first ] ) ); break;
      case 1: object->setMeasuredMass( (*info)[ particle.first ] ); break;
      case 2: xyz.SetZ( (*info)[ particle.first ] ); break;
      case 3: xyz.SetX( (*info)[ particle.first ] );  break;
      case 4: xyz.SetY( (*info)[ particle.first ] ); break;
      case 5: slopex = (*info)[ particle.first ] ; break;
      case 6: slopey = (*info)[ particle.first ] ; break;
      case 7: p=1.0/( (*info)[ particle.first ] ); break;
      case 8: object->setConfLevel( (*info)[ particle.first ] ); break;
      case 9: object->setMeasuredMassErr( (*info)[ particle.first ] ); break;
      case 10: momCov(0,0) = (*info)[ particle.first ] ; break;
      case 11: momCov(1,1) = (*info)[ particle.first ] ; break;
      case 12: momCov(2,2) = (*info)[ particle.first ] ; break;
      case 13: momCov(3,3) = (*info)[ particle.first ] ; break;
      case 14: momCov(1,0) = (*info)[ particle.first ] ; break;
      case 15: momCov(2,0) = (*info)[ particle.first ] ; break;
      case 16: momCov(2,1) = (*info)[ particle.first ] ; break;
      case 17: momCov(3,0) = (*info)[ particle.first ] ; break;
      case 18: momCov(3,1) = (*info)[ particle.first ] ; break;
      case 19: momCov(3,2) = (*info)[ particle.first ] ; break;
      case 20: posMomCov(0,0) = (*info)[ particle.first ] ; break;
      case 21: posMomCov(1,1) = (*info)[ particle.first ] ; break;
      case 22: posMomCov(2,2) = (*info)[ particle.first ] ; break;
      case 23: posMomCov(1,0) = (*info)[ particle.first ] ; break;
      case 24: posMomCov(0,1) = (*info)[ particle.first ] ; break;
      case 25: posMomCov(2,0) = (*info)[ particle.first ] ; break;
      case 26: posMomCov(0,2) = (*info)[ particle.first ] ; break;
      case 27: posMomCov(2,1) = (*info)[ particle.first ] ; break;
      case 28: posMomCov(1,2) = (*info)[ particle.first ] ; break;
      case 29: posMomCov(3,0) = (*info)[ particle.first ] ; break;
      case 30: posMomCov(3,1) = (*info)[ particle.first ] ; break;
      case 31: posMomCov(3,2) = (*info)[ particle.first ] ; break;
      case 32: posCov(0,0) = (*info)[ particle.first ] ; break;
      case 33: posCov(1,1) = (*info)[ particle.first ] ; break;
      case 34: posCov(2,2) = (*info)[ particle.first ] ; break;
      case 35: posCov(1,0) = (*info)[ particle.first ] ; break;
      case 36: posCov(2,0) = (*info)[ particle.first ] ; break;
      case 37: posCov(2,1) = (*info)[ particle.first ] ; break;
      case 38: raw_m = (*info)[ particle.first ] ; break;
      case 39: raw_p1 = (*info)[ particle.first ] ; break;
      case 40: raw_p2 = (*info)[ particle.first ] ; break;
      case 41: raw_p3 = (*info)[ particle.first ] ; break;
    }
  }
  object->setReferencePoint(xyz);

  // Old way with slopes
  const double slopez = 1/sqrt(slopex*slopex + slopey*slopey + 1.0);
  const double pz = slopez*p;
  const double px = slopex*pz;
  const double py = slopey*pz;
  const double mm = object->measuredMass();
  const double pe = sqrt(p*p+mm*mm);
  Gaudi::LorentzVector part_mom(px,py,pz,pe);
  //

  double psq = raw_p1*raw_p1 + raw_p2*raw_p2 + raw_p3*raw_p3;
  double raw_pe = sqrt(psq+raw_m*raw_m);
  Gaudi::LorentzVector part_mom_raw(raw_p1,raw_p2,raw_p3,raw_pe);
  if(raw_p3>0.001) object->setMomentum(part_mom_raw);
  else object->setMomentum(part_mom);
}

void ReportConvertTool::ProtoParticleObjectFromSummary( const HltObjectSummary::Info* info, LHCb::ProtoParticle* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_proto_unordered_map2_Turbo
                                : m_proto_unordered_map2 );
  for(const auto& proto : used_map.at( findBestPrevious( used_map, m_version ) )) {
    assert(proto.second.second<static_cast<int>(s_proto_map.size()));
    auto i = (*info)[proto.first];
    if (i != -1000 ) object->addInfo( s_proto_map[proto.second.second], i);
  }

}

void ReportConvertTool::TrackObjectFromSummary( const HltObjectSummary::Info* info, LHCb::Track* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_track_unordered_map2_Turbo
                                : m_track_unordered_map2 );
  if( m_version < 3 ){
    int run1version=-999;
    // find size we care about (i.e. make sure extra info not counted)
    auto Isize = count_info( *info );
    // Looking at Run 1 data, need to know which map to use
    if( Isize == (m_track_unordered_map2.at(1)).size() ) run1version = 1;
    else if( Isize == (m_track_unordered_map2.at(2)).size() ) run1version = 2;
    else Error( "Track requested in Run 1 reports, but reports are unknown" , StatusCode::FAILURE, 100 );
    m_version=run1version;
  }

  LHCb::State first;
  LHCb::State  last;

  Gaudi::TrackSymMatrix cov;
  for(const auto& track : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( track.second.second )
    {
      case 0: first.setZ( (*info)[ track.first ] ); break;
      case 1: first.setX( (*info)[ track.first ] ); break;
      case 2: first.setY( (*info)[ track.first ] ); break;
      case 3: first.setTx( (*info)[ track.first ] ); break;
      case 4: first.setTy( (*info)[ track.first ] ); break;
      case 5: first.setQOverP( (*info)[ track.first ] ); break;
      case 6: object->setChi2PerDoF( (*info)[ track.first ] ); break;
      case 7: object->setNDoF( int( (*info)[ track.first ] ) ); break;
      case 8: object->setLikelihood( (*info)[ track.first ] ); break;
      case 9: object->setGhostProbability( (*info)[ track.first ] ); break;
      case 10: object->setFlags( (unsigned int)(*info)[ track.first ] ); break;
      case 11: last.setZ( (*info)[ track.first ] ); break;
      case 12: last.setX( (*info)[ track.first ] ); break;
      case 13: last.setY( (*info)[ track.first ] ); break;
      case 14: last.setTx( (*info)[ track.first ] ); break;
      case 15: last.setTy( (*info)[ track.first ] ); break;
      case 16: last.setQOverP( (*info)[ track.first ] ); break;
      case 17: if( (*info)[ track.first ] != -1000 ) object->addInfo( LHCb::Track::CloneDist, (*info)[ track.first ] ); break;
      case 18: if( (*info)[ track.first ] != -1000 ) object->addInfo( LHCb::Track::FitMatchChi2, (*info)[ track.first ] ); break;
      case 19: if( (*info)[ track.first ] != -1000 ) object->addInfo( LHCb::Track::FitVeloChi2, (*info)[ track.first ] ); break;
      case 20: if( (*info)[ track.first ] != -1000 ) object->addInfo( LHCb::Track::FitTChi2, (*info)[ track.first ] ); break;
      case 21: if( (*info)[ track.first ] != -1000 ) object->addInfo( LHCb::Track::FitVeloNDoF, (*info)[ track.first ] ); break;
      case 22: if( (*info)[ track.first ] != -1000 ) object->addInfo( LHCb::Track::FitTNDoF, (*info)[ track.first ] ); break;
      case 23: first.setFlags( (*info)[ track.first ] ); break;
      case 24: last.setFlags( (*info)[ track.first ] ); break;
      case 25: cov(0,0) = (*info)[ track.first ] ; break;
      case 26: cov(1,1) = (*info)[ track.first ] ; break;
      case 27: cov(2,2) = (*info)[ track.first ] ; break;
      case 28: cov(3,3) = (*info)[ track.first ] ; break;
      case 29: cov(4,4) = (*info)[ track.first ] ; break;
      case 30: cov(0,1) = (*info)[ track.first ] ; break;
      case 31: cov(0,2) = (*info)[ track.first ] ; break;
      case 32: cov(0,3) = (*info)[ track.first ] ; break;
      case 33: cov(0,4) = (*info)[ track.first ] ; break;
      case 34: cov(1,2) = (*info)[ track.first ] ; break;
      case 35: cov(1,3) = (*info)[ track.first ] ; break;
      case 36: cov(1,4) = (*info)[ track.first ] ; break;
      case 37: cov(2,3) = (*info)[ track.first ] ; break;
      case 38: cov(2,4) = (*info)[ track.first ] ; break;
      case 39: cov(3,4) = (*info)[ track.first ] ; break;
    }
  }

  // I am sure there is a smarter way of doing this
  if(cov(0,0)>0.0){
    cov(1,0)=cov(0,1);
    cov(2,0)=cov(0,2);
    cov(3,0)=cov(0,3);
    cov(4,0)=cov(0,4);
    cov(2,1)=cov(1,2);
    cov(3,1)=cov(1,3);
    cov(4,1)=cov(1,4);
    cov(3,2)=cov(2,3);
    cov(4,2)=cov(2,4);
    cov(4,3)=cov(3,4);
  }
  first.setCovariance(cov);

  object->addToStates(std::move(first));
  if(turbo) object->addToStates(std::move(last));
}

void ReportConvertTool::RichPIDObjectFromSummary( const HltObjectSummary::Info*info, LHCb::RichPID* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = ( turbo ? m_rpid_unordered_map2_Turbo
                                 : m_rpid_unordered_map2 );

  for(const auto& rpid : used_map.at( findBestPrevious( used_map, m_version ) ) ) {
    switch( rpid.second.second )
    {
      case 0: object->setPidResultCode( (unsigned int)(*info)[ rpid.first ] ); break;
      case 1: object->setParticleDeltaLL( Rich::ParticleIDType::Electron, (*info)[ rpid.first ] ); break;
      case 2: object->setParticleDeltaLL( Rich::ParticleIDType::Muon, (*info)[ rpid.first ] ); break;
      case 3: object->setParticleDeltaLL( Rich::ParticleIDType::Pion, (*info)[ rpid.first ] ); break;
      case 4: object->setParticleDeltaLL( Rich::ParticleIDType::Kaon, (*info)[ rpid.first ] ); break;
      case 5: object->setParticleDeltaLL( Rich::ParticleIDType::Proton, (*info)[ rpid.first ] ); break;
      case 6: object->setParticleDeltaLL( Rich::ParticleIDType::BelowThreshold, (*info)[ rpid.first ] ); break;
      case 7: object->setParticleDeltaLL( Rich::ParticleIDType::Deuteron, (*info)[ rpid.first ] ); break;
    }
  }

}

void ReportConvertTool::MuonPIDObjectFromSummary( const HltObjectSummary::Info* info, LHCb::MuonPID* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_mpid_unordered_map2_Turbo
                                : m_mpid_unordered_map2 );

  for(const auto& mpid: used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( mpid.second.second )
    {
      case 0: object->setMuonLLMu( (*info)[ mpid.first ] ); break;
      case 1: object->setMuonLLBg( (*info)[ mpid.first ] ); break;
      case 2: object->setNShared( int( (*info)[ mpid.first ] ) ); break;
      case 3: object->setStatus( int( (*info)[ mpid.first ] ) ); break;
      case 4: object->setIsMuon( int( (*info)[ mpid.first ] ) ); break;
      case 5: object->setIsMuonLoose( int( (*info)[ mpid.first ] ) ); break;
      case 6: object->setIsMuonTight( int( (*info)[ mpid.first ] ) ); break;
    }
  }

}

void ReportConvertTool::CaloClusterObjectFromSummary( const HltObjectSummary::Info* info, LHCb::CaloCluster* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_calo_unordered_map2_Turbo
                                : m_calo_unordered_map2 );
  Gaudi::Vector3 & xye = *(const_cast<Gaudi::Vector3*>(&object->position().parameters()));
  //
  xye(0)=0;
  xye(1)=0;
  xye(2)=0;

  for(const auto& calo : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( calo.second.second )
    {
      case 0:  // [[fallthrough]]
      case 1:  // [[fallthrough]]
      case 2:
        xye(calo.second.second) = (*info)[ calo.first ]; break;
      case 3:
        object->position().setZ( (*info)[ calo.first ] ); break;
    }
  }
}

void ReportConvertTool::CaloHypoObjectFromSummary( const HltObjectSummary::Info* info, LHCb::CaloHypo* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_calohypo_unordered_map2_Turbo
                                : m_calohypo_unordered_map2 );
  LHCb::CaloHypo::Position* pos = new LHCb::CaloHypo::Position();
  object->setPosition(pos);

  Gaudi::Vector3 & xye = *(const_cast<Gaudi::Vector3*>(&object->position()->parameters()));
  //
  double e=0;
  double x=0;
  double y=0;
  double z=0;

  for(const auto& calo : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( calo.second.second )
    {
      case 0: e = (*info)[ calo.first ]; break;
      case 1: x = (*info)[ calo.first ]; break;
      case 2: y = (*info)[ calo.first ]; break;
      case 3: z = (*info)[ calo.first ]; break;
      case 4: object->setLh( (*info)[ calo.first ] ); break;
      case 5: object->setHypothesis( static_cast<LHCb::CaloHypo::Hypothesis>( (*info)[ calo.first ] ) ); break;
    }
  }
  if(fabs(z+1.0)>0.01){
    xye(0) = x;
    xye(1) = y;
    xye(2) = e;
    object->position()->setZ( z );
  }
}

void ReportConvertTool::RecVertexObjectFromSummary( const HltObjectSummary::Info* info, LHCb::RecVertex* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_recvertex_unordered_map2_Turbo
                                : m_recvertex_unordered_map2 );
  if( m_version < 3 ){
    int run1version=-999;
    // find size we care about (i.e. make sure extra info not counted)
    unsigned int Isize = count_info( *info );
    // Looking at Run 1 data, need to know which map to use
    if( Isize == (m_recvertex_unordered_map2.at(1)).size() ) run1version = 1;
    else if( Isize == (m_recvertex_unordered_map2.at(2)).size() ) run1version = 2;
    else Error( "Track requested in Run 1 reports, but reports are unknown" , StatusCode::FAILURE, 100 );
    m_version=run1version;
  }

  Gaudi::XYZPoint xyz;
  Gaudi::SymMatrix3x3 & cov = *(const_cast<Gaudi::SymMatrix3x3*>(&object->covMatrix()));
  for(const auto& recvertex: used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( recvertex.second.second )
    {
      case 0: xyz.SetX( (*info)[ recvertex.first ] ); break;
      case 1: xyz.SetY( (*info)[ recvertex.first ] ); break;
      case 2: xyz.SetZ( (*info)[ recvertex.first ] ); break;
      case 3: object->setChi2( (*info)[ recvertex.first ] ); break;
      case 4: object->setNDoF( int( (*info)[ recvertex.first ] ) ); break;
      case 5: object->setTechnique( static_cast<LHCb::RecVertex::RecVertexType>( (*info)[ recvertex.first ] ) ); break;
      case 6: cov(0,0) = (*info)[ recvertex.first ] ; break;
      case 7: cov(1,1) = (*info)[ recvertex.first ] ; break;
      case 8: cov(2,2) = (*info)[ recvertex.first ] ; break;
      case 9: cov(1,0) = (*info)[ recvertex.first ] ; break;
      case 10: cov(2,0) = (*info)[ recvertex.first ] ; break;
      case 11: cov(2,1) = (*info)[ recvertex.first ] ; break;
    }
  }
  object->setPosition( xyz );

}

void ReportConvertTool::VertexObjectFromSummary( const HltObjectSummary::Info* info, LHCb::Vertex* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_vertex_unordered_map2_Turbo
                                : m_vertex_unordered_map2 );
  Gaudi::XYZPoint xyz;
  Gaudi::SymMatrix3x3 & cov = *(const_cast<Gaudi::SymMatrix3x3*>(&object->covMatrix()));

  for(const auto& vertex: used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( vertex.second.second )
    {
      case 0: object->setChi2( (*info)[ vertex.first ] ); break;
      case 1: object->setNDoF( int( (*info)[ vertex.first ] ) ); break;
      case 2: xyz.SetX( (*info)[ vertex.first ] ); break;
      case 3: xyz.SetY( (*info)[ vertex.first ] ); break;
      case 4: xyz.SetZ( (*info)[ vertex.first ] ); break;
      case 5: object->setTechnique( static_cast<LHCb::Vertex::CreationMethod>( (*info)[ vertex.first ] ) ); break;
      case 6: cov(0,0) = (*info)[ vertex.first ] ; break;
      case 7: cov(1,1) = (*info)[ vertex.first ] ; break;
      case 8: cov(2,2) = (*info)[ vertex.first ] ; break;
      case 9: cov(1,0) = (*info)[ vertex.first ] ; break;
      case 10: cov(2,0) = (*info)[ vertex.first ] ; break;
      case 11: cov(2,1) = (*info)[ vertex.first ] ; break;
    }
  }
  object->setPosition( xyz );
}

void ReportConvertTool::RecSummaryObjectFromSummary( const HltObjectSummary::Info* info, LHCb::RecSummary* object) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = m_recsummary_unordered_map2;
  for(const auto& recsummary : used_map.at( findBestPrevious( used_map, m_version ) ) ) {
    assert(recsummary.second.second<static_cast<int>(s_rsum_map.size()));
    object->addInfo( s_rsum_map[recsummary.second.second], (*info)[ recsummary.first ] );
  }
}

// Convert generic Gaudi::VectorMap summary back into the object
void ReportConvertTool::GenericMapObjectFromSummary( const HltObjectSummary::Info* info , GaudiUtils::VectorMap<short,float>* map) {
  unsigned int n=0;
  while(n < (info->size()) ){
    // First put the map key inside
    std::stringstream key;
    int len = snprintf(nullptr, 0, "%d", n);
    int padding = 4 - len;
    for(int m=0; m<padding; m++) key << "0";
    key << n << "#GenericKey";
    n++;

    short i_key = short( (*info)[key.str().c_str()] );
    // Then do the same for the value
    std::stringstream keykey;
    int len2 = snprintf(nullptr, 0, "%d", n);
    int padding2 = 4 - len2;
    for(int m=0; m<padding2; m++) keykey << "0";
    keykey << n << "#GenericValue";
    n++;

    float i_value = float( (*info)[keykey.str().c_str()] );

    map->insert(std::pair<short,float>(i_key,i_value));
  }
}
