/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "Event/MuonPID_v2.h"
#include "Event/PrFittedForwardTracks.h"
#include "Event/PrIterableFittedForwardTracks.h"
#include "Event/PrIterableMuonPIDs.h"
#include "Event/PrMuonPIDs.h"
#include "Event/PrZip.h"
#include "Event/StateParameters.h"
#include "GaudiKernel/SerializeSTL.h"
#include "SOAExtensions/ZipAlgorithms.h"
#include "SOAExtensions/ZipUtils.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE utestZipInfrastructure
#include <boost/test/unit_test.hpp>

#include <chrono>
#include <ctime>
#include <functional>
#include <iostream>
#include <random>
#include <tuple>
#include <utility>

using Tracks   = LHCb::Pr::Fitted::Forward::Tracks;
using MuonPIDs = LHCb::Pr::Muon::PIDs;

Tracks make_tracks( size_t ntracks ) {
  using dType = SIMDWrapper::scalar::types;
  using F     = dType::float_v;
  using I     = dType::int_v;
  std::mt19937                    gen( 42 ); // Random engine with fixed seed
  std::normal_distribution<float> xy_dist{0.f, 0.1f}, z_dist{0.f, 1.f}, txy_dist{0.f, 5e-3f};
  Tracks                          tracks{nullptr};
  for ( unsigned int i = 0; i < ntracks; ++i ) {
    F       x{xy_dist( gen )}, y{xy_dist( gen )}, z{z_dist( gen )}, tx{txy_dist( gen )}, ty{txy_dist( gen )};
    Vec3<F> beam_pos{x, y, z}, beam_dir{tx, ty, 1.f},
        covX{1e-5f /* x_err**2 */, 0.f /* cov(x, tx) */, 1e-8 /* tx_err**2 */},
        covY{1e-5f /* y_err**2 */, 0.f /* cov(y, ty) */, 1e-8 /* ty_err**2 */};
    tracks.store_trackFT( i, I{0} );     // Ancestor index.
    tracks.store_QoP( i, F{1.f / 5e4} ); // q/p for q = +1, p = 50 GeV
    tracks.store_beamStatePos( i, beam_pos );
    tracks.store_beamStateDir( i, beam_dir );
    tracks.store_chi2( i, F{1.f} );    // chi2/dof
    tracks.store_chi2nDof( i, I{42} ); // chi2 dof
    tracks.store_covX( i, covX );
    tracks.store_covY( i, covY );
    ++tracks.size();
  }
  return tracks;
}

BOOST_AUTO_TEST_CASE( test_emplace_back ) {
  using dType_scalar = SIMDWrapper::scalar::types;
  LHCb::Pr::Muon::PIDs pids{Zipping::generateZipIdentifier()};
  LHCb::Pr::Muon::PID  pid_true{true, 10.f};
  pids.emplace_back( pid_true );                        // 0
  pids.emplace_back( LHCb::Pr::Muon::PID{false, 5.f} ); // 1
  LHCb::Event::v2::MuonPID muon_pid_v2{};
  muon_pid_v2.setIsMuon( true );
  muon_pid_v2.setChi2Corr( 1.f );
  pids.emplace_back( std::move( muon_pid_v2 ) ); // 2

  BOOST_CHECK( ( pids.IsMuon<dType_scalar, true>( 0 ) == true ) );
  BOOST_CHECK( ( pids.IsMuon<dType_scalar, true>( 1 ) == false ) );
  BOOST_CHECK( ( pids.IsMuon<dType_scalar, true>( 2 ) == true ) );
  BOOST_CHECK( ( pids.Chi2Corr<dType_scalar, true>( 0 ) == 10.f ) );
  BOOST_CHECK( ( pids.Chi2Corr<dType_scalar, true>( 1 ) == 5.f ) );
  BOOST_CHECK( ( pids.Chi2Corr<dType_scalar, true>( 2 ) == 1.f ) );
}

template <typename Ismuon>
auto count_muons( MuonPIDs const& muonPIDs, Ismuon ismuon ) {
  auto   iterable_muonPIDs = LHCb::Pr::make_zip( muonPIDs );
  size_t nrec_muons        = 0;
  size_t count             = 0;
  for ( auto const& pid : iterable_muonPIDs.with<SIMDWrapper::InstructionSet::Scalar>() ) {
    if ( pid.IsMuon() ) { nrec_muons++; }
    BOOST_CHECK( ( ismuon( count ) ) == pid.IsMuon() );
    count++;
  }
  return nrec_muons;
}

