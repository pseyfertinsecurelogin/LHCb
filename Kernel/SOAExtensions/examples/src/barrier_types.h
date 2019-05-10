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

#ifndef BARRIER_EXAMPLE_TYPES
#define BARRIER_EXAMPLE_TYPES

#include "SOAContainer/SOAField.h"
#include "SOAContainer/SOASkin.h"
#include "SOAContainer/SOAContainer.h"
#include "SOAExtensions/ZipContainer.h"
#include "SOAExtensions/ZipSelection.h"
#include <vector>

namespace Examples {
  SOAFIELD_TRIVIAL( NumberField, number, int );
  SOASKIN_TRIVIAL( NumberSkin, NumberField );

  using NumberContainer = Zipping::ZipContainer<SOA::Container<std::vector, NumberSkin>>;

} // namespace Examples
#endif
