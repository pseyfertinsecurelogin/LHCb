/*
 * Copyright (C) 2019  CERN for the benefit of the LHCb collaboration
 * Author: Paul Seyfert <pseyfert@cern.ch>
 *
 * This software is distributed under the terms of the GNU General Public
 * Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
 *
 * In applying this licence, CERN does not waive the privileges and immunities
 * granted to it by virtue of its status as an Intergovernmental Organization
 * or submit itself to any jurisdiction.
 */

#include "GaudiAlg/Producer.h"
#include "barrier_types.h"

namespace Examples {
  using BaseClass_t = Gaudi::Functional::Traits::BaseClass_t<::Algorithm>;

  struct ZipBarrierExampleProducer final
      : Gaudi::Functional::Producer<std::tuple<NumberContainer, Zipping::ExportedSelection<>>(), BaseClass_t> {

    ZipBarrierExampleProducer( const std::string& name, ISvcLocator* svcLoc )
        : Producer(
              name, svcLoc,
              {KeyValue( "OutputLocation", "/Event/MyVector" ), KeyValue( "OutputSelection", "/Event/SelectAll" )} ) {}

    std::tuple<NumberContainer, Zipping::ExportedSelection<>> operator()() const override {
      NumberContainer retval;
      for ( int i = 1; i < 14; ++i ) retval.push_back( i );
      Zipping::ExportedSelection<> trivial_selection( retval, Zipping::details::alwaysTrue );
      return {std::move( retval ), std::move( trivial_selection )};
    }
  };

  DECLARE_COMPONENT( ZipBarrierExampleProducer )
} // namespace Examples
