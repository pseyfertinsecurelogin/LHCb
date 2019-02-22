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
#include "TimeDecoderList.h"

// Constructor
TimeDecoderList::TimeDecoderList( const std::string& type, const std::string& name, const IInterface* parent )
    : base_class( type, name, parent ) {
  declareProperty( "Decoders", m_decoderNames, "List of IEventTimeDecoder tools to use" );
}

StatusCode TimeDecoderList::initialize() {
  const StatusCode sc = base_class::initialize();
  if ( sc.isFailure() ) return sc;

  m_decoders.clear();
  for ( const auto& name : m_decoderNames ) {
    IEventTimeDecoder* p = tool<IEventTimeDecoder>( name, this );
    if ( p ) {
      m_decoders.push_back( p );
    } else {
      return Error( "Cannot retrieve " + name );
    }
  }

  return sc;
}

Gaudi::Time TimeDecoderList::getTime() const {
  Gaudi::Time t;
  for ( const auto& d : m_decoders ) {
    if ( ( t = d->getTime() ).ns() ) { return t; }
  }
  return Gaudi::Time::epoch();
}

DECLARE_COMPONENT( TimeDecoderList )
