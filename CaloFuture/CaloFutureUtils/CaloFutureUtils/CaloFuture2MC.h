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
#pragma once
#include "Event/CaloHypo.h"

namespace LHCb {
  class MCParticle;
  class CaloCluster;
  class CaloDigit;
} // namespace LHCb

template <class FROM, class TO, class WEIGHT>
class IRelationWeighted;
template <class FROM, class TO>
class LinkerWithKey;
template <class FROM, class TO>
class LinkedTo;
namespace LHCb {
  template <class FROM, class TO, class WEIGHT>
  class RelationWeighted1D;
} // namespace LHCb

/** @class CaloFuture2MC CaloFuture2MC.h Event/CaloFuture2MC.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2006-02-21
 */
namespace LHCb::CaloFuture2MC {
  using IClusterTable = IRelationWeighted<LHCb::CaloCluster, LHCb::MCParticle, float>;
  using ClusterTable  = RelationWeighted1D<LHCb::CaloCluster, LHCb::MCParticle, float>;

  using IDigitTable = IRelationWeighted<LHCb::CaloDigit, LHCb::MCParticle, float>;
  using DigitTable  = RelationWeighted1D<LHCb::CaloDigit, LHCb::MCParticle, float>;

  using DigitLink   = LinkerWithKey<LHCb::MCParticle, LHCb::CaloDigit>;
  using DigitLinkTo = LinkedTo<LHCb::MCParticle, LHCb::CaloDigit>;

  using HypoLink   = LinkerWithKey<LHCb::MCParticle, LHCb::CaloHypo>;
  using HypoLinkTo = LinkedTo<LHCb::MCParticle, LHCb::CaloHypo>;

  //    using TrgClusterTable = RelationWeighted1D<TrgCaloFutureCluster,MCParticle,float> ;

} // namespace LHCb::CaloFuture2MC
