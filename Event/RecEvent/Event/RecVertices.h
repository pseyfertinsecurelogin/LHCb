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

#include "Event/Chi2PerDoF.h"
#include "Event/Track_v2.h" // Chi2PerDof
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"
#include "Kernel/STLExtensions.h" // LHCb::span
#include "LHCbMath/SIMDWrapper.h"
#include "LHCbMath/Vec3.h"
#include "SOAContainer/SOAField.h"  // for FieldBase, SOAFIELD_TRIVIAL
#include "SOAContainer/SOASkin.h"   // for SOASkinCreatorSimple<>::type
#include "SOAExtensions/ZipUtils.h" // zip identifiers
#include <boost/container/small_vector.hpp>
#include <cstddef> // std::size_t
#include <vector>

namespace LHCb::Rec::PV {
  using TrackRef = std::size_t;
  using weight_t = float;
  SOAFIELD_TRIVIAL( ref_field, index, LHCb::Rec::PV::TrackRef );
  SOAFIELD_TRIVIAL( wei_field, weight, weight_t );
  SOASKIN_TRIVIAL( WeightedTrack, ref_field, wei_field ); // for reading convenience
  const std::size_t default_vertex_size = 60;
  // const std::size_t default_vertex_size = 0;
  // const std::size_t default_vertex_size = 100;
  class PVs {
  public:
    struct Vertex {
      Gaudi::XYZPoint                    m_position;
      Gaudi::SymMatrix3x3                m_covMatrix;
      LHCb::Event::v2::Track::Chi2PerDoF m_chi2PerDoF;
    };

  private:
    std::vector<Vertex> m_vertices;

    std::vector<boost::container::small_vector<TrackRef, default_vertex_size>> m_fwdTracks;
    std::vector<boost::container::small_vector<TrackRef, default_vertex_size>> m_bkwTracks;
    std::vector<boost::container::small_vector<weight_t, default_vertex_size>> m_fwdWeights;
    std::vector<boost::container::small_vector<weight_t, default_vertex_size>> m_bkwWeights;

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

    [[nodiscard]] const boost::container::small_vector<weight_t, default_vertex_size>& fwdWeights( const int i ) const {
      return m_fwdWeights[i];
    }
    [[nodiscard]] const boost::container::small_vector<weight_t, default_vertex_size>& bkwWeights( const int i ) const {
      return m_bkwWeights[i];
    }
    [[nodiscard]] const boost::container::small_vector<TrackRef, default_vertex_size>& fwdTracks( const int i ) const {
      return m_fwdTracks[i];
    }
    [[nodiscard]] const boost::container::small_vector<TrackRef, default_vertex_size>& bkwTracks( const int i ) const {
      return m_bkwTracks[i];
    }

    [[nodiscard]] Zipping::ZipFamilyNumber fwdZipIdentifier() const { return m_fwdTracksIdentifier; }
    [[nodiscard]] Zipping::ZipFamilyNumber bkwZipIdentifier() const { return m_bkwTracksIdentifier; }

    // container interfaces
    [[nodiscard]] std::size_t size() const { return m_vertices.size(); }
    void                      reserve( std::size_t capacity ) {
      m_vertices.reserve( capacity );
      m_fwdTracks.reserve( capacity );
      m_bkwTracks.reserve( capacity );
      m_fwdWeights.reserve( capacity );
      m_bkwWeights.reserve( capacity );
    }
    void resize( std::size_t capacity ) {
      m_vertices.resize( capacity );
      m_fwdTracks.resize( capacity );
      m_bkwTracks.resize( capacity );
      m_fwdWeights.resize( capacity );
      m_bkwWeights.resize( capacity );
    }

    [[nodiscard]] Zipping::ZipFamilyNumber zipIdentifier() const { return m_zipIdentifier; }

