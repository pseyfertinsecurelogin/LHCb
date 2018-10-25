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
// =============================================================================
// Include files
// =============================================================================
// LHCbMath
// =============================================================================
#include "LHCbMath/LHCbMath.h"
// =============================================================================
// Local
// =============================================================================
#include "LoKi/Constants.h"
#include "LoKi/Kinematics.h"
#include "LoKi/GenParticles4.h"
#include "LoKi/GenChildSelector.h"
// =============================================================================
// GaudiKernel
// =============================================================================
#include "GaudiKernel/ToStream.h"
// =============================================================================
/** @file
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2012-01-28
 *
 */
// ============================================================================
namespace
{
  // ==========================================================================
  /** @var s_equal
   *  comparison criteria for doubles
   */
  const LHCb::Math::Equal_To<double> s_equal ;
  // ==========================================================================
  // adjust delta phi into the range of [-180:180]degrees
  // ============================================================================
  double  adjust ( double angle )
  {
    static constexpr double s_180 = 180 * Gaudi::Units::degree ;
    static constexpr double s_360 = 360 * Gaudi::Units::degree ;
    //
    while ( angle >       s_180 ) { angle -= s_360 ; }
    while ( angle <  -1 * s_180 ) { angle += s_360 ; }
    //
    return angle ;
  }
}

