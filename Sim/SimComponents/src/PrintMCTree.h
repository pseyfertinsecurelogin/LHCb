#ifndef PRINTMCTREE_H
#define PRINTMCTREE_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
class IDebugTool;


/** @class PrintMCTree PrintMCTree.h
 *
 *  Prints the decay tree of all MC particles with a given PID
 *
 *  @author Patrick KOPPENBURG
 *  @date   2004-09-10
 */
class PrintMCTree : public GaudiAlgorithm {
public:
  /// Standard constructor
  PrintMCTree( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  IPrintMCDecayTreeTool *m_printMCTree = nullptr;
  std::vector<std::string> m_particleNames ; ///< particle names
  std::vector<int> m_particleIDs ;
  int m_depth = -1 ;  ///< depth of tree

};
#endif // PRINTMCTREE_H
