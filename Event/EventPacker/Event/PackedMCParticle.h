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
#ifndef EVENT_PACKEDMCPARTICLE_H
#define EVENT_PACKEDMCPARTICLE_H 1

// Include files
#include "GaudiKernel/DataObject.h"
#include <string>
#include <vector>

namespace LHCb {

  /** @class PackedMCParticle Event/PackedMCParticle.h
   *
   *  Packed LHCb::MCParticle
   *
   *  @author Olivier Callot
   *  @date   2005-03-18
   */
  struct PackedMCParticle {
    int                    key{0};
    int                    px{0};
    int                    py{0};
    int                    pz{0};
    float                  mass{0};
    int                    PID{0};
    long long              originVertex{-1};
    std::vector<long long> endVertices;
    unsigned int           flags{0};
  };

  constexpr CLID CLID_PackedMCParticles = 1510;

  // Namespace for locations in TDS
  namespace PackedMCParticleLocation {
    inline const std::string Default = "pSim/MCParticles";
  }

  /** @class PackedMCParticles Event/PackedMCParticle.h
   *
   *  DataObject containing a vector of packed MCParticles
   *
   *  @author Olivier Callot
   *  @date   2005-03-18
   */
  class PackedMCParticles : public DataObject {

  public:
    /// Default Packing Version
    static char defaultPackingVersion() { return 1; }

  public:
    const CLID&        clID() const override { return PackedMCParticles::classID(); }
    static const CLID& classID() { return CLID_PackedMCParticles; }

  public:
    std::vector<PackedMCParticle>&       mcParts() { return m_vect; }
    const std::vector<PackedMCParticle>& mcParts() const { return m_vect; }

  public:
    /// Set the packing version
    void setPackingVersion( const char ver ) { m_packingVersion = ver; }

    /// Access the packing version
    char packingVersion() const { return m_packingVersion; }

  private:
    /// Packed MCParticles
    std::vector<PackedMCParticle> m_vect;

    /// Data packing version
    char m_packingVersion{0};
  };

} // namespace LHCb

#endif // EVENT_PACKEDMCPARTICLES_H
