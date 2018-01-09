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

//=============================================================================
// Class to test if the z of a class T is < than a certain value
//=============================================================================
  class less_z final {
    double m_z;
  public:
    explicit less_z( double z ):m_z(z) {}
    template <typename T, typename = std::enable_if_t<!std::is_pointer<T>::value>>
    bool operator()( const T& t ) const
    { return t.z() < m_z; }
    template <typename T> bool operator()( const T* t ) const
    { return (*this)(*t); }
  };

//=============================================================================
// Class to test if the z of a class T is > than a certain value
//=============================================================================
  class greater_z final  {
    double m_z;
  public:
    explicit greater_z( double z ):m_z(z) {}
    template <typename T, typename = std::enable_if_t<!std::is_pointer<T>::value>>
    bool operator()( const T& t ) const
    { return t.z() > m_z; }
    template <typename T> bool operator()( const T* t1, const T* t2 ) const
    { return operator()(*t1,*t2); }
  };

//=============================================================================
// Compare the distance along z to the specified z-value, of 2 objects
//=============================================================================
  class distanceAlongZ final  {
    double m_z0;
  public:
    explicit distanceAlongZ( double z0 = 0.):m_z0(z0) {}
    template <typename T, typename = std::enable_if_t<!std::is_pointer<T>::value>>
    bool operator()( const T& t1, const T& t2 ) const
    { return ( (std::abs(t1.z()-m_z0) < std::abs(t2.z()-m_z0)) ); }
    template <typename T> bool operator()( const T* t1, const T* t2 ) const
    { return operator()(*t1,*t2); }
  };

//=============================================================================
// Compare the distance of 2 objects to a plane
//=============================================================================
  class distanceToPlane final {
    Gaudi::Plane3D m_plane;
  public:
    explicit distanceToPlane(const Gaudi::Plane3D& plane):m_plane(plane) {}
    template <typename T, typename = std::enable_if_t<!std::is_pointer<T>::value>>
    bool operator()( const T& t1, const T& t2 ) const
    {
      auto d1 = std::abs(m_plane.Distance(t1.position()));
      auto d2 = std::abs(m_plane.Distance(t2.position()));
      return d1 < d2;
    }
    template <typename T> bool operator()( const T* t1, const T* t2 ) const
    { return operator()(*t1,*t2); }
  };

//=============================================================================
// Class for sorting class T by z in order (+1/-1)
//=============================================================================
  class orderByZ final{
    int m_order;
  public:
    explicit orderByZ( int order = +1):m_order(order) {}
    template <typename T, typename = std::enable_if_t<!std::is_pointer<T>::value>>
    bool operator()( const T& t1, const T& t2 ) const
    { return m_order*t1.z() < m_order*t2.z(); }
    template <typename T> bool operator()( const T* t1, const T* t2 ) const
    { return operator()(*t1,*t2); }
  };


//=============================================================================
// Class for sorting class T by increasing z
//=============================================================================
  struct increasingByZ final {
    template <typename T, typename = std::enable_if_t<!std::is_pointer<T>::value>>
    bool operator()( const T& t1, const T& t2 ) const
    { return t1.z() < t2.z(); }
    template <typename T> bool operator()( const T* t1, const T* t2 ) const
    { return operator()(*t1,*t2); }
  };

//=============================================================================
// Class for sorting class T by decreasing z
//=============================================================================
  struct decreasingByZ final {
    template <typename T, typename = std::enable_if_t<!std::is_pointer<T>::value>>
    bool operator()( const T& t1, const T& t2 ) const
    { return t1.z() > t2.z(); }
    template <typename T> bool operator()( const T* t1, const T* t2 ) const
    { return operator()(*t1,*t2); }
  };

//=============================================================================
// Helper class for checking the existence of a value of a member function
//=============================================================================
  template <typename T, typename E>
  class HasKey  {
  public:
    // A predicate (unary bool function):
    // example:
    // HasKey<Track> isBackward(&Track::checkFlag,Track::Backwards)
    // if (isBackward(track)) ...
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
    auto iter = std::min_element( allstates.begin(), allstates.end(), fun );
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
    auto iter = std::min_element( allstates.begin(), allstates.end(), fun );
    if ( iter == allstates.end() )
      throw GaudiException( "No states","TrackFunctor.h",
                            StatusCode::FAILURE );
    return *(*iter);
  }

//=============================================================================
// Retrieve the number of LHCbIDs that fulfill a predicate
// (using e.g. the HasKey template in TrackKeys.h)
//=============================================================================
  template <typename Predicate>
  unsigned int nLHCbIDs( const LHCb::Track& track, const Predicate& pred )
  {
    const auto& ids = track.lhcbIDs();
    return std::count_if( ids.begin(), ids.end(), std::cref(pred) );
  }

//=============================================================================
// Retrieve the number of Measurements that fulfill a predicate
// (using e.g. the HasKey template in TrackKeys.h)
//=============================================================================
  template <typename Container, typename Predicate>
  unsigned int nMeasurements( const Container& meas, const Predicate& pred )
  {
    return std::count_if( meas.begin(), meas.end(), std::cref(pred) );
  }

//=============================================================================
// Retrieve the number of Measurements that fulfill a predicate
// (using e.g. the HasKey template in TrackKeys.h)
//=============================================================================
  template <typename Predicate>
  unsigned int nMeasurements( const LHCb::Track& track, const Predicate& pred )
  {
    const LHCb::TrackFitResult* fit = track.fitResult() ;
    return fit ? nMeasurements( fit->measurements(), std::cref(pred) ) : 0 ;
  }

}

#endif   /// TrackEvent_TrackFunctor_H
