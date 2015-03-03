// $Id: ProcessPhase.cpp,v 1.5 2006-09-04 15:25:11 cattanem Exp $

// Include files
#include "ProcessPhase.h"
#include "GaudiKernel/AlgFactory.h" 
#include "GaudiKernel/IJobOptionsSvc.h"

//------------------------------------------------------------------------------
//
// Implementation of class :  ProcessPhase
//
// Author:      Marco Cattaneo
// Created:     17th December 1999
//------------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( ProcessPhase );


ProcessPhase::ProcessPhase( const std::string& name, ISvcLocator* pSvcLocator )
 : GaudiSequencer( name, pSvcLocator ) {
	m_detList.clear();
	declareProperty( "DetectorList", m_detList );
  setProperty( "MeasureTime", "true" ); // Overrides GaudiSequencer default
}

ProcessPhase::~ProcessPhase() { 
}

StatusCode ProcessPhase::initialize() {

  std::string myMeasureProp;
  getProperty( "MeasureTime", myMeasureProp );
  IJobOptionsSvc* jobSvc = svc<IJobOptionsSvc>("JobOptionsSvc");

	// Declare sequences to the phase
  std::string myMembers = "{";
	for ( VectorName::iterator it=m_detList.begin(); it != m_detList.end(); it++){
    if( m_detList.begin() != it ) myMembers += ",";
    std::string algName = name() + (*it) + "Seq";
    myMembers +=  "\"GaudiSequencer/" + algName + "\"";
    // Sequences are not yet instantiated, so set MeasureTime property directly 
    // in the catalogue. Uses same value as the parent ProcessPhase
    StringProperty p( "MeasureTime", myMeasureProp );
    jobSvc->addPropertyToCatalogue( algName, p );
  }
  myMembers += "}";
  setProperty( "Members", myMembers );
  release( jobSvc );

  return GaudiSequencer::initialize();
}
