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

#ifndef ZipUtils_H
#define ZipUtils_H 1
#include "SOAContainer/SOAUtils.h" // for ALL
#include "ZipTraits.h"             // IWYU pragma: keep
#include <type_traits>             // for enable_if_t

namespace Zipping {
  template <typename FIRST, typename SECOND, typename... OTHERS,
            typename = typename std::enable_if_t<SOA::Utils::ALL( has_semantic_zip_v<FIRST>, has_semantic_zip_v<SECOND>,
                                                                  has_semantic_zip_v<OTHERS>... )>>
  bool are_semantically_compatible( FIRST& first, SECOND& second, OTHERS&... others ) {
    return ( first.zipIdentifier() == second.zipIdentifier() ) && ( are_semantically_compatible( first, others... ) );
  }

  template <typename FIRST, typename = typename std::enable_if_t<has_semantic_zip_v<FIRST>>>
  bool are_semantically_compatible( FIRST& /*unused*/ ) {
    return true;
  }
} // namespace Zipping

#endif
