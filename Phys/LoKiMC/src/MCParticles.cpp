// ============================================================================
// Include files
// ============================================================================
// STD &STL
// ============================================================================
#include <algorithm>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/ToStream.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/MCTypes.h"
#include "LoKi/MCParticles.h"
#include "LoKi/Kinematics.h"
#include "LoKi/MCTrees.h"
#include "LoKi/Print.h"
#include "LoKi/MCChild.h"
#include "LoKi/MCAlgs.h"
#include "LoKi/Math.h"
#include "LoKi/ParticleProperties.h"
#include "LoKi/ToCpp.h"
// ============================================================================
// MCInterfaces
// ============================================================================
#include "MCInterfaces/IMCDecayFinder.h"
#include "MCInterfaces/IMCParticleSelector.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/format.hpp"
// ============================================================================
/*  @file
 *
 *  Implementation file for functions from namespace LoKi::MCParticles
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
namespace LoKi { namespace MCParticles {
// ============================================================================
double Momentum::operator() ( const LHCb::MCParticle* p ) const
{
  if ( p ) { return p -> momentum ().P() ; }    // RETURN
  Error (" Invalid Particle, return 'InvalidMomenum'") ;
  return LoKi::Constants::InvalidMomentum;                   // RETURN
}
// ============================================================================
Momentum* Momentum::clone() const { return new Momentum(*this) ; }
// ============================================================================
std::ostream&
Momentum::fillStream( std::ostream& s ) const
{ return s << "MCP" ; }
// ============================================================================
double Energy::operator() ( const LHCb::MCParticle* p ) const
{
  if ( p ) { return p -> momentum () .e ()  ; }          // RETURN
  Error (" Invalid Particle, return 'InvalidEnergy'") ;
  return LoKi::Constants::InvalidEnergy;                     // RETURN
}
// ============================================================================
Energy* Energy::clone() const { return new Energy(*this) ; }
// ============================================================================
std::ostream& Energy::fillStream( std::ostream& s ) const
{ return s << "MCE" ; }
// ============================================================================
bool Oscillated::operator() ( const LHCb::MCParticle* p ) const
{
  if ( p ) { return p -> hasOscillated() ; }          // RETURN
  Error (" Invalid Particle, return 'false'") ;
  return false ;                     // RETURN
}
// ============================================================================
Oscillated* Oscillated::clone() const { return new Oscillated(*this) ; }
// ============================================================================
std::ostream& Oscillated::fillStream( std::ostream& s ) const
{ return s << "MCOSCILLATED" ; }
// ============================================================================
double TransverseMomentum::operator() ( const LHCb::MCParticle* p ) const
{
  if ( p ) { return p -> momentum().Pt() ; }           // RETURN
  Error(" Invalid Particle, return 'InvalidMomentum'") ;
  return LoKi::Constants::InvalidMomentum;                   // RETURN
}
// ============================================================================
TransverseMomentum* TransverseMomentum::clone() const
{ return new TransverseMomentum(*this) ; }
// ============================================================================
std::ostream& TransverseMomentum::fillStream ( std::ostream& s ) const
{ return s << "MCPT" ; }
// ============================================================================
double MomentumX::operator() ( const LHCb::MCParticle* p ) const
{
  if( p ) { return p -> momentum () . px () ; }    // RETURN
  Error(" Invalid Particle, return 'InvalidMomenum'") ;
  return LoKi::Constants::InvalidMomentum;                   // RETURN
}
// ============================================================================
MomentumX* MomentumX::clone() const { return new MomentumX(*this) ; }
// ============================================================================
std::ostream& MomentumX::fillStream ( std::ostream& s ) const
{ return s << "MCPX" ; }
// ============================================================================
double MomentumY::operator() ( const LHCb::MCParticle* p ) const
{
  if( p ) { return p -> momentum () . py () ; }    // RETURN
  Error(" Invalid Particle, return 'InvalidMomenum'") ;
  return LoKi::Constants::InvalidMomentum;                   // RETURN
}
// ============================================================================
MomentumY* MomentumY::clone() const { return new MomentumY(*this) ; }
// ============================================================================
std::ostream& MomentumY::fillStream ( std::ostream& s ) const
{ return s << "MCPY" ; }
// ============================================================================
double MomentumZ::operator() ( const LHCb::MCParticle* p ) const
{
  if( p ) { return p -> momentum () . pz () ; }    // RETURN
  Error(" Invalid Particle, return 'InvalidMomenum'") ;
  return LoKi::Constants::InvalidMomentum;                   // RETURN
}
// ============================================================================
MomentumZ* MomentumZ::clone() const { return new MomentumZ(*this) ; }
// ============================================================================
std::ostream& MomentumZ::fillStream ( std::ostream& s ) const
{ return s << "MCPZ" ; }
// ============================================================================
double PseudoRapidity::operator() ( const LHCb::MCParticle* p ) const
{
  if( p ) { return eta ( p ) ; }    // RETURN
  Error(" Invalid Particle, return 'InvalidMomenum'") ;
  return LoKi::Constants::InvalidMomentum;                   // RETURN
}
// ============================================================================
PseudoRapidity* PseudoRapidity::clone() const
{ return new PseudoRapidity(*this) ; }
// ============================================================================
std::ostream& PseudoRapidity::fillStream ( std::ostream& s ) const
{ return s << "MCETA" ; }
// ============================================================================
double PseudoRapidity::y ( const Gaudi::LorentzVector& v ) const
{
  const double e  = v.E  () ;
  const double pz = v.Pz () ;
  return 0.5*std::log( (e+pz)/(e-pz) ) ;
}
// ============================================================================
double PseudoRapidity::y0 ( const Gaudi::LorentzVector& v ) const
{
  const double e  = v.E  () ;
  const double p  = v.P  () ;
  return 0.5*std::log( (e+p)/(e-p) ) ;
}
// ============================================================================
Rapidity* Rapidity::clone() const { return new Rapidity(*this) ; }
// ============================================================================
Rapidity0* Rapidity0::clone() const { return new Rapidity0(*this) ; }
// ============================================================================
double Rapidity::operator() ( const LHCb::MCParticle* p ) const
{
  if ( p ) { return y ( p ) ; }    // RETURN
  Error(" Invalid Particle, return 'InvalidMomenum'") ;
  return LoKi::Constants::InvalidMomentum;                   // RETURN
}
// ============================================================================
double Rapidity0::operator() ( const LHCb::MCParticle* p ) const
{
  if ( p ) { return y0 ( p ) ; }    // RETURN
  Error(" Invalid Particle, return 'InvalidMomenum'") ;
  return LoKi::Constants::InvalidMomentum;                   // RETURN
}
// ============================================================================
std::ostream& Rapidity::fillStream ( std::ostream& s ) const
{ return s << "MCY" ; }
// ============================================================================
std::ostream& Rapidity0::fillStream ( std::ostream& s ) const
{ return s << "MCY0" ; }
// ============================================================================


// ============================================================================
double Theta::operator() ( const LHCb::MCParticle* p ) const
{
  if( p ) { return p -> momentum () . theta () ; }    // RETURN
  Error(" Invalid Particle, return 'InvalidMomenum'");
  return LoKi::Constants::InvalidMomentum;                   // RETURN
}
// ============================================================================
Theta* Theta::clone() const { return new Theta(*this) ; }
// ============================================================================
std::ostream& Theta::fillStream ( std::ostream& s ) const
{ return s << "MCTHETA" ; }
// ============================================================================
double Phi::operator() ( const LHCb::MCParticle* p ) const
{
  if ( p ) { return adjust ( phi ( p ) ) ; }    // RETURN
  Error(" Invalid Particle, return 'InvalidMomenum'");
  return LoKi::Constants::InvalidMomentum;                   // RETURN
}
// ============================================================================
Phi* Phi::clone() const { return new Phi(*this) ; }
// ============================================================================
std::ostream& Phi::fillStream ( std::ostream& s ) const
{ return s << "MCPHI" ; }
// ============================================================================
double Mass::operator() ( const LHCb::MCParticle* p ) const
{
  if( p ) { return p -> momentum () . M  () ; }     // RETURN
  Error(" Invalid Particle, return 'InvalidMass'");
  return LoKi::Constants::InvalidMass;                   // RETURN
}
// ============================================================================
Mass* Mass::clone() const { return new Mass(*this) ; }
// ============================================================================
std::ostream& Mass::fillStream ( std::ostream& s ) const
{ return s << "MCMASS" ; }
// ============================================================================
double Identifier::operator() ( const LHCb::MCParticle* p ) const
{
  if( p ) { return p -> particleID() . pid () ; }         // RETURN
  Error(" Invalid Particle, return 'InvalidID'");
  return LoKi::Constants::InvalidID;                           // RETURN
}
// ============================================================================
Identifier* Identifier::clone() const { return new Identifier(*this) ; }
// ============================================================================
std::ostream& Identifier::fillStream ( std::ostream& s ) const
{ return s << "MCID" ; }
// ============================================================================
double AbsIdentifier::operator() ( const LHCb::MCParticle* p ) const
{
  if( p ) { return p -> particleID() . abspid () ; }     // RETURN
  Error(" Invalid Particle, return 'InvalidID'");
  return LoKi::Constants::InvalidID;                           // RETURN
}
// ============================================================================
// IsID
// ============================================================================
IsID::IsID ( const long                      id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , LoKi::Pids::GetPids ( id )
{}
// ============================================================================
IsID::IsID( const unsigned long  id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , LoKi::Pids::GetPids ( id )
{}
// ============================================================================
IsID::IsID( const LHCb::ParticleID&         id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , LoKi::Pids::GetPids ( id )
{}
// ============================================================================
IsID::IsID( const std::string& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , LoKi::Pids::GetPids ( id )
{}
// ============================================================================
IsID::IsID( const std::vector<int>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , LoKi::Pids::GetPids ( id )
{}
// ============================================================================
IsID::IsID( const std::vector<unsigned int>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , LoKi::Pids::GetPids ( id )
{}
// ============================================================================
IsID::IsID( const std::vector<long>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , LoKi::Pids::GetPids ( id )
{}
// ============================================================================
IsID::IsID( const std::vector<unsigned long>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , LoKi::Pids::GetPids ( id )
{}
// ============================================================================
IsID::IsID( const std::vector<LHCb::ParticleID>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , LoKi::Pids::GetPids ( id )
{}
// ============================================================================
IsID::IsID( const std::vector<std::string>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , LoKi::Pids::GetPids ( id )
{}
// ============================================================================
IsID::IsID( const LoKi::Pids::GetPids& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , LoKi::Pids::GetPids ( id )
{}
// ============================================================================
IsID*
IsID::clone() const
{ return new LoKi::MCParticles::IsID(*this) ; }
// ============================================================================
std::ostream& IsID::fillStream( std::ostream& s ) const
{
  //
  s << "(MCID== " ;
  //
  if      ( 1 == m_names.size() ) { Gaudi::Utils::toStream ( m_names[0] , s ) ; }
  else if ( !m_names.empty()    ) { Gaudi::Utils::toStream ( m_names    , s ) ; }
  else if ( 1 == m_ints.size()  ) { Gaudi::Utils::toStream ( m_ints [0] , s ) ; }
  else                            { Gaudi::Utils::toStream ( m_ints     , s ) ; }
  //
  return s << ")" ;
}
// ============================================================================
// the only one essential method
// ============================================================================
bool
IsID::operator()
  ( const LHCb::MCParticle* p ) const
{
  //
  if ( !p )
  {
    Error("Invalid Particle, return 'False'");
    return false ;                     // RETURN
  }
  //
  return in_list ( p->particleID () ) ;
}
// ============================================================================
// IsNotID
// ============================================================================
IsNotID::IsNotID ( const long                id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsNotID::IsNotID( const unsigned long  id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsNotID::IsNotID( const LHCb::ParticleID&         id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsNotID::IsNotID( const std::string& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsNotID::IsNotID( const std::vector<int>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsNotID::IsNotID( const std::vector<unsigned int>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsNotID::IsNotID( const std::vector<long>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsNotID::IsNotID( const std::vector<unsigned long>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsNotID::IsNotID( const std::vector<LHCb::ParticleID>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsNotID::IsNotID( const std::vector<std::string>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsNotID*
IsNotID::clone() const
{ return new IsNotID(*this) ; }
// ============================================================================
std::ostream& IsNotID::fillStream( std::ostream& s ) const
{
  //
  s << "(MCID!= " ;
  //
  if      ( 1 == m_names.size() ) { Gaudi::Utils::toStream ( m_names[0] , s ) ; }
  else if ( !m_names.empty()    ) { Gaudi::Utils::toStream ( m_names    , s ) ; }
  else if ( 1 == m_ints.size()  ) { Gaudi::Utils::toStream ( m_ints [0] , s ) ; }
  else                            { Gaudi::Utils::toStream ( m_ints     , s ) ; }
  //
  return s << ")" ;
}
// ============================================================================
// the only one essential method
// ============================================================================
bool IsNotID::operator() ( const LHCb::MCParticle* p ) const
{
  //
  if ( !p )
  {
    Error("Invalid Particle, return 'False'");
    return false ;                     // RETURN
  }
  return not_in_list ( p->particleID() ) ;
}


// ============================================================================
// IsAbsID
// ============================================================================
IsAbsID::IsAbsID ( const long id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsAbsID::IsAbsID ( const unsigned long  id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsAbsID::IsAbsID ( const LHCb::ParticleID&         id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsAbsID::IsAbsID ( const std::string& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsAbsID::IsAbsID ( const std::vector<int>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsAbsID::IsAbsID ( const std::vector<unsigned int>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsAbsID::IsAbsID ( const std::vector<long>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsAbsID::IsAbsID ( const std::vector<unsigned long>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsAbsID::IsAbsID ( const std::vector<LHCb::ParticleID>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsAbsID::IsAbsID ( const std::vector<std::string>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsAbsID::IsAbsID ( const LoKi::Pids::GetPids& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsID ( id )
{}
// ============================================================================
IsAbsID* IsAbsID::clone() const { return new IsAbsID(*this) ; }
// ============================================================================
std::ostream& IsAbsID::fillStream( std::ostream& s ) const
{
  //
  s << "(MCABSID== " ;
  //
  if      ( 1 == m_names.size() ) { Gaudi::Utils::toStream ( m_names[0] , s ) ; }
  else if ( !m_names.empty()    ) { Gaudi::Utils::toStream ( m_names    , s ) ; }
  else if ( 1 == m_ints.size()  ) { Gaudi::Utils::toStream ( m_ints [0] , s ) ; }
  else                            { Gaudi::Utils::toStream ( m_ints     , s ) ; }
  //
  return s << ")" ;
}
// ============================================================================
// the only one essential method
// ============================================================================
bool IsAbsID::operator() ( const LHCb::MCParticle* p ) const
{
  //
  if ( !p )
  {
    Error("Invalid Particle, return 'False'");
    return false ;                     // RETURN
  }
  //
  return in_abs_list ( p->particleID() ) ;
}




// ============================================================================
// IsNotAbsID
// ============================================================================
IsNotAbsID::IsNotAbsID ( const long         id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsAbsID ( id )
{}
// ============================================================================
IsNotAbsID::IsNotAbsID ( const unsigned long id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsAbsID ( id )
{}
// ============================================================================
IsNotAbsID::IsNotAbsID ( const LHCb::ParticleID&         id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsAbsID ( id )
{}
// ============================================================================
IsNotAbsID::IsNotAbsID ( const std::string& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsAbsID ( id )
{}
// ============================================================================
IsNotAbsID::IsNotAbsID ( const std::vector<int>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsAbsID ( id )
{}
// ============================================================================
IsNotAbsID::IsNotAbsID ( const std::vector<unsigned int>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsAbsID ( id )
{}
// ============================================================================
IsNotAbsID::IsNotAbsID ( const std::vector<long>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsAbsID ( id )
{}
// ============================================================================
IsNotAbsID::IsNotAbsID ( const std::vector<unsigned long>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsAbsID ( id )
{}
// ============================================================================
IsNotAbsID::IsNotAbsID ( const std::vector<LHCb::ParticleID>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsAbsID ( id )
{}
// ============================================================================
IsNotAbsID::IsNotAbsID ( const std::vector<std::string>& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsAbsID ( id )
{}
// ============================================================================
IsNotAbsID::IsNotAbsID ( const LoKi::Pids::GetPids& id  )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , IsAbsID ( id )
{}
// ============================================================================
IsNotAbsID* IsNotAbsID::clone() const { return new IsNotAbsID(*this) ; }
// ============================================================================
std::ostream& IsNotAbsID::fillStream( std::ostream& s ) const
{
  //
  s << "(MCABSID!= " ;
  //
  if      ( 1 == m_names.size() ) { Gaudi::Utils::toStream ( m_names[0] , s ) ; }
  else if ( !m_names.empty()    ) { Gaudi::Utils::toStream ( m_names    , s ) ; }
  else if ( 1 == m_ints.size()  ) { Gaudi::Utils::toStream ( m_ints [0] , s ) ; }
  else                            { Gaudi::Utils::toStream ( m_ints     , s ) ; }
  //
  return s << ")" ;
}
// ============================================================================
// the only one essential method
// ============================================================================
bool IsNotAbsID::operator() ( const LHCb::MCParticle* p ) const
{
  //
  if ( !p )
  {
    Error("Invalid Particle, return 'False'");
    return false ;                     // RETURN
  }
  //
  return not_in_abs_list ( p->particleID() ) ;
}
// ============================================================================



// ============================================================================
AbsIdentifier* AbsIdentifier::clone() const { return new AbsIdentifier(*this) ; }
// ============================================================================
std::ostream& AbsIdentifier::fillStream ( std::ostream& s ) const
{ return s << "MCABSID" ; }
// ============================================================================
double ThreeCharge::operator() ( const LHCb::MCParticle* p ) const
{
  if( p ) { return p -> particleID().threeCharge()  ; } // RETURN
  Error(" Invalid Particle, return 'InvalidCharge'");
  return LoKi::Constants::InvalidCharge;                     // RETURN
}
// ============================================================================
ThreeCharge* ThreeCharge::clone() const { return new ThreeCharge(*this) ; }
// ============================================================================
std::ostream& ThreeCharge::fillStream ( std::ostream& s ) const
{ return s << "MC3Q" ; }
// ============================================================================
double ProperLifeTime::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error(" Invalid Particle, return 'InvalidTime'");
    return LoKi::Constants::InvalidTime;                     // RETURN
  }
  //
  const LHCb::MCVertex* originVertex = p->originVertex() ;
  if ( !originVertex )
  {
    Error(" Invalid 'originVertex' , return 'InvalidTime'");
    return LoKi::Constants::InvalidTime;                     // RETURN
  }
  //
  if ( p->endVertices().empty() )
  { return LoKi::Constants::InfiniteTime ; }                // RETURN}
  //
  // get the first vertex as "endVertex"
  const LHCb::MCVertex* endVertex = p->endVertices()[0] ;
  // try to find the decay vertex
  if ( !endVertex || ( LHCb::MCVertex::MCVertexType::DecayVertex        != endVertex->type() &&
                       LHCb::MCVertex::MCVertexType::OscillatedAndDecay != endVertex->type() ) )
  {
    for ( const LHCb::MCVertex* ev :  p->endVertices() )
    {

      if      ( !ev ) { continue ; } // CONTINUE
      else if ( LHCb::MCVertex::MCVertexType::DecayVertex         == ev->type() ) { endVertex = ev ; break ; } // BREAK
      else if ( LHCb::MCVertex::MCVertexType::OscillatedAndDecay  == ev->type() ) { endVertex = ev ; break ; } // BREAK
      else if ( LHCb::MCVertex::MCVertexType::HadronicInteraction == ev->type() ) { endVertex = ev ; }         // ???
    }
  }
  //
  if ( !endVertex )
  {
    Error (" Invalid 'endVertex' , return 'InfiniteTime'");
    return LoKi::Constants::InfiniteTime ;                  // RETURN
  }
  // evaluate the distance
  const double dist = ( endVertex     -> position() -
                        originVertex  -> position() ).R();
  // recalculate distance to the time
  const double mass     = p->momentum().M() ;
  const double mom      = p->momentum().P() ;

  if ( 0 >= mass )
  {
    Error (" Mass is invalid, return 'InvalidTime' ");
    return  LoKi::Constants::InvalidTime ;              // RETURN
  }
  if ( 0 >= mom  )
  {
    Error (" |Momentum| is invalid, return 'InvalidTime' ");
    return  LoKi::Constants::InvalidTime ;              // RETURN
  }
  const double betagamma   = mom / mass ;
  //
  if ( 0 >= betagamma  )
  {
    Error (" betagamma is invalid, return 'InvalidTime' ");
    return  LoKi::Constants::InvalidTime ;              // RETURN
  }
  //
  return dist/betagamma ;
}
// ============================================================================
ProperLifeTime* ProperLifeTime::clone() const { return new ProperLifeTime(*this) ; }
// ============================================================================
std::ostream& ProperLifeTime::fillStream ( std::ostream& s ) const
{ return s << "MCCTAU" ; }
// ============================================================================
//  constructor from Quark
// ============================================================================
HasQuark::HasQuark ( const LHCb::ParticleID::Quark quark )
  : m_quark ( quark  )
{}
// ============================================================================
// the only one essential method
// ============================================================================
bool HasQuark::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error("MCParticle* point to NULL, returs 'false'");
    return false ;
  }
  return p->particleID().hasQuark( quark() ) ;
}
// ============================================================================
HasQuark* HasQuark::clone() const { return new HasQuark(*this) ; }
// ============================================================================
std::ostream& HasQuark::fillStream ( std::ostream& s ) const
{
  switch ( quark() )
  {
  case LHCb::ParticleID::top      :
    return s << "TOP"     ;
    break                 ;
  case LHCb::ParticleID::bottom   :
    return s << "BEAUTY"  ;
    break                 ;
  case LHCb::ParticleID::charm    :
    return s << "CHARM"   ;
    break                 ;
  case LHCb::ParticleID::strange  :
    return s << "STRANGE" ;
    break                 ;
  default                 :
    return s << "MCQUARK(" << quark() << ")" ;
    break                 ;
  }
}
// ============================================================================
// the only one essential method
// ============================================================================
bool IsCharged::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error("MCParticle* point to NULL, returs 'false'");
    return false ;
  }
  return 0 != p->particleID().threeCharge() ;
}
// ============================================================================
IsCharged* IsCharged::clone() const { return new IsCharged(*this) ; }
// ============================================================================
std::ostream& IsCharged::fillStream ( std::ostream& s ) const
{ return s << "CHARGED" ; }
// ============================================================================
// the only one essential method
// ============================================================================
bool IsNeutral::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error("MCParticle* point to NULL, returs 'false'");
    return false ;
  }
  return 0 == p->particleID().threeCharge() ;
}
// ============================================================================
IsNeutral* IsNeutral::clone() const { return new IsNeutral(*this) ; }
// ============================================================================
std::ostream& IsNeutral::fillStream ( std::ostream& s ) const
{ return s << "NEUTRAL" ; }
// ============================================================================
// the only one essential method
// ============================================================================
bool IsLepton::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error("MCParticle* point to NULL, returs 'false'");
    return false ;
  }
  return p->particleID().isLepton() ;
}
// ============================================================================
IsLepton* IsLepton::clone() const { return new IsLepton(*this) ; }
// ============================================================================
std::ostream& IsLepton::fillStream ( std::ostream& s ) const
{ return s << "LEPTON" ; }
// ============================================================================
// the only one essential method
// ============================================================================
bool IsMeson::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error("MCParticle* point to NULL, returs 'false'");
    return false ;
  }
  return p->particleID().isMeson() ;
}
// ============================================================================
IsMeson* IsMeson::clone() const { return new IsMeson(*this) ; }
// ============================================================================
std::ostream& IsMeson::fillStream ( std::ostream& s ) const
{ return s << "MESON" ; }
// ============================================================================
// the only one essential method
// ============================================================================
bool IsBaryon::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error("MCParticle* point to NULL, returs 'false'");
    return false ;
  }
  return p->particleID().isBaryon() ;
}
// ============================================================================
IsBaryon* IsBaryon::clone() const { return new IsBaryon(*this) ; }
// ============================================================================
std::ostream& IsBaryon::fillStream ( std::ostream& s ) const
{ return s << "BARYON" ; }
// ============================================================================
// the only one essential method
// ============================================================================
bool IsHadron::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error("MCParticle* point to NULL, returs 'false'");
    return false ;
  }
  return p->particleID().isHadron() ;
}
// ============================================================================
IsHadron* IsHadron::clone() const { return new IsHadron(*this) ; }
// ============================================================================
std::ostream& IsHadron::fillStream ( std::ostream& s ) const
{ return s << "HADRON" ; }
// ============================================================================
// the only one essential method
// ============================================================================
bool IsNucleus::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error("MCParticle* point to NULL, returs 'false'");
    return false ;
  }
  return p->particleID().isNucleus() ;
}
// ============================================================================
IsNucleus* IsNucleus::clone() const { return new IsNucleus(*this) ; }
// ============================================================================
std::ostream& IsNucleus::fillStream ( std::ostream& s ) const
{ return s << "NUCLEUS" ; }
// ============================================================================
FromMCDecayTree::FromMCDecayTree ( const LHCb::MCParticle* mcp )
  : m_cont ( 1 , mcp ) {}
// ============================================================================
FromMCDecayTree::FromMCDecayTree ( const LoKi::MCTypes::MCRange& cnt  )
  : m_cont ( cnt.begin() , cnt.end() ) {}
// ============================================================================
FromMCDecayTree::FromMCDecayTree ( const LoKi::MCTypes::MCContainer& cnt  )
  : m_cont ( cnt.begin() , cnt.end() ) {}
// ============================================================================
FromMCDecayTree::FromMCDecayTree ( const LHCb::MCParticles* cnt  )
  : m_cont ()
{
  if ( !cnt ) { Error(" ::MCParticles* points to NULL!");}
  else
  {
    m_cont.reserve ( cnt->size() ) ;
    std::copy( cnt->begin() , cnt->end() , std::back_inserter ( m_cont ) ) ;
  }
}
// ============================================================================
FromMCDecayTree* FromMCDecayTree::clone() const
{ return new FromMCDecayTree(*this) ; }
// ============================================================================
bool FromMCDecayTree::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error( "MCParticle* points to NULL, return 'false'");
    return false ;
  }
  for ( const LHCb::MCParticle* parent : m_cont )
  {
    if ( !parent )
    { Error("MCParticle* point to NULL, skip it ") ;    continue ; }
    if ( LoKi::MCTrees::fromMCTree( p , parent ) ) { return true ; } // RETURN
  }
  return false ;
}
// ============================================================================
/*  the add MCparticle into the list of tested particles
 *  @param p pointet to the particle
 *  @return status code
 */
