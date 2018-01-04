// Include files

#include <string>
#include <map>
#include <array>
#include <algorithm>

// from gsl
#include "gsl/gsl_cdf.h"

// local
#include "Event/Track.h"
#include "Event/TrackFunctor.h"
#include "Event/TrackFitResult.h"
#include "Event/Node.h"

using namespace Gaudi;
using namespace LHCb;

// ============================================================================

//-----------------------------------------------------------------------------
// Implementation file for class : Track
//
// 2004-12-14 : Jose Hernando, Eduardo Rodrigues
//-----------------------------------------------------------------------------

//=============================================================================
// Retrieve a pointer to the fit result
//=============================================================================
const TrackFitResult* Track::fitResult() const
{
  return m_fitResult ;
}

//=============================================================================
// Retrieve a pointer to the fit result
//=============================================================================
TrackFitResult* Track::fitResult()
{
  return m_fitResult ;
}

//=============================================================================
// Set the fit result. This takes ownership.
//=============================================================================
void Track::setFitResult(LHCb::TrackFitResult* absfit)
{
  if ( m_fitResult != absfit ) delete std::exchange( m_fitResult, absfit );
}

//=============================================================================
// Move assignement operator
//=============================================================================
LHCb::Track& Track::operator=(LHCb::Track&& track)
{
  setChi2PerDoF( track.chi2PerDoF() );
  setNDoF( track.nDoF() );
  setLikelihood( track.likelihood() );
  setGhostProbability( track.ghostProbability() );
  setFlags( track.flags() );
  m_lhcbIDs = std::move(track.m_lhcbIDs);

  // copy the states
  clearStates();
  m_states = std::move(track.m_states);

  // copy the track fit info
  std::swap (m_fitResult, track.m_fitResult);

  setExtraInfo( track.extraInfo() );

  // copy the ancestors
  m_ancestors = std::move(track.m_ancestors);

  return *this;
}

//=============================================================================
// Get a range of nodes in this track
//=============================================================================
Track::ConstNodeRange Track::nodes() const
{
  if ( !m_fitResult ) { return Track::ConstNodeRange() ; }
  //
  const LHCb::TrackFitResult* _result = m_fitResult ;
  // cast the const container to a container of const pointers
  const LHCb::TrackFitResult::NodeContainer& nodes_ = _result->nodes() ;
  //
  typedef LHCb::TrackFitResult::NodeContainer::const_iterator Iterator1 ;
  typedef Track::ConstNodeRange::const_iterator               Iterator2 ;
  static_assert( sizeof(Iterator1) == sizeof(Iterator2), "iterator sizes must be equal" ) ;
  //
  const auto begin = nodes_ . cbegin () ;
  const auto end   = nodes_ . cend   () ;
  //
  // Helper union to avoid reinterpret_cast
  union _IteratorCast
  {
    const Iterator1* input  ;
    const Iterator2* output ;
  } ;
  // somehow, volatile didn't work here in gcc46
  _IteratorCast _begin ;
  _IteratorCast _end   ;
  //
  _begin . input = &begin ;
  _end   . input = &end   ;
  //
  return Track::ConstNodeRange ( *_begin.output , *_end.output ) ;
}

//=============================================================================
// Get the measurements on the track. Node that it does not return a
// reference. This is because I want to remove this container from
// fitresult.
//=============================================================================
Track::MeasurementContainer Track::measurements() const
{
  return m_fitResult ? MeasurementContainer{ m_fitResult->measurements().begin(),
                                             m_fitResult->measurements().end() }
                     : MeasurementContainer{} ;
}

namespace {

  template <size_t N>
  std::array<double,N+1>  generate_chi2max(double limit) {
    std::array<double,N+1> c = { 0 };
    std::generate( std::next(std::begin(c)), std::end(c),
                   [limit, i = 1u]() mutable
                   { return gsl_cdf_chisq_Qinv(limit, i++ ); } );
    return c;
  }

  // could put this into probChi2, but then the table is generated at
  // first use of probChi2, i.e. during the event loop.
  // This way, it tends to be generated when libTrackEvent.so is
  // dynamically linked into the executable, i.e. very early.
  static const auto chi2max = generate_chi2max<256>(1e-15);
}

//=============================================================================
// Retrieve the probability of chi^2
//=============================================================================
double Track::probChi2() const
{
  double val(0) ;
  if ( nDoF() > 0 )
  {
    // what to do if nDoF is bigger than the lookup table?
    // let's just do a range-checked acess, and have it throw
    // an exception... maybe not the most elegant solution...
    // alternative: chi2max[  std::min( nDoF(), int(chi2max.size() ) ]
    // in which case for unreasonable nDoF we don't go until 1e-15...
    val = ( chi2() < chi2max.at( nDoF() ) ? gsl_cdf_chisq_Q(chi2(),nDoF()) : 0 );
  }
  return val ;
}

