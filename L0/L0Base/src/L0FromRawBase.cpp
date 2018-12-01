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

// from Gaudi
#include "Event/RawEvent.h"

// local
#include "L0Base/L0FromRawBase.h"

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
: Decoder::AlgBase ( name , pSvcLocator )
{
  m_rawEventLocations= {LHCb::RawEventLocation::Trigger, LHCb::RawEventLocation::Default };
  initRawEventSearch();
  m_useRootInTES = IgnoreRootInTES;
}
//=============================================================================
// Select the raw event location
//=============================================================================
StatusCode L0FromRawBase::selectRawEventLocation(std::string &location)
{

  for (const auto & loc : m_rawEventLocations) {
    if (exist<LHCb::RawEvent>( loc, IgnoreRootInTES )) {
      m_useRootInTES = IgnoreRootInTES;
      location = loc;
      break;
    }
    if (exist<LHCb::RawEvent>( loc , UseRootInTES )) {
      m_useRootInTES = UseRootInTES;
      location = loc;
      break;
    }
  }

  m_statusOnTES = (location==LHCb::RawEventLocation::Default);

  return location.empty() ? Error("No valid raw event location found",StatusCode::FAILURE,50)
                          : StatusCode::SUCCESS;
}
