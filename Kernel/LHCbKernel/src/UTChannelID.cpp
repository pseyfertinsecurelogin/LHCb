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
#include "Kernel/UTChannelID.h"
#include <sstream>

//-----------------------------------------------------------------------------
// Implementation file for class : UTChannelID
//
// 2018-09-04 : A Beiter (based on code by M Needham)
//-----------------------------------------------------------------------------

std::string  LHCb::UTChannelID::toString() const{
  std::ostringstream o;
  fillStream(o);
  return o.str();
}

std::ostream& LHCb::UTChannelID::fillStream(std::ostream& s) const
{
  s << "{ ";
  if ( isUT() ) s << "UT ";
  s << "UTChannelID : " << channelID()
    << " : type=" << type()
    << " strip=" << strip()
    << " sector=" << sector()
    << " detRegion=" << detRegion()
    << " layer=" << layer()
    << " station=" << station();

  return s << " }";
}
