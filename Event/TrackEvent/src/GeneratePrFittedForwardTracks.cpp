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

#include <random>

LHCb::Pr::Fitted::Forward::Tracks LHCb::Pr::Fitted::Forward::generate_tracks( std::size_t nTracks ) {
  using dType = SIMDWrapper::scalar::types;
  using F     = dType::float_v;
  using I     = dType::int_v;
  std::mt19937                    gen( 42 ); // Random engine with fixed seed
  std::normal_distribution<float> xy_dist{0.f, 0.1f}, z_dist{0.f, 1.f}, txy_dist{0.f, 5e-3f};
  Tracks                          tracks{nullptr};
  for ( unsigned int i = 0; i < nTracks; ++i ) {
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