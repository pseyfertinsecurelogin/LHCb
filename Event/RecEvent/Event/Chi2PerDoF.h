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
#include "LHCbMath/SIMDWrapper.h"

namespace LHCb::Rec {
  template <typename dType = SIMDWrapper::scalar::types>
  struct Chi2PerDoF {
    typename dType::float_v               chi2PerDoF;
    typename dType::int_v                 nDoF;
    [[nodiscard]] typename dType::float_v chi2() const {
      using float_v = typename dType::float_v;
      return chi2PerDoF * (float_v)nDoF;
    }
  };
} // namespace LHCb::Rec
