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
// ============================================================================
#pragma once
#include "Event/HepMCEvent.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "Kernel/ParticleID.h"
#include <string>
#include <vector>

namespace LHCb {
  class IParticlePropertySvc;
}

namespace HepMC {
  class GenParticle;
  class GenVertex;
} // namespace HepMC

/** @class DumpHepMCDecay DumpHepMCDecay.h
 *
 *  Dump the decays of certain particles
 *  This class was previously called DumpMCDecay (Gen/Generators package)
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date   2004-02-18
 */
class DumpHepMCDecay : public GaudiAlgorithm {
public:
  /// the actual type of container with addresses
  typedef std::vector<std::string> Addresses;
  /// the actual type of list of PIDs
  typedef std::vector<int> PIDs;

public:
  /** initialization of the algoritm
   *  @see GaudiAlgorithm
   *  @see      Algorithm
   *  @see     IAlgorithm
   *  @return status code
   */
  StatusCode initialize() override;
  /** execution of the algoritm
   *  @see IAlgorithm
   *  @return status code
   */
  StatusCode execute() override;

public:
  /** print the decay tree of the particle
   *  @param particle pointer to teh particle to be printed
   *  @param stream   output stream
   *  @param level    decay level
   *  @return statsu code
   */
  StatusCode printDecay( const HepMC::GenParticle* particle, std::ostream& stream = std::cout,
                         unsigned int level = 0 ) const;
  /** get the particle name in the string fixed form
   *  @param particle pointer to the particle
   *  @param particle name
   */
  std::string particleName( const HepMC::GenParticle* particle ) const;

  /** standard constructor
   *  @see GaudiAlgorithm
   *  @see      Algorithm
   *  @see      AlgFactory
   *  @see     IAlgFactory
   *  @param name algorithm instance's name
   *  @param iscv pointer to Service Locator
   */
  DumpHepMCDecay( const std::string& name, ISvcLocator* isvc );

protected:
  // addresses of HepMC events
  Gaudi::Property<Addresses> m_addresses{this, "Addresses", {LHCb::HepMCEventLocation::Default}};
  // particles to be printed
  Gaudi::Property<PIDs> m_particles{this, "Particles", {}};
  // quarks to be printes
  Gaudi::Property<PIDs> m_quarks{this, "Quarks", {LHCb::ParticleID::bottom}};
  // maximal number of levels
  Gaudi::Property<int> m_levels{this, "MaxLevels", 4};

private:
  // pointer to particle property service
  mutable LHCb::IParticlePropertySvc* m_ppSvc{nullptr};
};
// ============================================================================
// The END
// ============================================================================
