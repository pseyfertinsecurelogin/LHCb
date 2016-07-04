// local
#include "HltDAQ/ReportConvertTool.h"

using LHCb::HltObjectSummary;
using LHCb::HltSelRepRBStdInfo;

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
  int sum=0;
  sum+=m_particle_unordered_map2.at(findBestPrevious(m_particle_unordered_map2,m_LatestVersion)).size();
  return sum;
}

int ReportConvertTool::findBestPrevious(const unordered_map<int, unordered_map<string,pair<int,int> > >& map,int in) const{
  int out=in;
  for(int m=in;m>0;m--){
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
        }
        else if( subbank->size()<m_track_unordered_map2_Turbo.at( findBestPrevious( m_track_unordered_map2_Turbo, m_version ) ).size() ) used_map = &m_track_unordered_map2;
        else used_map = &m_track_unordered_map2_Turbo;
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
        }
        else if( subbank->size()<m_recvertex_unordered_map2_Turbo.at( findBestPrevious( m_recvertex_unordered_map2_Turbo, m_version ) ).size() ) used_map = &m_recvertex_unordered_map2;
        else used_map = &m_recvertex_unordered_map2_Turbo;
      }
      break;
    case LHCb::CLID_Vertex:
      {
        if( subbank->size()<m_vertex_unordered_map2_Turbo.at( findBestPrevious( m_vertex_unordered_map2_Turbo, m_version ) ).size() ) used_map = &m_vertex_unordered_map2;
        else used_map = &m_vertex_unordered_map2_Turbo;
      }
      break;
    case LHCb::CLID_Particle:
      {
        if(m_version<3) {
          // Looking at Run 1 data, need to know which map to use
          if( subbank->size() == (m_particle_unordered_map2.at(1)).size() ) run1version = 1;
          else if( subbank->size() == (m_particle_unordered_map2.at(2)).size() ) run1version = 2;
          used_map = &m_particle_unordered_map2;
          m_version=run1version;
        }
        else if( subbank->size()<m_particle_unordered_map2_Turbo.at( findBestPrevious( m_particle_unordered_map2_Turbo, m_version ) ).size() ) used_map = &m_particle_unordered_map2;
        else used_map = &m_particle_unordered_map2_Turbo;
      }
      break;
    case LHCb::CLID_ProtoParticle:
      {
        if( subbank->size()<m_proto_unordered_map2_Turbo.at( findBestPrevious( m_proto_unordered_map2_Turbo, m_version ) ).size() ) used_map = &m_proto_unordered_map2;
        else used_map = &m_proto_unordered_map2_Turbo;
      }
      break;
    case LHCb::CLID_RichPID:
      {
        if( subbank->size()<m_rpid_unordered_map2_Turbo.at( findBestPrevious( m_rpid_unordered_map2_Turbo, m_version ) ).size() ) used_map = &m_rpid_unordered_map2;
        else used_map = &m_rpid_unordered_map2_Turbo;
      }
      break;
    case LHCb::CLID_CaloHypo:
      {
        if( subbank->size()<m_calohypo_unordered_map2_Turbo.at( findBestPrevious( m_calohypo_unordered_map2_Turbo, m_version ) ).size() ) used_map = &m_calohypo_unordered_map2;
        else used_map = &m_calohypo_unordered_map2_Turbo;
      }
      break;
    case LHCb::CLID_MuonPID:
      {
        if( subbank->size()<m_mpid_unordered_map2_Turbo.at( findBestPrevious( m_mpid_unordered_map2_Turbo, m_version ) ).size() ) used_map = &m_mpid_unordered_map2;
        else used_map = &m_mpid_unordered_map2_Turbo;
      }
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
      {
        if(m_version<3) {
          // Looking at Run 1 data, need to know which map to use
          if( subbank->size() == (m_calo_unordered_map2.at(1)).size() ) run1version = 1;
          else if( subbank->size() == (m_calo_unordered_map2.at(2)).size() ) run1version = 2;
          used_map = &m_calo_unordered_map2;
          m_version=run1version;
        }
        else if( subbank->size()<m_calo_unordered_map2_Turbo.at( findBestPrevious( m_calo_unordered_map2_Turbo, m_version ) ).size() ) used_map = &m_calo_unordered_map2;
        else used_map = &m_calo_unordered_map2_Turbo;
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
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
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
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = ( turbo ? m_proto_unordered_map2_Turbo : m_proto_unordered_map2 );

  for(const auto& proto : used_map.at( findBestPrevious( used_map, m_version ) ) ) {
    switch( proto.second.second )
    {
      case 0:  info->insert( proto.first, float( object->info( LHCb::ProtoParticle::IsPhoton, -1000 ) ) ); break;
      case 1:  info->insert( proto.first, float( object->info( LHCb::ProtoParticle::IsNotE, -1000 ) ) ); break;
      case 2:  info->insert( proto.first, float( object->info( LHCb::ProtoParticle::IsNotH, -1000 ) ) ); break;
      case 3:  info->insert( proto.first, float( object->info( LHCb::ProtoParticle::EcalPIDe, -1000 ) ) ); break;
      case 4:  info->insert( proto.first, float( object->info( LHCb::ProtoParticle::PrsPIDe, -1000 ) ) ); break;
      case 5:  info->insert( proto.first, float( object->info( LHCb::ProtoParticle::BremPIDe, -1000 ) ) ); break;
      case 6:  info->insert( proto.first, float( object->info( LHCb::ProtoParticle::HcalPIDe, -1000 ) ) ); break;
      case 7:  info->insert( proto.first, float( object->info( LHCb::ProtoParticle::HcalPIDmu, -1000 ) ) ); break;
      case 8:  info->insert( proto.first, float( object->info( LHCb::ProtoParticle::EcalPIDmu, -1000 ) ) ); break;
      case 9:  info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloTrMatch, -1000 ) ) ); break;
      case 10: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloElectronMatch, -1000 ) ) ); break;
      case 11: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloBremMatch, -1000 ) ) ); break;
      case 12: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloNeutralSpd, -1000 ) ) ); break;
      case 13: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloNeutralPrs, -1000 ) ) ); break;
      case 14: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloNeutralEcal, -1000 ) ) ); break;
      case 15: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloNeutralHcal2Ecal, -1000 ) ) ); break;
      case 16: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloNeutralE49, -1000 ) ) ); break;
      case 17: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloNeutralID, -1000 ) ) ); break;
      case 18: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloDepositID, -1000 ) ) ); break;
      case 19: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::ShowerShape, -1000 ) ) ); break;
      case 20: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::ClusterMass, -1000 ) ) ); break;
      case 21: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloSpdE, -1000 ) ) ); break;
      case 22: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloPrsE, -1000 ) ) ); break;
      case 23: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloEcalE, -1000 ) ) ); break;
      case 24: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloHcalE, -1000 ) ) ); break;
      case 25: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloEcalChi2, -1000 ) ) ); break;
      case 26: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloBremChi2, -1000 ) ) ); break;
      case 27: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloClusChi2, -1000 ) ) ); break;
      case 28: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloNeutralPrsM, -1000 ) ) ); break;
      case 29: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloShapeFr2r4, -1000 ) ) ); break;
      case 30: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloShapeKappa, -1000 ) ) ); break;
      case 31: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloShapeAsym, -1000 ) ) ); break;
      case 32: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloShapeE1, -1000 ) ) ); break;
      case 33: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloShapeE2, -1000 ) ) ); break;
      case 34: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloPrsShapeE2, -1000 ) ) ); break;
      case 35: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloPrsShapeEmax, -1000 ) ) ); break;
      case 36: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloPrsShapeFr2, -1000 ) ) ); break;
      case 37: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloPrsShapeAsym, -1000 ) ) ); break;
      case 38: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloPrsM, -1000 ) ) ); break;
      case 39: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloPrsM15, -1000 ) ) ); break;
      case 40: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloPrsM30, -1000 ) ) ); break;
      case 41: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloPrsM45, -1000 ) ) ); break;
      case 42: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloClusterCode, -1000 ) ) ); break;
      case 43: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloClusterFrac, -1000 ) ) ); break;
      case 44: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CombDLLe, -1000 ) ) ); break;
      case 45: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CombDLLmu, -1000 ) ) ); break;
      case 46: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CombDLLpi, -1000 ) ) ); break;
      case 47: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CombDLLk, -1000 ) ) ); break;
      case 48: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CombDLLp, -1000 ) ) ); break;
      case 49: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::InAccBrem, -1000 ) ) ); break;
      case 50: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::InAccSpd, -1000 ) ) ); break;
      case 51: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::InAccPrs, -1000 ) ) ); break;
      case 52: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::InAccEcal, -1000 ) ) ); break;
      case 53: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::InAccHcal, -1000 ) ) ); break;
      case 54: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::VeloCharge, -1000 ) ) ); break;
      case 55: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::RichPIDStatus, -1000 ) ) ); break;
      case 56: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloChargedID, -1000 ) ) ); break;
      case 57: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloChargedEcal, -1000 ) ) ); break;
      case 58: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloChargedPrs, -1000 ) ) ); break;
      case 59: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::CaloChargedSpd, -1000 ) ) ); break;
      case 60: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::ProbNNe, -1000 ) ) ); break;
      case 61: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::ProbNNmu, -1000 ) ) ); break;
      case 62: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::ProbNNpi, -1000 ) ) ); break;
      case 63: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::ProbNNk, -1000 ) ) ); break;
      case 64: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::ProbNNp, -1000 ) ) ); break;
      case 65: info->insert( proto.first, float( object->info( LHCb::ProtoParticle::ProbNNghost, -1000 ) ) ); break;
    }
  }

}

