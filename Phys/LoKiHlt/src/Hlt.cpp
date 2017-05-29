// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include <boost/regex/v4/match_flags.hpp>
#include <boost/regex/v4/perl_matcher_common.hpp>
#include <boost/regex/v4/perl_matcher_non_recursive.hpp>
#include <boost/regex/v4/regex_match.hpp>
#include <ext/alloc_traits.h>
#include <stddef.h>
#include <algorithm>
#include <iterator>
#include <memory>
#include <ostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "Event/HltDecReport.h"
#include "Event/HltDecReports.h"
#include "GaudiKernel/StringKey.h"
#include "GaudiKernel/ToStream.h"
#include "LoKi/AuxFunBase.h"
#include "LoKi/Functor.h"
#include "LoKi/HLT.h"
#include "LoKi/RoutingBits.h"

// ============================================================================
/** @file
 *  Implementation file for namespace LoKi::HLT
 *  @date 2008-09-19
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 */
// ============================================================================
namespace
{
  // ==========================================================================
  inline bool isTurbo ( const LHCb::HltDecReport& rep )
  { return rep.executionStage()&0x80u; }
  // ==========================================================================
}
// ============================================================================
// constructor form the decision names ("OR")
// ============================================================================
LoKi::HLT::HasDecision::HasDecision
( LoKi::HLT::HasDecision::Names names )
  : LoKi::AuxFunBase ( std::tie ( names ) )
  , m_names ( std::move(names) )
{}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
LoKi::HLT::HasDecision::result_type
LoKi::HLT::HasDecision::operator()
  ( LoKi::HLT::HasDecision::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  return
    std::any_of( begin(), end(), [&](Names::const_reference n)
                 { return a -> hasDecisionName ( n ); });
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::HLT::HasDecision::print
( std::ostream&      s    ,
  const std::string& name ) const
{
  switch ( names().size() )
  {
  case 1 :
    return s << name << "('"
             <<   names().front()    << "') " ;  // RETURN
  case 2 :
    return s << name << "('"
             << *(names().begin()  ) << "','"
             << *(names().begin()+1) << "') " ;  // RETURN
  case 3 :
    return s << name << "('"
             << *(names().begin()  ) << "','"
             << *(names().begin()+1) << "','"
             << *(names().begin()+2) << "') " ;  // RETURN
  case 4 :
    return s << name << "('"
             << *(names().begin()  ) << "','"
             << *(names().begin()+1) << "','"
             << *(names().begin()+2) << "','"
             << *(names().begin()+3) << "') " ;  // RETURN

  default:
    return s << name << "("
             << Gaudi::Utils::toString ( names() ) << "') " ;  // RETURN
  }
  // This is unreachable
  //  return s << name << "("
  //           << Gaudi::Utils::toString ( names() ) << "') " ;  // RETURN
}
// ============================================================================


// ============================================================================
// constructor form the decision names ("OR")
// ============================================================================
LoKi::HLT::PassDecision::PassDecision
( Names names )
  : LoKi::AuxFunBase ( std::tie ( names ) )
  , LoKi::HLT::HasDecision ( std::move(names) )
{}
// ============================================================================




// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
LoKi::HLT::PassDecision::result_type
LoKi::HLT::PassDecision::operator()
  ( LoKi::HLT::PassDecision::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  //
  auto  _e = std::end(a -> decReports ());
  return std::any_of( begin(), end(),
                      [&](Names::const_reference name) {
    auto found = a -> find ( name ) ;
    return  _e != found && found->second.decision ();
  } );
}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
LoKi::HLT::Size::result_type
LoKi::HLT::Size::operator()
  ( LoKi::HLT::Size::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  return a -> size() ;
}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
LoKi::HLT::NPass::result_type
LoKi::HLT::NPass::operator()
  ( LoKi::HLT::NPass::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  return std::count_if( std::begin(*a), std::end(*a),
                      [](LHCb::HltDecReports::Container::const_reference i) {
                            return i.second.decision();
  } );
}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
LoKi::HLT::Decision::result_type
LoKi::HLT::Decision::operator()
  ( LoKi::HLT::Decision::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  return std::any_of( std::begin(*a), std::end(*a),
                      [](LHCb::HltDecReports::Container::const_reference i) {
                            return  i.second.decision() ;
  }) ;
}


// ============================================================================
// constructor from vector of "special" decicions
// ============================================================================
LoKi::HLT::DecisionBut::DecisionBut
( LoKi::HLT::DecisionBut::Names names )
  : LoKi::AuxFunBase ( std::tie ( names ) )
  , m_special ( std::move(names) )
{}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
LoKi::HLT::DecisionBut::result_type
LoKi::HLT::DecisionBut::operator()
  ( LoKi::HLT::DecisionBut::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  //
  size_t nPositive = 0 ;
  for ( const auto&  i: *a )
  {
    // the decision
    if (!i.second.decision()) { continue ; }              // CONTINUE
    ++nPositive ;
    if ( nPositive > m_special.size ()  ||
         m_special.end() == std::find ( m_special.begin () ,
                                        m_special.end   () ,
                                        i.first ) )  {
        return true ;
    }
  }
  //
  return false ;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::HLT::DecisionBut::fillStream
( std::ostream& s ) const
{
  s << " HLT_PASSIGNORING(" ;
  switch ( m_special.size() )
  {
  case 1 :
    s << "'"  << m_special[0] << "'"    ; break ;
  case 2 :
    s << "'"  << m_special[0] << "'"
      << ",'" << m_special[1] << "'"    ; break ;
  case 3 :
    s << "'"  << m_special[0] << "'"
      << ",'" << m_special[1] << "'"
      << ",'" << m_special[2] << "'"    ; break ;
  case 4 :
    s << "'"  << m_special[0] << "'"
      << ",'" << m_special[1] << "'"
      << ",'" << m_special[2] << "'"
      << ",'" << m_special[3] << "'"    ; break ;
  default :
    s << Gaudi::Utils::toString ( m_special ) ;
  }
  //
  return s << ") " ;
}



// ============================================================================
// constructor
// ============================================================================
LoKi::HLT::HasDecisionSubString::HasDecisionSubString
( const std::string& substr )
  : LoKi::AuxFunBase ( std::tie ( substr ) )
  , m_substr ( substr )
{}
// ============================================================================
// clone method
// ============================================================================
LoKi::HLT::HasDecisionSubString*
LoKi::HLT::HasDecisionSubString::clone() const
{ return new LoKi::HLT::HasDecisionSubString(*this) ; }
// ============================================================================
// the actual evaluation
// ============================================================================
LoKi::HLT::HasDecisionSubString::result_type
LoKi::HLT::HasDecisionSubString::operator()
  ( LoKi::HLT::HasDecisionSubString::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  // loop over all selecttions and match the names
  return std::any_of( std::begin(*a), std::end(*a),
                      [&](LHCb::HltDecReports::Container::const_reference i) {
            return i.first.find( substr() ) != std::string::npos ;
  } );
}
// ============================================================================
// printout
// ============================================================================
std::ostream&
LoKi::HLT::HasDecisionSubString::fillStream ( std::ostream& s ) const
{ return s << " HLT_HAS_SUBSTR('" << substr() << "') " ; }



// ============================================================================
// constructor
// ============================================================================
LoKi::HLT::PassDecisionSubString::PassDecisionSubString
( const std::string& substr )
  : LoKi::AuxFunBase ( std::tie ( substr ) )
  , LoKi::HLT::HasDecisionSubString( substr )
{}
// ============================================================================
// clone method
// ============================================================================
LoKi::HLT::PassDecisionSubString*
LoKi::HLT::PassDecisionSubString::clone() const
{ return new LoKi::HLT::PassDecisionSubString(*this) ; }
// ============================================================================
// the actual evaluation
// ============================================================================
LoKi::HLT::PassDecisionSubString::result_type
LoKi::HLT::PassDecisionSubString::operator()
  ( LoKi::HLT::PassDecisionSubString::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  // loop over all selecttions and match the names
  return std::any_of( std::begin(*a), std::end(*a),
                      [&](LHCb::HltDecReports::Container::const_reference i) {
                return  i.second.decision() &&
                        std::string::npos != i.first.find( substr() ) ;
  });
}
// ============================================================================
// printout
// ============================================================================
std::ostream&
LoKi::HLT::PassDecisionSubString::fillStream ( std::ostream& s ) const
{ return s << " HLT_PASS_SUBSTR('" << substr() << "') " ; }




// ============================================================================
// constructor
// ============================================================================
LoKi::HLT::DecisionButSubString::DecisionButSubString
( const std::string& substr )
  : LoKi::AuxFunBase ( std::tie ( substr ) )
  , LoKi::HLT::PassDecisionSubString( substr )
{}
// ============================================================================
// clone method
// ============================================================================
LoKi::HLT::DecisionButSubString*
LoKi::HLT::DecisionButSubString::clone() const
{ return new LoKi::HLT::DecisionButSubString(*this) ; }
// ============================================================================
// the actual evaluation
// ============================================================================
LoKi::HLT::DecisionButSubString::result_type
LoKi::HLT::DecisionButSubString::operator()
  ( LoKi::HLT::DecisionButSubString::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  // loop over all selections and match the names
  return std::any_of( std::begin(*a), std::end(*a),
                      [&](LHCb::HltDecReports::Container::const_reference i) {
    return  i.second.decision() && std::string::npos == i.first.find( substr() );
  } );
}
// ============================================================================
// printout
// ============================================================================
std::ostream&
LoKi::HLT::DecisionButSubString::fillStream ( std::ostream& s ) const
{ return s << " HLT_PASSIGNORING_SUBSTR('" << substr() << "') " ; }


// ============================================================================
// constructor
// ============================================================================
LoKi::HLT::HasDecisionRegex::HasDecisionRegex
( const std::string& substr )
  : LoKi::AuxFunBase ( std::tie ( substr ) )
  , LoKi::HLT::HasDecisionSubString( substr )
  , m_expression ( substr )
{}
// ============================================================================
// clone method
// ============================================================================
LoKi::HLT::HasDecisionRegex*
LoKi::HLT::HasDecisionRegex::clone() const
{ return new LoKi::HLT::HasDecisionRegex(*this) ; }
// ============================================================================
// the actual evaluation
// ============================================================================
LoKi::HLT::HasDecisionRegex::result_type
LoKi::HLT::HasDecisionRegex::operator()
  ( LoKi::HLT::HasDecisionRegex::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  // loop over all selections and match the names
  return std::any_of(std::begin(*a),std::end(*a),
                     [&](LHCb::HltDecReports::Container::const_reference i) {
                        return boost::regex_match ( i.first , expression() );
                     } );
}
// ============================================================================
// printout
// ============================================================================
std::ostream&
LoKi::HLT::HasDecisionRegex::fillStream ( std::ostream& s ) const
{ return s << " HLT_HAS_RE('" << substr() << "') " ; }

// ============================================================================
// constructor
// ============================================================================
LoKi::HLT::PassDecisionRegex::PassDecisionRegex
( const std::string& substr )
  : LoKi::AuxFunBase ( std::tie ( substr ) )
  , LoKi::HLT::HasDecisionRegex( substr )
{}
// ============================================================================
// clone method
// ============================================================================
LoKi::HLT::PassDecisionRegex*
LoKi::HLT::PassDecisionRegex::clone() const
{ return new LoKi::HLT::PassDecisionRegex(*this) ; }
// ============================================================================
// the actual evaluation
// ============================================================================
LoKi::HLT::PassDecisionRegex::result_type
LoKi::HLT::PassDecisionRegex::operator()
  ( LoKi::HLT::PassDecisionRegex::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  // loop over all selections and match the names
  return std::any_of( std::begin(*a), std::end(*a),
                      [&](LHCb::HltDecReports::Container::const_reference i) {
            return  i.second.decision() &&
                    boost::regex_match ( i.first , expression() );
  } );
}
// ============================================================================
// printout
// ============================================================================
std::ostream&
LoKi::HLT::PassDecisionRegex::fillStream ( std::ostream& s ) const
{ return s << " HLT_PASS_RE('" << substr() << "') " ; }



// ============================================================================
// constructor
// ============================================================================
LoKi::HLT::DecisionButRegex::DecisionButRegex
( const std::string& substr )
  : LoKi::AuxFunBase ( std::tie ( substr ) )
  , LoKi::HLT::PassDecisionRegex( substr )
{}
// ============================================================================
// clone method
// ============================================================================
LoKi::HLT::DecisionButRegex*
LoKi::HLT::DecisionButRegex::clone() const
{ return new LoKi::HLT::DecisionButRegex(*this) ; }
// ============================================================================
// the actual evaluation
// ============================================================================
LoKi::HLT::DecisionButRegex::result_type
LoKi::HLT::DecisionButRegex::operator()
  ( LoKi::HLT::DecisionButRegex::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  // loop over all selections and match the names
  return std::any_of( std::begin(*a), std::end(*a),
                      [&](LHCb::HltDecReports::Container::const_reference i) {
      return i.second.decision() &&
             !boost::regex_match( i.first , expression() )  ;
  } );
}
// ============================================================================
// printout
// ============================================================================
std::ostream&
LoKi::HLT::DecisionButRegex::fillStream ( std::ostream& s ) const
{ return s << " HLT_PASSIGNORING_RE('" << substr() << "') " ; }
// ============================================================================

// ============================================================================
// constructor from the channel name
// ============================================================================
LoKi::HLT::ErrorBits::ErrorBits ( const std::string& name )
  : LoKi::AuxFunBase ( std::tie ( name ) )
  , m_key ( name )
{}
// ============================================================================
// MANDATORY: clone method ( "virtual constructor")
// ============================================================================
LoKi::HLT::ErrorBits* LoKi::HLT::ErrorBits::clone () const
{ return new LoKi::HLT::ErrorBits ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential methor
// ============================================================================
LoKi::HLT::ErrorBits::result_type
LoKi::HLT::ErrorBits::operator() ( LoKi::HLT::ErrorBits::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  // find the selection by name
  auto isel = a->find ( m_key ) ;
  if ( isel != std::end(*a) ) return isel->second.errorBits() ;
  Error ( "No decision has been found for '" + m_key.str() + "', return -1" ) ;
  return -1 ;
  //
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream&
LoKi::HLT::ErrorBits::fillStream ( std::ostream& s ) const
{ return s << " HLT_ERRORBITS('" + m_key.str() + ") " ; }
// ============================================================================


// ============================================================================
// constructor from the channel name
// ============================================================================
LoKi::HLT::NonTurboPass::NonTurboPass ( const std::string& name )
  : LoKi::AuxFunBase ( std::tie ( name ) )
  , LoKi::HLT::HasDecisionRegex ( name )
{}
// ============================================================================
// MANDATORY: clone method ( "virtual constructor")
// ============================================================================
LoKi::HLT::NonTurboPass* LoKi::HLT::NonTurboPass::clone () const
{ return new LoKi::HLT::NonTurboPass ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
LoKi::HLT::NonTurboPass::result_type
LoKi::HLT::NonTurboPass::operator()
  ( LoKi::HLT::NonTurboPass::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  // loop over all selections and match the names
  return std::any_of( std::begin(*a), std::end(*a),
                      [&](LHCb::HltDecReports::Container::const_reference i) {
            return  ( !isTurbo(i.second) ) && i.second.decision()  &&
                    boost::regex_match ( i.first , expression() );
  } );
  //
}

// ============================================================================
// constructor from the channel name
// ============================================================================
LoKi::HLT::TurboPass::TurboPass ( const std::string& name )
  : LoKi::AuxFunBase ( std::tie ( name ) )
  , LoKi::HLT::HasDecisionRegex ( name )
{}
// ============================================================================
// MANDATORY: clone method ( "virtual constructor")
// ============================================================================
LoKi::HLT::TurboPass* LoKi::HLT::TurboPass::clone () const
{ return new LoKi::HLT::TurboPass ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
LoKi::HLT::TurboPass::result_type
LoKi::HLT::TurboPass::operator()
  ( LoKi::HLT::TurboPass::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  // loop over all selections and match the names
  return std::any_of( std::begin(*a), std::end(*a),
                      [&](LHCb::HltDecReports::Container::const_reference i) {
            return  isTurbo(i.second) && i.second.decision() &&
                    boost::regex_match ( i.first , expression() );
  } );
  //
}

// ============================================================================
// constructor from the channel name
// ============================================================================
LoKi::HLT::ExecutionStage::ExecutionStage ( const std::string& name )
  : LoKi::AuxFunBase ( std::tie ( name ) )
  , LoKi::HLT::ErrorBits ( name )
{}
// ============================================================================
// MANDATORY: clone method ( "virtual constructor")
// ============================================================================
LoKi::HLT::ExecutionStage* LoKi::HLT::ExecutionStage::clone () const
{ return new LoKi::HLT::ExecutionStage ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential methor
// ============================================================================
LoKi::HLT::ExecutionStage::result_type
LoKi::HLT::ExecutionStage::operator()
  ( LoKi::HLT::ExecutionStage::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  // find the selection by name
  auto isel = a->find ( channel()  ) ;
  if ( isel != std::end(*a) ) return isel->second.executionStage () ;
  Error ( "No decision has been found for '" +
            channel().str() + "', return -1" ) ;
  return -1 ;
  //
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream&
LoKi::HLT::ExecutionStage::fillStream ( std::ostream& s ) const
{ return s << " HLT_EXECUTIONSTAGE('" + channel().str() + ") " ; }
// ============================================================================

// ============================================================================
// constructor from the channel name
// ============================================================================
LoKi::HLT::NumberOfCandidates::NumberOfCandidates ( const std::string& name )
  : LoKi::AuxFunBase ( std::tie ( name ) )
  , LoKi::HLT::ErrorBits ( name )
{}
// ============================================================================
// MANDATORY: clone method ( "virtual constructor")
// ============================================================================
LoKi::HLT::NumberOfCandidates* LoKi::HLT::NumberOfCandidates::clone () const
{ return new LoKi::HLT::NumberOfCandidates ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential methor
// ============================================================================
LoKi::HLT::NumberOfCandidates::result_type
LoKi::HLT::NumberOfCandidates::operator() ( LoKi::HLT::NumberOfCandidates::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  // find the selection by name
  auto isel = a->find ( channel () ) ;
  if ( isel != std::end(*a)  ) return isel->second.numberOfCandidates () ;
  Error ( "No decision has been found for '"
          + channel().str() + "', return -1" ) ;
  return -1 ;
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream&
LoKi::HLT::NumberOfCandidates::fillStream ( std::ostream& s ) const
{ return s << " HLT_NCANDIDATES('" + channel().str() + ") " ; }
// ============================================================================

// ============================================================================
// constructor from the channel name
// ============================================================================
LoKi::HLT::Saturated::Saturated
( const std::string&      name )
  : LoKi::AuxFunBase ( std::tie ( name ) )
  , m_key ( name )
{}
// ============================================================================
// MANDATORY: clone method ( "virtual constructor")
// ============================================================================
LoKi::HLT::Saturated*
LoKi::HLT::Saturated::clone() const { return new Saturated(*this) ; }
// ============================================================================
// MANDATORY: the only one essential methor
// ============================================================================
LoKi::HLT::Saturated::result_type
LoKi::HLT::Saturated::operator()
  ( LoKi::HLT::Saturated::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  // find the selection by name
  auto isel = a->find ( channel () ) ;
  if ( isel != std::end(*a) )  {
    return isel->second.numberOfCandidates () ==
           LHCb::HltDecReport::saturatedNumberOfCandidates() ;
  }
  Error ( "No decision has been found for '"
            + channel().str() + "', return false" ) ;
  return false ;
  //
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream&
LoKi::HLT::Saturated::fillStream ( std::ostream& s ) const
{ return s << " HLT_SATURATED('" + channel().str() + ") " ; }
// ============================================================================

// ============================================================================
// constructor from the list of lines & mask
// ============================================================================
LoKi::HLT::CountErrorBits::CountErrorBits
( const std::vector<std::string>&      lines ,
  const unsigned int                   mask  )
  : LoKi::AuxFunBase ( std::tie ( lines , mask ) )
  , m_lines ( lines.begin() , lines.end() )
  , m_mask  ( mask )
{}
// ============================================================================
// constructor from the list of lines & mask
// ============================================================================
LoKi::HLT::CountErrorBits::CountErrorBits
( const std::vector<Gaudi::StringKey>&  lines ,
  const unsigned int                    mask  )
  : LoKi::AuxFunBase ( std::tie ( lines , mask ) )
  , m_lines ( lines )
  , m_mask  ( mask  )
{}
// ============================================================================
// MANDATORY: clone method ( "virtual constructor")
// ============================================================================
LoKi::HLT::CountErrorBits*
LoKi::HLT::CountErrorBits::clone() const
{ return new CountErrorBits ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential methor
// ============================================================================
LoKi::HLT::CountErrorBits::result_type
LoKi::HLT::CountErrorBits::operator()
  ( LoKi::HLT::CountErrorBits::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  // loop over the lines
  result_type result = 0 ;
  for ( const auto& line : m_lines )
  {
    // find the selection by name
    auto isel = a->find ( line ) ;
    if ( isel == std::end(*a) )
    {
      Error ( "No decision found for '" + line.str() + "'" ) ;
      continue ;
    }
    if ( isel->second.errorBits() & m_mask ) { ++result ; }
  }
  //
  return result  ;
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream&
LoKi::HLT::CountErrorBits::fillStream ( std::ostream& s ) const
{
  s << " HLT_COUNT_ERRORBITS( " ;
  //
  switch ( m_lines.size() )
  {
  case 2 :
    return s << "'"   << m_lines[0]
             << "','" << m_lines[1]
             << "',"  << m_mask     << ") " ;
  case 3 :
    return s << "'"   << m_lines[0]
             << "','" << m_lines[1]
             << "','" << m_lines[2]
             << "',"  << m_mask     << ") " ;
  case 4 :
    return s << "'"   << m_lines[0]
             << "','" << m_lines[1]
             << "','" << m_lines[2]
             << "','" << m_lines[3]
             << "',"  << m_mask     << ") " ;
  default :
    break ;
  }
  //
  Gaudi::Utils::toStream ( m_lines , s ) ;
  return s << "," << m_mask << ") " ;
}
// ============================================================================

// ============================================================================
// constructor from the regular expression & mask
// ============================================================================
LoKi::HLT::CountErrorBitsRegex::CountErrorBitsRegex
( const std::string&  expression ,
  const unsigned int  mask       )
  : LoKi::AuxFunBase ( std::tie ( expression  ,  mask ) )
  , m_pattern    ( expression )
  , m_expression ( expression )
  , m_mask       ( mask )
{}
// ============================================================================
// MANDATORY: clone method ( "virtual constructor")
// ============================================================================
LoKi::HLT::CountErrorBitsRegex*
LoKi::HLT::CountErrorBitsRegex::clone() const
{ return new CountErrorBitsRegex ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential methor
// ============================================================================
LoKi::HLT::CountErrorBitsRegex::result_type
LoKi::HLT::CountErrorBitsRegex::operator()
  ( LoKi::HLT::CountErrorBitsRegex::argument a ) const
{
  Assert ( a , "const LHCb::HltDecReports* points to NULL!" ) ;
  //
  return std::count_if( std::begin(*a), std::end(*a),
                        [&](LHCb::HltDecReports::Container::const_reference i) {
        return ( i.second.errorBits() & m_mask  ) &&
               boost::regex_match( i.first , expression() );
  });
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream&
LoKi::HLT::CountErrorBitsRegex::fillStream ( std::ostream& s ) const
{ return s << " HLT_COUNT_ERRORBITS_RE('" << m_pattern << "' ) "; }
// ============================================================================


// ============================================================================
// constructor from routing bits
// ============================================================================
LoKi::HLT::HltRoutingBits::HltRoutingBits
( const LoKi::HLT::RoutingBits& bits )
  : LoKi::AuxFunBase ( std::tie ( bits ) )
  , m_bits ( bits )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::HLT::HltRoutingBits*
LoKi::HLT::HltRoutingBits::clone() const
{ return new HltRoutingBits ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
LoKi::HLT::HltRoutingBits::result_type
LoKi::HLT::HltRoutingBits::operator()
  ( LoKi::HLT::HltRoutingBits::argument /* a */ ) const
{
  return m_bits() ;
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::HLT::HltRoutingBits::fillStream ( std::ostream& s ) const
{ return s << " HLT_" << m_bits ; }
// ============================================================================

// ============================================================================
// The END
// ============================================================================
