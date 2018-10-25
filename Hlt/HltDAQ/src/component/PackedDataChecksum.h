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
#ifndef PACKEDOBJECTCHECKSUM_H
#define PACKEDOBJECTCHECKSUM_H 1

#include <map>
#include <boost/crc.hpp>

#include "Event/PackedTrack.h"
#include "Event/PackedCaloCluster.h"
#include "Event/PackedRichPID.h"
#include "Event/PackedMuonPID.h"
#include "Event/PackedCaloHypo.h"
#include "Event/PackedProtoParticle.h"
#include "Event/PackedParticle.h"
#include "Event/PackedVertex.h"
#include "Event/PackedRecVertex.h"
#include "Event/PackedFlavourTag.h"
#include "Event/PackedRelations.h"
#include "Event/PackedPartToRelatedInfoRelation.h"
#include "Event/PackedCaloDigit.h"
#include "Event/PackedCluster.h"
#include "Event/PackedCaloAdc.h"

namespace PackedDataPersistence {

/** @class PackedDataChecksum PackedDataChecksum.h
 *  Helper class that calculates packed object checksums
 *
 *  @author Rosen Matev
 *  @date   2016-01-03
 */
class PackedDataChecksum {
public:

  template<typename T>
  void processObject(const T& object, const std::string& key = "");

  /// Obtain the current value of a checksum
  int checksum(const std::string& key = "") {
    return m_result[key].checksum();
  }

  /// Obtain the current value of all checksums
  std::map<std::string, int> checksums() const {
    std::map<std::string, int> result;
    for (const auto& x: m_result) {
      result.emplace(x.first, x.second.checksum());
    }
    return result;
  }

  /// Reset the checksums
  void reset() {
    for (auto& x: m_result) {
      x.second.reset(0);
    }
  }

private:

  template<typename T>
  void process(const std::string& key, const T& x);

  template<typename T1, typename T2>
  void process(const std::string& key, const std::pair<T1, T2>& x);

  template<typename T>
  void processVector(const std::string& key, const std::vector<T>& v);

  template <typename ...Args>
  void processMany(const std::string& key, Args&&... args);


  std::map<std::string, boost::crc_32_type> m_result;

};

}

#endif // PACKEDOBJECTCHECKSUM_H