// ============================================================================
StatusCode  FromMCDecayTree::add ( const LHCb::MCParticle* p )
{
  if ( !p )
  { Error ("add(): MCParticle* points to NULL") ; return StatusCode::FAILURE ; }
  m_cont.push_back( p ) ;
  return StatusCode::SUCCESS ;
}
// ============================================================================
/*  remove MCParticle from the list of tested MCParticles
 *  @param p pointe to the MCParticle
 *  @return status code
 */
// ============================================================================
StatusCode  FromMCDecayTree::remove ( const LHCb::MCParticle* p )
{
  auto it = std::remove ( m_cont.begin() , m_cont.end() , p ) ;
  m_cont.erase ( it , m_cont.end() ) ;
  return StatusCode::SUCCESS ;
}
// ============================================================================
std::ostream& FromMCDecayTree::fillStream ( std::ostream& s ) const
{ return s << "FROMMCTREE" ; }
// ============================================================================
NinMCdownTree::NinMCdownTree
( const LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate& cut )
  : LoKi::AuxFunBase ( std::tie ( cut ) )
  , m_cut ( cut )
{}
// ============================================================================
NinMCdownTree* NinMCdownTree::clone() const { return new NinMCdownTree(*this); }
// ============================================================================
double NinMCdownTree::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p ) { Warning ( "MCParticle* points to NULL" ) ; }

  double number = 0 ;
  // check the particle itself
  if ( m_cut( p ) ) { number += 1 ; }

  // traverse the tree

  // the traversal is not possible
  if ( !p     ) { return number ; }

  for ( const LHCb::MCVertex* vertex : p->endVertices() )
  {
    if ( !vertex ) { continue ; }
    for ( const LHCb::MCParticle* mcp :vertex->products() )
    {
      if ( !mcp ) { continue ; }
      // start the recursion
      number += (*this)(mcp) ;
    };
  }
  return number ;
}
// ============================================================================
std::ostream& NinMCdownTree::fillStream ( std::ostream& s ) const
{ return s << "NINMCDOWN(" << m_cut << ")" ; }
// ============================================================================
MCMotherFunction::MCMotherFunction
( const LoKi::BasicFunctors<const LHCb::MCParticle*>::Function& fun ,
  const double                                                  val )
  : LoKi::AuxFunBase ( std::tie ( fun , val ) )
  , m_val ( val )
  , m_fun ( fun )
{}
// ============================================================================
MCMotherFunction* MCMotherFunction::clone() const
{ return new MCMotherFunction( *this ) ; }
// ============================================================================
double MCMotherFunction::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  { Error ( ": MCParticle* point to NULL, return -1.e+6" ) ; return -1.e+6 ; }
  const LHCb::MCParticle* mother = p->mother() ;
  if ( !mother ) { return m_val ; }
  return m_fun ( mother ) ;
}
// ============================================================================
std::ostream& MCMotherFunction::fillStream ( std::ostream& s ) const
{ return s << "MCMOTHER(" << m_fun << "," << m_val<< ")" ; }
// ============================================================================
MCMotherPredicate::MCMotherPredicate
( const LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate& cut ,
  const bool                                val )
  : LoKi::AuxFunBase ( std::tie ( cut , val ) )
  , m_val ( val )
  , m_cut ( cut )
{}
// ============================================================================
MCMotherPredicate* MCMotherPredicate::clone() const
{ return new MCMotherPredicate( *this ) ; }
// ============================================================================
bool MCMotherPredicate::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  { Error ( ": MCParticle* point to NULL, return 'false'" ) ; return false ; }
  const LHCb::MCParticle* mother = p->mother() ;
  if ( !mother ) { return m_val ; }
  return m_cut ( mother ) ;
}
// ============================================================================
std::ostream& MCMotherPredicate::fillStream ( std::ostream& s ) const
{ return s << "MCMOTHCUT(" << m_cut << ( m_val ? ",True) " : ",False) " ) ; }
// ============================================================================
/*  constructor from 1 particle
 *  @param object object to be compared
 */
