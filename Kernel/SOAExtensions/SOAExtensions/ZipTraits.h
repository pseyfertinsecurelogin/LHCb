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

#ifndef ZipTraits_h
#define ZipTraits_h 1
#include "SOAContainer/SOAUtils.h" // for is_view
#include <type_traits>             // for decay_t

namespace Zipping {
  template <typename CONTAINER>
  class ZipContainer;

  namespace details {
    template <typename T>
    struct has_semantic_zip_helper {
      static constexpr bool value = false;
    };

    template <typename VIEW>
    struct has_semantic_zip_helper<ZipContainer<VIEW>> {
      static constexpr bool value = SOA::Utils::is_view<std::decay_t<VIEW>>::value;
    };
  } // namespace details

  /** type trait to identify ZipContainers as such
   *
   * has_semantic_zip qualifies if a type can be argument or return value of Zipping::semantic_zip.
   */
  template <typename T>
  struct has_semantic_zip {
    // remove const and reference from ZipContainer before going one level deeper
    static constexpr bool value = details::has_semantic_zip_helper<std::decay_t<T>>::value;
  };

  template <typename VIEW>
  constexpr static bool has_semantic_zip_v = has_semantic_zip<VIEW>::value;
} // namespace Zipping

#endif
