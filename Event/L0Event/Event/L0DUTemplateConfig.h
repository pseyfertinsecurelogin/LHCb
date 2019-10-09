/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

// Include files
#include <string>

// Forward declarations

/** @namespace L0DUTemplateConfig
 *
 * L0DU template configuration attributes
 *
 * @author unknown
 *
 */

namespace LHCb {
  // Forward declarations
  namespace L0DUTemplateConfig {
    static const std::string Name     = "Template"; ///< default name of the Template configuration
    static const long        TCKValue = 0x10000;    ///< fake TCK value of the Template configuration

  } // namespace L0DUTemplateConfig
} // namespace LHCb

// -----------------------------------------------------------------------------
// end of namespace
// -----------------------------------------------------------------------------

// Including forward declarations
