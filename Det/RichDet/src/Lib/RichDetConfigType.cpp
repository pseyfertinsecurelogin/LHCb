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

// local
#include "RichDet/RichDetConfigType.h"

//-----------------------------------------------------------------------------
// Implementation file for class : RichDetConfigType
//-----------------------------------------------------------------------------

std::string Rich::text( const Rich::RichPhDetConfigType conf ) {
  switch ( conf ) {
  case Rich::HPDConfig:
    return "HPDs";
  case Rich::PMTConfig:
    return "MaPMTs";
  case Rich::InvalidPDConfig:
    return "Invalid PDConfig";
  default:
    return "SHOULD NEVER SEE THIS";
  }
}
