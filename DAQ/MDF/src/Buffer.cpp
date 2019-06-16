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

#include "MDF/Buffer.h"
#include "MDF/RawEventPrintout.h"

#include "GaudiKernel/GaudiException.h"

std::optional<LHCb::RawEvent> LHCb::MDF::Buffer::get() {
  /// Atomically returns a unique eventID or <= 0 number if no events remain
  int evtId = m_nbAvailableEvents--;
  /// no event remains
  if ( evtId <= 0 ) return {};
  /// get the event we've picked
  auto& event = m_events[size() - evtId];
  // Decode banks of the event
  event.event().reserve( 1000 ); // reserving enough space for all banks in most cases
  std::byte* start = event.data();
  std::byte* end   = event.data() + event.size();
  while ( start < end ) {
    const RawBank* bank = reinterpret_cast<const RawBank*>( start );
    // check Bank sanity
    if ( bank->magic() != RawBank::MagicPattern ) {
      throw GaudiException( "Unknown Bank type in Tell1 bank : " + RawEventPrintout::bankHeader( bank ), "MDF::Buffer",
                            StatusCode::FAILURE );
    }
    if ( bank->type() >= RawBank::LastType ) {
      throw GaudiException( "Bad magic pattern in Tell1 bank : " + RawEventPrintout::bankHeader( bank ), "MDF::Buffer",
                            StatusCode::FAILURE );
    }
    event.event().adoptBank( bank, false );
    start += bank->totalSize();
  }
  return std::move( event.event() );
}
