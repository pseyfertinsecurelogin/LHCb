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
#include "SOAContainer/SOAUtils.h"
#include <type_traits>

namespace Zipping {
  template <typename CONTAINER>
  class ZipContainer;

  namespace details {
    /** @class has_semantic_zip_helper ZipTraits.h
     *
     * @brief helper type for Zipping::has_semantic_zip. Not intended for use outside ZipTraits.h
     *
     * @tparam T any other type than a Zipping::ZipContainer
     */
    template <typename T>
    struct has_semantic_zip_helper : std::false_type {};

    /**
     * @brief template specialization of helper type for Zipping::has_semantic_zip. Not intended for use outside
     * ZipTraits.h
     *
     * @tparam SOA::View that is wrapped in a Zipping::ZipContainer
     */
    template <typename VIEW>
    struct has_semantic_zip_helper<ZipContainer<VIEW>>
        : std::bool_constant<SOA::Utils::is_view<std::decay_t<VIEW>>::value> {};
  } // namespace details

  /** @class has_semantic_zip ZipTraits.h
   *
   * @brief Type trait type to ZipContainers. Whether or not a type can be argument/return to 'semantic_zip'.
   *
   * @tparam type of which availability of 'Zipping::semantic_zip' is queried
   */

  // remove const and reference from ZipContainer before going one level deeper
  template <typename T>
  using has_semantic_zip = details::has_semantic_zip_helper<std::decay_t<T>>;

  /**
   * @brief Type trait value to ZipContainers. Whether or not a type can be argument/return to 'semantic_zip'.
   *
   * @tparam type of which availability of 'Zipping::semantic_zip' is queried
   */
  template <typename VIEW>
  constexpr static bool has_semantic_zip_v = has_semantic_zip<VIEW>::value;
} // namespace Zipping

#endif
