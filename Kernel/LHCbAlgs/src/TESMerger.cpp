/** @class TESMerger TESMerger.h
 *
 *  Merge different track lists.
 *
 *  @author Sean Benson
 *  @date   24/02/2014
 */
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

    template <typename T>
    T* clone(T const& t) {
        if constexpr ( details::has_clone_v<T> ) {
            return t.clone();
        } else {
            return new T(t);
        }
    }
    // adapt between containers which use 'insert' (eg. KeyedContainer)
    // and those which use 'push_back' (eg. std::vector)
    using Gaudi::Functional::details::insert;
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
          if (!container) continue;
          for (auto const* obj : *container) {
              details::insert( out, details::clone(*obj) );
          }
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
