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
// local
#include "Kernel/STChannelID.h"
#include <sstream>

//-----------------------------------------------------------------------------
// Implementation file for class : STChannelID
//
// 2007-08-11 : M Needham
//-----------------------------------------------------------------------------

std::string LHCb::STChannelID::toString() const {
  std::ostringstream o;
  fillStream( o );
  return o.str();
}

std::ostream& LHCb::STChannelID::fillStream( std::ostream& s ) const {
  s << "{ ";
  if ( isTT() ) s << "TT ";
  if ( isIT() ) s << "IT ";
  s << "STChannelID : " << channelID() << " : type=" << type() << " strip=" << strip() << " sector=" << sector()
    << " detRegion=" << detRegion() << " layer=" << layer() << " station=" << station();

  return s << " }";
}