    // fitting into current TrackBeamLineVertexFinderSoA
    // @todo: tracks are taken as non-const reference!
    void emplace_back( Gaudi::XYZPoint pos, Gaudi::SymMatrix3x3 poscov, LHCb::Event::v2::Track::Chi2PerDoF chi2perdof,
                       std::vector<std::pair<int, weight_t>>& tracks // from namespace { struct Vertex TODO: make signed
                                                                     // should not have a hard coded type here
    ) {
      // TODO:
      // push_back / emplace_back / move?
      // google style guide says push_back
      m_vertices.emplace_back( Vertex{pos, poscov, chi2perdof} );
      m_fwdTracks.emplace_back();
      m_fwdWeights.emplace_back();
      m_bkwTracks.emplace_back();
      m_bkwWeights.emplace_back();
      // TODO: write other than with for loop
      // NB: tried application of std::partition to pre-sort. Lead to regression in runtime.
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

    template <typename dType = SIMDWrapper::best::types, bool unwrap = true>
    [[nodiscard]] decltype( auto ) chi2perdof( const std::size_t i ) const {
      if constexpr ( unwrap ) {
        return m_vertices[i].m_chi2PerDoF;
      } else {
        std::array<float, dType::size> tmp_f;
        std::array<int, dType::size>   tmp_i;
        for ( std::size_t j = 0; j < dType::size; j++ ) {
          tmp_f[j] = m_vertices[i + j].m_chi2PerDoF.chi2PerDoF;
          tmp_i[j] = m_vertices[i + j].m_chi2PerDoF.nDoF;
        }
        return LHCb::Rec::Chi2PerDoF<dType>{typename dType::float_v{&tmp_f[0]}, typename dType::int_v{&tmp_i[0]}};
      }
    }

    template <typename dType, bool unwrap = true>
    [[nodiscard]] decltype( auto ) cov( const std::size_t chunkposition ) const {
      if constexpr ( unwrap ) {
        return m_vertices[chunkposition].m_covMatrix;
      } else {
        using float_v = typename dType::float_v;
        ROOT::Math::SMatrix<float_v, 3, 3, ROOT::Math::MatRepSym<float_v, 3>> retval{ROOT::Math::SMatrixNoInit{}};
        for ( std::size_t i = 0; i < 3; i++ ) {
          for ( std::size_t j = 0; j < 3; j++ ) {
            std::array<float, dType::size> tmp;
            for ( std::size_t simdelement = 0; simdelement < dType::size; simdelement++ ) {
              tmp[simdelement] = m_vertices[chunkposition + simdelement].m_covMatrix.At( i, j );
            }
            retval.At( i, j ) = float_v{&tmp[0]};
          }
        }
        return retval;
      }
    }

    // transposes internally
    template <typename dType, bool unwrap>
    [[nodiscard]] decltype( auto ) pos( const std::size_t i ) const {
      if constexpr ( unwrap ) {
        return m_vertices[i].m_position;
      } else {
        std::array<float, dType::size> tmpx;
        std::array<float, dType::size> tmpy;
        std::array<float, dType::size> tmpz;
        for ( std::size_t j = 0; j < dType::size; j++ ) {
          tmpx[j] = m_vertices[i + j].m_position.x();
          tmpy[j] = m_vertices[i + j].m_position.y();
          tmpz[j] = m_vertices[i + j].m_position.z();
        }
        Vec3<typename dType::float_v> tmp{&tmpx[0], &tmpy[0], &tmpz[0]};
        return tmp;
      }
    }

    // TODO: experimental function
    template <typename dType, bool unwrap>
    [[nodiscard]] typename std::conditional<unwrap, float, typename dType::float_v>::type
    pos_x( const std::size_t i ) const {
      if constexpr ( unwrap ) {
        return m_vertices[i].m_position.x();
      } else {
        std::array<float, dType::size> tmp;
        for ( std::size_t j = 0; j < dType::size; j++ ) {
          // the assembly for this is absurdly optimal,
          // no loop, no copying through tmp to retval to return
          tmp[j] = m_vertices[i + j].m_position.x();
        }
        return {&tmp[0]};
      }
    }
    template <typename dType, bool unwrap>
    [[nodiscard]] typename std::conditional<unwrap, float, typename dType::float_v>::type
    pos_y( const std::size_t i ) const {
      if constexpr ( unwrap ) {
        return m_vertices[i].m_position.y();
      } else {
        std::array<float, dType::size> tmp;
        for ( std::size_t j = 0; j < dType::size; j++ ) {
          // the assembly for this is absurdly optimal,
          // no loop, no copying through tmp to retval to return
          tmp[j] = m_vertices[i + j].m_position.y();
        }
        return {&tmp[0]};
      }
    }
    template <typename dType, bool unwrap>
    [[nodiscard]] typename std::conditional<unwrap, float, typename dType::float_v>::type
    pos_z( const std::size_t i ) const {
      if constexpr ( unwrap ) {
        return m_vertices[i].m_position.z();
      } else {
        std::array<float, dType::size> tmp;
        for ( std::size_t j = 0; j < dType::size; j++ ) {
          // the assembly for this is absurdly optimal,
          // no loop, no copying through tmp to retval to return
          tmp[j] = m_vertices[i + j].m_position.z();
        }
        return {&tmp[0]};
      }
    }
  };
} // namespace LHCb::Rec::PV
