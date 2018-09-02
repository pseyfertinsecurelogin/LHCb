// @author Niklas Nolte
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/SharedObjectsContainer.h"
#include "Event/Track.h"
#include "GaudiAlg/MergingTransformer.h"
#include "GaudiAlg/Transformer.h"
#include "GaudiAlg/SplittingTransformer.h"

#include <array>

/*  Barrier algorithms:

    - The Gatherer takes multiple optional input-containers (LHCb::Tracks for instance) (selections from
      different lines) and puts them into one big vector

    - The Merger creates the union of all those containers to have one container where some shared algorithm
      can loop over (implemented for Keyed(SharedObject)Containers and Masks (with integral types of values 0
      and 1)

    - The next Barrier step is some shared algorithm, taking the output of the merger (and in case of masks
      some other data)  as input

    - The scatterer takes the output of the shared algorithm + the gatherer output + merger output how to
      scatter the inputs to the different lines again

*/

template <typename T> using VOC = Gaudi::Functional::vector_of_const_<T>;
template<typename T> using KC = KeyedContainer<T, Containers::HashMap>;
template<typename T> using SOC = SharedObjectsContainer<T>;
using KeyValue = std::pair<std::string, std::string>;

//cType = container type that the gatherer gets as input
//vcType is the vector of those containers
template <typename cType, typename vcType = VOC<cType>>
struct BarrierGatherer;


//Input and output of the merger
template <typename In, typename Out = VOC<In>>
struct BarrierMerger;

//scatteredOut = type of output of the scatterer ( container of container of transformed things )
//transformedIn = output of the transformer ( container of transformed things )
//gatheredIn = output of Gatherer ( container of container of things )
template<typename scatteredOut, typename transformedIn, typename gatheredIn = scatteredOut>
struct BarrierScatterer;



template <typename cType>
struct BarrierGatherer<VOC<cType*>, VOC<cType*>> final
    : Gaudi::Functional::MergingTransformer<VOC<cType*>(VOC<cType*> const &)> {

  BarrierGatherer(std::string const &name, ISvcLocator *pSvcLocator)
      : Gaudi::Functional::MergingTransformer<VOC<cType*>(VOC<cType*> const &)>(
            name, pSvcLocator,
            {"InputLocations", {LHCb::TrackLocation::Velo}},
            {"OutputLocation", {"Rec/Track/GatheredTracks"}}) {}

  VOC<cType*> operator()(VOC<cType*> const &vec) const override {
    return vec;
  }
};


template<typename Type>
struct BarrierMerger<SOC<Type>, VOC<SOC<Type>*>> final : public Gaudi::Functional::Transformer<SOC<Type>(VOC<SOC<Type>*> const &)> {

  BarrierMerger(std::string const &name, ISvcLocator *pSvcLocator)
      : Gaudi::Functional::Transformer<SOC<Type>(VOC<SOC<Type>*> const &)>(
            name, pSvcLocator, {"InputLocation", "Rec/Track/GatheredTracks"},
            {"OutputLocation", "Rec/Track/MergedTracks"}) {}

  SOC<Type> operator()(VOC<SOC<Type>*> const &vec) const override {
    SOC<Type> out;
    for (SOC<Type> const *container : vec) {
      if(container == nullptr) continue;
      for (auto *thing : *container) {
        if (std::find(std::begin(out), std::end(out), thing) == std::end(out)) {
          out.insert(thing);
        }
      }
    }
    return out;
  }
};


template <typename OutType, typename InType, typename TransformedCType>
struct BarrierScatterer<std::vector<SOC<OutType>>, TransformedCType, VOC<SOC<InType> *>> final
    : public Gaudi::Functional::SplittingTransformer<std::vector<SOC<OutType>>(
          TransformedCType const &, VOC<SOC<InType> *> const &)> {
  BarrierScatterer(std::string const &name, ISvcLocator *pSvcLocator)
      : Gaudi::Functional::SplittingTransformer<std::vector<SOC<OutType>>(
            TransformedCType const &, VOC<SOC<InType> *> const &)>(
            name, pSvcLocator,
            std::array<KeyValue, 2>{KeyValue{"transformedInput", "Rec/Track/ForwardFastFitted"},
                                    KeyValue{"gatheredInput", "Rec/Track/GatheredTracks"}},
            {"OutputLocations", {""}}) {}

  std::vector<SOC<OutType>> operator()(TransformedCType const &transformedIn,
                                       VOC<SOC<InType> *> const &gatheredIn) const override {
    std::vector<SOC<OutType>> Out{gatheredIn.size()};
    for (std::size_t i = 0; i < gatheredIn.size(); ++i) {
      if (gatheredIn[i] == nullptr) {
        continue;
      }
      for (auto *obj : *gatheredIn[i]) {
        Out[i].insert(transformedIn(gatheredIn[i]->index(obj)));
      }

      if ( this->msgLevel(MSG::DEBUG) ) {
        std::cout << Out[i].size() << std::endl;
      }
    }
    return Out;
  }
};


// need this shit because the macro fucks up the type otherwise
using BarrierGatherer_LHCbSelection = BarrierGatherer<VOC<LHCb::Track::Selection*>, VOC<LHCb::Track::Selection*>>;
using BarrierMerger_LHCbSelection = BarrierMerger<LHCb::Track::Selection, VOC<LHCb::Track::Selection*>>;
using BarrierScatterer_LHCbSelection = BarrierScatterer<std::vector<LHCb::Track::Selection>, LHCb::Tracks, VOC<LHCb::Track::Selection*>>;

DECLARE_COMPONENT_WITH_ID(BarrierGatherer_LHCbSelection, "TrackVectorGatherer")
DECLARE_COMPONENT_WITH_ID(BarrierMerger_LHCbSelection, "TrackMergerTransformer")
DECLARE_COMPONENT_WITH_ID(BarrierScatterer_LHCbSelection, "TrackVectorScatterer")
