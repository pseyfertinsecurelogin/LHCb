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

#include "GaudiKernel/GenericMatrixTypes.h"
#include "GaudiKernel/GenericVectorTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"

#ifdef NDEBUG
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  define GSL_UNENFORCED_ON_CONTRACT_VIOLATION
#endif
#include "gsl/span"
#ifdef NDEBUG
#  pragma GCC diagnostic pop
#endif

namespace LHCb::Math {

  using gsl::span;

  namespace detail {
    template <typename T, unsigned int N>
    inline span<const T, N> to_span( const ROOT::Math::SVector<T, N>& x ) {
      return {x.Array(), N};
    }
    template <typename T, unsigned int N>
    inline span<T, N> to_span( ROOT::Math::SVector<T, N>& x ) {
      return {x.Array(), N};
    }

    template <typename T, unsigned int N>
    inline span<double, N*( N + 1 ) / 2> to_span( ROOT::Math::SMatrix<T, N, N, ROOT::Math::MatRepSym<T, N>>& x ) {
      return {x.Array(), N * ( N + 1 ) / 2};
    }
    template <typename T, unsigned int N>
    inline span<const double, N*( N + 1 ) / 2>
    to_span( const ROOT::Math::SMatrix<T, N, N, ROOT::Math::MatRepSym<T, N>>& x ) {
      return {x.Array(), N * ( N + 1 ) / 2};
    }

    template <typename T, unsigned int N, unsigned int M>
    inline span<double, N * M> to_span( ROOT::Math::SMatrix<T, N, M, ROOT::Math::MatRepStd<T, N, M>>& x ) {
      return {x.Array(), N * M};
    }
    template <typename T, unsigned int N, unsigned int M>
    inline span<const double, N * M> to_span( const ROOT::Math::SMatrix<T, N, M, ROOT::Math::MatRepStd<T, N, M>>& x ) {
      return {x.Array(), N * M};
    }

    void similarity_5_1( span<const double, 15> Ci, span<const double, 5> Fi, span<double, 1> Ti );
    void similarity_5_5( span<const double, 15> Ci, span<const double, 25> Fi, span<double, 15> Ti );
    void similarity_5_7( span<const double, 15> Ci, span<const double, 35> Fi, span<double, 28> Ti );
    void similarity_2_5( span<const double, 3> Ci, span<const double, 10> Fi, span<double, 15> Ti );

    bool average( span<const double, 5> X1, span<const double, 15> C1, span<const double, 5> X2,
                  span<const double, 15> C2, span<double, 5> X, span<double, 15> C );

    double filter( span<double, 5> X, span<double, 15> C, span<const double, 5> Xref, span<const double, 5> H,
                   double refResidual, double errorMeas2 );

  } // namespace detail

  // perform similarity transform --
  inline double Similarity( const Gaudi::Vector5& F, const Gaudi::SymMatrix5x5& origin ) {
    double target;
    detail::similarity_5_1( detail::to_span( origin ), detail::to_span( F ), {&target, 1} );
    return target;
  }
  inline double Similarity( const Gaudi::Matrix1x5& F, const Gaudi::SymMatrix5x5& origin ) {
    double target;
    detail::similarity_5_1( detail::to_span( origin ), detail::to_span( F ), {&target, 1} );
    return target;
  }

  // 'target' and 'origin' are NOT allowed to be the same object.
  // (actually, in the SSE3 and AVX versions it is possible, but not in
  // the generic version -- hence this could be a very confusing bug)
  inline void Similarity( const Gaudi::Matrix5x5&    F,      //
                          const Gaudi::SymMatrix5x5& origin, //
                          Gaudi::SymMatrix5x5&       target ) {
    detail::similarity_5_5( detail::to_span( origin ), detail::to_span( F ), detail::to_span( target ) );
  }

  inline Gaudi::SymMatrix5x5 Similarity( const Gaudi::Matrix5x5&    F, //
                                         const Gaudi::SymMatrix5x5& origin ) {
    Gaudi::SymMatrix5x5 target;
    Similarity( F, origin, target );
    return target; // rely on RVO to make this efficient...
  }

  // 'target' and 'origin' are NOT allowed to be the same object.
  // (actually, in the SSE3 and AVX versions it is possible, but not in
  // the generic version -- hence this could be a very confusing bug)
  inline void Similarity( const ROOT::Math::SMatrix<double, 7, 5>& F,      //
                          const Gaudi::SymMatrix5x5&               origin, //
                          Gaudi::SymMatrix7x7&                     target ) {
    detail::similarity_5_7( detail::to_span( origin ), detail::to_span( F ), detail::to_span( target ) );
  }

  inline Gaudi::SymMatrix7x7 Similarity( const ROOT::Math::SMatrix<double, 7, 5>& F,
                                         const Gaudi::SymMatrix5x5&               origin ) {
    Gaudi::SymMatrix7x7 target;
    Similarity( F, origin, target );
    return target; // rely on RVO to make this efficient...
  }

  // 'target' and 'origin' are NOT allowed to be the same object.
  inline void Similarity( const ROOT::Math::SMatrix<double, 5, 2>& F,      //
                          const Gaudi::SymMatrix2x2&               origin, //
                          Gaudi::SymMatrix5x5&                     target ) {
    detail::similarity_2_5( detail::to_span( origin ), detail::to_span( F ), detail::to_span( target ) );
  }

  inline Gaudi::SymMatrix5x5 Similarity( const ROOT::Math::SMatrix<double, 5, 2>& F,
                                         const Gaudi::SymMatrix2x2&               origin ) {
    Gaudi::SymMatrix5x5 target;
    Similarity( F, origin, target );
    return target; // rely on RVO to make this efficient...
  }

  inline bool Average( const Gaudi::Vector5&      X1, //
                       const Gaudi::SymMatrix5x5& C1, //
                       const Gaudi::Vector5&      X2, //
                       const Gaudi::SymMatrix5x5& C2, //
                       Gaudi::Vector5&            X,  //
                       Gaudi::SymMatrix5x5&       C ) {
    return detail::average( detail::to_span( X1 ), detail::to_span( C1 ), //
                            detail::to_span( X2 ), detail::to_span( C2 ), //
                            detail::to_span( X ), detail::to_span( C ) );
  }

  // compute a filter step in a Kalman filter
  // updates X and C in situ, and returns the chisquared
  inline double Filter( Gaudi::Vector5&         X,           //
                        Gaudi::SymMatrix5x5&    C,           //
                        const Gaudi::Vector5&   Xref,        //
                        const Gaudi::Matrix1x5& H,           //
                        double                  refResidual, //
                        double                  errorMeas2 ) {
    return detail::filter( detail::to_span( X ), detail::to_span( C ),    //
                           detail::to_span( Xref ), detail::to_span( H ), //
                           refResidual, errorMeas2 );
  }

} // namespace LHCb::Math
