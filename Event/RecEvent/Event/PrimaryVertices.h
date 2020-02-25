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
#include "Event/PrZip.h"
#include "Event/Track_v2.h" // Chi2PerDof
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h" // IWYU pragma: export
#include "Kernel/EventLocalAllocator.h"
#include "Kernel/STLExtensions.h" // LHCb::span
#include "LHCbMath/SIMDWrapper.h"
#include "LHCbMath/Vec3.h"
#include "SOAExtensions/ZipUtils.h" // zip identifiers
#include <boost/container/small_vector.hpp>
#include <cstddef> // std::size_t
#include <vector>

namespace LHCb::Rec::PV {
  class PrimaryVertices {
  public:
    struct Vertex {
      Gaudi::XYZPoint                    m_position;
      Gaudi::SymMatrix3x3                m_covMatrix;
      LHCb::Event::v2::Track::Chi2PerDoF m_chi2PerDoF;
    };

  public:
    using allocator_type = LHCb::Allocators::EventLocal<Vertex>;

    std::vector<Vertex, allocator_type> m_vertices;

    Zipping::ZipFamilyNumber m_zipIdentifier{Zipping::generateZipIdentifier()};

  public:
    // construction/destruction
    //  - default construction of empty structure w/ auto-generated family number and meaningless, generated ancestor
    //  families
    //  - disabled copy construction (forced move)
    //  - default move
    //  - constructor with two family identifieres (initialize forward and backward families and auto-generate the own
    //  identifier // TODO: make harder to use wrong
    PrimaryVertices() = default;
    PrimaryVertices( allocator_type alloc ) : m_vertices{alloc} {}
    PrimaryVertices( Zipping::ZipFamilyNumber family, allocator_type alloc = {} )
        : m_vertices{alloc}, m_zipIdentifier( family ) {}
    PrimaryVertices( PrimaryVertices const& ) = delete;
    PrimaryVertices( PrimaryVertices&& )      = default;

    // TODO: copied from example, intent unclear
    PrimaryVertices( Zipping::ZipFamilyNumber family, PrimaryVertices const& /*unused*/ ) : m_zipIdentifier( family ) {}

    // container interfaces
    [[nodiscard]] std::size_t size() const { return m_vertices.size(); }
    void                      reserve( std::size_t capacity ) { m_vertices.reserve( align_size( capacity ) ); }
    void                      resize( std::size_t capacity ) {
      reserve( capacity );
      m_vertices.resize( capacity );
    }

    template <typename... T>
    decltype( auto ) emplace_back( T&&... t ) {
      reserve( size() + 1 );
      return m_vertices.emplace_back( std::forward<T>( t )... );
    }

    /**
     * @brief helper for filtering containers. copy&paste from PrMuonPIDs.h
     *
     * @param from   origin of the copied PVs
     * @param at     at which position in the source container to start copying
     * @param mask   which of the source PVs to copy
     * @tparam dType SIMDWrapper that is used (specifies stride length
     * @tparam Mask  type of mask (auto deduced)
     *
     * since the internal data structure is not SIMDified, this method does
     * not exploit the full potential of our SIMDWrappers. The
     * SIMDWrapper-compatible interface is just provided for caller
     * convenience.
     *
     */
    template <typename dType, typename Mask>
    void copy_back( PrimaryVertices const& from, int at, Mask mask ) {
      using details::popcount;
      auto to_add   = popcount( mask );
      auto old_size = size();
      reserve( old_size + to_add );

      for ( std::size_t i = 0; i < dType::size; ++i ) {
        if ( testbit( mask, i ) ) { m_vertices.push_back( from[at + i] ); }
      }
    }

    auto& back() { return m_vertices.back(); }

    auto back() const { return m_vertices.back(); }

    void pop_back() { m_vertices.pop_back(); }

    [[nodiscard]] Zipping::ZipFamilyNumber zipIdentifier() const { return m_zipIdentifier; }

