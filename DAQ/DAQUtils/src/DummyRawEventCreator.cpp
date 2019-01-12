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
#include <numeric>
// from Gaudi
#include "GaudiKernel/IEventTimeDecoder.h"

// local
#include "Event/RawEvent.h"
#include "Event/ODIN.h"

#include "DummyRawEventCreator.h"

//-----------------------------------------------------------------------------
// Implementation file for class : DummyRawEventCreator
//
// 2005-10-13 : Markus Frank
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( DAQEventTests::DummyRawEventCreator )

#ifndef ODIN_VERSION
#define ODIN_VERSION 3
#endif

using namespace LHCb;
using namespace DAQEventTests;

StatusCode DummyRawEventCreator::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize();
  if (sc.isFailure()) return sc;

  m_eventTimeDecoder = tool<IEventTimeDecoder>("OdinTimeDecoder",this,true);

  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode DummyRawEventCreator::execute() {

  debug() << "==> Execute" << endmsg;

  RawEvent* raw = new RawEvent();
  for(int i=0; i<16; ++i)  {
    int len  = (i+1)*64;
    RawBank* bank = raw->createBank(i, RawBank::DAQ, 1, len, 0);
    std::iota( bank->begin<int>(), bank->end<int>(), 0 );
    raw->adoptBank(bank, true);
  }

  for(int i=0; i<9; ++i)  {
    int len  = (i+1)*32;
    RawBank* bank = raw->createBank(i, RawBank::PrsE, 1, len, 0);
    std::iota( bank->begin<int>(), bank->end<int>(), 0 );
    raw->adoptBank(bank, true);
  }

  put( raw, RawEventLocation::Default );

  return StatusCode::SUCCESS;
}
