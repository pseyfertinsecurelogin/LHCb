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
#include "GaudiAlg/GaudiSequencer.h"
#include "GaudiKernel/IJobOptionsSvc.h"

//------------------------------------------------------------------------------
//
// Implementation of class :  ProcessPhase
//
// Author:      Marco Cattaneo
// Created:     17th December 1999
//------------------------------------------------------------------------------

/** @class ProcessPhase ProcessPhase.h LHCbAlgs/ProcessPhase.h
 *
 * Processing phase of LHCb application
 *
 * Creates and invokes subdetector processing algorithms
 * Convention: algorithm name = \<phase name\> + \<detector name\>
 *
 * @author: Marco Cattaneo
 * @date:   17th December 1999
 */

class ProcessPhase final : public GaudiSequencer {

public:
  using GaudiSequencer::GaudiSequencer;

  StatusCode initialize() override; ///> Create and initialise sequences of this phase

private:
  Gaudi::Property<std::vector<std::string>> m_detList{this, "DetectorList", {}}; ///> List of subdetectors to be
                                                                                 /// processed
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( ProcessPhase )

StatusCode ProcessPhase::initialize() {
  std::string myMeasureProp;

  auto sc = getProperty( "MeasureTime", myMeasureProp );
  if ( !sc ) return sc;

  IJobOptionsSvc* jobSvc = svc<IJobOptionsSvc>( "JobOptionsSvc" );

  // Declare sequences to the phase
  std::string myMembers = "{";
  for ( auto it = m_detList.begin(); it != m_detList.end(); ++it ) {
    if ( m_detList.begin() != it ) myMembers += ",";
    const auto algName = name() + ( *it ) + "Seq";
    myMembers += "\"GaudiSequencer/" + algName + "\"";
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
