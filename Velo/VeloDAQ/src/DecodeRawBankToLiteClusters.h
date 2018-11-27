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
#ifndef VELODAQ_DECODERAWBANKTOLITECLUSTERS_H 
#define VELODAQ_DECODERAWBANKTOLITECLUSTERS_H 1

#include "Event/VeloLiteCluster.h"
#include "SiDAQ/SiRawBufferWord.h"

class DeVeloSensor;

namespace VeloDAQ {
  /**  Append lite clusters to fast container  
   *   This function decodes the cluster position part of the
   *   raw bank and appends the resulting VeloLiteClusters to
   *   to the fast container provided by the client.
   *
   *   @arg bank pointer to raw bank
   *   @arg sensor sensor corresponding to bank, needed to create channel id and for channel to strip mapping
   *   @arg assumeChipChannels if true, assume chip channels instead of strip numbers in raw bank
   *   @arg clusters fast container to which decoded clusters will be appended
   *   @return number of decoded clusters appended (-1 = Header error)
   *   @see VeloLiteCluster
   *   @author Kurt Rinnert
   *   @date   2006-02-17
   */
  int decodeRawBankToLiteClusters(
      const SiDAQ::buffer_word* bank, 
      const DeVeloSensor& sensor,
      const bool assumeChipChannels,
      LHCb::VeloLiteCluster::FastContainer& clusters,
      int & bytesCount,
      bool ignoreErrors); 

  /**  Append lite clusters to fast container  
   *   This function decodes the cluster position part of the
   *   raw bank and appends the resulting VeloLiteClusters to
   *   to the fast container provided by the client. Fast version
   *   that skips all test on ADC part of bank to check length is correct.
   *
   *   @arg bank pointer to raw bank
   *   @arg sensor sensor corresponding to bank, needed to create channel id and for channel to strip mapping
   *   @arg clusters fast container to which decoded clusters will be appended
   *   @return number of decoded clusters appended (-1 = Header error)
   *   @see VeloLiteCluster
   *   @author Kurt Rinnert
   *   @date   2006-02-17
   */
  int decodeRawBankToLiteClusters(
      const SiDAQ::buffer_word* bank, 
      const DeVeloSensor& sensor,
      LHCb::VeloLiteCluster::FastContainer& clusters,
      bool ignoreErrors); 
}
#endif // VELODAQ_DECODERAWBANKTOLITECLUSTERS_H