// ============================================================================
IsParticle::IsParticle ( const LHCb::MCParticle* object )
  : m_objects ( 1 , object )
{}
// ============================================================================
/*  constructor from range of particles
 *  @param range range of object to be compared
 */
// ============================================================================
IsParticle::IsParticle ( const LoKi::MCTypes::MCRange& range )
  : m_objects ( range.begin() , range.end() )
{}
// ============================================================================
//  MANDATORY: clone method ('virtual constructor')
// ============================================================================
IsParticle* IsParticle::clone() const { return new IsParticle( *this ) ; }
// ============================================================================
//  MANDATORY: the only one essential method
// ============================================================================
bool IsParticle::operator() ( const LHCb::MCParticle* p ) const
{
  auto it = std::find( m_objects.begin() , m_objects.end() , p ) ;
  return m_objects.end() != it ;
}
// ============================================================================
std::ostream& IsParticle::fillStream ( std::ostream& s ) const
{ return s << "ISMCPART" ; }
// ============================================================================
/*  constructor from contained object
 *  @param object object to be compared
 */
// ============================================================================
IsContainedObject::IsContainedObject ( const ContainedObject* object )
  : m_objects ( 1 , object )
{}
// ============================================================================
//  MANDATORY: clone method ('virtual constructor')
// ============================================================================
IsContainedObject* IsContainedObject::clone() const
{ return new IsContainedObject( *this ) ; }
// ============================================================================
bool IsContainedObject::operator() ( const LHCb::MCParticle* p ) const
{
  auto it = std::find( m_objects.begin() , m_objects.end() , p ) ;
  return m_objects.end() != it ;
}
// ============================================================================
std::ostream& IsContainedObject::fillStream ( std::ostream& s ) const
{ return s << "ISMCCO" ; }
// ============================================================================
//  constructor
// ============================================================================
MomentumDistance::MomentumDistance ( const LoKi::LorentzVector& vct )
  : LoKi::AuxFunBase ( std::tie ( vct ) )
  , m_vct ( vct )
{}
// ============================================================================
// constructor from four components
// ============================================================================
MomentumDistance::MomentumDistance
( const double px ,
  const double py ,
  const double pz ,
  const double e  )
  : LoKi::AuxFunBase ( std::tie ( px , py , pz , e ) )
{
  m_vct.SetXYZT ( px , py , pz , e ) ;
}
// ============================================================================
//  MANDATORY: clone function ("virtual constructor")
// ============================================================================
MomentumDistance* MomentumDistance::clone() const
{ return new MomentumDistance(*this) ; }
// ============================================================================
//  MANDATORY: the only one essential method
// ============================================================================
double MomentumDistance::operator() ( const LHCb::MCParticle* p ) const
{
  if ( p )
  { return LoKi::Kinematics::delta2euclidian ( p->momentum() , m_vct ) ; }
  Error ( "Invalid MCParticle*, return (InvalidMomentum)^2") ;
  return
    LoKi::Constants::InvalidMomentum *
    LoKi::Constants::InvalidMomentum ;
}
// ============================================================================
//  "SHORT" representation, @see LoKi::AuxFunBase
// ============================================================================
std::ostream& MomentumDistance::fillStream ( std::ostream& s ) const
{ return s << "MCMOMDIST*"
           << m_vct.Px () << ","
           << m_vct.Py () << ","
           << m_vct.Pz () << ","
           << m_vct.E  () << ")" ; }
