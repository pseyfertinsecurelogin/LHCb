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
