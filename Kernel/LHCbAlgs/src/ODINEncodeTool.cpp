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
#include "Event/ODIN.h"
#include "Event/RawBank.h"
#include "Event/RawEvent.h"
#include "ODINCodecBaseTool.h"
#include <memory>
#include <vector>

/** @class ODINEncodeTool ODINEncodeTool.h
 *
 *  Tool to encode the ODIN object into a RawBank. The raw bank is then added to
 *  the RawEvent, which must exist.
 *
 *  @see ODINCodecBaseTool for the properties.
 *
 *  @author Marco Clemencic
 *  @date   2009-02-02
 */
class ODINEncodeTool final : public ODINCodecBaseTool {

public:
  /// Standard constructor
  ODINEncodeTool( const std::string& type, const std::string& name, const IInterface* parent );

  /// Do the conversion
  void execute() override;

private:
  /// Location in the transient store of the ODIN object.
  DataObjectReadHandle<LHCb::ODIN> m_odinLocation{this, "ODINLocation", LHCb::ODINLocation::Default,
                                                  "Location of the ODIN object in the transient store."};

  /// Location in the transient store of the RawEvent object
  DataObjectReadHandle<LHCb::RawEvent> m_rawEventLocation{this, "RawEventLocation", LHCb::RawEventLocation::Default,
                                                          "Location of the RawEvent object in the transient store."};
};

//=============================================================================
// IMPLEMENTATION
//=============================================================================

// Declaration of the Tool Factory
DECLARE_COMPONENT( ODINEncodeTool )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
ODINEncodeTool::ODINEncodeTool( const std::string& type, const std::string& name, const IInterface* parent )
    : ODINCodecBaseTool( type, name, parent ) {}
//=============================================================================
// Main function
//=============================================================================
void ODINEncodeTool::execute() {
  // Check if there is an ODIN object
  LHCb::RawEvent* raw = m_rawEventLocation.getIfExists();
  if ( raw ) {
    // Add the raw bank to the raw event
    debug() << "Getting " << m_rawEventLocation.objKey() << endmsg;

    // pointer for a pre-existing bank
    const LHCb::RawBank* old_bank = nullptr;
    // Check if have an ODIN bank already
    const auto& odinBanks = raw->banks( LHCb::RawBank::ODIN );
    if ( !odinBanks.empty() ) {
      if ( m_force ) {
        // we have to replace it... remember which it is, so we can do it if the
        // encoding is successful, just before adding the new bank
        old_bank = *odinBanks.begin();
      } else {
        // keep the existing bank (the new one is automatically disposed)
        return;
      }
    }
    // Encode ODIN object
    LHCb::RawBank* bank = i_encode( m_odinLocation.get() );
    if ( bank ) {
      if ( old_bank ) raw->removeBank( old_bank );
      // add the bank
      raw->adoptBank( bank, true );
    }
  } else {
    Warning( "Cannot find ODIN object to encode" ).ignore();
  }
}

//=============================================================================
