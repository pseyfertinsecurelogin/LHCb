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

