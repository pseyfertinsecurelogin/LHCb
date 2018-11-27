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
#include <cmath>
#include <limits>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/Incident.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/ILoKiSvc.h"
#include "LoKi/Scalers.h"
// ============================================================================
namespace
{
  // ==========================================================================
  /// the type/name of "reference rate" servce
  const std::string s_RATESVC = "HltReferenceRateSvc" ;
  // ==========================================================================
}
// ===========================================================================
/*  constructor from probability and random service
 *  @param prop accep probability
 *  @param service the random number service
 */
// ===========================================================================
LoKi::Scalers::RandomScaleV::RandomScaleV
( const double prob    ,
  IRndmGenSvc* service )
  : LoKi::AuxFunBase ( std::tie ( prob ) )
  , m_uniform ( 0.0 , 1.0 , service )
  , m_prob    ( prob )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Scalers::RandomScaleV* LoKi::Scalers::RandomScaleV::clone() const
{ return new LoKi::Scalers::RandomScaleV( *this ) ;}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Scalers::RandomScaleV::operator()() const
{ return eval (  ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Scalers::RandomScaleV::eval() const
{ return  m_uniform ( 0 ) <= m_prob ; }
// ============================================================================
std::ostream& LoKi::Scalers::RandomScaleV::fillStream( std::ostream& s ) const
{ return s << " FSCALE( " << m_prob << " ) " ; }
// ===========================================================================

// ===========================================================================
/*  constructor from probability and random service
 *  @param prop accep probability
 *  @param service the random number service
 */
// ===========================================================================
LoKi::Scalers::RandomScale::RandomScale
( const double prob    ,
  IRndmGenSvc* service )
  : LoKi::AuxFunBase ( std::tie ( prob ) )
  , m_scaler ( prob , service )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Scalers::RandomScale* LoKi::Scalers::RandomScale::clone() const
{ return new LoKi::Scalers::RandomScale( *this ) ;}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Scalers::RandomScale::operator()( double v ) const
{ return eval ( v ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Scalers::RandomScale::eval
( LoKi::Scalers::RandomScale::argument /* v */ ) const
{ return  m_scaler.eval() ; }
// ============================================================================
std::ostream& LoKi::Scalers::RandomScale::fillStream( std::ostream& s ) const
{ return s << " XSCALE( " << m_scaler.prob() << " ) " ; }
// ===========================================================================

// ===========================================================================
/* constructor from probability and random service
 *  @param prop accep probability
 *  @param service the random number servce
 */
// ===========================================================================
LoKi::Scalers::SkipperV::SkipperV ( const size_t skip )
  : LoKi::AuxFunBase ( std::tie ( skip ) )
  , m_skip ( skip )
{}
// ===========================================================================
// MANDATORY: clone method ("virtual constructor")
// ===========================================================================
LoKi::Scalers::SkipperV* LoKi::Scalers::SkipperV::clone() const
{ return new LoKi::Scalers::SkipperV ( *this ) ; }
// ===========================================================================
// MANDATORY: the only one essential method
// ===========================================================================
bool LoKi::Scalers::SkipperV::operator()( ) const
{ return eval ( ) ; }
// ===========================================================================
bool LoKi::Scalers::SkipperV::eval( ) const
{
  (++m_curr) %= m_skip ;
  return  0 == m_curr ;
}
// ===========================================================================
std::ostream& LoKi::Scalers::SkipperV::fillStream( std::ostream& s ) const
{ return s << " FSKIP ( " << m_skip << " ) " ; }
// ===========================================================================

// ===========================================================================
/* constructor from probability and random service
 *  @param prop accep probability
 *  @param service the random number servce
 */
// ===========================================================================
LoKi::Scalers::Skipper::Skipper ( const size_t skip )
  : LoKi::AuxFunBase ( std::tie ( skip ) )
  , m_skipper ( skip )
{}
// ===========================================================================
// MANDATORY: clone method ("virtual constructor")
// ===========================================================================
LoKi::Scalers::Skipper* LoKi::Scalers::Skipper::clone() const
{ return new LoKi::Scalers::Skipper ( *this ) ; }
// ===========================================================================
// MANDATORY: the only one essential method
// ===========================================================================
bool LoKi::Scalers::Skipper::operator()( double v ) const { return eval( v ) ; }
// ===========================================================================
bool LoKi::Scalers::Skipper::eval( double ) const
{
  return m_skipper.eval() ;
}
// ===========================================================================
std::ostream& LoKi::Scalers::Skipper::fillStream( std::ostream& s ) const
{ return s << " XSKIP ( " << m_skipper.skip() << " ) " ; }
// ===========================================================================

namespace
{
  inline
  std::string _toString ( const LoKi::Scalers::RateLimitType&  t )
  {
    switch ( t )
    {
    case      LoKi::Scalers::RandomPhasePeriodicLimiter :
      return "LoKi::Scalers::RandomPhasePeriodicLimiter" ;
    case      LoKi::Scalers::RandomLimiter :
      return "LoKi::Scalers::RandomLimiter" ;
    case      LoKi::Scalers::PlainPeriodicLimiter :
      return "LoKi::Scalers::PlainPeriodicLimiter" ;
    }
    return "static_cast<LoKi::Scalers::RateLimitType>(" + Gaudi::Utils::toCpp ( (int) t ) + ")" ;
  }
}
// ============================================================================
/*  constructor from rate and "random" flag
 *  @param maxRate the maximal rate
 *  @param random the random flag
 */
// ============================================================================
LoKi::Scalers::RateLimitV::RateLimitV
( const double                       maxRate ,
  const LoKi::Scalers::RateLimitType flag    )
  : LoKi::AuxFunBase ( std::make_tuple ( maxRate , LoKi::StrKeep( _toString ( flag ) ) ) )
  , m_uniform   ( 0.0 , 1.0 )
  , m_rate      ( maxRate   )
  , m_limitType ( flag      )
  , m_interval  ( 1 )
  , m_next      ( 1 )
  , m_previous  ( 1 )
{
  initialize_ ( s_RATESVC ) ;
}
// ============================================================================
/*  constructor from the service , rate and "random" flag
 *  @param service the rate service
 *  @param maxRate the maximal rate
 *  @param random the random flag
 */
// ============================================================================
LoKi::Scalers::RateLimitV::RateLimitV
( const IReferenceRate*              service ,
  const double                       maxRate ,
  const LoKi::Scalers::RateLimitType flag    )
  : m_rateSvc   ( service   )
  , m_uniform   ( 0.0 , 1.0 )
  , m_rate      ( maxRate   )
  , m_limitType ( flag      )
  , m_interval  ( 1 )
  , m_next      ( 1 )
  , m_previous  ( 1 )
{
  initialize_ ( s_RATESVC ) ;
}
// ============================================================================
/*  constructor from the service , rate and "random" flag
 *  @param service the rate service
 *  @param maxRate the maximal rate
 *  @param random the random flag
 */
// ============================================================================
LoKi::Scalers::RateLimitV::RateLimitV
( const std::string&                 service ,
  const double                       maxRate ,
  const LoKi::Scalers::RateLimitType flag    )
  : LoKi::AuxFunBase ( std::tie ( service , maxRate , flag ) )
  , m_uniform   ( 0.0 , 1.0 )
  , m_rate      ( maxRate   )
  , m_limitType ( flag      )
  , m_interval  ( 1 )
  , m_next      ( 1 )
  , m_previous  ( 1 )
{
  initialize_ ( service ) ;
}
// ============================================================================
// perform the initialization
// ============================================================================
void LoKi::Scalers::RateLimitV::initialize_ ( const std::string& svc )
{
  if ( !m_rateSvc )
  {
    StatusCode sc = getService  ( svc ) ;
    Assert ( sc.isSuccess() , "Unable to locate the service" , sc );
  }
  Assert ( !(!m_rateSvc) , "IReferenceRate* is invalid" ) ;
  //
  if ( 0 < m_rate ) { m_interval = m_rateSvc->rate() / m_rate ; }
  m_previous = m_rateSvc -> tick () ;
  //
  // randomize initial phase in case of perioding limiter
  double offset = 0;
  switch ( limitType() )
  {
  case LoKi::Scalers::RandomPhasePeriodicLimiter :
    offset = m_interval * m_uniform ( m_previous ) ;  break ;
  default:
    offset = 1 ; // wait at least one tick..
  }
  m_next = m_previous + offset;
  //
  // subscribe the incident:
  //
  subscribe (              "RunChange", std::numeric_limits<long>::min() ).ignore() ;
  subscribe ( IncidentType::BeginRun,   std::numeric_limits<long>::min() ).ignore() ;
  //
}
// ============================================================================
/* copy constructor
 *  take care about randomization of initial phase
 */
// ============================================================================
LoKi::Scalers::RateLimitV::RateLimitV
( const LoKi::Scalers::RateLimitV& right )
  : LoKi::AuxFunBase           ( right )
  , IInterface                 ( right )
  , IIncidentListener          ( right )
  , extend_interfaces1<IIncidentListener> ( right )
  , implements1<IIncidentListener>        ( right )
  , LoKi::Functor<void,bool>   ( right )
  , LoKi::Listener             ( right )
  , m_rateSvc   ( right.m_rateSvc   )
  , m_uniform   ( right.m_uniform   )
  , m_rate      ( right.m_rate      )
  , m_limitType ( right.m_limitType )
  , m_interval  ( right.m_interval  )
  , m_previous  ( right.m_previous  )
{
  // randomize initial phase in case of periodic limiter
  double offset = 0.;
  switch ( limitType() )
  {
  case LoKi::Scalers::RandomPhasePeriodicLimiter :
    offset = m_interval * m_uniform ( 0 ) ; break ;
  case LoKi::Scalers::RandomLimiter :
    offset = 1 + m_interval * -std::log ( m_uniform ( 0 ) ) ; break ;
  default:
    offset = 1 ; // wait at least one tick
  }
  m_next = m_previous + offset;
}
// ============================================================================
// MANDATORY: virtual destructor
// ============================================================================
LoKi::Scalers::RateLimitV::~RateLimitV()
{
  if ( m_rateSvc && !gaudi() )
  {
    // Warning("Manual reset of IReferenceRate") ;
    m_rateSvc.reset() ;
  }
}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Scalers::RateLimitV* LoKi::Scalers::RateLimitV::clone() const
{ return new LoKi::Scalers::RateLimitV ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Scalers::RateLimitV::operator() ( ) const { return eval (  ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Scalers::RateLimitV::eval(  ) const
{
  //
  if ( m_rate <= 0 ) { return false ; }

  // get the current tick form the service
  const size_t currenttick   = m_rateSvc->tick() ;

  // accept ?
  const bool accept = currenttick >= m_next ;

  // adjust the next:
  if ( accept )
  {
    // recompute interval -- the rate of the rateSvc may have changed!
    m_interval = m_rateSvc->rate() / m_rate ;

    m_previous = m_next;
    switch ( limitType() )
    {
    case LoKi::Scalers::RandomLimiter :
      m_next += m_interval * -std::log ( m_uniform( 0 ) ) ; break ;
    default:
      m_next += m_interval ;
    }
  }
  //
  return accept ;
}
// ============================================================================
// get the service
// ============================================================================
StatusCode
LoKi::Scalers::RateLimitV::getService
( const std::string& name ) const  // get the service
{
  m_rateSvc = nullptr;
  //
  SmartIF<ISvcLocator> svcloc ( lokiSvc().getObject() ) ;
  Assert ( !(!svcloc ) , "Unable to access ISvcLocator*" );
  //
  m_rateSvc = svcloc->service<IReferenceRate> ( name , true ) ;
  Assert ( !(!m_rateSvc) , "Unable to locate IReferenceRate* \"" +
           name + "\"");
  //
  return !m_rateSvc ? StatusCode::FAILURE : StatusCode::SUCCESS ;
  //
}
// ===========================================================================
/*  handle incidents
 *  @see LoKi::Listener
 *  @see IIncidentListener
 *  @param incident (INPUT) incident to listen
 */
// ===========================================================================
void LoKi::Scalers::RateLimitV::handle( const Incident& /* incident */ )
{
  //
  if ( 0 < m_rate ) { m_interval = m_rateSvc->rate() / m_rate ; }
  m_previous = m_rateSvc -> tick () ;
  //
  // randomize initial phase in case of perioding limiter
  double offset = 0;
  switch ( limitType() )
  {
  case LoKi::Scalers::RandomPhasePeriodicLimiter :
    offset = m_interval * m_uniform ( 0 ) ; break ;
  default:
    offset = 1 ; // wait at least one tick...
  }
  //
  m_next = m_previous + offset;
}
// ===========================================================================
std::ostream& LoKi::Scalers::RateLimitV::fillStream( std::ostream& s ) const
{
  s << " RATE( " << m_rate ;
  switch ( limitType() )
  {
    // pure random limiter
  case LoKi::Scalers::RandomLimiter              :      break ;
  case LoKi::Scalers::RandomPhasePeriodicLimiter :
    s << ", LoKi.Scalers.RandomPhasePeriodicLimiter " ; break ;
  case LoKi::Scalers::PlainPeriodicLimiter       :
    s << ", LoKi.Scalers.PlainPeriodicLimiter "       ; break ;
  default :
    s << "," << (int) limitType() << " " ;
  }
  return s << ") ";
}
// ===========================================================================


// ============================================================================
/*  constructor from rate and "random" flag
 *  @param maxRate the maximal rate
 *  @param random the random flag
 */
// ============================================================================
LoKi::Scalers::RateLimit::RateLimit
( const double                       maxRate ,
  const LoKi::Scalers::RateLimitType flag    )
  : LoKi::AuxFunBase ( std::tie ( maxRate , flag ) )
  , m_rateLimit ( maxRate , flag )
{}
// ============================================================================
/*  constructor from the service , rate and "random" flag
 *  @param service the rate service
 *  @param maxRate the maximal rate
 *  @param random the random flag
 */
// ============================================================================
LoKi::Scalers::RateLimit::RateLimit
( const IReferenceRate*              service ,
  const double                       maxRate ,
  const LoKi::Scalers::RateLimitType flag    )
  : m_rateLimit ( service , maxRate , flag )
{}
// ============================================================================
/*  constructor from the service , rate and "random" flag
 *  @param service the rate service
 *  @param maxRate the maximal rate
 *  @param random the random flag
 */
// ============================================================================
LoKi::Scalers::RateLimit::RateLimit
( const std::string&                 service ,
  const double                       maxRate ,
  const LoKi::Scalers::RateLimitType flag    )
  : LoKi::AuxFunBase ( std::tie ( service , maxRate , flag ) )
  , m_rateLimit ( service , maxRate , flag )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Scalers::RateLimit* LoKi::Scalers::RateLimit::clone() const
{ return new LoKi::Scalers::RateLimit ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Scalers::RateLimit::operator() ( argument v ) const
{ return eval ( v ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Scalers::RateLimit::eval( double ) const
{ return m_rateLimit.eval() ; }
// ===========================================================================
std::ostream& LoKi::Scalers::RateLimit::fillStream( std::ostream& s ) const
{
  s << "XRATE( " << rate() ;
  //
  switch ( limitType() )
  {
  case LoKi::Scalers::RandomLimiter              :      break ;
  case LoKi::Scalers::RandomPhasePeriodicLimiter :
    s << ", LoKi.Scalers.RandomPhasePeriodicLimiter " ; break ;
  case LoKi::Scalers::PlainPeriodicLimiter       :
    s << ", LoKi.Scalers.PlainPeriodicLimiter "       ; break ;
  default :
    s << "," << (int) limitType() << " " ;
  }
  //
  return s << ") ";
}
// ===========================================================================
// The END
// ===========================================================================
