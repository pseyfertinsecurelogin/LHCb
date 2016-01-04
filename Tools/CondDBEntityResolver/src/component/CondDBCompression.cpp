// Include files
#include <string.h>

#include <iostream>
#include <memory>
#include <algorithm>
extern "C"{
#include <ZipLZMA.h>
}
#include "GaudiKernel/GaudiException.h"

// local
#include "CondDBCompression.h"
#include "base64.h"

constexpr auto MAXBUFFSIZE = 33554432; // 32 MB maximum xml buffer size

namespace {

std::string::size_type unzip_LZMA_size(const unsigned char* tgt, std::string::size_type size_if_unknown) {
  if (tgt[0]=='X'&&tgt[1]=='Z'&&tgt[2]==0) {
    // assume this is a 9 byte ROOT header in front LZMA header
    // (see ZipLZMA.c in the ROOT distribution, search for tgt[0]
    //uint64_t out_size = (  uint64_t(tgt[3])
    //                    | (uint64_t(tgt[4])<<8)
    //                    | (uint64_t(tgt[5])<<16) );
    uint64_t in_size = (  uint64_t(tgt[6])
                       | (uint64_t(tgt[7])<<8)
                       | (uint64_t(tgt[8])<<16) );
    return in_size;
  }
  return size_if_unknown;
}

}

//-----------------------------------------------------------------------------
// Implementation file for class : CondDBCompression
//
// 2013-02-14 : Liang SUN
//-----------------------------------------------------------------------------

std::string CondDBCompression::compress(const std::string& strin, const int8_t method){
	char *dest = new char[MAXBUFFSIZE]; //Output buffer
	unsigned int destLen = MAXBUFFSIZE;
	int retbit(0);
    int srcsize(0);
    switch (method){
    case 0: //LZMA method from ROOT package
        srcsize = strin.length();
        R__zipLZMA(9, &srcsize, const_cast<char*>(strin.c_str()), (int*)(&destLen), dest, &retbit);
        if (retbit == 0 )
            throw GaudiException("Error during LZMA compression", "CondDBCompression.cpp", StatusCode::FAILURE );
        destLen = retbit;
        break;
    default: //Do nothing if method not recognized
        delete [] dest;
        return strin;
    }

    std::string deststr(dest, destLen);
	delete [] dest;
	std::string out;
	base64_encode(deststr, out);
    out.insert(0, 1, '0'+method); //Set first byte to identify compression method
	return out;
}


std::string CondDBCompression::decompress(const std::string& strin){
  if (strin.empty() || !(strin[0] >= '0' && strin[0]<='9'))
    return strin; //If not starting from a digit, do nothing as the compression method is not recognizable

  const int8_t method = strin[0] - '0';
  std::string zdata = base64_decode(strin.data()+1, strin.size()-1);// Strip the method character before decoding
  int output_length = zdata.length();
  if (!output_length) return strin; // Do nothing if the string fails the compression process

  switch (method){
  case 0: {
    const unsigned char* tgt = reinterpret_cast<const unsigned char*>(zdata.data());
    auto uncompressed_size = unzip_LZMA_size(tgt,MAXBUFFSIZE);
    std::string dest(uncompressed_size, char{} ); //Output buffer
    int destSize = dest.size();
    int retbit = 0;
    // strictly speaking, writing directly to string::data invokes undefined behaviour...
    // (but at least in C++11, string::data is guaranteed to be contiguous memor )
    R__unzipLZMA(&output_length, const_cast<unsigned char*>(tgt),
                 &destSize, const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(dest.data())),
                 &retbit);
    if (retbit == 0) {
      throw GaudiException("Error during LZMA decompression", "CondDBCompression.cpp", StatusCode::FAILURE );
    }
    destSize = dest.size();
    if (retbit!=destSize) {
      if (destSize != MAXBUFFSIZE) {
          throw GaudiException("Uncompressed size not what was expected???", "CondDBCompression.cpp", StatusCode::FAILURE );
      }
      // didn't have a-priori knowledge of the size, so shrink down to the right size
      dest.resize(retbit);
    }
    return dest;
  }
  default: //Do nothing if method not recognized
    return strin;
  }
}
