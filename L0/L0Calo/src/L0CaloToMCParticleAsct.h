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
// $Id: L0CaloToMCParticleAsct.h,v 1.4 2005-01-12 09:19:38 ocallot Exp $
#ifndef L0CALOTOMCPARTICLEASCT_H
#define L0CALOTOMCPARTICLEASCT_H 1

// Include files

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

class MCParticle;
class L0CaloCandidate;

/** @class L0CaloToMCParticleAsct L0CaloToMCParticleAsct.h
 *
 *
 *  @author Olivier Callot
 *  @date   2002-07-01
 */
class L0CaloToMCParticleAsct : public GaudiAlgorithm {
public:
  /// Standard constructor
  L0CaloToMCParticleAsct( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~L0CaloToMCParticleAsct( ); ///< Destructor

  StatusCode execute   () override;    ///< Algorithm execution

protected:

private:
  std::string m_inputContainer;
  double      m_minimalFraction;
};
#endif // L0CALOTOMCPARTICLEASCT_H
