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
#include <string_view>

namespace {
  static const Status::StatusToStringMap s_map    = {{DeUTSector::OK, "OK"},
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
  static const Status::StatusVector      s_vec_bs = {DeUTSector::OK, DeUTSector::ReadoutProblems, DeUTSector::Dead,
                                                DeUTSector::OtherFault};
  static const Status::StatusVector      s_vec_ps = {DeUTSector::NotBonded};
} // namespace

const Status::StatusToStringMap& Status::statusDescription() { return s_map; }

DeUTSector::Status Status::toStatus( const std::string& str ) {
  const auto& map = statusDescription();
  auto        i   = std::find_if( map.begin(), map.end(), [&]( const auto& i ) { return i.second == str; } );
  return i == map.end() ? DeUTSector::UnknownStatus : i->first;
}

std::string Status::toString( const DeUTSector::Status& tstatus ) {
  const auto& map = statusDescription();
  auto        i   = map.find( tstatus );
  return i == map.end() ? "UnknownStatus" : i->second;
}

const Status::StatusVector& Status::validBeetleStates() { return s_vec_bs; }

const Status::StatusVector& Status::protectedStates() { return s_vec_ps; }

/** stream operator for status */
std::ostream& operator<<( std::ostream& s, DeUTSector::Status e ) {
  using namespace std::string_view_literals;
  const auto& map = Status::statusDescription();
  auto        i   = map.find( e );
  return s << ( i == map.end() ? "Unknown"sv : std::string_view{i->second} );
}
