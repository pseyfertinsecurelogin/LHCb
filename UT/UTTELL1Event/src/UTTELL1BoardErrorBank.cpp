
#include "Event/UTTELL1BoardErrorBank.h"
#include <iostream>
#include <string>
#include <numeric>

#include "GaudiKernel/IRegistry.h"

#include "Kernel/UTBoardMapping.h"

//-----------------------------------------------------------------------------
// Implementation file for class : UTChannelID
//
// 2008-06-07 : M Needham
//-----------------------------------------------------------------------------

double LHCb::UTTELL1BoardErrorBank::fractionOK(const unsigned int pcn) const{

  auto  frac = std::accumulate( m_errorInfo.begin(), m_errorInfo.end(), 0.0,
                                [&](double f, const auto* eI)
                                { return f+eI->fractionOK(pcn); } );
  return frac/m_errorInfo.size();
}

std::string LHCb::UTTELL1BoardErrorBank::regName() const{

  // get the name of the container in the TES
  const ObjectContainerBase* parentCont = parent();
  const IRegistry* reg =  parentCont->registry();
  return reg->identifier();
}

bool LHCb::UTTELL1BoardErrorBank::isUT() const {
  return regName().find("UT") != std::string::npos;
}

std::ostream& LHCb::UTTELL1BoardErrorBank::fillStream(std::ostream& s) const
{
  s << "Error bank for TELL1 board source "  << key() << '\n';
  s << UTBoardMapping::find(key(), UTBoardMapping::UTSourceIDToNumberMap()) << '\n';


  unsigned int pp = 0;
  for(const auto& eI : m_errorInfo ) {
    s << "pp "  << pp++ << std::endl;
    s << *eI << '\n';
  }

  return s << std::endl;
}
