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
// LHCb
#include "DetDesc/Condition.h"
#include "Event/RawEvent.h"
#include "Event/HCDigit.h"
#include "Kernel/HCCellID.h"

// Local
#include "HCRawBankDecoderHlt.h"

DECLARE_COMPONENT( HCRawBankDecoderHlt )

//=============================================================================
// Constructor
//=============================================================================
HCRawBankDecoderHlt::HCRawBankDecoderHlt(const std::string& name,
                                         ISvcLocator* pSvcLocator)
    : Decoder::HistoAlgBase(name, pSvcLocator) {

  declareProperty("DigitLocation", 
                  m_digitLocation = "Raw/HC/Sums");
 
  // Initialize search path, and then call the base method.
  m_rawEventLocations = {LHCb::RawEventLocation::HC,
                         LHCb::RawEventLocation::Default,
                         LHCb::RawEventLocation::Other};
  initRawEventSearch();
}

//=============================================================================
// Destructor
//=============================================================================
HCRawBankDecoderHlt::~HCRawBankDecoderHlt() {}

//=============================================================================
// Initialisation
//=============================================================================
StatusCode HCRawBankDecoderHlt::initialize() {

  StatusCode sc = Decoder::HistoAlgBase::initialize();
  if (sc.isFailure()) return sc;

  m_masked.set();
  m_station.fill(0);
  // Check if the mapping is available in the conditions database.
  const std::string location = "Conditions/ReadoutConf/HC/Mapping";
  if (!existDet<Condition>(location)) {
    return Error("Cannot find " + location + " in database", 
                 StatusCode::SUCCESS);
  }
  registerCondition(location, m_cond, &HCRawBankDecoderHlt::cacheMapping);
  // First update.
  sc = updMgrSvc()->update(this);
  if (sc.isFailure()) {
    return Error("Cannot update mapping.", StatusCode::SUCCESS);
  }
  return StatusCode::SUCCESS;
}

//=============================================================================
// Execution
//=============================================================================
StatusCode HCRawBankDecoderHlt::execute() {

  // Create containers and pass their ownership to the TES.
  LHCb::HCDigits* digits = new LHCb::HCDigits();
  put(digits, m_digitLocation);

  // Retrieve the raw event.
  LHCb::RawEvent* rawEvent = findFirstRawEvent();
  if (!rawEvent) return Warning("Cannot find raw event", StatusCode::SUCCESS);

  // Retrieve the HC raw banks.
  auto banks = rawEvent->banks(LHCb::RawBank::HC);
  if (banks.empty()) {
    return Warning("Cannot find HC raw banks", StatusCode::SUCCESS);
  }

  const unsigned int nStations = 5;
  std::vector<int> sums(nStations, 0);
  for (const auto& bank: banks) {
    // Make sure the bank is not corrupted.
    if (LHCb::RawBank::MagicPattern != bank->magic()) {
      error() << "Bad magic pattern" << endmsg;
      continue;
    }
    const unsigned int source = bank->sourceID();
    const unsigned int version = bank->version();
    if (UNLIKELY(msgLevel(MSG::DEBUG))) {
      debug() << "Source: " << source << ", version: " << version << endmsg;
    }
    // Decode the raw bank.
    if (version == 2 || version == 3) {
      decode(*bank, sums);
    } else {
      return Error("Unknown raw bank version: " + std::to_string(version),
                   StatusCode::SUCCESS);
    }
  }

  for (unsigned int i = 0; i < nStations; ++i) {
    // Crate a "fake" Cell ID, using just the station number.
    LHCb::HCCellID cell(i); 
    LHCb::HCDigit* digit = new LHCb::HCDigit(sums[i]);
    digits->insert(digit, cell);
  }
  return StatusCode::SUCCESS;

}

//=============================================================================
// Decoding of raw banks (compressed format, PS/SPD)
//=============================================================================
bool HCRawBankDecoderHlt::decode(const LHCb::RawBank& bank ,
                                 std::vector<int>& sums) const {

  const uint32_t* data = bank.data();
  unsigned int nWords = bank.size() / sizeof(uint32_t);
  while (nWords > 0) {
    // Read the bank header.
    uint32_t word = *data++;
    --nWords;
    // Length of the trigger part.
    unsigned int lenTrig = word & 0x7F;
    // Length of the ADC part.
    unsigned int lenAdc = (word >> 7) & 0x7F;
    // Crate number.
    const unsigned int crate = (word >> 18) & 0x1F; 
    // Skip the trigger words.
    const unsigned int nSkip = (lenTrig + 3) / 4;
    data += nSkip;
    nWords -= nSkip;
    // Read the ADC data
    unsigned int offset = 32;
    while (lenAdc > 0) {
      if (32 == offset) {
        // Read the next word.
        word = *data++;
        --nWords;
        offset = 0;
      }
      const int adc = (word >> offset) & 0x3FF;
      const unsigned int channel = (word >> (offset + 10)) & 0x3F;
      --lenAdc;
      offset += 16;
      // Skip non-active channels.
      unsigned int station = 0;
      if (crate == m_crateB) {
        if (m_masked[channel]) continue;
        station = m_station[channel];
      } else if (crate == m_crateF) {
        if (m_masked[channel + 64]) continue;
        station = m_station[channel + 64];
      } else {
        warning() << "Unexpected crate number " << crate << endmsg;
        continue;
      }
      if (UNLIKELY(msgLevel(MSG::VERBOSE))) {
        verbose() << format("Channel: %06u ", channel) 
                  << format("ADC: %06i", adc) 
                  << format("Station: %06u", station) << endmsg;
      }
      sums[station] += adc;
    }
  }
  return true;
}

//=============================================================================
// Update the channel map using the conditions database.
//=============================================================================
StatusCode HCRawBankDecoderHlt::cacheMapping() {

  m_crateB = m_cond->param<int>("CrateB");
  m_crateF = m_cond->param<int>("CrateF");

  const unsigned int nChannels = 64;
  m_masked.set();
  m_station.fill(0);
  // Reset the list of station numbers and mask flags.
  const std::vector<std::string> stations = {"B0", "B1", "B2", "F1", "F2"};
  const unsigned int nStations = stations.size();
  for (unsigned int i = 0; i < nStations; ++i) {
    const std::string st = stations[i];
    std::vector<int> channels = m_cond->paramVect<int>("Channels" + st);
    std::vector<int> masks = m_cond->paramVect<int>("Masks" + st);
    const unsigned int offset = i > 2 ? nChannels : 0;
    if (channels.size() < 4 || masks.size() < 4) {
      return Error("Invalid configuration for station " + st);
    }
    // Set the station number and enable active channels.
    for (unsigned int j = 0; j < 4; ++j) {
      const int channel = channels[j];
      m_station[channel + offset] = i; 
      if (masks[j] == 0) m_masked[channel + offset] = false;
    }
  }
  if (msgLevel(MSG::DEBUG)) {
    for (unsigned int i = 0; i < nChannels; ++i) {
      debug() << "Channel" << endmsg;
      const std::string stb = m_masked[i] ? "--" : stations[m_station[i]];
      const std::string stf = m_masked[i + nChannels] ? "--" : stations[m_station[i + nChannels]];
      debug() << format("   %2d   ", i) << stb << "  " << stf << endmsg;
    }
  }
  return StatusCode::SUCCESS;
}
