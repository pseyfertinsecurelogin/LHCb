#ifndef SORTDIGITINODE_H 
#define SORTDIGITINODE_H 1

#include "MuonDAQ/MuonHLTDigitFormat.h"
#include "MuonHLTBase.h"

struct SortDigitInODE {
  bool operator()(  const unsigned int first, 
                    const unsigned int second ) const {
     return (( first  & MuonHLTBaseDC06::MaskAddress ) >> MuonHLTBaseDC06::ShiftAddress) 
          < (( second & MuonHLTBaseDC06::MaskAddress ) >> MuonHLTBaseDC06::ShiftAddress);

  }
};


#endif // SORTDIGITINODE_H
