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
// STD & STL
// ============================================================================
#include <stdlib.h>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IRndmGenSvc.h"
#include "GaudiKernel/RndmGenerators.h"
// ============================================================================
// LHcbMath
// ============================================================================
#include "LHCbMath/Blind.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Random.h"
#include "LoKi/Services.h"
// ============================================================================
#ifdef __INTEL_COMPILER             // Disable ICC remark
#  pragma warning( disable : 1572 ) // floating-point equality and inequality comparisons are unreliable
#endif
// ============================================================================
/** @file
 *  the implementation file for functors from the namespace LoKi::Random
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date   2008-03-19
 */
// ============================================================================
// constructor from the numbers
// ============================================================================
LoKi::Random::Numbers::Numbers( const Rndm::Numbers& numbers ) : m_service( nullptr ), m_numbers( numbers ) {}
// ============================================================================
// constructor from the generator and the service
// ============================================================================
LoKi::Random::Numbers::Numbers( const IRndmGen::Param& param, IRndmGenSvc* service )
    : m_service( service ), m_param( param.clone() ) {}
// ============================================================================
// constructor from the generator and the service
// ============================================================================
LoKi::Random::Numbers::Numbers( IRndmGenSvc* service, const IRndmGen::Param& param )
    : m_service( service ), m_param( param.clone() ) {}
// ============================================================================
// copy constructor
// ============================================================================
LoKi::Random::Numbers::Numbers( const LoKi::Random::Numbers& right )
    : LoKi::AuxFunBase( right )
    , LoKi::BasicFunctors<double>::Function( right )
    , m_service( right.m_service )
    , m_param( right.m_param ? right.m_param->clone() : nullptr )
    , m_numbers( right.m_numbers ) {}
