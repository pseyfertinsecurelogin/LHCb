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
#ifndef TrackEvent_TrackFunctor_H
#define TrackEvent_TrackFunctor_H 1

// Include files
// -------------
#include <type_traits>
#include <functional>

// from GaudiKernel
#include "GaudiKernel/Plane3DTypes.h"

// from TrackEvent
#include "Event/Track.h"
#include "Event/TrackFitResult.h"

/** @namespace TrackFunctor
 *
 *  This namespace:
 * - contains functors (previously contained in associated classes)
 * - other small classes and things that do not fit (yet)
 *
 *  @author Jose A. Hernando
 *  @author Eduardo Rodrigues
 *  @date   2005-04-05
 *
 * @author Rutger van der Eijk
 *  created Thu Jun 20 14:47:34 2002
 *
 */


namespace TrackFunctor
{
    namespace details
    {
        template <typename Callable>
        struct deref : Callable {

            // as we inherit from the type of the passed-in callable,
            // construct it from the passed-in callable
            // F is a template parameter so we can perfectly forward
            // the callable, but then F must be constrained to avoid
            // hijacking the copy/move constructor!
            template <typename F, typename = std::enable_if_t<std::is_constructible<Callable,F>::value>>
            constexpr deref(F&& g) : Callable{ std::forward<F>(g) } {}


            // make the call operator of 'original' callable
            // available
            using Callable::operator();

            // add a call operator which first dereferences
            // its arguments prior to invoking the 'orignal'
            // callable

            // binary
            template <typename Ptr1, typename Ptr2>
            auto operator()(Ptr1* p1, Ptr2* p2) const
            { return Callable::operator()(*p1, *p2); }

            // unary
            template <typename Ptr>
            auto operator()(Ptr* p) const
            { return Callable::operator()(*p); }

        };

        // Given some 'callable' which takes one or two arguments,
        // (of the same type), return another, "decorated" callable which
        // can _also_ be called with pointers to the (one or) two
        // arguments.
        //
        // For example (see https://godbolt.org/g/S5epFA,
        // or the unit test in this package)
        //
        // bool cmp() {
        //
        //     auto g = add_deref( [](int i, int j) { return i<j; } );
        //
        //     int a = 3;
        //     int b = 5;
        //
        //     return g(a,b) == g(&a,&b); // evaluates to true
        // }
        //
        template <typename T>
        constexpr deref<std::decay_t<T>> add_deref( T&& t ) {
            return { std::forward<T>(t) };
        }
    }

//=============================================================================
// Compare the distance along z to the specified z-value, of 2 objects
//=============================================================================
constexpr auto distanceAlongZ = [](double z0 = 0.) {
    return details::add_deref( [=](const auto& t1, const auto& t2) {
        return ( (std::abs(t1.z()-z0) < std::abs(t2.z()-z0)) );
    } );
};

//=============================================================================
// Compare the distance of 2 objects to a plane
//=============================================================================
constexpr auto distanceToPlane = [](const Gaudi::Plane3D& plane) {
    return details::add_deref( [=](const auto& t1, const auto& t2) {
      auto d1 = std::abs(plane.Distance(t1.position()));
      auto d2 = std::abs(plane.Distance(t2.position()));
      return d1 < d2;
    } );
};

//=============================================================================
// Class for sorting class T by z in order (+1/-1)
//=============================================================================
constexpr auto orderByZ = []( int order = +1 ) {
    return details::add_deref( [=](const auto& t1, const auto& t2 )
                       { return order*t1.z() < order*t2.z(); } );
};

//=============================================================================
// Class for sorting class T by increasing z
//=============================================================================
constexpr auto increasingByZ = []() {
    return details::add_deref(
        [](const auto& t1, const auto& t2) { return t1.z() < t2.z() ; }  );
};

//=============================================================================
// Class for sorting class T by decreasing z
//=============================================================================
constexpr auto decreasingByZ = []() {
    return details::add_deref(
        [](const auto& t1, const auto& t2) { return t1.z() > t2.z() ; }  );
};

//=============================================================================
// Helper class for checking the existence of a value of a member function
// example:
// HasKey<Track> isBackward(&Track::checkFlag,Track::Backwards)
// if (isBackward(track)) ...
//=============================================================================
  template <typename T, typename E>
  class HasKey  {
  public:
    typedef bool (T::* ptr_memfun) (E) const;
  private:
    ptr_memfun m_pmf;
    E m_key;
  public:
    HasKey(ptr_memfun check, E key ):
      m_pmf(check),m_key(key) {}
    bool operator()( const T& t ) const
    { return (t.*m_pmf)(m_key); }
    bool operator()( const T* t ) const
    { return (*this)(*t); }
  };

//=============================================================================
// Class to delete an element from a vector
//=============================================================================
  template <typename T>
  void deleteFromList( std::vector<T*>& List, const T* value )
  {
    auto it = std::find( List.begin(), List.end(), value );
    if (it!=List.end()) { delete *it ; List.erase(it); }
  }

//=============================================================================
// Functor to delete an object
//=============================================================================
  struct deleteObject
  {
    template <typename T>
    void operator()(T*& ptr){ delete ptr; ptr=nullptr ; }
  };

//=============================================================================
// Retrieve the reference to the state closest to the given object
// e.g.: closestState( aTrack, TrackFunctor::distanceAlongZ(z) )
//       closestState( aTrack, TrackFunctor::distanceToPlane(aPlane) )
//=============================================================================
  template <typename Fun>
  LHCb::State& closestState( LHCb::Track& track, const Fun& fun )
  {
    auto& allstates = track.states();
    auto iter = std::min_element( allstates.begin(), allstates.end(), std::cref(fun) );
    if ( iter == allstates.end() )
      throw GaudiException( "No states","TrackFunctor.h",
                            StatusCode::FAILURE );
    return *(*iter);
  }

//=============================================================================
// Retrieve the const reference to the state closest to the given object
//=============================================================================
  template <typename Fun>
  const LHCb::State& closestState( const LHCb::Track& track, const Fun& fun )
  {
    const auto& allstates = track.states();
    auto iter = std::min_element( allstates.begin(), allstates.end(), std::cref(fun) );
    if ( iter == allstates.end() )
      throw GaudiException( "No states","TrackFunctor.h",
                            StatusCode::FAILURE );
    return *(*iter);
  }

//=============================================================================
// Retrieve the number of LHCbIDs that fulfill a predicate
//=============================================================================
  template <typename Predicate>
  unsigned int nLHCbIDs( const LHCb::Track& track, const Predicate& pred )
  {
    const auto& ids = track.lhcbIDs();
    return std::count_if( ids.begin(), ids.end(), std::cref(pred) );
  }

//=============================================================================
// Retrieve the number of Measurements that fulfill a predicate
//=============================================================================
  template <typename Container, typename Predicate>
  unsigned int nMeasurements( const Container& meas, const Predicate& pred )
  {
    return std::count_if( meas.begin(), meas.end(), std::cref(pred) );
  }

//=============================================================================
// Retrieve the number of Measurements that fulfill a predicate
//=============================================================================
  template <typename Predicate>
  unsigned int nMeasurements( const LHCb::Track& track, const Predicate& pred )
  {
    const LHCb::TrackFitResult* fit = track.fitResult() ;
    return fit ? nMeasurements( fit->measurements(), std::cref(pred) ) : 0 ;
  }

}

#endif   /// TrackEvent_TrackFunctor_H