// ============================================================================
// constructor from theta & phi
// ============================================================================
TransverseMomentumRel::TransverseMomentumRel ( const double theta ,
                                               const double phi )
  : LoKi::AuxFunBase ( std::tie ( theta , phi ) )
{
  m_vct.SetXYZ
    ( ::sin ( theta ) * ::cos ( phi ) ,
      ::sin ( theta ) * ::sin ( phi ) ,
      ::cos ( theta )                 ) ;
}
// ============================================================================
/*  constructor
 *  @param vct direction vector
 *  @see LoKi::ThreeVector
 */
// ============================================================================
TransverseMomentumRel::TransverseMomentumRel ( const LoKi::ThreeVector& vct )
  : LoKi::AuxFunBase ( std::tie ( vct ) )
  , m_vct ( vct )
{}
// ============================================================================
//  MANDATORY: clone function ("virtual constructor")
// ============================================================================
TransverseMomentumRel* TransverseMomentumRel::clone() const
{ return new TransverseMomentumRel(*this) ; }
// ============================================================================
//  MANDATORY: the only one essential method
// ============================================================================
double TransverseMomentumRel::operator() ( const LHCb::MCParticle* p ) const
{
  if ( p )
  { return LoKi::Kinematics::transverseMomentumDir( p->momentum() , m_vct ) ; }
  Error ( "Invalid MCParticle*, return 'InvalidMomentum'") ;
  return LoKi::Constants::InvalidMomentum ;
}
// ============================================================================
//  "SHORT" representation, @see LoKi::AuxFunBase
// ============================================================================
std::ostream& TransverseMomentumRel::fillStream ( std::ostream& s ) const
{ return s << "MCPTDIR("
           << m_vct.Theta () << ","
           << m_vct.Phi   () << ")" ; }
// ============================================================================
//  MANDATORY: clone method ("virtual constructor")
// ============================================================================
ValidOrigin* ValidOrigin::clone() const { return new ValidOrigin(*this) ; }
// ============================================================================
//  MANDATORY: the only one essential method
// ============================================================================
bool ValidOrigin::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error ( " MCParticle* points to NULL, return 'false' ") ;
    return false ;
  };
  return 0 != p->originVertex() ;
}
// ============================================================================
//  "SHORT" representation, @see LoKi::AuxFunBase
// ============================================================================
std::ostream& ValidOrigin::fillStream ( std::ostream& s ) const
{ return s << "MCOVALID" ; }
// ============================================================================
/*  constructor from vertex funtion
 *  @param vfunc vertex function
 *  @param err1  value to be returned
 *         for invalid argument or
 */
// ============================================================================
MCVertexFunAdapter::MCVertexFunAdapter ( const LoKi::MCTypes::MCVFunc& vfunc ,
                                         const double                  err   )
  : LoKi::AuxFunBase ( std::tie ( vfunc , err ) )
  , m_fun ( vfunc )
  , m_err ( err   )
{}
// ============================================================================
//  MANDATORY : clone method ("virtual constructor")
// ============================================================================
MCVertexFunAdapter* MCVertexFunAdapter::clone() const
{ return new MCVertexFunAdapter(*this) ; }
// ============================================================================
//  MANDATORY: the only one essential method
// ============================================================================
double MCVertexFunAdapter::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error ( " MCParticle* points to NULL, return 'm_err'") ;
    return m_err ;
  }
  //
  return m_fun ( p->originVertex() ) ;
}
// ============================================================================
//  "SHORT" representation, @see LoKi::AuxFunBase
// ============================================================================
std::ostream& MCVertexFunAdapter::fillStream ( std::ostream& s ) const
{ return s << "MCVFASPF(" << m_fun << ")" ; }
// ============================================================================
/*  constructor
 *  @param decay  decay descriptor
 *  @param finder decay finder tool
 *  @see IMCDecayFinder
 */
