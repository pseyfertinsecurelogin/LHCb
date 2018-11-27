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
#include "ODINCodecBaseTool.h"

//=============================================================================
// IMPLEMENTATION of ODINCodecBaseTool
//=============================================================================

#include <sstream>
#include <memory>
#include <algorithm>
// from LHCb
#include "Event/ODIN.h"
#include "Event/RawBank.h"
#include "Event/RawEvent.h"
#include "DAQKernel/DecoderToolBase.h"
#include "ODINCodec.h"

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
ODINCodecBaseTool::ODINCodecBaseTool( const std::string& type,
                                      const std::string& name,
                                      const IInterface* parent )
  : base_class( type, name , parent )
{
  declareProperty("Force", m_force = false,
                  "If already present, override the destination object.");
  declareProperty("IgnoreUnknownBankVersion", m_ignoreBankVersion = false,
                  "Do not stop in case of unknown bank version number, assuming"
                  " it is binary compatible with the latest known version.");
}
//=============================================================================
// Decode
//=============================================================================
LHCb::ODIN* ODINCodecBaseTool::i_decode(const LHCb::RawBank* bank, LHCb::ODIN* odin)
{
  // Check the passed pointers
  Assert( bank, "Called without a RawBank object (pointer NULL)");

  if ( !odin )
  {
    return new LHCb::ODIN{LHCb::ODINCodec::decode( *bank, m_ignoreBankVersion )};
  } else {
    *odin = LHCb::ODINCodec::decode( *bank, m_ignoreBankVersion );
    return odin;
  }
}
//=============================================================================
// Encode
//=============================================================================
LHCb::RawBank* ODINCodecBaseTool::i_encode(const LHCb::ODIN *odin) {
  // Check the passed pointer
  Assert( odin, "Called without an ODIN object (pointer NULL)");

  using LHCb::ODINCodec::Bank::VERSION;
  using LHCb::ODINCodec::Bank::SIZE;

  const unsigned int version = odin->version();
  if (version && (version != VERSION)) {
    Warning("Trying to convert ODIN object of version " + std::to_string(version) +
            " to bank version " + std::to_string(VERSION)).ignore();
  }

  // Create the new bank
  // Note that we cannot delete it, so better that there is no failure after
  // this line.
  auto data = LHCb::ODINCodec::encode(*odin);
  LHCb::RawBank* bank = LHCb::RawEvent::createBank(0, LHCb::RawBank::ODIN, VERSION, SIZE, data.data());
  return bank;
}
//=============================================================================
