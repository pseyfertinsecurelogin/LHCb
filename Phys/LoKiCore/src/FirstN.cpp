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
// STD/ STL
// ============================================================================
#include <ostream>
// ============================================================================
// local
// ============================================================================
#include "LoKi/FirstN.h"
#include "LoKi/ToCpp.h"
// ============================================================================
/** @file
 *  Implementation file for class LoKi::FirstN
 *
 *  @see LoKi::FirstN
 *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
 *  @date   2011-10-30
 *
 */
// ============================================================================
// constructor
// ============================================================================
LoKi::FirstN::FirstN( const unsigned int N ) // constructor
    : m_N( N ) {}
// ============================================================================
// printout
// ============================================================================
std::ostream& LoKi::FirstN::fillStream( std::ostream& s ) const { return s << " FirstN(" << m_N << ") "; }
// ============================================================================
// output operator
// ============================================================================
std::ostream& LoKi::operator<<( std::ostream& s, const FirstN& o ) { return o.fillStream( s ); }
// ============================================================================

// ============================================================================
// printout
// ============================================================================
std::ostream& LoKi::Reverse::fillStream( std::ostream& s ) const { return s << " reverse "; }
// ============================================================================
// output operator
// ============================================================================
std::ostream& LoKi::operator<<( std::ostream& s, const LoKi::Reverse& o ) { return o.fillStream( s ); }
// ============================================================================

// ============================================================================
// constructor
// ============================================================================
LoKi::Sort::Sort( const int  N, // ALL
                  const bool ascending )
    : m_N( N ), m_ascending( ascending ) {}
// ============================================================================
// printout
// ============================================================================
std::ostream& LoKi::Sort::fillStream( std::ostream& s ) const {
  s << " Sort(";
  if ( !m_ascending || 0 <= m_N ) { s << "," << m_N; }
  if ( !m_ascending || 0 <= m_N ) {
    s << ","
      << "False";
  }
  return s << ") ";
}
// ============================================================================
// output operator
// ============================================================================
std::ostream& LoKi::operator<<( std::ostream& s, const LoKi::Sort& o ) { return o.fillStream( s ); }
// ============================================================================

// ============================================================================
std::string Gaudi::Utils::toCpp( const LoKi::FirstN& f ) {
  return "LoKi::FirstN(" + Gaudi::Utils::toCpp( f.N() ) + ")";
}
// ============================================================================
std::string Gaudi::Utils::toCpp( const LoKi::Sort& s ) {
  return "LoKi::Sort(" + Gaudi::Utils::toCpp( s.N() ) + "," + ( s.ascending() ? "true" : "false" ) + ") ";
}
std::string Gaudi::Utils::toCpp( const LoKi::Reverse& /* r */ ) { return "LoKi::Reverse()"; }
// ========================================================================

// ============================================================================
// The EN
// ============================================================================
