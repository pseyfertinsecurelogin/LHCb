/*****************************************************************************\
* (c) Copyright 2020 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

#include "SIMDWrapper.h"
#include "Utils.h"
#include <cassert>

namespace LHCb::LinAlg {

  // fwd declarations
  template <typename T, int N, int M>
  struct Mat;
  template <typename T, int N>
  struct MatSym;
  template <typename T, int N>
  struct Vec;

  template <typename T>
  constexpr static bool is_mat_v = false;

  template <typename T, int N, int M>
  constexpr static bool is_mat_v<Mat<T, N, M>> = true;

  template <typename T>
  constexpr static bool is_sym_mat_v = false;

  template <typename T, int N>
  constexpr static bool is_sym_mat_v<MatSym<T, N>> = true;

  template <typename T>
  constexpr static bool is_vec_v = false;

  template <typename T, int N>
  constexpr static bool is_vec_v<Vec<T, N>> = true;

  template <typename F>
  constexpr auto initialize_with_zeros() {
    static_assert( is_mat_v<F> or is_sym_mat_v<F> or is_vec_v<F> );
    F out{};
    out.m.fill( 0. );
    return out;
  }

  // Generic Vector object
  /** @class Vec
   *  implementation of an N-dim Vector of type T
   */
  template <typename T, int N>
  struct Vec {
    static constexpr auto size = N;
    using value_type           = T;
    std::array<T, N> m{};

    /** get a val from the vector
     *  @param i index
     */
    [[gnu::pure]] const T& operator()( int i ) const { return m[i]; }

    /** get a val from the vector
     *  @param i index
     */
    T& operator()( int i ) { return m[i]; }

    /// elementwise comparison ==
    [[gnu::pure]] friend bool operator==( Vec<T, N> const& lhs, Vec<T, N> const& rhs ) { return lhs.m == rhs.m; }
    /// elementwise comparison !=
    [[gnu::pure]] friend bool operator!=( Vec<T, N> const& lhs, Vec<T, N> const& rhs ) { return not( lhs == rhs ); }

    /// elementwise addition
    [[gnu::pure]] friend Vec<T, N> operator+( const Vec<T, N>& A, const Vec<T, N>& B ) {
      Vec<T, N> C;
      Utils::unwind<0, N>( [&]( auto i ) { C.m[i] = A.m[i] + B.m[i]; } );
      return C;
    }
    /// elementwise substraction
    [[gnu::pure]] friend Vec<T, N> operator-( const Vec<T, N>& A, const Vec<T, N>& B ) {
      Vec<T, N> C;
      Utils::unwind<0, N>( [&]( auto i ) { C.m[i] = A.m[i] - B.m[i]; } );
      return C;
    }
    /// elementwise multiplication
    [[gnu::pure]] friend Vec<T, N> operator*( const Vec<T, N>& A, const Vec<T, N>& B ) {
      Vec<T, N> C;
      Utils::unwind<0, N>( [&]( auto i ) { C.m[i] = A.m[i] * B.m[i]; } );
      return C;
    }
    /// elementwise multiplication with a value
    [[gnu::pure]] friend Vec<T, N> operator*( const Vec<T, N>& A, const T& B ) {
      Vec<T, N> C;
      Utils::unwind<0, N>( [&]( auto i ) { C.m[i] = A.m[i] * B; } );
      return C;
    }
    /// dot product
    [[gnu::pure]] T dot( const Vec<T, N>& B ) const {
      return Utils::unwind_sum<0, N>( [&]( auto i ) { return m[i] * B.m[i]; } );
    }
    /// transpose
    [[gnu::pure]] Mat<T, 1, N> transpose() const {
      Mat<T, 1, N> M;
      Utils::unwind<0, N>( [&]( auto i ) { M( 0, i ) = m[i]; } );
      return M;
    }
    /// operator<< for printout of the vector
    friend std::ostream& operator<<( std::ostream& o, const Vec<T, N>& v ) {
      o << "[";
      Utils::unwind<0, N - 1>( [&]( auto _n ) { o << v( _n ) << ", "; } );
      o << v( N - 1 ) << ']';
      return o;
    }

    /// if the dimension of the vector is 1, you can cast it to the single value it holds
    [[gnu::pure]] T cast_to_value() {
      static_assert( N == 1, "Vec with dimension > 1 cannot be cast to a value" );
      return m[0];
    }

    /// get a smaller vector of type SubType from the original vector, starting at index Start
    template <typename SubType, int Start>
    SubType sub() const {
      static_assert( is_vec_v<SubType>, "SubType needs to be of type Vec" );
      SubType out{};
      Utils::unwind<0, SubType::size>( [&]( auto i ) { out( i ) = ( *this )( i + Start ); } );
      return out;
    }
  };

  template <typename First, typename... Nexts, std::enable_if_t<( std::is_same_v<First, Nexts> && ... ), int> = 0>
  Vec( First, Nexts... )->Vec<First, sizeof...( Nexts ) + 1>;

  // Generic Matrix object
  /** @class Matrix
   *  0, 1, 2
   *  3, 4, 5
   *  6, 7, 8
   *  implementation of an N-dim Vector of type T
   */
  template <typename T, int N /* row */, int M /* column */ = N>
  struct Mat {
    static constexpr auto n_rows = N;
    static constexpr auto n_cols = M;
    using value_type             = T;

    std::array<T, N * M> m{};

    /// elementwise comparison ==
    [[gnu::pure]] friend bool operator==( Mat<T, N, M> const& lhs, Mat<T, N, M> const& rhs ) { return lhs.m == rhs.m; }
    /// elementwise comparison !=
    [[gnu::pure]] friend bool operator!=( Mat<T, N, M> const& lhs, Mat<T, N, M> const& rhs ) {
      return not( lhs == rhs );
    }

    /// get element
    /// @param i row
    /// @param j col
    [[gnu::pure]] const T& operator()( int i, int j ) const {
      assert( i >= 0 && j >= 0 && i < N && j < M );
      return m[i * M + j];
    }
    /// get element reference (use for assignment)
    /// @param i row
    /// @param j col
    T& operator()( int i, int j ) {
      assert( i >= 0 && j >= 0 && i < N && j < M );
      return m[i * M + j];
    }

    /// elementwise addition
    [[gnu::pure]] friend Mat<T, N, M> operator+( const Mat<T, N, M>& A, const Mat<T, N, M>& B ) {
      Mat<T, N, M> C;
      Utils::unwind<0, N * M>( [&]( auto i ) { C.m[i] = A.m[i] + B.m[i]; } );
      return C;
    }

    /// elementwise substraction
    [[gnu::pure]] friend Mat<T, N, M> operator-( const Mat<T, N, M>& A, const Mat<T, N, M>& B ) {
      Mat<T, N, M> C;
      Utils::unwind<0, N * M>( [&]( auto i ) { C.m[i] = A.m[i] - B.m[i]; } );
      return C;
    }

    /// Mat * Vec
    [[gnu::pure]] friend Vec<T, N> operator*( const Mat<T, N, M>& m, const Vec<T, M>& v ) {
      Vec<T, N> X;
      Utils::unwind<0, N>(
          [&]( auto i ) { X( i ) = Utils::unwind_sum<0, M>( [&]( auto j ) -> T { return m( i, j ) * v( j ); } ); } );
      return X;
    }

    /// elementwise multiplication with a value
    [[gnu::pure]] friend Mat<T, N, M> operator*( const Mat<T, N, M>& A, const T& s ) {
      Mat<T, N, M> X;
      Utils::unwind<0, N * M>( [&]( auto i ) { X.m[i] = A.m[i] * s; } );
      return X;
    }

    /// operator<< for printing
    friend std::ostream& operator<<( std::ostream& o, const Mat<T, N, M>& A ) {
      o << "[";
      Utils::unwind<0, N - 1>( [&]( auto _n ) {
        Utils::unwind<0, M>( [&]( auto _m ) { o << A( _n, _m ) << ", "; } );
        o << '\n';
      } );
      Utils::unwind<0, M - 1>( [&]( auto _m ) { o << A( N - 1, _m ) << ", "; } );
      o << A( N - 1, M - 1 ) << ']';
      return o;
    }

    /// transposition of Matrix
    Mat<T, M, N> transpose() const {
      Mat<T, M, N> C;
      Utils::unwind<0, M>( [&]( auto i ) { Utils::unwind<0, N>( [&]( auto j ) { C( i, j ) = ( *this )( j, i ); } ); } );
      return C;
    }

    /// cast to symmetrical matrix, using the lower triangle
    [[gnu::pure]] MatSym<T, N> cast_to_sym() const {
      static_assert( N == M, "only square matrices can be cast to MatSym" );
      MatSym<T, N> out{};
      Utils::unwind<0, N>(
          [&]( auto i ) { Utils::unwind<0, i + 1>( [&]( auto j ) { out( i, j ) = ( *this )( i, j ); } ); } );
      return out;
    }

    /// get the submatrix of type SubType, starting at row start_row and col start_col
    template <typename SubType, int start_row, int start_col>
    SubType sub() const {
      static_assert( is_mat_v<SubType>, "SubType needs to be of type Mat" );
      SubType out{};
      Utils::unwind<0, SubType::n_rows>( [&]( auto i ) {
        Utils::unwind<0, SubType::n_cols>( [&]( auto j ) { out( i, j ) = ( *this )( i + start_row, j + start_col ); } );
      } );
      return out;
    }
  };

  template <typename First, typename... Nexts, std::enable_if_t<( std::is_same_v<First, Nexts> && ... ), int> = 0>
  Mat( First, Nexts... )->Mat<First, sizeof...( Nexts ) + 1>;

  /// Vec * Mat
  template <typename T, int N>
  [[gnu::pure]] Mat<T, N> operator*( const Vec<T, N>& v, const Mat<T, N, 1>& m ) {
    Mat<T, N> C;
    Utils::unwind<0, N>( [&]( auto i ) { Utils::unwind<0, N>( [&]( auto j ) { C( i, j ) = v( i ) * m( j, 0 ); } ); } );
    return C;
  }

  /// Matrix multiplication
  template <typename T, int N, int NM, int M>
  [[gnu::pure]] Mat<T, N, M> operator*( const Mat<T, N, NM>& A, const Mat<T, NM, M>& B ) {
    Mat<T, N, M> C;
    Utils::unwind<0, N>( [&]( auto i ) {
      Utils::unwind<0, M>( [&]( auto j ) {
        C( i, j ) = Utils::unwind_sum<0, NM>( [&]( auto k ) -> T { return A( i, k ) * B( k, j ); } );
      } );
    } );
    return C;
  }

  /// @class MatSym
  /// Generic Symetric Matrix object
  ///  0
  ///  1  2
  ///  3  4  5
  ///  6  7  8  9
  template <typename T, int N>
  struct MatSym {
    static constexpr auto n_rows = N;
    static constexpr auto n_cols = N;
    using value_type             = T;
    std::array<T, N*( N + 1 ) / 2> m{};

    /// elementwise comparison ==
    [[gnu::pure]] friend bool operator==( MatSym<T, N> const& lhs, MatSym<T, N> const& rhs ) { return lhs.m == rhs.m; }
    /// elementwise comparison !=
    [[gnu::pure]] friend bool operator!=( MatSym<T, N> const& lhs, MatSym<T, N> const& rhs ) {
      return not( lhs == rhs );
    }

    /// get element
    /// @param i row
    /// @param j col
    [[gnu::pure]] const T& operator()( int i, int j ) const {
      if ( i < j ) std::swap( i, j );
      assert( i >= j && i >= 0 && j >= 0 && i < N && j < N );
      return m[i * ( i + 1 ) / 2 + j];
    }
    /// get element reference (use for assignment)
    /// @param i row
    /// @param j col
    T& operator()( int i, int j ) {
      if ( i < j ) std::swap( i, j );
      assert( i >= j && i >= 0 && j >= 0 && i < N && j < N );
      return m[i * ( i + 1 ) / 2 + j];
    }

    /// Cholesky factorization and substitution based on:
    /// https://hal.archives-ouvertes.fr/hal-01550129/document
    [[gnu::pure]] MatSym<T, N> invChol() const {
      const MatSym<T, N>& A = *this;
      MatSym<T, N>        L;
      auto                inv = initialize_with_zeros<Mat<T, N>>();

      // Cholesky factorization
      Utils::unwind<0, N>( [&]( auto j ) {
        T s = A( j, j );
        Utils::unwind<0, j>( [&]( auto k ) { s = s - L( j, k ) * L( j, k ); } );
        const T rsqrt_s = T{1} / sqrt( s ); // TODO reintroduce rsqrt
        L( j, j )       = rsqrt_s;          // store directly the reciprocal
        Utils::unwind<j + 1, N>(
            []( auto i, auto j, auto& L, const auto& A, auto rsqrt_s ) {
              T s = A( i, j );
              Utils::unwind<0, j>(
                  []( auto k, const auto& L, auto& s, auto i, auto j ) { s = s - L( i, k ) * L( j, k ); }, L, s, i, j );
              L( i, j ) = s * rsqrt_s;
            },
            j, L, A, rsqrt_s );
      } );

      // Forward substitution
      Utils::unwind<0, N>( [&]( auto k ) {
        inv( k, k ) = L( k, k );
        Utils::unwind<k + 1, N>( [&]( auto i ) {
          auto s      = Utils::unwind_sum<0, i>( [&]( auto j ) { return -L( i, j ) * inv( j, k ); } );
          inv( i, k ) = s * L( i, i ); // multiply by the reciprocal
        } );
      } );

      // Backward substitution
      Utils::unwind<0, N>( [&]( auto k ) {
        Utils::unwind<N - 1, k - 1, -1>( [&]( auto i ) {
          auto s = inv( i, k );
          Utils::unwind<i + 1, N>( [&]( auto j ) { s = s - L( j, i ) * inv( j, k ); } );
          inv( i, k ) = s * L( i, i ); // multiply by the reciprocal
        } );
      } );
      return inv.cast_to_sym();
    }

    /// elementwise addition
    [[gnu::pure]] friend MatSym<T, N> operator+( const MatSym<T, N>& A, const MatSym<T, N>& B ) {
      MatSym<T, N> C;
      Utils::unwind<0, N*( N + 1 ) / 2>( [&]( auto i ) { C.m[i] = A.m[i] + B.m[i]; } );
      return C;
    }
    /// elementwise substraction
    [[gnu::pure]] friend MatSym<T, N> operator-( const MatSym<T, N>& A, const MatSym<T, N>& B ) {
      MatSym<T, N> C;
      Utils::unwind<0, N*( N + 1 ) / 2>( [&]( auto i ) { C.m[i] = A.m[i] - B.m[i]; } );
      return C;
    }
    /// matrix multiplication
    [[gnu::pure]] friend Mat<T, N> operator*( const MatSym<T, N>& A, const MatSym<T, N>& B ) {
      Mat<T, N> C;
      Utils::unwind<0, N>( [&]( auto i ) {
        Utils::unwind<0, N>( [&]( auto j ) {
          C( i, j ) = Utils::unwind_sum<0, N>( [&]( auto k ) -> T { return A( i, k ) * B( k, j ); } );
        } );
      } );
      return C;
    }
    /// Matrix * Vector
    [[gnu::pure]] friend Vec<T, N> operator*( const MatSym<T, N>& m, const Vec<T, N>& v ) {
      Vec<T, N> X;
      Utils::unwind<0, N>(
          [&]( auto i ) { X( i ) = Utils::unwind_sum<0, N>( [&]( auto j ) -> T { return m( i, j ) * v( j ); } ); } );
      return X;
    }

    /// elementwise multiplication with a value
    [[gnu::pure]] friend MatSym<T, N> operator*( const MatSym<T, N>& A, const T& s ) {
      MatSym<T, N> X;
      Utils::unwind<0, N*( N + 1 ) / 2>( [&]( auto i ) { X.m[i] = A.m[i] * s; } );
      return X;
    }

    /// operator<< for printing
    friend std::ostream& operator<<( std::ostream& o, const MatSym<T, N>& A ) {
      o << "[";
      Utils::unwind<0, N - 1>( [&]( auto _n ) {
        Utils::unwind<0, N>( [&]( auto _m ) { o << A( _n, _m ) << ", "; } );
        o << '\n';
      } );
      Utils::unwind<0, N - 1>( [&]( auto _m ) { o << A( N - 1, _m ) << ", "; } );
      o << A( N - 1, N - 1 ) << ']';
      return o;
    }

    /// conversion to non symmetrical matrix
    Mat<T, N> cast_to_mat() const {
      Mat<T, N> out{};
      Utils::unwind<0, N>(
          [&]( auto i ) { Utils::unwind<0, N>( [&]( auto j ) { out( i, j ) = ( *this )( i, j ); } ); } );
      return out;
    }

    /// get the submatrix of type SubType, starting at row start_row and col start_col
    template <typename SubType, int start_row, int start_col>
    SubType sub() const {
      static_assert( is_sym_mat_v<SubType> or is_mat_v<SubType>, "SubType needs to be of type Mat or MatSym" );
      SubType out{};
      if constexpr ( is_mat_v<SubType> ) {
        Utils::unwind<0, SubType::n_rows>( [&]( auto i ) {
          Utils::unwind<0, SubType::n_cols>(
              [&]( auto j ) { out( i, j ) = ( *this )( i + start_row, j + start_col ); } );
        } );
      } else if constexpr ( is_sym_mat_v<SubType> ) {
        Utils::unwind<0, SubType::n_rows>( [&]( auto i ) {
          Utils::unwind<0, i + 1>( [&]( auto j ) { out( i, j ) = ( *this )( i + start_row, j + start_col ); } );
        } );
      }
      return out;
    }
  };

  template <typename First, typename... Nexts, std::enable_if_t<( std::is_same_v<First, Nexts> && ... ), int> = 0>
  MatSym( First, Nexts... )->MatSym<First, sizeof...( Nexts ) + 1>;

  /// Similarity of MatSym S with Mat M, defined as (M * S * tranpose(M)) -> MatSym
  template <typename T, int U, int V>
  [[gnu::pure]] MatSym<T, V> similarity( const Mat<T, V, U>& M, const MatSym<T, U>& S ) {
    return ( M * S.cast_to_mat() * M.transpose() ).cast_to_sym();
  }

  /// Similarity of MatSym S with Vec V, defined as (tranpose(V) * M * V) -> scalar
  template <typename T, int N>
  [[gnu::pure]] T similarity( const Vec<T, N>& V, const MatSym<T, N>& S ) {
    return ( V.transpose() * S.cast_to_mat() * V ).cast_to_value();
  }

} // namespace LHCb::LinAlg
