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
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/System.h"
#include "Kernel/STLExtensions.h"
#include "boost/callable_traits.hpp"
#include <any>
#include <functional>
#include <memory>
#include <type_traits>
#include <unordered_map>

class ValidDataObject;

namespace LHCb::DetDesc {
  namespace details {
    template <typename Transform>
    struct construct_constructor;

    template <typename Ret, typename... Args>
    struct construct_constructor<Ret( Args const&... )> {
      static_assert( std::is_constructible_v<Ret, Args const&...> );
      [[nodiscard]] static constexpr auto
      construct( Args const&... args ) noexcept( std::is_nothrow_constructible_v<Ret, Args const&...> ) {
        return Ret{args...};
      }
    };

    template <typename Transform>
    constexpr auto invoke_constructor = construct_constructor<Transform>::construct;

  } // namespace details
  /// Class used to access the conditions accessible to the current transformation.
  using ConditionUpdateContext = std::unordered_map<ConditionKey, ValidDataObject const*>;

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
    using result_of_t = boost::callable_traits::return_type_t<Callable>;

    template <typename Callable>
    inline constexpr auto arity_v = std::tuple_size_v<boost::callable_traits::args_t<Callable>>;

    inline void throwTypeMismatch( std::type_info const& FOUND,    //
                                   std::type_info const& EXPECTED, //
                                   ConditionKey const&   key ) {
      throw GaudiException{"Key='" + key + "' type mis-match. Found='" + System::typeinfoName( FOUND ) +
                               "' Expected='" + System::typeinfoName( EXPECTED ) + "'",
                           "addConditionDerivation", StatusCode::FAILURE};
    }

    template <typename OUT, typename IN>
    inline decltype( auto ) castAndCheck( IN& in, ConditionKey const& key ) {
      if constexpr ( std::is_same_v<std::any, std::decay_t<IN>> ) {
        const auto* p = std::any_cast<OUT>( &in );
        if ( UNLIKELY( !p ) ) { throwTypeMismatch( in.type(), typeid( OUT ), key ); }
        return *p;
      } else {
        const auto* p = dynamic_cast<OUT>( in );
        if ( UNLIKELY( !p ) ) { throwTypeMismatch( typeid( decltype( in ) ), typeid( OUT ), key ); }
        return *p;
      }
    }

    template <typename Input>
    decltype( auto ) fetch_1( ConditionUpdateContext const& ctx, ConditionKey const& key ) {
      auto i = ctx.find( key );
      if ( UNLIKELY( i == ctx.end() ) ) {
        throw GaudiException{"No object found at " + key, "addConditionDerivation", StatusCode::FAILURE};
      }
      using Exp = std::decay_t<Input>;
      if constexpr ( std::is_base_of_v<ValidDataObject, Exp> ) {
        return castAndCheck<std::add_pointer_t<typename std::add_const_t<Exp>>>( i->second, key );
      } else {
        const auto& p = castAndCheck<ParamValidDataObject const*>( i->second, key );
        return castAndCheck<Exp>( p.payload, key );
      }
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
  IConditionDerivationMgr::DerivationId
  addConditionDerivation( IConditionDerivationMgr& cdm, std::array<ConditionKey, N> inputKeys, ConditionKey outputKey,
                          Transform f = details::invoke_constructor<Transform> ) {
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
