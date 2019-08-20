/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

#include "DetDesc/Condition.h"
#include "DetDesc/ConditionKey.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IInterface.h"
#include "Kernel/STLExtensions.h"
#include "boost/callable_traits.hpp"
#include <functional>
#include <memory>
#include <unordered_map>

class ParamValidDataObject;

namespace LHCb::DetDesc {
  /// Class used to access the conditions accessible to the current transformation.
  using ConditionUpdateContext = std::unordered_map<ConditionKey, ParamValidDataObject const*>;

  /// Type for a user provided callback function.
  /// The first argument is the ConditionKey of the target and is used to be
  /// able to reuse a transformation function that behaves differently depending
  /// on the requested output, The ConditionUpdateContext will be filled with the
  /// input conditions, and the last argument is the Condition instance to update.
  using ConditionCallbackFunction = std::function<void(
      ConditionKey const& /* target */, ConditionUpdateContext const& /* ctx */, Condition& /* output */ )>;

  /// Interface for managers of condition derivations.
  class GAUDI_API IConditionDerivationMgr : virtual public IInterface {
  public:
    /// InterfaceID
    DeclareInterfaceID( IConditionDerivationMgr, 1, 0 );

    /// Identifier of a specific derivation object in the manager.
    using DerivationId = std::size_t;

    /// Special DerivationId value to indicate invalid value.
    static constexpr DerivationId NoDerivation = -1;

    /// Add a condition derivation to the manager.
    virtual DerivationId add( LHCb::span<const ConditionKey> inputs, ConditionKey output,
                              ConditionCallbackFunction func ) = 0;
    DerivationId         add( ConditionKey input, ConditionKey output, ConditionCallbackFunction func ) {
      return add( LHCb::range::single{input}, std::move( output ), std::move( func ) );
    }

    /// Method to find the DerivationId of the derivation responsible of producing
    /// the object with ConditionKey `key`.
    /// If no derivation is registered for that key, the special value NoDerivation
    /// is returned.
    virtual DerivationId derivationFor( const ConditionKey& key ) const = 0;

    /// Retire a specific condition derivation from the manager.
    virtual void remove( DerivationId dId ) = 0;
  };

  namespace detail {
    template <typename Callable>
    inline constexpr auto arity_v = std::tuple_size_v<boost::callable_traits::args_t<Callable>>;

    template <typename Input>
    decltype( auto ) fetch_1( ConditionUpdateContext const& ctx, ConditionKey const& key ) {
      auto i = ctx.find( key );
      if ( i == ctx.end() )
        throw GaudiException{"No object found at " + key, "addConditionDerivation", StatusCode::FAILURE};
      auto* p = dynamic_cast<std::decay_t<Input> const*>( i->second );
      if ( !p )
        throw GaudiException{"The object found at " + key + " is not of the expected type", "addConditionDerivation",
                             StatusCode::FAILURE};
      return *p;
    }

    template <typename TypeList, std::ptrdiff_t N, std::size_t... I>
    auto fetch_n( ConditionUpdateContext const& ctx, span<const ConditionKey, N> keys, std::index_sequence<I...> ) {
      return std::forward_as_tuple( fetch_1<std::tuple_element_t<I, TypeList>>( ctx, keys[I] )... );
    }

    template <typename Transform, std::ptrdiff_t N = detail::arity_v<Transform>>
    auto fetch_inputs_for( ConditionUpdateContext const& ctx, span<const ConditionKey, N> keys ) {
      using InputTypes = boost::callable_traits::args_t<Transform>;
      return fetch_n<InputTypes>( ctx, keys, std::make_index_sequence<N>{} );
    }

  } // namespace detail

  template <typename Transform, size_t N = detail::arity_v<Transform>>
  IConditionDerivationMgr::DerivationId addConditionDerivation( IConditionDerivationMgr&    cdm,
                                                                std::array<ConditionKey, N> inputKeys,
                                                                ConditionKey outputKey, Transform f ) {
    // check if the derivation was already registered
    auto dId = cdm.derivationFor( outputKey );
    if ( dId != IConditionDerivationMgr::NoDerivation ) { return dId; }
    // it was not, so we have to register it now.
    return cdm.add(
        inputKeys, std::move( outputKey ),
        [=, f = std::move( f )]( ConditionKey const&, ConditionUpdateContext const& ctx, Condition& output ) {
          output.payload = std::apply( f, detail::fetch_inputs_for<Transform>( ctx, make_span( inputKeys ) ) );
        } );
  }

  template <typename Transform>
  IConditionDerivationMgr::DerivationId addConditionDerivation( IConditionDerivationMgr& cdm, ConditionKey inputKey,
                                                                ConditionKey outputKey, Transform f ) {

    return addConditionDerivation( cdm, std::array{inputKey}, std::move( outputKey ), std::move( f ) );
  }

} // namespace LHCb::DetDesc