std::tuple<size_t, MuonPIDs> create_muonids( Tracks const& tracks ) {
  auto ismuon = []( int t ) { return ( t % 2 == 0 ); };
  auto nmuons = tracks.size() / 2 + tracks.size() % 2; // Even true, odd false

  int  counter      = 0;
  auto track_ismuon = [&counter, &ismuon]( auto const t ) {
    auto result = ismuon( counter ) && t.pt() > 0.f;
    counter++;
    return LHCb::Pr::Muon::PID{result, 0.f};
  };
  auto iterable_tracks_scalar = LHCb::Pr::make_zip<SIMDWrapper::InstructionSet::Scalar, true>( tracks );
  auto muonPIDs               = Zipping::transform<MuonPIDs>( iterable_tracks_scalar, track_ismuon );
  // Check if counted correctly with scalar version
  BOOST_CHECK( ( count_muons( muonPIDs, ismuon ) == static_cast<size_t>( nmuons ) ) );
  BOOST_CHECK( ( muonPIDs.size() == static_cast<size_t>( tracks.size() ) ) );
  return std::tuple<int, MuonPIDs>{nmuons, std::move( muonPIDs )};
}

BOOST_AUTO_TEST_CASE( test_transform_from_tracks ) {
  unsigned int ntracks    = 999;
  auto         tracks     = make_tracks( ntracks );
  auto [nmuons, muonPIDs] = create_muonids( tracks );

  // Check if counted correctly using vectorized version, compiler flag dependent.
  size_t nrec_muons        = 0;
  auto   iterable_muonPIDs = LHCb::Pr::make_zip( muonPIDs );
  for ( auto const& pid : iterable_muonPIDs ) { nrec_muons += popcount( pid.IsMuon() && pid.loop_mask() ); }
  BOOST_CHECK( ( nrec_muons == nmuons ) );
}

BOOST_AUTO_TEST_CASE( test_constructor_from_tracks_and_lambda ) {
  unsigned int ntracks                = 999;
  auto         tracks                 = make_tracks( ntracks );
  auto         iterable_tracks_scalar = LHCb::Pr::make_zip<SIMDWrapper::InstructionSet::Scalar, true>( tracks );

  auto ismuon = []( int t ) { return ( t % 3 == 0 ); };
  auto nmuons = tracks.size() / 3 + tracks.size() % 3; // false, false, true

  int  counter      = 0;
  auto track_ismuon = [&counter, &ismuon]( auto const t ) {
    auto result = ismuon( counter ) && t.pt() > 0.f;
    counter++;
    return LHCb::Pr::Muon::PID{result, 0.f};
  };

  // Test transform using Muon::PIDs constructor taking lambda
  MuonPIDs muonPIDs{iterable_tracks_scalar, track_ismuon};
  BOOST_CHECK( ( count_muons( muonPIDs, ismuon ) == static_cast<size_t>( nmuons ) ) );
  BOOST_CHECK( ( muonPIDs.size() == static_cast<size_t>( tracks.size() ) ) );
}

BOOST_AUTO_TEST_CASE( test_zipping_of_tracks_and_muonids ) {
  unsigned int ntracks    = 999;
  auto         tracks     = make_tracks( ntracks );
  auto [nmuons, muonPIDs] = create_muonids( tracks );

  // Test zipping
  auto zipped = LHCb::Pr::make_zip( tracks, muonPIDs );

  Tracks   output_tracks{nullptr};
  MuonPIDs output_pids( output_tracks.zipIdentifier() );
  output_pids.reserve( output_tracks.size() );

  // Filter
  using dType = decltype( zipped )::default_simd_t;
  for ( auto const& chunk : zipped ) {
    auto loop_mask = chunk.loop_mask();
    auto filt_mask = ( chunk.pt() > 0.f ) && ( chunk.IsMuon() );
    output_tracks.copy_back<dType>( tracks, chunk.offset(), loop_mask && filt_mask );
    output_pids.copy_back<dType>( muonPIDs, chunk.offset(), loop_mask && filt_mask );
  }

  BOOST_CHECK( ( output_tracks.size() == (int)nmuons ) );
  BOOST_CHECK( ( output_pids.size() == nmuons ) );

  auto iterable_new_muonPIDs = LHCb::Pr::make_zip( output_pids );
  BOOST_CHECK( iterable_new_muonPIDs.size() == nmuons );

  for ( auto const& pid : iterable_new_muonPIDs.with<SIMDWrapper::InstructionSet::Scalar>() ) {
    BOOST_CHECK( pid.IsMuon() );
  }

  // Pr::Zip filter returning combined object
  auto new_data = zipped.filter( []( auto const& chunk ) { return ( chunk.pt() > 0.f ) && ( chunk.IsMuon() ); } );
  BOOST_CHECK( ( new_data.size() == nmuons ) );
}

