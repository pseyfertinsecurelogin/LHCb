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
#ifndef PACKEDBANK_H
#define PACKEDBANK_H 1

// Include files
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/LinkManager.h"

#include <cstring>

/** @class PackedBank PackedBank.h
 *  This is a simple class to store theinformation of a DataObject as a bank
 *
 *  @author Olivier Callot
 *  @date   2008-12-03
 */
class PackedBank {
public:
  /// Standard constructor
  PackedBank( DataObject* obj ) {
    m_bank.reserve( 200 );
    if ( 0 == obj ) return;
    m_bank.push_back( obj->clID() );
    storeString( obj->registry()->identifier() );
    unsigned int objSize = obj->linkMgr()->size();
    m_bank.push_back( objSize );
    for ( unsigned int kl = 0; objSize > kl; ++kl ) { storeString( obj->linkMgr()->link( kl )->path() ); }
  };

  void storeInt( const unsigned int k ) { m_bank.push_back( k ); };

  void storeString( const std::string& str ) {
    unsigned int kk = str.length();
    m_bank.push_back( kk );
    if ( 0 == kk ) return;
    kk                  = kk / 4 + 1; // size in int of 4 bytes, includes trailing zero.
    unsigned int offset = m_bank.size();
    while ( 0 < kk ) {
      m_bank.push_back( 0 );
      --kk;
    }
    char* to = (char*)&m_bank[offset];
    strcpy( to, str.c_str() );
  }

  std::vector<unsigned int>& data() { return m_bank; }

  void addEntry( unsigned int len, unsigned int nItem, unsigned int blobNb ) {
    m_bank.push_back( len );
    m_bank.push_back( nItem );
    m_bank.push_back( blobNb );
  }

private:
  std::vector<unsigned int> m_bank;
};
#endif // PACKEDBANK_H
