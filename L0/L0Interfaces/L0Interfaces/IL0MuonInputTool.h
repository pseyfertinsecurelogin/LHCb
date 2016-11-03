#ifndef L0INTERFACES_IL0MUONINPUTTOOL_H 
#define L0INTERFACES_IL0MUONINPUTTOOL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "Kernel/MuonTileID.h"


/** @class IL0MuonInputTool IL0MuonInputTool.h L0Interfaces/IL0MuonInputTool.h
 *  Interface to retrieve the muon tiles in input of the L0Muon trigger
 *
 *  @author Julien Cogan
 *  @date   2010-02-25
 */
struct IL0MuonInputTool : extend_interfaces<IAlgTool> {

  // Return the interface ID
  DeclareInterfaceID( IL0MuonInputTool, 2, 0 );
  // Fill the list of logical channels seen in input of the L0Muon trigger
  virtual StatusCode getTiles(std::vector<LHCb::MuonTileID> & tiles, std::string rootInTes="")=0;
  // Fill the list of logical pads seen in input of the L0Muon trigger
  virtual StatusCode getPads(std::vector<LHCb::MuonTileID> & pads, std::string rootInTes="")=0;
  
};
#endif // L0INTERFACES_IL0MUONINPUTTOOL_H
