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
#include <Kernel/VPConstants.h>
#include <VPDet/DeVP.h>
#include <array>
/** @class VPGeometry VPGeometry.h VPDet/VPGeometry.h
 *
 *  Provides derived geometry information to Pixel pattern recognition
 *  algorithms and allows registration of a ConditionsDerivation to
 *  update itself
 *
 *  @author Marco Clemencic marco.clemencic@cern.ch
 *  @date 27/01/2019
 */
struct VPGeometry {
  VPGeometry() = default;
  VPGeometry( const DeVP& vp );

  inline static const LHCb::DetDesc::ConditionKey CondKey = DeVPLocation::Default + "/VPGeometry";

  //@{
  std::array<std::array<float, 12>, VP::NSensors> m_ltg{{}};
  std::array<float, VP::NSensorColumns>           m_local_x{};
  std::array<float, VP::NSensorColumns>           m_x_pitch{};
  float                                           m_pixel_size = 0;
  //@}

  static LHCb::DetDesc::IConditionDerivationMgr::DerivationId
  registerDerivation( LHCb::DetDesc::IConditionDerivationMgr& cdm, LHCb::DetDesc::ConditionKey key = CondKey,
                      bool withAlignment = true );
};
