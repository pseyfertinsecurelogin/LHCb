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
#include "RecEventTime.h"

#include "Event/RecHeader.h"

RecEventTime::RecEventTime( const std::string& type, const std::string& name, const IInterface* parent )
    : base_class( type, name, parent ) {
  declareProperty( "RecHeaderLocation", m_recHeaderLoc = LHCb::RecHeaderLocation::Default, "Location for RecHeader" );
}

Gaudi::Time RecEventTime::getTime() const {
  // If the rec header is available, return its event time
  auto* h = getIfExists<LHCb::RecHeader>( evtSvc(), m_recHeaderLoc );
  // return the time
  return ( h ? h->gpsTime() * 1000 : Gaudi::Time::epoch() );
}

DECLARE_COMPONENT( RecEventTime )
