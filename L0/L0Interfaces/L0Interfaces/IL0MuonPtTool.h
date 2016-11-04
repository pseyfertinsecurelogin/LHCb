#ifndef COMPONENT_IL0MUONPTTOOL_H 
#define COMPONENT_IL0MUONPTTOOL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "Event/L0MuonCandidate.h"
#include "Kernel/MuonTileID.h"

/** @class IL0MuonPtTool IL0MuonPtTool.h component/IL0MuonPtTool.h
 *
 *  @author Julien Cogan
 *  @date   2012-04-17
 */
struct IL0MuonPtTool : extend_interfaces<IAlgTool> {

  // Return the interface ID
  DeclareInterfaceID( IL0MuonPtTool, 2, 0 );

  typedef std::map<LHCb::MuonTileID,int> m1ptMap;
  typedef std::map< LHCb::MuonTileID, m1ptMap > m2m1Lut;

  virtual StatusCode readFromFile(std::string filename)=0;
  
  virtual void getCandidatePads(LHCb::L0MuonCandidate * l0candidate, LHCb::MuonTileID &m2, LHCb::MuonTileID &m1)=0;
  virtual int getPTFromLUT( LHCb::MuonTileID m2, LHCb::MuonTileID m1)=0;
  virtual int getCandidatePT(LHCb::L0MuonCandidate * l0candidate)=0;

};
#endif // COMPONENT_IL0MUONPTTOOL_H
