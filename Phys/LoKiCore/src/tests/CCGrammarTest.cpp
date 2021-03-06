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
// Include files
// ============================================================================
// STT & STL
// ============================================================================
#include <iostream>
#include <string>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/StatusCode.h"
// ============================================================================
/** @file
 *  Simple application to test CC-utilities parsers
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2009-05-12
 */
// ============================================================================
namespace {
  std::string cc( const std::string& input ) { return " [ " + input + " , #" + input + "#] "; }
  // ==========================================================================
  std::string ccfy( std::string result ) {

    // Current position at result
    int pos = result.length() - 1;

    int backpos = 0; // Position from back of string
    int level   = 0; // Current nesting level  INSIDE [...]cc construction
    int end     = 0; // Position before the ]cc

    for ( ; pos >= 0; pos--, backpos++ ) {

      if ( result[pos] == ']' ) {
        if ( ( backpos > 1 ) && ( result[pos + 1] == 'c' ) && ( result[pos + 2] == 'c' ) && ( end == 0 ) ) {
          end = pos - 1;
        } else if ( end != 0 ) {
          // We count only brackets inside [...]cc construction
          level++;
        }
        continue;
      }

      if ( result[pos] == '[' ) {
        if ( end != 0 ) { // At the current cycle we also do not interested at [...]cc construction nested at another
                          // [...]cc
          if ( level == 0 ) { // The start of [...]cc construction was founded
            // Build new string
            result = result.substr( 0, pos ) + cc( result.substr( pos + 1, end - pos ) ) +
                     result.substr( end + 4, result.length() - end + 4 );
            // Reset positions to the default values. And we start from the end.
            pos     = result.length() - 1;
            backpos = level = end = 0;
          } else {
            level--;
          }
        }
      }
    }
    return result;
  }

} // namespace

// ============================================================================
int main() {

  std::string input;
  std::cout << " Enter the expression " << std::endl;

  while ( std::getline( std::cin, input ) ) {
    if ( input.empty() ) { break; }

    std::cout << "INPUT   : '" << input << "'" << std::endl << "RESULT  : '" << ccfy( input ) << "'" << std::endl;
  }
}
// ============================================================================
// The END
// ============================================================================
