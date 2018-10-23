/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "RZip.h"

#include "Event/RawEvent.h"
#include "Event/PackedTrack.h"
#include "Event/PackedCaloCluster.h"
#include "Event/PackedRichPID.h"
#include "Event/PackedMuonPID.h"
#include "Event/PackedCaloHypo.h"
#include "Event/PackedProtoParticle.h"
#include "Event/PackedParticle.h"
#include "Event/PackedVertex.h"
#include "Event/PackedRecVertex.h"
#include "Event/PackedFlavourTag.h"
#include "Event/PackedRelations.h"
#include "Event/PackedPartToRelatedInfoRelation.h"
#include "Event/PackedCaloDigit.h"
#include "Event/PackedCluster.h"
#include "Event/PackedCaloAdc.h"

#include "HltPackedDataWriter.h"
#include "HltPackedDataDecoder.h"

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( HltPackedDataDecoder )


HltPackedDataDecoder::HltPackedDataDecoder(const std::string& name,
                                           ISvcLocator* pSvcLocator)
  : HltRawBankDecoderBase(name, pSvcLocator)
{
  //new for decoders, initialize search path, and then call the base method
  m_rawEventLocations = {LHCb::RawEventLocation::Trigger, LHCb::RawEventLocation::Copied, LHCb::RawEventLocation::Default};
  initRawEventSearch();
  declareProperty("EnableChecksum", m_enableChecksum = false);
  declareProperty("ContainerMap", m_containerMap);
  // The default m_sourceID=0 triggers a warning in HltRawBankDecoderBase::initialize
  // Since we only care about HLT2 persistence, set it explicitly:
  setProperty("SourceID", kSourceID_Hlt2);
  m_rawEventLocations.push_back(LHCb::RawEventLocation::PersistReco);
}

template<typename PackedData>
void HltPackedDataDecoder::register_object() {
  m_loaders[PackedData::classID()] =
    [this](const std::string& location){
      return this->loadObject<PackedData>(location);
    };
}

StatusCode HltPackedDataDecoder::initialize() {
  const StatusCode sc = HltRawBankDecoderBase::initialize();
  if (sc.isFailure()) return sc;

  register_object<LHCb::PackedTracks>();
  register_object<LHCb::PackedRichPIDs>();
  register_object<LHCb::PackedMuonPIDs>();
  register_object<LHCb::PackedCaloHypos>();
  register_object<LHCb::PackedProtoParticles>();
  register_object<LHCb::PackedCaloClusters>();
  register_object<LHCb::PackedParticles>();
  register_object<LHCb::PackedVertices>();
  register_object<LHCb::PackedRecVertices>();
  register_object<LHCb::PackedFlavourTags>();
  register_object<LHCb::PackedRelations>();
  register_object<LHCb::PackedRelatedInfoRelations>();
  register_object<LHCb::PackedCaloDigits>();
  register_object<LHCb::PackedClusters>();
  register_object<LHCb::PackedCaloAdcs>();

  if (UNLIKELY(m_enableChecksum)) {
    m_checksum.reset( new PackedDataPersistence::PackedDataChecksum());
  }

  return StatusCode::SUCCESS;
}


template<typename T>
std::pair<DataObject*, size_t> HltPackedDataDecoder::loadObject(const std::string& location) {
  auto object = new T{};
  put(object, location);
  auto nBytesRead = m_buffer.load(*object);
  if (UNLIKELY(m_enableChecksum)) m_checksum->processObject(*object, location);
  return { object, nBytesRead };
}


