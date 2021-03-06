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
#ifndef EVENT_PackedPartToRelatedInfoRelation_H
#define EVENT_PackedPartToRelatedInfoRelation_H 1

// STL
#include <string>
#include <vector>

// Kernel
#include "Event/StandardPacker.h"
#include "Relations/Relation1D.h"

// Event
#include "Event/Particle.h"
#include "Event/RelatedInfoMap.h"

// Gaudi
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/LinkManager.h"
#include "GaudiKernel/StatusCode.h"

namespace LHCb {
  // -----------------------------------------------------------------------

  /** @struct PackedRelatedInfoMap Event/PackedPartToRelatedInfoRelation.h
   *
   *  Utility class to implement the packing of Particle RelatedInfo relations
   *
   *  @author Christopher Rob Jones
   *  @date   2014-08-01
   */
  struct PackedRelatedInfoMap {
    // first and last entry in the info vector
    unsigned int first{0}, last{0};
    // reference
    long long reference{-1};

    template <typename T>
    inline void save( T& buf ) const {
      buf.io( first, last, reference );
    }
    template <typename T>
    inline void load( T& buf, unsigned int /*version*/ ) {
      save( buf ); // identical operation until version is incremented
    }
  };

  // -----------------------------------------------------------------------

  constexpr CLID CLID_PackedRelatedInfoRelations = 1584; // Tell Marco !!

  /// Namespace for locations in TDS
  namespace PackedRelatedInfoLocation {
    inline const std::string User     = "pPhys/User/PartToRelatedInfoRelations";
    inline const std::string InStream = "/pPhys/PartToRelatedInfoRelations";
  } // namespace PackedRelatedInfoLocation

  /** @class PackedParticles Event/PackedParticle.h
   *
   *  Packed Particle to Related info relations
   *
   *  @author Christopher Rob Jones
   *  @date   2014-08-01
   */
  class PackedRelatedInfoRelations : public DataObject {

  public:
    /// Related info pair (key,data)
    typedef std::pair<short, float> PackedRelatedInfo;

    /// Vector of info objects
    typedef std::vector<PackedRelatedInfo> InfoVector;

    /// Vector of Info objects
    typedef std::vector<LHCb::PackedRelatedInfoMap> RelationVector;

  public:
    /// Default Packing Version
    static char defaultPackingVersion() { return 0; }

  public:
    /// Class ID
    static const CLID& classID() { return CLID_PackedRelatedInfoRelations; }

    /// Class ID
    const CLID& clID() const override { return PackedRelatedInfoRelations::classID(); }

  public:
    /// Write access to the info vector
    InfoVector& info() { return m_info; }

    /// Read access to the data vector
    const InfoVector& info() const { return m_info; }

    /// Write access to the relation vector
    RelationVector& relations() { return m_relations; }

    /// Read access to the relation vector
    const RelationVector& relations() const { return m_relations; }

    /// Write access to the containers vector
    RelationVector& containers() { return m_containers; }

    /// Read access to the containers vector
    const RelationVector& containers() const { return m_containers; }

    /// Set the packing version
    void setPackingVersion( const char ver ) { m_packingVersion = ver; }

    /// Access the packing version
    char packingVersion() const { return m_packingVersion; }

  public: // For templated algorithms
    typedef RelationVector Vector;

    /// Write access to the data vector
    Vector& data() { return relations(); }

    /// Read access to the data vector
    const Vector& data() const { return relations(); }

    /// Describe serialization of object
    template <typename T>
    inline void save( T& buf ) const {
      buf.template save<uint8_t>( m_packingVersion );
      buf.template save<uint8_t>( version() );
      buf.save( m_info );
      buf.save( m_relations );
      buf.save( m_containers );
    }
    /// Describe de-serialization of object
    template <typename T>
    inline void load( T& buf ) {
      setPackingVersion( buf.template load<uint8_t>() );
      setVersion( buf.template load<uint8_t>() );
      if ( m_packingVersion > defaultPackingVersion() ) {
        throw std::runtime_error( "PackedPartToRelatedInfoRelations packing version is not supported: " +
                                  std::to_string( m_packingVersion ) );
      }
      buf.load( m_info, m_packingVersion );
      buf.load( m_relations, m_packingVersion );
      buf.load( m_containers, m_packingVersion );
    }

  private:
    /// Data packing version (not used as yet, but for any future schema evolution)
    char m_packingVersion{defaultPackingVersion()};

    /// The packed info vector
    InfoVector m_info;

    /// The packed relation vector
    RelationVector m_relations;

    /// The packed containers vector
    RelationVector m_containers;
  };

  // -----------------------------------------------------------------------

  /** @class RelatedInfoRelationsPacker Event/PackedPartToRelatedInfoRelation.h
   *
   *  Utility class to handle the packing and unpacking of the Particles
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-13
   */
  class RelatedInfoRelationsPacker {
  public:
    // These are required by the templated algorithms
    typedef LHCb::Particle                   FROM;
    typedef LHCb::RelatedInfoMap             TO;
    typedef LHCb::Relation1D<FROM, TO>       DataVector;
    typedef LHCb::PackedRelatedInfoRelations PackedDataVector;
    static const std::string&                packedLocation() { return LHCb::PackedRelatedInfoLocation::User; }
    static const std::string unpackedLocation() { return "/Event/Phys/User/PartToRelatedInfoRelations"; }

  private:
    /// Default Constructor hidden
    RelatedInfoRelationsPacker() {}

  public:
    /// Default Constructor
    RelatedInfoRelationsPacker( const GaudiAlgorithm* p ) : m_pack( p ) {}

  public:
    /// Pack the relations
    void pack( const DataVector& rels, PackedDataVector& prels ) const;

    /// Unpack the relations
    void unpack( const PackedDataVector& prels, DataVector& rels, const std::string& location = "" ) const;

    /// Unpack the relations
    void unpack( const LHCb::PackedRelatedInfoMap& pmap, const PackedDataVector& prels, DataVector& rels ) const;

    /// Compare two sets of relations to check the packing -> unpacking performance
    StatusCode check( const DataVector& dataA, const DataVector& dataB ) const;

  private:
    /// Access the parent algorithm
    const GaudiAlgorithm& parent() const { return *( m_pack.parent() ); }

    /// Check if the given packing version is supported
    bool isSupportedVer( const char& ver ) const {
      const bool OK = ( 0 == ver );
      if ( UNLIKELY( !OK ) ) {
        std::ostringstream mess;
        mess << "Unknown packed data version " << (int)ver;
        throw GaudiException( mess.str(), "RelatedInfoRelationsPacker", StatusCode::FAILURE );
      }
      return OK;
    }

  private:
    /// Standard packing of quantities into integers ...
    StandardPacker m_pack;
  };

  // -----------------------------------------------------------------------

} // namespace LHCb

#endif // EVENT_PackedPartToRelatedInfoRelation_H
