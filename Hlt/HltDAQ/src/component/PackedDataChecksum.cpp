#include "PackedDataChecksum.h"

namespace PackedDataPersistence {

void PackedDataChecksum::processObject(const LHCb::PackedTracks& x) {
  // no packingVersion here
  process(x.version());
  processVector(x.tracks());
  processVector(x.states());
  processVector(x.ids());
  processVector(x.extras());
}

void PackedDataChecksum::processObject(const LHCb::PackedCaloClusters& x) {
  process(x.packingVersion());
  process(x.version());
  processVector(x.data());
  processVector(x.entries());
}

void PackedDataChecksum::processObject(const LHCb::PackedRichPIDs& x) {
  process(x.packingVersion());
  process(x.version());
  processVector(x.data());
}

void PackedDataChecksum::processObject(const LHCb::PackedMuonPIDs& x) {
  process(x.packingVersion());
  process(x.version());
  processVector(x.data());
}

void PackedDataChecksum::processObject(const LHCb::PackedCaloHypos& x) {
  process(x.packingVersion());
  process(x.version());
  processVector(x.hypos());
  processVector(x.refs());
}

void PackedDataChecksum::processObject(const LHCb::PackedProtoParticles& x) {
  process(x.packingVersion());
  process(x.version());
  processVector(x.protos());
  processVector(x.refs());
  processVector(x.extras());
}

void PackedDataChecksum::processObject(const LHCb::PackedRecVertices& x) {
  process(x.packingVersion());
  process(x.version());
  processVector(x.vertices());
  processVector(x.refs());
  processVector(x.extras());
  processVector(x.weights());
}

template<typename T>
void PackedDataChecksum::process(const T& x) {
  m_result.process_bytes((void*)&x, sizeof(x));
}

template <typename ...Args>
void PackedDataChecksum::processMany(Args&&... args) {
  auto list = {(process(args),1)...};
  (void)list;
}

template<typename T>
void PackedDataChecksum::processVector(const std::vector<T>& v) {
  for (const auto& x: v)
    process(x);
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
void PackedDataChecksum::process(const LHCb::PackedTrack& x) {
  processMany(x.key, x.chi2PerDoF, x.nDoF, x.flags,
              x.firstId, x.lastId,
              x.firstState, x.lastState,
              x.firstExtra, x.lastExtra,
              x.likelihood, x.ghostProba);
}
template<>
void PackedDataChecksum::process(const LHCb::PackedRichPID& x) {
  processMany(x.pidResultCode, x.dllEl, x.dllMu, x.dllPi, x.dllKa, x.dllPr,
              x.track, x.dllBt, x.key, x.dllDe);
}
template<>
void PackedDataChecksum::process(const LHCb::PackedCaloClusterEntry& x) {
  processMany(x.digit, x.status, x.fraction);
}
template<>
void PackedDataChecksum::process(const LHCb::PackedCaloCluster& x) {
  processMany(x.key, x.type, x.seed,
              x.pos_x, x.pos_y, x.pos_z, x.pos_e,
              x.pos_c0, x.pos_c1,
              x.pos_cov00, x.pos_cov11, x.pos_cov22,
              x.pos_cov10, x.pos_cov20, x.pos_cov21,
              x.pos_spread00, x.pos_spread11,
              x.pos_spread10,
              x.firstEntry, x.lastEntry);
}
template<>
void PackedDataChecksum::process(const LHCb::PackedRecVertex& x) {
  processMany(x.key, x.technique, x.chi2, x.nDoF,
              x.x, x.y, x.z,
              x.cov00, x.cov11, x.cov22, x.cov10, x.cov20, x.cov21,
              x.firstTrack, x.lastTrack,
              x.firstInfo, x.lastInfo);
              // skip x.container since it is not serialized, see PackedRecVertex.h
}


}