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
/** @class TESMerger TESMerger.h
 *
 *  Merge different track lists.
 *
 *  @author Sean Benson
 *  @date   24/02/2014
 */
#include "Event/Particle.h"
#include "Event/ProtoParticle.h"
#include "Event/Track.h"
#include "GaudiAlg/MergingTransformer.h"
#include "GaudiKernel/detected.h"
#include <algorithm>
#include <numeric>
#include <type_traits>

namespace details
{

  template <typename T>
  using has_reserve_ = decltype( std::declval<T&>().reserve( typename T::size_type( 0 ) ) );
  template <typename T>
  constexpr bool has_reserve_v = Gaudi::cpp17::is_detected<has_reserve_, T>::value;

  template <typename T>
  using has_clone_ = decltype( std::declval<const T&>().clone() );
  template <typename T>
  constexpr bool has_clone_v = Gaudi::cpp17::is_detected<has_clone_, T>::value;

  constexpr struct Clone_t {
    template <typename T>
    auto* operator()( T const& t ) const
    {
      if constexpr( std::is_pointer_v<T> ) {
        assert( t != nullptr );
        return operator()( *t );
      } else if constexpr( details::has_clone_v<T> ) {
        return t.clone();
      } else {
        return new T( t );
      }
    }
  } clone{};

  // TODO: move to Gaudi::Functional::details
  template <typename C>
  class Inserter
  {
    C& container;

  public:
    Inserter( C& c ) : container( c ) {}
    template <typename ValueType>
    Inserter& operator=( ValueType&& value )
    {
      // adapt between containers which use 'insert' (eg. KeyedContainer)
      // and those which use 'push_back' (eg. std::vector)
      Gaudi::Functional::details::insert( container, std::forward<ValueType>( value ) );
      return *this;
    }
    Inserter& operator*() { return *this; }
    Inserter& operator++() { return *this; }
    Inserter& operator++( int ) { return *this; }
  };
}

template <typename T>
using VOC = Gaudi::Functional::vector_of_const_<T>;

template <class Container>
struct TESMerger final : Gaudi::Functional::MergingTransformer<Container( VOC<Container*> const& )> {
  TESMerger( std::string const& name, ISvcLocator* pSvcLocator )
      : Gaudi::Functional::MergingTransformer<Container( VOC<Container*> const& )>(
            name, pSvcLocator, {"inputLocations", {}}, {"outputLocation", {}} )
  {
  }

  Container operator()( VOC<Container*> const& vcont ) const override
  {
    Container out;
    if constexpr( details::has_reserve_v<Container> ) {
      auto n = std::accumulate( vcont.begin(), vcont.end(), 0,
                                []( int n, auto const* c ) { return c ? n + c->size() : n; } );
      out.reserve( n );
    }
    for ( auto const* container : vcont ) {
      if ( container ) std::transform( container->begin(), container->end(),
                                       details::Inserter{out}, details::clone );
    }
    return out;
  }
};

template <typename ValueType>
using KC = KeyedContainer<ValueType, Containers::HashMap>;

DECLARE_COMPONENT_WITH_ID( TESMerger<KC<LHCb::ProtoParticle>>, "TESMergerProtoParticle" )
DECLARE_COMPONENT_WITH_ID( TESMerger<KC<LHCb::Particle>>, "TESMergerParticle" )
DECLARE_COMPONENT_WITH_ID( TESMerger<KC<LHCb::Track>>, "TESMergerTrack" )
