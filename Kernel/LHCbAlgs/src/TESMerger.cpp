/** @class TESMerger TESMerger.h
 *
 *  Merge different track lists.
 *
 *  @author Sean Benson
 *  @date   24/02/2014
 */
#include <algorithm>
#include <type_traits>
#include "Event/Track.h"
#include "Event/ProtoParticle.h"
#include "Event/Particle.h"
#include "GaudiAlg/MergingTransformer.h"
#include "GaudiKernel/detected.h"

namespace details {

  template <typename T, typename = int>
  using has_clone_ = decltype( std::declval<const T&>().clone() );
  template <typename T>
  constexpr bool has_clone_v = Gaudi::cpp17::is_detected<has_clone_, T>::value;

  constexpr struct Clone_t {
    template <typename T>
    auto operator()(T const& t) const {
      if constexpr ( std::is_pointer_v<T> ) {
        assert(t!=nullptr);
        return operator()(*t);
      } else if constexpr ( details::has_clone_v<T> ) {
        return t.clone();
      } else {
        return new T(t);
      }
    }
  } clone{};

  // TODO: move to Gaudi::Functional::details
  template <typename C>
  class Inserter {
    C& container;
  public:
    Inserter(C& c) : container(c) {}
    template <typename ValueType>
    Inserter& operator=( ValueType&& value ) {
      // adapt between containers which use 'insert' (eg. KeyedContainer)
      // and those which use 'push_back' (eg. std::vector)
      Gaudi::Functional::details::insert(container,std::forward<ValueType>(value));
      return *this;
    }
    Inserter& operator*() { return *this; }
    Inserter& operator++() { return *this; }
    Inserter& operator++(int) { return *this; }
  };

}

template <typename T> using VOC = Gaudi::Functional::vector_of_const_<T>;

template <class Container>
struct TESMerger final : Gaudi::Functional::MergingTransformer<Container(VOC<Container*> const &)>
{
  TESMerger(std::string const& name, ISvcLocator* pSvcLocator)
  : Gaudi::Functional::MergingTransformer<Container(VOC<Container*>const&)>(name,pSvcLocator,
                       { "inputLocations", {}},
                       { "outputLocation", {}} )
  {}

  Container operator()(VOC<Container*>const& vcont) const override {
    Container out;
    for (auto const* container : vcont) {
      if (container) std::transform( container->begin(), container->end(),
                                     details::Inserter{out}, details::clone );
    }
    return out;
  }
};

template <typename ValueType> using KC = KeyedContainer< ValueType , Containers::HashMap >;

using TESMergerProtoParticle = TESMerger<KC<LHCb::ProtoParticle>>;
DECLARE_COMPONENT_WITH_ID( TESMergerProtoParticle, "TESMergerProtoParticle" )
using TESMergerParticle = TESMerger<KC<LHCb::Particle>>;
DECLARE_COMPONENT_WITH_ID( TESMergerParticle , "TESMergerParticle" )
using TESMergerTrack = TESMerger<KC<LHCb::Track>>;
DECLARE_COMPONENT_WITH_ID( TESMergerTrack , "TESMergerTrack" )
