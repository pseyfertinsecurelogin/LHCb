#ifndef _UTDecoder_H 
#define _UTDecoder_H 
#include "SiDAQ/SiRawBankDecoder.h"
#include "UTClusterWord.h"
typedef SiRawBankDecoder<UTClusterWord > UTDecoder;
#endif
