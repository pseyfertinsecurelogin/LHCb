#ifndef _IFTReadoutTool_H
#define _IFTReadoutTool_H 1

#include "GaudiKernel/IAlgTool.h"

#include <string>
#include <vector>
#include "Event/FTCluster.h"

#include "Kernel/FTChannelID.h"

//namespace LHCb{
//  class FTChannelID;
//}

/** @class IFTReadoutTool IFTReadoutTool.h  FTDAQ/IFTReadoutTool
*
*  Interface Class for things related to the Readout - Tell1 Board
*
*  @author L.Henry
*  @date   07/11/2017
*/

// Declaration of the interface ID ( interface id, major version, minor version)

struct IFTReadoutTool: extend_interfaces< IAlgTool >{

  /// Static access to interface id
  DeclareInterfaceID(IFTReadoutTool, 1 , 0);
  
  /// number of boards
  virtual unsigned int nBoard() const = 0;
  
  /// get station,layer,quarter
  virtual LHCb::FTChannelID station      (const unsigned int aChan) const = 0;
  virtual LHCb::FTChannelID layer        (const unsigned int aChan) const = 0;
  virtual LHCb::FTChannelID quarter      (const unsigned int aChan) const = 0;
  virtual LHCb::FTChannelID uniqueQuarter(const unsigned int aChan) const = 0;
  virtual LHCb::FTChannelID module       (const unsigned int aChan) const = 0;
  virtual LHCb::FTChannelID mat          (const unsigned int aChan) const = 0;
  virtual LHCb::FTChannelID sipm         (const unsigned int aChan) const = 0;

  /// printout
  virtual void printMapping() const = 0;
  
  /// write an xml file
  virtual StatusCode writeMappingToXML() const = 0;
  
};


#endif // _IFTReadoutTool_H
