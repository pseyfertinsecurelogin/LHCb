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
#include "Event/UTTELL1BoardErrorBank.h"
#include "GaudiAlg/Consumer.h"
#include "UTDecodingBaseAlg.h"

/** @class UTErrorDecoding UTErrorDecoding.h public/UTErrorDecoding.h
 *
 *
 *  @author A Beiter (based on code by Mathias Knecht, M Needham, S Ponce)
 *  @date   2018-09-04
 */

class UTErrorDecoding : public Gaudi::Functional::Consumer<void( const LHCb::RawEvent& ),
                                                           Gaudi::Functional::Traits::BaseClass_t<UTDecodingBaseAlg>> {
public:
  /// Standard constructor
  UTErrorDecoding( const std::string& name, ISvcLocator* pSvcLocator );

  /// Algorithm execution
  void operator()( const LHCb::RawEvent& ) const override;

private:
  Gaudi::Property<bool> m_PrintErrorInfo{this, "PrintErrorInfo", false};
};

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : UTErrorDecoding
//
// 2007-11-29: Mathias Knecht
// Update 2008 M Needham
// Update 2016 S Ponce
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Declaration of the Algorithm Factory
DECLARE_COMPONENT( UTErrorDecoding )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
UTErrorDecoding::UTErrorDecoding( const std::string& name, ISvcLocator* pSvcLocator )
    : Consumer( name, pSvcLocator,
                KeyValue{"RawEventLocations", Gaudi::Functional::concat_alternatives(
                                                  LHCb::RawEventLocation::Tracker, LHCb::RawEventLocation::Other,
                                                  LHCb::RawEventLocation::Default )} ) {}

//=============================================================================
// Main execution
//=============================================================================
void UTErrorDecoding::operator()( const LHCb::RawEvent& raw ) const {
  // in fact all the work is delegated to the base class
  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Execute " << endmsg;
  auto errorBanks = decodeErrors( raw );
  // print out the error banks
  if ( m_PrintErrorInfo ) {
    for ( const auto& b : *errorBanks ) info() << b << endmsg;
  }
}
