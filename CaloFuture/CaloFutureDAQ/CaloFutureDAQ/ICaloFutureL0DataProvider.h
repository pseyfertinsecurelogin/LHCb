#ifndef CALOFUTUREDAQ_ICALOFUTUREL0DATAPROVIDER_H 
#define CALOFUTUREDAQ_ICALOFUTUREL0DATAPROVIDER_H 1

// Include files
// from STL
#include <string>

#include "CaloFutureDAQ/ICaloFutureReadoutTool.h"
#include "Event/L0CaloAdc.h"
#include "CaloKernel/CaloVector.h"

/** @class ICaloFutureL0DataProvider ICaloFutureL0DataProvider.h CaloFutureDAQ/ICaloFutureL0DataProvider.h
 *  Interface to the tool for fast decoding of CaloFutureBanks
 *
 *  @author Olivier Deschamps
 *  @date   2007-12-03
 */
struct ICaloFutureL0DataProvider : extend_interfaces<ICaloFutureReadoutTool> {

  DeclareInterfaceID( ICaloFutureL0DataProvider, 4, 0 );

  virtual int l0Adc(LHCb::CaloCellID id,int def=0)=0;
  virtual unsigned int nTell1s() const =0;
  virtual const CaloVector<LHCb::L0CaloAdc>& l0Adcs(int source=-1,bool clean=true)=0;
  virtual const CaloVector<LHCb::L0CaloAdc>& l0Adcs(std::vector<int> sources,bool clean=true)=0;

};
#endif // CALOFUTUREDAQ_ICALOFUTUREL0DATAPROVIDER_H
