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
#ifndef PACKMCPARTICLE_H
#define PACKMCPARTICLE_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

/** @class PackMCParticle PackMCParticle.h
 *  Pack the MCParticles
 *
 *  @author Olivier Callot
 *  @date   2005-03-18
 */
class PackMCParticle : public GaudiAlgorithm {

public:
  /// Standard constructor
  PackMCParticle( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:
  std::string m_inputName;
  std::string m_outputName;
  bool        m_alwaysOutput; ///< Flag to turn on the creation of output, even when input is missing
  bool        m_deleteInput;  ///< Flag to turn on the removal of the input data after packing
};

#endif // PACKMCPARTICLE_H
