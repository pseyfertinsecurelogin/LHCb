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
#include "DAQKernel/DecoderAlgBase.h"
#include "Event/RawEvent.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include <algorithm>

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : RawEventDump
//
// 2005-10-13 : Markus Frank
//-----------------------------------------------------------------------------

/** @class RawEventDump RawEventDump.h tests/RawEventDump.h
 *  Dump a RawEvent
 *
 *  @author Markus Frank
 *  @date   2005-10-13
 */
class RawEventDump : public Decoder::AlgBase {
public:
  /// Standard constructor
  RawEventDump( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:
  bool                                      acceptBank( LHCb::RawBank::BankType i );
  Gaudi::Property<bool>                     m_dump{this, "DumpData",
                               false}; ///< Property "DumpData". If true, full bank contents are dumped
  Gaudi::Property<std::vector<std::string>> m_banks{this, "RawBanks"}; // RawBanks to be dumped  (default ALL banks)
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( RawEventDump )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
RawEventDump::RawEventDump( const std::string& name, ISvcLocator* pSvcLocator )
    : Decoder::AlgBase( name, pSvcLocator ) {
  // new for decoders, initialize search path, and then call the base method
  m_rawEventLocations = {LHCb::RawEventLocation::Default, LHCb::RawEventLocation::Trigger,
                         LHCb::RawEventLocation::Rich,    LHCb::RawEventLocation::Calo,
                         LHCb::RawEventLocation::Muon,    LHCb::RawEventLocation::Other,
                         LHCb::RawEventLocation::Copied,  LHCb::RawEventLocation::Emulated};
  initRawEventSearch();
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode RawEventDump::execute() {

  for ( auto loc : m_rawEventLocations ) {
    RawEvent* raw = tryEventAt( loc );

    if ( !raw ) continue;

    for ( int j = 0; j < int( RawBank::BankType::LastType ); ++j ) {
      const auto i = RawBank::BankType( j );

      if ( !acceptBank( i ) ) continue;

      const auto& b = raw->banks( i );
      if ( !b.empty() ) {
        info() << "banks of type " << i << "(" << RawBank::typeName( i ) << ") discovered in " << loc << endmsg;

        info() << b.size() << " banks of type " << i << ": [size, source, version, magic]";
        int k = 0;
        for ( const RawBank* r : b ) {
          if ( ( ( k++ ) % 4 ) == 0 ) info() << endmsg << "  ";
          info() << "[" << int( r->size() ) << ", " << int( r->sourceID() ) << ", " << int( r->version() ) << ", "
                 << std::hex << r->magic() << std::dec << "] ";
          if ( m_dump ) {
            info() << "Data follows..." << std::hex;
            int cnt = 0;
            for ( const int* p = r->begin<int>(); p != r->end<int>(); ++p ) {
              if ( ( ( cnt++ ) % 10 ) == 0 ) info() << endmsg << "   ... ";
              info() << "[" << *p << "] ";
            }
            info() << std::dec << endmsg << "  ";
          }
        }
        info() << endmsg;
      }
    }
  }

  return StatusCode::SUCCESS;
}

bool RawEventDump::acceptBank( LHCb::RawBank::BankType i ) {
  return m_banks.empty() ||
         std::any_of( m_banks.begin(), m_banks.end(),
                      [tn = LHCb::RawBank::typeName( i )]( const std::string& i ) { return i == tn; } );
}
//=============================================================================
