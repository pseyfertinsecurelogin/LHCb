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

/** @file
 *
 * Consumers that serve purely for the purpose of giving an example of a barrier
 * using SOAExtensions. All algorithms in this file are consumers and print
 * contents of TES locations while taking selections (in the form of ExportedSelection)
 * into account.
 *
 */

#include "GaudiAlg/Consumer.h"
#include "barrier_types.h"

namespace Examples {
  using BaseClass_t = Gaudi::Functional::Traits::BaseClass_t<::Algorithm>;

  /** @class PrintInts Print.cpp
   *
   * @brief Print the selected content of a NumberContainer
   */
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

  /** @class PrintSquaredInts Print.cpp
   *
   * @brief Print the selected content of a SquaredNumberContainer
   */
  struct PrintSquaredInts final
      : Gaudi::Functional::Consumer<void( const SquaredNumberContainer&, const Zipping::ExportedSelection<>& ),
                                    BaseClass_t> {
    PrintSquaredInts( const std::string& name, ISvcLocator* svcLoc )
        : Consumer( name, svcLoc,
                    {
                        KeyValue( "InputLocation", "/Event/MyInt" ),
                        KeyValue( "InputSelection", "/Event/SelectAll" ),
                    } ) {}

    void operator()( const SquaredNumberContainer& data, const Zipping::ExportedSelection<>& sel ) const override {
      Zipping::SelectionView input{&data, sel};
      for ( const auto& i : input ) { info() << "found a " << i << endmsg; }
    }
  };

  DECLARE_COMPONENT( PrintSquaredInts )

  /** @class PrintIntsAndSquaredInts Print.cpp
   *
   * @brief Print the selected content of a (internally zipped) NumberContainer and a SquaredNumberContainer
   */
  struct PrintIntsAndSquaredInts final
      : Gaudi::Functional::Consumer<void( const NumberContainer&, const SquaredNumberContainer&,
                                          const Zipping::ExportedSelection<>& ),
                                    BaseClass_t> {
    PrintIntsAndSquaredInts( const std::string& name, ISvcLocator* svcLoc )
        : Consumer( name, svcLoc,
                    {
                        KeyValue( "InputIntLocation", "/Event/MyInt" ),
                        KeyValue( "InputSquaredIntLocation", "/Event/MyInt" ),
                        KeyValue( "InputSelection", "/Event/SelectAll" ),
                    } ) {}

    void operator()( const NumberContainer& data1, const SquaredNumberContainer& data2,
                     const Zipping::ExportedSelection<>& sel ) const override {
      auto                   data = Zipping::semantic_zip<NumberAndSquaredNumberSkin>( data1, data2 );
      Zipping::SelectionView input{&data, sel};
      for ( const auto& i : input ) { info() << "found a " << i << endmsg; }
    }
  };

  DECLARE_COMPONENT( PrintIntsAndSquaredInts )
} // namespace Examples