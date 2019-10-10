/*
 * Copyright (C) 2019  CERN for the benefit of the LHCb collaboration
 *
 * This software is distributed under the terms of the GNU General Public
 * Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
 *
 * In applying this licence, CERN does not waive the privileges and immunities
 * granted to it by virtue of its status as an Intergovernmental Organization
 * or submit itself to any jurisdiction.
 */
#pragma once

#include "Event/Track_v2.h" // Chi2PerDof
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"
#include "Kernel/STLExtensions.h" // LHCb::span
#include "LHCbMath/SIMDWrapper.h"
#include "LHCbMath/Vec3.h"
#include "SOAExtensions/ZipUtils.h" // zip identifiers
#include <boost/container/small_vector.hpp>
#include <cstddef> // std::size_t
#include <vector>

namespace LHCb::Rec::PV {
  using TrackRef                        = std::size_t;
  const std::size_t default_vertex_size = 30;
  class PVs {
  private:
    std::vector<Gaudi::XYZPoint>                    m_positions;
    std::vector<Gaudi::SymMatrix3x3>                m_covMatrices;
    std::vector<LHCb::Event::v2::Track::Chi2PerDoF> m_chi2PerDoFs;

    std::vector<boost::container::small_vector<TrackRef, default_vertex_size>> m_fwdTracks;
    std::vector<boost::container::small_vector<TrackRef, default_vertex_size>> m_bkwTracks;
    std::vector<boost::container::small_vector<float, default_vertex_size>>    m_fwdWeights;
    std::vector<boost::container::small_vector<float, default_vertex_size>>    m_bkwWeights;

    Zipping::ZipFamilyNumber m_zipIdentifier{Zipping::generateZipIdentifier()};

    // initializers to enable default constructors - results in physically meaningless instances
    Zipping::ZipFamilyNumber m_fwdTracksIdentifier{Zipping::generateZipIdentifier()};
    Zipping::ZipFamilyNumber m_bkwTracksIdentifier{Zipping::generateZipIdentifier()};

  public:
    // construction/destruction
    //  - default construction of empty structure w/ auto-generated family number and meaningless, generated ancestor
    //  families
    //  - disabled copy construction (forced move)
    //  - default move
    //  - constructor with two family identifieres (initialize forward and backward families and auto-generate the own
    //  identifier // TODO: make harder to use wrong
    PVs() = default;
    PVs( Zipping::ZipFamilyNumber family ) : m_zipIdentifier( family ) {}
    PVs( Zipping::ZipFamilyNumber fwd, Zipping::ZipFamilyNumber bkw )
        : m_fwdTracksIdentifier{fwd}, m_bkwTracksIdentifier{bkw} {}
    PVs( PVs const& ) = delete;
    PVs( PVs&& )      = default;

    // TODO: understand
    PVs( Zipping::ZipFamilyNumber family, PVs const& /*unused*/ ) : m_zipIdentifier( family ) {}

    // container interfaces
    [[nodiscard]] std::size_t size() const { return m_positions.size(); }
    void                      reserve( std::size_t capacity ) {
      m_positions.reserve( capacity );
      m_covMatrices.reserve( capacity );
      m_chi2PerDoFs.reserve( capacity );
      m_fwdTracks.reserve( capacity );
      m_bkwTracks.reserve( capacity );
      m_fwdWeights.reserve( capacity );
      m_bkwWeights.reserve( capacity );
    }
    void resize( std::size_t capacity ) {
      m_positions.resize( capacity );
      m_covMatrices.resize( capacity );
      m_chi2PerDoFs.resize( capacity );
      m_fwdTracks.resize( capacity );
      m_bkwTracks.resize( capacity );
      m_fwdWeights.resize( capacity );
      m_bkwWeights.resize( capacity );
    }

    [[nodiscard]] Zipping::ZipFamilyNumber zipIdentifier() const { return m_zipIdentifier; }

    // fitting into current TrackBeamLineVertexFinderSoA
    void emplace_back( Gaudi::XYZPoint pos, Gaudi::SymMatrix3x3 poscov, LHCb::Event::v2::Track::Chi2PerDoF chi2perdof,
                       std::vector<std::pair<int, float>> tracks // from namespace { struct Vertex TODO: make signed
                                                                 // should not have a hard coded type here
    ) {
      // TODO:
      // push_back / emplace_back / move?
      m_positions.emplace_back( std::move( pos ) );
      m_covMatrices.emplace_back( std::move( poscov ) );
      m_chi2PerDoFs.emplace_back( std::move( chi2perdof ) );
      m_fwdTracks.emplace_back();
      m_fwdWeights.emplace_back();
      m_bkwTracks.emplace_back();
      m_bkwWeights.emplace_back();
      // TODO: write other than with for loop
      for ( auto track : tracks ) {
        if ( track.first >= 0 ) {
          m_fwdTracks.back().push_back( track.first );
          m_fwdWeights.back().push_back( track.second );
        } else {
          m_bkwTracks.back().push_back( ( -1 ) - track.first );
          m_bkwWeights.back().push_back( track.second );
        }
      }
    }

    // transposes internally
    template <typename dType, bool unwrap>
    [[nodiscard]] decltype( auto ) pos( int i ) const {
      if constexpr ( unwrap ) {
        return m_positions[i];
      } else {
        std::array<float, dType::size> tmpx;
        std::array<float, dType::size> tmpy;
        std::array<float, dType::size> tmpz;
        for ( std::size_t j = 0; j < dType::size; j++ ) {
          tmpx[j] = m_positions[i + j].x();
          tmpy[j] = m_positions[i + j].y();
          tmpz[j] = m_positions[i + j].z();
        }
        Vec3<typename dType::float_v> tmp{&tmpx[0], &tmpy[0], &tmpz[0]};
        return tmp;
      }
    }

    // TODO: experimental function
    template <typename dType, bool unwrap>
    [[nodiscard]] typename dType::float_v pos_x( int i ) const {
      if constexpr ( unwrap ) {
        return m_positions[i].x();
      } else {
        std::array<float, dType::size> tmp;
        for ( std::size_t j = 0; j < dType::size; j++ ) {
          // the assembly for this is absurdly optimal,
          // no loop, no copying through tmp to retval to return
          tmp[j] = m_positions[i + j].x();
        }
        return {&tmp[0]};
      }
    }
    template <typename dType, bool unwrap>
    [[nodiscard]] typename dType::float_v pos_y( int i ) const {
      if constexpr ( unwrap ) {
        return m_positions[i].y();
      } else {
        std::array<float, dType::size> tmp;
        for ( std::size_t j = 0; j < dType::size; j++ ) {
          // the assembly for this is absurdly optimal,
          // no loop, no copying through tmp to retval to return
          tmp[j] = m_positions[i + j].y();
        }
        return {&tmp[0]};
      }
    }
    template <typename dType, bool unwrap>
    [[nodiscard]] typename dType::float_v pos_z( int i ) const {
      if constexpr ( unwrap ) {
        return m_positions[i].z();
      } else {
        std::array<float, dType::size> tmp;
        for ( std::size_t j = 0; j < dType::size; j++ ) {
          // the assembly for this is absurdly optimal,
          // no loop, no copying through tmp to retval to return
          tmp[j] = m_positions[i + j].z();
        }
        return {&tmp[0]};
      }
    }
  };
} // namespace LHCb::Rec::PV