// ============================================================================
MCDecayPattern::MCDecayPattern ( const std::string& decay  ,
                                 IMCDecayFinder*    finder )
  : m_finder ( finder )
  , m_decay  ( decay  )
{}
// ============================================================================
/*  constructor
 *  @param decay  decay descriptor
 *  @param finder decay finder tool
 *  @see IMCDecayFinder
 */
// ============================================================================
MCDecayPattern::MCDecayPattern
( const std::string&                     decay  ,
  const LoKi::Interface<IMCDecayFinder>& finder )
  : m_finder ( finder )
  , m_decay  ( decay  )
{}
// ============================================================================
//  MANDATORY: clone method ("virtual constructor")
// ============================================================================
MCDecayPattern* MCDecayPattern::clone() const
{ return new MCDecayPattern(*this) ; }
// ============================================================================
//  MANDATORY: the only one essential method
// ============================================================================
bool MCDecayPattern::operator() ( const LHCb::MCParticle* p ) const
{
  const LHCb::MCParticle* mc = p ;
  if ( !p || !mc )
  {
    Error ( " MCParticle* points to NULL, return 'false'") ;
    return false ;
  }
  if ( !m_finder.validPointer() )
  {
    Error ( " IMCDecayFinder* points to NULL, return 'false'") ;
    return false ;
  }
  if ( m_finder->decay() != m_decay )
  {
    StatusCode sc = m_finder->setDecay( m_decay ) ;
    if ( sc.isFailure() )
    {
      Error ( "Error from IMCDecayFinder::setDecay('"
              + m_decay + "') , return 'false' " , sc ) ;
      return false ;
    }
  }
  // prepare helper vector
  std::vector<const LHCb::MCParticle*> vMC ( 1 , mc ) ;
  // use the tool
  return m_finder->hasDecay( vMC ) ;
}
// ============================================================================
//  "SHORT" representation, @see LoKi::AuxFunBase
// ============================================================================
std::ostream& MCDecayPattern::fillStream ( std::ostream& s ) const
{
  s << "MCDECAY(" ;
  if ( m_finder.validPointer() ) { s << "'" << m_finder->name() << "'," ; }
  return s << Gaudi::Utils::toString ( m_decay ) << ")" ;
}
// ============================================================================
MCFilter::MCFilter ( const IMCParticleSelector* selector )
  : m_selector( selector )
{}
// ============================================================================
MCFilter::MCFilter ( const LoKi::Interface<IMCParticleSelector>& selector )
  : m_selector( selector )
{}
// ============================================================================
MCFilter* MCFilter::clone() const { return new MCFilter(*this) ; }
// ============================================================================
bool MCFilter::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error ( " MCParticle* points to NULL, return 'false'") ;
    return false ;
  }
  if ( !m_selector.validPointer() )
  {
    Error ( " IMCParticleSelector* points to NULL, return 'false'") ;
    return false ;
  }
  // use the tool
  return m_selector->accept ( p ) ;
}
// ============================================================================
std::ostream& MCFilter::fillStream ( std::ostream& s ) const
{
  s << "MCFILTER('" ;
  if ( m_selector.validPointer() )
  { s << m_selector->type() << "'/'" << m_selector->name() ; }
  return s << "')" ;
}
// ============================================================================
//  constructor from the phi
// ============================================================================
DeltaPhi::DeltaPhi ( const double phi )
  : LoKi::AuxFunBase ( std::tie ( phi ) )
  , m_phi  ( phi )
{ m_phi = adjust ( m_phi ) ; }
// ============================================================================
//  constructor from the vector
// ============================================================================
DeltaPhi::DeltaPhi ( const LoKi::ThreeVector& v )
  : LoKi::AuxFunBase ( std::tie ( v ) )
  , m_phi  ( v.Phi() )
{ m_phi = adjust ( m_phi ) ; }
// ============================================================================
//  constructor from the vector
// ============================================================================
DeltaPhi::DeltaPhi ( const LoKi::LorentzVector& v )
  : LoKi::AuxFunBase ( std::tie ( v ) )
  , m_phi  ( v.Phi() )
{ m_phi = adjust ( m_phi ) ; }
// ============================================================================
//  constructor from the particle
// ============================================================================
DeltaPhi::DeltaPhi ( const LHCb::MCParticle* p )
{
  if ( !p ) { Exception("Invalid LHCb::MCParticle*") ;}
  m_phi = p->momentum().Phi() ;
  m_phi = adjust ( m_phi ) ;
}
// ============================================================================
//  MANDATORY: the only essential method
// ============================================================================
double DeltaPhi::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error ("LHCb::MCParticle* points to NULL, return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  return  dphi ( p ) ;
}
// ============================================================================
//  OPTIONAL: "SHORT" representation
// ============================================================================
std::ostream& DeltaPhi::fillStream ( std::ostream& s ) const
{ return s << "MCDPHI(" << m_phi << ")" ; }
// ============================================================================
//  constructor from the eta
// ============================================================================
DeltaEta::DeltaEta ( const double eta  )
  : LoKi::AuxFunBase ( std::tie ( eta ) )
  , PseudoRapidity ()
  , m_eta  ( eta )
{}
// ============================================================================
//  constructor from the vector
// ============================================================================
DeltaEta::DeltaEta ( const LoKi::ThreeVector& v )
  : LoKi::AuxFunBase ( std::tie ( v ) )
  , PseudoRapidity ()
  , m_eta  ( v.Eta () )
{}
// ============================================================================
//  constructor from the lorentz vector
// ============================================================================
DeltaEta::DeltaEta ( const LoKi::LorentzVector& v )
  : LoKi::AuxFunBase ( std::tie ( v ) )
  , PseudoRapidity ()
  , m_eta  ( v.Eta () )
{}
// ============================================================================
//  constructor from the particle
// ============================================================================
DeltaEta::DeltaEta ( const LHCb::MCParticle* p )
{
  if ( !p ) { Exception("Invalid LHCb::MCParticle*") ;}
  m_eta = p->momentum().Eta() ;
}
// ============================================================================
//  MANDATORY: the only essential method
// ============================================================================
double DeltaEta::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error ("LHCb::MCParticle* points to NULL, return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  return deta ( p  ) ;
}
// ============================================================================
//  OPTIONAL: "SHORT" representation
// ============================================================================
std::ostream& DeltaEta::fillStream ( std::ostream& s ) const
{ return s << "MCDETA(" << m_eta << ")" ; }
// ============================================================================
//  constructor from the eta and  phi
// ============================================================================
DeltaR2::DeltaR2 ( const double eta ,
                   const double phi )
  : LoKi::AuxFunBase ( std::tie ( eta , phi ) )
  , DeltaPhi ( phi )
  , m_deta ( eta )
{}
// ============================================================================
//  constructor from the vector
// ============================================================================
DeltaR2::DeltaR2 ( const LoKi::ThreeVector& v )
  : LoKi::AuxFunBase ( std::tie ( v ) )
  , DeltaPhi ( v )
  , m_deta ( v )
{}
// ============================================================================
//  constructor from the vector
// ============================================================================
DeltaR2::DeltaR2 ( const LoKi::LorentzVector& v )
  : LoKi::AuxFunBase ( std::tie ( v ) )
  , DeltaPhi ( v )
  , m_deta ( v )
{}
// ============================================================================
//  constructor from the particle
// ============================================================================
DeltaR2::DeltaR2 ( const LHCb::MCParticle* v )
  : DeltaPhi ( v )
  , m_deta ( v )
{}
// ============================================================================
//  MANDATORY: the only essential method
// ============================================================================
double DeltaR2::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error ("LHCb::MCParticle* points to NULL, return 'InvalidDistance'") ;
    return LoKi::Constants::InvalidDistance;
  }
  const double dphi_ = dphi ( p ) ;
  const double deta_ = deta ( p ) ;
  //
  return dphi_*dphi_ + deta_*deta_ ;
}
// ============================================================================
//  OPTIONAL: "SHORT" representation
// ============================================================================
std::ostream& DeltaR2::fillStream ( std::ostream& s ) const
{ return s << "MCDR2(" << eta0() << "," << phi0() << ")" ; }
// ============================================================================
// constructor form the tool
// ============================================================================
MCReconstructible::MCReconstructible ( const IMCReconstructible* tool )
  : m_eval ( tool )
{}
// ============================================================================
// constructor form the tool
// ============================================================================
MCReconstructible::MCReconstructible
( const LoKi::Interface<IMCReconstructible>& tool )
  : m_eval ( tool )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
