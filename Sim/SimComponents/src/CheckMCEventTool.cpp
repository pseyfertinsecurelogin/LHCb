// Include files

#include <functional>

// local
#include "CheckMCEventTool.h"
// Event model
#include "Event/MCHeader.h"
#include "Event/MCParticle.h"
#include "Event/MCVertex.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/IInterface.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/SmartRef.h"
#include "Kernel/ICheckTool.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CheckMCEventTool
//
// 2003-02-24 : Marco Cattaneo
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_TOOL_FACTORY(CheckMCEventTool)

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CheckMCEventTool::CheckMCEventTool( const std::string& type,
                                    const std::string& name,
                                    const IInterface* parent )
: base_class ( type, name , parent )
{
  declareInterface<ICheckTool>(this);
}

StatusCode CheckMCEventTool::check()
{
  // If already processed this event, return previous status
  LHCb::MCHeader* evt = get<LHCb::MCHeader>( LHCb::MCHeaderLocation::Default );
  if( m_event == evt->evtNumber() ) return m_status;

  m_event  = evt->evtNumber();
  m_status = StatusCode::SUCCESS;

  // Loop over MCVertices
  LHCb::MCVertices*  mcVerts =
    get<LHCb::MCVertices>( LHCb::MCVertexLocation::Default );
  for(const auto& itV : *mcVerts ) {
    for(const auto& itP : itV->products() ) {
      // Check that mother of daughter corresponds...
      if( !itP->originVertex() ) {
        err() << "Inconsistent mother-daughter relation!!"
              << "  Particle " << itP->key()
              << " is daughter of vertex " << itV->key()
              << " but has no origin vertex" << endmsg;
        m_status = StatusCode::FAILURE;
      } else if( itP->originVertex()->key() != itV->key() ) {
        err() << "Inconsistent mother-daughter relation!!"
              << MSG::ERROR << "  Particle " << itP->key()
              << " is daughter of vertex " << itV->key()
              << " but has vertex " << itP->originVertex()->key()
              << " as mother" << endmsg;
        m_status = StatusCode::FAILURE;
      }
    }
  }
  return m_status;
}

//=============================================================================
