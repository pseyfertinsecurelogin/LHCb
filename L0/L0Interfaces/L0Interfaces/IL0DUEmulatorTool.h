#ifndef IL0DUEMULATORTOOL_H 
#define IL0DUEMULATORTOOL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "Event/L0ProcessorData.h"
#include "Event/L0DUConfig.h"
#include "Event/L0DUReport.h"

/** @class IL0DUEmulatorTool IL0DUEmulatorTool.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2007-10-10
 */
struct IL0DUEmulatorTool : extend_interfaces<IAlgTool> {

  // Declare the interface ID
  DeclareInterfaceID( IL0DUEmulatorTool, 2, 0 );

  virtual StatusCode process(LHCb::L0DUConfig* config,  LHCb::L0ProcessorDatas* datas)=0;
  virtual StatusCode process(LHCb::L0DUConfig* config,  std::vector<std::string> dataLocs)=0;
  virtual const LHCb::L0DUReport emulatedReport()=0;
  virtual const std::vector<unsigned int> bank(unsigned int version)=0;  

};
#endif // IL0DUEMULATORTOOL_H
