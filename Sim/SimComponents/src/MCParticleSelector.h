#ifndef _MCPARTICLESELECTOR_H
#define _MCPARTICLESELECTOR_H

// base class
#include "GaudiAlg/GaudiTool.h"

// interface
#include "Kernel/IMCParticleSelector.h"

/** @class MCParticleSelector MCParticleSelector.h MCTools/MCParticleSelector.h
 *
 *  Interface Class for selection of MCParticles given a criteria
 *
 *  @author M.Needham
 *  @date   13/3/2002
 */

class MCParticleSelector : public GaudiTool,
                           virtual public IMCParticleSelector
{

public:

  /// constructor
  MCParticleSelector( const std::string& type,
                      const std::string& name,
                      const IInterface* parent );

  // destructor
  virtual ~MCParticleSelector();

  // select
  virtual bool accept(const LHCb::MCParticle* aParticle) const;

private:

  double m_zOrigin;       ///< Maximum z position of origin vertex
  double m_pMin;          ///< Minimum momentum
  double m_pMax;          ///< Maximum momentum
  double m_betaGammaMin;  ///< Minimum cut on beta * gamma

  double m_etaMax;        ///< Max eta cut
  double m_etaMin;        ///< Min eta cut

  bool m_selCharged;      ///< Select charged particles
  bool m_selNeutral;      ///< Select neutral particles

  bool m_rejectElectrons; /// Reject electrons

};

#endif // _MCPARTICLESELECTOR_H
