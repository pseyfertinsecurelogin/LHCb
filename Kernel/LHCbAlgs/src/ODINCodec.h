#ifndef ODIN_CODEC_H
#define ODIN_CODEC_H

#include "Event/ODIN.h"
#include "Event/RawBank.h"
#include <array>

namespace LHCb { namespace ODINCodec {
  namespace Bank {
    constexpr const unsigned int VERSION = 6;
    using array = std::array<unsigned int, 10>;
    const size_t SIZE = sizeof(array);
  }
  LHCb::ODIN decode( const LHCb::RawBank& bank, const bool ignoreBankVersion = false );
  Bank::array encode( const LHCb::ODIN& odin );
} } // LHCb::ODINCodec

#endif // ODIN_CODEC_H
