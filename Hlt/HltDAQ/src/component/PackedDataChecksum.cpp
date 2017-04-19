#include "PackedDataChecksum.h"

namespace PackedDataPersistence {

template<>
void PackedDataChecksum::processObject(const LHCb::PackedTracks& x, const std::string& key) {
  // no packingVersion here
  process(key, x.version());
  processVector(key, x.tracks());
  processVector(key, x.states());
  processVector(key, x.ids());
  processVector(key, x.extras());
}

template<>
void PackedDataChecksum::processObject(const LHCb::PackedCaloClusters& x, const std::string& key) {
  process(key, x.packingVersion());
  process(key, x.version());
  processVector(key, x.data());
  processVector(key, x.entries());
}

template<>
void PackedDataChecksum::processObject(const LHCb::PackedRichPIDs& x, const std::string& key) {
  process(key, x.packingVersion());
  process(key, x.version());
  processVector(key, x.data());
}

template<>
void PackedDataChecksum::processObject(const LHCb::PackedMuonPIDs& x, const std::string& key) {
  process(key, x.packingVersion());
  process(key, x.version());
  processVector(key, x.data());
}

template<>
void PackedDataChecksum::processObject(const LHCb::PackedCaloHypos& x, const std::string& key) {
  process(key, x.packingVersion());
  process(key, x.version());
  processVector(key, x.hypos());
  processVector(key, x.refs());
}

template<>
void PackedDataChecksum::processObject(const LHCb::PackedProtoParticles& x, const std::string& key) {
  process(key, x.packingVersion());
  process(key, x.version());
  processVector(key, x.protos());
  processVector(key, x.refs());
  processVector(key, x.extras());
}

template<>
void PackedDataChecksum::processObject(const LHCb::PackedParticles& x, const std::string& key) {
  process(key, x.packingVersion());
  process(key, x.version());
  processVector(key, x.data());
  processVector(key, x.daughters());
  processVector(key, x.extra());
}

template<>
void PackedDataChecksum::processObject(const LHCb::PackedVertices& x, const std::string& key) {
  process(key, x.packingVersion());
  process(key, x.version());
  processVector(key, x.data());
  processVector(key, x.outgoingParticles());
  processVector(key, x.extras());
}

template<>
void PackedDataChecksum::processObject(const LHCb::PackedRecVertices& x, const std::string& key) {
  process(key, x.packingVersion());
  process(key, x.version());
  processVector(key, x.vertices());
  processVector(key, x.refs());
  processVector(key, x.extras());
  processVector(key, x.weights());
}

template<>
void PackedDataChecksum::processObject(const LHCb::PackedFlavourTags& x, const std::string& key) {
  process(key, x.packingVersion());
  process(key, x.version());
  processVector(key, x.data());
  processVector(key, x.taggers());
  processVector(key, x.taggeringPs());
}

template<>
void PackedDataChecksum::processObject(const LHCb::PackedRelations& x, const std::string& key) {
  process(key, x.version());
  processVector(key, x.relations());
  processVector(key, x.sources());
  processVector(key, x.dests());
}

template<>
void PackedDataChecksum::processObject(const LHCb::PackedRelatedInfoRelations& x, const std::string& key) {
  process(key, x.packingVersion());
  process(key, x.version());
  processVector(key, x.info());
  processVector(key, x.relations());
  processVector(key, x.containers());
}

template<typename T>
void PackedDataChecksum::process(const std::string& key, const T& x) {
  m_result[key].process_bytes((void*)&x, sizeof(x));
  // Add to a "global" checksum, too
  m_result[""].process_bytes((void*)&x, sizeof(x));

}

template <typename ...Args>
void PackedDataChecksum::processMany(const std::string& key, Args&&... args) {
  auto list = {(process(key, args),1)...};
  (void)list;
}

template<typename T>
void PackedDataChecksum::processVector(const std::string& key, const std::vector<T>& v) {
  for (const auto& x: v)
    process(key, x);
}

// We need to make sure the checksum is properly implemented even after
// structures evolve. The following is an ungly way to force us to make sure
// that is the case. Not sure it catches all possible cases...
#ifdef __GNUC__
static_assert(sizeof(LHCb::PackedTrack) == 56, "PackedTrack has changed!");  // padded!
static_assert(sizeof(LHCb::PackedState) == 68, "PackedState has changed!");
static_assert(sizeof(LHCb::PackedRichPID) == 56, "PackedRichPID has changed!");  // padded!
static_assert(sizeof(LHCb::PackedMuonPID) == 40, "PackedMuonPID has changed!");
static_assert(sizeof(LHCb::PackedCaloCluster) == 72, "PackedCaloCluster has changed!");  // padded!
static_assert(sizeof(LHCb::PackedCaloClusterEntry) == 16, "PackedCaloClusterEntry has changed!");  // padded!
static_assert(sizeof(LHCb::PackedCaloHypo) == 76, "PackedCaloHypo has changed!");
static_assert(sizeof(LHCb::PackedProtoParticle) == 40, "PackedProtoParticle has changed!");
static_assert(sizeof(LHCb::PackedRecVertex) == 60, "PackedRecVertex has changed!");  // padded!
#endif

// We need to define custom functions for some structures. This is because
// they have padding bytes, which are undefined and will spoil the checksum.
template<>
void PackedDataChecksum::process(const std::string& key, const LHCb::PackedTrack& x) {
  processMany(key,
              x.key, x.chi2PerDoF, x.nDoF, x.flags,
              x.firstId, x.lastId,
              x.firstState, x.lastState,
              x.firstExtra, x.lastExtra,
              x.likelihood, x.ghostProba);
}
template<>
void PackedDataChecksum::process(const std::string& key, const LHCb::PackedRichPID& x) {
  processMany(key,
              x.pidResultCode, x.dllEl, x.dllMu, x.dllPi, x.dllKa, x.dllPr,
              x.track, x.dllBt, x.key, x.dllDe);
}
template<>
void PackedDataChecksum::process(const std::string& key, const LHCb::PackedCaloClusterEntry& x) {
  processMany(key,
              x.digit, x.status, x.fraction);
}
template<>
void PackedDataChecksum::process(const std::string& key, const LHCb::PackedCaloCluster& x) {
  processMany(key,
              x.key, x.type, x.seed,
              x.pos_x, x.pos_y, x.pos_z, x.pos_e,
              x.pos_c0, x.pos_c1,
              x.pos_cov00, x.pos_cov11, x.pos_cov22,
              x.pos_cov10, x.pos_cov20, x.pos_cov21,
              x.pos_spread00, x.pos_spread11,
              x.pos_spread10,
              x.firstEntry, x.lastEntry);
}

template<>
void PackedDataChecksum::process(const std::string& key, const LHCb::PackedRecVertex& x) {
  processMany(key,
              x.key, x.technique, x.chi2, x.nDoF,
              x.x, x.y, x.z,
              x.cov00, x.cov11, x.cov22, x.cov10, x.cov20, x.cov21,
              x.firstTrack, x.lastTrack,
              x.firstInfo, x.lastInfo);
              // skip x.container since it is not serialized, see PackedRecVertex.h
}

template<>
void PackedDataChecksum::process(const std::string& key, const LHCb::PackedParticle& x) {
  processMany(key,
              x.particleID, x.measMass, x.measMassErr, x.lv_px, x.lv_py, x.lv_pz, x.lv_mass,
              x.refx, x.refy, x.refz, x.momCov00, x.momCov11, x.momCov22, x.momCov33, x.momCov10, x.momCov20,
              x.momCov21, x.momCov30, x.momCov31, x.momCov32, x.posCov00, x.posCov11, x.posCov22, x.posCov10,
              x.posCov20, x.posCov21, x.pmCov00, x.pmCov01, x.pmCov02, x.pmCov10, x.pmCov11, x.pmCov12,
              x.pmCov20, x.pmCov21, x.pmCov22, x.pmCov30, x.pmCov31, x.pmCov32,
              x.vertex, x.proto, x.firstExtra, x.lastExtra, x.firstDaughter, x.lastDaughter);
}

template<>
void PackedDataChecksum::process(const std::string& key, const LHCb::PackedVertex& x) {
  processMany(key,
              x.key, x.technique, x.chi2, x.nDoF,
              x.x, x.y, x.z,
              x.cov00, x.cov11, x.cov22, x.cov10, x.cov20, x.cov21,
              x.firstOutgoingPart, x.lastOutgoingPart,
              x.firstInfo, x.lastInfo);
}

}  // namespace PackedDataPersistence
