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
#include "Kernel/STDetSwitch.h"

void STDetSwitch::flip( std::string aType, std::string& aString ) {

  std::string::size_type iChar = 0;

  if ( aType == "TT" ) {
    while ( ( iChar = aString.find( "IT" ) ) != std::string::npos ) { aString.replace( iChar, 2, aType ); }
  }

  if ( aType == "IT" ) {
    while ( ( iChar = aString.find( "TT" ) ) != std::string::npos ) { aString.replace( iChar, 2, aType ); }
  }
}
