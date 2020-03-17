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
#include "Event/GenCollision.h"
#include "Event/MCHeader.h"
#include "Event/ODIN.h"
#include "Event/ProcessHeader.h"
#include "Event/RawEvent.h"
#include "GaudiAlg/IGenericTool.h"
#include "GaudiKernel/RndmGenerators.h"
#include "Kernel/LbAppInit.h"
#include <string>
#include <vector>

class IGenericTool;

/** @class BooleInit BooleInit.h
 *  First TopAlg for Boole. Initializes random number and fills memory histogram
 *
 *  @author Marco Cattaneo
 *  @date   2005-12-15
 */
class BooleInit : public LbAppInit {
public:
  /// Standard constructor
  BooleInit( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

protected:
  virtual void modifyOdin( LHCb::ODIN* odin ); ///< fills some Odin fields
  virtual void simpleOdin( LHCb::ODIN* odin ); ///< sets reasonable defaults for some Odin fields

private:
  ToolHandle<IGenericTool>       m_memoryTool{this, "MemoryTool",
                                        "MemoryTool/BooleMemory"}; ///< Pointer to (private) memory histogram tool
  PublicToolHandle<IGenericTool> m_odinTool{this, "OdinTool", "ODINEncodeTool"}; ///< Pointer to odin encoding tool
  Rndm::Numbers                  m_FlatDist;
  Gaudi::Property<bool>          m_modifyOdin{this, "ModifyOdin", false};
  Gaudi::Property<std::string>   m_genCollisionLocation{this, "GenCollisionLocation",
                                                      LHCb::GenCollisionLocation::Default};
  Gaudi::Property<std::vector<double>> m_thresInteraction{this, "ThresInteraction", {0.01, 0.03, 0.05}};
  Gaudi::Property<std::vector<double>> m_thresDiffractive{this, "ThresDiffractive", {0.1, 0.3, 0.6}};
  Gaudi::Property<std::vector<double>> m_thresElastic{this, "ThresElastic", {0.25, 0.5, 0.75}};
  Gaudi::Property<double>              m_threstrigger{this, "ThresTiggerType", 0.05};
  Gaudi::Property<bool>                m_odinRndTrig{this, "SetOdinRndTrigger", false};
};
//-----------------------------------------------------------------------------
// Implementation file for class : BooleInit
//
// 2005-12-15 : Marco Cattaneo
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( BooleInit )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
BooleInit::BooleInit( const std::string& name, ISvcLocator* pSvcLocator ) : LbAppInit( name, pSvcLocator ) {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode BooleInit::initialize() {
  return LbAppInit::initialize().andThen( [&] {
    // Private tool to plot the memory usage
    m_memoryTool.setEnabled( rootInTES().empty() );

    // Initialize thresholds if we want to modify Odin:
    if ( m_modifyOdin.value() ) {

      const auto ok = m_FlatDist.initialize( randSvc(), Rndm::Flat( 0., 1. ) );
      if ( !ok ) { info() << "Flat distribution could not be initialized" << endmsg; }

      info() << "Thresholds for Interaction processes : " << m_thresInteraction << endmsg;
      info() << "Thresholds for Diffractive processes : " << m_thresDiffractive << endmsg;
      info() << "Thresholds for Elastic processes : " << m_thresElastic << endmsg;
    }
  } );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode BooleInit::execute() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;
  increaseEventCounter();

  // Memory size check
  checkMem();

  // Plot the memory usage
  if ( m_memoryTool.isEnabled() ) m_memoryTool->execute();

  // Get the run and event number from the MC Header
  LHCb::MCHeader* evt = get<LHCb::MCHeader>( LHCb::MCHeaderLocation::Default, IgnoreRootInTES );
  if ( rootInTES().empty() ) printEventRun( evt->evtNumber(), evt->runNumber(), 0, evt->evtTime() );

  // Initialize the random number:

  if ( rootInTES().empty() ) {
    std::vector<long int> seeds = getSeeds( evt->runNumber(), evt->evtNumber() );
    auto                  sc    = this->initRndm( seeds );
    if ( sc.isFailure() ) return sc; // error printed already by initRndm

    // Create the Boole event header
    LHCb::ProcessHeader* header = new LHCb::ProcessHeader();
    header->setApplicationName( this->appName() );
    header->setApplicationVersion( this->appVersion() );
    header->setRunNumber( evt->runNumber() );
    header->setCondDBTags( this->condDBTags() );
    put( header, LHCb::ProcessHeaderLocation::Digi );
  }

  // Create an empty RawEvent
  LHCb::RawEvent* raw = new LHCb::RawEvent();
  put( raw, LHCb::RawEventLocation::Default );

  // Create ODIN
  LHCb::ODIN* odin = new LHCb::ODIN();
  put( odin, LHCb::ODINLocation::Default );

  // Fill ODIN from event header
  odin->setRunNumber( evt->runNumber() );
  odin->setEventNumber( evt->evtNumber() );
  odin->setEventTime( evt->evtTime() );

  // Simulate ODIN data id requested
  if ( m_modifyOdin.value() ) {
    modifyOdin( odin );
  } else {
    // put some reasonable defaults
    simpleOdin( odin );
  }

  // Create the Raw Bank
  m_odinTool->execute();

  return StatusCode::SUCCESS;
}

//=============================================================================
// modify ODIN bank
//=============================================================================
void BooleInit::simpleOdin( LHCb::ODIN* odin ) {

  // set the types
  LHCb::ODIN::TriggerType TriggerType  = LHCb::ODIN::TriggerType::LumiTrigger;
  LHCb::ODIN::BXTypes     BXType       = LHCb::ODIN::BXTypes::BeamCrossing;
  unsigned int            BunchCurrent = 8 + ( 8 << 4 );

  if ( msgLevel( MSG::DEBUG ) )
    debug() << "Bunch crossing type: " << BXType << " TriggerType " << TriggerType << " BunchCurrent  " << BunchCurrent
            << endmsg;

  odin->setBunchCrossingType( BXType );
  odin->setTriggerType( TriggerType );
  odin->setBunchCurrent( BunchCurrent );

  if ( m_odinRndTrig.value() ) {
    unsigned int EventType = 1 << 2;
    odin->setEventType( EventType );
  }
}

void BooleInit::modifyOdin( LHCb::ODIN* odin ) {

  // Get info from Gen
  LHCb::GenCollisions* Collisions    = get<LHCb::GenCollisions>( m_genCollisionLocation, IgnoreRootInTES );
  int                  numCollisions = Collisions->size();
  if ( msgLevel( MSG::DEBUG ) ) debug() << "Number of Collisions: " << numCollisions << endmsg;

  int interaction = 0;

  for ( const auto& process : *Collisions ) {
    int type = process->processType();

    if ( msgLevel( MSG::DEBUG ) ) {
      debug() << "Collision type: " << type << endmsg;
      if ( numCollisions == 1 ) {
        if ( type == 91 ) {
          debug() << "Single Proton Elastic collision!" << endmsg;
        } else if ( type == 92 || type == 93 ) {
          debug() << "Single Diffractive collision!" << endmsg;
        }
      }
    }

    if ( type == 91 ) {
    } else if ( type == 92 || type == 93 ) {
      interaction = std::max( 1, interaction );
    }

    else {
      interaction = std::max( 2, interaction );
    };
  };

  if ( msgLevel( MSG::DEBUG ) ) debug() << "Interaction class " << interaction << endmsg;

  // Random number for BX type and Trigger type
  double randNumber     = m_FlatDist.shoot();
  double trigRandNumber = m_FlatDist.shoot();
  if ( msgLevel( MSG::DEBUG ) ) debug() << "Random number:" << randNumber << endmsg;

  // set the types
  LHCb::ODIN::TriggerType TriggerType  = LHCb::ODIN::TriggerType::LumiTrigger;
  LHCb::ODIN::BXTypes     BXType       = LHCb::ODIN::BXTypes::NoBeam;
  unsigned int            BunchCurrent = 0;
  if ( interaction == 2 ) {
    if ( trigRandNumber > m_threstrigger ) {
      BXType = LHCb::ODIN::BXTypes::BeamCrossing;
    } else {
      if ( randNumber < m_thresInteraction[0] ) BXType = LHCb::ODIN::BXTypes::NoBeam;
      if ( randNumber > m_thresInteraction[0] && randNumber < m_thresInteraction[1] )
        BXType = LHCb::ODIN::BXTypes::Beam1;
      if ( randNumber > m_thresInteraction[1] && randNumber < m_thresInteraction[2] )
        BXType = LHCb::ODIN::BXTypes::Beam2;
      if ( randNumber > m_thresInteraction[2] ) BXType = LHCb::ODIN::BXTypes::BeamCrossing;
    }
  }

  if ( interaction == 1 ) {
    if ( randNumber < m_thresDiffractive[0] ) BXType = LHCb::ODIN::BXTypes::NoBeam;
    if ( randNumber > m_thresDiffractive[0] && randNumber < m_thresDiffractive[1] ) BXType = LHCb::ODIN::BXTypes::Beam1;
    if ( randNumber > m_thresDiffractive[1] && randNumber < m_thresDiffractive[2] ) BXType = LHCb::ODIN::BXTypes::Beam2;
    if ( randNumber > m_thresDiffractive[2] ) BXType = LHCb::ODIN::BXTypes::BeamCrossing;
    // with random number some will be Reserve
  }
  if ( interaction == 0 ) {
    if ( randNumber < m_thresElastic[0] ) BXType = LHCb::ODIN::BXTypes::NoBeam;
    if ( randNumber > m_thresElastic[0] && randNumber < m_thresElastic[1] ) BXType = LHCb::ODIN::BXTypes::Beam1;
    if ( randNumber > m_thresElastic[1] && randNumber < m_thresElastic[2] ) BXType = LHCb::ODIN::BXTypes::Beam2;
    if ( randNumber > m_thresElastic[2] ) BXType = LHCb::ODIN::BXTypes::BeamCrossing;
    // with random number some will be Reserve
  }

  if ( BXType == LHCb::ODIN::BXTypes::BeamCrossing ) BunchCurrent = 8 + ( 8 << 4 ); // some current..
  if ( BXType == LHCb::ODIN::BXTypes::Beam1 ) BunchCurrent = 8;                     // some current..
  if ( BXType == LHCb::ODIN::BXTypes::Beam2 ) BunchCurrent = ( 8 << 4 );            // some current..

  if ( msgLevel( MSG::DEBUG ) )
    debug() << "Bunch crossing type: " << BXType << " TriggerType " << TriggerType << " BunchCurrent  " << BunchCurrent
            << endmsg;

  odin->setBunchCrossingType( BXType );
  odin->setTriggerType( TriggerType );
  odin->setBunchCurrent( BunchCurrent );

  if ( m_odinRndTrig.value() ) {
    unsigned int EventType = 1 << 2;
    odin->setEventType( EventType );
  }
}
//=============================================================================
