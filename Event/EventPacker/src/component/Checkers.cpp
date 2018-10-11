/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "CheckerBaseAlg.icpp"

#include "Event/PackedMCRichHit.h"
#include "Event/PackedMCRichOpticalPhoton.h"
#include "Event/PackedMCRichSegment.h"
#include "Event/PackedMCRichTrack.h"
#include "Event/PackedMCCaloHit.h"
#include "Event/PackedMCHit.h"
#include "Event/PackedMCRichDigitSummary.h"
#include "Event/PackedMuonPID.h"
#include "Event/PackedRichPID.h"
#include "Event/PackedParticle.h"
#include "Event/PackedVertex.h"
#include "Event/PackedWeightsVector.h"
#include "Event/PackedCaloAdc.h"
#include "Event/PackedCaloDigit.h"
#include "Event/PackedCaloCluster.h"
#include "Event/PackedFlavourTag.h"
#include "Event/PackedPartToRelatedInfoRelation.h"

template class DataPacking::Check<LHCb::MCRichHitPacker>;
typedef DataPacking::Check<LHCb::MCRichHitPacker> A;
DECLARE_COMPONENT( A )

template class DataPacking::Check<LHCb::MCRichOpticalPhotonPacker>;
typedef DataPacking::Check<LHCb::MCRichOpticalPhotonPacker> B;
DECLARE_COMPONENT( B )

template class DataPacking::Check<LHCb::MCRichSegmentPacker>;
typedef DataPacking::Check<LHCb::MCRichSegmentPacker> C;
DECLARE_COMPONENT( C )

template class DataPacking::Check<LHCb::MCRichTrackPacker>;
typedef DataPacking::Check<LHCb::MCRichTrackPacker> D;
DECLARE_COMPONENT( D )

template class DataPacking::Check<LHCb::MCPrsHitPacker>;
typedef DataPacking::Check<LHCb::MCPrsHitPacker> Ea;
DECLARE_COMPONENT( Ea )

template class DataPacking::Check<LHCb::MCSpdHitPacker>;
typedef DataPacking::Check<LHCb::MCSpdHitPacker> Eb;
DECLARE_COMPONENT( Eb )

template class DataPacking::Check<LHCb::MCEcalHitPacker>;
typedef DataPacking::Check<LHCb::MCEcalHitPacker> Ec;
DECLARE_COMPONENT( Ec )

template class DataPacking::Check<LHCb::MCHcalHitPacker>;
typedef DataPacking::Check<LHCb::MCHcalHitPacker> Ed;
DECLARE_COMPONENT( Ed )

template class DataPacking::Check<LHCb::MCVeloHitPacker>;
typedef DataPacking::Check<LHCb::MCVeloHitPacker> F;
DECLARE_COMPONENT( F )

template class DataPacking::Check<LHCb::MCPuVetoHitPacker>;
typedef DataPacking::Check<LHCb::MCPuVetoHitPacker> G;
DECLARE_COMPONENT( G )

template class DataPacking::Check<LHCb::MCVPHitPacker>;
typedef DataPacking::Check<LHCb::MCVPHitPacker> Fp;
DECLARE_COMPONENT( Fp )

template class DataPacking::Check<LHCb::MCTTHitPacker>;
typedef DataPacking::Check<LHCb::MCTTHitPacker> H;
DECLARE_COMPONENT( H )

template class DataPacking::Check<LHCb::MCUTHitPacker>;
typedef DataPacking::Check<LHCb::MCUTHitPacker> Hu;
DECLARE_COMPONENT( Hu )

template class DataPacking::Check<LHCb::MCITHitPacker>;
typedef DataPacking::Check<LHCb::MCITHitPacker> I;
DECLARE_COMPONENT( I )

template class DataPacking::Check<LHCb::MCOTHitPacker>;
typedef DataPacking::Check<LHCb::MCOTHitPacker> J;
DECLARE_COMPONENT( J )

template class DataPacking::Check<LHCb::MCMuonHitPacker>;
typedef DataPacking::Check<LHCb::MCMuonHitPacker> K;
DECLARE_COMPONENT( K )

template class DataPacking::Check<LHCb::MCRichDigitSummaryPacker>;
typedef DataPacking::Check<LHCb::MCRichDigitSummaryPacker> L;
DECLARE_COMPONENT( L )

template class DataPacking::Check<LHCb::RichPIDPacker>;
typedef DataPacking::Check<LHCb::RichPIDPacker> M;
DECLARE_COMPONENT( M )

template class DataPacking::Check<LHCb::MuonPIDPacker>;
typedef DataPacking::Check<LHCb::MuonPIDPacker> N;
DECLARE_COMPONENT( N )

template class DataPacking::Check<LHCb::ParticlePacker>;
typedef DataPacking::Check<LHCb::ParticlePacker> O;
DECLARE_COMPONENT( O )

template class DataPacking::Check<LHCb::VertexPacker>;
typedef DataPacking::Check<LHCb::VertexPacker> P;
DECLARE_COMPONENT( P )

template class DataPacking::Check<LHCb::WeightsVectorPacker>;
typedef DataPacking::Check<LHCb::WeightsVectorPacker> Q;
DECLARE_COMPONENT( Q )

template class DataPacking::Check<LHCb::CaloClusterPacker>;
typedef DataPacking::Check<LHCb::CaloClusterPacker> R;
DECLARE_COMPONENT( R )

template class DataPacking::Check<LHCb::CaloDigitPacker>;
typedef DataPacking::Check<LHCb::CaloDigitPacker> Ra;
DECLARE_COMPONENT( Ra )

template class DataPacking::Check<LHCb::CaloAdcPacker>;
typedef DataPacking::Check<LHCb::CaloAdcPacker> Rb;
DECLARE_COMPONENT( Rb )

template class DataPacking::Check<LHCb::MCFTHitPacker>;
typedef DataPacking::Check<LHCb::MCFTHitPacker> S_;
DECLARE_COMPONENT( S_ )

template class DataPacking::Check<LHCb::MCSLHitPacker>;
typedef DataPacking::Check<LHCb::MCSLHitPacker> T;
DECLARE_COMPONENT( T )

template class DataPacking::Check<LHCb::FlavourTagPacker>;
typedef DataPacking::Check<LHCb::FlavourTagPacker> U;
DECLARE_COMPONENT( U )

template class DataPacking::Check<LHCb::MCHCHitPacker>;
typedef DataPacking::Check<LHCb::MCHCHitPacker> V;
DECLARE_COMPONENT( V )

template class DataPacking::Check<LHCb::MCBcmHitPacker>;
typedef DataPacking::Check<LHCb::MCBcmHitPacker> Va;
DECLARE_COMPONENT( Va )

template class DataPacking::Check<LHCb::MCBlsHitPacker>;
typedef DataPacking::Check<LHCb::MCBlsHitPacker> Vb;
DECLARE_COMPONENT( Vb )

template class DataPacking::Check<LHCb::RelatedInfoRelationsPacker>;
typedef DataPacking::Check<LHCb::RelatedInfoRelationsPacker> W;
DECLARE_COMPONENT( W )
