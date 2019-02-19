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
// $Id: PackedEventDict.h,v 1.6 2010-05-18 09:04:35 jonrob Exp $
#ifndef DICT_PACKEDEVENTDICT_H
#define DICT_PACKEDEVENTDICT_H 1

// Include files for LCG dictionary
#include "Event/GenericBuffer.h"
#include "Event/PackedCaloAdc.h"
#include "Event/PackedCaloCluster.h"
#include "Event/PackedCaloDigit.h"
#include "Event/PackedCaloHypo.h"
#include "Event/PackedCluster.h"
#include "Event/PackedDecReport.h"
#include "Event/PackedFlavourTag.h"
#include "Event/PackedMCCaloHit.h"
#include "Event/PackedMCHit.h"
#include "Event/PackedMCParticle.h"
#include "Event/PackedMCRichDigitSummary.h"
#include "Event/PackedMCRichHit.h"
#include "Event/PackedMCRichOpticalPhoton.h"
#include "Event/PackedMCRichSegment.h"
#include "Event/PackedMCRichTrack.h"
#include "Event/PackedMCVertex.h"
#include "Event/PackedMuonPID.h"
#include "Event/PackedPartToRelatedInfoRelation.h"
#include "Event/PackedParticle.h"
#include "Event/PackedParticle2Ints.h"
#include "Event/PackedProtoParticle.h"
#include "Event/PackedRecVertex.h"
#include "Event/PackedRelations.h"
#include "Event/PackedRichPID.h"
#include "Event/PackedTrack.h"
#include "Event/PackedTwoProngVertex.h"
#include "Event/PackedVertex.h"
#include "Event/PackedWeightsVector.h"

namespace {
  struct _Instantiations {
    LHCb::PackedRelatedInfoRelations::PackedRelatedInfo i1;
    LHCb::PackedRelatedInfoRelations::InfoVector        i2;
  };
} // namespace

#endif // DICT_PACKEDEVENTDICT_H
