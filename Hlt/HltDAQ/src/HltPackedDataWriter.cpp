#include "RZip.h"

#include "Event/RawEvent.h"
#include "Event/PackedTrack.h"
#include "Event/PackedCaloCluster.h"
#include "Event/PackedRichPID.h"
#include "Event/PackedMuonPID.h"
#include "Event/PackedCaloHypo.h"
#include "Event/PackedProtoParticle.h"

#include "PackedDataChecksum.h"
#include "PackedDataBuffer.h"
#include "HltPackedDataWriter.h"

static const Gaudi::StringKey PackedObjectLocations{"PackedObjectLocations"};


// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY(HltPackedDataWriter)


HltPackedDataWriter::HltPackedDataWriter(const std::string& name, ISvcLocator* pSvcLocator)
  : GaudiAlgorithm(name, pSvcLocator)
{
  declareProperty("Containers", m_containers);
  declareProperty("OutputRawEventLocation",
                  m_outputRawEventLocation=LHCb::RawEventLocation::Default);
  declareProperty("Compression", m_compression = LZMA);
  declareProperty("CompressionLevel", m_compressionLevel = 6);

  using namespace std::placeholders; 
  m_savers[LHCb::CLID_PackedTracks] =
    std::bind(&HltPackedDataWriter::saveObject<LHCb::PackedTracks>, this, _1);
  m_savers[LHCb::CLID_PackedRichPIDs] = 
    std::bind(&HltPackedDataWriter::saveObject<LHCb::PackedRichPIDs>, this, _1);
  m_savers[LHCb::CLID_PackedMuonPIDs] = 
    std::bind(&HltPackedDataWriter::saveObject<LHCb::PackedMuonPIDs>, this, _1);
  m_savers[LHCb::CLID_PackedCaloHypos] = 
    std::bind(&HltPackedDataWriter::saveObject<LHCb::PackedCaloHypos>, this, _1);
  m_savers[LHCb::CLID_PackedProtoParticles] = 
    std::bind(&HltPackedDataWriter::saveObject<LHCb::PackedProtoParticles>, this, _1);
  m_savers[LHCb::CLID_PackedCaloClusters] = 
    std::bind(&HltPackedDataWriter::saveObject<LHCb::PackedCaloClusters>, this, _1);
}


StatusCode HltPackedDataWriter::initialize() {
  const StatusCode sc = GaudiAlgorithm::initialize();
  if (sc.isFailure()) return sc;

  info() << "Configured to persist containers ";
  for (const auto& path: m_containers) {
    info() << " '" << path << "',";
  }
  info() << endmsg;

  m_hltANNSvc = svc<IANNSvc>("HltANNSvc");

  switch (m_compression) {
    case NoCompression: { m_compressionAlg = ROOT::kUndefinedCompressionAlgorithm; break; }
    case ZLIB: { m_compressionAlg = ROOT::kZLIB; break; }
    case LZMA: { m_compressionAlg = ROOT::kLZMA; break; }
    default: {
      return Error("Unrecognized compression algorithm.");
    }
  }

  if (msgLevel(MSG::DEBUG)) {
    m_checksum = new PackedDataPersistence::PackedDataChecksum();
  }

  return sc;
}


template<typename T>
size_t HltPackedDataWriter::saveObject(const DataObject& dataObject) {
  const auto& object = dynamic_cast<const T&>(dataObject);
  
  // Reserve bytes for the size of the object
  auto posObjectSize = m_buffer.save<uint32_t>(0).first;
  // Save the object actual object and see how many bytes were written
  auto objectSize = m_buffer.save(object).second;
  // Save the object's size in the correct position
  m_buffer.saveAt<uint32_t>(objectSize, posObjectSize);

  if (msgLevel(MSG::DEBUG)) { m_checksum->processObject(object); }
  return objectSize;
}


StatusCode HltPackedDataWriter::execute() {
  if (msgLevel(MSG::DEBUG)) debug() << "==> Execute" << endmsg;

  // Get the raw event
  auto* rawEvent = getIfExists<LHCb::RawEvent>(m_outputRawEventLocation);
  if (!rawEvent) {
    return Error("No RawEvent at " + m_outputRawEventLocation.value(), StatusCode::SUCCESS, 20);
  }

  m_buffer.clear();

  for (const auto& containerPath: m_containers) {
    const auto* dataObject = getIfExists<DataObject>(containerPath);
    if (!dataObject) {
      return Error("Container " + containerPath + " does not exist.");
    }
    auto classID = dataObject->clID();

    // Obtain the integer ID to be saved instead of the location string
    auto locationID = m_hltANNSvc->value(PackedObjectLocations, containerPath);
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

    // Save the CLID, location and object itself
    m_buffer.save<uint32_t>(classID);
    m_buffer.save<int32_t>(locationID->second);
    auto objectSize = saveObjectFun(*dataObject);

    if (msgLevel(MSG::DEBUG)) {
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
  addBanks(*rawEvent, output, compressed?static_cast<Compression>(m_compression):NoCompression);

  if (msgLevel(MSG::DEBUG)) {
    counter("Size of serialized data") += m_buffer.buffer().size();
    counter("Size of comppressed data") += output.size();
    debug() << "Total size of serialized data " << m_buffer.buffer().size() << endmsg;
    debug() << "Wrote " << output.size() << " compressed bytes" << endmsg;    
    debug() << "Packed data checksum = " << m_checksum->checksum() << endmsg;
  }

  m_buffer.clear();
  m_compressedBuffer.clear();
  // TODO do we / can we shrink the buffers if they take too much space for an event

  return StatusCode::SUCCESS;
}


StatusCode HltPackedDataWriter::finalize() {
  if (msgLevel(MSG::DEBUG)) {
    debug() << "Global packed data checksum = " << m_checksum->checksum() << endmsg;
    delete m_checksum;
  }
  return GaudiAlgorithm::finalize();  // must be called after all other actions
}


void HltPackedDataWriter::addBanks(LHCb::RawEvent& rawEvent, const std::vector<uint8_t>& data, Compression compression) {
  /// Maximum bank payload size = 65535 (max uint16) - 8 (header) - 3 (alignment)
  static constexpr size_t MAX_PAYLOAD_SIZE{65524};

  uint16_t sourceIDCommon = (compression << CompressionBits) & CompressionMask;

  const size_t nbanks = data.size() / MAX_PAYLOAD_SIZE + 1;
  if (nbanks > (PartIDMask >> PartIDBits)) {
    Error("Packed objects too long to save", StatusCode::SUCCESS, 50);
    return;
  }
  debug() << "Writing " << nbanks << " banks" << endmsg;
  

  for (unsigned int ibank = 0; ibank < nbanks; ++ibank) {
    uint16_t sourceID = sourceIDCommon | ((ibank << PartIDBits) & PartIDMask);
    const int offset = ibank * MAX_PAYLOAD_SIZE;
    const size_t length = std::min(MAX_PAYLOAD_SIZE, data.size() - offset);
    debug() << "Adding raw bank with sourceID=" << sourceID << ", length=" << length << " from " << (void*)&(data[offset]) << endmsg;
    auto bank = rawEvent.createBank(sourceID, LHCb::RawBank::DstData,
                                    kVersionNumber, length, &(data[offset]));
    rawEvent.adoptBank(bank, false);  // TODO Everyone using true, but it seems that we want false
  }
}