StatusCode HltPackedDataDecoder::execute() {
  if (UNLIKELY(msgLevel(MSG::DEBUG))) debug() << "==> Execute" << endmsg;

  auto* rawEvent = findFirstRawEvent();
  if (!rawEvent) {
    return Error("Raw event not found!");
  }

  const auto& rawBanksConst = rawEvent->banks(LHCb::RawBank::DstData);
  if (rawBanksConst.empty()) {
    return Warning("No HltPackedData raw bank (the DstData bank) in raw event. Quitting.",
                   StatusCode::SUCCESS, 10);
  }

  std::vector<const LHCb::RawBank*> rawBanks;
  rawBanks.reserve(rawBanksConst.size());
  std::copy(std::begin(rawBanksConst), std::end(rawBanksConst),
            std::back_inserter(rawBanks));

  const auto* rawBank0 = *(rawBanks.begin());

  // Check we know how to decode this version
  if (rawBank0->version() < 2 || rawBank0->version() > kVersionNumber) {
    return Error("HltPackedData raw bank version is not supported.");
  }

  // Put the banks into the right order
  std::sort(
    std::begin(rawBanks), std::end(rawBanks),
    [](const LHCb::RawBank* lhs, const LHCb::RawBank* rhs) {
      auto l = HltPackedDataWriter::partID(lhs->sourceID());
      auto r = HltPackedDataWriter::partID(rhs->sourceID());
      return l < r;
    });

  // Check that the banks have sequential part IDs
  unsigned int payloadSize = 0;
  for (unsigned int i = 0; i < rawBanks.size(); ++i) {
    if (HltPackedDataWriter::partID(rawBanks[i]->sourceID()) != i)
      return Error("Part IDs for HltPackedData banks are not sequential. Quitting.", StatusCode::SUCCESS, 20);
    payloadSize += rawBanks[i]->size();
  }

  // Collect the data into a contiguous buffer
  std::vector<uint8_t> payload;
  payload.reserve(payloadSize);
  for(const auto* bank: rawBanks) {
    std::copy(bank->begin<uint8_t>(), bank->end<uint8_t>(), std::back_inserter(payload));
  }

  auto compression = HltPackedDataWriter::compression(rawBank0->sourceID());
  if (UNLIKELY(msgLevel(MSG::DEBUG))) {
    debug() << "Compression " << compression << ", payload size " << payload.size() << endmsg;
  }

  // Decompress the payload and load into the buffer
  switch (compression) {
    case HltPackedDataWriter::NoCompression: {
      m_buffer.init(payload);
      break;
    }
    case HltPackedDataWriter::ZLIB:
    case HltPackedDataWriter::LZMA: {
      if (!m_buffer.init(payload, true))
        return Error("Failed to decompress HltPackedData. Quitting.");
      break;
    }
    default: {
      return Error("Unknown compression method. Quitting.", StatusCode::SUCCESS, 20);
      break;
    }
  }

  // Get the map of ids to locations (may differ between events)
  const auto& locationsMap = packedObjectLocation2string(tck(*rawEvent));

  std::vector<int32_t> linkLocationIDs;

  if (m_enableChecksum) {
    m_checksum->reset();
    debug() << "Control checksum " << m_checksum->checksum("") << endmsg;
  }

  // Do the actual loading of the objects
  while (!m_buffer.eof()) {
    // Load the metadata
    CLID classID = m_buffer.load<uint32_t>();
    auto locationID = m_buffer.load<int32_t>();
    m_buffer.load(linkLocationIDs);
    auto storedObjectSize = m_buffer.load<uint32_t>();

    auto locationIt = locationsMap.find(locationID);
    if (locationIt == std::end(locationsMap)) {
      error() << "Packed object location not found in ANNSvc for id=" << locationID
              << ". Skipping reading the container!" << endmsg;
      m_buffer.skip(storedObjectSize);
      continue;
    }
    std::string containerPath = locationIt->second;

    if (UNLIKELY(msgLevel(MSG::DEBUG))) {
      debug() << "Reading " << storedObjectSize << " bytes "
              << "for object with CLID " << classID << " into TES location "
              << containerPath << endmsg;
    }

    const auto it = m_loaders.find(classID);
    if (it == m_loaders.end()) {
      error() << "Unknown class ID " << classID << endmsg;
      m_buffer.skip(storedObjectSize);
      continue;
    }
    // Load the packed object
    auto ret = (it->second)(containerPath);
    auto dataObject = ret.first;
    auto readObjectSize = ret.second;

    if (readObjectSize != storedObjectSize) {
      fatal() << "Loading of object (CLID=" << classID << ") "
              << "to " << containerPath << " "
              << "consumed " << readObjectSize << " bytes, "
              << "but " << storedObjectSize << " were stored!" << endmsg;
      return StatusCode::FAILURE;
    }

    // Restore the links to other containers on the TES
    for (const auto& linkLocationID: linkLocationIDs) {
      auto persistedLocation = locationsMap.find(linkLocationID);
      if (persistedLocation == std::end(locationsMap)) {
        Error("Packed object location not found in ANNSvc for id=" +
              std::to_string(linkLocationID) +
              ". Skipping this link, unpacking may fail!").ignore();
        continue;
      }

      auto location = persistedLocation->second;
      auto mappedLocation = m_containerMap.find(persistedLocation->second);
      if (mappedLocation != std::end(m_containerMap)) {
        location = mappedLocation->second;
      }

      dataObject->linkMgr()->addLink(location, nullptr);
    }
  }

  if (UNLIKELY(msgLevel(MSG::DEBUG) && m_enableChecksum)) {
    for (const auto& x: m_checksum->checksums())
      debug() << "Packed data checksum for '" << x.first << "' = " << x.second << endmsg;
  }

  m_buffer.clear();
  // TODO do we / can we shrink the buffer if it takes too much space for an event

  return StatusCode::SUCCESS;
}


StatusCode HltPackedDataDecoder::finalize() {
  if (UNLIKELY(m_enableChecksum)) {
    for (const auto& x: m_checksum->checksums())
      info() << "Packed data checksum for '" << x.first << "' = " << x.second << endmsg;
    m_checksum.reset();
  }
  return HltRawBankDecoderBase::finalize();  // must be called after all other actions
}
