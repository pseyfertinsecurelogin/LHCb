#ifndef OTRAWBANKENCODER_H
#define OTRAWBANKENCODER_H 1

#include <stddef.h>
// Include files
// from std
#include <string>
#include <vector>

// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/extends.h"
// Event
#include "Kernel/OTChannelID.h"
// Channel map tool
#include "OTDAQ/IOTChannelMapTool.h"
// Interface
#include "OTDAQ/IOTRawBankEncoder.h"

class IInterface;
struct IOTChannelMapTool;
struct IOTRawBankEncoder;

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
  OTRawBankEncoder( const std::string& type,
		    const std::string& name,
		    const IInterface* parent );

  /// Tool initialization
  StatusCode initialize() override;

  /// Abstract virtual in IOTRawBankEncoder
  StatusCode encodeChannels( const std::vector<LHCb::OTChannelID>& channels ) const override;

private:
  /// Some handy typedefs
  typedef std::vector< OTDAQ::OTBank > OTBanks;
  typedef std::vector< unsigned int >  OTRawBank;

  void createBanks();

  /// Returns the bank == Tell1 number for a given channel
  size_t channelToBank( const LHCb::OTChannelID& channel ) const;
  /// Creates a RawBank for a given bank
  const OTRawBank& createRawBank(const OTDAQ::OTBank& bank) const;
  /// Clear OTBanks and OTRawbank after each event
  void clear() const;

  IOTChannelMapTool* m_channelmaptool;   ///< Pointer to IOTChannelMapTool
  bool               m_addEmptyBanks;    ///< Falg to add empty banks
  std::string        m_rawEventLocation; ///< Location of RawEvent
  mutable OTBanks    m_banks;            ///< Vector of banks
  mutable OTRawBank  m_rawBank;          ///< A raw bank. This goes into the raw buffer
};

inline size_t OTRawBankEncoder::channelToBank( const LHCb::OTChannelID& channel ) const {
  /// Tells1 start from 1 and go to 48
  return ( ( channel.station() - 1 )*16 + channel.layer()*4 + ( channel.quarter() + 1 ) );
}

#endif // OTRAWBANKENCODER_H
