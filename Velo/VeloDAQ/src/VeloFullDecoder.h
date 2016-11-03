#ifndef VELOTELL1ALGORITHM_VELOFULLDECODER_H
#define VELOTELL1ALGORITHM_VELOFULLDECODER_H 1

// Include files
#include "VeloEvent/VeloFullBank.h"
#include "Tell1Kernel/VeloDecodeConf.h"
#include "Tell1Kernel/VeloDecodeCore.h"

/** @class VeloFullDecoder VeloFullDecoder.h VeloFullDecoder.h
 *
 *
 *  @author Tomasz Szumlak
 *  @date   2006-04-11
 */

class VeloFullDecoder final {

  enum initialShifts{
    NoShift=0,
    EmptyData=2,
    ADCShift=4
  };

public:

  /// Standard constructor
  VeloFullDecoder(const int decoderType);
  void decode(VeloFullBank* inData, VeloTELL1::sdataVec& decodedData) const;

private:

  void decoderIni(const int decoderType);

  int m_initialShift;  // if decoding adcs skip headers
  int m_dataLenght;    // lenght of the ALink
  int m_wordsToDecode; // words to decode

};
#endif // VELOTELL1ALG_VELOFULLDECODER_H
//