namespace LoKi { namespace GenParticles {
// ============================================================================
// constructor from one selector
// ============================================================================
FourMomentum::FourMomentum ( const LoKi::GenChild::Selector& c1 )
  : LoKi::GenTypes::GFunc  ()
  , m_children ( 1 , c1 )
{}
// ============================================================================
// constructor from two selectors
// ============================================================================
FourMomentum::FourMomentum ( const LoKi::GenChild::Selector& c1 ,
                             const LoKi::GenChild::Selector& c2 )
: m_children { c1, c2 }
{ }
// ============================================================================
// constructor from three selectors
// ============================================================================
FourMomentum::FourMomentum ( const LoKi::GenChild::Selector& c1 ,
                             const LoKi::GenChild::Selector& c2 ,
                             const LoKi::GenChild::Selector& c3 )
: m_children{ c1, c2, c3 }
{ }
// ============================================================================
// constructor from four selectors
// ============================================================================
FourMomentum::FourMomentum ( const LoKi::GenChild::Selector& c1 ,
                             const LoKi::GenChild::Selector& c2 ,
                             const LoKi::GenChild::Selector& c3 ,
                             const LoKi::GenChild::Selector& c4 )
: m_children{ c1, c2, c3, c4 }
{ }
// ============================================================================
// constructor from list of  selectors
// ============================================================================
FourMomentum::FourMomentum ( const LoKi::GenChild::Selector::Vector& cs )
  : m_children ( cs )
{}
// ============================================================================
// constructor from one selector
// ============================================================================
FourMomentum::FourMomentum ( const LoKi::GenTypes::GCuts& c1 )
  : m_children ( 1 , c1 )
{}
// ============================================================================
// constructor from two selectors
// ============================================================================
FourMomentum::FourMomentum ( const LoKi::GenTypes::GCuts& c1 ,
                             const LoKi::GenTypes::GCuts& c2 )
: m_children{ c1, c2 }
{ }
// ============================================================================
// constructor from three selectors
// ============================================================================
FourMomentum::FourMomentum ( const LoKi::GenTypes::GCuts& c1 ,
                             const LoKi::GenTypes::GCuts& c2 ,
                             const LoKi::GenTypes::GCuts& c3 )
: m_children{ c1, c2, c3 }
{ }
// ============================================================================
// constructor from four selectors
// ============================================================================
FourMomentum::FourMomentum ( const LoKi::GenTypes::GCuts& c1 ,
                             const LoKi::GenTypes::GCuts& c2 ,
                             const LoKi::GenTypes::GCuts& c3 ,
                             const LoKi::GenTypes::GCuts& c4 )
: m_children{ c1, c2, c3, c4 }
{ }
// ============================================================================
// base class printput
// ============================================================================
std::ostream& FourMomentum::print_ ( std::ostream&      s  ,
                                     const std::string& n1 ,
                                     const std::string& n2 ) const
{
  //
  if ( m_children.empty() ) { return s << n2 ; } // RETURN
  //
  if ( 4 < m_children.size() )
  {
    s << n1 << "([ "  ;
    //
    for (  auto ii =m_children.cbegin() ; m_children.cend() != ii ; ++ii )
    {
      if ( m_children.cbegin() != ii ) { s << "," ; }
      s << (*ii) ;
    }
    //
    return s <<  " ]) " ;                                 // REUTRN
  }
  //
  s << n1 << "(" ;
  for (  auto ii = m_children.cbegin() ; m_children.cend() != ii ; ++ii )
  {
    if ( m_children.cbegin() != ii ) { s << "," ; }
    s << (*ii) ;
  }
  //
  return s << ")" ;
}
// ============================================================================
// evaluate the full four momentum of combination
// ============================================================================
LoKi::LorentzVector FourMomentum::momentum ( const HepMC::GenParticle* p ) const
{
  if ( UNLIKELY(!p) )
  {
    Error ("HepMC::GenParticle* points to NULL, return empty 4-momentum") ;
    return LoKi::LorentzVector () ;
  }
  //
  if ( m_children.empty() ) { return LoKi::LorentzVector ( p-> momentum() ) ; }
  //
  LoKi::LorentzVector result = LoKi::LorentzVector() ;
  //
  for ( auto ii = m_children.begin() ; m_children.end() != ii ; ++ii )
  {
    const HepMC::GenParticle* c = ii->child ( p ) ;
    if ( !c )
    {
      Error ( "daughter HepMC::GenParticle* points to NULL, skip it" ) ;
      continue ;
    }
    result += LoKi::LorentzVector ( c->momentum () ) ;
  }
  //
  return result ;
}
// ============================================================================
// get the rapidity
// ============================================================================
double FourMomentum::y ( const LoKi::LorentzVector& lv ) const
{
  const double e  = lv . E  () ;
  const double pz = lv . Pz () ;
  //
  return 0.5 * std::log( ( e + pz ) / ( e - pz ) ) ;
}
// ============================================================================
// get the rapidity
// ============================================================================
double FourMomentum::y0 ( const LoKi::LorentzVector& lv ) const
{
  const double e  = lv . E  () ;
  const double p  = lv . P  () ;
  //
  return 0.5 * std::log( ( e + p ) / ( e - p ) ) ;
}
// ============================================================================
// get the phi
// ============================================================================
double FourMomentum::phi ( const LoKi::LorentzVector& lv ) const
{ return adjust ( lv.phi () ) ; }
// ============================================================================
// get the eta
// ============================================================================
double FourMomentum::eta ( const LoKi::LorentzVector& lv ) const
{ return lv . Eta() ; }
// ==========================================================================
// get the beta
// ============================================================================
double FourMomentum::beta ( const LoKi::LorentzVector& lv ) const
{ return lv . Beta  () ; }
// ==========================================================================
// get the gamma
// ============================================================================
double FourMomentum::gamma ( const LoKi::LorentzVector& lv ) const
{ return lv . Gamma () ; }
// ==========================================================================
// get the beta*gamma
// ============================================================================
double FourMomentum::bgamma( const LoKi::LorentzVector& lv ) const
{
  //
  const double m = lv.M () ;
  //
  if ( s_equal ( m , 0 ) ) { return LoKi::Constants::PositiveInfinity ; }
  //
  return lv.P () / m ;
}
// ==========================================================================


// ============================================================================
// MANDATORY: clone method ("virtual destructor")
// ============================================================================
Mass* Mass::clone() const { return new Mass(*this) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double Mass::operator()( const HepMC::GenParticle* p ) const
{
  //
  if ( UNLIKELY(!p) )
  {
    Error ("HepMC::GenParticle* points to NULL, return Invalid Mass") ;
    return LoKi::Constants::InvalidMass ;
  }
  //
  return momentum (  p ) . M () ;
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& Mass::fillStream ( std::ostream& s ) const
{ return print_ ( s , "GMASS" , "GM" ) ; }
// ============================================================================



// ============================================================================
// MANDATORY: clone method ("virtual destructor")
// ============================================================================
Energy* Energy::clone() const { return new Energy(*this) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double Energy::operator() ( const HepMC::GenParticle* p ) const
{
  //
  if ( UNLIKELY(!p) )
  {
    Error ("HepMC::GenParticle* points to NULL, return Invalid Energy") ;
    return LoKi::Constants::InvalidEnergy ;
  }
  //
  return momentum (  p ) . E () ;
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& Energy::fillStream ( std::ostream& s ) const
{ return print_ ( s , "GENERGY" , "GE" ) ; }



// ============================================================================
// MANDATORY: clone method ("virtual destructor")
// ============================================================================
Momentum* Momentum::clone() const { return new Momentum(*this) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double Momentum::operator() ( const HepMC::GenParticle* p ) const
{
  //
  if ( UNLIKELY(!p) )
  {
    Error ("HepMC::GenParticle* points to NULL, return Invalid Momentum") ;
    return LoKi::Constants::InvalidMomentum ;
  }
  //
  return momentum (  p ) . P () ;
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& Momentum::fillStream ( std::ostream& s ) const
{ return print_ ( s , "GMOMENTUM" , "GP" ) ; }


// ============================================================================
// MANDATORY: clone method ("virtual destructor")
// ============================================================================
MomentumX* MomentumX::clone() const { return new MomentumX(*this) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double MomentumX::operator() ( const HepMC::GenParticle* p ) const
{
  //
  if ( UNLIKELY(!p) )
  {
    Error ("HepMC::GenParticle* points to NULL, return Invalid Momentum") ;
    return LoKi::Constants::InvalidMomentum ;
  }
  //
  return momentum (  p ) . Px () ;
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& MomentumX::fillStream ( std::ostream& s ) const
{ return print_ ( s , "GMOMX" , "GPX" ) ; }

// ============================================================================
// MANDATORY: clone method ("virtual destructor")
// ============================================================================
MomentumY* MomentumY::clone() const { return new MomentumY(*this) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double MomentumY::operator() ( const HepMC::GenParticle* p ) const
{
  //
  if ( UNLIKELY(!p) )
  {
    Error ("HepMC::GenParticle* points to NULL, return Invalid Momentum") ;
    return LoKi::Constants::InvalidMomentum ;
  }
  //
  return momentum (  p ) . Py () ;
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& MomentumY::fillStream ( std::ostream& s ) const
{ return print_ ( s , "GMOMY" , "GPY" ) ; }


// ============================================================================
// MANDATORY: clone method ("virtual destructor")
// ============================================================================
MomentumZ* MomentumZ::clone() const { return new MomentumZ(*this) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double MomentumZ::operator() ( const HepMC::GenParticle* p ) const
{
  //
  if ( UNLIKELY(!p) )
  {
    Error ("HepMC::GenParticle* points to NULL, return Invalid Momentum") ;
    return LoKi::Constants::InvalidMomentum ;
  }
  //
  return momentum (  p ) . Pz () ;
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& MomentumZ::fillStream ( std::ostream& s ) const
{ return print_ ( s , "GMOMZ" , "GPZ" ) ; }



// ============================================================================
// MANDATORY: clone method ("virtual destructor")
// ============================================================================
TransverseMomentum* TransverseMomentum::clone() const
{ return new TransverseMomentum(*this) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double TransverseMomentum::operator() ( const HepMC::GenParticle* p ) const
{
  //
  if ( UNLIKELY(!p) )
  {
    Error ("HepMC::GenParticle* points to NULL, return Invalid Momentum") ;
    return LoKi::Constants::InvalidMomentum ;
  }
  //
  return momentum (  p ) . Pt () ;
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& TransverseMomentum::fillStream ( std::ostream& s ) const
{ return print_ ( s , "GMOMT" , "GPT" ) ; }



// ============================================================================
// MANDATORY: clone method ("virtual destructor")
// ============================================================================
PseudoRapidity* PseudoRapidity::clone() const { return new PseudoRapidity(*this) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double PseudoRapidity::operator() ( const HepMC::GenParticle* p ) const
{
  //
  if ( UNLIKELY(!p) )
  {
    Error ("HepMC::GenParticle* points to NULL, return NegativeInfinity") ;
    return LoKi::Constants::NegativeInfinity;
  }
  //
  return eta ( momentum (  p ) ) ;
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& PseudoRapidity::fillStream ( std::ostream& s ) const
{ return print_ ( s , "GPSEUDORAPIDITY" , "GETA" ) ; }


// ============================================================================
// MANDATORY: clone method ("virtual destructor")
// ============================================================================
Rapidity* Rapidity::clone() const { return new Rapidity(*this) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double Rapidity::operator() ( const HepMC::GenParticle* p ) const
{
  //
  if ( UNLIKELY(!p) )
  {
    Error ("HepMC::GenParticle* points to NULL, return NegativeInfinity") ;
    return LoKi::Constants::NegativeInfinity;
  }
  //
  return y ( momentum (  p ) ) ;
  //
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& Rapidity::fillStream ( std::ostream& s ) const
{ return print_ ( s , "GRAPIDITY" , "GY" ) ; }


// ============================================================================
// MANDATORY: clone method ("virtual destructor")
// ============================================================================
Rapidity0* Rapidity0::clone() const { return new Rapidity0(*this) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double Rapidity0::operator() ( const HepMC::GenParticle* p ) const
{
  //
  if ( UNLIKELY(!p) )
  {
    Error ("HepMC::GenParticle* points to NULL, return NegativeInfinity") ;
    return LoKi::Constants::NegativeInfinity;
  }
  //
  return y0 ( momentum (  p ) ) ;
  //
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& Rapidity0::fillStream ( std::ostream& s ) const
{ return print_ ( s , "GRAPIDITY0" , "GY0" ) ; }


// ============================================================================
// MANDATORY: clone method ("virtual destructor")
// ============================================================================
Phi* Phi::clone() const { return new Phi(*this) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double Phi::operator()( const HepMC::GenParticle* p ) const
{
  //
  if ( UNLIKELY(!p) )
  {
    Error ( "HepMC::GenParticle* points to NULL, return -100 ") ;
    return -100 ;
  }
  //
  return phi ( momentum (  p ) ) ;
  //
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& Phi::fillStream ( std::ostream& s ) const
{ return print_ ( s , "GASYMUTH" , "GPHI" ) ; }
// ============================================================================

// ============================================================================
// MANDATORY: clone method ("virtual destructor")
// ============================================================================
Theta* Theta::clone() const { return new Theta(*this) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double Theta::operator()( const HepMC::GenParticle* p ) const
{
  //
  if ( 0 == p )
  {
    Error ( "HepMC::GenParticle* points to NULL, return -100 ") ;
    return -100 ;
  }
  //
  return momentum (  p ) . Theta () ;
  //
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& Theta::fillStream ( std::ostream& s ) const
{ return print_ ( s , "GPOLAR" , "GTHETA" ) ; }



// ============================================================================
// MANDATORY: clone method ("virtual destructor")
// ============================================================================
Beta* Beta::clone() const { return new Beta ( *this ) ; }
// ============================================================================
// get beta-factor
// ============================================================================
double Beta::beta( const HepMC::GenParticle* p ) const
{
  if ( 0 == p )
  {
    Error ( "HepMC::GenParticle* points to NULL, return -1 ") ;
    return -1 ;
  }
  //
  return LoKi::LorentzVector ( p->momentum() ).Beta() ;
  //
}
// ============================================================================
// get gamma-factor
// ============================================================================
double Beta::gamma( const HepMC::GenParticle* p ) const
{
  if ( 0 == p )
  {
    Error ( "HepMC::GenParticle* points to NULL, return -1 ") ;
    return -1 ;
  }
  //
  const LoKi::LorentzVector lv ( p->momentum() ) ;
  //
  const auto m = lv.M () ;
  const auto E = lv.E () ;
  //
  if ( s_equal ( E , m ) ) { return 1 ; }
  //
  if ( UNLIKELY(s_equal ( m , 0 )) )
  {
    //
    //
    Error ( "Light-like particle, return +infinity") ;
    return LoKi::Constants::PositiveInfinity ;
  }
  //
  return E / m ;
  //
}
// ============================================================================
// get gamma-factor
// ============================================================================
double Beta::betagamma( const HepMC::GenParticle* p ) const
{
  if ( UNLIKELY(!p) )
  {
    Error ( "HepMC::GenParticle* points to NULL, return -1 ") ;
    return -1 ;
  }
  //
  const LoKi::LorentzVector lv ( p->momentum() ) ;
  const double P = lv.P () ;
  if ( s_equal ( P , 0 ) ) { return 0 ; }
  //
  const double m = lv.M () ;
  if ( s_equal ( m , 0 ) )
  {
    //
    Error ( "Light-like particle, return +infinity") ;
    return LoKi::Constants::PositiveInfinity ;
  }
  //
  return P / m ;
  //
}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double Beta::operator() ( const HepMC::GenParticle* p ) const
{ return beta ( p ) ; }
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& Beta::fillStream ( std::ostream& s ) const
{ return s << "GBETA"; }



// ============================================================================
// MANDATORY: clone method ("virtual destructor")
// ============================================================================
Gamma* Gamma::clone() const { return new Gamma ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double Gamma::operator() ( const HepMC::GenParticle* p ) const
{ return gamma ( p ) ; }
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& Gamma::fillStream ( std::ostream& s ) const
{ return s << "GGAMMA"; }


// ============================================================================
// MANDATORY: clone method ("virtual destructor")
// ============================================================================
BetaGamma* BetaGamma::clone() const { return new BetaGamma ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double BetaGamma::operator() ( const HepMC::GenParticle* p ) const
{ return betagamma ( p ) ; }
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& BetaGamma::fillStream ( std::ostream& s ) const
{ return s << "GBETAGAMMA"; }
// ============================================================================



// ============================================================================
//  constructor from the angle
// ============================================================================
DeltaPhi::DeltaPhi ( const double phi )
  : m_phi  ( adjust( phi ) )
{ }
// ============================================================================
//  constructor from the vector
// ============================================================================
DeltaPhi::DeltaPhi ( const LoKi::ThreeVector& v )
  : m_phi  ( adjust( v.Phi() ) )
{ }
// ============================================================================
//  constructor from the vector
// ============================================================================
DeltaPhi::DeltaPhi ( const LoKi::LorentzVector& v )
  : m_phi  ( adjust( v.Phi() ) )
{ }
// ============================================================================
//  constructor from the particle
// ============================================================================
DeltaPhi::DeltaPhi ( const HepMC::GenParticle* p )
{
  if ( !p ) { Exception("Invalid HepMC::GenParticle*") ;}
  m_phi = adjust( p->momentum().phi() );
}
// ============================================================================
//  MANDATORY: clone method ("virtual constructor")
// ============================================================================
DeltaPhi* DeltaPhi::clone() const { return new DeltaPhi(*this) ; }
// ============================================================================
//  MANDATORY: the only essential method
// ============================================================================
double DeltaPhi::operator() ( const HepMC::GenParticle* p ) const
{
  if ( 0 == p )
  {
    Error ( "HepMC::GenParticle* points to NULL, return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  return dphi ( p )  ;
}
// ============================================================================
double DeltaPhi::dphi ( const HepMC::GenParticle* p ) const
{ return adjust ( phi ( momentum ( p )  ) - phi0 () ) ; }
// ============================================================================
//  OPTIONAL: "SHORT" representation
// ============================================================================
std::ostream& DeltaPhi::fillStream ( std::ostream& s ) const
{ return s << "GDPHI(" << phi0() << ")" ; }
// ============================================================================
//  constructor from the angle
// ============================================================================
DeltaEta::DeltaEta ( const double eta )
  : m_eta  ( eta )
{}
// ============================================================================
//  constructor from the vector
// ============================================================================
DeltaEta::DeltaEta ( const LoKi::ThreeVector& v )
  : m_eta  ( v.Eta() )
{}
// ============================================================================
//  constructor from the vector
// ============================================================================
DeltaEta::DeltaEta ( const LoKi::LorentzVector& v )
  : m_eta  ( v.Eta() )
{}
// ============================================================================
//  constructor from the particle
// ============================================================================
DeltaEta::DeltaEta ( const HepMC::GenParticle* p )
{
  if ( !p ) { Exception("Invalid HepMC::GenParticle*") ;}
  m_eta = p->momentum().pseudoRapidity() ;
}
// ============================================================================
//  MANDATORY: clone method ("virtual constructor")
// ============================================================================
DeltaEta* DeltaEta::clone() const { return new DeltaEta(*this) ; }
// ============================================================================
//  MANDATORY: the only essential method
// ============================================================================
double DeltaEta::operator() ( const HepMC::GenParticle* p ) const
{
  if ( UNLIKELY(!p) )
  {
    Error ("HepMC::GenParticle* points to NULL, return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  return deta ( p ) ;
}
// ============================================================================
double DeltaEta::deta ( const HepMC::GenParticle* p ) const
{ return eta ( momentum ( p )  ) - eta0 () ; }
// ============================================================================
//  OPTIONAL: "SHORT" representation
// ============================================================================
std::ostream& DeltaEta::fillStream ( std::ostream& s ) const
{ return s << "GDETA(" << m_eta << ")" ; }
// ============================================================================
//  constructor from eta and phi
// ============================================================================
DeltaR2::DeltaR2 ( const double eta , const double phi )
  : DeltaPhi ( phi )
  , m_deta ( eta )
{}
// ============================================================================
//  constructor from the vector
// ============================================================================
DeltaR2::DeltaR2 ( const LoKi::ThreeVector& v )
  : DeltaPhi ( v )
  , m_deta ( v )
{}
// ============================================================================
//  constructor from the vector
// ============================================================================
DeltaR2::DeltaR2 ( const LoKi::LorentzVector& v )
  : DeltaPhi ( v )
  , m_deta ( v )
{}
// ============================================================================
//  constructor from the particle
// ============================================================================
DeltaR2::DeltaR2 ( const HepMC::GenParticle* v )
  : DeltaPhi ( v )
  , m_deta ( v )
{}
// ============================================================================
//  MANDATORY: clone method ("virtual constructor")
// ============================================================================
DeltaR2* DeltaR2::clone() const { return new DeltaR2(*this) ; }
// ============================================================================
//  MANDATORY: the only essential method
// ============================================================================
double DeltaR2::operator() ( const HepMC::GenParticle* p ) const
{
  if ( 0 == p )
  {
    Error ("HepMC::GenParticle* points to NULL, return 'InvalidDustance'") ;
    return LoKi::Constants::InvalidDistance ;
  }
  //
  const double dphi_ = dphi ( p ) ;
  const double deta_ = deta ( p ) ;
  //
  return dphi_*dphi_ + deta_*deta_ ;
}
// ============================================================================
//  OPTIONAL: "SHORT" representation
// ============================================================================
std::ostream& DeltaR2::fillStream ( std::ostream& s ) const
{ return s << "GDR2( " << eta0() << "," << phi0() << ")" ; }

} }
// ============================================================================
// The END
// ============================================================================
