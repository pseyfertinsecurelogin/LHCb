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
#include "LHCbMath/Similarity.h"
#include "LHCbMath/CPUDispatch.h"
#include <type_traits>

namespace {
  template <typename Vtbl, typename Trampoline, typename... Args>
  decltype( auto ) dispatch_fn( const Vtbl& vtbl, Trampoline& t, Args&&... args ) {
    t = LHCb::CPU::dispatch( vtbl );
    return ( *t )( std::forward<Args>( args )... );
  }
} // namespace

namespace LHCb {
  namespace Math {
    namespace detail {

      // TODO: replace by (static?) functor (instance)s...
      //       (maybe templated, with multiple explicit specializations & tag dispatch???)
      //
      //       or can we replace the 'explicit' vtbl with inheritance? After all,
      //       this is 'just' runtime-polymorphism... but then the 'global'
      //       instance in the trampoline functions should be updated somehow.
      //       That way, they get all shunted simultaneously on the first invocation
      //       of any individual one...
      //
      using gsl::span;

      namespace avx2 {
        extern void   similarity_5_1( span<const double, 15> Ci, span<const double, 5> Fi, span<double, 1> Ti );
        extern void   similarity_5_5( span<const double, 15> Ci, span<const double, 25> Fi, span<double, 15> Ti );
        extern void   similarity_5_7( span<const double, 15> Ci, span<const double, 35> Fi, span<double, 28> Ti );
        extern bool   average( span<const double, 5> X1, span<const double, 15> C1, span<const double, 5> X2,
                               span<const double, 15> C2, span<double, 5> X, span<double, 15> C );
        extern double filter( span<double, 5> X, span<double, 15> C, span<const double, 5> Xref,
                              span<const double, 5> H, double refResidual, double errorMeas2 );
      } // namespace avx2
      namespace avx {
        extern void   similarity_5_1( span<const double, 15> Ci, span<const double, 5> Fi, span<double, 1> Ti );
        extern void   similarity_5_5( span<const double, 15> Ci, span<const double, 25> Fi, span<double, 15> Ti );
        extern void   similarity_5_7( span<const double, 15> Ci, span<const double, 35> Fi, span<double, 28> Ti );
        extern bool   average( span<const double, 5> X1, span<const double, 15> C1, span<const double, 5> X2,
                               span<const double, 15> C2, span<double, 5> X, span<double, 15> C );
        extern double filter( span<double, 5> X, span<double, 15> C, span<const double, 5> Xref,
                              span<const double, 5> H, double refResidual, double errorMeas2 );
      } // namespace avx
      namespace sse3 {
        extern void similarity_5_1( span<const double, 15> Ci, span<const double, 5> Fi, span<double, 1> Ti );
        extern void similarity_5_5( span<const double, 15> Ci, span<const double, 25> Fi, span<double, 15> Ti );
        extern void similarity_5_7( span<const double, 15> Ci, span<const double, 35> Fi, span<double, 28> Ti );
      } // namespace sse3
      namespace generic {
        extern void   similarity_5_1( span<const double, 15> Ci, span<const double, 5> Fi, span<double, 1> Ti );
        extern void   similarity_5_5( span<const double, 15> Ci, span<const double, 25> Fi, span<double, 15> Ti );
        extern void   similarity_5_7( span<const double, 15> Ci, span<const double, 35> Fi, span<double, 28> Ti );
        extern void   similarity_2_5( span<const double, 3> Ci, span<const double, 10> Fi, span<double, 15> Ti );
        extern bool   average( span<const double, 5> X1, span<const double, 15> C1, span<const double, 5> X2,
                               span<const double, 15> C2, span<double, 5> X, span<double, 15> C );
        extern double filter( span<double, 5> X, span<double, 15> C, span<const double, 5> Xref,
                              span<const double, 5> H, double refResidual, double errorMeas2 );
      } // namespace generic

      namespace dispatch {

        void similarity_5_1( span<const double, 15> Ci, span<const double, 5> Fi, span<double, 1> ti ) {
          auto vtbl = {std::pair{CPU::AVX2, &avx2::similarity_5_1}, std::pair{CPU::AVX, &avx::similarity_5_1},
                       std::pair{CPU::SSE3, &sse3::similarity_5_1}, std::pair{CPU::GENERIC, &generic::similarity_5_1}};
          dispatch_fn( vtbl, LHCb::Math::detail::similarity_5_1, Ci, Fi, ti );
        }

        void similarity_5_5( span<const double, 15> Ci, span<const double, 25> Fi, span<double, 15> ti ) {
          auto vtbl = {std::pair{CPU::AVX2, &avx2::similarity_5_5}, std::pair{CPU::AVX, &avx::similarity_5_5},
                       std::pair{CPU::SSE3, &sse3::similarity_5_5}, std::pair{CPU::GENERIC, &generic::similarity_5_5}};
          dispatch_fn( vtbl, LHCb::Math::detail::similarity_5_5, Ci, Fi, ti );
        }

        void similarity_5_7( span<const double, 15> Ci, span<const double, 35> Fi, span<double, 28> ti ) {
          auto vtbl = {std::pair{CPU::AVX2, &avx2::similarity_5_7}, std::pair{CPU::AVX, &avx::similarity_5_7},
                       std::pair{CPU::SSE3, &sse3::similarity_5_7}, std::pair{CPU::GENERIC, &generic::similarity_5_7}};
          dispatch_fn( vtbl, LHCb::Math::detail::similarity_5_7, Ci, Fi, ti );
        }

        void similarity_2_5( span<const double, 3> Ci, span<const double, 10> Fi, span<double, 15> ti ) {
          auto vtbl = {std::pair{CPU::GENERIC, &generic::similarity_2_5}};
          dispatch_fn( vtbl, LHCb::Math::detail::similarity_2_5, Ci, Fi, ti );
        }

        bool average( span<const double, 5> X1, span<const double, 15> C1, span<const double, 5> X2,
                      span<const double, 15> C2, span<double, 5> X, span<double, 15> C ) {
          auto vtbl = {std::pair{CPU::AVX2, &avx2::average}, std::pair{CPU::AVX, &avx::average},
                       std::pair{CPU::GENERIC, &generic::average}};
          return dispatch_fn( vtbl, LHCb::Math::detail::average, X1, C1, X2, C2, X, C );
        }

        double filter( span<double, 5> X, span<double, 15> C, span<const double, 5> Xref, span<const double, 5> H,
                       double refResidual, double errorMeas2 ) {
          auto vtbl = {std::pair{CPU::AVX2, &avx2::filter}, std::pair{CPU::AVX, &avx::filter},
                       std::pair{CPU::GENERIC, &generic::filter}};
          return dispatch_fn( vtbl, LHCb::Math::detail::filter, X, C, Xref, H, refResidual, errorMeas2 );
        }

      } // namespace dispatch

      similarity_t<5, 1> similarity_5_1 = &dispatch::similarity_5_1;
      similarity_t<5, 5> similarity_5_5 = &dispatch::similarity_5_5;
      similarity_t<5, 7> similarity_5_7 = &dispatch::similarity_5_7;
      similarity_t<2, 5> similarity_2_5 = &dispatch::similarity_2_5;

      average_t average = &dispatch::average;
      filter_t  filter  = &dispatch::filter;

    } // namespace detail
  }   // namespace Math
} // namespace LHCb
