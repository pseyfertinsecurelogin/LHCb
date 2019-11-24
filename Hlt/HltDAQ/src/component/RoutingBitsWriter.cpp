/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// from Gaudi
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/DataObjectHandle.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/ParsersFactory.h"

#include "Event/HltDecReports.h"
#include "Event/ODIN.h"

#include "Event/RawEvent.h"

/** @class RoutingBitsWriter RoutingBitsWriter.h
 *  @brief Write routing bits based on ODIN flags and DecReports into raw event.
 *
 *  The definition of the routing bits is given as a map between the routing bit
 *  and a list of line names contributing to this bit. The combined decision of the
 *  lines is evaluated and used to set a Routing bit. The Routing bits are written to the raw event.
 *  The maximum number of bits is 128.
 *
 *  Example:
 *
 *  Set Routing bit 33 based on Hlt1TrackMVALine and Hlt1TwoTrackMVALine decsion
 *
 *  @code
 *  from PyConf.Algorithms import Hlt_RoutingBitsWriter
 *  Hlt_RoutingBitsWriter(
 *     RoutingBits = { "33" : ["Hlt1TrackMVALine", "Hlt1TwoTrackMVALine"] },
 *     DecReports = <dec_reports>,
 *     ODIN= <odin>,
 *     RawEventLocation = <raw_event>,
 *  )
 *  @endcode
 */

namespace Hlt {
  class RoutingBitsWriter : public Gaudi::Algorithm {
  public:
    /// Standard constructor
    using Gaudi::Algorithm::Algorithm;

    StatusCode execute( EventContext const& ) const override; ///< Algorithm execution

  private:
    enum { nBits = 3 * sizeof( unsigned int ) * 8 };

    DataObjectReadHandle<LHCb::ODIN>          m_odin{this, "ODIN", ""};
    DataObjectReadHandle<LHCb::HltDecReports> m_dec_reports{this, "DecReports", ""};

    Gaudi::Property<std::string> m_raw_location{this, "RawEventLocation", LHCb::RawEventLocation::Default};

    Gaudi::Property<std::map<unsigned int, std::vector<std::string>>> m_bits{
        this, "RoutingBits", {}, [this]( auto& ) {
          for ( const auto& row : m_bits ) {
            auto bit = row.first;
            if ( bit > 127 ) {
              this->error() << "Too large Routing bit defined. Number must be smaller than 128." << endmsg;
              throw GaudiException{"Too large Routing bit defined. Number must be smaller than 128.",
                                   "Hlt::RoutingBitsWriter", StatusCode::FAILURE};
            }
          }
        }};
  };
} // namespace Hlt

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( Hlt::RoutingBitsWriter )

namespace Hlt {

  //=============================================================================
  // Main execution
  //=============================================================================
  StatusCode RoutingBitsWriter::execute( EventContext const& ) const {
    // auto const* odin        = m_odin.get();
    // TODO: Add evaluators of odin bits
    auto const* dec_reports = m_dec_reports.get();

    // The routing bits
    std::vector<unsigned int> bits( 3, 0 );

    // Evaluate the routing bits
    for ( auto const& entry : m_bits ) {
      auto result = std::any_of( begin( entry.second ), end( entry.second ), [&dec_reports]( auto const& line ) {
        auto const* dec_report = dec_reports->decReport( line );
        if ( dec_report == nullptr ) { return false; }
        return dec_report->decision() == 1;
      } );
      auto bit    = entry.first;
      int  word   = bit / 32;
      if ( result ) bits[word] |= ( 0x01UL << ( bit - 32 * word ) );
    }

    // Get the raw event and update or add the routing bits bank.
    DataObject* tmp{nullptr};
    if ( !evtSvc()->retrieveObject( m_raw_location.value(), tmp ) ) {
      error() << "Could not retrieve " << m_raw_location.value() << endmsg;
      return StatusCode::FAILURE;
    }

    LHCb::RawEvent* rawEvent = dynamic_cast<LHCb::RawEvent*>( tmp );
    if ( rawEvent == nullptr ) {
      error() << "No RawEvent at " << m_raw_location.value() << endmsg;
      return StatusCode::FAILURE;
    }
    rawEvent->addBank( 0, LHCb::RawBank::HltRoutingBits, 0, bits );

    return StatusCode::SUCCESS;
  }
} // namespace Hlt