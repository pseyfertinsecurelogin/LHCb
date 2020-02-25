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
 * Data types that are used in the example how to use SOAExtensions for a barrier.
 *
 */

#ifndef BARRIER_EXAMPLE_TYPES
#define BARRIER_EXAMPLE_TYPES

#include "SOAContainer/SOAContainer.h"
#include "SOAContainer/SOAField.h"
#include "SOAContainer/SOASkin.h"
#include "SOAExtensions/ZipContainer.h"
#include "SOAExtensions/ZipSelection.h"
#include <vector>

namespace Examples {
  /*
   * @class NumberField
   *
   * data type that is stored in a NumberContainer.
   *
   * @brief an alias for int for the type system (could be a track in a real LHCb example)
   */
  SOAFIELD_TRIVIAL( NumberField, number, int );
  /*
   * @class NumberSkin
   *
   * @brief  data type that defines the proxy API of a NumberContainer.
   */
  SOASKIN_TRIVIAL( NumberSkin, NumberField );
  /*
   * @class SquaredNumberField
   *
   * data type that is stored in a SquaredNumberContainer.
   *
   * @brief an alias for int for the type system (could be a MUON PID in a real LHCb example)
   */
  SOAFIELD_TRIVIAL( SquaredNumberField, squarednumber, int );
  /*
   * @class SquaredNumberSkin
   *
   * @brief data type that defines the proxy API of a SquaredNumberContainer.
   */
  SOASKIN_TRIVIAL( SquaredNumberSkin, SquaredNumberField );
  /*
   * @class NumberAndSquaredNumberSkin
   *
   * @brief data type that defines the proxy API of a NumberAndSquaredNumberContainer.
   */
  SOASKIN_TRIVIAL( NumberAndSquaredNumberSkin, NumberField, SquaredNumberField );

  /*
   * @class NumberContainer
   *
   * @brief container of NumberField objects in the SOAExtensions framework
   */
  using NumberContainer = Zipping::ZipContainer<SOA::Container<std::vector, NumberSkin>>;
  /*
   * @class SquaredNumberContainer
   *
   * @brief container of SquaredNumberField objects in the SOAExtensions framework
   */
  using SquaredNumberContainer = Zipping::ZipContainer<SOA::Container<std::vector, SquaredNumberSkin>>;
  /*
   * @class NumberAndSquaredNumberContainer
   *
   * @brief zip of a NumberContainer and a SquaredNumberContainer
   */
  using NumberAndSquaredNumberContainer = Zipping::ZipContainer<SOA::Container<std::vector, SquaredNumberSkin>>;

} // namespace Examples
#endif