MCReconstructible* MCReconstructible::clone() const
{ return new  MCReconstructible ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double MCReconstructible::operator() ( const LHCb::MCParticle* p ) const
{
  static const IMCReconstructible::RecCategory s_bad =
    IMCReconstructible::NoClassification ;
  //
  if ( !p )
  {
    Error ( "LHCb::MCParticle* points to NULL, return '" +
            IMCReconstructible::text ( s_bad ) + "'" ) ;
    return s_bad ;
  }
  if ( !m_eval.validPointer() )
  {
    Error ( "IMCRecontructible* points to NULL, return '" +
            IMCReconstructible::text ( s_bad ) + "'" ) ;
    return s_bad ;
  }
  // use the tool
  return m_eval->reconstructible( p ) ; ///< use the tool
}
// ============================================================================
// OPTIONAL: "short representation"
// ============================================================================
std::ostream& MCReconstructible::fillStream( std::ostream& s ) const
{ return s << "MCREC('" << m_eval->name() << "')" ; }
// ============================================================================
// constructor from the tool and category
// ============================================================================
MCReconstructibleAs::MCReconstructibleAs
( const IMCReconstructible*             tool ,
  const IMCReconstructible::RecCategory cat  )
  : m_eval ( tool )
  , m_cat ( cat )
{}
// ============================================================================
// constructor from the tool and category
// ============================================================================
MCReconstructibleAs::MCReconstructibleAs
( const IMCReconstructible::RecCategory cat  ,
  const IMCReconstructible*             tool )
  : m_eval ( tool )
  , m_cat ( cat )
{}
// ============================================================================
// constructor from the holder and category
// ============================================================================
MCReconstructibleAs::MCReconstructibleAs
( const LoKi::Interface<IMCReconstructible>& tool ,
  const IMCReconstructible::RecCategory      cat  )
  : m_eval ( tool )
  , m_cat ( cat )
{}
// ============================================================================
// constructor from the holder and category
// ============================================================================
MCReconstructibleAs::MCReconstructibleAs
( const IMCReconstructible::RecCategory      cat  ,
  const LoKi::Interface<IMCReconstructible>& tool )
  : m_eval ( tool )
  , m_cat ( cat )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
MCReconstructibleAs* MCReconstructibleAs::clone() const
{ return new MCReconstructibleAs(*this); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool MCReconstructibleAs::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error ("LHCb::MCParticle* points to NULL, return 'False'") ;
    return false ;
  }
  //
  if ( !m_eval.validPointer() )
  {
    Error ( "IMCRecontructible* points to NULL, return 'False'" ) ;
    return false ;
  }
  // use the tool!
  return m_cat == m_eval->reconstructible( p ) ; ///< use the tool
}
// ============================================================================
// OPTIONAL: "short representation"
// ============================================================================
std::ostream& MCReconstructibleAs::fillStream( std::ostream& s ) const
{ return s << "MCRECAS[\"" << IMCReconstructible::text ( m_cat ) << "\"]" ; }
// ============================================================================
/*  constructor from the function and child selector
 *  @param fun    the function to be used
 *  @param index  the index of daughter particle
 */
// ============================================================================
ChildFunction::ChildFunction
( const LoKi::MCTypes::MCFunc&   fun   ,
  const LoKi::MCChild::Selector& child ,
  const double                   bad   )
  : LoKi::AuxFunBase ( std::tie ( fun , child , bad ) )
  , m_fun   ( fun   )
  , m_child ( child )
  , m_bad   ( bad   )
{
  Assert ( m_child.valid() , "Child selector is invalid!" ) ;
}
// ============================================================================
/* constructor from the function and child selector
 *  @param fun    the function to be used
 *  @param child  the selector of the proper   child
 *  @param bad    the value to be returned for invalid particle
 */
// ============================================================================
ChildFunction::ChildFunction
( const LoKi::MCTypes::MCFunc& fun   ,
  const LoKi::MCTypes::MCCuts& child ,
  const double                 bad   )
  : LoKi::AuxFunBase ( std::tie ( fun , child , bad ) )
  , m_fun   ( fun   )
  , m_child ( child )
  , m_bad   ( bad   )
{
  Assert ( m_child.valid() , "Child selector is invalid!" ) ;
}
// ============================================================================
/* constructor from the function and child selector
 *  @param fun    the function to be used
 *  @param child  the selector of the proper   child
 *  @param bad    the value to be returned for invalid particle
 */
// ============================================================================
ChildFunction::ChildFunction
( const LoKi::MCTypes::MCFunc& fun   ,
  const std::string&           child ,
  const double                 bad   )
  : LoKi::AuxFunBase ( std::tie ( fun , child , bad ) )
  , m_fun   ( fun   )
  , m_child ( child )
  , m_bad   ( bad   )
{
  Assert ( m_child.valid() , "Child selector is invalid!" ) ;
}
// ============================================================================
/* constructor from the function and child selector
 *  @param fun    the function to be used
 *  @param child  the selector of the proper   child
 *  @param bad    the value to be returned for invalid particle
 */
// ============================================================================
ChildFunction::ChildFunction
( const LoKi::MCTypes::MCFunc&   fun   ,
  const Decays::IMCDecay::iTree& child ,
  const double                   bad   )
  : LoKi::AuxFunBase () // can't be reconstructed in C++
  , m_fun   ( fun   )
  , m_child ( child )
  , m_bad   ( bad   )
{
  Assert ( m_child.valid() , "Child selector is invalid!" ) ;
}
// ============================================================================
/* constructor from the function and child selector
 *  @param fun    the function to be used
 *  @param child  the selector of the proper   child
 *  @param bad    the value to be returned for invalid particle
 */
// ============================================================================
ChildFunction::ChildFunction
( const LoKi::MCTypes::MCFunc& fun   ,
  const Decays::iNode&         child ,
  const double                 bad   )
  : LoKi::AuxFunBase ( std::tie ( fun , child , bad ) )
  , m_fun   ( fun   )
  , m_child ( child )
  , m_bad   ( bad   )
{
  Assert ( m_child.valid() , "Child selector is invalid!" ) ;
}
// ============================================================================
//  MANDATORY: clone method ("virtual constructor")
// ============================================================================
ChildFunction* ChildFunction::clone() const
{ return new ChildFunction ( *this ) ; }
// ============================================================================
//  MANDATORY: the only one essential method
// ============================================================================
double ChildFunction::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error ( "LHCb::MCParticle* points to NULL, return " + LoKi::Print::toString( m_bad ) ) ;
    return m_bad ;
  }
  // extract the daughter
  const LHCb::MCParticle* daughter = m_child.child ( p ) ;
  if ( !daughter ) { Warning ( "Daughter particle points to NULL!" ) ; }
  return m_fun ( daughter ) ;
}
// ============================================================================
// OPTIONAL:  specific printout
// ============================================================================
std::ostream& ChildFunction::fillStream ( std::ostream& s ) const
{
  s << "MCCHILD("
    << m_fun
    << "," << m_child ;
  if ( LoKi::Constants::NegativeInfinity != m_bad ) { s << "," << m_bad ; }
  return s << ")" ;
}
// ============================================================================
/*  constructor from the function and child selector
 *  @param fun    the function to be used
 *  @param child  the child selector
 *  @param bad    the return valeu for invalid particle
 */
// ============================================================================
ChildPredicate::ChildPredicate
( const LoKi::MCTypes::MCCuts&   cut   ,
  const LoKi::MCChild::Selector& child ,
  const bool                     bad   )
  : LoKi::AuxFunBase ( std::tie ( cut , child , bad ) )
  , m_cut   ( cut   )
  , m_child ( child )
  , m_bad   ( bad   )
{
  Assert ( m_child.valid() , "Child selector is invalid!" ) ;
}
// ============================================================================
/*  constructor from the function and child selector
 *  @param fun    the function to be used
 *  @param child  the child selector
 *  @param bad    the return valeu for invalid particle
 */
// ============================================================================
ChildPredicate::ChildPredicate
( const LoKi::MCTypes::MCCuts&   cut   ,
  const LoKi::MCTypes::MCCuts& child   ,
  const bool                     bad   )
  : LoKi::AuxFunBase ( std::tie ( cut , child , bad ) )
  , m_cut   ( cut   )
  , m_child ( child )
  , m_bad   ( bad   )
{
  Assert ( m_child.valid() , "Child selector is invalid!" ) ;
}
// ============================================================================
/*  constructor from the function and child selector
 *  @param fun    the function to be used
 *  @param child  the child selector
 *  @param bad    the return valeu for invalid particle
 */
// ============================================================================
ChildPredicate::ChildPredicate
( const LoKi::MCTypes::MCCuts&   cut   ,
  const std::string&             child ,
  const bool                     bad   )
  : LoKi::AuxFunBase ( std::tie ( cut , child , bad ) )
  , m_cut   ( cut   )
  , m_child ( child )
  , m_bad   ( bad   )
{
  Assert ( m_child.valid() , "Child selector is invalid!" ) ;
}
// ============================================================================
/*  constructor from the function and child selector
 *  @param fun    the function to be used
 *  @param child  the child selector
 *  @param bad    the return valeu for invalid particle
 */
// ============================================================================
ChildPredicate::ChildPredicate
( const LoKi::MCTypes::MCCuts&   cut   ,
  const Decays::IMCDecay::iTree& child ,
  const bool                     bad   )
  : LoKi::AuxFunBase () // can't be reconstructed from C++
  , m_cut   ( cut   )
  , m_child ( child )
  , m_bad   ( bad   )
{
  Assert ( m_child.valid() , "Child selector is invalid!" ) ;
}
// ============================================================================
/*  constructor from the function and child selector
 *  @param fun    the function to be used
 *  @param child  the child selector
 *  @param bad    the return valeu for invalid particle
 */
// ============================================================================
ChildPredicate::ChildPredicate
( const LoKi::MCTypes::MCCuts&   cut   ,
  const Decays::iNode&           child ,
  const bool                     bad   )
  : LoKi::AuxFunBase () // can't be reconstructed from C++
  , m_cut   ( cut   )
  , m_child ( child )
  , m_bad   ( bad   )
{
  Assert ( m_child.valid() , "Child selector is invalid!" ) ;
}



