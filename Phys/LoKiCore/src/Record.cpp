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
// STD & STL
// ============================================================================
#include <cstdarg>
// ============================================================================
// local
// ============================================================================
#include "LoKi/Record.h"
#include "LoKi/Separators.h"
#include "LoKi/Tokens.h"
// ============================================================================
/** @file
 *
 *  Implementation file for class LoKi::Record
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
// ============================================================================
/*  constructor
 *  @param tuple pointer to LoKi Tuple adapter
 *  @param format blank-separated list of variables,
 *          followed by variable number of arguments
 *  @attention  All variables are assumed to be <c>double</c> numbers
 */
// ============================================================================
LoKi::Record::Record( const Tuples::Tuple& ntuple, const char* format... ) : Tuples::Tuple( ntuple ) {
  if ( valid() ) {
    // decode format string into tokens
    LoKi::Tokens tokens;
    LoKi::tokenize( format, tokens, LoKi::Separators );
    if ( !tokens.empty() ) {
      // decode arguments
      va_list valist;
      va_start( valist, format );
      // loop over all tokens
      StatusCode status = StatusCode::SUCCESS;
      for ( LoKi::Tokens::const_iterator token = tokens.begin(); tokens.end() != token && status.isSuccess();
            ++token ) {
        const double val = va_arg( valist, double );
        status           = tuple()->column( *token, val );
      }
      // mandatory !!!
      va_end( valist );
      //
    }
  }
}
// ============================================================================
// destructor
// ============================================================================
LoKi::Record::~Record() {
  if ( valid() ) { tuple()->write(); }
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
