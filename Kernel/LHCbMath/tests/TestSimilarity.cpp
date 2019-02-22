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
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <iostream>
#include <map>
// ============================================================================
// VectorClass
// ============================================================================
#include "instrset.h"
#include "vectorclass.h"
// ============================================================================
// Gaudi and ROOT types
// ============================================================================
#include "GaudiKernel/GenericMatrixTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"
#include "TRandom3.h"
// ============================================================================
// LHCbMath
// ============================================================================
#include "LHCbMath/Similarity.h"

using LHCb::Math::detail::to_span;
// ============================================================================
// Advance declaration for similarity methods
// ============================================================================
namespace LHCb {
  namespace Math {
    namespace detail {
      namespace avx2 {
        extern void similarity_5_1( span<const double, 15> Ci, span<const double, 5> Fi, span<double, 1> Ti );
        extern void similarity_5_5( span<const double, 15> Ci, span<const double, 25> Fi, span<double, 15> Ti );
        extern void similarity_5_7( span<const double, 15> Ci, span<const double, 35> Fi, span<double, 28> Ti );
      } // namespace avx2
      namespace avx {
        extern void similarity_5_1( span<const double, 15> Ci, span<const double, 5> Fi, span<double, 1> Ti );
        extern void similarity_5_5( span<const double, 15> Ci, span<const double, 25> Fi, span<double, 15> Ti );
        extern void similarity_5_7( span<const double, 15> Ci, span<const double, 35> Fi, span<double, 28> Ti );
      } // namespace avx
      namespace sse3 {
        extern void similarity_5_1( span<const double, 15> Ci, span<const double, 5> Fi, span<double, 1> Ti );
        extern void similarity_5_5( span<const double, 15> Ci, span<const double, 25> Fi, span<double, 15> Ti );
        extern void similarity_5_7( span<const double, 15> Ci, span<const double, 35> Fi, span<double, 28> Ti );
      } // namespace sse3
      namespace generic {
        extern void similarity_5_1( span<const double, 15> Ci, span<const double, 5> Fi, span<double, 1> Ti );
        extern void similarity_5_5( span<const double, 15> Ci, span<const double, 25> Fi, span<double, 15> Ti );
        extern void similarity_5_7( span<const double, 15> Ci, span<const double, 35> Fi, span<double, 28> Ti );
        extern void similarity_2_5( span<const double, 3> Ci, span<const double, 10> Fi, span<double, 15> Ti );
      } // namespace generic
    }   // namespace detail
  }     // namespace Math
} // namespace LHCb

enum ISet : std::int8_t { GENERIC = 0, SSE3 = 3, AVX = 7, AVX2 = 8 };

std::map<ISet, LHCb::Math::detail::similarity_t<5, 1>> vtbl_5_1 = {
    {ISet::AVX2, LHCb::Math::detail::avx2::similarity_5_1},
    {ISet::AVX, LHCb::Math::detail::avx::similarity_5_1},
    {ISet::SSE3, LHCb::Math::detail::sse3::similarity_5_1},
    {ISet::GENERIC, LHCb::Math::detail::generic::similarity_5_1}};
std::map<ISet, LHCb::Math::detail::similarity_t<5, 5>> vtbl_5_5 = {
    {ISet::AVX2, LHCb::Math::detail::avx2::similarity_5_5},
    {ISet::AVX, LHCb::Math::detail::avx::similarity_5_5},
    {ISet::SSE3, LHCb::Math::detail::sse3::similarity_5_5},
    {ISet::GENERIC, LHCb::Math::detail::generic::similarity_5_5}};
std::map<ISet, LHCb::Math::detail::similarity_t<5, 7>> vtbl_5_7 = {
    {ISet::AVX2, LHCb::Math::detail::avx2::similarity_5_7},
    {ISet::AVX, LHCb::Math::detail::avx::similarity_5_7},
    {ISet::SSE3, LHCb::Math::detail::sse3::similarity_5_7},
    {ISet::GENERIC, LHCb::Math::detail::generic::similarity_5_7}};
std::map<ISet, LHCb::Math::detail::similarity_t<2, 5>> vtbl_2_5 = {
    {ISet::AVX2, LHCb::Math::detail::generic::similarity_2_5},
    {ISet::AVX, LHCb::Math::detail::generic::similarity_2_5},
    {ISet::SSE3, LHCb::Math::detail::generic::similarity_2_5},
    {ISet::GENERIC, LHCb::Math::detail::generic::similarity_2_5}};

// ============================================================================
// Util methods
// ============================================================================

/**
 * Check whether the AVX instruction set is available
 */
