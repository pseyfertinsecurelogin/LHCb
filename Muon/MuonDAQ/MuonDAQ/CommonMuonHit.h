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
#include "Kernel/MuonTileID.h"
#include "Kernel/STLExtensions.h"

#include "GaudiKernel/Range.h"

/** @class CommonMuonHit CommonMuonHit.h
 * A container for a muon hit that holds a reference to the corresponding tile
 *as well as information on x, y, z and corresponding errors.
 */
class CommonMuonHit final {
public:
  CommonMuonHit() = default;

  CommonMuonHit( LHCb::MuonTileID tile, double x, double dx, double y, double dy, double z, double dz, bool uncrossed,
                 unsigned int t, int deltat, int cls = 0 )
      : m_params{{x, y, dx, z, dy, dz}}
      , m_tile{std::move( tile )}
      , m_uncrossed{uncrossed}
      , m_t{t}
      , m_dt{deltat}
      , m_clusize{cls} {}

  CommonMuonHit( const LHCb::MuonTileID& tile, double x, double dx, double y, double dy, double z, double dz,
                 bool uncrossed )
      : CommonMuonHit( tile, x, dx, y, dy, z, dz, uncrossed, 0, 0, 0 ) {}

  CommonMuonHit( LHCb::MuonTileID tile, LHCb::MuonTileID subtile1, LHCb::MuonTileID subtile2, double x, double dx,
                 double y, double dy, double z, double dz, unsigned int t, int deltat, int cls )
      : CommonMuonHit( std::move( tile ), x, dx, y, dy, z, dz, false, t, deltat, cls ) {
    m_subtiles[0] = std::move( subtile1 );
    m_subtiles[1] = std::move( subtile2 );
  }

  const LHCb::MuonTileID&            tile() const { return m_tile; }
  unsigned int                       station() const { return m_tile.station(); }
  unsigned int                       region() const { return m_tile.region(); }
  double                             x() const { return m_params[0]; }
  double                             dx() const { return m_params[2]; }
  double                             y() const { return m_params[1]; }
  double                             dy() const { return m_params[4]; }
  double                             z() const { return m_params[3]; }
  double                             dz() const { return m_params[5]; }
  bool                               uncrossed() const { return m_uncrossed; }
  unsigned int                       time() const { return m_t; }
  int                                deltaTime() const { return m_dt; }
  int                                clusterSize() const { return m_clusize; }
  LHCb::span<LHCb::MuonTileID const> subtiles() const {
    if ( m_uncrossed ) {
      return {&m_tile, 1};
    } else {
      return m_subtiles;
    }
  }

private:
  std::array<double, 6>           m_params = {{0}};
  LHCb::MuonTileID                m_tile;
  bool                            m_uncrossed = false;
  unsigned int                    m_t         = 0;
  int                             m_dt        = 0;
  int                             m_clusize   = 0;
  std::array<LHCb::MuonTileID, 2> m_subtiles;
};

typedef std::vector<CommonMuonHit>               CommonMuonHits;
typedef std::vector<const CommonMuonHit*>        CommonConstMuonHits;
typedef const Gaudi::Range_<CommonMuonHits>      CommonMuonHitRange;
typedef const Gaudi::Range_<CommonConstMuonHits> CommonConstMuonHitRange;