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
#include "UTDet/StatusMap.h"
#if !(defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L)
#include <boost/assign/list_of.hpp>
#endif

const Status::StatusToStringMap& Status::statusDescription() {
  static const Status::StatusToStringMap s_map = {{DeUTSector::OK, "OK"},
                                             {DeUTSector::Open, "Open"},
                                             {DeUTSector::Short,"Short"},
                                             {DeUTSector::Pinhole,"Pinhole"},
                                             {DeUTSector::NotBonded, "NotBonded"},
                                             {DeUTSector::LowGain, "LowGain"},
                                             {DeUTSector::Noisy, "Noisy"},
                                             {DeUTSector::ReadoutProblems,"ReadoutProblems"},
                                             {DeUTSector::OtherFault,"OtherFault"},
                                             {DeUTSector::Dead,"Dead"},
                                             {DeUTSector::UnknownStatus, "Unknown"}};
  return s_map;
}


DeUTSector::Status Status::toStatus(const std::string& str) {

  const ::Status::StatusToStringMap& statusMap = ::Status::statusDescription();
  ::Status::StatusToStringMap::const_iterator iterMap = statusMap.begin();
  while (iterMap != statusMap.end()){
    if (iterMap->second == str) break;
      ++iterMap;
  } // iterMap
  return (iterMap == statusMap.end() ? DeUTSector::UnknownStatus : iterMap->first);
}

std::string Status::toString(const DeUTSector::Status& tstatus) {

  const ::Status::StatusToStringMap& statusMap = ::Status::statusDescription();
  ::Status::StatusToStringMap::const_iterator iterMap = statusMap.find(tstatus);
   return (iterMap == statusMap.end() ? "UnknownStatus" : iterMap->second);
}


const Status::StatusVector& Status::validBeetleStates() {
  static const StatusVector s_vec = {DeUTSector::OK, DeUTSector::ReadoutProblems,
                                DeUTSector::Dead, DeUTSector::OtherFault};
  return s_vec;
}

const Status::StatusVector& Status::protectedStates() {
  static const StatusVector s_vec = {DeUTSector::NotBonded};
  return s_vec;
}

/** stream operator for status */
std::ostream& operator << (std::ostream& s, DeUTSector::Status e){
  const Status::StatusToStringMap& theMap = Status::statusDescription();
  Status::StatusToStringMap::const_iterator iter = theMap.find(e);
  return (iter == theMap.end() ? s << "Unknown":s <<iter->second);
}


