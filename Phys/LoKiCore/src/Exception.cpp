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
#include <cstdio>
#include <string>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Exception.h"
// ============================================================================
/** @file
 *
 *  Implementation file for class LoKi::Exception
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
/*  Constructor
 *  @param Message error message
 *  @param Code status code
 */
// ============================================================================
LoKi::Exception::Exception( const std::string& Message, const StatusCode& Code, const std::string& File,
                            const unsigned long& Line )
    : GaudiException( Message, "*LoKi::Exception*", Code ) {
  if ( !File.empty() || 0 != Line ) {
    char buf[10];
    m_message += " in file '" + File + "' at line: " + std::string( buf, buf + sprintf( buf, "%lu", Line ) );
  }
}
// ============================================================================
/*  Constructor
 *  @param Message error message
 *  @param Code    status code
 *  @param excpt   "previous"  exception
 */
// ============================================================================
LoKi::Exception::Exception( const std::string& Message, const StatusCode& Code, const GaudiException& excpt,
                            const std::string& File, const unsigned long& Line )
    : GaudiException( Message, "*LoKi::Exception*", Code, excpt ) {
  if ( !File.empty() || 0 != Line ) {
    char buf[10];
    m_message += " in file '" + File + "' at line: " + std::string( buf, buf + sprintf( buf, "%lu", Line ) );
  };
}
// ============================================================================
/*  clone operation
 *  @see GaudiException
 *  @return the clone
 */
// ============================================================================
LoKi::Exception* LoKi::Exception::clone() const { return new LoKi::Exception( *this ); }
// ============================================================================
// The END
// ============================================================================
