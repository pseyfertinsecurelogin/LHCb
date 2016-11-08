#ifndef ODIN_CODEC_H
#define ODIN_CODEC_H

#include "Event/ODIN.h"
#include "Event/RawBank.h"

namespace LHCb { namespace ODINCodec {
  namespace Bank {
    constexpr const unsigned int VERSION = 6;
    const size_t SIZE = 10 * sizeof(int);
  }
  LHCb::ODIN decode( const LHCb::RawBank& bank, const bool ignoreBankVersion = false );
  std::array<unsigned int, Bank::SIZE> encode( const LHCb::ODIN& odin );
} } // LHCb::ODINCodec

#endif // ODIN_CODEC_H
