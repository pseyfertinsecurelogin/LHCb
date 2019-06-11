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
 * Track data after the Kalman fit
 * 
 * @author: Arthur Hennequin
 */

namespace LHCb::Event {
  class TracksFit {
    constexpr static int max_tracks = align_size(1024);
  public:
    TracksFit() {
      const size_t size = max_tracks * 15;
      m_data = static_cast<int*>(std::aligned_alloc(64, size*sizeof(int)));
    }

    TracksFit(const TracksFit&) = delete;

    TracksFit(TracksFit&& other) {
      m_data = other.m_data;
      other.m_data = nullptr;
      m_size = other.m_size;
    }

    inline int size() const { return m_size; }
    inline int& size() { return m_size; }

    // Index in TracksFT container of the track's ancestor
    SOA_ACCESSOR(trackFT, m_data)

    // QoP estimate from FT
    SOA_ACCESSOR(QoP, (float*)&m_data[max_tracks])

    // Closest to beam state after after fit
    VEC3_SOA_ACCESSOR(beamStatePos,
                      (float*)&m_data[2*max_tracks],
                      (float*)&m_data[3*max_tracks],
                      (float*)&m_data[4*max_tracks])

    VEC3_XY_SOA_ACCESSOR(beamStateDir,
                         (float*)&m_data[5*max_tracks],
                         (float*)&m_data[6*max_tracks],
                         1.f)

    // Chi2 from fit
    SOA_ACCESSOR(chi2, (float*)&m_data[7*max_tracks])

    // Chi2 degrees of freedom
    SOA_ACCESSOR(chi2nDof, &m_data[8*max_tracks])

    // Covariances matrix for X and Y (can be merged if wx=wy)
    VEC3_SOA_ACCESSOR(covX,
                      (float*)&m_data[ 9*max_tracks],
                      (float*)&m_data[10*max_tracks],
                      (float*)&m_data[11*max_tracks])

    VEC3_SOA_ACCESSOR(covY,
                      (float*)&m_data[12*max_tracks],
                      (float*)&m_data[13*max_tracks],
                      (float*)&m_data[14*max_tracks])

    /// Retrieve the momentum
    template<typename T>
    inline T p(int t) const {
      T qop = QoP<T>(t);
      return 1 / qop;
    }

    template<typename simd, typename maskT>
    inline void copy_back(const TracksFit& from, int at, maskT mask) {
      using intT = typename simd::int_v;
      for (int i=0 ; i<15 ; i++) {
        intT(&from.m_data[i*max_tracks+at]).compressstore(mask, &m_data[i*max_tracks+m_size]);
      }
      m_size += simd::popcount(mask);
    }

    ~TracksFit() {
      std::free(m_data);
    }

  private:
    alignas(64) int* m_data;
    int m_size = 0;
  };
}
