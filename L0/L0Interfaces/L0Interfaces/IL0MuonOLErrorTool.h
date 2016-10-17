#ifndef COMPONENT_IL0MUONOLERRORTOOL_H 
#define COMPONENT_IL0MUONOLERRORTOOL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "Kernel/MuonTileID.h"


/** @class IL0MuonOLErrorTool IL0MuonOLErrorTool.h component/IL0MuonOLErrorTool.h
 *  Interface to access the list of optical link error in input of the L0Muon trigger
 *
 *  @author Julien Cogan
 *  @date   2010-02-25
 */
struct IL0MuonOLErrorTool :  extend_interfaces<IAlgTool> {

  // Return the interface ID
  DeclareInterfaceID( IL0MuonOLErrorTool, 2, 0 );

  // Fill the list of MuonTileIDs with the optical link in error 
  virtual StatusCode getTiles(std::vector<LHCb::MuonTileID> & ols, std::string rootInTes="")=0;

};
#endif // COMPONENT_IL0MUONOLERRORTOOL_H
