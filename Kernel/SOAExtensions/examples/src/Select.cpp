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

#include "GaudiAlg/Transformer.h"
#include "barrier_types.h"

namespace Examples {
  using BaseClass_t = Gaudi::Functional::Traits::BaseClass_t<::Algorithm>;

  struct ZipBarrierExampleSelector final
      : Gaudi::Functional::Transformer<
            Zipping::ExportedSelection<>( const NumberContainer&, const Zipping::ExportedSelection<>& ), BaseClass_t> {
    ZipBarrierExampleSelector( const std::string& name, ISvcLocator* svcLoc )
        : Transformer( name, svcLoc,
                       {
                           KeyValue( "InputLocation", "/Event/MyInt" ),
                           KeyValue( "InputSelection", "/Event/SelectAll" ),
                       },
                       KeyValue( "OutputLocation", "/Event/MySelection" ) ) {}

    Gaudi::Property<int> m_divisor{this, "Divisor", 2};

    Zipping::ExportedSelection<> operator()( const NumberContainer&              data,
                                             const Zipping::ExportedSelection<>& sel ) const override {
      Zipping::SelectionView input{&data, sel};

      return input.select( [this]( NumberContainer::proxy i ) { return ( i.number() % m_divisor ) == 0; } );
    }
  };

  DECLARE_COMPONENT( ZipBarrierExampleSelector )
} // namespace Examples
