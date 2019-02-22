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

//=============================================================================
/** @file DeRichPD.cpp
 *
 *  Implementation file for class : DeRichPD
 *
 *  @author Chris Jones  christopher.rob.jones@cern.ch
 *  @date   2011-09-23
 */
//=============================================================================

// local
#include "RichDet/DeRichPD.h"

//=============================================================================

StatusCode DeRichPD::initialize() {

  // Which RICH are we in ?
  {
    const auto atestGP = geometry()->toGlobalMatrix() * Gaudi::XYZPoint{0, 0, 0};
    m_rich             = ( atestGP.z() > 6000.0 ? Rich::Rich2 : Rich::Rich1 );
    _ri_debug << "In " << rich() << endmsg;
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
