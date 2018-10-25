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
#ifndef PREPAREVELORAWBUFFER_H
#define PREPAREVELORAWBUFFER_H 1

#include <vector>
#include <algorithm>
#include <string>

#include "VeloEvent/InternalVeloCluster.h"

#include "GaudiAlg/GaudiAlgorithm.h"

class DeVelo;
namespace LHCb {
  class RawEvent;
}

/** @class PrepareVeloRawBuffer PrepareVeloRawBuffer.h
 *  Prepare the cluster container from 1MHz raw buffer.
 *  Completely rewritten for the new 1MHz raw buffer.
 *
 *  @author David Jones
 *  @author Kurt Rinnert
 *  @author Olivier Callot
 *  @date   2003-04-11
 *  2004-04-13 : modified and moved to Velo area Chris Parkes
 *  2004-11-04 : modified to use new GaudiAlgorithm by David Hutchcroft
 *  2006-02-15 : completely rewritten for 1MHz raw buffer, Kurt Rinnert, David Jones
 */
class PrepareVeloRawBuffer : public GaudiAlgorithm {

public:
  /// Standard constructor
  PrepareVeloRawBuffer( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~PrepareVeloRawBuffer( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:

  void dumpInputClusters() const;

  unsigned int
makeBank (std::vector<const LHCb::InternalVeloCluster*>::const_iterator begin,
	  std::vector<const LHCb::InternalVeloCluster*>::const_iterator end);
	

void
storeBank(int sensor,
	  std::vector<const LHCb::InternalVeloCluster*>::const_iterator begin,
	  std::vector<const LHCb::InternalVeloCluster*>::const_iterator end,
	  LHCb::RawEvent* rawEvent);
private:

  // configurable locations in the TES
  std::string m_clusterLoc;
  std::string m_rawEventLoc;

  // configurable cluster dump flag
  bool m_dumpInputClusters;

  /// version tag
  unsigned int m_bankVersion;

  // long lived containers for performance reasons. Also used to communicate
  // with makeBank() method
  std::vector<const LHCb::InternalVeloCluster*> m_sortedClusters;
  std::vector<SiDAQ::buffer_word> m_rawData;
  std::vector<SiDAQ::buffer_word> m_clusterADCBuffer;
  std::vector<SiDAQ::buffer_word> m_clusterPosBuffer;

  // size of raw bank in bytes, inclding the 4 byte header but
  // *without* the padding bytes at the end
  unsigned int m_bankSizeInBytes = 0;

  /// pointer to Velo Detector Element
  DeVelo* m_velo = nullptr;

  /// list of sensor numbers to check for empty sensors
  std::vector<int> m_sensorNumbers;

};
#endif // PREPAREVELORAWBUFFER_H
