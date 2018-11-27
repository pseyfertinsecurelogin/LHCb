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
#include "Kernel/LHCbID.h"

//-----------------------------------------------------------------------------
// Implementation file for class : LHCbID
//
// 2007-07-11 : Chris Jones
//-----------------------------------------------------------------------------

std::ostream& LHCb::LHCbID::fillStream(std::ostream& s) const
{
  s << "{ LHCbID : ";
  if (isVelo()) { s << veloID(); }
  if (isVP())   { s << vpID();   }
  if (isST())   { s << stID();   }
  if (isUT())   { s << utID();   }
  if (isOT())   { s << otID();   }
  if (isRich()) { s << richID(); }
  if (isCalo()) { s << caloID(); }
  if (isMuon()) { s << muonID(); }
  if (isFT())   { s << ftID();   }
  return s << " }";
}
