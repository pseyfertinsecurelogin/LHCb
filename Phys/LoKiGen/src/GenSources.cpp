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
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/SmartDataPtr.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/GenAlgs.h"
#include "LoKi/GenExtract.h"
#include "LoKi/GenSources.h"
#include "LoKi/Services.h"
// ============================================================================
/** @file
 *  Implementation file for various sources
 *  @author Vanya BELYAEV ibelyav@physics.syr.edu
 *  @date 2006-12-07
 */
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::GenParticles::SourceTES::SourceTES( const IDataProviderSvc* svc, const LoKi::GenTypes::GCuts& cuts,
                                          const std::string& path )
    : LoKi::AuxFunBase( std::tie( svc, cuts, path ) ), SourceTES::_Base( svc, path ), m_cut( cuts ) {}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::GenParticles::SourceTES::SourceTES( const IDataProviderSvc* svc, const std::string& path )
    : LoKi::AuxFunBase( std::tie( svc, path ) )
    , SourceTES::_Base( svc, path )
    , m_cut( LoKi::BasicFunctors<const HepMC::GenParticle*>::BooleanConstant( true ) ) {}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::GenParticles::SourceTES::SourceTES( const GaudiAlgorithm* svc, const LoKi::GenTypes::GCuts& cuts,
                                          const std::string& path, const bool useRootInTES )
    : LoKi::AuxFunBase( std::tie( svc, cuts, path, useRootInTES ) )
    , SourceTES::_Base( svc, path, useRootInTES )
    , m_cut( cuts ) {}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::GenParticles::SourceTES::SourceTES( const GaudiAlgorithm* svc, const std::string& path, const bool useRootInTES )
    : LoKi::AuxFunBase( std::tie( svc, path, useRootInTES ) )
    , SourceTES::_Base( svc, path, useRootInTES )
    , m_cut( LoKi::BasicFunctors<const HepMC::GenParticle*>::BooleanConstant( true ) ) {}
// ============================================================================
// MANDATORY: the only essential method:
std::vector<const HepMC::GenParticle*> LoKi::GenParticles::SourceTES::operator()() const {

  const LHCb::HepMCEvent::Container* events = LoKi::TES::get_<LHCb::HepMCEvent::Container>( *this );
  Assert( events, "No valid data is found at location '" + path() + "'" );
  std::vector<const HepMC::GenParticle*> particles;
  particles.reserve( 2000 );
  //
  LoKi::Extract::getGenParticles( events, std::back_inserter( particles ), m_cut.func() );
  //
  return particles;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::GenParticles::SourceTES::fillStream( std::ostream& o ) const {
  return o << "GSOURCE(" << path() << "," << m_cut << ")";
}
// ============================================================================

// ============================================================================
// constructor
// ============================================================================
LoKi::GenParticles::TESData::TESData( const GaudiAlgorithm* algorithm, const std::string& path )
    : TESData( algorithm, path, LoKi::BasicFunctors<const HepMC::GenParticle*>::BooleanConstant( true ) ) {}
// ============================================================================
// constructor
// ============================================================================
LoKi::GenParticles::TESData::TESData( const GaudiAlgorithm* algorithm, const std::string& path,
                                      const LoKi::GenTypes::GCuts& cuts )
    : LoKi::AuxFunBase( std::tie( algorithm, path, cuts ) )
    , LoKi::BasicFunctors<const HepMC::GenParticle*>::Source()
    , LoKi::TES::DataHandle<LHCb::HepMCEvent::Container>( algorithm, path )
    , m_cuts( cuts ) {}
// ============================================================================
// constructor
// ============================================================================
LoKi::GenParticles::TESData::TESData( const GaudiAlgorithm* algorithm, const LoKi::GenTypes::GCuts& cuts,
                                      const std::string& path )
    : TESData( algorithm, path, cuts ) {}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::GenParticles::TESData* LoKi::GenParticles::TESData::clone() const {
  return new LoKi::GenParticles::TESData( *this );
}
// ============================================================================
// MANDATORY: the only essential method:
// ============================================================================
LoKi::GenParticles::TESData::result_type LoKi::GenParticles::TESData::operator()() const {
  const auto* data = get();
  result_type result;
  result.reserve( data->size() / 4 );
  LoKi::Extract::genParticles( data, std::back_inserter( result ), m_cuts );
  return result;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::GenParticles::TESData::fillStream( std::ostream& o ) const {
  return o << "GTESDATA('" << location() << "'," << m_cuts << ")";
}
// ============================================================================

// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::GenParticles::TESCounter::TESCounter( const LoKi::BasicFunctors<const HepMC::GenParticle*>::Source& s )
    : LoKi::AuxFunBase( std::tie( s ) ), LoKi::Functor<void, double>(), m_source( s ) {}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::GenParticles::TESCounter* LoKi::GenParticles::TESCounter::clone() const {
  return new LoKi::GenParticles::TESCounter( *this );
}
// ============================================================================
// MANDATORY: the only essential method:
// ============================================================================
double LoKi::GenParticles::TESCounter::operator()() const { return m_source().size(); }
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::GenParticles::TESCounter::fillStream( std::ostream& o ) const {
  return o << "GNUM(" << m_source << ")";
}
// ============================================================================

// ============================================================================
//                                                                      The END
// ============================================================================
