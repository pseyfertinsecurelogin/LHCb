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
// $Id: L0DUElementaryCondition.cpp,v 1.6 2010-01-20 15:59:06 odescham Exp $
// Include files
#include <map>
#include <sstream>
#include <string>
#include <utility>

#include "Event/L0DUElementaryCondition.h"

//-----------------------------------------------------------------------------
// Auxilliary Implementation file for L0DUElementaryCondition Event classes
//
// 09/02/2006 : Olivier Deschamps
//-----------------------------------------------------------------------------

bool LHCb::L0DUElementaryCondition::comparison( unsigned int a, const std::string& comparator, unsigned int b ) {
  if ( "<" == comparator ) { return ( a < b ); }
  if ( ">" == comparator ) { return ( a > b ); }
  if ( "==" == comparator ) { return ( a == b ); }
  if ( "!=" == comparator ) { return ( a != b ); }
  return false;
}

std::string LHCb::L0DUElementaryCondition::summary() {

  std::ostringstream s( " " );
  std::ostringstream ss( " " );
#ifdef __INTEL_COMPILER             // Disable ICC remark
#  pragma warning( disable : 1572 ) // Floating-point equality and inequality comparisons are unreliable
#endif
  if ( m_data->scale() != 1. ) ss << " ( => " << m_data->scale() * (double)m_threshold << " MeV )";

  s << "Condition [ " << m_name << " : " << m_data->summary( m_bx ) << " " << m_comparator << " " << m_threshold
    << ss.str() << "] ?  ==> " << emulatedValue();
  return s.str();
}

std::string LHCb::L0DUElementaryCondition::description() {

  std::ostringstream s( " " );

  s << "Condition [ " << m_name << " : " << m_data->description( m_bx ) << " " << m_comparator << " " << m_threshold
    << "] ?";
  return s.str();
}
