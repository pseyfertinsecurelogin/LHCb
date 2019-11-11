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
#include "Event/GeneratePrFittedForwardTracks.h"

#include <TMath.h>

#include <random>

LHCb::Pr::Fitted::Forward::Tracks LHCb::Pr::Fitted::Forward::generate_tracks( std::size_t nTracks, unsigned int seed ) {
  using dType = SIMDWrapper::scalar::types;
  using F     = dType::float_v;
  using I     = dType::int_v;
  // Try to ensure that:
  // - increasing nTracks from N -> M does not affect the first N tracks
  // - adding a new field to the output would not change the existing fields
  // Achieve this by using a bunch of different random engines with fixed seeds
  seed *= 30; // avoid reusing a seed in event n-1 in event n, giving some spare slots if this needs to be made more
              // sophisticated in future
  std::mt19937 gen_x{seed + 1}, gen_y{seed + 2}, gen_z{seed + 3}, gen_phi{seed + 4}, gen_eta{seed + 5},
      gen_cov_x_x{seed + 6}, gen_corr_x_tx{seed + 7}, gen_cov_tx_tx{seed + 8}, gen_qoverp{seed + 9},
      gen_ndof{seed + 10}, gen_chi2{seed + 11};
  // The various parametrisations were fitted to the output of the VeloKalman
  // algorithm in the default Moore configuration in early November 2019. The
  // parametrisations were chosen based on the available distributions in the
  // standard library. They aim to reproduce the gross features of the
  // distributions without worrying too much about capturing the details.

  // Helpers for sampling (x, y)
  auto make_xy_maker = [part_choice = std::uniform_real_distribution<float>{0.f, 1.f},
                        core_dist   = std::normal_distribution<float>{0.f, 4.51e-2f},
                        tail_dist   = std::cauchy_distribution<float>{0.f, 9.78e-1f}]() mutable {
    return [&]( auto&& gen ) {
      for ( ;; ) {
        auto x = part_choice( gen ) > 5e-2 ? core_dist( gen ) : tail_dist( gen );
        if ( std::abs( x ) < 10.f ) return x;
      }
    };
  };
  auto get_x = make_xy_maker();
  auto get_y = make_xy_maker();
  // Helper for sampling z
  auto get_z = [part_choice = std::uniform_real_distribution<float>{0.f, 1.f},
                core        = std::normal_distribution<float>{0.f, 64.f},
                tail        = std::normal_distribution<float>{313.f, 200.f}]( auto&& gen ) mutable {
    for ( ;; ) {
      auto z = part_choice( gen ) > 1.3e-2 ? core( gen ) : tail( gen );
      if ( z > -400.f && z < 900.f ) return z;
    }
  };
  auto get_phi = [sign_gen = std::uniform_real_distribution<float>{0.f, 1.f},
                  gaus     = std::normal_distribution<float>{TMath::PiOver2(), 1.5f}]( auto&& gen ) mutable {
    auto sign = sign_gen( gen ) > 0.5f ? 1.f : -1.f;
    return sign * gaus( gen );
  };
  auto get_eta = [dist = std::gamma_distribution<float>{5.f, 0.4f}]( auto&& gen ) mutable {
    for ( ;; ) {
      auto eta = 1.4f + dist( gen );
      if ( eta < 5.f ) return eta;
    }
  };
  auto get_qoverp = [sign_choice = std::uniform_real_distribution<float>{0.f, 1.f},
                     dist        = std::lognormal_distribution<float>{-2.1e-1f, 0.986f}]( auto&& gen ) mutable {
    for ( ;; ) {
      auto abs_qoverp = 1e-4f * dist( gen );
      if ( abs_qoverp > 1e-6 && abs_qoverp < 4.5e-4 ) return ( sign_choice( gen ) > 0.5 ? 1.f : -1.f ) * abs_qoverp;
    }
  };
  auto get_ndof = [dist = std::poisson_distribution<int>{36}]( auto&& gen ) mutable {
    for ( ;; ) {
      auto ndof = dist( gen ) - 22;
      if ( ndof >= 2 && ndof % 2 == 0 ) return ndof;
    }
  };
  auto get_cov_tx_tx = [dist = std::gamma_distribution<float>{1.3f, 1.9e-2f}]( auto&& gen ) mutable {
    return 1e-6f * ( 1e-3f + dist( gen ) );
  };
  auto get_cov_x_tx = [dist = std::lognormal_distribution<float>{-3.3, 0.555f}]( auto&& gen, float cov_x_x,
                                                                                 float cov_tx_tx ) mutable {
    for ( ;; ) {
      auto corr_x_tx = -0.999f + dist( gen );
      if ( corr_x_tx < +0.999f ) return corr_x_tx * std::sqrt( cov_x_x * cov_tx_tx );
    }
  };
  auto get_cov_x_x = [dist = std::gamma_distribution<float>{2.5f, 4e-4f}]( auto&& gen ) mutable { return dist( gen ); };
  auto get_chi2    = [dist = std::chi_squared_distribution<float>{}]( auto&& gen, int ndof ) mutable {
    dist.param( std::chi_squared_distribution<float>::param_type( ndof ) );
    return dist( gen );
  };

  Tracks tracks{/* ancestor relations */ nullptr};
  for ( int i = 0; i < int( nTracks ); ++i ) {
    // Generate (x, y, z) directly
    F x{get_x( gen_x )}, y{get_y( gen_y )}, z{get_z( gen_z )};
    // Generate (eta, phi) then derive (tx, ty)
    float phi{get_phi( gen_phi )}, eta{get_eta( gen_eta )};
    float tan_theta{( 2.f * std::exp( eta ) ) / ( std::exp( 2.f * eta ) - 1.f )};
    F     tx{tan_theta * std::cos( phi )}, ty{tan_theta * std::sin( phi )};
    // Generate cov(x, x) and cov(tx, tx)
    float cov_x_x{get_cov_x_x( gen_cov_x_x )}, cov_tx_tx{get_cov_tx_tx( gen_cov_tx_tx )};
    // Generate corr(x, tx) and calculate cov(x, tx)
    F cov_x_tx{get_cov_x_tx( gen_corr_x_tx, cov_x_x, cov_tx_tx )};
    // Generate the number of degrees of freedom
    int ndof{get_ndof( gen_ndof )};
    // Generate the chi2 itself
    float   chi2{get_chi2( gen_chi2, ndof )};
    Vec3<F> beam_pos{x, y, z}, beam_dir{tx, ty, 1.f},
        covX{F{cov_x_x} /* x_err**2 */, cov_x_tx /* cov(x, tx) */, F{cov_tx_tx} /* tx_err**2 */},
        covY{F{cov_x_x} /* y_err**2 */, cov_x_tx /* cov(y, ty) */, F{cov_tx_tx} /* ty_err**2 */};
    tracks.store_trackFT( i, I{i} ); // Ancestor index.
    tracks.store_QoP( i, F{get_qoverp( gen_qoverp )} );
    tracks.store_beamStatePos( i, beam_pos );
    tracks.store_beamStateDir( i, beam_dir );
    tracks.store_chi2( i, F{chi2 / ndof} ); // chi2/dof
    tracks.store_chi2nDof( i, I{ndof} );    // chi2 dof
    tracks.store_covX( i, covX );
    tracks.store_covY( i, covY );
    ++tracks.size();
  }
  return tracks;
}