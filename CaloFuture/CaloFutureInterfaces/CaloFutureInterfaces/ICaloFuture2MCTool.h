#ifndef ICALOFUTURE2MCTOOL_H
#define ICALOFUTURE2MCTOOL_H 1

// Include files
// from STL
#include <string>
// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/IProperty.h"

namespace LHCb{
  class CaloCluster;
  class CaloDigit;
  class CaloHypo;
  class ProtoParticle;
  class Particle;
  class MCParticle;
  class ParticleID;
}

/** @class ICaloFuture2MCTool ICaloFuture2MCTool.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2009-07-27
 */
struct ICaloFuture2MCTool : extend_interfaces<IAlgTool,IProperty>
{

  DeclareInterfaceID( ICaloFuture2MCTool, 5, 0 );

  // setters
  virtual ICaloFuture2MCTool* from(const LHCb::CaloDigit*     digit    )=0;
  virtual ICaloFuture2MCTool* from(const LHCb::CaloCluster*   cluster  )=0;
  virtual ICaloFuture2MCTool* from(const LHCb::CaloHypo*      hypo     )=0;
  virtual ICaloFuture2MCTool* from(const LHCb::ProtoParticle* proto    )=0;
  virtual ICaloFuture2MCTool* from(const LHCb::Particle*      particle )=0;
  // getters
  virtual const LHCb::MCParticle* bestMC() const = 0;
  virtual const LHCb::MCParticle* maxMC() const = 0;
  virtual const LHCb::MCParticle* findMC(LHCb::ParticleID id, double threshold = 0. ) const = 0;
  virtual const LHCb::MCParticle* findMCOrBest(LHCb::ParticleID id, double threshold = 0. ) const = 0;
  virtual const LHCb::MCParticle* findMC(std::string name, double threshold = 0. ) const = 0;
  virtual const LHCb::MCParticle* findMCOrBest(std::string name, double threshold = 0. ) const = 0;
  virtual double weight(const LHCb::MCParticle*) const  = 0;
  virtual double quality(const LHCb::MCParticle*) const = 0;
  virtual std::string descriptor() const = 0;
  virtual bool isCaloFuture(LHCb::Particle* particle) const = 0;
  virtual bool isPureNeutralCaloFuture(const LHCb::Particle* particle) const = 0;

};
#endif // ICALOFUTURE2MCTOOL_H
