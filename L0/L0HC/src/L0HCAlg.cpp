#include "Event/HCDigit.h"
#include "Event/L0ProcessorData.h"

// Local
#include "L0HCAlg.h"

DECLARE_ALGORITHM_FACTORY(L0HCAlg)

//=============================================================================
// Constructor
//=============================================================================
L0HCAlg::L0HCAlg(const std::string& name, ISvcLocator* pSvcLocator)
    : L0AlgBase(name, pSvcLocator) {

  declareProperty("DigitLocation",
                  m_digitLocation = LHCb::HCDigitLocation::Default);
  declareProperty("ChannelsB0", m_channelsB0);
  declareProperty("ChannelsB1", m_channelsB1);
  declareProperty("ChannelsB2", m_channelsB2);
  declareProperty("ChannelsF1", m_channelsF1);
  declareProperty("ChannelsF2", m_channelsF2);
}

//=============================================================================
// Destructor
//=============================================================================
L0HCAlg::~L0HCAlg() {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode L0HCAlg::initialize() {
  StatusCode sc = L0AlgBase::initialize();
  if (sc.isFailure()) return sc;

  // Check if the mapping is available in the conditions database.
  const std::string location = "Conditions/ReadoutConf/HC/Mapping";
  if (existDet<Condition>(location)) {
    registerCondition(location, m_cond, &L0HCAlg::cacheMapping);
    // First update.
    sc = updMgrSvc()->update(this);
    if (sc.isFailure()) {
      return Error("Cannot update mapping.", StatusCode::FAILURE);
    }
  } else {
    warning() << "Cannot find " << location << " in database" << endmsg;
    mapChannels(m_channelsB0, 0, true);
    mapChannels(m_channelsB1, 1, true);
    mapChannels(m_channelsB2, 2, true);
    mapChannels(m_channelsF1, 1, false);
    mapChannels(m_channelsF2, 2, false);
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
// Execute
//=============================================================================
StatusCode L0HCAlg::execute() {
  // Retrieve Herschel digits
  LHCb::HCDigits* digits = getIfExists<LHCb::HCDigits>(m_digitLocation);
  if (!digits) {
    return Error("Cannot retrieve HC digits from " + m_digitLocation,
                 StatusCode::SUCCESS);
  }

  unsigned int multB = 0;
  // Construct B-side sum
  for (unsigned int i = 0; i < 3; ++i) {
    // Loop over all quadrants on this station
    for (unsigned int j = 0; j < 4; ++j) {
      // Find the cell ID for this quadrant
      LHCb::HCCellID id(m_channels[i][j]);
      // Retrieve the digit
      const LHCb::HCDigit* digit = digits->object(id);
      if (!digit) {
        const std::string ch = "B" + std::to_string(i) + std::to_string(j);
        Warning("Cannot retrieve digit for " + ch).ignore();
        continue;
      }
      if (digit->adc() > 300) ++multB;
    }
  }

  unsigned int multF = 0;
  // Construct F-side sum
  for (unsigned int i = 3; i < 5; ++i) {
    // Loop over all quadrants on this station
    for (unsigned int j = 0; j < 4; ++j) {
      // Find the cell ID for this quadrant
      LHCb::HCCellID id(m_channels[i][j]);
      // Retrieve the digit
      const LHCb::HCDigit* digit = digits->object(id);
      if (!digit) {
        const std::string ch = "F" + std::to_string(i - 2) + std::to_string(j);
        Warning("Cannot retrieve digit for " + ch).ignore();
        continue;
      }
      if (digit->adc() > 300) ++multF;
    }
  }

  // Save the candidates in HCProcessor data location (for L0DU)
  LHCb::L0ProcessorDatas* L0HC = new LHCb::L0ProcessorDatas();
  put(L0HC, LHCb::L0ProcessorDataLocation::HC);

  // Using the same bit-shift as for Calo Pi0Global/Local data word; Et goes in bits 1-8 of the 32
  const unsigned int codeB = 0x10000 + (multB << L0DUBase::Calo::Et::Shift );
  L0HC->add(new LHCb::L0ProcessorData(L0DUBase::Fiber::CaloPi0Global, codeB));

  const unsigned int codeF = 0x10000 + (multF << L0DUBase::Calo::Et::Shift );
  L0HC->add(new LHCb::L0ProcessorData(L0DUBase::Fiber::CaloPi0Local, codeF));

  // Debug now the L0 candidates
  if (msgLevel(MSG::DEBUG)) {
    for (LHCb::L0ProcessorData* cand : *L0HC) {
      debug() << format("Key %2d Word %8x", cand->key(), cand->word())
              << endmsg;
    }
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
//  Save the Herschel channel mapping
//=============================================================================
StatusCode L0HCAlg::cacheMapping() {
  // Extract crate IDs and channel numbers from CondDB
  m_crateB = m_cond->param<int>("CrateB");
  m_crateF = m_cond->param<int>("CrateF");
  m_channelsB0 = m_cond->paramVect<int>("ChannelsB0");
  m_channelsB1 = m_cond->paramVect<int>("ChannelsB1");
  m_channelsB2 = m_cond->paramVect<int>("ChannelsB2");
  m_channelsF1 = m_cond->paramVect<int>("ChannelsF1");
  m_channelsF2 = m_cond->paramVect<int>("ChannelsF2");
  mapChannels(m_channelsB0, 0, true);
  mapChannels(m_channelsB1, 1, true);
  mapChannels(m_channelsB2, 2, true);
  mapChannels(m_channelsF1, 1, false);
  mapChannels(m_channelsF2, 2, false);
  return StatusCode::SUCCESS;
}

//=============================================================================
// Setup the channel map for a given station.
//=============================================================================
bool L0HCAlg::mapChannels(const std::vector<int>& channels,
                          const unsigned int station, const bool bwd) {
  // Indices 0,1,2 are B-side; 3,4 are F-side
  const unsigned int offset = bwd ? 0 : 2;

  // Check if the input is valid and if not return 0 mapping
  if (channels.size()) {
    std::string s = bwd ? "B" : "F";
    s += std::to_string(station);
    warning() << "Invalid channel map for station " << s << endmsg;
    m_channels[station + offset].assign(4, 0);
    return false;
  }

  // Determine whether dealing with F-side or B-side crate
  const unsigned int crate = bwd ? m_crateB : m_crateF;

  // Resize mapping vector for 4 quadrants
  m_channels[station + offset].resize(4);

  // Loop over the four quadrants and assign channel numbers
  for (unsigned int i = 0; i < 4; ++i) {
    if (channels[i] < 0) {
      std::string s = bwd ? "B" : "F";
      s += std::to_string(station);
      warning() << "Invalid channel number " << channels[i] << " for quadrant "
                << s << i << endmsg;
      continue;
    }
    m_channels[station + offset][i] = (crate << 6) | channels[i];
  }
  return true;
}
