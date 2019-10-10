/*
 * Copyright (C) 2019  CERN for the benefit of the LHCb collaboration
 * Author: Paul Seyfert <pseyfert@cern.ch>
 *
 * This software is distributed under the terms of the GNU General Public
 * Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
 *
 * In applying this licence, CERN does not waive the privileges and immunities
 * granted to it by virtue of its status as an Intergovernmental Organization
 * or submit itself to any jurisdiction.
 */
#pragma once

#include "Event/PrZip.h"
#include "Event/RecVertices.h"

namespace LHCb::Rec::PV {
  // copy&pasted
  template <typename MergedProxy, typename dType, bool unwrap>
  struct Proxy {
    PVs const* m_pvs{nullptr};
    Proxy( PVs const* pvs ) : m_pvs{pvs} {}
    [[nodiscard]] auto offset() const { return static_cast<MergedProxy const&>( *this ).offset(); }
    [[nodiscard]] auto size() const { return m_pvs->size(); }

    [[nodiscard]] auto x() const { return this->m_pvs->template pos_x<dType, unwrap>( this->offset() ); }
    [[nodiscard]] auto y() const { return this->m_pvs->template pos_y<dType, unwrap>( this->offset() ); }
    [[nodiscard]] auto z() const { return this->m_pvs->template pos_z<dType, unwrap>( this->offset() ); }
    [[nodiscard]] auto pos() const { return this->m_pvs->template pos<dType, unwrap>( this->offset() ); }
  };
} // namespace LHCb::Rec::PV

template <>
struct LHCb::Pr::Proxy<LHCb::Rec::PV::PVs> {
  template <typename MergedProxy, typename dType, bool unwrap>
  using type = LHCb::Rec::PV::Proxy<MergedProxy, dType, unwrap>;
};

namespace LHCb::Rec::Iterable::PV {
  using PVs = LHCb::Pr::zip_t<LHCb::Rec::PV::PVs>;
} // namespace LHCb::Rec::Iterable::PV

namespace LHCb::Rec::Iterable::Scalar::PV {
  using PVs = LHCb::Pr::unwrapped_zip_t<LHCb::Rec::PV::PVs>;
} // namespace LHCb::Rec::Iterable::Scalar::PV
