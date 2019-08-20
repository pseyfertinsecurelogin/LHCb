/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

#include <DetDesc/ConditionKey.h>
#include <array>

namespace LHCb::Det::VP {
  // Path in the TDS to the VP detector
  static const LHCb::DetDesc::ConditionKey det_path = "/dd/Structure/LHCb/BeforeMagnetRegion/VP";
  // Path to a couple of alignement conditions (one per XML file in the db):
  // - Conditions/VP/Alignment/Global.xml
  static const LHCb::DetDesc::ConditionKey system_align = "/dd/Conditions/Alignment/VP/VPSystem";
  // - Conditions/VP/Alignment/Modules.xml
  static const LHCb::DetDesc::ConditionKey module_align = "/dd/Conditions/Alignment/VP/Module00";

  static const auto paths = std::array{det_path, system_align, module_align};
} // namespace LHCb::Det::VP
