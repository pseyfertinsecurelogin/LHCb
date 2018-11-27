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
#ifndef _Status_H
#define _Status_H 1

#include<map>
#include <string>
#include <vector>

#include "STDet/DeSTSector.h"

namespace Status{

  typedef std:: map<DeSTSector::Status,std::string> StatusToStringMap;
  typedef std::vector<DeSTSector::Status> StatusVector;

  /** map conversion from enum to string */
  const StatusToStringMap& statusDescription();

  /** lookup the enum corresponding to a string */
  DeSTSector::Status toStatus(const std::string& str);

  /** lookup the string corresponding to a status enum */
  std::string toString(const DeSTSector::Status& tstatus) ;

  /** allow state for beetles */
  const StatusVector& validBeetleStates();

  /** protected states, should not be overwritten */
  const StatusVector& protectedStates();

}

/** stream operator for status */
std::ostream& operator << (std::ostream& s, DeSTSector::Status e);
#endif
