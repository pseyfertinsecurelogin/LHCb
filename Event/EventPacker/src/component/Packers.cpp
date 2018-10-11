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
#include "PackerBaseAlg.icpp"

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

template class DataPacking::Pack<LHCb::MCRichHitPacker>;
typedef DataPacking::Pack<LHCb::MCRichHitPacker> A;
DECLARE_COMPONENT( A )

template class DataPacking::Pack<LHCb::MCRichOpticalPhotonPacker>;
typedef DataPacking::Pack<LHCb::MCRichOpticalPhotonPacker> B;
DECLARE_COMPONENT( B )

template class DataPacking::Pack<LHCb::MCRichSegmentPacker>;
typedef DataPacking::Pack<LHCb::MCRichSegmentPacker> C;
DECLARE_COMPONENT( C )

template class DataPacking::Pack<LHCb::MCRichTrackPacker>;
typedef DataPacking::Pack<LHCb::MCRichTrackPacker> D;
DECLARE_COMPONENT( D )

template class DataPacking::Pack<LHCb::MCPrsHitPacker>;
typedef DataPacking::Pack<LHCb::MCPrsHitPacker> Ea;
DECLARE_COMPONENT( Ea )

template class DataPacking::Pack<LHCb::MCSpdHitPacker>;
typedef DataPacking::Pack<LHCb::MCSpdHitPacker> Eb;
DECLARE_COMPONENT( Eb )

template class DataPacking::Pack<LHCb::MCEcalHitPacker>;
typedef DataPacking::Pack<LHCb::MCEcalHitPacker> Ec;
DECLARE_COMPONENT( Ec )

template class DataPacking::Pack<LHCb::MCHcalHitPacker>;
typedef DataPacking::Pack<LHCb::MCHcalHitPacker> Ed;
DECLARE_COMPONENT( Ed )

template class DataPacking::Pack<LHCb::MCVeloHitPacker>;
typedef DataPacking::Pack<LHCb::MCVeloHitPacker> F;
DECLARE_COMPONENT( F )

template class DataPacking::Pack<LHCb::MCVPHitPacker>;
typedef DataPacking::Pack<LHCb::MCVPHitPacker> Fp;
DECLARE_COMPONENT( Fp )

template class DataPacking::Pack<LHCb::MCPuVetoHitPacker>;
typedef DataPacking::Pack<LHCb::MCPuVetoHitPacker> G;
DECLARE_COMPONENT( G )

template class DataPacking::Pack<LHCb::MCTTHitPacker>;
typedef DataPacking::Pack<LHCb::MCTTHitPacker> H;
DECLARE_COMPONENT( H )

template class DataPacking::Pack<LHCb::MCUTHitPacker>;
typedef DataPacking::Pack<LHCb::MCUTHitPacker> Hu;
DECLARE_COMPONENT( Hu )

template class DataPacking::Pack<LHCb::MCITHitPacker>;
typedef DataPacking::Pack<LHCb::MCITHitPacker> I;
DECLARE_COMPONENT( I )

template class DataPacking::Pack<LHCb::MCOTHitPacker>;
typedef DataPacking::Pack<LHCb::MCOTHitPacker> J;
DECLARE_COMPONENT( J )

template class DataPacking::Pack<LHCb::MCMuonHitPacker>;
typedef DataPacking::Pack<LHCb::MCMuonHitPacker> K;
DECLARE_COMPONENT( K )

template class DataPacking::Pack<LHCb::MCRichDigitSummaryPacker>;
typedef DataPacking::Pack<LHCb::MCRichDigitSummaryPacker> L;
DECLARE_COMPONENT( L )

template class DataPacking::Pack<LHCb::RichPIDPacker>;
typedef DataPacking::Pack<LHCb::RichPIDPacker> M;
DECLARE_COMPONENT( M )

template class DataPacking::Pack<LHCb::MuonPIDPacker>;
typedef DataPacking::Pack<LHCb::MuonPIDPacker> N;
DECLARE_COMPONENT( N )

template class DataPacking::Pack<LHCb::ParticlePacker>;
typedef DataPacking::Pack<LHCb::ParticlePacker> O;
DECLARE_COMPONENT( O )

template class DataPacking::Pack<LHCb::VertexPacker>;
typedef DataPacking::Pack<LHCb::VertexPacker> P;
DECLARE_COMPONENT( P )

template class DataPacking::Pack<LHCb::WeightsVectorPacker>;
typedef DataPacking::Pack<LHCb::WeightsVectorPacker> Q;
DECLARE_COMPONENT( Q )

template class DataPacking::Pack<LHCb::CaloClusterPacker>;
typedef DataPacking::Pack<LHCb::CaloClusterPacker> R;
DECLARE_COMPONENT( R )

template class DataPacking::Pack<LHCb::CaloDigitPacker>;
typedef DataPacking::Pack<LHCb::CaloDigitPacker> Ra;
DECLARE_COMPONENT( Ra )

template class DataPacking::Pack<LHCb::CaloAdcPacker>;
typedef DataPacking::Pack<LHCb::CaloAdcPacker> Rb;
DECLARE_COMPONENT( Rb )

template class DataPacking::Pack<LHCb::MCFTHitPacker>;
typedef DataPacking::Pack<LHCb::MCFTHitPacker> S_;
DECLARE_COMPONENT( S_ )

template class DataPacking::Pack<LHCb::MCSLHitPacker>;
typedef DataPacking::Pack<LHCb::MCSLHitPacker> T;
DECLARE_COMPONENT( T )

template class DataPacking::Pack<LHCb::FlavourTagPacker>;
typedef DataPacking::Pack<LHCb::FlavourTagPacker> U;
DECLARE_COMPONENT( U )

template class DataPacking::Pack<LHCb::MCHCHitPacker>;
typedef DataPacking::Pack<LHCb::MCHCHitPacker> V;
DECLARE_COMPONENT( V )

template class DataPacking::Pack<LHCb::MCBcmHitPacker>;
typedef DataPacking::Pack<LHCb::MCBcmHitPacker> Va;
DECLARE_COMPONENT( Va )

template class DataPacking::Pack<LHCb::MCBlsHitPacker>;
typedef DataPacking::Pack<LHCb::MCBlsHitPacker> Vb;
DECLARE_COMPONENT( Vb )

template class DataPacking::Pack<LHCb::RelatedInfoRelationsPacker>;
typedef DataPacking::Pack<LHCb::RelatedInfoRelationsPacker> W;
DECLARE_COMPONENT( W )



 
