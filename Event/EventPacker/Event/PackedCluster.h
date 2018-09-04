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
#ifndef EVENT_PACKEDCLUSTER_H
#define EVENT_PACKEDCLUSTER_H 1
#include "GaudiKernel/DataObject.h"
#include "Event/VeloCluster.h"
#include "Event/STCluster.h"
#include "Event/UTCluster.h"
#include <string>
#include <vector>

namespace LHCb
{

  /** @class PackedCluster PackedCluster.h Event/PackedCluster.h
   *
   *  Packed description of a cluster (Velo, ST) for mDST transmission
   *
   *  @author Olivier Callot
   *  @date   2012-03-05
   */
  struct PackedCluster
  {
    unsigned int id{0};
    unsigned int begin{0};
    unsigned int end{0};
    unsigned int sum{0};
    unsigned int sourceID{0};
    unsigned int tell1Channel{0};
    int spill{0};

    template<typename T>
    inline void save(T& buf) const {
      buf.io(
        id, begin, end, sum, sourceID,
        tell1Channel, spill
      );
    }

    template<typename T>
    inline void load(T& buf, unsigned int /*version*/) {
      save(buf); // identical operation until version is incremented
    }

  };

  static const CLID CLID_PackedClusters = 1540;

  namespace PackedClusterLocation
  {
    static const std::string& Default = "pRec/Track/Clusters";
    static const std::string& Velo = "pRec/Velo/Clusters";
    static const std::string& IT = "pRec/IT/Clusters";
    static const std::string& TT = "pRec/TT/Clusters";
    static const std::string& UT = "pRec/TT/Clusters";
  }

  /** @class PackedClusters PackedCluster.h Event/PackedCluster.h
   *
   *  Container of packed Clusters
   *
   *  @author Olivier Callot
   *  @date   2012-03-05
   */

  class PackedClusters : public DataObject
  {

  public:

    /// Standard constructor
    PackedClusters()
    {
      m_clusters.reserve ( 100  );
      m_strips.reserve   ( 1000 );
      m_adcs.reserve     ( 1000 );
    }

  public:

    /// Default Packing Version
    static char defaultPackingVersion() { return 1; }

  public:

    const CLID& clID()  const override { return PackedClusters::classID(); }
    static  const CLID& classID()     { return CLID_PackedClusters;       }

  public:

    void addVeloCluster( const LHCb::VeloCluster* vCl );

    void addTTCluster( const LHCb::STCluster* sCl )
    {
      addSTCluster( sCl, 0x40000000 );
    }

    void addUTCluster( const LHCb::UTCluster* sCl )
    {
      addUTCluster( sCl, 0x50000000 ); // need to find the proper key (JC)
    }

    void addITCluster( const LHCb::STCluster* sCl )
    {
      addSTCluster( sCl, 0x60000000 );
    }

    void addSTCluster( const LHCb::STCluster* sCl,
                       const unsigned int key );

    void addUTCluster( const LHCb::UTCluster* uCl,
                       const unsigned int key );

  public:

    const std::vector<PackedCluster>& clusters() const { return m_clusters; }
    const std::vector<int>& strips()             const { return m_strips; }
    const std::vector<unsigned int>& adcs()      const { return m_adcs; }

  public:

    /// Set the packing version
    void setPackingVersion( const char ver ) { m_packingVersion = ver; }

    /// Access the packing version
    char packingVersion() const { return m_packingVersion; }

    /// Describe serialization of object
    template<typename T>
    inline void save(T& buf) const {
      buf.template save<uint8_t>(m_packingVersion);
      buf.template save<uint8_t>(version());
      buf.save(m_clusters);
      buf.save(m_strips);
      buf.save(m_adcs);
    }

    /// Describe de-serialization of object
    template<typename T>
    inline void load(T& buf) {
      setPackingVersion(buf.template load<uint8_t>());
      setVersion(buf.template load<uint8_t>());
      if (m_packingVersion < 1 || m_packingVersion > defaultPackingVersion()) {
        throw std::runtime_error("PackedClusters packing version is not supported: "
                                 + std::to_string(m_packingVersion));
      }
      buf.load(m_clusters, m_packingVersion);
      buf.load(m_strips);
      buf.load(m_adcs);
    }

  private:

    std::vector<PackedCluster> m_clusters;
    std::vector<int>           m_strips;
    std::vector<unsigned int>  m_adcs;

    /** Data packing version
     *  Note the default packing version here must stay as zero, for compatibility
     *  with data written before the packing version was added, to implicitly
     *  define the version as 0 for this data  */
    char m_packingVersion{0};

  };

}

#endif // EVENT_PACKEDCLUSTER_H
