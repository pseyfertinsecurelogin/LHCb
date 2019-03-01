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
template <typename CONTAINER>
class ZipContainer;

template <typename T>
struct has_semantic_zip {
  bool value = false;
};

template <typename VIEW>
struct has_semantic_zip<ZipContainer<VIEW>> {
  static constexpr bool value = SOA::Utils::is_view<std::decay_t<std::decay_t<VIEW>>>::value;
};

template <typename VIEW>
constexpr static bool has_semantic_zip_v = has_semantic_zip<VIEW>::value;

#endif
