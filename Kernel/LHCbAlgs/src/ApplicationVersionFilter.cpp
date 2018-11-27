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
#include "GaudiAlg/FilterPredicate.h"
#include "Event/ProcessHeader.h"
#include <regex>

/** @class ApplicationVersionFilter ApplicationVersionFilter.h
 *
 *  Checks the application version from the given ProcessHeader,
 *  comparing it to a given regex pattern to pass/reject the event.
 *
 *  @author Chris Jones
 *  @date   18-04-2013
 */
class ApplicationVersionFilter final
: public Gaudi::Functional::FilterPredicate<bool(const LHCb::ProcessHeader&)>
{
public:

  /// Standard constructor
  ApplicationVersionFilter( const std::string& name, ISvcLocator* pSvcLocator )
  : FilterPredicate{ name , pSvcLocator,
                     KeyValue{ "HeaderLocation", LHCb::ProcessHeaderLocation::Rec } }
  {}

  bool operator()(const LHCb::ProcessHeader& header) const override {
    // Apply the regex to the application version
    bool pass = ( m_str.empty() ||
                  std::regex_match( header.applicationVersion(), m_regex ) );
    if ( msgLevel(MSG::DEBUG) )
      debug() << "Application Version = " << header.applicationVersion()
              << " Regex = " << m_str
              << " Match = " << pass
              << endmsg;
    return pass;
  }

private:

  std::regex m_regex;
  Gaudi::Property<std::string> m_str
  { this, "VersionRegex", "", [=](const auto&) { this->m_regex = this->m_str.value(); },
    "Version match regex" };

};

//=============================================================================

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( ApplicationVersionFilter )

//=============================================================================
