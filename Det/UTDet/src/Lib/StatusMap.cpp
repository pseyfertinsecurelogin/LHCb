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

namespace {
  static const Status::StatusToStringMap s_statusDescription = {{DeUTSector::OK, "OK"},
                                                                {DeUTSector::Open, "Open"},
                                                                {DeUTSector::Short, "Short"},
                                                                {DeUTSector::Pinhole, "Pinhole"},
                                                                {DeUTSector::NotBonded, "NotBonded"},
                                                                {DeUTSector::LowGain, "LowGain"},
                                                                {DeUTSector::Noisy, "Noisy"},
                                                                {DeUTSector::ReadoutProblems, "ReadoutProblems"},
                                                                {DeUTSector::OtherFault, "OtherFault"},
                                                                {DeUTSector::Dead, "Dead"},
                                                                {DeUTSector::UnknownStatus, "Unknown"}};
  static const Status::StatusVector      s_validBeetleStates = {DeUTSector::OK, DeUTSector::ReadoutProblems,
                                                           DeUTSector::Dead, DeUTSector::OtherFault};
  static const Status::StatusVector      s_protectedStates   = {DeUTSector::NotBonded};
} // namespace

const Status::StatusToStringMap& Status::statusDescription() { return s_statusDescription; }

DeUTSector::Status Status::toStatus( const std::string& str ) {
  const auto& statusMap = ::Status::statusDescription();
  auto        i = std::find_if( statusMap.begin(), statusMap.end(), [&]( const auto& v ) { return v.second == str; } );
  return i == statusMap.end() ? DeUTSector::UnknownStatus : i->first;
}

std::string Status::toString( const DeUTSector::Status& tstatus ) {
  const auto& statusMap = ::Status::statusDescription();
  auto        i         = statusMap.find( tstatus );
  return i == statusMap.end() ? "UnknownStatus" : i->second;
}

const Status::StatusVector& Status::validBeetleStates() { return s_validBeetleStates; }

const Status::StatusVector& Status::protectedStates() { return s_protectedStates; }

/** stream operator for status */
std::ostream& operator<<( std::ostream& s, DeUTSector::Status e ) {
  const auto& theMap = Status::statusDescription();
  auto        i      = theMap.find( e );
  return i == theMap.end() ? s << "Unknown" : s << i->second;
}