//=============================================================================
// Retrieve the reference to the state closest to the given z-position
//=============================================================================
State & Track::closestState( double z )
{
  auto iter = std::min_element( m_states.begin(),m_states.end(),
                                TrackFunctor::distanceAlongZ(z) );
  if ( iter == m_states.end() )
    throw GaudiException( "No state closest to z","Track.cpp",
                          StatusCode::FAILURE );
  return *(*iter);
}

//=============================================================================
// Retrieve the (const) reference to the state closest to the given z-position
//=============================================================================
const State & Track::closestState( double z ) const
{
  if ( m_fitResult && !m_fitResult->nodes().empty() ) {
    auto iter = std::min_element( m_fitResult->nodes().begin(),m_fitResult->nodes().end(),
                                  TrackFunctor::distanceAlongZ(z) );
    if ( iter == m_fitResult->nodes().end() )
      throw GaudiException( "No state closest to z","Track.cpp",
                            StatusCode::FAILURE );
    return (*iter)->state();
  } else {
    auto iter = std::min_element( m_states.begin(),m_states.end(),
                                  TrackFunctor::distanceAlongZ(z) );
    if ( iter == m_states.end() )
      throw GaudiException( "No state closest to z","Track.cpp",
                            StatusCode::FAILURE );
    return *(*iter);
  }
}

//=============================================================================
// Retrieve the (const) reference to the state closest to the given plane
//=============================================================================
const State & Track::closestState( const Gaudi::Plane3D& plane ) const
{
  if ( m_fitResult && !m_fitResult->nodes().empty() ) {
    auto iter = std::min_element( m_fitResult->nodes().begin(),m_fitResult->nodes().end(),
                                  TrackFunctor::distanceToPlane(plane) );
    if ( iter == m_fitResult->nodes().end() )
      throw GaudiException( "No state closest to z","Track.cpp",
                            StatusCode::FAILURE );
    return (*iter)->state();
  } else {
    auto iter = std::min_element( m_states.begin(),m_states.end(),
                                  TrackFunctor::distanceToPlane(plane) );
    if ( iter == m_states.end() )
      throw GaudiException( "No state closest to plane","Track.cpp",
                            StatusCode::FAILURE );
    return *(*iter);
  }
}

//=============================================================================
// check the existence of a state at a certain predefined location
//=============================================================================
bool Track::hasStateAt( const LHCb::State::Location& location ) const
{
  return stateAt(location) != nullptr;
}

//=============================================================================
// Retrieve the pointer to the state closest to the given plane
//=============================================================================
State* Track::stateAt( const LHCb::State::Location& location )
{
  auto iter = std::find_if( m_states.begin(),m_states.end(),
                            [&](const LHCb::State* s)
                            { return s->checkLocation(location); } );
  return iter != m_states.end() ? *iter : nullptr;
}

//=============================================================================
// Retrieve the (const) pointer to the state at a given location
//=============================================================================
const State* Track::stateAt( const LHCb::State::Location& location ) const
{
  auto iter = std::find_if( m_states.begin(),m_states.end(),
                            [&](const LHCb::State* s)
                            { return s->checkLocation(location); } );
  return iter != m_states.end() ? *iter : nullptr;
}

//=============================================================================
// Add a State to the list of States associated to the Track
//=============================================================================
void Track::addToStates( const State& state )
{
  auto local = state.clone();
  const int order = ( checkFlag(Track::Flags::Backward) ? -1 : 1 );
  auto ipos = std::upper_bound(m_states.begin(),
                               m_states.end(),
                               local,
                               TrackFunctor::orderByZ(order));
  m_states.insert(ipos,local);
}

//=============================================================================
// Add a list of states to the list associated to the Track. This takes ownership.
//=============================================================================
void Track::addToStates( const StateContainer& states )
{
  auto middle = m_states.insert(m_states.end(), states.begin(), states.end()) ;
  // do not assumme that the incoming states are properly sorted.
  // The 'if' is ugly, but more efficient than using 'orderByZ'.
  if (checkFlag(Track::Flags::Backward)) {
    // it's already sorted in most cases
    if( ! std::is_sorted(states.begin(), states.end(), TrackFunctor::decreasingByZ())) {
      std::sort(middle,m_states.end(),TrackFunctor::decreasingByZ());
    }

    std::inplace_merge(m_states.begin(),middle,m_states.end(),TrackFunctor::decreasingByZ()) ;
  } else {
    // it's already sorted in most cases
    if( ! std::is_sorted(states.begin(), states.end(), TrackFunctor::increasingByZ())) {
      std::sort(middle,m_states.end(),TrackFunctor::increasingByZ());
    }

    std::inplace_merge(m_states.begin(),middle,m_states.end(),TrackFunctor::increasingByZ());
  }
}

