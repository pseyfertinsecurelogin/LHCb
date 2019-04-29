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
#include <vector>

// It's bad practice to use anonymous namespace in headers, but I just want to
// share a few constants.
namespace {
  // Path in the TDS to the VP detector
  static const LHCb::DetDesc::ConditionKey VP_det_path = "/dd/Structure/LHCb/BeforeMagnetRegion/VP";
  // Path to a couple of alignement conditions (one per XML file in the db):
  // - Conditions/VP/Alignment/Global.xml
  static const LHCb::DetDesc::ConditionKey VP_system_align = "/dd/Conditions/Alignment/VP/VPSystem";
  // - Conditions/VP/Alignment/Modules.xml
  static const LHCb::DetDesc::ConditionKey VP_module_align = "/dd/Conditions/Alignment/VP/Module00";

  static const std::vector<LHCb::DetDesc::ConditionKey> VP_paths{VP_det_path, VP_system_align, VP_module_align};
} // namespace
