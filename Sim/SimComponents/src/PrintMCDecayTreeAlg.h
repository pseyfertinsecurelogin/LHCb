/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef PRINTMCDECAYTREEALG_H
#define PRINTMCDECAYTREEALG_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"


// forward declaration
struct IPrintMCDecayTreeTool;

/** @class PrintMCDecayTreeAlg PrintMCDecayTreeAlg.h
 *  Prints a dump of the MC event tree, using an implementation of the
 *  IPrintMCDecayTreeTool interface.
 *
 *  The MC event tree is defined by the TES location of an
 *  LHCb::MCParticle container. This is controlled by the property
 *  <b>MCParticleLocation</b>. The default is LHCb::MCParticleLocation::Default
 *
 *  The IPrintMCDecayTreeTool implementation to use is controlled by the
 *  property <b>PrintTool</b>. the default is "PrintMCDecayTreeTool".
 *
 *  @author Vladimir Gligorov, adapted by Marco Cattaneo
 *  @date   26/11/2007
 */
class PrintMCDecayTreeAlg : public GaudiAlgorithm {
public:
  /// Standard constructor
  PrintMCDecayTreeAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  IPrintMCDecayTreeTool *m_printTool = nullptr;
  std::string m_printToolName = "PrintMCDecayTreeTool";
  std::string m_particleLocation =  LHCb::MCParticleLocation::Default;

};
#endif // PRINTMCDECAYTREEALG_H