//=============================================================================
// Remove an LHCbID from the list of LHCbIDs associated to the Track
//=============================================================================
void Track::removeFromLhcbIDs( const LHCbID& value )
{
  auto pos = std::lower_bound( m_lhcbIDs.begin(), m_lhcbIDs.end(), value ) ;
  if (pos!=m_lhcbIDs.end() && *pos==value) m_lhcbIDs.erase( pos ) ;
}

//=============================================================================
// Remove a State from the list of States associated to the Track
//=============================================================================
void Track::removeFromStates( State* state )
{
  TrackFunctor::deleteFromList(m_states,state);
}

//=============================================================================
// Add LHCbIDs to track
//=============================================================================
bool Track::addToLhcbIDs( const LHCb::LHCbID& value )
{
  auto pos = std::lower_bound( m_lhcbIDs.begin(),m_lhcbIDs.end(),value ) ;
  const bool rc = (pos == m_lhcbIDs.end()) || !( *pos == value ) ;
  if ( rc ) m_lhcbIDs.insert( pos, value ) ;
  return rc ;
}

//=============================================================================
// Add LHCbIDs to track
//=============================================================================
bool Track::addSortedToLhcbIDs( const LHCbIDContainer& ids )
{
  LHCbIDContainer result; result.reserve( ids.size() + m_lhcbIDs.size() ) ;
  std::set_union( ids.begin(), ids.end(),
                  m_lhcbIDs.begin(), m_lhcbIDs.end(),
                  std::back_inserter(result) );
  auto rc = ( result.size() == ids.size()+m_lhcbIDs.size() );
  m_lhcbIDs = std::move(result);
  return rc ;
}

//=============================================================================
// Count the number of LHCbIDs that two tracks have in common
//=============================================================================
namespace {

// inserter which only counts how often something is inserted
struct counting_inserter {
    size_t count = 0;
    counting_inserter& operator++() { return *this; } // nop
    counting_inserter& operator*() { return *this; } // redirect to self, so that out op= is called
    counting_inserter& operator=(const LHCbID&) { ++count; return *this; } // raison d'etre
};

}

size_t Track::nCommonLhcbIDs(const Track& rhs) const
{
  return std::set_intersection( begin(m_lhcbIDs), end(m_lhcbIDs),
                                begin(rhs.m_lhcbIDs), end(rhs.m_lhcbIDs),
                                counting_inserter{} ).count;
}

//=============================================================================
// Check whether the given LHCbID is on the Track
//=============================================================================
bool Track::isOnTrack( const LHCb::LHCbID& value ) const
{
  auto pos = std::lower_bound( m_lhcbIDs.begin(), m_lhcbIDs.end(), value ) ;
  return pos != m_lhcbIDs.end() && *pos == value ;
}

//=============================================================================
// Return the Measurement on the Track corresponding to the input LHCbID
//=============================================================================
const Measurement* Track::measurement( const LHCbID& value ) const
{
  return m_fitResult ? m_fitResult->measurement(value) : nullptr;
}

//=============================================================================
// reset the track
//=============================================================================
void Track::reset()
{
  setChi2PerDoF ( 0 );
  setNDoF       ( 0 );
  setFlags      ( 0 );
  setGhostProbability ( 999 );
  setLikelihood ( 999 );

  m_lhcbIDs.clear();
  std::for_each( m_states.begin(), m_states.end(), TrackFunctor::deleteObject() );
  m_states.clear();
  m_ancestors.clear();
  m_extraInfo.clear();
  delete m_fitResult ;
  m_fitResult = nullptr;
}

//=============================================================================
// Clone the track keeping the key
//=============================================================================
Track* Track::cloneWithKey( ) const
{
  Track* tr = new Track( this -> key() );
  tr -> copy( *this );
  return tr;
}

//=============================================================================
// Clone the track
//=============================================================================
Track* Track::clone() const
{
  Track* tr = new Track();
  tr -> copy( *this );
  return tr;
}

//=============================================================================
// Copy the info from the argument track into this track
//=============================================================================
void Track::copy( const Track& track )
{
  setChi2PerDoF( track.chi2PerDoF() );
  setNDoF( track.nDoF() );
  setLikelihood( track.likelihood() );
  setGhostProbability( track.ghostProbability() );
  setFlags( track.flags() );
  m_lhcbIDs = track.lhcbIDs();

  // copy the states
  clearStates();
  m_states.reserve( track.states().size() ) ;
  std::transform( track.states().begin(), track.states().end(),
                  std::back_inserter(m_states),
                  [](const LHCb::State* s) { return s->clone(); });

  // copy the track fit info
  delete std::exchange( m_fitResult, track.m_fitResult ? track.m_fitResult->clone()
                                                       : nullptr );

  setExtraInfo( track.extraInfo() );

  // copy the ancestors
  m_ancestors = track.m_ancestors;
}

