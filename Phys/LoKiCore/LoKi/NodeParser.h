/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// ============================================================================
#ifndef DECAYS_NODEPARSER_H
#define DECAYS_NODEPARSER_H 1
// ============================================================================
// Include files
// ============================================================================
#include "GaudiKernel/StatusCode.h"
#include "Kernel/Nodes.h"
#include <ostream>
#include <vector>
namespace Decays {
  // ==========================================================================
  namespace Parsers {
    // ========================================================================
    /** parse the decay node
     *  @param node      (OUTPUT) the decay node
     *  @param input     (INPUT)  the input string to be parsed
     *  @param symbols   (INPUT)  the list of known predefiend symbols
     *  @param particles (INPUT)  the list of particle
     *  @param stream    the sterem to report the errors
     */
    StatusCode parse( Decays::Node& node, const std::string& input, const std::vector<std::string>& symbols,
                      const std::vector<std::string>& particles, std::ostream& stream );
    // ========================================================================
  } // namespace Parsers
  // ==========================================================================
} // end of namespace Decays
// ============================================================================
// The END
// ============================================================================
#endif // DECAYS_NODEPARSER_H
