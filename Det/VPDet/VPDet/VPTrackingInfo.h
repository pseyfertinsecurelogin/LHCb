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

#include <DetDesc/IConditionDerivationMgr.h>

#include <VPDet/DeVP.h>

struct VPTrackingInfo {
  VPTrackingInfo( const DeVP& vp, float minZ_forward, float maxZ_backward );

  inline static const LHCb::DetDesc::ConditionKey CondKey = DeVPLocation::Default + "/VPTrackingInfo";

  const DeVP& m_vp;

  /// z positions of the modules
  std::array<float, VP::NModules> m_moduleZPositions{};
  /// first and last module
  unsigned int m_firstModule;
  unsigned int m_lastModule;

  // Used in the searchByPair<Forward>
  std::vector<unsigned int> m_modulesDefaultLoop;
  std::vector<unsigned int> m_modulesForwardLoop;
  std::vector<unsigned int> m_modulesBackwardLoop;

  std::vector<float> m_errorX;
  std::vector<float> m_errorY;
  std::vector<float> m_errorXLong;
  std::vector<float> m_errorYLong;

  static LHCb::DetDesc::IConditionDerivationMgr::DerivationId
  registerDerivation( LHCb::DetDesc::IConditionDerivationMgr& cdm, LHCb::DetDesc::ConditionKey key, bool withAlignment,
                      float minZ_forward, float maxZ_backward );
};
