#include "TimeDecoderList.h"

// Constructor
TimeDecoderList::TimeDecoderList( const std::string & type,
                                  const std::string & name, 
                                  const IInterface *parent )
  : base_class(type, name, parent)
{
  declareProperty("Decoders", m_decoderNames,
                  "List of IEventTimeDecoder tools to use");
}

StatusCode TimeDecoderList::initialize()
{
  const StatusCode sc = base_class::initialize();
  if (sc.isFailure()) return sc;

  m_decoders.clear();
  for ( const auto & name : m_decoderNames )
  {
    IEventTimeDecoder * p = tool<IEventTimeDecoder>( name, this);
    if ( p ) { m_decoders.push_back(p); }
    else 
    {
      return Error( "Cannot retrieve " + name );
    }
  }
  
  return sc;
}

Gaudi::Time TimeDecoderList::getTime() const
{
  Gaudi::Time t;
  for ( const auto & d : m_decoders )
  {
    if ( ( t = d->getTime() ).ns() ) { return t; }
  }
  return Gaudi::Time::epoch();
}

DECLARE_TOOL_FACTORY(TimeDecoderList)
