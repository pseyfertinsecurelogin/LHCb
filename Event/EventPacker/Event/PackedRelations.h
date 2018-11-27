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
#ifndef EVENT_PACKEDRELATIONS_H
#define EVENT_PACKEDRELATIONS_H 1

// Kernel
#include "Event/StandardPacker.h"

// Include files
namespace LHCb
{

  /** @class PackedRelation PackedRelations.h Event/PackedRelations.h
   *
   *  Packed Relation
   *
   *  @author Olivier Callot
   *  @date   2012-01-24
   */
  class PackedRelation
  {
  public:
    long long container{0};
    int start{0};
    int end{0};

    template<typename T>
    inline void save(T& buf) const {
      buf.io(
        container, start, end
      );
    }
    template<typename T>
    inline void load(T& buf, unsigned int /*version*/) {
      save(buf); // identical operation until version is incremented
     }
  };

  // =================== Unweighted Relations =========================

  constexpr CLID CLID_PackedRelations = 1560;

  /// Namespace for locations in TDS
  namespace PackedRelationsLocation
  {
    inline const std::string InStream = "/pPhys/Relations";
    inline const std::string P2MCP    = "/pPhys/P2MCPRelations";
    inline const std::string P2Int    = "/pPhys/P2IntRelations";
  }

  /** @class PackedRelations PackedRelations.h Event/PackedRelations.h
   *
   *  Packed Relations
   *
   *  @author Olivier Callot
   *  @date   2012-01-24
   */
  class PackedRelations : public DataObject
  {

  public:

    /// Class ID
    static const CLID& classID() { return CLID_PackedRelations; }

    /// Class ID
    const CLID& clID() const  override { return PackedRelations::classID(); }


  public:

    std::vector<PackedRelation>&       relations()       { return m_relations; }
    const std::vector<PackedRelation>& relations() const { return m_relations; }

    std::vector<long long>&            sources()         { return m_source; }
    const std::vector<long long>&      sources()   const { return m_source; }

    std::vector<long long>&            dests()           { return m_dest; }
    const std::vector<long long>&      dests()     const { return m_dest; }

    /// Describe serialization of object
    template<typename T>
    inline void save(T& buf) const
    {
      // the object does not define a packing version, so save 0
      buf.template save<uint8_t>(0);
      buf.template save<uint8_t>(version());
      buf.save(m_relations);
      buf.save(m_source);
      buf.save(m_dest);
    }

    /// Describe de-serialization of object
    template<typename T>
    inline void load(T& buf)
    {
      // the object does not define a packing version, but we save one
      auto packingVersion = buf.template load<uint8_t>();
      setVersion(buf.template load<uint8_t>());
      if (packingVersion != 0) {
        throw std::runtime_error("PackedRelations packing version is not supported: "
                                 + std::to_string(packingVersion));
      }
      buf.load(m_relations, packingVersion);
      buf.load(m_source);
      buf.load(m_dest);
    }

  private:

    std::vector<PackedRelation> m_relations;
    std::vector<long long>      m_source;
    std::vector<long long>      m_dest;

  };

  // =================== Weighted Relations =========================

  constexpr CLID CLID_PackedWeightedRelations = 1562;

  /// Namespace for locations in TDS
  namespace PackedWeightedRelationsLocation
  {
    inline const std::string PP2MCP = "/pPhys/PP2MCPRelations";
  }

  /** @class PackedWeightedRelations PackedRelations.h Event/PackedRelations.h
   *
   *  Packed Weighted Relations
   *
   *  @author Chris Jones
   *  @date   2015-04-13
   */
  class PackedWeightedRelations : public DataObject
  {

  public:

    /// Class ID
    static const CLID& classID() { return CLID_PackedWeightedRelations; }

    /// Class ID
    const CLID& clID() const  override { return PackedWeightedRelations::classID(); }

  public:

    std::vector<PackedRelation>&       relations()       { return m_relations; }
    const std::vector<PackedRelation>& relations() const { return m_relations; }

    std::vector<long long>&            sources()         { return m_source; }
    const std::vector<long long>&      sources()   const { return m_source; }

    std::vector<long long>&            dests()           { return m_dest; }
    const std::vector<long long>&      dests()     const { return m_dest; }

    std::vector<float>&                weights()         { return m_weights; }
    const std::vector<float>&          weights()   const { return m_weights; }

  private:

    std::vector<PackedRelation> m_relations;
    std::vector<long long>      m_source;
    std::vector<long long>      m_dest;
    std::vector<float>          m_weights;

  };

}

#endif // EVENT_PACKEDRELATIONS_H
