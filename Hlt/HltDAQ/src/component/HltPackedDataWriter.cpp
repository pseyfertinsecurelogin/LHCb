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

#include "PackedDataChecksum.h"
#include "PackedDataBuffer.h"
#include "HltPackedDataWriter.h"

static const Gaudi::StringKey PackedObjectLocations{"PackedObjectLocations"};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( HltPackedDataWriter )


template<typename PackedData>
void HltPackedDataWriter::register_object() {
  m_savers[PackedData::classID()] =
    [this](const DataObject& dataObject, const std::string& location){
      return this->saveObject<PackedData>(dataObject, location);
    };
}

StatusCode HltPackedDataWriter::initialize() {
  const StatusCode sc = GaudiAlgorithm::initialize();
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

  info() << "Configured to persist containers ";
  for (const auto& path: m_packedContainers) {
    info() << " '" << path << "',";
  }
  info() << endmsg;

  m_hltANNSvc = service("HltANNSvc");

  if (UNLIKELY(m_enableChecksum)) {
    m_checksum.emplace();
  }
  return sc;
}


template<typename T>
size_t HltPackedDataWriter::saveObject(const DataObject& dataObject, const std::string& location) {
  const auto& object = dynamic_cast<const T&>(dataObject);

  // Reserve bytes for the size of the object
  auto posObjectSize = m_buffer.saveSize(0).first;
  // Save the object actual object and see how many bytes were written
  auto objectSize = m_buffer.save(object).second;
  // Save the object's size in the correct position
  m_buffer.saveAt<uint32_t>(objectSize, posObjectSize);

  if (UNLIKELY(m_enableChecksum)) { m_checksum->processObject(object, location); }
  return objectSize;
}


StatusCode HltPackedDataWriter::execute() {
  if (UNLIKELY(msgLevel(MSG::DEBUG))) debug() << "==> Execute" << endmsg;

  // Get the raw event
  auto* rawEvent = getIfExists<LHCb::RawEvent>(m_outputRawEventLocation);
  if (!rawEvent) {
    return Error("No RawEvent at " + m_outputRawEventLocation.value(), StatusCode::SUCCESS, 20);
  }

  m_buffer.clear();

  if (m_enableChecksum) {
    m_checksum->reset();
    debug() << "Control checksum " << m_checksum->checksum("") << endmsg;
  }

  for (const auto& containerPath: m_packedContainers) {
    const auto* dataObject = getIfExists<DataObject>(containerPath);
    if (!dataObject) {
      return Error("Container " + containerPath + " does not exist.");
    }
    auto classID = dataObject->clID();

    // Obtain the integer ID to be saved instead of the location string
    auto locationID = m_hltANNSvc->value(PackedObjectLocations, containerPath);
    // TODO fail if unknown location, even if HltANNSvc.allowUndefined=True
    if (!locationID) {
      error() << "Requested to persist " << containerPath
              << " but no ID is registered for it in the HltANNSvc, skipping!"
              << endmsg;
      return StatusCode::FAILURE;
    }

    // Obtain the function which saves the object with this CLID
    const auto it = m_savers.find(classID);
    if (it == m_savers.end()) {
      fatal() << "Unknown class ID " << classID
              << " for container " << containerPath << endmsg;
      return StatusCode::FAILURE;
    }
    const auto saveObjectFun = it->second;

    // Save the CLID and location
    m_buffer.save<uint32_t>(classID);
    m_buffer.save<int32_t>(locationID->second);

    // Save the links to other containers on the TES
    StatusCode status{StatusCode::SUCCESS};
    auto* linkMgr = dataObject->linkMgr();
    unsigned int nlinks = linkMgr->size();
    m_buffer.saveSize(nlinks);
    for (unsigned int id = 0; id < nlinks; ++id) {
      const auto& location = linkMgr->link(id)->path();

      auto packedLocation = m_containerMap.find(location);

      auto persistedLocation = location;
      if (packedLocation != std::end(m_containerMap)) {
        persistedLocation = packedLocation->second;
      }

      auto linkID = m_hltANNSvc->value(PackedObjectLocations, persistedLocation);
      if (!linkID) {
        status = Error("Requested to persist link to " + persistedLocation +
                       " but no ID is registered for it in the HltANNSvc!");
        continue;
      }

      m_buffer.save<int32_t>(linkID->second);

      if (UNLIKELY(msgLevel(MSG::DEBUG))) {
        debug() << "Packed link " << id << "/" << nlinks << " to " << location
                << " (" << persistedLocation << ") with ID " << linkID->second << endmsg;
      }
    }
    if (!status) return status;

    // Save the packed object itself
    auto objectSize = saveObjectFun(*dataObject, containerPath);

    if (UNLIKELY(msgLevel(MSG::DEBUG))) {
      debug() << "Packed " << containerPath << " with ID " << locationID->second
              << " and CLID " << classID << " into " << objectSize << " bytes" << endmsg;
      counter(containerPath) += objectSize;
    }
  }

  // Compress the buffer
  auto compressed = (m_compression != NoCompression) &&
      m_buffer.compress(m_compressionAlg, m_compressionLevel, m_compressedBuffer);
  const auto& output = compressed?m_compressedBuffer:m_buffer.buffer();

  // Write the data to the raw event
  addBanks(*rawEvent, output, compressed?static_cast<Compression>(m_compression.value()):NoCompression);

  if (UNLIKELY(msgLevel(MSG::DEBUG))) {
    counter("Size of serialized data") += m_buffer.buffer().size();
    counter("Size of comppressed data") += output.size();
    debug() << "Total size of serialized data " << m_buffer.buffer().size() << endmsg;
    debug() << "Wrote " << output.size() << " compressed bytes" << endmsg;
    if (m_enableChecksum) {
      for (const auto& x: m_checksum->checksums())
        debug() << "Packed data checksum for '" << x.first << "' = " << x.second << endmsg;
    }
  }

  m_buffer.clear();
  m_compressedBuffer.clear();
  // TODO do we / can we shrink the buffers if they take too much space for an event

  return StatusCode::SUCCESS;
}


