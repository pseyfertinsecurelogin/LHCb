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
#ifndef DECODEVELORAWBUFFER_H
#define DECODEVELORAWBUFFER_H 1

#include <string>

#include "GaudiAlg/GaudiAlgorithm.h"
#include "DAQKernel/DecoderAlgBase.h"
#include "GaudiKernel/DataObjectHandle.h"
#include "VeloEvent/VeloDecodeStatus.h"

#include "SiDAQ/SiRawBufferWord.h"

namespace LHCb {
  class RawBank;
}

class DeVelo;
class IIncidentSvc;

/** @class DecodeVeloRawBuffer DecodeVeloRawBuffer.h
 *  Decode the Velo data from the 1MHZ Raw buffer.
 *  Completely re-written for the new 1Mhz raw buffer.
 *
 *  @author David Jones
 *  @author Kurt Rinnert
 *  @author Chris Parkes
 *  @date   2004-02-02
 */
class DecodeVeloRawBuffer : public Decoder::AlgBase {
public:

  /// Standard constructor
  DecodeVeloRawBuffer( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

  enum AlgStatusType{
    OK = 0,
    BadTELL1IDMapping = 1,
    UnsupportedBufferVersion = 2,
    CorruptVeloBuffer = 3,
    TooManyClusters = 4,
    HeaderErrorBit = 5,
    Other = 99
  };


private:

  /** Decode raw buffer to lite clusters
   * This decodes the raw buffer to VeloLiteClusters and
   * adds a VeloLiteCluster::FastContainer to the TES.
   *
   * @see VeloLiteCluster
   */
  LHCb::VeloLiteCluster::FastContainer decodeToVeloLiteClusters(LHCb::span<const LHCb::RawBank*> banks,
				  LHCb::VeloDecodeStatus* decStatus) const;

  /** Decode raw buffer to clusters
   * This decodes the raw buffer to VeloClusters and
   * adds themn to the TES.
   *
   * @see VeloCluster
   */
  LHCb::VeloClusters decodeToVeloClusters(LHCb::span<const LHCb::RawBank*> banks,
				  LHCb::VeloDecodeStatus *decStatus);

  /** Write VeloClusters to stdout
   *
   *  @see VeloCluster
   */
  void dumpVeloClusters(const LHCb::VeloClusters& clusters) const;

  /** Create empty banks
   *
   * Creates empty cluster and lite cluster banks on the TES.
   * This called as a failsafe option in case the raw event
   * is missing.
   * */
  void createEmptyBanks();

  /** Add DecodeVeloRawBuffer to list of failed algorithms
   *  if procAbort = true set the ProcStatus to "aborted" to show
   *  this event should be removed from physics streams
   */
  void failEvent(const std::string &ErrorText,
                 const std::string &ProcText,
                 AlgStatusType status,
                 bool procAborted) const;

  /** Replace the full clusters for a specific sensor with faked
   *  clusters from the lite container
   */
  StatusCode replaceFullFromLite(LHCb::VeloClusters& clusters,
                                 unsigned int nSensor,
                                 LHCb::span<const LHCb::RawBank*> banks,
				 LHCb::VeloDecodeStatus *decStatus);

  /// Add a fake lite cluster to the full cluster container
  void makeFakeCluster(LHCb::VeloLiteCluster const &liteCluster,
                       LHCb::VeloClusters& fakeClusters) const;

private:

  /// data handler for writing lite clusters to the transient event store
  DataObjectWriteHandle<LHCb::VeloLiteCluster::FastContainer> m_liteClusters = { this,"VeloLiteClustersLocation", LHCb::VeloLiteClusterLocation::Default };

  /// data handler for writing clusters to the transient event store
  DataObjectWriteHandle<LHCb::VeloClusters> m_clusters = { this, "VeloClusterLocation", LHCb::VeloClusterLocation::Default  };

  bool m_decodeToVeloLiteClusters;
  bool m_decodeToVeloClusters;

  bool m_dumpVeloClusters;

  unsigned int m_forcedBankVersion; ///< user forced bank version

  /// do we assume chip channels instead of strips in the raw buffer?
  /// (useful for some testbeam TELL1 cinfigurations, defaults to false)
  bool m_assumeChipChannelsInRawBuffer;

  /// helpers
  const DeVelo* m_velo = nullptr;

  /// maximum permissible number of VELO clusters,
  /// more than this will force an IncidentType::AbortEvent
  unsigned int m_maxVeloClusters;

  /// m_errorCount is the number of errors per fault to print
  /// default = 0
  /// 10 is added to it if the message level is debug
  unsigned int m_errorCount;

  /// if true, clusters will be decoded even in the presence of errors
  bool m_ignoreErrors = false;

  /// if true hide the errors from multiple cluster using the same strip
  bool m_hideWarnings;

  /// Check when decoding lite clusters that the bank length is correct
  bool m_doLengthCheck;

  /// default raw event locations: not set in options to allow comparison
  std::vector<std::string> m_defaultRawEventLocations;

};
#endif // DECODEVELORAWBUFFER_H
