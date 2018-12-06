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
#ifndef HLTPACKEDDATAWRITER_H
#define HLTPACKEDDATAWRITER_H 1

#include "GaudiAlg/GaudiAlgorithm.h"
#include "Kernel/IANNSvc.h"
#include "PackedDataBuffer.h"
#include "PackedDataChecksum.h"
#include "Event/RawEvent.h"

#include "GaudiAlg/GaudiAlgorithm.h"

#include <optional>

/** @class HltPackedDataWriter HltPackedDataWriter.h
 *  Algorithm that writes packed objects to raw banks.
 *
 *  @author Rosen Matev, Sean Benson
 *  @date   2016-01-03
 */
class HltPackedDataWriter: public GaudiAlgorithm {
public:

  enum HeaderIDs{kVersionNumber = 2};

  /// Bit postions in the SourceID word
  enum SourceIDBits {
    PartIDBits = 0,
    ReservedBits = 8,
    CompressionBits = 13
  };
  /// Bit masks in the SourceID word
  enum SourceIDMasks {
    PartIDMask = 0x00FF,
    ReservedMask = 0x1F00,
    CompressionMask = 0xE000
  };
  /// Compression algorithms
  enum Compression {
    NoCompression = 0,
    ZLIB = 1,
    LZMA = 2,
    MaxCompression = 7
  };

  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override; ///< Algorithm execution
  StatusCode finalize() override; ///< Algorithm finalization

  /// Return the part ID from the SourceID word
  static unsigned int partID(uint16_t sourceID) {
    return (sourceID & PartIDMask) >> PartIDBits;
  }
  /// Return the compression from the SourceID word
  static Compression compression(uint16_t sourceID) {
    return static_cast<Compression>((sourceID & CompressionMask) >> CompressionBits);
  }


private:
  /// Put the (compressed) data buffer into raw banks and register them.
  void addBanks(LHCb::RawEvent& rawEvent, const std::vector<uint8_t>& data, Compression compression);
  /// Save an object to the buffer.
  template<typename T> size_t saveObject(const DataObject& dataObject, const std::string& location);
  /// register the packed objects that can be saved
  template<typename PackedData> void register_object();

  /// Property giving the locations of packed containers to be persisted
  Gaudi::Property<std::vector<std::string>> m_packedContainers{ this,"PackedContainers"};
  /// Property giving the mapping between containers and packed containers
  Gaudi::Property<std::map<std::string, std::string>> m_containerMap{ this,"ContainerMap"};
  /// Property giving the location of the raw event
  Gaudi::Property<std::string> m_outputRawEventLocation { this, "OutputRawEventLocation", LHCb::RawEventLocation::Default};
  /// ROOT compression algorithm
  ROOT::RCompressionSetting::EAlgorithm::EValues m_compressionAlg;
  /// Property setting the compression algorithm
  Gaudi::Property<int> m_compression { this, "Compression", LZMA,
    [=](Property&) {
      switch (this->m_compression) {
        case NoCompression: { this->m_compressionAlg = ROOT::RCompressionSetting::EAlgorithm::kUndefined; break; }
        case ZLIB: { this->m_compressionAlg = ROOT::RCompressionSetting::EAlgorithm::kZLIB; break; }
        case LZMA: { this->m_compressionAlg = ROOT::RCompressionSetting::EAlgorithm::kLZMA; break; }
        default: throw GaudiException( "Unrecognized compression algorithm.", this->name(), StatusCode::FAILURE );
        }
    }, Gaudi::Details::Property::ImmediatelyInvokeHandler{true} };
  /// Property setting the compression level
  Gaudi::Property<int> m_compressionLevel{ this, "CompressionLevel", 6};
  /// Property enabling calculation and print of checksums
  Gaudi::Property<bool> m_enableChecksum{ this,"EnableChecksum", false};

  /// HltANNSvc for making selection names to int selection ID
  SmartIF<IANNSvc> m_hltANNSvc;

  /// Map between CLIDs and save functions
  std::map<CLID, std::function<size_t(const DataObject&, const std::string&)> > m_savers;
  /// Buffer for serialization of the packed objects
  PackedDataPersistence::PackedDataOutBuffer m_buffer;
  /// Buffer for the compressed data
  std::vector<uint8_t> m_compressedBuffer;
  /// Helper for computing checksums
  std::optional<PackedDataPersistence::PackedDataChecksum> m_checksum;

};

#endif // HLTPACKEDDATAWRITER_H