StatusCode HltPackedDataWriter::finalize() {
  if (UNLIKELY(m_enableChecksum)) {
    for (const auto& x: m_checksum->checksums())
      info() << "Packed data checksum for '" << x.first << "' = " << x.second << endmsg;
    m_checksum.reset();
  }
  return GaudiAlgorithm::finalize();  // must be called after all other actions
}


void HltPackedDataWriter::addBanks(LHCb::RawEvent& rawEvent, const std::vector<uint8_t>& data, Compression compression) {
  /// Maximum bank payload size = 65535 (max uint16) - 8 (header) - 3 (alignment)
  static constexpr size_t MAX_PAYLOAD_SIZE{65524};

  uint16_t sourceIDCommon = (compression << CompressionBits) & CompressionMask;

  const size_t nbanks = (data.size() + MAX_PAYLOAD_SIZE - 1) / MAX_PAYLOAD_SIZE;
  if (nbanks > (PartIDMask >> PartIDBits)) {
    Error("Packed objects too long to save", StatusCode::SUCCESS, 50).ignore();
    return;
  }
  if (UNLIKELY(msgLevel(MSG::DEBUG))) {
    debug() << "Writing " << nbanks << " banks" << endmsg;
  }


  for (unsigned int ibank = 0; ibank < nbanks; ++ibank) {
    uint16_t sourceID = sourceIDCommon | ((ibank << PartIDBits) & PartIDMask);
    const int offset = ibank * MAX_PAYLOAD_SIZE;
    const size_t length = std::min(MAX_PAYLOAD_SIZE, data.size() - offset);
    if (UNLIKELY(msgLevel(MSG::DEBUG))) {
      debug() << "Adding raw bank with sourceID=" << sourceID << ", length=" << length << ", offset=" << offset << endmsg;
    }
    auto bank = rawEvent.createBank(sourceID, LHCb::RawBank::DstData,
                                    kVersionNumber, length, &(data[offset]));
    rawEvent.adoptBank(bank, true);
  }
}
