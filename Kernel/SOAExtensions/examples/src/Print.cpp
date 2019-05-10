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

#include "GaudiAlg/Consumer.h"
#include "barrier_types.h"

namespace Examples {
  using BaseClass_t = Gaudi::Functional::Traits::BaseClass_t<::Algorithm>;

  struct PrintInts final
      : Gaudi::Functional::Consumer<void( const NumberContainer&, const Zipping::ExportedSelection<>& ), BaseClass_t> {
    PrintInts( const std::string& name, ISvcLocator* svcLoc )
        : Consumer( name, svcLoc,
                    {
                        KeyValue( "InputLocation", "/Event/MyInt" ),
                        KeyValue( "InputSelection", "/Event/SelectAll" ),
                    } ) {}

    void operator()( const NumberContainer& data, const Zipping::ExportedSelection<>& sel ) const override {
      Zipping::SelectionView input{&data, sel};
      for ( const auto& i : input ) { info() << "found a " << i << endmsg; }
    }
  };

  DECLARE_COMPONENT( PrintInts )
} // namespace Examples
