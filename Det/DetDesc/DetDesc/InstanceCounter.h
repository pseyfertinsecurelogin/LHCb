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
#ifndef DETDESC_INSTANCECOUNTER_H
#define DETDESC_INSTANCECOUNTER_H 1
#include <atomic>

namespace Details {
  template <typename T>
  class InstanceCounter {
    static std::atomic<std::size_t> s_counter;

  public:
    InstanceCounter() { ++s_counter; }
    InstanceCounter( const InstanceCounter& ) { ++s_counter; }
    ~InstanceCounter() { --s_counter; }
    static std::size_t count() { return s_counter; }
  };

  template <typename T>
  std::atomic<std::size_t> InstanceCounter<T>::s_counter = {0};
} // namespace Details
#endif
