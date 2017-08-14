// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/Service.h"
#include "GaudiKernel/AlgTool.h"
// ============================================================================
#include "Event/Track.h"
#include "Event/TrackUse.h"
// ============================================================================

// ============================================================================
/** @file
 *  The implementation file for class : TrackUse
 *
 *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
 *  @date   2004-10-27
 */
// ============================================================================
namespace {
  template <typename T>
  constexpr auto to_()
  { return [](const auto& i) { return static_cast<T>(i); }; }
}
// ============================================================================
/// the basic method for delegation of properties
// ============================================================================
template <class TYPE>
inline StatusCode TrackUse::i_declareProperties ( TYPE* object )
{
  if ( !object ) { return StatusCode::FAILURE ; }
  //
  object -> declareProperty ( "CheckTracks"      , m_check       ) ;
  //
  object -> declareProperty ( "SkipClones"       , m_skipClones  ) ;
  object -> declareProperty ( "SkipInvalid"      , m_skipInvalid ) ;
  object -> declareProperty ( "SkipBackward"     , m_skipBackward) ;
  //
  object -> declareProperty ( "AcceptedType"     , m_type        ) ;
  object -> declareProperty ( "AcceptedFitStatus", m_fitstatus   ) ;
  object -> declareProperty ( "RejectedHistory"  , m_history     ) ;
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================
/// declare 'own' properties for the algorithm
// ============================================================================
StatusCode TrackUse::declareProperties ( Algorithm* alg )
{ return i_declareProperties ( alg ) ; }
// ============================================================================
/// declare 'own' properties for the service
// ============================================================================
StatusCode TrackUse::declareProperties ( Service*  svc )
{ return i_declareProperties ( svc ) ; }
// ============================================================================
/// declare 'own' properties for the tool
// ============================================================================
StatusCode TrackUse::declareProperties ( AlgTool*  tool )
{ return i_declareProperties ( tool ) ; }
// ============================================================================
/// get the list of accepted status
// ============================================================================
size_t TrackUse::acceptedFitStatus( std::vector<LHCb::Track::FitStatus>& s ) const
{
  std::transform( m_fitstatus.begin(), m_fitstatus.end(),
                  std::back_inserter(s), to_<LHCb::Track::FitStatus>() );
  return m_fitstatus.size() ;
}
// ============================================================================
/// get the list of accepted types
// ============================================================================
size_t TrackUse::acceptedType( std::vector<LHCb::Track::Types>&   t ) const
{
  std::transform( m_type.begin(), m_type.end(),
                  std::back_inserter(t), to_<LHCb::Track::Types>() );
  return m_type.size() ;
}
// ============================================================================
/// get the list of rejected history
// ============================================================================
size_t TrackUse::rejectedHistory( std::vector<LHCb::Track::History>& h ) const
{
  std::transform( m_history.begin(), m_history.end(),
                  std::back_inserter(h), to_<LHCb::Track::History>() );
  return m_history.size() ;
}
// ============================================================================
/// printout of the track into the stream
// ============================================================================
MsgStream&
TrackUse::print ( MsgStream& stream , const LHCb::Track* track ) const
{
  if ( !stream.isActive() ) { return stream                ; }   // RETURN
  if ( 0 == track         ) { return stream << "<INVALID>" ; }   // RETURN
  // Key
  if ( track->hasKey() ) { stream << "Key: " << track->key() << " " ; }
  // Flag
  stream << "Flags: "<< track->flags() << " [";
  if ( track->checkFlag ( LHCb::Track::Flags::Invalid     ) )
  { stream << " '" << LHCb::Track::Flags::Invalid      << "' "; }
  if ( track->checkFlag ( LHCb::Track::Flags::Clone       ) )
  { stream << " '" << LHCb::Track::Flags::Clone        << "' "; }
  if ( track->checkFlag ( LHCb::Track::Flags::Backward    ) )
  { stream << " '" << LHCb::Track::Flags::Backward     << "' "; }
  if ( track->checkFlag ( LHCb::Track::Flags::IPSelected  ) )
  { stream << " '" << LHCb::Track::Flags::IPSelected   << "' "; }
  if ( track->checkFlag ( LHCb::Track::Flags::PIDSelected ) )
  { stream << " '" << LHCb::Track::Flags::PIDSelected  << "' "; }
  if ( track->checkFlag ( LHCb::Track::Flags::Used ) )
  { stream << " '" << LHCb::Track::Flags::Used  << "' "; }
  stream << "] ";
  // Type
  stream << "Type:"       << " '" << track->type       () << "' " ;
  // Status
  stream << "FitStatus:"  << " '" << track->fitStatus  () << "' " ;
  // History
  stream << "History:"    << " '" << track->history    () << "' " ;
  // HistoryFit
  stream << "FitHistory:" << " '" << track->fitHistory () << "' " ;
  //
  return stream ;
}
// ============================================================================
/// printout of the track into the staream
// ============================================================================
MsgStream& TrackUse::fillStream ( MsgStream& s ) const
{
  s << " Check Tracks: " ;
  //
  if  ( check() ) {        s << "YES" ; }
  else            { return s << "NO"  ; }                          // RETURN
  //
  s << " Rejected flags: [ " ;
  if ( m_skipInvalid  ) { s << " '" << LHCb::Track::Flags::Invalid  << "' " ;}
  if ( m_skipClones   ) { s << " '" << LHCb::Track::Flags::Clone    << "' " ;}
  if ( m_skipBackward ) { s << " '" << LHCb::Track::Flags::Backward << "' " ;}
  s << "]" ;

  {
    s << " Accepted FitStatus: [" ;
    for ( const auto& i : m_fitstatus )
    { s << " '" << static_cast<LHCb::Track::FitStatus>(i) << "' " ; }
    s << "];" ;
  }
  {
    s << " Accepted Types: [" ;
    for ( const auto& i : m_type )
    { s << " '" << static_cast<LHCb::Track::Types>(i) << "' " ; }
    s << "];" ;
  }
  {
    s << " Rejected History: [" ;
    for ( const auto& i : m_history )
    { s << " '" << static_cast<LHCb::Track::History>(i) << "' " ; }
    s << "];" ;
  }
  return s ;
}
// ============================================================================
/// The END
// ============================================================================
