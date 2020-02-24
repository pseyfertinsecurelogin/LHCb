/*
 * Copyright (C) 2020  CERN for the benefit of the LHCb collaboration
 * Author: Paul Seyfert <pseyfert@cern.ch>
 *
 * This software is distributed under the terms of the GNU General Public
 * Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
 *
 * In applying this licence, CERN does not waive the privileges and immunities
 * granted to it by virtue of its status as an Intergovernmental Organization
 * or submit itself to any jurisdiction.
 */

#include "GaudiAlg/Transformer.h"
#include "SOAExtensions/ZipAlgorithms.h"
#include "barrier_types.h"

namespace Examples {
  using BaseClass_t = Gaudi::Functional::Traits::BaseClass_t<::Algorithm>;

  struct ZipBarrierExampleWorker final
      : Gaudi::Functional::Transformer<
            SquaredNumberContainer( const NumberContainer&, const Zipping::ExportedSelection<>& ), BaseClass_t> {
    ZipBarrierExampleWorker( const std::string& name, ISvcLocator* svcLoc )
        : Transformer( name, svcLoc,
                       {
                           KeyValue( "InputLocation", "/Event/MyInt" ),
                           KeyValue( "InputSelection", "/Event/MySelection" ),
                       },
                       KeyValue( "OutputLocation", "/Event/MyTransformedInts" ) ) {}

    SquaredNumberContainer operator()( const NumberContainer&              data,
                                       const Zipping::ExportedSelection<>& sel ) const override {

      // squaring a number is a (silly) example for a computationally expensive task that we want to perform only for
      // selected inputs. The output type (SquaredNumber can be constructed from an int), but one still needs to
      // retrieve the `number` data member from the `Number` proxy.
      auto expensive_operation = []( auto input ) { return input.number() * input.number(); };
      return Zipping::transform<SquaredNumberSkin>( data, std::move( expensive_operation ), sel );
    }
  };

  DECLARE_COMPONENT( ZipBarrierExampleWorker )
} // namespace Examples