// ============================================================================
//  MANDATORY: clone method ("virtual constructor")
// ============================================================================
ChildPredicate* ChildPredicate::clone() const
{ return new ChildPredicate(*this) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool ChildPredicate::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error ( "LHCb::MCParticle* points to NULL, return " +
            LoKi::Print::toString( m_bad ) ) ;
    return m_bad ;
  }
  // extract the daughter
  const LHCb::MCParticle* daughter = m_child.child ( p ) ;
  if ( !daughter ) { Warning ( "Daughter particle points to NULL" ) ; }
  return m_cut ( daughter ) ;
}
// ============================================================================
// OPTIONAL:  specific printout
// ============================================================================
std::ostream& ChildPredicate::fillStream ( std::ostream& s ) const
{
  s << "MCCHILDCUT("
    << m_cut
    << "," << m_child ;
  if ( false != m_bad ) { s << "," << LoKi::Print::toString ( m_bad )  ; }
  return s << ")" ;
}
// ============================================================================
/*  standard constructor
 *  @param cut cut to be checked
 *  @param decayOnly flag to indicat the search through decay products only
 */
// ============================================================================
InTree::InTree
( const LoKi::MCTypes::MCCuts& cut       ,
  const bool                   decayOnly )
  : LoKi::AuxFunBase ( std::tie ( cut , decayOnly  ) )
  , m_cut       ( cut       )
  , m_decayOnly ( decayOnly )
{}
// ============================================================================
// MANDATORY: clone function ("virtual constructor")
// ============================================================================
InTree* InTree::clone() const { return new InTree(*this); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool InTree::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p ) { Warning ("LHCb::MCParticle* points to NULL" ) ; }
  return LoKi::MCAlgs::found ( p , m_cut , m_decayOnly ) ;
}
// ============================================================================
// OPTIONAL: the specific printout
// ============================================================================
std::ostream& InTree::fillStream ( std::ostream& s ) const
{
  return s << "MCINTREE(" << LoKi::Print::print ( m_cut )
           << ","         << LoKi::Print::print ( m_decayOnly ) << ")" ;
}
// ============================================================================
/*  standard constructor
 *  @param cut cut to be checked
 *  @param decayOnly flag to indicat the search through decay products only
 */
// ============================================================================
NinTree::NinTree
( const LoKi::MCTypes::MCCuts& cut       ,
  const bool                   decayOnly )
  : LoKi::AuxFunBase ( std::tie ( cut , decayOnly  ) )
  , m_cut       ( cut       )
  , m_decayOnly ( decayOnly )
{}
// ============================================================================
// MANDATORY: clone function ("virtual constructor")
// ============================================================================
NinTree* NinTree::clone() const { return new NinTree(*this); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double NinTree::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p ) { Warning ("LHCb::MCParticle* points to NULL" ) ; }
  return LoKi::MCAlgs::count_if ( p , m_cut , m_decayOnly ) ;
}
// ============================================================================
// OPTIONAL: the specific printout
// ============================================================================
std::ostream& NinTree::fillStream ( std::ostream& s ) const
{
  return s << "MCNINTREE(" << LoKi::Print::print ( m_cut )
           << ","          << LoKi::Print::print ( m_decayOnly ) << ")" ;
}
// ============================================================================
/*  standard constructor
 *  @param fun the function to be evaluated
 *  @param cut cut to be applied
 *  @param decayObnly flag to traverse only decay tree
 *  @param res initial value
 */
// ============================================================================
SumTree::SumTree
( const LoKi::MCTypes::MCFunc& fun       ,
  const LoKi::MCTypes::MCCuts& cut       ,
  const bool                   decayOnly ,
  const double                 result    )
  : LoKi::AuxFunBase ( std::tie ( fun , cut , decayOnly  , result ) )
  , m_fun       ( fun       )
  , m_cut       ( cut       )
  , m_decayOnly ( decayOnly )
  , m_res       ( result    )
{}
// ============================================================================
/*  standard constructor
 *  @param fun the function to be evaluated
 *  @param cut cut to be applied
 *  @param decayObnly flag to traverse only decay tree
 *  @param res initial value
 */
// ============================================================================
SumTree::SumTree
( const LoKi::MCTypes::MCCuts& cut       ,
  const LoKi::MCTypes::MCFunc& fun       ,
  const bool                   decayOnly ,
  const double                 result    )
  : LoKi::AuxFunBase ( std::tie ( cut , fun , decayOnly  , result ) )
  , m_fun       ( fun       )
  , m_cut       ( cut       )
  , m_decayOnly ( decayOnly )
  , m_res       ( result    )
{}
// ============================================================================
// MANDATORY: clone function ("virtual constructor")
// ============================================================================
SumTree* SumTree::clone() const { return new SumTree(*this); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double SumTree::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p ) { Warning ("LHCb::MCParticle* points to NULL" ) ; }
  return LoKi::MCAlgs::accumulate
    ( p , m_fun , m_cut , m_res ,  std::plus<>() , m_decayOnly ) ;
}
// ============================================================================
// OPTIONAL: the specific printout
// ============================================================================
std::ostream& SumTree::fillStream ( std::ostream& s ) const
{
  s << "MCSUMTREE("   << m_fun
    << ","            << m_cut
    << ","            << LoKi::Print::print ( m_decayOnly ) ;
  if ( 0 != m_res )
  { s << ","          << LoKi::Print::print ( m_res  ) ; }
  return s << ")" ;
}
// ============================================================================
/*  standard constructor
 *  @param fun the function to be evaluated
 *  @param cut cut to be applied
 *  @param decayObnly flag to traverse only decay tree
 *  @param res initial value
 */
// ============================================================================
MultTree::MultTree
( const LoKi::MCTypes::MCFunc& fun       ,
  const LoKi::MCTypes::MCCuts& cut       ,
  const bool                   decayOnly ,
  const double                 result    )
  : LoKi::AuxFunBase ( std::tie ( fun , cut , decayOnly  , result ) )
  , m_fun       ( fun       )
  , m_cut       ( cut       )
  , m_decayOnly ( decayOnly )
  , m_res       ( result    )
{}
// ============================================================================
/*  standard constructor
 *  @param fun the function to be evaluated
 *  @param cut cut to be applied
 *  @param decayObnly flag to traverse only decay tree
 *  @param res initial value
 */
// ============================================================================
MultTree::MultTree
( const LoKi::MCTypes::MCCuts& cut       ,
  const LoKi::MCTypes::MCFunc& fun       ,
  const bool                   decayOnly ,
  const double                 result    )
  : LoKi::AuxFunBase ( std::tie ( cut , fun , decayOnly  , result ) )
  , m_fun       ( fun       )
  , m_cut       ( cut       )
  , m_decayOnly ( decayOnly )
  , m_res       ( result    )
{}
// ============================================================================
// MANDATORY: clone function ("virtual constructor")
// ============================================================================
MultTree* MultTree::clone() const { return new MultTree(*this); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double MultTree::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p ) { Warning ("LHCb::MCParticle* points to NULL" ) ; }
  return LoKi::MCAlgs::accumulate
    ( p , m_fun , m_cut , m_res ,  std::multiplies<>() , m_decayOnly ) ;
}
// ============================================================================
// OPTIONAL: the specific printout
// ============================================================================
std::ostream& MultTree::fillStream ( std::ostream& s ) const
{
  s << "MCMULTTREE("   << m_fun
    << ","             << m_cut
    << ","             << LoKi::Print::print ( m_decayOnly ) ;
  if ( 1 != m_res )
  { s << ","           << LoKi::Print::print ( m_res  ) ; }
  return s << ")" ;
}
// ============================================================================
/*  standard constructor
 *  @param fun the function to be evaluated
 *  @param cut cut to be applied
 *  @param decayObnly flag to traverse only decay tree
 *  @param res initial value
 */
// ============================================================================
MinTree::MinTree
( const LoKi::MCTypes::MCFunc& fun       ,
  const LoKi::MCTypes::MCCuts& cut       ,
  const bool                   decayOnly ,
  const double                 result    )
  : LoKi::AuxFunBase ( std::tie ( fun , cut , decayOnly  , result ) )
  , m_fun       ( fun       )
  , m_cut       ( cut       )
  , m_decayOnly ( decayOnly )
  , m_res       ( result    )
{}
// ============================================================================
/*  standard constructor
 *  @param fun the function to be evaluated
 *  @param cut cut to be applied
 *  @param decayObnly flag to traverse only decay tree
 *  @param res initial value
 */
// ============================================================================
MinTree::MinTree
( const LoKi::MCTypes::MCCuts& cut       ,
  const LoKi::MCTypes::MCFunc& fun       ,
  const bool                   decayOnly ,
  const double                 result    )
  : LoKi::AuxFunBase ( std::tie ( cut , fun , decayOnly  , result ) )
  , m_fun       ( fun       )
  , m_cut       ( cut       )
  , m_decayOnly ( decayOnly )
  , m_res       ( result    )
{}
// ============================================================================
// MANDATORY: clone function ("virtual constructor")
// ============================================================================
MinTree* MinTree::clone() const { return new MinTree(*this); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double MinTree::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p ) { Warning ("LHCb::MCParticle* points to NULL" ) ; }
  return LoKi::MCAlgs::accumulate
    ( p , m_fun , m_cut , m_res ,  std::multiplies<>() , m_decayOnly ) ;
}
// ============================================================================
// OPTIONAL: the specific printout
// ============================================================================
std::ostream& MinTree::fillStream ( std::ostream& s ) const
{
  s << "MCMINTREE("   << m_fun
    << ","            << m_cut
    << ","            << LoKi::Print::print ( m_decayOnly ) ;
  if ( LoKi::Constants::PositiveInfinity != m_res )
  { s << ","          << LoKi::Print::print ( m_res  ) ; }
  return s << ")" ;
}
// ============================================================================
/*  standard constructor
 *  @param fun the function to be evaluated
 *  @param cut cut to be applied
 *  @param decayObnly flag to traverse only decay tree
 *  @param res initial value
 */
