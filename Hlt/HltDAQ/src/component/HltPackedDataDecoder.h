#ifndef PACKEDDATADECODER_H
#define PACKEDDATADECODER_H 1

#include "GaudiAlg/GaudiAlgorithm.h"

#include "HltRawBankDecoderBase.h"
#include "PackedDataChecksum.h"

/** @class HltPackedDataDecoder HltPackedDataDecoder.h
 *  Algorithm that reads packed objects from raw banks.
 *
 *  @author Rosen Matev, Sean Benson
 *  @date   2016-01-03
 */
class HltPackedDataDecoder: public HltRawBankDecoderBase {
public:

  enum HeaderIDs { kVersionNumber = 2 };

  /// Standard constructor
  HltPackedDataDecoder(const std::string& name, ISvcLocator* pSvcLocator);

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override; ///< Algorithm execution
  StatusCode finalize() override; ///< Algorithm finalization

private:
  /// register the packed objects that can be loaded
  template<typename PackedData> void register_object();

  /// Property giving the mapping between packed containers and containers
  Gaudi::Property<std::map<std::string, std::string>> m_containerMap { this, "ContainerMap" };
  /// Property enabling calculation and print of checksums
  Gaudi::Property<bool> m_enableChecksum { this, "EnableChecksum", false};

  /// Load an object from the buffer and save it to TES.
  template<typename T> std::pair<DataObject*, size_t> loadObject(const std::string& location);

  /// Map between CLIDs and load functions
  std::map<CLID, std::function<std::pair<DataObject*, size_t>(const std::string&)> > m_loaders;
  /// Buffer for de-serialization of the packed objects
  PackedDataPersistence::PackedDataInBuffer m_buffer;
  /// Helper for computing checksums
  PackedDataPersistence::PackedDataChecksum* m_checksum{nullptr};
};

#endif // PACKEDDATADECODER_H
