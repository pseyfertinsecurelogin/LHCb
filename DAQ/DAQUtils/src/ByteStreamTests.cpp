/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// Include files 
#include <iostream>

// local
#include "ByteStreamTests.h"
#include "Event/SmartBank.h"
#include "Event/ByteStream.h"
#include "Event/BankWriter.h"

using namespace LHCb;
using namespace DAQEventTests;
//-----------------------------------------------------------------------------
// Implementation file for class : ByteStreamTests
//
// 2005-10-13 : Matt Needham
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( DAQEventTests::ByteStreamTests )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
ByteStreamTests::ByteStreamTests( const std::string& name,
                                  ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{

}
//=============================================================================
// Destructor
//=============================================================================
ByteStreamTests::~ByteStreamTests() {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode ByteStreamTests::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  debug() << "==> Initialize" << endmsg;

  BankWriter bw(6u);

  unsigned char  t1 = 6;
  unsigned short t2 = 0;
  unsigned int   t3 = 12;

  // each type must be written out enough times to fill a complete multiple 
  // of 32 bits to avoid 'store to misaligned address' sanitizer errors
  bw << t1 << t1 << t1 << t1; 
  bw << t2 << t2;
  bw << t3 << t3 << t3;
  bw << t2 << t2;
  bw << t1 << t1 << t1 << t1;

  info() << bw.dataBank().size() << endmsg;

  unsigned int a[3] = {0,0,0};
  a[0] = 1+ (2<<8) + (3<<16) + (4<<24);
  a[1] = 0;
  a[2] = 256;

  info() << a[0] << endmsg;

  unsigned int* c = &a[0];

  SmartBank<char> cont(c,3*4);

  for ( auto c : cont ) { info() << int(c) << endmsg; }
  
  ByteStream smart(c,3*4); 
  unsigned int   e{0}; 
  unsigned short f[2]{0,0};
  unsigned char  d[4]{0,0,0,0}; 
  smart >> e >> f[0] >> f[1] >> d[0] >> d[1] >> d[2] >> d[3];
  info() << e << " " << f[0] << " " << f[1]
         << " " << (int)d[0]  
         << " " << (int)d[1]  
         << " " << (int)d[2] 
         << " " << (int)d[3] 
         << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
// Execution
//=============================================================================
StatusCode ByteStreamTests::execute() {
  return StatusCode::SUCCESS;
}

//=============================================================================