//=============================================================================
// Clear the state vector
//=============================================================================
void Track::clearStates()
{
  std::for_each( m_states.begin(),
                 m_states.end(),TrackFunctor::deleteObject() );
  m_states.clear();
}

//=============================================================================
/** Check the presence of the information associated with
 *  a given key
 *
 *  @code
 *
 *  const Track* p = ... ;
 *
 *  Track::Key key = ... ;
 *  bool hasKey = p->hasInfo( key ) ;
 *
 *  @endcode
 *  @param    key key to be checked
 *  @return  'true' if there is informaiton with the 'key',
 *           'false' otherwise
 */
//=============================================================================
bool LHCb::Track::hasInfo ( const int key ) const
{
  return m_extraInfo.end() != m_extraInfo.find(key);
}

//=============================================================================
/** Add new information, associated with the specified key.
 *  This method cannot be used to modify information for an already existing key
 *
 *  @code
 *
 *  Track* p = ... ;
 *
 *  Track::Key  key   = ... ;
 *  Track::Info info  = ... ;
 *
 *  bool inserted = p->addInfo( key , info ) ;
 *
 *  @endcode
 *
 *  @param key key for the information
 *  @param info information to be associated with the key
 *  @return 'true' if information is inserted,
 *         'false' if information was not inserted, due to already existing key
 */
//=============================================================================
bool  LHCb::Track::addInfo ( const int key, const double info )
{
  return m_extraInfo.insert( key , info ).second;
}

//=============================================================================
/** extract the information associated with the given key
 *  If there is no such infomration the default value will
 *  be returned
 *
 *  @code
 *
 *  const Track* p = ... ;
 *
 *  Track::Key  key   = ... ;
 *
 *  // extract the information
 *  Track::Info info = p->info( key, -999 ) ;
 *
 *  @endcode
 *
 *  @param key key for the information
 *  @param def the default value to be returned
 *         in the case of missing info
 *  @return information associated with the key if there
 *          is such information, the default value otherwise
 */
//=============================================================================
double LHCb::Track::info( const int key, const double def ) const
{
  auto i = m_extraInfo.find( key ) ;
  return m_extraInfo.end() == i ? def : i->second;
}

//=============================================================================
// Count the number of outliers
//=============================================================================

unsigned int LHCb::Track::nMeasurementsRemoved() const
{
  return m_fitResult ? m_fitResult->nOutliers() : 0;
}

//=============================================================================
// Count the number of outliers
//=============================================================================

unsigned int LHCb::Track::nMeasurements() const
{
  return m_fitResult ? m_fitResult->nActiveMeasurements() : 0;
}

//=============================================================================
/** erase the information associated with the given key
 *
 *  @code
 *
 *  Track* p = ... ;
 *
 *  Track::Key  key   = ... ;
 *
 *  int erased = p->eraseInfo( key ) ;
 *
 *  @endcode
 *
 *  @param key key for the information
 *  @return return number of erased elements
 */
//=============================================================================
LHCb::Track::ExtraInfo::size_type
LHCb::Track::eraseInfo( const int key )
{
  return m_extraInfo.erase( key ) ;
}

//=============================================================================
// Fill stream
//=============================================================================
std::ostream& LHCb::Track::fillStream(std::ostream& os) const
{
  os << "*** Track ***" << std::endl
     << " key        : " << key() << std::endl
     << " type       : " << type() << std::endl
     << " history    : " << history() << std::endl
     << " fitstatus  : " << fitStatus() << std::endl
     << " # ids      : " << nLHCbIDs() << std::endl
     << " # meas     : " << nMeasurements() << std::endl
     << " chi2PerDoF : " << (float)m_chi2PerDoF << std::endl
     << " nDoF       : " << m_nDoF << std::endl
     << " GhostProb  : " << ghostProbability() << std::endl
     << " Likelihood : " << likelihood() << std::endl;

  os << " extraInfo : [";
  for ( const auto& i : extraInfo() ) {
    const LHCb::Track::AdditionalInfo info =
      static_cast<LHCb::Track::AdditionalInfo>(i.first);
    os << " " << info << "=" << i.second << " ";
  }
  os << "]" << std::endl;

  if ( !m_states.empty() ) {
    os << " p  : " << (float) firstState().p() <<std::endl
       << " pt : " << (float) firstState().pt() <<std::endl
       << " " << nStates() << " states at z =";
    for ( const auto& s : states() ) {
      if (s) os << " " << s->z();
    }
    os << "  :-" << std::endl;
    for ( const auto& s : states() ) {
      os << " " << *s;
    }
    os << std::endl;
  } else {
    os << " # states : " << nStates() << std::endl;
  }

  return os;
}
