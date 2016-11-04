#ifndef CALODAQ_ICALOL0DATAPROVIDER_H 
#define CALODAQ_ICALOL0DATAPROVIDER_H 1

// Include files
// from STL
#include <string>

#include "CaloDAQ/ICaloReadoutTool.h"
#include "Event/L0CaloAdc.h"
#include "CaloKernel/CaloVector.h"

/** @class ICaloL0DataProvider ICaloL0DataProvider.h CaloDAQ/ICaloL0DataProvider.h
 *  Interface to the tool for fast decoding of CaloBanks
 *
 *  @author Olivier Deschamps
 *  @date   2007-12-03
 */
struct ICaloL0DataProvider : extend_interfaces<ICaloReadoutTool> {

  DeclareInterfaceID( ICaloL0DataProvider, 4, 0 );

  virtual int l0Adc(LHCb::CaloCellID id,int def=0)=0;
  virtual unsigned int nTell1s()=0;
  virtual const CaloVector<LHCb::L0CaloAdc>& l0Adcs(int source=-1,bool clean=true)=0;
  virtual const CaloVector<LHCb::L0CaloAdc>& l0Adcs(std::vector<int> sources,bool clean=true)=0;

};
#endif // CALODAQ_ICALOL0DATAPROVIDER_H
