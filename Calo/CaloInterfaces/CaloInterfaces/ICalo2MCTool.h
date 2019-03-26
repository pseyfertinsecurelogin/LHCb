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
#ifndef ICALO2MCTOOL_H
#define ICALO2MCTOOL_H 1

// Include files
// from STL
#include <string>
// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/IProperty.h"

namespace LHCb {
  class CaloCluster;
  class CaloDigit;
  class CaloHypo;
  class ProtoParticle;
  class Particle;
  class MCParticle;
  class ParticleID;
} // namespace LHCb

/** @class ICalo2MCTool ICalo2MCTool.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2009-07-27
 */
struct ICalo2MCTool : extend_interfaces<IAlgTool, IProperty> {

  DeclareInterfaceID( ICalo2MCTool, 5, 0 );

  // setters
  virtual ICalo2MCTool* from( const LHCb::CaloDigit* digit )     = 0;
  virtual ICalo2MCTool* from( const LHCb::CaloCluster* cluster ) = 0;
  virtual ICalo2MCTool* from( const LHCb::CaloHypo* hypo )       = 0;
  virtual ICalo2MCTool* from( const LHCb::ProtoParticle* proto ) = 0;
  virtual ICalo2MCTool* from( const LHCb::Particle* particle )   = 0;
  // getters
  virtual const LHCb::MCParticle* bestMC() const                                                   = 0;
  virtual const LHCb::MCParticle* maxMC() const                                                    = 0;
  virtual const LHCb::MCParticle* findMC( LHCb::ParticleID id, double threshold = 0. ) const       = 0;
  virtual const LHCb::MCParticle* findMCOrBest( LHCb::ParticleID id, double threshold = 0. ) const = 0;
  virtual const LHCb::MCParticle* findMC( std::string name, double threshold = 0. ) const          = 0;
  virtual const LHCb::MCParticle* findMCOrBest( std::string name, double threshold = 0. ) const    = 0;
  virtual double                  weight( const LHCb::MCParticle* ) const                          = 0;
  virtual double                  quality( const LHCb::MCParticle* ) const                         = 0;
  virtual std::string             descriptor() const                                               = 0;
  virtual bool                    isCalo( LHCb::Particle* particle ) const                         = 0;
  virtual bool                    isPureNeutralCalo( const LHCb::Particle* particle ) const        = 0;
};
#endif // ICALO2MCTOOL_H
