#ifndef MUONHLTBASE_H
#define MUONHLTBASE_H 1

// Include files

/** @class MuonHLTBase MuonHLTBase.h
 *
 *
 *  @author Alessia Satta
 *  @date   2004-01-23
 */
namespace MuonHLTBaseDC06
{
  constexpr unsigned int BitsAddress   = 8;
  constexpr unsigned int BitsTime      = 4;

  constexpr unsigned int BitsFirstHalf     = 16;
  constexpr unsigned int BitsSecondHalf     = 16;

  constexpr unsigned int ShiftAddress  = 0;
  constexpr unsigned int ShiftTime     = ShiftAddress +BitsAddress;

  constexpr unsigned int ShiftFirstHalf    = 0;
  constexpr unsigned int ShiftSecondHalf    = BitsFirstHalf;

  constexpr unsigned int  MaskAddress     =
  ( ( ( (unsigned int) 1 ) << BitsAddress  ) - 1  ) << ShiftAddress  ;
  constexpr unsigned int  MaskTime     =
  ( ( ( (unsigned int) 1 ) << BitsTime  ) - 1  ) << ShiftTime  ;
  constexpr unsigned int  MaskFirstHalf    =
  ( ( ( (unsigned int) 1 ) << BitsFirstHalf  ) - 1  ) << ShiftFirstHalf  ;
  constexpr unsigned int  MaskSecondHalf    =
  ( ( ( (unsigned int) 1 ) << BitsSecondHalf  ) - 1  ) << ShiftSecondHalf
;


}
namespace MuonHLTBaseV1
{
  constexpr unsigned int BitsAddress   = 12;
  constexpr unsigned int BitsTime      = 4;

  constexpr unsigned int BitsFirstHalf     = 16;
  constexpr unsigned int BitsSecondHalf     = 16;

  constexpr unsigned int ShiftAddress  = 0;
  constexpr unsigned int ShiftTime     = ShiftAddress +BitsAddress;

  constexpr unsigned int ShiftFirstHalf    = 0;
  constexpr unsigned int ShiftSecondHalf    = BitsFirstHalf;

  constexpr unsigned int  MaskAddress     =
  ( ( ( (unsigned int) 1 ) << BitsAddress  ) - 1  ) << ShiftAddress  ;
  constexpr unsigned int  MaskTime     =
  ( ( ( (unsigned int) 1 ) << BitsTime  ) - 1  ) << ShiftTime  ;
  constexpr unsigned int  MaskFirstHalf    =
  ( ( ( (unsigned int) 1 ) << BitsFirstHalf  ) - 1  ) << ShiftFirstHalf  ;
  constexpr unsigned int  MaskSecondHalf    =
  ( ( ( (unsigned int) 1 ) << BitsSecondHalf  ) - 1  ) << ShiftSecondHalf
;


}


#endif // MUONHLTBASE_H
