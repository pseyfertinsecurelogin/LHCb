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
// $Id: ProcessPhase.cpp,v 1.7 2007-05-31 12:54:59 cattanem Exp $

// Include files
#include "ProcessPhase.h"
#include "GaudiKernel/IJobOptionsSvc.h"

//------------------------------------------------------------------------------
//
// Implementation of class :  ProcessPhase
//
// Author:      Marco Cattaneo
// Created:     17th December 1999
//------------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( ProcessPhase )


ProcessPhase::ProcessPhase( const std::string& name, ISvcLocator* pSvcLocator )
 : GaudiSequencer( name, pSvcLocator ) {
	m_detList.clear();
	declareProperty( "DetectorList", m_detList );
}

StatusCode ProcessPhase::initialize() 
{
  std::string myMeasureProp;

  auto sc = getProperty( "MeasureTime", myMeasureProp );
  if ( !sc ) return sc;

  IJobOptionsSvc* jobSvc = svc<IJobOptionsSvc>("JobOptionsSvc");
  
	// Declare sequences to the phase
  std::string myMembers = "{";
	for ( auto it = m_detList.begin(); it != m_detList.end(); ++it )
  {
    if ( m_detList.begin() != it ) myMembers += ",";
    const auto algName = name() + (*it) + "Seq";
    myMembers +=  "\"GaudiSequencer/" + algName + "\"";
    // Sequences are not yet instantiated, so set MeasureTime property directly 
    // in the catalogue. Uses same value as the parent ProcessPhase
    Gaudi::Property<bool> p( "MeasureTime", false );
    p.fromString( myMeasureProp ).ignore();
    jobSvc->addPropertyToCatalogue( algName, p ).ignore();
  }
  myMembers += "}";
  setProperty( "Members", myMembers ).ignore();
  release( jobSvc ).ignore();

  return GaudiSequencer::initialize();
}
