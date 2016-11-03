// $Id: MCProperty.cpp,v 1.2 2005-12-05 16:50:59 cattanem Exp $
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
int LHCb::MCProperty::property ( const LHCb::MCParticle* part) {
  if ( !part ) return 0;
  const int key = part->key();
  int indx(0);
  if ( findIndex( key, indx ) )  return m_property[indx].second;
  return 0;
}

//=========================================================================
//  
//=========================================================================
bool LHCb::MCProperty::findIndex ( int key, int& index) {

  // binary search
  int iF = 0;
  int iL = m_property.size() - 1;
  while ( iF <= iL ) 
  {
    const int iM = (iF + iL ) / 2;
    const int tmpKey = m_property[iM].first;

    if ( key == tmpKey ) {
      index = iM;
      return true;
    } else if ( key < tmpKey ) {
      iL = iM-1;
    } else {
      iF = iM+1;
    }
  }
  index = iF;
  return false;
}

//=========================================================================
//  Insert the pair (MCParticle->key,value) at the appropriate place.
//  l Replace if key exists
//=========================================================================
void LHCb::MCProperty::setProperty ( const LHCb::MCParticle* part, int value ) 
{
  if ( !part ) return;
  const int key = part->key();
  int iF(0);
  if ( findIndex( key, iF ) )
  {
    m_property[iF].second = value;
  } 
  else 
  {
    m_property.push_back( std::pair<int,int>( 0, 0 ) );
    int iL = m_property.size() - 1;
    while ( iL > iF ) {
      m_property[iL] = m_property[iL-1];
      --iL;
    }
    m_property[iF] = std::pair<int,int>( key, value );
  }
}
//=============================================================================
