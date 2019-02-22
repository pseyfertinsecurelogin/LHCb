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
// LoKi
// ============================================================================
#include "LoKi/MCDump.h"
#include "LoKi/MCDecayChain.h"
#include "LoKi/Objects.h"
// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
 *  @date   2011-06-03
 *
 */
// ============================================================================
/*  template specialization of ``dumper''
 *  @see LoKi::Fuctors::Dump_
 *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
 *  @date   2011-06-03
 */
// ============================================================================
template <>
std::vector<const LHCb::MCParticle*> LoKi::Functors::Dump_<const LHCb::MCParticle*>::
                                     operator()( const std::vector<const LHCb::MCParticle*>& a ) const {
  //
  const unsigned int mx = std::min( m_dump.nMax(), a.size() );
  //
  m_stream << m_dump.open();
  //
  LoKi::MCDecayChain printer;
  printer.print( a.begin(), a.begin() + mx, m_stream, '\n', LoKi::Objects::_VALID_, LoKi::Objects::_NONE_, " ", 0 );
  //
  m_stream << m_dump.close();
  //
  return a;
}
// ============================================================================
template <>
bool LoKi::Functors::Dump1_<const LHCb::MCParticle*, bool>::operator()( const LHCb::MCParticle* a ) const {
  //
  if ( m_right ) {
    m_stream << m_dump.open();
    //
    LoKi::MCDecayChain printer;
    printer.print( a, m_stream, '\n', LoKi::Objects::_VALID_, LoKi::Objects::_NONE_, " ", 0 );
    //
    m_stream << m_dump.close();
    //
    return m_fun( a );
  }
  //
  auto result = m_fun( a );
  //
  m_stream << m_dump.open();
  //
  LoKi::MCDecayChain printer;
  printer.print( a, m_stream, '\n', LoKi::Objects::_VALID_, LoKi::Objects::_NONE_, " ", 0 );
  //
  m_stream << m_dump.close();
  //
  return result;
}
// ============================================================================
template <>
double LoKi::Functors::Dump1_<const LHCb::MCParticle*, double>::operator()( const LHCb::MCParticle* a ) const {
  //
  if ( m_right ) {
    m_stream << m_dump.open();
    //
    LoKi::MCDecayChain printer;
    printer.print( a, m_stream, '\n', LoKi::Objects::_VALID_, LoKi::Objects::_NONE_, " ", 0 );
    //
    m_stream << m_dump.close();
    //
    return m_fun( a );
  }
  //
  auto result = m_fun( a );
  //
  m_stream << m_dump.open();
  //
  LoKi::MCDecayChain printer;
  printer.print( a, m_stream, '\n', LoKi::Objects::_VALID_, LoKi::Objects::_NONE_, " ", 0 );
  //
  m_stream << m_dump.close();
  //
  return result;
}

// ============================================================================
// The END
// ============================================================================
