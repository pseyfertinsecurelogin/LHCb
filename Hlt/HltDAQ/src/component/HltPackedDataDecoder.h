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
#ifndef PACKEDDATADECODER_H
#define PACKEDDATADECODER_H 1

//// TODO: split this up into RawEvent -> [ loc, Out_i ]
//         and then use SuperAlgorithm to have a vector over i....

#include "PackedDataBuffer.h"

#include "HltRawBankDecoderBase.h"
#include "PackedDataChecksum.h"

#include "GaudiAlg/GaudiAlgorithm.h"

/** @class HltPackedDataDecoder HltPackedDataDecoder.h
 *  Algorithm that reads packed objects from raw banks.
 *
 *  @author Rosen Matev, Sean Benson
 *  @date   2016-01-03
 */
class HltPackedDataDecoder : public HltRawBankDecoderBase {
public:
  enum HeaderIDs { kVersionNumber = 2 };

  /// Standard constructor
  HltPackedDataDecoder( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

private:
  /// register the packed objects that can be loaded
  template <typename PackedData>
  void register_object();

  /// Property giving the mapping between packed containers and containers
  Gaudi::Property<std::map<std::string, std::string>> m_containerMap{this, "ContainerMap"};
  /// Property enabling calculation and print of checksums
  Gaudi::Property<bool> m_enableChecksum{this, "EnableChecksum", false};

  /// Load an object from the buffer and save it to TES.
  template <typename T>
  std::pair<DataObject*, size_t> loadObject( const std::string& location );

  /// Map between CLIDs and load functions
  std::map<CLID, std::function<std::pair<DataObject*, size_t>( const std::string& )>> m_loaders;
  /// Buffer for de-serialization of the packed objects
  PackedDataPersistence::PackedDataInBuffer m_buffer;
  /// Helper for computing checksums
  std::unique_ptr<PackedDataPersistence::PackedDataChecksum> m_checksum;
};

#endif // PACKEDDATADECODER_H
