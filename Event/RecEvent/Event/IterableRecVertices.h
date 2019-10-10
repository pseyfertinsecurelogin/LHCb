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
  DECLARE_PROXY( Proxy ) {
    PROXY_METHODS( dType, unwrap, PVs, m_pvs );

    [[nodiscard]] auto x() const { return this->m_pvs->template pos_x<dType, unwrap>( this->offset() ); }
    [[nodiscard]] auto y() const { return this->m_pvs->template pos_y<dType, unwrap>( this->offset() ); }
    [[nodiscard]] auto z() const { return this->m_pvs->template pos_z<dType, unwrap>( this->offset() ); }
    [[nodiscard]] auto pos() const { return this->m_pvs->template pos<dType, unwrap>( this->offset() ); }
    [[nodiscard]] auto position() const { return this->m_pvs->template pos<dType, unwrap>( this->offset() ); }
    [[nodiscard]] auto covMatrix() const { return this->m_pvs->template cov<dType, unwrap>( this->offset() ); }
    [[nodiscard]] auto chi2perdof() const { return this->m_pvs->template chi2perdof<dType, unwrap>( this->offset() ); }
  };
} // namespace LHCb::Rec::PV

REGISTER_PROXY( LHCb::Rec::PV::PVs, LHCb::Rec::PV::Proxy );
REGISTER_HEADER( LHCb::Rec::PV::PVs, "Event/IterableRecVertices.h" );

namespace LHCb::Rec::Iterable::PV {
  using PVs = LHCb::Pr::zip_t<LHCb::Rec::PV::PVs>;
} // namespace LHCb::Rec::Iterable::PV

namespace LHCb::Rec::Iterable::Scalar::PV {
  using PVs = LHCb::Pr::unwrapped_zip_t<LHCb::Rec::PV::PVs>;
} // namespace LHCb::Rec::Iterable::Scalar::PV
