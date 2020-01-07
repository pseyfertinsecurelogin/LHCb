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
#include "VeloDet/DeVeloSensor.h"

#include "DecodeRawBankToLiteClusters.h"
#include "VeloRawBankDecoder.h"

int VeloDAQ::decodeRawBankToLiteClusters( const SiDAQ::buffer_word* bank, const DeVeloSensor& sensor,
                                          const bool assumeChipChannels, LHCb::VeloLiteCluster::FastContainer& clusters,
                                          int& byteCount, bool ignoreErrors ) {
  // construct new raw decoder, implicitely decodes header
  VeloRawBankDecoder decoder( bank );

  // only decode banks without errors
  if ( decoder.hasError() && !ignoreErrors ) { return -1; }

  // decode the clusterpositions, create lite clusters and
  // append them to the container
  unsigned int sensorNumber = sensor.sensorNumber();
  // only need to read ADCs to check for wrong bank length
  auto cpi = decoder.posAdcBegin();
  for ( ; cpi != decoder.posAdcEnd(); ++cpi ) {
    auto stripNumber = cpi->first.channelID();
    if ( assumeChipChannels ) stripNumber = sensor.ChipChannelToStrip( stripNumber );
    clusters.emplace_back( cpi->first.fracStripBits(), cpi->first.pseudoSizeBits(), cpi->first.hasHighThreshold(),
                           LHCb::VeloChannelID( sensorNumber, stripNumber ) );
  }

  // fetch number of decoded bytes, including 4 byte header, without
  // the padding bytes at the end.
  byteCount = cpi.bytesRead();
  return decoder.nClusters();
}

int VeloDAQ::decodeRawBankToLiteClusters( const SiDAQ::buffer_word* bank, const DeVeloSensor& sensor,
                                          LHCb::VeloLiteCluster::FastContainer& clusters, bool ignoreErrors ) {
  // construct new raw decoder, implicitely decodes header
  VeloRawBankDecoder decoder( bank );

  // only decode banks without errors
  if ( decoder.hasError() && !ignoreErrors ) { return -1; }

  // decode the clusterpositions, create lite clusters and
  // append them to the container
  unsigned int sensorNumber = sensor.sensorNumber();
  // decode the clusterpositions, create lite clusters and
  // append them to the container
  for ( auto cpi = decoder.posBegin(); cpi != decoder.posEnd(); ++cpi ) {
    clusters.emplace_back( ( *cpi ).fracStripBits(), ( *cpi ).pseudoSizeBits(), ( *cpi ).hasHighThreshold(),
                           LHCb::VeloChannelID( sensorNumber, ( *cpi ).channelID() ) );
  }
  return decoder.nClusters();
}
