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
#include "UnpackerBaseAlg.icpp"

#include "Event/PackedCaloAdc.h"
#include "Event/PackedCaloCluster.h"
#include "Event/PackedCaloDigit.h"
#include "Event/PackedFlavourTag.h"
#include "Event/PackedMCCaloHit.h"
#include "Event/PackedMCHit.h"
#include "Event/PackedMCRichDigitSummary.h"
#include "Event/PackedMCRichHit.h"
#include "Event/PackedMCRichOpticalPhoton.h"
#include "Event/PackedMCRichSegment.h"
#include "Event/PackedMCRichTrack.h"
#include "Event/PackedMuonPID.h"
#include "Event/PackedPartToRelatedInfoRelation.h"
#include "Event/PackedParticle.h"
#include "Event/PackedRichPID.h"
#include "Event/PackedVertex.h"
#include "Event/PackedWeightsVector.h"

template class DataPacking::Unpack<LHCb::MCRichHitPacker>;
typedef DataPacking::Unpack<LHCb::MCRichHitPacker> A;
DECLARE_COMPONENT( A )

template class DataPacking::Unpack<LHCb::MCRichOpticalPhotonPacker>;
typedef DataPacking::Unpack<LHCb::MCRichOpticalPhotonPacker> B;
DECLARE_COMPONENT( B )

template class DataPacking::Unpack<LHCb::MCRichSegmentPacker>;
typedef DataPacking::Unpack<LHCb::MCRichSegmentPacker> C;
DECLARE_COMPONENT( C )

template class DataPacking::Unpack<LHCb::MCRichTrackPacker>;
typedef DataPacking::Unpack<LHCb::MCRichTrackPacker> D;
DECLARE_COMPONENT( D )

template class DataPacking::Unpack<LHCb::MCPrsHitPacker>;
typedef DataPacking::Unpack<LHCb::MCPrsHitPacker> Ea;
DECLARE_COMPONENT( Ea )

template class DataPacking::Unpack<LHCb::MCSpdHitPacker>;
typedef DataPacking::Unpack<LHCb::MCSpdHitPacker> Eb;
DECLARE_COMPONENT( Eb )

template class DataPacking::Unpack<LHCb::MCEcalHitPacker>;
typedef DataPacking::Unpack<LHCb::MCEcalHitPacker> Ec;
DECLARE_COMPONENT( Ec )

template class DataPacking::Unpack<LHCb::MCHcalHitPacker>;
typedef DataPacking::Unpack<LHCb::MCHcalHitPacker> Ed;
DECLARE_COMPONENT( Ed )

template class DataPacking::Unpack<LHCb::MCVeloHitPacker>;
typedef DataPacking::Unpack<LHCb::MCVeloHitPacker> F;
DECLARE_COMPONENT( F )

template class DataPacking::Unpack<LHCb::MCVPHitPacker>;
typedef DataPacking::Unpack<LHCb::MCVPHitPacker> Fp;
DECLARE_COMPONENT( Fp )

template class DataPacking::Unpack<LHCb::MCPuVetoHitPacker>;
typedef DataPacking::Unpack<LHCb::MCPuVetoHitPacker> G;
DECLARE_COMPONENT( G )

template class DataPacking::Unpack<LHCb::MCTTHitPacker>;
typedef DataPacking::Unpack<LHCb::MCTTHitPacker> H;
DECLARE_COMPONENT( H )

template class DataPacking::Unpack<LHCb::MCUTHitPacker>;
typedef DataPacking::Unpack<LHCb::MCUTHitPacker> Hu;
DECLARE_COMPONENT( Hu )

template class DataPacking::Unpack<LHCb::MCITHitPacker>;
typedef DataPacking::Unpack<LHCb::MCITHitPacker> I;
DECLARE_COMPONENT( I )

template class DataPacking::Unpack<LHCb::MCOTHitPacker>;
typedef DataPacking::Unpack<LHCb::MCOTHitPacker> J;
DECLARE_COMPONENT( J )

template class DataPacking::Unpack<LHCb::MCMuonHitPacker>;
typedef DataPacking::Unpack<LHCb::MCMuonHitPacker> K;
DECLARE_COMPONENT( K )

template class DataPacking::Unpack<LHCb::MCRichDigitSummaryPacker>;
typedef DataPacking::Unpack<LHCb::MCRichDigitSummaryPacker> L;
DECLARE_COMPONENT( L )

template class DataPacking::Unpack<LHCb::RichPIDPacker>;
typedef DataPacking::Unpack<LHCb::RichPIDPacker> M;
DECLARE_COMPONENT( M )

template class DataPacking::Unpack<LHCb::MuonPIDPacker>;
typedef DataPacking::Unpack<LHCb::MuonPIDPacker> N;
DECLARE_COMPONENT( N )

template class DataPacking::Unpack<LHCb::ParticlePacker>;
typedef DataPacking::Unpack<LHCb::ParticlePacker> O;
DECLARE_COMPONENT( O )

template class DataPacking::Unpack<LHCb::VertexPacker>;
typedef DataPacking::Unpack<LHCb::VertexPacker> P;
DECLARE_COMPONENT( P )

template class DataPacking::Unpack<LHCb::WeightsVectorPacker>;
typedef DataPacking::Unpack<LHCb::WeightsVectorPacker> Q;
DECLARE_COMPONENT( Q )

template class DataPacking::Unpack<LHCb::CaloClusterPacker>;
typedef DataPacking::Unpack<LHCb::CaloClusterPacker> R;
DECLARE_COMPONENT( R )

template class DataPacking::Unpack<LHCb::CaloDigitPacker>;
typedef DataPacking::Unpack<LHCb::CaloDigitPacker> Ra;
DECLARE_COMPONENT( Ra )

template class DataPacking::Unpack<LHCb::CaloAdcPacker>;
typedef DataPacking::Unpack<LHCb::CaloAdcPacker> Rb;
DECLARE_COMPONENT( Rb )

template class DataPacking::Unpack<LHCb::MCFTHitPacker>;
typedef DataPacking::Unpack<LHCb::MCFTHitPacker> S_;
DECLARE_COMPONENT( S_ )

template class DataPacking::Unpack<LHCb::MCSLHitPacker>;
typedef DataPacking::Unpack<LHCb::MCSLHitPacker> T;
DECLARE_COMPONENT( T )

template class DataPacking::Unpack<LHCb::FlavourTagPacker>;
typedef DataPacking::Unpack<LHCb::FlavourTagPacker> U;
DECLARE_COMPONENT( U )

template class DataPacking::Unpack<LHCb::MCHCHitPacker>;
typedef DataPacking::Unpack<LHCb::MCHCHitPacker> V;
DECLARE_COMPONENT( V )

template class DataPacking::Unpack<LHCb::MCBcmHitPacker>;
typedef DataPacking::Unpack<LHCb::MCBcmHitPacker> Va;
DECLARE_COMPONENT( Va )

template class DataPacking::Unpack<LHCb::MCBlsHitPacker>;
typedef DataPacking::Unpack<LHCb::MCBlsHitPacker> Vb;
DECLARE_COMPONENT( Vb )

template class DataPacking::Unpack<LHCb::RelatedInfoRelationsPacker>;
typedef DataPacking::Unpack<LHCb::RelatedInfoRelationsPacker> W;
DECLARE_COMPONENT( W )
