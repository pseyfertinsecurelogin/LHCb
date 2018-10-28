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

#include "Event/STTELL1BoardErrorBank.h"
#include <iostream>
#include <string>
#include <numeric>

#include "GaudiKernel/IRegistry.h"

#include "Kernel/STBoardMapping.h"

//-----------------------------------------------------------------------------
// Implementation file for class : STChannelID
//
// 2008-06-07 : M Needham
//-----------------------------------------------------------------------------

double LHCb::STTELL1BoardErrorBank::fractionOK(const unsigned int pcn) const{

  auto  frac = std::accumulate( m_errorInfo.begin(), m_errorInfo.end(), 0.0,
                                [&](double f, const auto* eI)
                                { return f+eI->fractionOK(pcn); } );
  return frac/m_errorInfo.size();
}

std::string LHCb::STTELL1BoardErrorBank::regName() const{

  // get the name of the container in the TES
  const ObjectContainerBase* parentCont = parent();
  const IRegistry* reg =  parentCont->registry();
  return reg->identifier();
}

bool LHCb::STTELL1BoardErrorBank::isIT() const {
  return regName().find("IT") != std::string::npos;
}


bool LHCb::STTELL1BoardErrorBank::isTT() const {
  return regName().find("TT") != std::string::npos;
}

std::ostream& LHCb::STTELL1BoardErrorBank::fillStream(std::ostream& s) const
{
  s << "Error bank for TELL1 board source "  << key() << '\n';
  if (isIT() ) {
    s << STBoardMapping::find(key(), STBoardMapping::ITSourceIDToNumberMap()) << '\n';
  }
  else if (isTT() ){
    s << STBoardMapping::find(key(), STBoardMapping::TTSourceIDToNumberMap()) << '\n';
  }

  unsigned int pp = 0;
  for(const auto& eI : m_errorInfo ) {
    s << "pp "  << pp++ << std::endl;
    s << *eI << '\n';
  }

  return s << std::endl;
}
