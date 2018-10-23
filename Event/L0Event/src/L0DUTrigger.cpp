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
// Include files
#include <utility>
#include <string>
#include <map>
#include <sstream>

#include "GaudiKernel/SerializeSTL.h"
#include "Event/L0DUTrigger.h"

//-----------------------------------------------------------------------------
// Auxilliary Implementation file for L0DUTrigger Event classes
//
// 09/02/2006 : Olivier Deschamps
//-----------------------------------------------------------------------------


std::string LHCb::L0DUTrigger::summary(){

  std::ostringstream s(" ");
  s << description()
    << "  ===== <** EMULATED DECISION **> = <"<< emulatedDecision() << ">" << std::endl;
  return s.str();


}

std::string LHCb::L0DUTrigger::description(){
  std::ostringstream s;
  GaudiUtils::details::ostream_joiner( s << "  ====  Trigger '"
                                         << L0DUDecision::Name[m_decisionType] << "|"
                                         << m_name
                                         << "' = [",
                                       m_channels, " || ",
                                       [](std::ostream& os, const auto& c) -> std::ostream&
                                       {
                                           return os << L0DUDecision::Name[c.second->decisionType()]
                                                     << "|" << c.second->name()  ;
                                       } ) << "]\n" ;
  return s.str();
}

