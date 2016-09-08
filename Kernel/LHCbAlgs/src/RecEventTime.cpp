#include "RecEventTime.h"

#include "Event/RecHeader.h"

RecEventTime::RecEventTime( const std::string & type,
                            const std::string & name,
                            const IInterface *parent ) :
  base_class(type, name, parent)
{
  declareProperty("RecHeaderLocation",
                  m_recHeaderLoc = LHCb::RecHeaderLocation::Default,
                  "Location for RecHeader" );
}

Gaudi::Time RecEventTime::getTime() const
{
  // If the rec header is available, return its event time
  auto * h = getIfExists<LHCb::RecHeader>(evtSvc(), m_recHeaderLoc);
  // return the time
  return ( h ? h->gpsTime() * 1000 : Gaudi::Time::epoch() );
}

DECLARE_TOOL_FACTORY(RecEventTime)
