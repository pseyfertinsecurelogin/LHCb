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
#include "GaudiKernel/SerializeSTL.h"
#include "ODINCodecBaseTool.h"

/** @class ODINDecodeTool ODINDecodeTool.h
 *
 *  Tool to decode the ODIN raw bank, fill a ODIN object with its data and
 *  register it to the transient store.
 *
 *  @see ODINCodecBaseTool for the properties.
 *
 *  @author Marco Clemencic
 *  @date   2009-02-02
 */
class ODINDecodeTool final : public ODINCodecBaseTool {

public:
  /// Standard constructor
  ODINDecodeTool( const std::string& type, const std::string& name, const IInterface* parent );

  virtual ~ODINDecodeTool() = default; ///< Destructor

  /// Initialize the tool
  inline StatusCode initialize() override;

  /// Do the conversion
  void execute() override;

private:
  /// Location in the transient store of the ODIN object.
  std::string m_odinLocation;
};

//=============================================================================
// IMPLEMENTATION
//=============================================================================

#include <sstream>
// from LHCb
#include "Event/ODIN.h"
#include "Event/RawBank.h"
#include "Event/RawEvent.h"

// Declaration of the Tool Factory
DECLARE_COMPONENT( ODINDecodeTool )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
ODINDecodeTool::ODINDecodeTool( const std::string& type, const std::string& name, const IInterface* parent )
    : ODINCodecBaseTool( type, name, parent ) {
  declareProperty( "ODINLocation", m_odinLocation = LHCb::ODINLocation::Default,
                   "Location of the ODIN object in the transient store. By "
                   "default is the content of LHCb::ODINLocation::Default." );
  // new for decoders, initialize search path, and then call the base method
  m_rawEventLocations = {LHCb::RawEventLocation::Trigger, LHCb::RawEventLocation::Default};
  initRawEventSearch();
}
//=============================================================================
// Initialize
//=============================================================================
StatusCode ODINDecodeTool::initialize() {
  StatusCode sc = ODINCodecBaseTool::initialize(); // always first
  if ( sc.isFailure() ) return sc;                 // error message already printed

  if ( m_odinLocation != LHCb::ODINLocation::Default ) {
    info() << "Using '" << m_odinLocation << "' as location of the ODIN object" << endmsg;
  }

  if ( m_rawEventLocations.empty() || ( m_rawEventLocations[0] != LHCb::RawEventLocation::Default &&
                                        m_rawEventLocations[0] != LHCb::RawEventLocation::Trigger ) ) {
    info() << "Using '" << m_rawEventLocations << "' as search path for the RawEvent object" << endmsg;
  }

  return sc;
}
//=============================================================================
// Main function
//=============================================================================
void ODINDecodeTool::execute() {
  // load the odin
  LHCb::ODIN* odin = getIfExists<LHCb::ODIN>( m_odinLocation );

  // Check if there is already an ODIN object
  if ( odin ) {
    if ( m_force ) {
      // Use the already registered object
      if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "Modify existing ODIN object" << endmsg;
    } else {
      // ODIN already there and we are not supposed to touch it
      if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "Keep existing ODIN object" << endmsg;
      return;
    }
  }

  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "Getting RawEvent" << endmsg;
  LHCb::RawEvent* rawEvent = findFirstRawEvent();
  if ( !rawEvent ) {
    using namespace GaudiUtils;
    // Throw a meaningful exception it the bank is not found;
    std::ostringstream out;
    out << "Cannot find RawEvent in " << m_rawEventLocations;
    Exception( out.str(), StatusCode( StatusCode::FAILURE, true ) );
  }

  // Check if have an ODIN bank...
  const auto& odinBanks = rawEvent->banks( LHCb::RawBank::ODIN );
  if ( !odinBanks.empty() ) { // ... good, we can decode it
    odin = this->i_decode( *odinBanks.begin(), odin );
    if ( odin && ( !odin->registry() ) ) // register ODIN object if valid and not yet registered
    {
      put( odin, m_odinLocation );
    }
  } else {
    Warning( "Cannot find ODIN bank in RawEvent" ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
  }
}

//=============================================================================
