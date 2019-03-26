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
#ifndef L0HC_L0HCALG_H
#define L0HC_L0HCALG_H 1

#include "DetDesc/Condition.h"
#include "Event/HCDigit.h"
#include "L0Base/L0AlgBase.h"

/** @class L0HCAlg L0HCAlg.h
 *
 *  Algorithm responsible for producing the L0 Herschel information.
 *
 *  @author  Dan Johnson
 *  @date    2 May 2017
 */

class L0HCAlg : public L0AlgBase {
public:
  using L0AlgBase::L0AlgBase;

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

  // Properties of the algorithm
  Gaudi::Property<std::string> m_l0digitLocation{this, "L0DigitLocation", LHCb::HCDigitLocation::L0,
                                                 "TES location of Herschel L0 digits"};
  Gaudi::Property<std::string> m_digitLocation{this, "DigitLocation", LHCb::HCDigitLocation::Default,
                                               "TES location of Herschel digits"};
  Gaudi::Property<bool>        m_triggerBitsFromADCs{
      this, "TriggerBitsFromADCs", false,
      "Cause Herschel trigger bit computation using raw Herschel data and thresholds defined below"};
  Gaudi::Property<bool> m_fakeHCL0Digits{this, "FakeHCL0Digits", false,
                                         "Cause creation of fake L0HCDigits, where all counters are over threshold"};

  // Define channel mapping
  Gaudi::Property<std::vector<int>> m_channelsB0{this, "ChannelsB0", {47, 46, 45, 44}, "B0 channel mapping"};
  Gaudi::Property<std::vector<int>> m_channelsB1{this, "ChannelsB1", {23, 22, 21, 20}, "B1 channel mapping"};
  Gaudi::Property<std::vector<int>> m_channelsB2{this, "ChannelsB2", {11, 10, 9, 8}, "B2 channel mapping"};
  Gaudi::Property<std::vector<int>> m_channelsF1{this, "ChannelsF1", {23, 22, 21, 46}, "F1 channel mapping"};
  Gaudi::Property<std::vector<int>> m_channelsF2{this, "ChannelsF2", {11, 10, 9, 8}, "F2 channel mapping"};

  // Define channel mapping
  Gaudi::Property<std::vector<int>> m_thresholdsB0{this, "ThresholdsB0", {0, 0, 0, 0}, "B0 thresholds"};
  Gaudi::Property<std::vector<int>> m_thresholdsB1{this, "ThresholdsB1", {0, 0, 0, 0}, "B1 thresholds"};
  Gaudi::Property<std::vector<int>> m_thresholdsB2{this, "ThresholdsB2", {0, 0, 0, 0}, "B2 thresholds"};
  Gaudi::Property<std::vector<int>> m_thresholdsF1{this, "ThresholdsF1", {0, 0, 0, 0}, "F1 thresholds"};
  Gaudi::Property<std::vector<int>> m_thresholdsF2{this, "ThresholdsF2", {0, 0, 0, 0}, "F2 thresholds"};

private:
  /// Pointer to conditions DB
  Condition* m_cond = nullptr;

  /// Number of B-side crate
  unsigned int m_crateB;
  /// Number of F-side crate
  unsigned int m_crateF;

  /// Channel numbers for each quadrant
  std::vector<std::vector<unsigned int>> m_channels;

  /// Thresholds for each quadrant
  std::vector<std::vector<int>> m_thresholds;

  /// Return the number of counters with a signal above threshold.
  unsigned int multiplicity( const LHCb::HCDigits& digits, const bool bwd ) const;

  /// Cache database content
  StatusCode cacheMapping();
  /// Setup the mapping for a given station.
  bool mapChannels( const std::vector<int>& channels, const unsigned int station, const bool bwd );
};

#endif // L0HC_L0HCALG_H