void ReportConvertTool::TrackObject2Summary( HltObjectSummary::Info* info, const LHCb::Track* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
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
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
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
    }
  }

}

void ReportConvertTool::MuonPIDObject2Summary( HltObjectSummary::Info* info , const LHCb::MuonPID* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
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
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
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
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
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
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
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
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
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
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = m_recsummary_unordered_map2;
  for(const auto& recsummary : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( recsummary.second.second )
    {
      case 0: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nLongTracks,0 ) ) ); break;
      case 1: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nDownstreamTracks,0 ) ) ); break;
      case 2: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nUpstreamTracks,0 ) ) ); break;
      case 3: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nVeloTracks,0 ) ) ); break;
      case 4: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nTTracks,0 ) ) ); break;
      case 5: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nBackTracks,0 ) ) ); break;
      case 6: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nTracks,0 ) ) ); break;
      case 7: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nRich1Hits,0 ) ) ); break;
      case 8: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nRich2Hits,0 ) ) ); break;
      case 9: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nVeloClusters,0 ) ) ); break;
      case 10: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nITClusters,0 ) ) ); break;
      case 11: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nTTClusters,0 ) ) ); break;
      case 12: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nUTClusters,0 ) ) ); break;
      case 13: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nOTClusters,0 ) ) ); break;
      case 14: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nFTClusters,0 ) ) ); break;
      case 15: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nSPDhits,0 ) ) ); break;
      case 16: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nMuonCoordsS0,0 ) ) ); break;
      case 17: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nMuonCoordsS1,0 ) ) ); break;
      case 18: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nMuonCoordsS2,0 ) ) ); break;
      case 19: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nMuonCoordsS3,0 ) ) ); break;
      case 20: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nMuonCoordsS4,0 ) ) ); break;
      case 21: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nMuonTracks,0 ) ) ); break;
      case 22: info->insert( recsummary.first, float( object->info( LHCb::RecSummary::nPVs,0 ) ) ); break;
    }
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
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_particle_unordered_map2_Turbo
                                : m_particle_unordered_map2 );
  if( m_version < 3 ){
    int run1version=-999;
    // find size we care about (i.e. make sure extra info not counted)
    unsigned int Isize = 0;
    for(const auto & elem : *info){
      if( (elem.first).find("#")!=std::string::npos ) Isize++;
    }
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
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_proto_unordered_map2_Turbo
                                : m_proto_unordered_map2 );
  for(const auto& proto : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( proto.second.second )
    {
      case 0: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::IsPhoton, ( (*info)[ proto.first ] ) ); break;//381
      case 1: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::IsNotE, ( (*info)[ proto.first ] ) ); break;//382
      case 2: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::IsNotH, ( (*info)[ proto.first ] ) ); break;//383
      case 3: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::EcalPIDe, ( (*info)[ proto.first ] ) ); break;//360
      case 4: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::PrsPIDe, ( (*info)[ proto.first ] ) ); break;//361
      case 5: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::BremPIDe, ( (*info)[ proto.first ] ) ); break;//362
      case 6: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::HcalPIDe, ( (*info)[ proto.first ] ) ); break;//363
      case 7: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::HcalPIDmu, ( (*info)[ proto.first ] ) ); break;//364
      case 8: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::EcalPIDmu, ( (*info)[ proto.first ] ) ); break;//365
      case 9: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloTrMatch, ( (*info)[ proto.first ] ) ); break;//310
      case 10: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloElectronMatch, ( (*info)[ proto.first ] ) ); break;//311
      case 11: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloBremMatch, ( (*info)[ proto.first ] ) ); break;//312
      case 12: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloNeutralSpd, ( (*info)[ proto.first ] ) ); break;//323
      case 13: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloNeutralPrs, ( (*info)[ proto.first ] ) ); break;//324
      case 14: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloNeutralEcal, ( (*info)[ proto.first ] ) ); break;//325
      case 15: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloNeutralHcal2Ecal, ( (*info)[ proto.first ] ) ); break;//326
      case 16: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloNeutralE49, ( (*info)[ proto.first ] ) ); break;//327
      case 17: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloNeutralID, ( (*info)[ proto.first ] ) ); break;//328
      case 18: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloDepositID, ( (*info)[ proto.first ] ) ); break;//320
      case 19: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::ShowerShape, ( (*info)[ proto.first ] ) ); break;//321
      case 20: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::ClusterMass, ( (*info)[ proto.first ] ) ); break;//322
      case 21: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloSpdE, ( (*info)[ proto.first ] ) ); break;//330
      case 22: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloPrsE, ( (*info)[ proto.first ] ) ); break;//331
      case 23: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloEcalE, ( (*info)[ proto.first ] ) ); break;//332
      case 24: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloHcalE, ( (*info)[ proto.first ] ) ); break;//333
      case 25: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloEcalChi2, ( (*info)[ proto.first ] ) ); break;//334
      case 26: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloBremChi2, ( (*info)[ proto.first ] ) ); break;//335
      case 27: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloClusChi2, ( (*info)[ proto.first ] ) ); break;//336
      case 28: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloNeutralPrsM, ( (*info)[ proto.first ] ) ); break;//343
      case 29: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloShapeFr2r4, ( (*info)[ proto.first ] ) ); break;//344
      case 30: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloShapeKappa, ( (*info)[ proto.first ] ) ); break;//345
      case 31: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloShapeAsym, ( (*info)[ proto.first ] ) ); break;//346
      case 32: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloShapeE1, ( (*info)[ proto.first ] ) ); break;//347
      case 33: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloShapeE2, ( (*info)[ proto.first ] ) ); break;//348
      case 34: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloPrsShapeE2, ( (*info)[ proto.first ] ) ); break;//349
      case 35: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloPrsShapeEmax, ( (*info)[ proto.first ] ) ); break;//350
      case 36: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloPrsShapeFr2, ( (*info)[ proto.first ] ) ); break;//351
      case 37: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloPrsShapeAsym, ( (*info)[ proto.first ] ) ); break;//352
      case 38: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloPrsM, ( (*info)[ proto.first ] ) ); break;//353
      case 39: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloPrsM15, ( (*info)[ proto.first ] ) ); break;//354
      case 40: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloPrsM30, ( (*info)[ proto.first ] ) ); break;//355
      case 41: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloPrsM45, ( (*info)[ proto.first ] ) ); break;//356
      case 42: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloClusterCode, ( (*info)[ proto.first ] ) ); break;//357
      case 43: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloClusterFrac, ( (*info)[ proto.first ] ) ); break;//358
      case 44: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CombDLLe, ( (*info)[ proto.first ] ) ); break;//600
      case 45: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CombDLLmu, ( (*info)[ proto.first ] ) ); break;//601
      case 46: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CombDLLpi, ( (*info)[ proto.first ] ) ); break;//602
      case 47: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CombDLLk, ( (*info)[ proto.first ] ) ); break;//603
      case 48: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CombDLLp, ( (*info)[ proto.first ] ) ); break;//604
      case 49: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::InAccBrem, ( (*info)[ proto.first ] ) ); break;// Extra for PID group
      case 50: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::InAccSpd, ( (*info)[ proto.first ] ) ); break;// ''
      case 51: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::InAccPrs, ( (*info)[ proto.first ] ) ); break;// ''
      case 52: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::InAccEcal, ( (*info)[ proto.first ] ) ); break;// ''
      case 53: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::InAccHcal, ( (*info)[ proto.first ] ) ); break;// ''
      case 54: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::VeloCharge, ( (*info)[ proto.first ] ) ); break;// ''
      case 55: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::RichPIDStatus, ( (*info)[ proto.first ] ) ); break;// ''
      case 56: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloChargedID, ( (*info)[ proto.first ] ) ); break;//
      case 57: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloChargedEcal, ( (*info)[ proto.first ] ) ); break;//
      case 58: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloChargedPrs, ( (*info)[ proto.first ] ) ); break;//
      case 59: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::CaloChargedSpd, ( (*info)[ proto.first ] ) ); break;//
      case 60: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::ProbNNe, ( (*info)[ proto.first ] ) ); break;//
      case 61: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::ProbNNmu, ( (*info)[ proto.first ] ) ); break;//
      case 62: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::ProbNNpi, ( (*info)[ proto.first ] ) ); break;//
      case 63: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::ProbNNk, ( (*info)[ proto.first ] ) ); break;//
      case 64: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::ProbNNp, ( (*info)[ proto.first ] ) ); break;//
      case 65: if( (*info)[ proto.first ] != -1000 ) object->addInfo( LHCb::ProtoParticle::ProbNNghost, ( (*info)[ proto.first ] ) ); break;//
    }
  }

}

