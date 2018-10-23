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
// local
#include "Kernel/VPChannelID.h"

//-----------------------------------------------------------------------------
// Implementation file for class : VPChannelID
//
// 2009-05-11 : Victor Coco
//-----------------------------------------------------------------------------

std::ostream& LHCb::VPChannelID::fillStream(std::ostream& s) const {
  s << "{ " << " VPChannelID : " << channelID()
    << " : row = " << row() << " col = " << col()
    << " chip = " << chip() << " sensor = " << sensor();
  return s << " }";
}
