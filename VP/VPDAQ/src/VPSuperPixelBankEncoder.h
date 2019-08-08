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
#pragma once
#include <algorithm>
#include <array>
#include <string>
#include <vector>

#include "GaudiAlg/GaudiAlgorithm.h"

#include "Event/RawEvent.h"
#include "Event/VPDigit.h"
#include "Kernel/VPConstants.h"

/** @class VPSuperPixelBankEncoder VPSuperPixelBankEncoder.h
 * Algorithm to create Super Pixel Raw Bank from VP digits.
 *
 * There is one raw bank per sensor, that is the sensor number (0-207)
 * is the source ID of the bank. Note that this means there is no
 * need to encode the sensor in the Super Pixel addresses.
 *
 * Each bank has a four byte word header, followed by a four byte
 * Super Pixel word for each Super Pixel on the sensor.
 *
 * The header word is currently simply the number of Super Pixels
 * on the sensor. There are plenty of bits available to encode
 * error conditions and such in the future.
 *
 * The Super Pixel word encoding is the following:
 *
 * bit 0-7    Super Pixel Pattern
 * bit 8-13   Super Pixel Row (0-63)
 * bit 14-22  Super Pixel Column (0-383)
 * bit 23-30  UNUSED
 * bit 31     Super Pixel isolation flag
 *
 * Super Pixel Pattern bits encode columns and rows as follows:
 *
 * row,y
 *
 *  ^  37
 *  |  26
 *  |  15
 *  |  04
 *  +---> col,x
 *
 * @author Karol Hennessy
 * @author Kurt Rinnert
 * @date   2014-03-06
 */

class VPSuperPixelBankEncoder : public GaudiAlgorithm {

public:
  /// Standard constructor
  VPSuperPixelBankEncoder( const std::string& name, ISvcLocator* pSvcLocator );
  StatusCode execute() override; ///< Algorithm execution

private:
  /// where to get the digits from
  std::string m_digitLocation;
  /// where to write the clusters to
  std::string m_rawEventLocation;
  /// event counter
  unsigned int m_evt = 0;
  /// per sensor buffers of super pixel words
  std::array<std::vector<unsigned int>, VP::NSensors> m_spBySensor;
};
