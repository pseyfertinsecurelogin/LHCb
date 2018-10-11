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
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
#include <numeric>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/ToStream.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Constants.h"
#include "LoKi/EvtNum.h"
#include "LoKi/Odin.h"
#include "LoKi/ToCpp.h"
// ============================================================================
/** @file
 *
 *  Implementation file for classes from namesapce LoKi::Odin
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @date 2008-09-17
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *
 */

namespace {

template<typename... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<typename... Ts> overloaded(Ts...) -> overloaded<Ts...>;


auto dispatch_variant = [](auto&& variant, auto&&... lambdas) -> decltype(auto) {
  return std::visit( overloaded{ std::forward<decltype(lambdas)>(lambdas)... },
                     std::forward<decltype(variant)>(variant) );
};

}

// ============================================================================
// the constructor from the start/stop times
// ============================================================================
LoKi::Odin::InTime::InTime( const Gaudi::Time& start, const Gaudi::Time& stop  )
  : LoKi::AuxFunBase ( std::tie ( start , stop ) )
  , m_start ( start )
  , m_stop  ( stop  )
{}
// ============================================================================
// the constructor from the start&span times
// ============================================================================
LoKi::Odin::InTime::InTime( const Gaudi::Time& start, const Gaudi::TimeSpan& span  )
  : LoKi::AuxFunBase ( std::tie ( start , span ) )
  , m_start ( start        )
  , m_stop  ( start + span )
{}
// ============================================================================
// MANDATORY: The only one essential method:
// ============================================================================
bool LoKi::Odin::InTime::operator()( const LHCb::ODIN* o ) const
{
  const Gaudi::Time time = o -> eventTime () ;
  return m_start <= time && time < m_stop ;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::Odin::InTime::fillStream ( std::ostream& s ) const
{
  return s << "ODIN_TIME("
           << "Gaudi.Time(" << m_start . ns () << ","
           << "Gaudi.Time(" << m_stop  . ns () << ")" ;
}
// ============================================================================


// ============================================================================
// constructor from the run number
// ============================================================================
LoKi::Odin::RunNumber::RunNumber( const LoKi::Odin::RunNumber::run_type run )
  : LoKi::AuxFunBase ( std::tie ( run ) )
  , m_runs( run_range{ run, run+1 } )
{}
// ============================================================================
// constructor from the run range
// ============================================================================
LoKi::Odin::RunNumber::RunNumber( const LoKi::Odin::RunNumber::run_type begin ,
                                  const LoKi::Odin::RunNumber::run_type end   )
  : LoKi::AuxFunBase ( std::tie ( begin , end  ) )
  , m_runs( run_range( begin, end) )
{}
// ============================================================================
// constructor from the run list
// ============================================================================
LoKi::Odin::RunNumber::RunNumber( const LoKi::Odin::RunNumber::run_list& runs )
  : LoKi::AuxFunBase ( std::tie ( runs ) )
  , m_runs( runs )
{
  run_list& r = std::get<run_list>(m_runs);
  std::sort ( r.begin() , r.end() ) ;
}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Odin::RunNumber* LoKi::Odin::RunNumber::clone() const
{ return new LoKi::Odin::RunNumber ( *this ) ; }
// ============================================================================
// MANDATORY: The only one essential method:
// ============================================================================
bool LoKi::Odin::RunNumber::operator()( const LHCb::ODIN* o ) const
{
  //
  auto r = o->runNumber();
  return dispatch_variant( m_runs,
         [&](const run_range& rng) { return rng.first <= r && r < rng.second ; },
         [&](const run_list& lst)  { return std::binary_search( lst.begin(), lst.end(), r ); } );

}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::Odin::RunNumber::fillStream ( std::ostream& s ) const
{
  s << "ODIN_RUNNUMBER(" ;
  //
  return dispatch_variant( m_runs,
         [&](const run_range& rng) -> std::ostream&
         { s << rng.first;
           if (rng.second!=rng.first+1) { s << "," << rng.second; }
           return s << ")"; },
         [&](const run_list& lst) -> std::ostream&
         { return s << Gaudi::Utils::toString ( lst ) << ")" ; }) ;
}
// ============================================================================

// ============================================================================
// constructor from the run number
// ============================================================================
LoKi::Odin::BXId::BXId( unsigned int bx   )
  : LoKi::AuxFunBase ( std::tie ( bx ) )
  , m_bxs( bx_range(bx,bx+1) )
{}
// ============================================================================
// constructor from the run range
// ============================================================================
LoKi::Odin::BXId::BXId( unsigned int begin , unsigned int end   )
  : LoKi::AuxFunBase ( std::tie ( begin , end ) )
  , m_bxs ( bx_range(begin,end) )
{}
// ============================================================================
// constructor from the run list
// ============================================================================
LoKi::Odin::BXId::BXId( std::vector<unsigned int> bxs )
  : LoKi::AuxFunBase ( std::tie ( bxs ) )
  , m_bxs( std::move(bxs) )
{
  auto& b = std::get<bx_vector>(m_bxs);
  std::sort ( b.begin() , b.end() ) ;
}
// ============================================================================
// MANDATORY: The only one essential method:
// ============================================================================
bool LoKi::Odin::BXId::operator() ( const LHCb::ODIN* o ) const
{
  //
  auto bx = o->bunchId();
  return dispatch_variant( m_bxs,
         [&](const bx_range& rng) { return rng.first <= bx && bx < rng.second; },
         [&](const bx_vector& v)  { return std::binary_search( v.begin(), v.end(), bx ); } );
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::Odin::BXId::fillStream ( std::ostream& s ) const
{
  s << "ODIN_BXID(" ;
  return dispatch_variant( m_bxs,
         [&](const bx_range& rng) -> std::ostream&
         { s << rng.first ;
           if (rng.second!=rng.first+1) s << "," << rng.second;
           return s << ")"; },
         [&](const bx_vector& v) -> std::ostream&
         { return s << Gaudi::Utils::toString ( v ) << ")" ;  } );
}
// ============================================================================


// ============================================================================
// constructor from vector of tck
// ============================================================================
LoKi::Odin::Tck::Tck( std::vector<unsigned int> tcks )
  : LoKi::AuxFunBase ( std::tie ( tcks ) )
  , m_tcks   ( std::move(tcks)  )
{
  std::sort ( m_tcks.begin() , m_tcks.end() ) ;
}
// ============================================================================
// MANDATORY: The only one essential method:
// ============================================================================
bool LoKi::Odin::Tck::operator()( const LHCb::ODIN* o ) const
{
  return std::binary_search
      ( m_tcks.begin() , m_tcks.end() , o->triggerConfigurationKey() );
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::Odin::Tck::fillStream ( std::ostream& s ) const
{
  s << "ODIN_TCKEYS(" ;
  return s << Gaudi::Utils::toString( m_tcks ) << ")" ;
}
// ============================================================================

// ============================================================================
// constructor from the event number
// ============================================================================
LoKi::Odin::EvtNumber::EvtNumber( const LoKi::Odin::EvtNumber::event_type evt )
  : LoKi::AuxFunBase ( std::tie ( evt ) )
  , m_evts( event_range{ evt, evt+1 } )
{}
// ============================================================================
// constructor from event range
// ============================================================================
LoKi::Odin::EvtNumber::EvtNumber( const LoKi::Odin::EvtNumber::event_type begin ,
                                  const LoKi::Odin::EvtNumber::event_type end   )
  : LoKi::AuxFunBase ( std::tie ( begin , end ) )
  , m_evts  ( event_range{ begin, end } )
{}
// ============================================================================
// constructor from event list
// ============================================================================
LoKi::Odin::EvtNumber::EvtNumber( const LoKi::Odin::EvtNumber::event_list& evts )
  : LoKi::AuxFunBase ( std::tie ( evts ) )
  , m_evts ( evts )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Odin::EvtNumber*
LoKi::Odin::EvtNumber::clone() const
{ return new LoKi::Odin::EvtNumber(*this) ; }
// ============================================================================
// MANDATORY: The only one essential method:
// ============================================================================
bool LoKi::Odin::EvtNumber::operator()( const LHCb::ODIN* o ) const
{
  event_type evt ( o->eventNumber() ) ;
  return dispatch_variant( m_evts,
         [&](const event_range& rng) { return rng.first <= evt && evt < rng.second; },
         [&](const event_list&  lst) { return lst.contains(evt); } );

}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::Odin::EvtNumber::fillStream ( std::ostream& s ) const
{
  s << "ODIN_EVTNUMBER(" ;
  //
  return dispatch_variant( m_evts,
         [&](const event_range& rng) -> std::ostream&
         { s << rng.first;
           if (rng.second+0 != rng.first+1) s << "," << rng.second;
           return s << ")" ; },
         [&](const event_list& lst) -> std::ostream&
         { return s << Gaudi::Utils::toString ( lst ) << ")" ; } );

}
// ============================================================================


// ============================================================================
// constructor from the run/event number
// ============================================================================
LoKi::Odin::RunEvtNumber::RunEvtNumber( const LoKi::Odin::RunEvtNumber::run_type run ,
                                        const LoKi::Odin::RunEvtNumber::evt_type evt )
  : LoKi::AuxFunBase ( std::tie ( run , evt  ) )
  , m_runevts{ runevt_range{ {run,evt}, { run,evt.evt()+1 } } }
{}
// ============================================================================
// constructor from the run/event number
// ============================================================================
LoKi::Odin::RunEvtNumber::RunEvtNumber( const LoKi::Odin::RunEvtNumber::runevt_type& runevt )
  : LoKi::AuxFunBase ( std::tie ( runevt  ) )
  , m_runevts{ runevt_range{ runevt, { runevt.run(), runevt.evt() +1 } } }
{}
// ============================================================================
// constructor from the run/event range
// ============================================================================
LoKi::Odin::RunEvtNumber::RunEvtNumber( const runevt_type& begin,
                                        const runevt_type& end  )
  : LoKi::AuxFunBase ( std::tie ( begin , end   ) )
  , m_runevts{ runevt_range{ begin, end } }
{}
// ============================================================================
// constructor from the run/event list
// ============================================================================
LoKi::Odin::RunEvtNumber::RunEvtNumber( const runevt_list&  runevts )
  : LoKi::AuxFunBase ( std::tie ( runevts ) )
  , m_runevts ( runevts )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Odin::RunEvtNumber* LoKi::Odin::RunEvtNumber::clone() const
{ return new LoKi::Odin::RunEvtNumber ( *this ) ; }
// ============================================================================
// MANDATORY: The only one essential method:
// ============================================================================
bool LoKi::Odin::RunEvtNumber::operator()( const LHCb::ODIN* o ) const
{
  runevt_type runevt ( o->runNumber() , o->eventNumber() );
  //
  return dispatch_variant( m_runevts,
        [&](const runevt_range& rng) { return rng.first <= runevt && runevt < rng.second; },
        [&](const runevt_list&  lst) { return lst.contains(runevt); } );
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::Odin::RunEvtNumber::fillStream ( std::ostream& s ) const
{
  s << "ODIN_RUNEVT ( " ;
  //
  return dispatch_variant( m_runevts,
                           [&](const runevt_range& rng) -> std::ostream&
                           { if (rng.first.run()==rng.second.run() && rng.first.evt()+1==rng.second.evt() )
                             {
                               return s << rng.first.run()   << " , "
                                        << rng.first.event() << " ) " ;
                             }
                             return s << Gaudi::Utils::toString ( rng.first   ) << " , "
                                      << Gaudi::Utils::toString ( rng.second  ) << " ) " ;
                           },
                           [&](const runevt_list&  lst) -> std::ostream&
                           { return Gaudi::Utils::toStream( lst, s ) << ")";  } );
}
// ============================================================================


// ============================================================================
// constructor from routing bits
// ============================================================================
LoKi::Odin::RoutingBits::RoutingBits( const LoKi::HLT::RoutingBits& bits )
  : LoKi::AuxFunBase ( std::tie ( bits ) )
  , m_bits ( bits )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Odin::RoutingBits* LoKi::Odin::RoutingBits::clone() const
{ return new LoKi::Odin::RoutingBits ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Odin::RoutingBits::operator() ( const LHCb::ODIN* ) const
{
  return m_bits() ;
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::Odin::RoutingBits::fillStream ( std::ostream& s ) const
{ return s << " ODIN_" << m_bits ; }
// ============================================================================


// ============================================================================
// constructor from the modulo
// ============================================================================
LoKi::Odin::Evt1::Evt1( unsigned long long modulo )
  : LoKi::AuxFunBase ( std::tie ( modulo ) )
  , m_modulo ( modulo )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Odin::Evt1* LoKi::Odin::Evt1::clone() const
{ return new LoKi::Odin::Evt1 ( *this ) ; }
// ============================================================================
// MANDATORY: the only essential method
// ============================================================================
double LoKi::Odin::Evt1::operator()( const LHCb::ODIN* a ) const
{ return a -> eventNumber() % modulo() ; }
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::Odin::Evt1::fillStream ( std::ostream& s ) const
{ return s << " ODIN_EVT1(" << modulo() << ") " ; }
// ============================================================================


// ============================================================================
// constructor from the modulo
// ============================================================================
LoKi::Odin::Evt2::Evt2( unsigned long long modulo )
  : LoKi::AuxFunBase ( std::tie ( modulo ) )
  , LoKi::Odin::Evt1 ( modulo )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Odin::Evt2* LoKi::Odin::Evt2::clone() const
{ return new LoKi::Odin::Evt2 ( *this ) ; }
// ============================================================================
// MANDATORY: the only essential method
// ============================================================================
double LoKi::Odin::Evt2::operator() ( const LHCb::ODIN* a ) const
{
  //
  return a -> eventNumber() / modulo() ;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::Odin::Evt2::fillStream ( std::ostream& s ) const
{ return s << " ODIN_EVT2(" << modulo() << ") " ; }
// ============================================================================

// ============================================================================
// The END
// ============================================================================
