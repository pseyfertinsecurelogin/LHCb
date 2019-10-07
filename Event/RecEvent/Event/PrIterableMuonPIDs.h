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

namespace LHCb::Pr::Muon {
  /** Proxy type for iterating over BestVertexRelations objects. */
  template <typename MergedProxy, typename dType, bool unwrap>
  struct Proxy {
    // TODO these next four lines could/should be macro'd
    PIDs const* m_pids{nullptr};
    Proxy( PIDs const* pids ) : m_pids{pids} {}
    auto offset() const { return static_cast<MergedProxy const&>( *this ).offset(); }
    auto size() const { return m_pids->size(); }

    auto IsMuon() const { return this->m_pids->template IsMuon<dType, unwrap>( this->offset() ); }
    auto Chi2Corr() const { return this->m_pids->template Chi2Corr<dType, unwrap>( this->offset() ); }
  };
} // namespace LHCb::Pr::Muon

template <>
struct LHCb::Pr::Proxy<LHCb::Pr::Muon::PIDs> {
  template <typename MergedProxy, typename dType, bool unwrap>
  using type = LHCb::Pr::Muon::Proxy<MergedProxy, dType, unwrap>;
};

namespace LHCb::Pr::Iterable::Muon {
  using PIDs = LHCb::Pr::zip_t<LHCb::Pr::Muon::PIDs>;
} // namespace LHCb::Pr::Iterable::Muon

namespace LHCb::Pr::Iterable::Scalar::Muon {
  using PIDs = LHCb::Pr::unwrapped_zip_t<LHCb::Pr::Muon::PIDs>;
} // namespace LHCb::Pr::Iterable::Scalar::Muon
