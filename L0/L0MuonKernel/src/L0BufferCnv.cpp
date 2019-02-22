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
#include "L0MuonKernel/L0BufferCnv.h"
#include <iostream>

/**
   Constructor.
*/
L0Muon::L0BufferCnv::L0BufferCnv( LHCb::MuonTileID id ) : m_mid( id ), m_valid( false ) {}

/**
   Destructor
*/
L0Muon::L0BufferCnv::~L0BufferCnv() {}

void L0Muon::L0BufferCnv::open( std::string path ) {
  if ( !m_valid ) return;

  // Contruct the filename root from the type and id of the unit
  char idbuf[4096];
  sprintf( idbuf, "Q%dR%d%d%d", m_mid.quarter() + 1, m_mid.region() + 1, m_mid.nX(), m_mid.nY() );
  std::string filename = path + "/" + type() + idbuf + ".txt";
  //   m_file = fopen(filename.c_str(),"w");
  m_file.open( filename.c_str() );
  //   m_file.setf(std::ios::uppercase) ;
  m_file << std::hex;
}

void L0Muon::L0BufferCnv::close() {
  if ( !m_valid ) return;
  //   fclose(m_file);
  m_file.close();
}
