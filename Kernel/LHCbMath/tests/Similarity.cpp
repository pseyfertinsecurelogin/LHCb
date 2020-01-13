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

// local
#include "LHCbMath/Similarity.h"

// Gaudi
#include "GaudiKernel/System.h"

// local timing utils
#include "Timing.h"

// STL
#include <iostream>
#include <random>
#include <string>
#include <typeinfo>
#include <vector>

#include "TRandom3.h"

/**
 * Generate a random Matrix and a SymMatrix
 */
template <typename Mat>
void fillRandomSMatrix( Mat& F, TRandom& r, const bool symmetric = false ) {
  for ( int i = 0; i < F.kRows; i++ ) {
    for ( int j = ( symmetric == true ? i : 0 ); j < F.kCols; j++ ) { F( i, j ) = r.Rndm(); }
  }
}

/**
 * Orthogonalize a SMatrix
 */
template <typename Mat>
Mat orthogonalizeMatrix( const Mat& M, bool& processOk ) {
  processOk = false;
  Mat Q;

  // Checking that we have a square matrix
  if ( M.kRows != M.kCols ) {
    processOk = false;
  } else {

    int n = M.kRows;
    Mat U;

    for ( int i = 0; i < n; i++ ) {

      // Initializing U with the original column from M
      for ( int k = 0; k < n; k++ ) { U( k, i ) = M( k, i ); }

      // Iterate of already established basis vectors to make sure we have an orthogonal vector
      for ( int j = 0; j < i; j++ ) {
        // Computing the scalar product
        double_t sp = 0.0;
        for ( int k = 0; k < n; k++ ) { sp += Q( k, j ) * M( k, i ); }

        // Now doing the actual projection
        for ( int k = 0; k < n; k++ ) { U( k, i ) += -sp * Q( k, j ); }
      }

      double_t norm = 0.0;
      for ( int k = 0; k < n; k++ ) { norm += U( k, i ) * U( k, i ); }
      norm = sqrt( norm );
      for ( int k = 0; k < n; k++ ) { Q( k, i ) = U( k, i ) / norm; }
    }

    processOk = true;
  }

  return Q;
}

/**
 * Generate a random SymMatrix with a given max Condition number
 */
template <typename Mat, typename SymMat>
void fillSMatrixSymWithCondNumber( SymMat& F, TRandom& r, const double condNumber = 1.0 ) {

  Mat  T;
  Mat  Q;
  bool processOk = false;

  do {
    fillRandomSMatrix( T, r );
    Q = orthogonalizeMatrix( T, processOk );
  } while ( !processOk );

  Mat tQ = ROOT::Math::Transpose( Q );

  Mat D;
  D( 0, 0 ) = 1;
  for ( int i = 1; i < D.kRows; i++ ) { D( i, i ) = r.Rndm() * condNumber; }

  Mat origin;
  origin = Q * D * tQ;

  for ( int i = 0; i < F.kRows; i++ ) {
    for ( int j = 0; j <= i; j++ ) { F( i, j ) = origin( i, j ); }
  }
}

template <std::size_t N>
using SymMatrix = ROOT::Math::SMatrix<double, N, N, ROOT::Math::MatRepSym<double, N>>;

template <std::size_t N>
using Matrix = ROOT::Math::SMatrix<double, N, N>;

template <std::size_t M, std::size_t N>
class Data {
public:
  using Vector = typename std::vector<Data<M, N>>;
  using T      = SymMatrix<N>;
  using O      = SymMatrix<M>;
  using F      = ROOT::Math::SMatrix<double, N, M>;

public:
  Data() {
    static TRandom3 r( 1 );
    fillRandomSMatrix( m_f, r );
    fillSMatrixSymWithCondNumber<Matrix<M>>( m_o, r );
  }

public:
  void test() {
    if constexpr ( N > 1 ) {
      LHCb::Math::Similarity( m_f, m_o, m_t );
    } else {
      m_t( 1, 1 ) = LHCb::Math::Similarity( m_f, m_o );
    }
  }

public:
  F m_f;
  O m_o; // origin
  T m_t; // target
};

template <std::size_t NTESTS, typename DATA>
unsigned long long int __attribute__( ( noinline ) ) test( DATA& dataV ) {
  unsigned long long int best_dur{99999999999999999};

  timespec start, end;

  for ( std::size_t i = 0; i < NTESTS; ++i ) {

    // start clock
    clock_gettime( CLOCK_MONOTONIC_RAW, &start );

    // iterate over the data
    for ( auto& data : dataV ) { data.test(); }

    // stop clock
    clock_gettime( CLOCK_MONOTONIC_RAW, &end );

    // get best time
    const auto duration = time_diff( &start, &end );
    if ( duration < best_dur ) { best_dur = duration; }
  }

  return best_dur;
}

int main() {

  constexpr std::size_t nData  = 10000;
  constexpr std::size_t nTests = 5000;

  // Generate the data
  std::cout << "Generating " << nData << " data values." << std::endl;
  Data<5, 5>::Vector data_5_5( nData );
  Data<5, 7>::Vector data_5_7( nData );
  Data<2, 5>::Vector data_2_5( nData );
  Data<5, 1>::Vector data_5_1( nData );

  std::cout << "Running " << nTests << " tests for each dataset :-" << std::endl;
  const auto time_2_5 = test<nTests>( data_2_5 );
  std::cout << "2x5 : " << time_2_5 << std::endl;
  const auto time_5_5 = test<nTests>( data_5_5 );
  std::cout << "5x5 : " << time_5_5 << std::endl;
  const auto time_5_7 = test<nTests>( data_5_7 );
  std::cout << "5x7 : " << time_5_7 << std::endl;
  const auto time_5_1 = test<nTests>( data_5_1 );
  std::cout << "5x1 : " << time_5_1 << std::endl;

  std::cout << data_5_5.front().m_t << std::endl;
  std::cout << data_5_7.front().m_t << std::endl;
  std::cout << data_2_5.front().m_t << std::endl;
  std::cout << data_5_1.front().m_t << std::endl;

  return 0;
}
