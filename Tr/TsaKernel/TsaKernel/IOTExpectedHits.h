#ifndef _IOTExpectedHits_H
#define _IOTExpectedHits_H

#include "GaudiKernel/IAlgTool.h"

static const InterfaceID IID_IOTExpectedHits( "IOTExpectedHits", 0, 0 );

/** @class IOTExpectedHits 
 *
 *  How many hits do we expect in a gven detector
 *
 *  @author M.Needham
 *  @date   31/05/2004
 */

namespace Tsa{
  class Parabola;
  class Line;
}

namespace LHCb{
 class OTChannelID;
}

#include <utility>
#include <vector>

class IOTExpectedHits: virtual public IAlgTool {

public:

  typedef std::pair<LHCb::OTChannelID,double> OTPair;

  /// Retrieve interface ID
  static const InterfaceID& interfaceID() { return IID_IOTExpectedHits ; }

  virtual StatusCode   collect(const Tsa::Parabola& parab,
                               const Tsa::Line& line,
                               const LHCb::OTChannelID& aChan,
                               std::vector<IOTExpectedHits::OTPair>& hits,
                               const unsigned int iSector) const = 0;

}; 


#endif // _IOTExpectedHits_H

