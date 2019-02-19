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
// $Id$
// Include files
// from Gaudi
#include "Event/HltDecReport.h"
#include "Event/HltDecReports.h"

// local
#include "HltDecReportsDecoder.h"
#include "HltDecReportsWriter.h"

//-----------------------------------------------------------------------------
// Implementation file for class : HltDecReportsDecoder
//
// 2008-08-02 : Tomasz Skwarnicki
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( HltDecReportsDecoder )

using namespace LHCb;

namespace {

  // version 1 layout:
  // decision:  0x        1                      x
  // error:     0x        e                   xxx0
  // #cand:     0x       f0              xxxx 0000
  // stage:     0x     ff00    xxxx xxxx 0000 0000
  // id:        0xffff 0000
  // version 0 layout:
  // decision:  0x        1                      x
  // error:     0x       70              0xxx 0000
  // #cand:     0x     ff80    xxxx xxxx x000 0000
  // stage:     0x        e                   xxx0
  // id:        0xffff 0000
  struct v0_v1 {
    HltDecReport convert( unsigned int x ) const {
      // ID & decision stay the same
      unsigned int temp = ( x & 0xffff0001 );
      // stage needs to be moved left
      temp |= ( x & 0xe ) << 7;
      // number of candidates -- move & truncate
      unsigned int nc = std::min( ( x >> 7 ) & 0x1ff, 0xfu );
      temp |= nc << 4;
      // error just moves to the right
      temp |= ( x & 0x70 ) >> 3;
      return HltDecReport( temp );
    }
  };

  struct vx_vx {
    HltDecReport convert( unsigned int x ) const { return HltDecReport( x ); }
  };

} // namespace

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
HltDecReportsDecoder::HltDecReportsDecoder( const std::string& name, ISvcLocator* pSvcLocator )
    : HltRawBankDecoder<LHCb::HltDecReports>(
          name, pSvcLocator,
          KeyValue{"RawEventLocations", Gaudi::Functional::concat_alternatives( LHCb::RawEventLocation::Trigger,
                                                                                LHCb::RawEventLocation::Copied,
                                                                                LHCb::RawEventLocation::Default )},
          KeyValue{"OutputHltDecReportsLocation", LHCb::HltDecReportsLocation::Default} ) {}
//=============================================================================
// Main execution
//=============================================================================
LHCb::HltDecReports HltDecReportsDecoder::operator()( const LHCb::RawEvent& rawEvent ) const {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  // create output container
  HltDecReports outputSummary;

  auto hltdecreportsRawBanks = selectRawBanks( rawEvent.banks( RawBank::HltDecReports ) );
  if ( hltdecreportsRawBanks.empty() ) {
    throw GaudiException( " No HltDecReports RawBank -- continuing, but not producing HltDecReports", name(),
                          StatusCode::SUCCESS );
  }
  if ( hltdecreportsRawBanks.size() != 1 ) {
    Warning(
        " More then one HltDecReports RawBanks for requested SourceID in RawEvent. Will only process the first one. ",
        StatusCode::SUCCESS, 20 )
        .ignore();
  }
  const RawBank* hltdecreportsRawBank = hltdecreportsRawBanks.front();
  if ( hltdecreportsRawBank->magic() != RawBank::MagicPattern ) {
    throw GaudiException( " HltDecReports RawBank has wrong magic number. Return without decoding.", name(),
                          StatusCode::FAILURE );
  }
  if ( hltdecreportsRawBank->version() > kVersionNumber ) {
    throw GaudiException(
        " HltDecReports RawBank version # is larger then the known ones.... cannot decode, use newer version.", name(),
        StatusCode::FAILURE );
  }

  // ----------------------------------------------------------
  const unsigned int* content = hltdecreportsRawBank->begin<unsigned int>();

  // version 0 has only decreps, version 1 has TCK, taskID, then decreps...
  if ( hltdecreportsRawBank->version() > 0 ) {
    outputSummary.setConfiguredTCK( *content++ );
    outputSummary.setTaskID( *content++ );
  }
  // --------------------------------- get configuration --------------------
  unsigned int tck = outputSummary.configuredTCK();
  const auto&  tbl = id2string( tck );

  // ---------------- loop over decisions in the bank body; insert them into the output container
  int err = 0;
  switch ( hltdecreportsRawBank->version() ) {
  case 0:
    err += this->decodeHDR<v0_v1>( content, hltdecreportsRawBank->end<unsigned int>(), outputSummary, tbl );
    break;
  case 1:
  case 2:
    err += this->decodeHDR<vx_vx>( content, hltdecreportsRawBank->end<unsigned int>(), outputSummary, tbl );
    break;
  default:
    Error( " HltDecReports RawBank version # is larger then the known ones.... cannot decode, use newer version. ",
           StatusCode::FAILURE )
        .ignore();
    err += 1;
  }

  if ( msgLevel( MSG::VERBOSE ) ) {
    // debugging info
    verbose() << " ====== HltDecReports container size=" << outputSummary.size() << endmsg;
    verbose() << outputSummary << endmsg;
  }
  if ( err != 0 ) {
    throw GaudiException( " HltDecReports RawBank error during decoding.", name(), StatusCode::FAILURE );
  }
  return outputSummary;
}

template <typename HDRConverter, typename I, typename Table>
int HltDecReportsDecoder::decodeHDR( I i, I end, HltDecReports& output, const Table& table ) const {
  int                ret = 0;
  const HDRConverter converter{};
  while ( i != end ) {
    auto dec  = converter.convert( *i++ );
    auto isel = table.find( dec.intDecisionID() );
    if ( isel == std::end( table ) ) { // oops missing.
      Error( " No string key found for trigger decision in storage id = " + std::to_string( dec.intDecisionID() ),
             StatusCode::FAILURE, 50 )
          .ignore();
      ++ret;
    } else if ( !!isel->second ) { // has a non-zero string -- insert!!
      // debug() << " adding " << id << " as " << isel->second << endmsg;
      if ( !output.insert( isel->second, dec ).isSuccess() ) {
        Error( " Duplicate decision report in storage " + std::string( isel->second ), StatusCode::FAILURE, 20 )
            .ignore();
        ++ret;
      }
    } // otherwise, present, but should be skipped
  }
  return ret;
}
//=============================================================================