// ============================================================================
MaxTree::MaxTree
( const LoKi::MCTypes::MCFunc& fun       ,
  const LoKi::MCTypes::MCCuts& cut       ,
  const bool                   decayOnly ,
  const double                 result    )
  : LoKi::AuxFunBase ( std::tie ( fun , cut , decayOnly  , result ) )
  , m_fun       ( fun       )
  , m_cut       ( cut       )
  , m_decayOnly ( decayOnly )
  , m_res       ( result    )
{}
// ============================================================================
/*  standard constructor
 *  @param fun the function to be evaluated
 *  @param cut cut to be applied
 *  @param decayObnly flag to traverse only decay tree
 *  @param res initial value
 */
// ============================================================================
MaxTree::MaxTree
( const LoKi::MCTypes::MCCuts& cut       ,
  const LoKi::MCTypes::MCFunc& fun       ,
  const bool                   decayOnly ,
  const double                 result    )
  : LoKi::AuxFunBase ( std::tie ( cut , fun , decayOnly  , result ) )
  , m_fun       ( fun       )
  , m_cut       ( cut       )
  , m_decayOnly ( decayOnly )
  , m_res       ( result    )
{}
// ============================================================================
// MANDATORY: clone function ("virtual constructor")
// ============================================================================
MaxTree* MaxTree::clone() const { return new MaxTree(*this); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double MaxTree::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p ) { Warning ("LHCb::MCParticle* points to NULL" ) ; }
  return LoKi::MCAlgs::accumulate
    ( p , m_fun , m_cut , m_res ,  std::multiplies<>() , m_decayOnly ) ;
}
// ============================================================================
// OPTIONAL: the specific printout
// ============================================================================
std::ostream& MaxTree::fillStream ( std::ostream& s ) const
{
  s << "MCMAXTREE("   << m_fun
    << ","            << m_cut
    << ","            << LoKi::Print::print ( m_decayOnly ) ;
  if ( LoKi::Constants::NegativeInfinity != m_res )
  { s << ","          << LoKi::Print::print ( m_res  ) ; }
  return s << ")" ;
}

// ============================================================================
// constructor from the predicate
// ============================================================================
NinAncestors::NinAncestors
( const LoKi::MCTypes::MCCuts& cuts      ,
  const bool                   decayOnly )
  : LoKi::AuxFunBase ( std::tie ( cuts , decayOnly ) )
  , m_cut       ( cuts      )
  , m_decayOnly ( decayOnly )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
NinAncestors* NinAncestors::clone() const { return new NinAncestors ( *this ) ; }
// ============================================================================
// MANDATORY: the only one important method
// ============================================================================
double NinAncestors::operator() ( const LHCb::MCParticle* p ) const
{
  return nInAncestors ( p ) ;
}
// ============================================================================
// the actual evaluator
// ============================================================================
unsigned int NinAncestors::nInAncestors ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error ( "LHCb::MCParticle* point to NULL, return 0") ;
    return 0 ;
  }
  unsigned int found = 0 ;
  const LHCb::MCVertex*   ov     = p->originVertex() ;
  const LHCb::MCParticle* mother = nullptr == ov ? nullptr : ov->mother() ;
  while ( 0 != mother )
  {
    if ( m_decayOnly && !ov->isDecay() ) { return found ; }
    //
    if ( m_cut ( mother ) ) { return ++found  ; }   // COUNT
    //
    ov     = mother->originVertex() ;
    mother = nullptr == ov ? nullptr : ov->mother() ;
  }
  return found ;
}
// ============================================================================
// OPTIONAL: "SHORT" representation
// ============================================================================
std::ostream& NinAncestors::fillStream( std::ostream& s ) const
{ return s << " MCNINANCESTORS( " << m_cut << ( m_decayOnly ? ",True) " : ",False) " ) ; }
// ============================================================================
// constructor from the predicate
// ============================================================================
InAncestors::InAncestors
( const LoKi::MCTypes::MCCuts& cuts      ,
  const bool                   decayOnly )
  : LoKi::AuxFunBase ( std::tie ( cuts , decayOnly ) )
  , LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate ()
  , m_cut       ( cuts      )
  , m_decayOnly ( decayOnly )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
InAncestors* InAncestors::clone() const { return new InAncestors ( *this ) ; }
// ============================================================================
// MANDATORY: the only one important method
// ============================================================================
bool InAncestors::operator() ( const LHCb::MCParticle* p ) const
{
  return inAncestors ( p ) ;
}
// ============================================================================
// the actual evaluator
// ============================================================================
bool InAncestors::inAncestors ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error ( "LHCb::MCParticle* point to NULL, return false") ;
    return false ;
  }
  const LHCb::MCVertex*   ov     = p->originVertex() ;
  const LHCb::MCParticle* mother = ( nullptr == ov ) ? nullptr : ov->mother() ;
  //
  while ( nullptr != mother )
  {
    if ( m_decayOnly && !ov->isDecay() ) { return false ; }   // RETURN
    if ( m_cut ( mother )              ) { return true  ; }   // RETURN
    //
    ov     = mother->originVertex() ;
    mother = nullptr == ov ? nullptr : ov->mother() ;
  }
  return false ;
}
// ============================================================================
// OPTIONAL: "SHORT" representation
// ============================================================================
std::ostream& InAncestors::fillStream( std::ostream& s ) const
{ return s << " MCINANCESTORS( " << m_cut << ( m_decayOnly ? ",True) " : ",False) ") ; }
// ============================================================================

// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
FromDecays* FromDecays::clone () const { return new FromDecays ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool FromDecays::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error ( "LHCb::MCParticle* points to NULL, return false") ;
    return false ;
  }
  const LHCb::MCVertex* vertex = p->originVertex() ;
  if ( !vertex ) { return false ; }               // RETURN
  //
  while ( 0 != vertex )
  {
    switch ( vertex->type() )
    {
    case LHCb::MCVertex::MCVertexType::ppCollision        : break ;
    case LHCb::MCVertex::MCVertexType::DecayVertex        : break ;
    case LHCb::MCVertex::MCVertexType::OscillatedAndDecay : break ;
    default:                                  return false ; // RETURN
    }
    const LHCb::MCParticle* mother = vertex->mother() ;
    if ( 0 != mother ) { vertex = mother->originVertex() ; }
    else               { vertex = 0 ; }
    //
  }
  //
  return true ;
}
// ============================================================================
// OPTIONAL: "SHORT" representation
// ============================================================================
std::ostream& FromDecays::fillStream( std::ostream& s ) const
{ return s << " MCFROMDECAYS " ; }
// ============================================================================


// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
FromInteractions* FromInteractions::clone () const
{ return new FromInteractions ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool FromInteractions::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error ( "LHCb::MCParticle* points to NULL, return false") ;
    return false ;
  }
  const LHCb::MCVertex* vertex = p->originVertex() ;
  if ( !vertex ) { return false ; }               // RETURN
  //
  while ( 0 != vertex )
  {
    switch ( vertex->type() )
    {
    case LHCb::MCVertex::MCVertexType::ppCollision        : break ;
    case LHCb::MCVertex::MCVertexType::DecayVertex        : break ;
    case LHCb::MCVertex::MCVertexType::OscillatedAndDecay : break ;
    default:                                  return true ; // RETURN
    }
    const LHCb::MCParticle* mother = vertex->mother() ;
    if ( 0 != mother ) { vertex = mother->originVertex() ; }
    else               { vertex = 0 ; }
    //
  }
  //
  return false ;
}
// ============================================================================
// OPTIONAL: "SHORT" representation
// ============================================================================
std::ostream& FromInteractions::fillStream ( std::ostream& s ) const
{ return s << " MCFROMXS" ; }
// ============================================================================


// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
Signal* Signal::clone () const { return new Signal ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool Signal::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error ( "LHCb::MCParticle* points to NULL, return false") ;
    return false ;
  }
  return signal ( p ) ;
}
// ============================================================================
// OPTIONAL: "SHORT" representation
// ============================================================================
std::ostream& Signal::fillStream ( std::ostream& s ) const
{ return s << " MCSIGNAL" ; }
// ============================================================================

// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
FromSignal* FromSignal::clone () const { return new FromSignal ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool FromSignal::operator() ( const LHCb::MCParticle* p ) const
{
  if ( !p )
  {
    Error ( "LHCb::MCParticle* points to NULL, return false") ;
    return false ;
  }
  const LHCb::MCParticle* mother = p ;
  while ( nullptr != mother )
  {
    if ( signal ( mother ) ) { return true ; }
    mother = mother->mother() ;
  }
  return false ;
}
// ============================================================================
// OPTIONAL: "SHORT" representation
// ============================================================================
std::ostream& FromSignal::fillStream ( std::ostream& s ) const
{ return s << " MCFROMSIGNAL" ; }
// ============================================================================


// ============================================================================
// get unique string for HepMC::Particle
// ============================================================================
std::string hex_id ( const LHCb::MCParticle* particle )
{
  if ( !particle ) { return "NULL" ; }
  boost::format fmt ( "%p" ) ;
  const void * p = particle ;
  fmt % p ;
  return fmt.str() ;
}


// ============================================================================
namespace
{
  // hashing function
  const std::hash<const void*> s_hash {} ;
}
// ============================================================================
// get hash function for HepMC::GenParticle
// ============================================================================
std::size_t hash ( const LHCb::MCParticle* particle )
{ return !particle ? 0 : s_hash ( particle ) ; }
// ============================================================================
// get hash function for HepMC::GenVertex
// ============================================================================
std::size_t hash ( const LHCb::MCVertex*   vertex )
{ return !vertex ? 0 : s_hash( vertex ) ; }


} }
// ============================================================================
// The END
// ============================================================================
