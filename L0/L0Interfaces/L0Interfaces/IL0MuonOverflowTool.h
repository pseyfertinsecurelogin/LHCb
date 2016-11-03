#ifndef L0INTERFACES_IL0MUONOVERFLOWTOOL_H 
#define L0INTERFACES_IL0MUONOVERFLOWTOOL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "Kernel/MuonTileID.h"


/** @class IL0MuonOverflowTool IL0MuonOverflowTool.h L0Interfaces/IL0MuonOverflowTool.h
 *  Interface to access the list of FPGA declaring an overflow
 *
 *  @author Julien Cogan
 *  @date   2010-03-09
 */
struct IL0MuonOverflowTool : extend_interfaces<IAlgTool> {

  // Return the interface ID
  DeclareInterfaceID( IL0MuonOverflowTool, 2, 0 );

  /// Fill the list of quarters with an overflow
  virtual StatusCode getQuarters(std::vector<int> & quarters, std::string rootInTes="")=0;
  /// Fill the list of BCSUs (as MuonTileIDs) with an overflow
  virtual StatusCode getBCSUs(std::vector<LHCb::MuonTileID> & bcsus, std::string rootInTes="")=0;
  /// Fill the list of PUs (as MuonTileIDs) with an overflow
  virtual StatusCode getPUs(std::vector<LHCb::MuonTileID> & pus, std::string rootInTes="")=0;

};
#endif // L0INTERFACES_IL0MUONOVERFLOWTOOL_H
