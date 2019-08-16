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

#include "DetDesc/ConditionKey.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IInterface.h"
#include "Kernel/STLExtensions.h"
#include <functional>
#include <memory>
#include <unordered_map>

#include "DetDesc/Condition.h"

class ParamValidDataObject;

namespace LHCb::DetDesc {
  /// Class used to access the conditions accessible to the current transformation.
  using ConditionUpdateContext = std::unordered_map<ConditionKey, ParamValidDataObject const*>;

  /// Type for a user provided callback function.
  /// The first argument is the ConditionKey of the target and is used to be
  /// able to reuse a transformation function that behaves differently depending
  /// on the requested output, The ConditionUpdateContext will be filled with the
  /// input conditions, and the last argument is the Condition instance to update.
  using ConditionCallbackFunction = std::function<void( const ConditionKey& /* target */,
                                                        ConditionUpdateContext& /* ctx */, Condition& /* output */ )>;

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
    template <typename Out>
    struct Convert {
      template <typename... Arg>
      Out operator()( Arg&&... arg ) const {
        return Out{std::forward<Arg>( arg )...};
      }
    };

    template <typename Out> inline constexpr auto to_ = Convert<Out>{};

  } // namespace detail

  template <typename OutputType, typename InputType = ParamValidDataObject, typename Transform>
  IConditionDerivationMgr::DerivationId registerDerivation( IConditionDerivationMgr& cdm, ConditionKey outputKey,
                                                            ConditionKey inputKey,
                                                            Transform&&  f = detail::to_<OutputType> ) {

    // check if the derivation was already registered
    auto dId = cdm.derivationFor( outputKey );
    if ( dId != IConditionDerivationMgr::NoDerivation ) { return dId; }
    // it was not, so we have to register it now.
    auto adapter = [f = std::forward<Transform>( f ), key = inputKey](
                       const ConditionKey& /* target */, ConditionUpdateContext& ctx, Condition& output ) {
      const auto input = dynamic_cast<const InputType*>( ctx[key] );
      if ( !input )
        throw GaudiException{"The object at " + key + " is either the wrong type or not present", "registerDerivation",
                             StatusCode::FAILURE};
      output.payload = f( *input );
    };
    // we declare a dependency on the detector and the conditions to make sure the call back
    // is called when there is a change we care about (even if in the code we seem to look
    // only at the detector)
    return cdm.add( std::move( inputKey ), std::move( outputKey ), std::move( adapter ) );
  }

} // namespace LHCb::DetDesc
