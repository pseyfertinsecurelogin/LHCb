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
#include "Kernel/OTChannelID.h"

//-----------------------------------------------------------------------------
// Implementation file for class : OTChannelID
//
// 2007-07-11 : Chris Jones
//-----------------------------------------------------------------------------

std::ostream& LHCb::OTChannelID::fillStream(std::ostream& s) const
{
  return s << "{ OTChannelID : " << channelID()
           << " : TDCtime=" << tdcTime()
           << " straw=" << straw()
           << " module=" << module()
           << " quarter=" << quarter()
           << " layer=" << layer()
           << " station=" << station()
           << " uniqueStraw=" << uniqueStraw()
           << " uniqueModule=" << uniqueModule()
           << " uniqueQuarter=" << uniqueQuarter()
           << " uniqueLayer=" << uniqueLayer()
           << " }";
}
