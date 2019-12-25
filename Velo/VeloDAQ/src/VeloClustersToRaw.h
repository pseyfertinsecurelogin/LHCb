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
#ifndef VELOCLUSTERSTORAW_H
#define VELOCLUSTERSTORAW_H 1

#include <algorithm>
#include <string>
#include <vector>

#include "Event/VeloCluster.h"
#include "SiDAQ/SiRawBufferWord.h"
#include "VeloRawBankVersions.h"

#include "GaudiAlg/GaudiAlgorithm.h"

class DeVelo;
namespace LHCb {
  class RawEvent;
}

/** @class VeloClustersToRaw VeloClustersToRaw.h
 *  Create a raw VELO cluster bank from VeloCluster container.
 *
 *  @author Kurt Rinnert
 *  @date   2011-09-20
 */
class VeloClustersToRaw : public GaudiAlgorithm {

public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

private:
  unsigned int makeBank( const unsigned int sensor, std::vector<const LHCb::VeloCluster*>::const_iterator& begin );

  StatusCode storeBank( const unsigned int sensor, std::vector<const LHCb::VeloCluster*>::const_iterator& begin );

  bool selfTest(); ///< run self test on default locations

private:
  // configurable locations in the TES
  DataObjectReadHandle<LHCb::VeloClusters> m_clusterLoc{
      this, "VeloClusterLocation", "Raw/Velo/ClustersSelected"}; ///< cluster container input location
  DataObjectHandle<LHCb::RawEvent> m_rawEventLoc{this, Gaudi::DataHandle::Updater, "RawEventLocation",
                                                 "DAQ/RawEventSelected"}; ///< raw bank output location

  Gaudi::Property<unsigned int> m_bankVersion{this, "BankVersion", VeloDAQ::v3}; ///< raw bank version tag
  Gaudi::Property<bool>         m_runSelfTest{this, "RunSelfTest", false,
                                      "Decodes endoced bank and compares to input clusters."}; ///< run self test on
                                                                                               ///< default locations

  // long lived containers for performance reasons. Also used to communicate
  // with makeBank() method
  std::vector<const LHCb::VeloCluster*> m_sortedClusters;
  std::vector<SiDAQ::buffer_word>       m_rawData;
  std::vector<SiDAQ::buffer_word>       m_clusterADCBuffer;
  std::vector<SiDAQ::buffer_word>       m_clusterPosBuffer;

  LHCb::RawEvent* m_rawEventOut = nullptr;

  // size of raw bank in bytes, including the 4 byte header but
  // *without* the padding bytes at the end
  unsigned int m_bankSizeInBytes = 0;

  /// pointer to Velo Detector Element
  DeVelo* m_velo = nullptr;
};
#endif // VELOCLUSTERSTORAW_H
