
// local
#include "ApplicationVersionFilter.h"

#include <boost/regex/v4/basic_regex.hpp>
#include <boost/regex/v4/match_flags.hpp>
#include <boost/regex/v4/perl_matcher_common.hpp>
#include <boost/regex/v4/perl_matcher_non_recursive.hpp>
#include <boost/regex/v4/regex_fwd.hpp>
#include <boost/regex/v4/regex_match.hpp>
#include <boost/regex/v4/regex_traits.hpp>
#include <algorithm>
#include <functional>
#include <vector>

#include "Event/ProcessHeader.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"

class ISvcLocator;

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
DECLARE_ALGORITHM_FACTORY( ApplicationVersionFilter )

//=============================================================================
