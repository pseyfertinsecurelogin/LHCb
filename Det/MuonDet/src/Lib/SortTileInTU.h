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
#ifndef LIB_SORTTILEINTU_H
#define LIB_SORTTILEINTU_H 1
#include <functional>
#include <iostream>

#include "Kernel/MuonTileID.h"
// Include files

/** @class SortTileInTU SortTileInTU.h Lib/SortTileInTU.h
 *
 *
 *  @author
 *  @date   2008-02-24
 */
struct SortTileInTU {
  bool operator()( const LHCb::MuonTileID& first, const LHCb::MuonTileID& second ) const;
};

bool SortTileInTU::operator()( const LHCb::MuonTileID& one, const LHCb::MuonTileID& two ) const {
  if ( one.nY() < two.nY() ) return true;
  if ( one.nY() > two.nY() ) return false;
  return one.nX() <= two.nX();
}

#endif // LIB_SORTTILEINTU_H