bool hasInstructionSet( ISet lvl ) {
  const auto level = instrset_detect();
  return ( level >= static_cast<int>( lvl ) );
}

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
void fillSMatrixSymWithCondNumber( SymMat& F, TRandom& r, const double condNumber ) {

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

/**
 * Compare SymMatrices
 */
template <typename M>
std::pair<M, bool> compareSMatrix( const M& A, const M& B, const bool symetric = true,
                                   const double diffThreshold = 1e-15 ) {
  M    cmpres;
  bool hasDiff = false;
  for ( int i = 0; i < A.kCols; ++i ) {
    for ( int j = ( symetric ? i : 0 ); j < A.kRows; ++j ) {
      cmpres( i, j ) = ( A( i, j ) - B( i, j ) );
      if ( TMath::Abs( cmpres( i, j ) ) > diffThreshold ) { hasDiff = true; }
    }
  }
  return {cmpres, hasDiff};
}

template <typename Mat, typename SymMat, typename TargetMat, typename VTable>
int compareInstructionSets( const SymMat& origin, Mat& F, TargetMat& target, const double conditionNumber,
                            const VTable& simFuncs, const bool printResults = true, const ISet iSet = ISet::GENERIC ) {

  // Checking the instruction sets to do
  // if ISet::GENERIC => test all
  // if ISet::AVX2/AVX/SSE3 => just do that one
  const bool doSSE3 = ( iSet == ISet::SSE3 || iSet == ISet::GENERIC );
  const bool doAVX  = ( iSet == ISet::AVX || iSet == ISet::GENERIC );
  const bool doAVX2 = ( iSet == ISet::AVX2 || iSet == ISet::GENERIC );

  // Setting the threshold for error
  const double diffThreshold = 1e-15 * conditionNumber;
  // Checking the instruction sets available
  const bool hasAVX  = hasInstructionSet( ISet::AVX );
  const bool hasAVX2 = hasInstructionSet( ISet::AVX2 );
  const bool hasSSE3 = hasInstructionSet( ISet::SSE3 );

  bool SSE3Diff = false;
  bool AVXDiff  = false;
  bool AVX2Diff = false;

  if ( printResults ) {
    std::cout << "Has SSE3: " << hasSSE3 << " Has AVX: " << hasAVX << " Has AVX2: " << hasAVX2 << std::endl;
    std::cout << "ISet = " << iSet << " | DoSSE3: " << doSSE3 << " DoAVX: " << doAVX << " Do AVX2: " << doAVX2
              << std::endl;
  }

  // Running the transform for the generic method
  simFuncs.at( ISet::GENERIC )( to_span( origin ), to_span( F ), to_span( target ) );
  if ( printResults ) {
    std::cout << "Generic similarity transform result" << std::endl;
    std::cout << target << std::endl;
  }

  if ( doSSE3 ) {
    if ( hasSSE3 ) {
      TargetMat targetSSE3;
      // Checking SSE3
      simFuncs.at( ISet::SSE3 )( to_span( origin ), to_span( F ), to_span( targetSSE3 ) );
      if ( printResults ) {
        std::cout << "SSE3 similarity transform result" << std::endl;
        std::cout << targetSSE3 << std::endl;
      }

      auto cmpSSE3Res = compareSMatrix( targetSSE3, target, true, diffThreshold );
      SSE3Diff        = cmpSSE3Res.second;

      if ( printResults ) std::cout << "SSE3 Differences" << std::endl << cmpSSE3Res.first << std::endl;
    } else {
      // Cannot test SSE3 if not present
      // Returning a special value to tell gaudi this was untested...
      return 77;
    }
  }

  // Checking AVX
  if ( doAVX ) {
    if ( hasAVX ) {
      TargetMat targetAVX;
      simFuncs.at( ISet::AVX )( to_span( origin ), to_span( F ), to_span( targetAVX ) );
      if ( printResults ) {
        std::cout << "AVX similarity transform result" << std::endl;
        std::cout << targetAVX << std::endl;
      }

      auto cmpAVXRes = compareSMatrix( targetAVX, target, true, diffThreshold );
      AVXDiff        = cmpAVXRes.second;

      if ( printResults ) std::cout << "AVX Differences" << std::endl << cmpAVXRes.first << std::endl;
    } else {
      // Cannot test AVX if not present
      // Returning a special value to tell gaudi this was untested...
      return 77;
    }
  }

  // Checking AVX2
  if ( doAVX2 ) {
    if ( hasAVX2 ) {
      TargetMat targetAVX2;
      simFuncs.at( ISet::AVX2 )( to_span( origin ), to_span( F ), to_span( targetAVX2 ) );
      if ( printResults ) {
        std::cout << "AVX2 similarity transform result" << std::endl;
        std::cout << targetAVX2 << std::endl;
      }

      auto cmpAVX2Res = compareSMatrix( targetAVX2, target, true, diffThreshold );
      AVX2Diff        = cmpAVX2Res.second;

      if ( printResults ) std::cout << "AVX2 Differences" << std::endl << cmpAVX2Res.first << std::endl;
    } else {
      // Cannot test AVX if not present
      // Returning a special value to tell gaudi this was untested...
      return 77;
    }
  }

  // Checking if we found errors
  return ( SSE3Diff || AVXDiff || AVX2Diff ? 1 : 0 );
}
// ============================================================================
// Main method
// ============================================================================

int main( int argc, char* argv[] ) {

  TRandom3 r( 1 );

  const int testcount = 10000;

  // Condition numbers to test
  const std::vector<double> condNumbers = {1.0, 1e6, 1e9};

  // Checking args to see the test to do
  ISet iSet = ISet::GENERIC; // Test ALL by default...
  if ( argc > 1 ) {
    const auto arg = std::string( argv[1] );
    if ( arg == "AVX2" ) {
      iSet = ISet::AVX2;
    } else if ( arg == "AVX" ) {
      iSet = ISet::AVX;
    } else if ( arg == "SSE3" ) {
      iSet = ISet::SSE3;
    }
  }

  std::cout << "Checking instruction set : " << iSet << std::endl;

  std::cout << "============= Similarity_5_5 Test =============" << std::endl;
  for ( int i = 0; i < testcount; ++i ) {
    for ( auto condNumber : condNumbers ) {
      Gaudi::Matrix5x5    F;
      Gaudi::SymMatrix5x5 origin;
      Gaudi::SymMatrix5x5 target;
      fillRandomSMatrix( F, r );
      fillSMatrixSymWithCondNumber<Gaudi::Matrix5x5, Gaudi::SymMatrix5x5>( origin, r, condNumber );
      const auto ret = compareInstructionSets( origin, F, target, condNumber, vtbl_5_5, ( i % 5000 ) == 0, iSet );
      if ( ret > 0 ) return ret;
    }
  }

  std::cout << "============= Similarity_5_7 Test =============" << std::endl;
  for ( int i = 0; i < testcount; ++i ) {
    for ( auto condNumber : condNumbers ) {
      Gaudi::SymMatrix5x5               origin;
      ROOT::Math::SMatrix<double, 7, 5> F;
      Gaudi::SymMatrix7x7               target;
      fillRandomSMatrix( F, r );
      fillSMatrixSymWithCondNumber<Gaudi::Matrix5x5, Gaudi::SymMatrix5x5>( origin, r, condNumber );
      const auto ret = compareInstructionSets( origin, F, target, condNumber, vtbl_5_7, ( i % 5000 ) == 0, iSet );
      if ( ret > 0 ) return ret;
    }
  }

  std::cout << "============= Similarity_2_5 Test =============" << std::endl;
  std::cout << "!! Only the generic version is implemented !!" << std::endl;
  for ( int i = 0; i < testcount; ++i ) {
    for ( auto condNumber : condNumbers ) {
      Gaudi::SymMatrix2x2               origin;
      ROOT::Math::SMatrix<double, 5, 2> F;
      Gaudi::SymMatrix5x5               target;
      fillRandomSMatrix( F, r );
      fillSMatrixSymWithCondNumber<Gaudi::Matrix2x2, Gaudi::SymMatrix2x2>( origin, r, condNumber );
      const auto ret = compareInstructionSets( origin, F, target, condNumber, vtbl_2_5, ( i % 5000 ) == 0, iSet );
      if ( ret > 0 ) return ret;
    }
  }

  std::cout << "============= Similarity_5_1 Test =============" << std::endl;
  for ( int i = 0; i < testcount; ++i ) {
    for ( auto condNumber : condNumbers ) {
      Gaudi::Matrix1x5    F;
      Gaudi::SymMatrix5x5 origin;
      Gaudi::Matrix1x1    target; // really just a double, but need a matrix with 'Array()'...
      fillRandomSMatrix( F, r );
      fillSMatrixSymWithCondNumber<Gaudi::Matrix5x5, Gaudi::SymMatrix5x5>( origin, r, condNumber );
      const auto ret = compareInstructionSets( origin, F, target, condNumber, vtbl_5_1, ( i % 5000 ) == 0, iSet );
      if ( ret > 0 ) return ret;
    }
  }

  return 0;
}
// ============================================================================
//                                                                      The END
// ============================================================================