    // no filling method
    // should be implemented by creating algorithm where the algorithm-internal structures are known

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
    [[nodiscard]] decltype( auto ) nTracks( const std::size_t i ) const {
      if constexpr ( unwrap ) {
        auto chi2perdof = m_vertices[i].m_chi2PerDoF;
        return ( chi2perdof.nDoF + 3 ) / 2;
      } else {
        std::array<int, dType::size> tmp_i;
        for ( std::size_t j = 0; j < dType::size; j++ ) { tmp_i[j] = m_vertices[i + j].m_chi2PerDoF.nDoF; }
        using int_v = typename dType::int_v;
        int_v ndofs{&tmp_i[0]};
        // return (ndofs + int_v{3})/int_v{2};
        return ( ndofs + int_v{3} ) >> int_v{1};
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

    template <typename dType, bool unwrap = true>
    [[nodiscard]] decltype( auto ) cov( const std::size_t chunkposition, int row, int col ) const {
      if constexpr ( unwrap ) {
        return m_vertices[chunkposition].m_covMatrix.At( row, col );
      } else {
        using float_v = typename dType::float_v;
        std::array<float, dType::size> tmp;
        // should not be needed once https://gitlab.cern.ch/lhcb/LHCb/issues/77 is fixed
        if ( !( chunkposition + dType::size - 1 < m_vertices.capacity() ) ) {
          throw GaudiException{
              "Insufficient space allocated during PrimaryVertices creation to perform SIMD operations",
              "PrimaryVertices.h", StatusCode::FAILURE};
        }
        for ( std::size_t simdelement = 0; simdelement < dType::size && chunkposition + simdelement < m_vertices.size();
              simdelement++ ) {
          tmp[simdelement] = std::as_const( m_vertices[chunkposition + simdelement].m_covMatrix ).At( row, col );
        }
        return float_v{&tmp[0]};
      }
    }

    // transposes internally for SIMD access
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

    template <typename dType, bool unwrap>
    [[nodiscard]] typename std::conditional<unwrap, float, typename dType::float_v>::type
    pos_x( const std::size_t i ) const {
      if constexpr ( unwrap ) {
        return m_vertices[i].m_position.x();
      } else {
        std::array<float, dType::size> tmp;
        for ( std::size_t j = 0; j < dType::size; j++ ) { tmp[j] = m_vertices[i + j].m_position.x(); }
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
        for ( std::size_t j = 0; j < dType::size; j++ ) { tmp[j] = m_vertices[i + j].m_position.y(); }
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
        // should not be needed once https://gitlab.cern.ch/lhcb/LHCb/issues/77 is fixed
        if ( !( i + dType::size - 1 < m_vertices.capacity() ) ) {
          throw GaudiException{
              "Insufficient space allocated during PrimaryVertices creation to perform SIMD operations",
              "PrimaryVertices.h", StatusCode::FAILURE};
        }
        for ( std::size_t j = 0; j < dType::size && i + j < m_vertices.size(); j++ ) {
          tmp[j] = m_vertices[i + j].m_position.z();
        }
        return {&tmp[0]};
      }
    }
  };
} // namespace LHCb::Rec::PV

namespace LHCb::Rec::PV {
  // copy&pasted
  DECLARE_PROXY( Proxy ) {
    PROXY_METHODS( Proxy, dType, unwrap, PrimaryVertices, m_pvs );

    [[nodiscard]] auto x() const { return this->m_pvs->template pos_x<dType, unwrap>( this->offset() ); }
    [[nodiscard]] auto y() const { return this->m_pvs->template pos_y<dType, unwrap>( this->offset() ); }
    [[nodiscard]] auto z() const { return this->m_pvs->template pos_z<dType, unwrap>( this->offset() ); }
    [[nodiscard]] auto pos() const { return this->m_pvs->template pos<dType, unwrap>( this->offset() ); }
    [[nodiscard]] auto position() const { return this->m_pvs->template pos<dType, unwrap>( this->offset() ); }
    [[nodiscard]] auto covMatrix() const { return this->m_pvs->template cov<dType, unwrap>( this->offset() ); }
    [[nodiscard]] auto covMatrixElement( int row, int col ) const {
      return this->m_pvs->template cov<dType, unwrap>( this->offset(), row, col );
    }
    [[nodiscard]] auto chi2perdof() const { return this->m_pvs->template chi2perdof<dType, unwrap>( this->offset() ); }
  };
} // namespace LHCb::Rec::PV

REGISTER_PROXY( LHCb::Rec::PV::PrimaryVertices, LHCb::Rec::PV::Proxy );
REGISTER_HEADER( LHCb::Rec::PV::PrimaryVertices, "Event/PrimaryVertices.h" );

namespace LHCb::Rec::Iterable::PV {
  using PrimaryVertices = LHCb::Pr::zip_t<LHCb::Rec::PV::PrimaryVertices>;
} // namespace LHCb::Rec::Iterable::PV

namespace LHCb::Rec::Iterable::Scalar::PV {
  using PrimaryVertices = LHCb::Pr::unwrapped_zip_t<LHCb::Rec::PV::PrimaryVertices>;
} // namespace LHCb::Rec::Iterable::Scalar::PV
