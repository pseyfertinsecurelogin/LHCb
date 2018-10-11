#ifndef CALOFUTUREDAQ_ICALOFUTUREENERGYFROMRAW_H 
#define CALOFUTUREDAQ_ICALOFUTUREENERGYFROMRAW_H 1

// Include files
// from STL
#include <string>

#include "ICaloFutureReadoutTool.h"
#include "Event/CaloAdc.h"
#include "Event/CaloDigit.h"


/** @class ICaloFutureEnergyFromRaw ICaloFutureEnergyFromRaw.h CaloFutureDAQ/ICaloFutureEnergyFromRaw.h
 *  Interface to the tool decoding the (cell+energy) from Raw
 *
 *  @author Olivier Callot
 *  @date   2005-01-10
 */
struct ICaloFutureEnergyFromRaw : extend_interfaces<ICaloFutureReadoutTool> {

  // Return the interface ID
  DeclareInterfaceID( ICaloFutureEnergyFromRaw, 5, 0 );

  virtual const std::vector<LHCb::CaloAdc>&   adcs( ) = 0;
  virtual const std::vector<LHCb::CaloAdc>&   adcs( int source ) = 0;
  virtual const std::vector<LHCb::CaloAdc>&   adcs( const LHCb::RawBank& bank ) = 0;
  virtual const std::vector<LHCb::CaloAdc>&   pinAdcs( ) = 0;
  
  virtual const std::vector<LHCb::CaloDigit>& digits( ) = 0;
  virtual const std::vector<LHCb::CaloDigit>& digits( int source  ) = 0;
  virtual const std::vector<LHCb::CaloDigit>& digits( const LHCb::RawBank& bank ) = 0;
  
};
#endif // CALOFUTUREDAQ_ICALOFUTUREENERGYFROMRAW_H
