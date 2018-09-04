// local
#include "Kernel/UTChannelID.h"
#include <sstream>

//-----------------------------------------------------------------------------
// Implementation file for class : UTChannelID
//
// 2007-08-11 : M Needham
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
