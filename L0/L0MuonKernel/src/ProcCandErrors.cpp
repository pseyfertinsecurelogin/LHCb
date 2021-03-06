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
#include "L0MuonKernel/ProcCandErrors.h"

L0Muon::ProcCandErrors::ProcCandErrors() {
  present       = L0Muon::ErrorHandler( " %01X", 0x1, "nb of banks seen" );
  decoding      = L0Muon::ErrorHandler( " %08X", 0xFFFFFFFF, "decoding" );
  l0EventNumber = L0Muon::ErrorHandler( " %03X", 0xFFF, "l0EventNumber" );
  l0_B_Id       = L0Muon::ErrorHandler( " %03X", 0xFFF, "l0_B_Id" );

  bcsu_bcid   = L0Muon::ErrorHandler( " %01X", 0x00F, "bcsu_bcid" );
  bcsu_status = L0Muon::ErrorHandler( " %01X", 0x00C, "bcsu_status" );
  for ( int ipu = 0; ipu < 4; ++ipu ) {
    std::string name;
    name            = ( boost::format( "PU%1d pus_bcid" ) % ipu ).str();
    pus_bcid[ipu]   = L0Muon::ErrorHandler( "%01X", 0x00F, name );
    name            = ( boost::format( "PU%1d pus_status" ) % ipu ).str();
    pus_status[ipu] = L0Muon::ErrorHandler( "%01X", 0x00C, name );
  }
  errJ = L0Muon::ErrorHandler( "%01X", 0x001, "errJ" );
  errK = L0Muon::ErrorHandler( "%01X", 0x001, "errK" );
  errH = L0Muon::ErrorHandler( "%01X", 0x00F, "errH" );
  errF = L0Muon::ErrorHandler( "%01X", 0x00F, "errF" );
}

L0Muon::ProcCandErrors::~ProcCandErrors() {}

bool L0Muon::ProcCandErrors::inError() const {

  bool error = false;

  error |= ( decodingError() > 0 );
  error |= ( hardwareError() > 0 );
  error |= ( bcidError() > -1 );
  error |= ( statusError() > 0 );

  return error;
}

bool L0Muon::ProcCandErrors::inError( int ipu ) const {

  bool error = false;

  error |= ( decodingError() > 0 );
  error |= ( bcidError( ipu ) > -1 );
  error |= ( statusError( ipu ) > 0 );

  return error;
}
int L0Muon::ProcCandErrors::decodingError() const { return int( decoding.inError() ); }

int L0Muon::ProcCandErrors::hardwareError() const {
  int error = 0;
  error |= ( ( errJ.value() << 12 ) & 0x00001000 );
  error |= ( ( errK.value() << 8 ) & 0x00000100 );
  error |= ( ( errH.value() << 4 ) & 0x000000F0 );
  error |= ( ( errF.value() << 0 ) & 0x0000000F );
  if ( l0_B_Id.inError() ) error |= 0x00002000;
  if ( l0EventNumber.inError() ) error |= 0x00000200;
  return error;
}

int L0Muon::ProcCandErrors::bcidError() const {
  if ( bcsu_bcid.inError() ) return bcsu_bcid.value();
  return -1;
}

int L0Muon::ProcCandErrors::bcidError( int ipu ) const {
  if ( pus_bcid[ipu].inError() ) return pus_bcid[ipu].value();
  return -1;
}

int L0Muon::ProcCandErrors::statusError() const { return bcsu_status.value(); }

int L0Muon::ProcCandErrors::statusError( int ipu ) const { return pus_status[ipu].value(); }

int L0Muon::ProcCandErrors::sumCounters() const {

  int n = 0;

  n += decoding.counter();
  n += l0EventNumber.counter();
  n += l0_B_Id.counter();

  n += bcsu_bcid.counter();
  n += bcsu_status.counter();
  for ( int i = 0; i < 4; ++i ) {
    n += pus_bcid[i].counter();
    n += pus_status[i].counter();
  }
  n += errJ.counter();
  n += errK.counter();
  n += errH.counter();
  n += errF.counter();

  return n;
}

std::ostream& L0Muon::operator<<( std::ostream& os, const L0Muon::ProcCandErrors& pberr ) {

  if ( pberr.decoding.inError() ) {
    os << " Data corrupted ";
  } else {
    os << pberr.l0EventNumber;
    os << pberr.l0_B_Id;
    os << "  ";
    os << pberr.bcsu_bcid;
    os << " ";
    for ( int ipu = 0; ipu < 4; ++ipu ) os << pberr.pus_bcid[ipu];
    os << "  ";
    os << pberr.bcsu_status;
    os << " ";
    for ( int ipu = 0; ipu < 4; ++ipu ) os << pberr.pus_status[ipu];
  }

  return os;
}
