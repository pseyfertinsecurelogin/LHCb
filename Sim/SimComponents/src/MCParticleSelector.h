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
#ifndef SIMCOMPONENTS_MCPARTICLESELECTOR_H
#define SIMCOMPONENTS_MCPARTICLESELECTOR_H

// base class
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/SystemOfUnits.h"

// interface
#include "MCInterfaces/IMCParticleSelector.h"

#include "boost/limits.hpp"
#include "boost/numeric/conversion/bounds.hpp"

/** @class MCParticleSelector MCParticleSelector.h
 *
 *  Class for selection of MCParticles given a criteria
 *
 *  @author M.Needham
 *  @date   4/12/2008 [revised]
 *  @date   13/3/2002
 */

class MCParticleSelector : public extends<GaudiTool, IMCParticleSelector>
{

public:

  /// constructor
  using base_class::base_class;

   // Tool initialization
  StatusCode initialize() override;

  // select
  bool accept(const LHCb::MCParticle* aParticle) const override;

private: // methods

  /// Determine if the given MCParticle is from a B decay or not
  bool fromBdecay( const LHCb::MCParticle * aParticle ) const;

private: // data

  Gaudi::Property<double> m_zOrigin { this,"zOrigin", boost::numeric::bounds<double>::highest() };  ///< Maximum z position of origin vertex
  Gaudi::Property<double> m_pMin { this,"pMin", 0.0*Gaudi::Units::GeV };                      ///< Minimum momentum
  Gaudi::Property<double> m_pMax { this,"pMax", boost::numeric::bounds<double>::highest() };  ///< Maximum momentum
  Gaudi::Property<double> m_betaGammaMin { this,"betaGammaMin", 0.0 };                                        ///< Minimum cut on beta * gamma

  Gaudi::Property<double> m_etaMin { this,"etaMin", -boost::numeric::bounds<double>::highest() };  ///< Min eta cut
  Gaudi::Property<double> m_etaMax { this,"etaMax",  boost::numeric::bounds<double>::highest() };  ///< Max eta cut

  Gaudi::Property<bool> m_selCharged { this,"SelectChargedParticles", true };  ///< Select charged particles
  Gaudi::Property<bool> m_selNeutral { this,"SelectNeutralParticles", true };  ///< Select neutral particles

  Gaudi::Property<bool> m_rejectElectrons { this,"rejectElectrons", false };   ///< Reject electrons

  Gaudi::Property<bool> m_rejectInteractions { this,"rejectInteractions", false };                         ///< Reject interactions
  Gaudi::Property<double> m_zInteraction { this,"zInteraction", -boost::numeric::bounds<double>::highest() };///< z of the interaction

  Gaudi::Property<bool> m_selBprods { this,"SelectOnlyBDecayProducts", false };  ///< Select only b decay products

};

#endif // SIMCOMPONENTS_MCPARTICLESELECTOR_H
