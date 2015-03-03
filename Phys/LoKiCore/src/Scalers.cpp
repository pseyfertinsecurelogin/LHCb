// $Id: Scalers.cpp,v 1.4 2010-04-04 12:20:56 ibelyaev Exp $
// ============================================================================
// Include files 
// ============================================================================
// STD & STL 
// ============================================================================
#include <cmath>
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
  : LoKi::Functor<void,bool> () 
  , m_uniform ( 0.0 , 1.0 , service ) 
  , m_prob    ( prob ) 
{}
// ============================================================================
// MANDATORY: virtual destructor 
// ============================================================================
LoKi::Scalers::RandomScaleV::~RandomScaleV () {}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Scalers::RandomScaleV*
LoKi::Scalers::RandomScaleV::clone() const 
{ return new LoKi::Scalers::RandomScaleV( *this ) ;}
// ============================================================================
// MANDATORY: the only one essential method 
// ============================================================================
LoKi::Scalers::RandomScaleV::result_type 
LoKi::Scalers::RandomScaleV::operator() 
  ( /* LoKi::Scalers::RandomScale::argument v */ ) const 
{ return eval ( /* v */ ) ; }
// ============================================================================
// MANDATORY: the only one essential method 
// ============================================================================
bool LoKi::Scalers::RandomScaleV::eval
( /* LoKi::Scalers::RandomScale::argument v */ ) const 
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
  : LoKi::Functor<double,bool> () 
  , m_scaler ( prob , service ) 
{}
// ============================================================================
// MANDATORY: virtual destructor 
// ============================================================================
LoKi::Scalers::RandomScale::~RandomScale () {}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Scalers::RandomScale*
LoKi::Scalers::RandomScale::clone() const 
{ return new LoKi::Scalers::RandomScale( *this ) ;}
// ============================================================================
// MANDATORY: the only one essential method 
// ============================================================================
LoKi::Scalers::RandomScale::result_type 
LoKi::Scalers::RandomScale::operator() 
  ( LoKi::Scalers::RandomScale::argument v ) const 
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
  : LoKi::Functor<void,bool> ()
  , m_skip ( skip ) 
  , m_curr ( 0    )
{}
// ===========================================================================
// MANDATORY: virtual destructor 
// ===========================================================================
LoKi::Scalers::SkipperV::~SkipperV (){}
// ===========================================================================
// MANDATORY: clone method ("virtual constructor")
// ===========================================================================
LoKi::Scalers::SkipperV*
LoKi::Scalers::SkipperV::clone() const
{ return new LoKi::Scalers::SkipperV ( *this ) ; }
// ===========================================================================
// MANDATORY: the only one essential method 
// ===========================================================================
LoKi::Scalers::SkipperV::result_type 
LoKi::Scalers::SkipperV::operator() 
  ( /* LoKi::Scalers::Skipper::argument v */ ) const 
{ return eval ( /* v */ ) ; }
// ===========================================================================
LoKi::Scalers::SkipperV::result_type 
LoKi::Scalers::SkipperV::eval 
(/*  LoKi::Scalers::SkipperV::argument v */ ) const 
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
  : LoKi::Functor<double,bool> ()
  , m_skipper ( skip )  
{}
// ===========================================================================
// MANDATORY: virtual destructor 
// ===========================================================================
LoKi::Scalers::Skipper::~Skipper (){}
// ===========================================================================
// MANDATORY: clone method ("virtual constructor")
// ===========================================================================
LoKi::Scalers::Skipper*
LoKi::Scalers::Skipper::clone() const
{ return new LoKi::Scalers::Skipper ( *this ) ; }
// ===========================================================================
// MANDATORY: the only one essential method 
// ===========================================================================
LoKi::Scalers::Skipper::result_type 
LoKi::Scalers::Skipper::operator() 
  ( LoKi::Scalers::Skipper::argument v ) const { return eval ( v ) ; }
// ===========================================================================
LoKi::Scalers::Skipper::result_type 
LoKi::Scalers::Skipper::eval 
( LoKi::Scalers::Skipper::argument /* v */ ) const 
{
  return m_skipper.eval() ;
}
// ===========================================================================
std::ostream& LoKi::Scalers::Skipper::fillStream( std::ostream& s ) const 
{ return s << " XSKIP ( " << m_skipper.skip() << " ) " ; }
// ===========================================================================



// ============================================================================
/*  constructor from rate and "random" flag 
 *  @param maxRate the maximal rate 
 *  @param random the random flag 
 */
