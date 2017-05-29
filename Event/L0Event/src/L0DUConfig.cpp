// Include files
#include <sstream>
#include <string>
#include <utility>

#include "Event/L0DUChannel.h"
#include "Event/L0DUConfig.h"
#include "Event/L0DUDecision.h"
#include "Event/L0DUTrigger.h"

//--------------------------------------------------------------------------
// Auxilliary Implementation file for L0DUConfig Event classes
//
// 19/10/2007 : Olivier Deschamps
//-----------------------------------------------------------------------------



std::string LHCb::L0DUConfig::summary(){

  std::ostringstream s(" ");
  s << " \n"
    << " <-----  L0DUConfig (" << key()   << " ) " << " --------------->\n";

  int mask = LHCb::L0DUDecision::Any;
  int typ  = 0x1;
  while(mask != 0x0){
    s << "   Emulated Decision (" << LHCb::L0DUDecision::Name[typ] << "): " << emulatedDecision(typ) << '\n';
    typ  = typ  << 1;
    mask = mask >> 1;
  }
  s << "   Recipe name : '" << m_recipe << "'\n"
    << "   Algorithm description : " << m_definition << '\n';
  for(const auto& cond : m_channels) s << cond.second->summary() << '\n';
  s << " ======= Trigger Sets ";
  if( !triggers().empty() ){
    s << " --> " << triggers().size() << " Trigger sets are defined : " << '\n';
    for(const auto& t : triggers()) {
      s << t.second->summary();
    }
  }
  return s.str();
}


std::string LHCb::L0DUConfig::description() const {
  std::ostringstream s(" ");
  s << " \n"
    << " <-----  L0DUConfig (" << key()   << " ) " << " --------------->\n"
    << "   Recipe name : '" << m_recipe << "'\n"
    << "   Description : " << m_definition <<'\n';
  for(const auto& cond : m_channels) s << cond.second->description() << '\n';
  s << " ======= Trigger Sets ";
  if(!triggers().empty()){
    s << " --> " << triggers().size() << " SubTrigger(s) are defined : \n";
    for(const auto& t : triggers()) s << t.second->description() ;
  }
  s << '\n';
  return s.str();
}

