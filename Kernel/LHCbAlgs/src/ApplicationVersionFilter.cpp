/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

// local
#include "ApplicationVersionFilter.h"

//-----------------------------------------------------------------------------
// Implementation file for class : ApplicationVersionFilter
//
// 2009-11-06 : Chris Jones
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
ApplicationVersionFilter::ApplicationVersionFilter( const std::string& name,
                                                    ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "VersionRegex",   m_regex = "" );
  declareProperty( "HeaderLocation", m_loc = LHCb::ProcessHeaderLocation::Rec );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode ApplicationVersionFilter::execute()
{
  bool OK = true;

  try
  {
    if ( !m_regex.empty() )
    {
      const auto * header = getIfExists<LHCb::ProcessHeader>(m_loc);
      if ( header )
      {
        // Apply the regex to the application version
        OK = boost::regex_match( header->applicationVersion().c_str(),
                                 boost::regex(m_regex) );
        if ( msgLevel(MSG::DEBUG) )
          debug() << "Application Version = " << header->applicationVersion()
                  << " Regex = " << m_regex
                  << " Match = " << OK
                  << endmsg;
      }
      else
      {
        OK = false;
      }
    }
  }
  catch ( const GaudiException & )
  {
    OK = false;
  }

  setFilterPassed(OK);

  return StatusCode::SUCCESS;
}

//=============================================================================

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( ApplicationVersionFilter )

//=============================================================================
