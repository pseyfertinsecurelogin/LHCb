// Include files 

#include <functional>
#include <string>
#include <vector>

#include "DAQKernel/DecoderAlgBase.h"
// from Gaudi
#include "Event/RawEvent.h"
#include "GaudiAlg/GaudiCommon.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/StatusCode.h"
// local
#include "L0Base/L0FromRawBase.h"

class ISvcLocator;

//-----------------------------------------------------------------------------
// Implementation file for class : L0FromRawBase
//
// 2009-10-20 : Julien Cogan
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
L0FromRawBase::L0FromRawBase( const std::string& name,
                              ISvcLocator* pSvcLocator)
  : Decoder::AlgBase ( name , pSvcLocator ),
    m_statusOnTES  ( true )
{
  m_rawEventLocations= {LHCb::RawEventLocation::Trigger, LHCb::RawEventLocation::Default };
  initRawEventSearch();
  
  declareProperty("WriteProcData"  , m_writeProcData   = false);
  declareProperty("WriteOnTES"  , m_writeOnTES   = true);
  declareProperty("L0Context"   , m_l0context   = "");
  m_useRootInTES = IgnoreRootInTES;
}
//=============================================================================
// Select the raw event location
//=============================================================================
StatusCode L0FromRawBase::selectRawEventLocation(std::string &location)
{
  
  for (std::vector<std::string>::iterator it_rawEventLocations = m_rawEventLocations.begin(); 
       it_rawEventLocations < m_rawEventLocations.end(); 
       ++it_rawEventLocations) {

    if (exist<LHCb::RawEvent>( *it_rawEventLocations , IgnoreRootInTES )) {
      m_useRootInTES = IgnoreRootInTES;
      location = *it_rawEventLocations;
      break;
    }

    if (exist<LHCb::RawEvent>( *it_rawEventLocations , UseRootInTES )) {
      m_useRootInTES = UseRootInTES;
      location = *it_rawEventLocations;
      break;
    }

  }

  m_statusOnTES = (location==LHCb::RawEventLocation::Default);

  if (location.empty()) { 
    return Error("No valid raw event location found",StatusCode::FAILURE,50);
  }

  return StatusCode::SUCCESS;
}
