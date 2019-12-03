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
#ifndef DECAYS_TREEPARSER_H
#define DECAYS_TREEPARSER_H 1
// ============================================================================
// Include files
// ============================================================================
#include "GaudiKernel/StatusCode.h"
#include "LoKi/ParserFactory.h"
#include <ostream>
#include <vector>

namespace Decays::Parsers {
  // ========================================================================
  /** parse the decay tree into the helper structue
   *  @param tree   (OUTPUT) the representation of the decay tree
   *  @param input  the input string to be parsed
   *  @param symbols  the list of prededined symbols
   *  @param paricles the list of particles
   *  @param stream   the stream to report the errors
   */
  StatusCode parse( Decays::Parsers::Tree& tree, const std::string& input, const std::vector<std::string>& symbols,
                    const std::vector<std::string>& particles, std::ostream& stream );
  // ========================================================================
} // namespace Decays::Parsers
// ============================================================================
// The END
// ============================================================================
#endif // DECAYS_TREEPARSER_H
