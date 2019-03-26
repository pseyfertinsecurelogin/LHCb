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
ParamList::ParamList( const ParamList& pl ) : base_type() { *this = pl; }

//=============================================================================
// Assignement
//=============================================================================
ParamList& ParamList::operator=( const ParamList& pl ) {
  clear();
  for ( const auto& i : pl ) { insert( {i.first, i.second->new_copy().release()} ); }
  return *this;
}
ParamList& ParamList::operator+=( const ParamList& pl ) {
  for ( const auto& i : pl ) {
    auto old = find( i.first );
    if ( old != end() ) { // key already used
      delete old->second;
      old->second = i.second->new_copy().release();
    } else {
      insert( {i.first, i.second->new_copy().release()} );
    }
  }
  return *this;
}

//=============================================================================
// Clear the list
//=============================================================================
void ParamList::clear() {
  deleteItems();
  base_type::clear();
}

//=============================================================================
// Delete the object referenced by the stored pointers
//=============================================================================
void ParamList::deleteItems() {
  std::for_each( begin(), end(), []( const std::pair<const std::string, BasicParam*>& i ) { delete i.second; } );
}

//=============================================================================
// return a vector containing all the stored keys
//=============================================================================
std::vector<std::string> ParamList::getKeys() const {
  std::vector<std::string> v;
  v.reserve( size() );
  std::transform( begin(), end(), std::back_inserter( v ),
                  []( const std::pair<const std::string, BasicParam*>& i ) { return i.first; } );
  return v;
}

//=============================================================================
