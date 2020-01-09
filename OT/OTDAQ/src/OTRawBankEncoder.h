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
#ifndef OTRAWBANKENCODER_H
#define OTRAWBANKENCODER_H 1

// Include files
// from std
#include <string>

// from Gaudi
#include "GaudiAlg/GaudiTool.h"

// Event
#include "Kernel/OTChannelID.h"

// Interface
#include "OTDAQ/IOTRawBankEncoder.h"

// Channel map tool
#include "OTDAQ/IOTChannelMapTool.h"

namespace OTDAQ {
  class OTBank;
}

/** @class OTRawBankEncoder OTRawBankEncoder.h
 *
 *  Encodes the channels and puts them in the raw bank.
 *
 *  @author Jan Amoraal
 *
 *  @date   2008-05-22
 */

namespace nTell1s {
  enum nTell1s { v2008 = 48u };
}

namespace nGols {
  enum nGols { v2008 = 9u };
}

class OTRawBankEncoder : public extends<GaudiTool, IOTRawBankEncoder> {

public:
  /// Standard constructor
  using extends::extends;

  /// Tool initialization
  StatusCode initialize() override;

  /// Abstract virtual in IOTRawBankEncoder
  StatusCode encodeChannels( const std::vector<LHCb::OTChannelID>& channels ) const override;

private:
  /// Some handy typedefs
  typedef std::vector<OTDAQ::OTBank> OTBanks;
  typedef std::vector<unsigned int>  OTRawBank;

  void createBanks();

  /// Returns the bank == Tell1 number for a given channel
  size_t channelToBank( const LHCb::OTChannelID& channel ) const;
  /// Creates a RawBank for a given bank
  const OTRawBank& createRawBank( const OTDAQ::OTBank& bank ) const;
  /// Clear OTBanks and OTRawbank after each event
  void clear() const;

  PublicToolHandle<IOTChannelMapTool> m_channelmaptool{
      this, "ChannelMap", "OTChannelMapTool"}; ///< Pointer to IOTChannelMapTool,access to the channel map
  Gaudi::Property<bool>                m_addEmptyBanks{this, "AddEmptyBanks", true, "Flag to add empty banks"};
  DataObjectReadHandle<LHCb::RawEvent> m_rawEventLocation{this, "RawEventLocation", LHCb::RawEventLocation::Default,
                                                          "Location of RawEvent"};
  mutable OTBanks                      m_banks;   ///< Vector of banks
  mutable OTRawBank                    m_rawBank; ///< A raw bank. This goes into the raw buffer
};

inline size_t OTRawBankEncoder::channelToBank( const LHCb::OTChannelID& channel ) const {
  /// Tells1 start from 1 and go to 48
  return ( ( channel.station() - 1 ) * 16 + channel.layer() * 4 + ( channel.quarter() + 1 ) );
}

#endif // OTRAWBANKENCODER_H
