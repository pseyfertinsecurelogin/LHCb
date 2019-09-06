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
// Include files
#include <algorithm>

// local
#include "DetDesc/ParamList.h"

//-----------------------------------------------------------------------------
// Implementation file for class : ParamList
//
// 2005-02-22 : Marco CLEMENCIC
//-----------------------------------------------------------------------------

//=============================================================================
// Copy constructor
//=============================================================================
ParamList::ParamList( const ParamList& pl ) { *this = pl; }

//=============================================================================
// Assignement
//=============================================================================
ParamList& ParamList::operator=( const ParamList& pl ) {
  clear();
  for ( const auto& [k, v] : pl.m_map ) { m_map.insert( {k, v->clone()} ); }
  return *this;
}
ParamList& ParamList::operator+=( const ParamList& pl ) {
  for ( const auto& [k, v] : pl.m_map ) { m_map.insert_or_assign( k, v->clone() ); }
  return *this;
}

//=============================================================================
// Clear the list
//=============================================================================
void ParamList::clear() { m_map.clear(); }

//=============================================================================
// return a vector containing all the stored keys
//=============================================================================
std::vector<std::string> ParamList::getKeys() const {
  std::vector<std::string> v;
  v.reserve( m_map.size() );
  std::transform( m_map.begin(), m_map.end(), std::back_inserter( v ), []( const auto& i ) { return i.first; } );
  return v;
}

//=============================================================================