BOOST_AUTO_TEST_CASE( test_timing_of_ismuon ) {
  std::cout << "Test timing of scalar and vectorised loops" << std::endl;

  unsigned int ntracks    = 999;
  auto         tracks     = make_tracks( ntracks );
  auto [nmuons, muonPIDs] = create_muonids( tracks );

  auto iterable_muonPIDs = LHCb::Pr::make_zip( muonPIDs );

  // Test timing
  unsigned int                                                iterations = 1000;
  unsigned int                                                tries      = 15;
  unsigned int                                                n_success  = 0;
  std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

  for ( unsigned int j = 0; j < tries; j++ ) {

    // vectorised loop
    bool correct_vector = false;
    start               = std::chrono::system_clock::now();
    for ( unsigned int i = 0; i < iterations; i++ ) {
      size_t nrec_muons_vector = 0;
      for ( auto const& pid : iterable_muonPIDs ) { nrec_muons_vector += popcount( pid.IsMuon() && pid.loop_mask() ); }
      correct_vector = nrec_muons_vector == nmuons;
    }
    end                            = std::chrono::system_clock::now();
    int elapsed_seconds_vectorized = std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count();

    // scalar loop
    bool correct_scalar = false;
    start               = std::chrono::system_clock::now();
    for ( unsigned int i = 0; i < iterations; i++ ) {
      size_t nrec_muons_scalar = 0;
      for ( auto const& pid : iterable_muonPIDs.with<SIMDWrapper::InstructionSet::Scalar>() ) {
        if ( pid.IsMuon() ) { nrec_muons_scalar++; }
      }
      correct_scalar = nrec_muons_scalar == nmuons;
    }
    end                        = std::chrono::system_clock::now();
    int elapsed_seconds_scalar = std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count();

    BOOST_CHECK( correct_vector );
    BOOST_CHECK( correct_scalar );

    using dType = decltype( iterable_muonPIDs )::default_simd_t;
    if constexpr ( !std::is_same_v<SIMDWrapper::scalar::types, dType> ) {
      std::cout << "Elapsed time (vector loops): " << elapsed_seconds_vectorized << " micro s. (Try " << j << ")\n";
      std::cout << "Elapsed time (scalar loops): " << elapsed_seconds_scalar << " micro s. (Try " << j << ")\n";
      // Check that vectorized is faster than scalar
      bool success = ( elapsed_seconds_vectorized < elapsed_seconds_scalar );
      if ( success ) {
        n_success++;
      } else {
        std::cout << "Failed" << std::endl;
      }
      // BOOST_CHECK( success  );
    } else {
      std::cout << "Elapsed time (scalar  (deduced) loops): " << elapsed_seconds_vectorized << " micro s. (Try " << j
                << ")\n";
      std::cout << "Elapsed time (scalar (explicit) loops): " << elapsed_seconds_scalar << " micro s. (Try " << j
                << ")\n";

      // Check that both scalar versions give about the same result.
      bool success = static_cast<double>( elapsed_seconds_vectorized ) / elapsed_seconds_scalar < 2.0 &&
                     static_cast<double>( elapsed_seconds_scalar ) / elapsed_seconds_vectorized < 2.0;
      if ( success ) {
        n_success++;
      } else {
        std::cout << "Failed" << std::endl;
      }
      // BOOST_CHECK( success  );
    }
  }
  BOOST_CHECK( ( 2 * n_success > tries ) );

  std::cout << "Success " << n_success << " / " << tries << "\n";
}
