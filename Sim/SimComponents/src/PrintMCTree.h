/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef PRINTMCTREE_H
#define PRINTMCTREE_H 1

#include "MCInterfaces/IPrintMCDecayTreeTool.h"

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

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

private:
  IPrintMCDecayTreeTool*                  m_printMCTree = nullptr;
  std::vector<std::string>                m_particleNames; ///< particle names
  std::vector<int>                        m_particleIDs;
  int                                     m_depth = -1; ///< depth of tree
  DataObjectReadHandle<LHCb::MCParticles> m_mcParts{this, "MCParticles", LHCb::MCParticleLocation::Default};
};
#endif // PRINTMCTREE_H