// ============================================================================
// destructor
// ============================================================================
LoKi::Random::Numbers::~Numbers() {
  if ( m_service && !gaudi() ) {
    // Warning ("manual reset of IRndmGenSvc") ;
    m_service.reset();
  }
}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Random::Numbers* LoKi::Random::Numbers::clone() const { return new LoKi::Random::Numbers( *this ); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double LoKi::Random::Numbers::operator()( LoKi::Random::Numbers::argument ) const { return shoot(); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double LoKi::Random::Numbers::shoot() const {
  // everything is ok ? SHOOT!
  if ( m_numbers ) { return m_numbers.shoot(); } // RETURN
  //
  Assert( !!m_param, "Invalid parameters for Random Number Generator" );
  // locate the service if needed
  if ( !m_service ) {
    LoKi::Services& svcs = LoKi::Services::instance();
    //  try to catch the random number service from LoKi Service
    m_service = svcs.randSvc();
    Assert( !!m_service, "Unable to locate the Random Numbers Service " );
  }
  // try to initialize it:
  StatusCode sc = m_numbers.initialize( m_service, *m_param );
  Assert( sc.isSuccess(), "Unable to initialize Random Number Generator", sc );
  //
  return m_numbers.shoot(); // RETURN
}
// ============================================================================
// constructor  from the descriptor and the service
// ============================================================================
LoKi::Random::Uniform::Uniform( const Rndm::Flat& flat, IRndmGenSvc* service )
    : LoKi::AuxFunBase( std::make_tuple( flat.minimum(), flat.maximum() ) )
    , LoKi::Random::Numbers( flat, service )
    , m_flat( flat ) {}
// ============================================================================
// constructor  from the descriptor and the service
// ============================================================================
LoKi::Random::Uniform::Uniform( IRndmGenSvc* service, const Rndm::Flat& flat )
    : LoKi::AuxFunBase( std::make_tuple( flat.minimum(), flat.maximum() ) )
    , LoKi::Random::Numbers( flat, service )
    , m_flat( flat ) {}
// ============================================================================
// constructor from low, high and service
// ============================================================================
LoKi::Random::Uniform::Uniform( const double low, const double high, IRndmGenSvc* service )
    : LoKi::AuxFunBase( std::tie( low, high ) )
    , LoKi::Random::Numbers( Rndm::Flat( low, high ), service )
    , m_flat( low, high ) {}
// ============================================================================
// constructor from service , low and high
// ============================================================================
LoKi::Random::Uniform::Uniform( IRndmGenSvc* service, const double low, const double high )
    : LoKi::AuxFunBase( std::tie( low, high ) )
    , LoKi::Random::Numbers( Rndm::Flat( low, high ), service )
    , m_flat( low, high ) {}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Random::Uniform* LoKi::Random::Uniform::clone() const { return new LoKi::Random::Uniform( *this ); }
// ============================================================================
// OPTIONAL: nice a nice printout
// ============================================================================
std::ostream& LoKi::Random::Uniform::fillStream( std::ostream& s ) const {
  if ( 0 == m_flat.minimum() && 1 == m_flat.maximum() ) { return s << "XRNDM"; }
  return s << "XRND(" << m_flat.minimum() << "," << m_flat.maximum() << ")";
}
// ============================================================================
// constructor  from the descriptor and the service
// ============================================================================
LoKi::Random::Gaussian::Gaussian( const Rndm::Gauss& gauss, IRndmGenSvc* service )
    : LoKi::AuxFunBase( std::make_tuple( gauss.mean(), gauss.sigma() ) )
    , LoKi::Random::Numbers( gauss, service )
    , m_gauss( gauss ) {}
// ============================================================================
// constructor  from the descriptor and the service
// ============================================================================
LoKi::Random::Gaussian::Gaussian( IRndmGenSvc* service, const Rndm::Gauss& gauss )
    : LoKi::AuxFunBase( std::make_tuple( gauss.mean(), gauss.sigma() ) )
    , LoKi::Random::Numbers( gauss, service )
    , m_gauss( gauss ) {}
// ============================================================================
// constructor from mean, sigma and service
// ============================================================================
LoKi::Random::Gaussian::Gaussian( const double mean, const double sigma, IRndmGenSvc* service )
    : LoKi::AuxFunBase( std::tie( mean, sigma ) )
    , LoKi::Random::Numbers( Rndm::Gauss( mean, sigma ), service )
    , m_gauss( mean, sigma ) {}
// ============================================================================
// constructor from the service, mean and sigma
// ============================================================================
LoKi::Random::Gaussian::Gaussian( IRndmGenSvc* service, const double mean, const double sigma )
    : LoKi::AuxFunBase( std::tie( mean, sigma ) )
    , LoKi::Random::Numbers( Rndm::Gauss( mean, sigma ), service )
    , m_gauss( mean, sigma ) {}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Random::Gaussian* LoKi::Random::Gaussian::clone() const { return new LoKi::Random::Gaussian( *this ); }
// ============================================================================
// OPTIONAL: nice a nice printout
// ============================================================================
std::ostream& LoKi::Random::Gaussian::fillStream( std::ostream& s ) const {
  if ( 0 == m_gauss.mean() && 1 == m_gauss.sigma() ) { return s << "XGAUSS"; }
  return s << "XGAUS(" << m_gauss.mean() << "," << m_gauss.sigma() << ")";
}
// ============================================================================
/*  Constructor
 *  if seed is not equal to 0, the call for <c>srand</c> will be performed
 *  @param seed the seed (if not zero)
 */
// ============================================================================
LoKi::Random::Rand::Rand( const unsigned int seed )
    : LoKi::AuxFunBase( std::tie( seed ) ), LoKi::BasicFunctors<double>::Function() {
  if ( 0 != seed ) { srand( seed ); } // reset the seed
}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Random::Rand* LoKi::Random::Rand::clone() const { return new LoKi::Random::Rand( *this ); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double LoKi::Random::Rand::operator()( LoKi::Random::Rand::argument /* a */ ) const {
  double res = rand();
  res /= RAND_MAX;
  return res;
}
// ============================================================================
// OPTIONAL: just a nice printout
// ============================================================================
std::ostream& LoKi::Random::Rand::fillStream( std::ostream& s ) const { return s << "XRAND"; }
// ============================================================================

// =============================================================================
// constructor from the seed , min & max values
// =============================================================================
LoKi::Random::Blind::Blind( const std::string& seed, const double minv, const double maxv )
    : LoKi::AuxFunBase( std::tie( seed, minv, maxv ) )
    , m_result( Gaudi::Math::blind( "LoKi::Random::Blind_" + seed, minv, maxv ) )
    , m_seed( seed )
    , m_min( minv )
    , m_max( minv ) {}
// =============================================================================
// constructor from the seed , min & max values
// =============================================================================
LoKi::Random::Blind::Blind( const std::string& seed )
    : LoKi::AuxFunBase( std::tie( seed ) )
    , m_result( Gaudi::Math::blind( "LoKi::Random::Blind_" + seed, -1, 1 ) )
    , m_seed( seed )
    , m_min( -1 )
    , m_max( 1 ) {}
// ============================================================================
// MANDATORY: clone method ( "virtual construtor")
// ============================================================================
LoKi::Random::Blind* LoKi::Random::Blind::clone() const { return new LoKi::Random::Blind( *this ); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double LoKi::Random::Blind::operator()() const { return m_result; }
// ============================================================================
// OPTIONAL: just a nice printout
// ============================================================================
std::ostream& LoKi::Random::Blind::fillStream( std::ostream& s ) const {
  s << " BLIND(";
  Gaudi::Utils::toStream( m_seed, s );
  if ( -1 != m_min || 1 != m_max ) { s << " , " << m_min << " , " << m_max; }
  //
  return s << ") ";
}
// ============================================================================

// =============================================================================
// constructor from the seed , min & max values
// =============================================================================
LoKi::Random::XBlind::XBlind( const std::string& seed, const double minv, const double maxv )
    : LoKi::AuxFunBase( std::tie( seed, minv, maxv ) ), m_blind( seed, minv, maxv ) {}
// =============================================================================
// constructor from the seed , min & max values
// =============================================================================
LoKi::Random::XBlind::XBlind( const std::string& seed ) : LoKi::AuxFunBase( std::tie( seed ) ), m_blind( seed ) {}
// ============================================================================
// MANDATORY: clone method ( "virtual construtor")
// ============================================================================
LoKi::Random::XBlind* LoKi::Random::XBlind::clone() const { return new LoKi::Random::XBlind( *this ); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double LoKi::Random::XBlind::operator()( LoKi::Random::XBlind::argument a ) const { return a + m_blind(); }
// ============================================================================
// OPTIONAL: just a nice printout
// ============================================================================
std::ostream& LoKi::Random::XBlind::fillStream( std::ostream& s ) const {
  s << " XBLIND(";
  Gaudi::Utils::toStream( seed(), s );
  if ( -1 != minv() || 1 != maxv() ) { s << " , " << minv() << " , " << maxv(); }
  //
  return s << ") ";
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
