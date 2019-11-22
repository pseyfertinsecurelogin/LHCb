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

#include <cassert>

namespace Rich::Future::HistoUtils {

  /// Utility method to save a histogram pointer with checks
  template <typename HISTO>
  [[nodiscard]] inline bool saveAndCheck( HISTO*& h1, HISTO* h2 ) noexcept {
    assert( h1 == nullptr );  // should never overwrite a previously cached valid pointer
    h1 = h2;                  // save input
    return ( h1 != nullptr ); // input should always be valid
  }

} // namespace Rich::Future::HistoUtils