// ============================================================================
LoKi::Scalers::RateLimitV::RateLimitV
( const double                       maxRate , 
  const LoKi::Scalers::RateLimitType flag    ) 
  : LoKi::Functor<void,bool> () 
  , LoKi::Listener           () 
  , m_rateSvc   ()
  , m_uniform   ( 0.0 , 1.0 ) 
  , m_rate      ( maxRate   ) 
  , m_limitType ( flag      ) 
  , m_interval  ( 1 ) 
  , m_next      ( 1 ) 
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
  : LoKi::Functor<void,bool> ()  
  , LoKi::Listener           () 
  , m_rateSvc   ( service   )
  , m_uniform   ( 0.0 , 1.0 ) 
  , m_rate      ( maxRate   ) 
  , m_limitType ( flag      ) 
  , m_interval  ( 1 ) 
  , m_next      ( 1 )
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
  : LoKi::Functor<void,bool> ()  
  , LoKi::Listener           () 
  , m_rateSvc   (  )
  , m_uniform   ( 0.0 , 1.0 ) 
  , m_rate      ( maxRate   ) 
  , m_limitType ( flag      ) 
  , m_interval  ( 1 ) 
  , m_next      ( 1 ) 
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
  m_next = m_rateSvc -> tick () ;
  //
  // randomize initial phase in case of perioding limiter
  switch ( limitType() ) 
  {
  case LoKi::Scalers::RandomPhasePeriodicLimiter : 
    m_next += m_interval * m_uniform ( m_next ) ;  break ;
  default:
    m_next += 0 ;
  }
  //
  // subscribe the incident:
  //
  subscribe (              "RunChange" ).ignore() ;
  subscribe ( IncidentType::BeginRun   ).ignore() ;
  //
}
// ============================================================================
/* copy constructor
 *  take care abotu rundomization of initial phase 
 */
// ============================================================================
LoKi::Scalers::RateLimitV::RateLimitV 
( const LoKi::Scalers::RateLimitV& right ) 
  : LoKi::AuxFunBase           ( right ) 
  , LoKi::Functor<void,bool>   ( right ) 
  , LoKi::Listener             ( right ) 
  , m_rateSvc   ( right.m_rateSvc   )
  , m_uniform   ( right.m_uniform   ) 
  , m_rate      ( right.m_rate      ) 
  , m_limitType ( right.m_limitType ) 
  , m_interval  ( right.m_interval  ) 
  , m_next      ( right.m_next      ) 
{
  // randomize initial phase in case of perioding limiter
  switch ( limitType() ) 
  {
  case LoKi::Scalers::RandomPhasePeriodicLimiter : 
    m_next += m_interval * m_uniform ( m_next ) ; break ;
  default:
    m_next += 0 ;
  }
}
// ============================================================================
// MANDATORY: virtual destructor 
// ============================================================================
LoKi::Scalers::RateLimitV::~RateLimitV(){}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Scalers::RateLimitV*
LoKi::Scalers::RateLimitV::clone() const 
{ return new LoKi::Scalers::RateLimitV ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method 
// ============================================================================
LoKi::Scalers::RateLimitV::result_type 
LoKi::Scalers::RateLimitV::operator() 
  ( /* LoKi::Scalers::RateLimit::argument v */ ) const 
{ return eval ( /* v */ ) ; }
// ============================================================================
// MANDATORY: the only one essential method 
// ============================================================================
bool LoKi::Scalers::RateLimitV::eval 
( /* LoKi::Scalers::RateLimitV::argument v */ ) const 
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
    switch ( limitType() ) 
    {
    case LoKi::Scalers::RandomLimiter : 
      m_next += m_interval * -std::log ( m_uniform ( m_next ) ) ; break ;
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
  IReferenceRate* svc = 0 ;
  m_rateSvc = svc ;
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
void LoKi::Scalers::RateLimitV:: handle ( const Incident& /* incident */ ) 
{
  //
  if ( 0 < m_rate ) { m_interval = m_rateSvc->rate() / m_rate ; }
  m_next = m_rateSvc -> tick () ;
  //
  // randomize initial phase in case of perioding limiter
  switch ( limitType() ) 
  {
  case LoKi::Scalers::RandomPhasePeriodicLimiter : 
    m_next += m_interval * m_uniform ( m_next ) ; break ;
  default:
    m_next += 0 ;
  }
  //
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
  : LoKi::Functor<double,bool> () 
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
  : LoKi::Functor<double,bool> () 
  , m_rateLimit ( service , maxRate , flag ) 
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
  : LoKi::Functor<double,bool> () 
  , m_rateLimit ( service , maxRate , flag ) 
{}
// ============================================================================
// MANDATORY: virtual destructor 
// ============================================================================
LoKi::Scalers::RateLimit::~RateLimit(){}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Scalers::RateLimit*
LoKi::Scalers::RateLimit::clone() const 
{ return new LoKi::Scalers::RateLimit ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method 
// ============================================================================
LoKi::Scalers::RateLimit::result_type 
LoKi::Scalers::RateLimit::operator() 
  ( LoKi::Scalers::RateLimit::argument v ) const 
{ return eval ( v ) ; }
// ============================================================================
// MANDATORY: the only one essential method 
// ============================================================================
bool LoKi::Scalers::RateLimit::eval 
( LoKi::Scalers::RateLimit::argument /* v */ ) const 
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




 





// ============================================================================
// The END 
// ============================================================================

