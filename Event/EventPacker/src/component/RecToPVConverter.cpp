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
#include "Event/PrimaryVertex.h"
#include "Event/RecVertex.h"
#include "GaudiAlg/GaudiAlgorithm.h"

class RecToPVConverter : public GaudiAlgorithm {
public:
  RecToPVConverter( const std::string&, ISvcLocator* );
  StatusCode execute() override;

private:
  std::string m_inputName;
  std::string m_outputName;
  bool        m_doFit;
};

DECLARE_COMPONENT( RecToPVConverter )

//-----------------------------------------------------------------------------
// Implementation file for class : RecToPVConverter
//
// 2015-09-25 : Wouter Hulsbergen
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
RecToPVConverter::RecToPVConverter( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {
  declareProperty( "InputLocation", m_inputName = LHCb::RecVertexLocation::Primary );
  declareProperty( "OutputLocation", m_outputName = LHCb::PrimaryVertexLocation::Default );
  declareProperty( "DoFit", m_doFit = false );
}

//=============================================================================
// Main execution
//=============================================================================

StatusCode RecToPVConverter::execute() {
  const auto recvertices  = get<LHCb::RecVertex::Range>( m_inputName );
  auto       slimvertices = new LHCb::PrimaryVertex::Container();
  put( slimvertices, m_outputName );
  for ( const auto& recpv : recvertices ) { slimvertices->insert( new LHCb::PrimaryVertex( *recpv, m_doFit ) ); }

  return StatusCode::SUCCESS;
}
