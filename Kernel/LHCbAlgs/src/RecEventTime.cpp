#include "RecEventTime.h"

#include <functional>

#include "Event/RecHeader.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/Time.icpp"

class IInterface;

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
