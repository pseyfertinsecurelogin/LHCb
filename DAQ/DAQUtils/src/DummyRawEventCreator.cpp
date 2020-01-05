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
#include "Event/RawEvent.h"
#include "GaudiAlg/Producer.h"
#include <numeric>

using namespace LHCb;
//-----------------------------------------------------------------------------
// Implementation file for class : DummyRawEventCreator
//
// 2005-10-13 : Markus Frank
//-----------------------------------------------------------------------------

namespace DAQEventTests {
  /** @class DummyRawEventCreator DummyRawEventCreator.h
   *  Creates and fills dummy RawEvent
   *
   *  @author Markus Frank
   *  @date   2005-10-13
   */
  struct DummyRawEventCreator : Gaudi::Functional::Producer<RawEvent()> {

    DummyRawEventCreator( const std::string& name, ISvcLocator* pSvcLocator )
        : Producer{name, pSvcLocator, {"RawEventLocation", RawEventLocation::Default}} {}

    RawEvent operator()() const override {
      RawEvent raw;
      for ( int i = 0; i < 16; ++i ) {
        int      len  = ( i + 1 ) * 64;
        RawBank* bank = raw.createBank( i, RawBank::DAQ, 1, len, 0 );
        std::iota( bank->begin<int>(), bank->end<int>(), 0 );
        raw.adoptBank( bank, true );
      }
      for ( int i = 0; i < 9; ++i ) {
        int      len  = ( i + 1 ) * 32;
        RawBank* bank = raw.createBank( i, RawBank::PrsE, 1, len, 0 );
        std::iota( bank->begin<int>(), bank->end<int>(), 0 );
        raw.adoptBank( bank, true );
      }
      return raw;
    }
  };

  // Declaration of the Algorithm Factory
  DECLARE_COMPONENT( DummyRawEventCreator )

} // namespace DAQEventTests
