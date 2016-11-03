#ifndef L0INTERFACES_IL0MUONMODIFYINPUTTOOL_H
#define L0INTERFACES_IL0MUONMODIFYINPUTTOOL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "Kernel/MuonTileID.h"


/** @class IL0MuonModifyInputTool IL0MuonModifyInputTool.h L0Interfaces/IL0MuonModifyInputTool.h
 *
 *
 *  @author Julien Cogan
 *  @date   2013-01-21
 */
struct IL0MuonModifyInputTool : extend_interfaces<IAlgTool> {

  // Declare the interface ID
  DeclareInterfaceID( IL0MuonModifyInputTool, 2, 0 );

  virtual StatusCode modifyInput(std::vector<LHCb::MuonTileID> &digits)=0;

};
#endif // L0INTERFACES_IL0MUONMODIFYINPUTTOOL_H
