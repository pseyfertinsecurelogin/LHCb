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
#ifndef LINKER_H
#define LINKER_H

#include <GaudiKernel/KeyedContainer.h>
#include <GaudiAlg/Transformer.h>
#include <Event/LinksByKey.h>

#include "Detail/Mixin.h"

namespace Gaudi {
namespace Functional {

template <typename Signature, typename Traits_ = Traits::useDefaults> class Linker;

// A base class to link reconstructed objects to MC objects to easy
// the transition from LinkerWithKey to a future event model for the
// MC.
// NOTE: Several different object containers are used by recontructed
// and MC objects, such as KeyedContainer and ObjectVector, these do
// not contain the same public typedefs, so some specialization tricks
// are needed to get access to the target and source types.
template <class Out,
          template <typename, typename...> class SourceContainer, typename Source, typename... SourceContainerArgs,
          typename... ExtraIn, typename Traits_>
class Linker<Out(const SourceContainer<Source, SourceContainerArgs...>&, const ExtraIn&...), Traits_>
: public Detail::LinkerMixin<Source>,
  public Transformer<Out(const SourceContainer<Source, SourceContainerArgs...>&, const ExtraIn&...), Traits_>
{
public:

   // Forwarding contructor saves a lot of hassle
   using Transformer<Out(const SourceContainer<Source, SourceContainerArgs...>&, const ExtraIn&...),
                     Traits_>::Transformer;

};

template <typename Signature, typename Traits_ = Traits::useDefaults> class MultiLinker;

// A base class to link reconstructed objects to MC objects to easy
// the transition from LinkerWithKey to a future event model for the
// MC.
// NOTE: Several different object containers are used by recontructed
// and MC objects, such as KeyedContainer and ObjectVector, these do
// not contain the same public typedefs, so some specialization tricks
// are needed to get access to the target and source types.
template <typename... Out,
          template <typename, typename...> class SourceContainer, typename Source, typename... SourceContainerArgs,
          typename... ExtraIn, typename Traits_>
class MultiLinker<std::tuple<Out...>(const SourceContainer<Source, SourceContainerArgs...>&, const ExtraIn&...), Traits_>
: public Detail::LinkerMixin<Source>,
  public MultiTransformer<std::tuple<Out...>(const SourceContainer<Source, SourceContainerArgs...>&, const ExtraIn&...), Traits_>
{
public:

   // Forwarding contructor saves a lot of hassle
   using MultiTransformer<std::tuple<Out...>(const SourceContainer<Source, SourceContainerArgs...>&, const ExtraIn&...),
                          Traits_>::MultiTransformer;

};

}
}

#endif
