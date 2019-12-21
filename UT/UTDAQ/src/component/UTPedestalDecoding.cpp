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
#include "Event/RawBank.h"
#include "Kernel/UTAlgBase.h"
//#include "Event/RawBank.h"
#include "Event/RawEvent.h"
#include "Event/UTTELL1Data.h"
#include "GaudiAlg/Transformer.h"
#include "Kernel/IUTReadoutTool.h"
#include "Kernel/UTRawBankMap.h"
#include "boost/lexical_cast.hpp"

/** @class UTPedestalDecoding UTPedestalDecoding.h
 *
 *  Algorithm to decode the pedestal bank in the RawEvent buffer into
 *  UTTELL1Data objects. Job options:
 *  - \b InputLocation: Location of RawEvent
 *  - \b OutputLocation: Location of output pedestal data, e.g. TTPedestal
 *  \sa{http://edms.cern.ch/doc/695007}.
 *
 *  @author Andy Beiter (based on code by Mathias Knecht, Jeroen van Tilburg)
 *  @date   2018-09-04
 */
class UTPedestalDecoding : public Gaudi::Functional::Transformer<LHCb::UTTELL1Datas( const LHCb::RawEvent& ),
                                                                 Gaudi::Functional::Traits::BaseClass_t<UT::AlgBase>> {

public:
  /// Standard constructor
  UTPedestalDecoding( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode         initialize() override;                              ///< Algorithm initialization
  LHCb::UTTELL1Datas operator()( const LHCb::RawEvent& ) const override; ///< Algorithm execution

private:
  LHCb::RawBank::BankType m_bankType;
};

using namespace LHCb;
using namespace UTDAQ;

//-----------------------------------------------------------------------------
// Implementation file for class : UTPedestalDecoding
//
// 2007-09-11: Mathias Knecht, Jeroen van Tilburg
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Declaration of the Algorithm Factory
DECLARE_COMPONENT( UTPedestalDecoding )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
UTPedestalDecoding::UTPedestalDecoding( const std::string& name, ISvcLocator* pSvcLocator )
    : Transformer{name,
                  pSvcLocator,
                  {"InputLocation", RawEventLocation::Default},
                  {"OutputLocation", UTTELL1DataLocation::UTPedestal}} {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode UTPedestalDecoding::initialize() {
  return Transformer::initialize().andThen(
      [&] { m_bankType = UTRawBankMap::stringToType( detType() + "Pedestal" ); } );
}

//=============================================================================
// Main execution
//=============================================================================
UTTELL1Datas UTPedestalDecoding::operator()( const RawEvent& raw ) const {

  // make container of TELL1 boards
  UTTELL1Datas outputPedestals;

  // Pick up pedestal bank
  const auto& itf = raw.banks( RawBank::BankType( m_bankType ) );
  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << "Starting to decode " << itf.size() << detType() << "Pedestal bank(s)" << endmsg;
  }

  for ( const LHCb::RawBank* p : itf ) {

    if ( msgLevel( MSG::DEBUG ) ) {
      debug() << "Decoding bank of type " << detType() << "Pedestal (TELL1 ID: " << p->sourceID()
              << ", Size: " << p->size() << " bytes)" << endmsg;
    }

    // Check if the board is valid
    UTTell1ID           tell1ID = UTTell1ID( (unsigned int)( p->sourceID() ), detType() == "UT" );
    const UTTell1Board* aBoard  = this->readoutTool()->findByBoardID( tell1ID );
    if ( !aBoard ) {
      std::string invalidSource = "Invalid source ID --> skip bank" + boost::lexical_cast<std::string>( p->sourceID() );
      Warning( invalidSource, StatusCode::SUCCESS, 2 ).ignore();
      ++counter( "skipped Banks" );
      continue;
    }

    // Create an empty tell1 pedestal object
    UTTELL1Data::Data pedestals;
    pedestals.resize( noptlinks );
    for ( auto& i : pedestals ) i.resize( nports * nstrips, 0 );

    if ( (unsigned int)p->size() != sizebankPedestal ) {
      error() << "Wrong bank size for this type!! You should have " << sizebankPedestal << " bytes" << endmsg;
    }

    // Counters
    unsigned int cntWD = 0; // Word counter, resets for each PP. Range 0 to 191.
    unsigned int cntPP = 0; // PP-FPGA counter, goes from 0 to 3.

    // Now loop over all WORDS in a bank
    for ( const unsigned int* w = p->begin<unsigned int>(); w != p->end<unsigned int>(); ++w ) {

      if ( cntWD == 192 ) { // Each 192 words we have a new PP-FPGA
        cntWD = 0;
        ++cntPP;
      }

      if ( cntWD == 0 && msgLevel( MSG::DEBUG ) ) {
        debug() << "#######  Parsing now data from PP " << cntPP << " ##################" << endmsg;
      }

      // Unpack the 32-bit word into 8-bit chunks
      unsigned int p1 = ( *w & mask1 );
      unsigned int p2 = ( ( *w & mask2 ) / 0x100 );
      unsigned int p3 = ( ( *w & mask3 ) / 0x10000 );
      unsigned int p4 = ( ( *w & mask4 ) / 0x1000000 );

      int iPort   = cntWD / ( nbeetles * nstrips );                    // range 0 to 1
      int iWord   = ( cntWD % ( nbeetles * nstrips ) ) / nbeetles;     // range: 0 to 32
      int iBeetle = 2 * ( cntWD % nbeetles ) + nBeetlesPerPPx * cntPP; // range: 0 to 22

      pedestals[iBeetle][iWord + nstrips * iPort]             = p1;
      pedestals[iBeetle][iWord + nstrips * ( iPort + 2 )]     = p2;
      pedestals[iBeetle + 1][iWord + nstrips * iPort]         = p3;
      pedestals[iBeetle + 1][iWord + nstrips * ( iPort + 2 )] = p4;

      ++cntWD;
    } // Loop over all words

    // make an empty tell1 data object
    // and put into the container, second argument is Tell1 id
    outputPedestals.insert( new UTTELL1Data( pedestals ), int( p->sourceID() ) );

  } // end of loop over banks of a certain type
  return outputPedestals;
}
