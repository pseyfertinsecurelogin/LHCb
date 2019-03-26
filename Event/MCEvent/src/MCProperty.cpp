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

// local
#include "Event/MCProperty.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MCProperty
//
// 2004-01-22 : Olivier Callot
//-----------------------------------------------------------------------------

//=========================================================================
//  Returns the second member of the pair with the specified key, or zero
//=========================================================================
int LHCb::MCProperty::property( const LHCb::MCParticle* part ) const {
  if ( !part ) return 0;
  const int key = part->key();
  int       indx( 0 );
  if ( findIndex( key, indx ) ) return m_property[indx].second;
  return 0;
}

//=========================================================================
//
//=========================================================================
bool LHCb::MCProperty::findIndex( int key, int& index ) const {

  // binary search
  int iF = 0;
  int iL = m_property.size() - 1;
  while ( iF <= iL ) {
    const int iM     = ( iF + iL ) / 2;
    const int tmpKey = m_property[iM].first;

    if ( key == tmpKey ) {
      index = iM;
      return true;
    }

    if ( key < tmpKey ) {
      iL = iM - 1;
    } else {
      iF = iM + 1;
    }
  }
  index = iF;
  return false;
}

//=========================================================================
//  Insert the pair (MCParticle->key,value) at the appropriate place.
//  l Replace if key exists
//=========================================================================
void LHCb::MCProperty::setProperty( const LHCb::MCParticle* part, int value ) {
  if ( !part ) return;
  const int key = part->key();
  int       iF( 0 );
  if ( findIndex( key, iF ) ) {
    m_property[iF].second = value;
  } else {
    m_property.push_back( std::pair<int, int>( 0, 0 ) );
    int iL = m_property.size() - 1;
    while ( iL > iF ) {
      m_property[iL] = m_property[iL - 1];
      --iL;
    }
    m_property[iF] = std::pair<int, int>( key, value );
  }
}
//=============================================================================
