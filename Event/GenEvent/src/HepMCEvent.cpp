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
// -------------------------------------------------------------------------
// File: Event/HepMCEvent.cpp
//
// AuthorList:
//
//         W. Pokorski: July 02
// -------------------------------------------------------------------------

#include "Event/HepMCEvent.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

// -------------------------------------------------------------------------
LHCb::HepMCEvent::HepMCEvent( const LHCb::HepMCEvent& evt )
    : KeyedObject<int>()
// -------------------------------------------------------------------------
{
  // deep copy
  *this = evt;
}

// -------------------------------------------------------------------------
LHCb::HepMCEvent& LHCb::HepMCEvent::operator=( const LHCb::HepMCEvent& evt )
// -------------------------------------------------------------------------
{
  if ( this != &evt ) {
    m_generatorName            = evt.generatorName();
    const HepMC::GenEvent* ptr = evt.pGenEvt();
    delete m_pGenEvt;
    m_pGenEvt = new HepMC::GenEvent( ptr->signal_process_id(), ptr->event_number() );
  }
  return *this;
}
