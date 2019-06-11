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

#include "LHCbMath/SIMDWrapper.h"
#include "LHCbMath/Vec3.h"

/**
 * Track data for exchanges between FT and Fit
 * 
 * @author: Arthur Hennequin
 */

namespace LHCb::Event {
  class TracksFT {
    constexpr static int max_tracks = align_size(1024);
    constexpr static int max_hits   = 12;
  public:
    TracksFT() {
      const size_t size = max_tracks * (max_hits + 4);
      m_data = static_cast<int*>(std::aligned_alloc(64, size*sizeof(int)));
    }

    TracksFT(const TracksFT&) = delete;

    TracksFT(TracksFT&& other) {
      m_data = other.m_data;
      other.m_data = nullptr;
      m_size = other.m_size;
    }

    inline int size() const { return m_size; }
    inline int& size() { return m_size; }

    // Index in TracksVP container of the track's ancestor
    SOA_ACCESSOR(trackVP, m_data)

    // Index in TracksUT container of the track's ancestor
    SOA_ACCESSOR(trackUT, &m_data[max_tracks])

    // QoP estimate from FT
    SOA_ACCESSOR(stateQoP, (float*)&m_data[2*max_tracks])

    // Hits (for now LHCBid) in FT
    // TODO: replace LHCbids by index in FT hit container
    SOA_ACCESSOR(nHits, &m_data[3*max_tracks])
    SOA_ACCESSOR_VAR(hit, &m_data[(hit+4)*max_tracks], int hit)

    /// Retrieve the momentum
    template<typename T>
    inline T p(int t) const {
      T qop = stateQoP<T>(t);
      return 1 / qop;
    }

    template<typename simd, typename maskT>
    inline void copy_back(const TracksFT& from, int at, maskT mask) {
      using intT = typename simd::int_v;
      for (int i=0 ; i<max_hits + 4 ; i++) {
        intT(&from.m_data[i*max_tracks+at]).compressstore(mask, &m_data[i*max_tracks+m_size]);
      }
      m_size += simd::popcount(mask);
    }

    ~TracksFT() {
      std::free(m_data);
    }

  private:
    alignas(64) int* m_data;
    int m_size = 0;
  };
}
