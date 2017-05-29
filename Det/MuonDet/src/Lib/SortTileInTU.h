#ifndef LIB_SORTTILEINTU_H
#define LIB_SORTTILEINTU_H 1
#include <iostream>
#include <functional>

#include "Kernel/MuonTileID.h"
// Include files

/** @class SortTileInTU SortTileInTU.h Lib/SortTileInTU.h
 *
 *
 *  @author
 *  @date   2008-02-24
 */
struct SortTileInTU {
  bool operator()(  const LHCb::MuonTileID& first,
                    const LHCb::MuonTileID& second ) const;
};

bool SortTileInTU::operator()(const LHCb::MuonTileID& one,
                              const LHCb::MuonTileID& two )const{
  if(one.nY()<two.nY())return true;
  if(one.nY()>two.nY())return false;
  return one.nX()<=two.nX();
}



#endif // LIB_SORTTILEINTU_H