void ReportConvertTool::TrackObjectFromSummary( const HltObjectSummary::Info* info, LHCb::Track* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_track_unordered_map2_Turbo
                                : m_track_unordered_map2 );
  if( m_version < 3 ){
    int run1version=-999;
    // find size we care about (i.e. make sure extra info not counted)
    unsigned int Isize = 0;
    for(const auto & elem : *info){
      if( (elem.first).find("#")!=std::string::npos ) Isize++;
    }
    // Looking at Run 1 data, need to know which map to use
    if( Isize == (m_track_unordered_map2.at(1)).size() ) run1version = 1;
    else if( Isize == (m_track_unordered_map2.at(2)).size() ) run1version = 2;
    else Error( "Track requested in Run 1 reports, but reports are unknown" , StatusCode::FAILURE, 100 );
    m_version=run1version;
  }

  auto  first = new LHCb::State();
  auto  last = new LHCb::State();

  Gaudi::TrackSymMatrix cov;
  for(const auto& track : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( track.second.second )
    {
      case 0: first->setZ( (*info)[ track.first ] ); break;
      case 1: first->setX( (*info)[ track.first ] ); break;
      case 2: first->setY( (*info)[ track.first ] ); break;
      case 3: first->setTx( (*info)[ track.first ] ); break;
      case 4: first->setTy( (*info)[ track.first ] ); break;
      case 5: first->setQOverP( (*info)[ track.first ] ); break;
      case 6: object->setChi2PerDoF( (*info)[ track.first ] ); break;
      case 7: object->setNDoF( int( (*info)[ track.first ] ) ); break;
      case 8: object->setLikelihood( (*info)[ track.first ] ); break;
      case 9: object->setGhostProbability( (*info)[ track.first ] ); break;
      case 10: object->setFlags( (unsigned int)(*info)[ track.first ] ); break;
      case 11: last->setZ( (*info)[ track.first ] ); break;
      case 12: last->setX( (*info)[ track.first ] ); break;
      case 13: last->setY( (*info)[ track.first ] ); break;
      case 14: last->setTx( (*info)[ track.first ] ); break;
      case 15: last->setTy( (*info)[ track.first ] ); break;
      case 16: last->setQOverP( (*info)[ track.first ] ); break;
      case 17: if( (*info)[ track.first ] != -1000 ) object->addInfo( LHCb::Track::CloneDist, (*info)[ track.first ] ); break;
      case 18: if( (*info)[ track.first ] != -1000 ) object->addInfo( LHCb::Track::FitMatchChi2, (*info)[ track.first ] ); break;
      case 19: if( (*info)[ track.first ] != -1000 ) object->addInfo( LHCb::Track::FitVeloChi2, (*info)[ track.first ] ); break;
      case 20: if( (*info)[ track.first ] != -1000 ) object->addInfo( LHCb::Track::FitTChi2, (*info)[ track.first ] ); break;
      case 21: if( (*info)[ track.first ] != -1000 ) object->addInfo( LHCb::Track::FitVeloNDoF, (*info)[ track.first ] ); break;
      case 22: if( (*info)[ track.first ] != -1000 ) object->addInfo( LHCb::Track::FitTNDoF, (*info)[ track.first ] ); break;
      case 23: first->setFlags( (*info)[ track.first ] ); break;
      case 24: last->setFlags( (*info)[ track.first ] ); break;
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
  first->setCovariance(cov);

  object->addToStates(*first);
  // stop memory leak
  if(turbo) object->addToStates(*last);
  else      delete last;
}

void ReportConvertTool::RichPIDObjectFromSummary( const HltObjectSummary::Info*info, LHCb::RichPID* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
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
    }
  }

}

