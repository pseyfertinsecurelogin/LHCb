#ifndef SORTDIGITINL1_H
#define SORTDIGITINL1_H 1

#include "MuonDAQ/MuonHLTDigitFormat.h"
#include "MuonHLTBase.h"

struct SortDigitInL1  {
  constexpr bool operator()( unsigned first, unsigned second ) const noexcept 
  {
      return (( first  & MuonHLTBaseV1::MaskAddress ) >> MuonHLTBaseV1::ShiftAddress)
           < (( second & MuonHLTBaseV1::MaskAddress ) >> MuonHLTBaseV1::ShiftAddress);
  }
};



#endif // SORTDIGITINL1_H
