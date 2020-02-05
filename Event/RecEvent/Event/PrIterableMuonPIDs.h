/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once
#include "Event/PrMuonPIDs.h"
#include "Event/PrZip.h"
#include "Kernel/HeaderMapping.h"

namespace LHCb::Pr::Muon {
  /** Proxy type for iterating over BestVertexRelations objects. */
  DECLARE_PROXY( Proxy ) {
    PROXY_METHODS( Proxy, dType, unwrap, PIDs, m_pids );
    auto IsMuon() const { return this->m_pids->template IsMuon<dType, unwrap>( this->offset() ); }
    auto Chi2Corr() const { return this->m_pids->template Chi2Corr<dType, unwrap>( this->offset() ); }
  };
} // namespace LHCb::Pr::Muon

// Register the proxy as appropriate for the container.
// Note that this must be done *before* we can write
// LHCb::Pr::unwrapped_zip_t<LHCb::Pr::Muon::PIDs> just below.
REGISTER_PROXY( LHCb::Pr::Muon::PIDs, LHCb::Pr::Muon::Proxy );
REGISTER_HEADER( LHCb::Pr::Muon::PIDs, "Event/PrIterableMuonPIDs.h" );

namespace LHCb::Pr::Iterable::Scalar::Muon {
  using PIDs = LHCb::Pr::unwrapped_zip_t<LHCb::Pr::Muon::PIDs>;
} // namespace LHCb::Pr::Iterable::Scalar::Muon