void ReportConvertTool::MuonPIDObjectFromSummary( const HltObjectSummary::Info* info, LHCb::MuonPID* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
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
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_calo_unordered_map2_Turbo
                                : m_calo_unordered_map2 );
  Gaudi::Vector3 & xye = *(const_cast<Gaudi::Vector3*>(&object->position().parameters()));
  //
  double e=0;
  double x=0;
  double y=0;

  for(const auto& calo : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( calo.second.second )
    {
      case 0: e = (*info)[ calo.first ]; break;
      case 1: x = (*info)[ calo.first ]; break;
      case 2: y = (*info)[ calo.first ]; break;
      case 3: object->position().setZ( (*info)[ calo.first ] ); break;
    }
  }
  xye(0) = x;
  xye(1) = y;
  xye(2) = e;

}

void ReportConvertTool::CaloHypoObjectFromSummary( const HltObjectSummary::Info* info, LHCb::CaloHypo* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
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
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? m_recvertex_unordered_map2_Turbo
                                : m_recvertex_unordered_map2 );
  if( m_version < 3 ){
    int run1version=-999;
    // find size we care about (i.e. make sure extra info not counted)
    unsigned int Isize = 0;
    for(const auto & elem : *info){
      if( (elem.first).find("#")!=std::string::npos ) Isize++;
    }
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
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
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
    Warning( "I have not been told a verision number to use, assuming the latest", StatusCode::SUCCESS, 20 );
    m_version = m_LatestVersion;
  }

  const auto& used_map = m_recsummary_unordered_map2;

  for(const auto& recsummary : used_map.at( findBestPrevious( used_map, m_version ) ) ) {
    switch( recsummary.second.second )
    {
      case 0: ( object->addInfo( LHCb::RecSummary::nLongTracks, (*info)[ recsummary.first ] ) ); break;
      case 1: ( object->addInfo( LHCb::RecSummary::nDownstreamTracks, (*info)[ recsummary.first ] ) ); break;
      case 2: ( object->addInfo( LHCb::RecSummary::nUpstreamTracks, (*info)[ recsummary.first ] ) ); break;
      case 3: ( object->addInfo( LHCb::RecSummary::nVeloTracks, (*info)[ recsummary.first ] ) ); break;
      case 4: ( object->addInfo( LHCb::RecSummary::nTTracks, (*info)[ recsummary.first ] ) ); break;
      case 5: ( object->addInfo( LHCb::RecSummary::nBackTracks, (*info)[ recsummary.first ] ) ); break;
      case 6: ( object->addInfo( LHCb::RecSummary::nTracks, (*info)[ recsummary.first ] ) ); break;
      case 7: ( object->addInfo( LHCb::RecSummary::nRich1Hits, (*info)[ recsummary.first ] ) ); break;
      case 8: ( object->addInfo( LHCb::RecSummary::nRich2Hits, (*info)[ recsummary.first ] ) ); break;
      case 9: ( object->addInfo( LHCb::RecSummary::nVeloClusters, (*info)[ recsummary.first ] ) ); break;
      case 10: ( object->addInfo( LHCb::RecSummary::nITClusters, (*info)[ recsummary.first ] ) ); break;
      case 11: ( object->addInfo( LHCb::RecSummary::nTTClusters, (*info)[ recsummary.first ] ) ); break;
      case 12: ( object->addInfo( LHCb::RecSummary::nUTClusters, (*info)[ recsummary.first ] ) ); break;
      case 13: ( object->addInfo( LHCb::RecSummary::nOTClusters, (*info)[ recsummary.first ] ) ); break;
      case 14: ( object->addInfo( LHCb::RecSummary::nFTClusters, (*info)[ recsummary.first ] ) ); break;
      case 15: ( object->addInfo( LHCb::RecSummary::nSPDhits, (*info)[ recsummary.first ] ) ); break;
      case 16: ( object->addInfo( LHCb::RecSummary::nMuonCoordsS0, (*info)[ recsummary.first ] ) ); break;
      case 17: ( object->addInfo( LHCb::RecSummary::nMuonCoordsS1, (*info)[ recsummary.first ] ) ); break;
      case 18: ( object->addInfo( LHCb::RecSummary::nMuonCoordsS2, (*info)[ recsummary.first ] ) ); break;
      case 19: ( object->addInfo( LHCb::RecSummary::nMuonCoordsS3, (*info)[ recsummary.first ] ) ); break;
      case 20: ( object->addInfo( LHCb::RecSummary::nMuonCoordsS4, (*info)[ recsummary.first ] ) ); break;
      case 21: ( object->addInfo( LHCb::RecSummary::nMuonTracks, (*info)[ recsummary.first ] ) ); break;
      case 22: ( object->addInfo( LHCb::RecSummary::nPVs, (*info)[ recsummary.first ] ) ); break;
    }
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
