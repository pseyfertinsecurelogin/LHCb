// -------------------------------------------------------------------------
// File: Event/HepMCEvent.cpp
//
// AuthorList:
//
//         W. Pokorski: July 02 
// -------------------------------------------------------------------------
 
#include "Event/HepMCEvent.h"

// -------------------------------------------------------------------------
LHCb::HepMCEvent::HepMCEvent(const LHCb::HepMCEvent& evt) : KeyedObject<int>()
// -------------------------------------------------------------------------
{
  // deep copy
  *this = evt;
}

// -------------------------------------------------------------------------
LHCb::HepMCEvent& LHCb::HepMCEvent:: operator = (const LHCb::HepMCEvent& evt)
// -------------------------------------------------------------------------
{
  if(this != &evt) {
     m_generatorName = evt.generatorName();
     const HepMC::GenEvent* ptr = evt.pGenEvt();
     delete m_pGenEvt;
     m_pGenEvt = new HepMC::GenEvent(ptr->signal_process_id(),
              ptr->event_number());
  }
  return *this;
}

