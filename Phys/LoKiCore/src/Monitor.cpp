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
// ============================================================================
// GaudiKernel
// ============================================================================
#include <utility>

#include "GaudiKernel/HistoDef.h"
#include "GaudiKernel/Kernel.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiHistoID.h"
// ============================================================================
// local
// ============================================================================
#include "LoKi/HistoBook.h"
#include "LoKi/Monitor.h"
#include "LoKi/ToCpp.h"
// ============================================================================
/** @file
 *  implementation of helper classes for monitoring
 *  @date 2015-01-14
 *  @author Vanya Belyaev  Ivan.Belyaev@itep.ru
 *
 */
// ============================================================================
// constructors for Histogram service
// ============================================================================
LoKi::Histo::Histo() : m_valid( false ) {}
// ============================================================================
LoKi::Histo::Histo( std::string dir, const std::string& id, Gaudi::Histo1DDef hdef, IHistogramSvc* svc )
    : m_path( std::move( dir ) ), m_id( id ), m_hdef( std::move( hdef ) ), m_hsvc( svc ) {}
// ============================================================================
LoKi::Histo::Histo( std::string dir, const int id, Gaudi::Histo1DDef hdef, IHistogramSvc* svc )
    : m_path( std::move( dir ) ), m_id( id ), m_hdef( std::move( hdef ) ), m_hsvc( svc ) {}
// ============================================================================
LoKi::Histo::Histo( std::string dir, const GaudiAlg::ID& id, Gaudi::Histo1DDef hdef, IHistogramSvc* svc )
    : m_path( std::move( dir ) ), m_id( id ), m_hdef( std::move( hdef ) ), m_hsvc( svc ) {}
// ============================================================================
LoKi::Histo::Histo( std::string path, Gaudi::Histo1DDef hdef, IHistogramSvc* svc )
    : m_path( std::move( path ) ), m_hdef( std::move( hdef ) ), m_hsvc( svc ) {}
// ============================================================================
// constructors for Context service
// ============================================================================
LoKi::Histo::Histo( Gaudi::Histo1DDef hdef, const std::string& id, IAlgContextSvc* svc )
    : m_id( id ), m_hdef( std::move( hdef ) ), m_case( false ), m_cntx( svc ) {}
// ============================================================================
LoKi::Histo::Histo( Gaudi::Histo1DDef hdef, const int id, IAlgContextSvc* svc )
    : m_id( id ), m_hdef( std::move( hdef ) ), m_case( false ), m_cntx( svc ) {}
// ============================================================================
LoKi::Histo::Histo( Gaudi::Histo1DDef hdef, const GaudiAlg::ID& id, IAlgContextSvc* svc )
    : m_id( id ), m_hdef( std::move( hdef ) ), m_case( false ), m_cntx( svc ) {}
// ============================================================================
std::string Gaudi::Utils::toCpp( const GaudiAlg::ID& o ) {
  return "GaudiAlg::ID(" + ( o.numeric() ? toCpp( o.numericID() ) : o.literal() ? toCpp( o.literalID() ) : "" ) + ") ";
}
// ============================================================================
std::string Gaudi::Utils::toCpp( const Gaudi::Histo1DDef& o ) {
  return "Gaudi::Histo1DDef(" + toCpp( o.lowEdge() ) + ", " + toCpp( o.highEdge() ) + ", " + toCpp( o.bins() ) + ", " +
         toCpp( o.title() ) + ") ";
}
// ============================================================================
std::string Gaudi::Utils::toCpp( const LoKi::Histo& o ) {
  //
  std::string s = "LoKi::Histo(";
  if ( !o.valid() ) { return s + ")"; } // RETURN
  //
  if ( o.hcase() ) {
    s += toCpp( o.path() ) + ", ";
    const GaudiAlg::ID& id = o.id();
    if ( id.undefined() ) {
    } else {
      s += toCpp( id ) + "; ";
    }
  }
  //
  s += toCpp( o.hdef() );
  if ( !o.hcase() ) { s += ", " + toCpp( o.id() ); }
  //
  return s + ")";
}
// ============================================================================
LoKi::CounterDef::CounterDef( std::string group, std::string name, const LoKi::Monitoring::Flag flag )
    : m_group( std::move( group ) ), m_name( std::move( name ) ), m_flag( flag ) {}
// ============================================================================
LoKi::CounterDef::CounterDef( std::string name, const LoKi::Monitoring::Flag flag )
    : m_name( std::move( name ) ), m_flag( flag ) {}
// ============================================================================
std::string Gaudi::Utils::toCpp( const LoKi::CounterDef& o ) {
  //
  std::string s = "LoKi::CounterDef(";
  if ( !o.valid() ) { return s + ")"; } // RETURN
  //
  if ( !o.group().empty() ) { s += toCpp( o.group() ) + ","; }
  s += toCpp( o.name() ) + ",";
  //
  switch ( o.flag() ) {
  case LoKi::Monitoring::ContextSvc:
    s += "LoKi::Monitoring::ContextSvc";
    break;
  case LoKi::Monitoring::StatSvc:
    s += "LoKi::Monitoring::StatSvc";
    break;
  default:
    s += toCpp( (int)o.flag() );
    break;
  }
  //
  return s + ")";
}

// ============================================================================
// The END
// ============================================================================
