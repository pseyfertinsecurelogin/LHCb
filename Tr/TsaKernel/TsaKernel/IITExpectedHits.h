#ifndef _IITExpectedHits_H
#define _IITExpectedHits_H

#include "GaudiKernel/IAlgTool.h"

static const InterfaceID IID_IITExpectedHits( "IITExpectedHits", 0, 0 );

/** @class IITExpectedHits 
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
 class STChannelID;
}

#include <utility>
#include <vector>

class IITExpectedHits: virtual public IAlgTool {

public:

  typedef std::pair<LHCb::STChannelID,double> ITPair;

  /// Retrieve interface ID
  static const InterfaceID& interfaceID() { return IID_IITExpectedHits ; }

  virtual StatusCode   collect(const Tsa::Parabola& parab,
                               const Tsa::Line& line,
                               const LHCb::STChannelID& aChan,
                               std::vector<IITExpectedHits::ITPair>& hits,
                               const unsigned int iSector) const = 0;

}; 


#endif // _IITExpectedHits_H

