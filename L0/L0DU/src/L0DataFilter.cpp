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
// Include files

// from Event
#include "Event/L0DUReport.h"

// local
#include "L0DataFilter.h"

//-----------------------------------------------------------------------------
// Implementation file for class : L0DataFilter
//
// 2012-04-10 : Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( L0DataFilter )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
L0DataFilter::L0DataFilter( const std::string& name,
                    ISvcLocator* pSvcLocator)
  : L0AlgBase ( name , pSvcLocator )
{
  declareProperty("L0DULocation"   , m_l0Location = LHCb::L0DUReportLocation::Default );
  declareProperty("Selection"   , m_selection );
  declareProperty("Logical"     , m_logical = "AND" ); // "AND" or "OR"
  declareProperty("Revert"         , m_revert = false );
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode L0DataFilter::initialize() {
  StatusCode sc = L0AlgBase::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;

  if( m_selection.empty()) info() << "No selection is defined - filter has no effect" << endmsg;

  // check property
  info() << "L0DU data selection : " << endmsg;
  unsigned int k=1;
  if( m_logical != "AND" && m_logical != "OR")return Error("Logical must be 'AND' or 'OR'",StatusCode::FAILURE);
  for(const auto& i : m_selection) {
    if( (i.second).size() != 2)return Error("Selection syntax is : 'DataName' : {'comparator','threshold'}",StatusCode::FAILURE);
    const std::string& name = i.first;
    const std::string& comp = i.second[0];
    const std::string& st = i.second[1];
    if( comp != ">" && comp != "<" && comp != ">=" && comp != "<=" && comp != "==")
      return Error("Comparator must be '>', '>=', '<', '<=' or '=='",StatusCode::FAILURE);
    info() << "  - ("<<name<<" "<<comp<<" "<<st<<") " ;
    if( k < m_selection.size()) info() << m_logical << endmsg;
    else info() << endmsg;
    k++;
  }
  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode L0DataFilter::execute() {

  if( m_selection.empty()){
    setFilterPassed( true  );
    return StatusCode::SUCCESS;
  }

  std::string loc = dataLocation( m_l0Location );
  const  LHCb::L0DUReport* l0 = getIfExists<LHCb::L0DUReport>( loc );
  if( !l0 ){
    counter("Report not found") += 1;
    setFilterPassed( false );
    return Warning("L0DUReport not found at location : '" + loc + "' - the event is rejected", StatusCode::SUCCESS );
  }

  //===== Processing
  bool oSel = false;
  bool aSel = true;
  for(const auto& i : m_selection) {
    const std::string& name = i.first;
    const std::string& comp = i.second[0];
    const std::string& st = i.second[1];
    int threshold;
    std::istringstream is( st.c_str() );
    is >> threshold;
    int value = l0->dataDigit( name );
    bool sel = false;
    if (comp == ">")       sel = (value > threshold);
    else if (comp == ">=") sel = (value >= threshold);
    else if (comp == "<")  sel = (value < threshold);
    else if (comp == "<=") sel = (value <= threshold);
    else if (comp == "==") sel = (value == threshold);
    if (sel) oSel = true;
    aSel = aSel & sel;
    counter( name +" "+comp+" "+st)+=(int)sel;
  }

  bool accept = true;
  if( m_logical == "AND") accept = ( m_revert ? !aSel : aSel );
  else if( m_logical == "OR") accept = ( m_revert ? !oSel : oSel );
  counter(" -> ALL (AND) ") += (int) aSel;
  counter(" -> ALL (OR)"  ) += (int) oSel;
  counter(" -> Accept") += (int) accept;


  setFilterPassed( accept );

  return StatusCode::SUCCESS;
}

//=============================================================================
