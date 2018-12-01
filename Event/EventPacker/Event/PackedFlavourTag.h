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
#ifndef EVENT_PackedFlavourTag_H
#define EVENT_PackedFlavourTag_H 1

// STL
#include <string>

// Kernel
#include "Event/StandardPacker.h"

// Event
#include "Event/FlavourTag.h"

// Gaudi
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/GaudiException.h"

namespace LHCb
{

  // ----------------------------------------------------------------------------------------

  /** @struct PackedFlavourTag Event/PackedFlavourTag.h
   *
   *  Packed FlavourTag
   *
   *  @author Christopher Rob Jones
   *  @date   2013-05-03
   */
  struct PackedFlavourTag
  {
    long long key{0};        ///< reference to the original container + key

    short int decision{0};   ///< The result of the tagging algorithm
    short int omega{0};      ///< Wrong tag fraction (predicted)

    short int decisionOS{0}; ///< decision of opposite side taggers only
    short int omegaOS{0};    ///< Wrong tag fraction (predicted) using opposite side only

    long long taggedB{-1};   ///< The B for which this tag has been made

    // Taggers
    unsigned int firstTagger{0}, lastTagger{0};

    template<typename T>
    inline void save(T& buf) const {
      buf.io(
        key, decision, omega, decisionOS, omegaOS,
        taggedB, firstTagger, lastTagger
      );
    }
    template<typename T>
    inline void load(T& buf, unsigned int /*version*/) {
      save(buf); // identical operation until version is incremented
     }
  };

  // ----------------------------------------------------------------------------------------

  /** @struct PackedTagger Event/PackedFlavourTag.h
   *
   *  Packed Flavour Tagger
   *
   *  @author Christopher Rob Jones
   *  @date   2013-05-03
   */
  struct PackedTagger
  {
    unsigned short int type{0}; ///< The type of tagger
    short int      decision{0}; ///< Decision of tagger
    short int         omega{0}; ///< Wrong tag fraction of tagger
    unsigned int firstTagP{0}, lastTagP{0}; ///< Tagging particles
    int          mvaValue{0};   ///< MVA used for classification
    short int    charge{0};     ///< Charge used for classification

    template<typename T>
    inline void save(T& buf) const {
      buf.io(type, decision, omega, firstTagP, lastTagP, mvaValue, charge);
    }
    template<typename T>
    inline void load(T& buf, unsigned int version) {
      switch(version) {
        case 0:   buf.io(type, decision, omega, firstTagP, lastTagP); break;
        default:  save(buf); break;
      }
    }
  };

  // ----------------------------------------------------------------------------------------

  constexpr CLID CLID_PackedFlavourTags = 1583;

  /// Namespace for locations in TDS
  namespace PackedFlavourTagLocation
  {
    inline const std::string Default  = "pPhys/FlavourTags";
    inline const std::string InStream = "/pPhys/FlavourTags";
  }

  /** @class PackedFlavourTags Event/PackedFlavourTag.h
   *
   *  Packed FlavourTags
   *
   *  @author Christopher Rob Jones
   *  @date   2013-05-03
   */
  class PackedFlavourTags : public DataObject
  {

  public:

    /// Vector of packed Flavour Tags
    typedef std::vector<LHCb::PackedFlavourTag> Vector;

    /// Vector of packed Taggers
    typedef std::vector<LHCb::PackedTagger> Taggers;

    /// Tagging Particles
    typedef std::vector<long long> TaggingParticles;

  public:

    /// Default Packing Version
    static char defaultPackingVersion() { return 1; }

  public:

    /// Class ID
    static const CLID& classID() { return CLID_PackedFlavourTags; }

    /// Class ID
    const CLID& clID() const override { return PackedFlavourTags::classID(); }

  public:

    /// Write access to the data vector
    Vector & data()             { return m_vect; }

    /// Read access to the data vector
    const Vector & data() const { return m_vect; }

    /// Set the packing version
    void setPackingVersion( const char ver ) { m_packingVersion = ver; }

    /// Access the packing version
    char packingVersion() const { return m_packingVersion; }

    /// Const access to taggers
    const Taggers& taggers() const { return m_taggers; }
    /// Access to taggers
    Taggers& taggers()             { return m_taggers; }

    /// Const access to tagging Particles
    const TaggingParticles& taggeringPs() const { return m_taggingPs; }
    /// Access to taggers
    TaggingParticles& taggeringPs()             { return m_taggingPs; }

    /// Describe serialization of object
    template<typename T>
    inline void save(T& buf) const {
      buf.template save<uint8_t>(m_packingVersion);
      buf.template save<uint8_t>(version());
      buf.save(m_vect);
      buf.save(m_taggers);
      buf.save(m_taggingPs);
    }
    /// Describe de-serialization of object
    template<typename T>
    inline void load(T& buf) {
      setPackingVersion(buf.template load<uint8_t>());
      setVersion(buf.template load<uint8_t>());
      if (m_packingVersion > defaultPackingVersion()) {
        throw std::runtime_error("PackedFlavourTags packing version is not supported: "
                                 + std::to_string(m_packingVersion));
      }
      buf.load(m_vect, m_packingVersion);
      buf.load(m_taggers, m_packingVersion);
      buf.load(m_taggingPs);
    }

  private:

    /// Data packing version (not used as yet, but for any future schema evolution)
    char m_packingVersion{ defaultPackingVersion() };

    /// The packed flavour tags
    Vector m_vect;

    /// Packed Taggers
    Taggers m_taggers;

    /// Tagging Particles
    TaggingParticles m_taggingPs;

  };

  // -----------------------------------------------------------------------

  /** @class FlavourTagPacker Event/PackedFlavourTag.h
   *
   *  Utility class to handle the packing and unpacking of the FlavourTags
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-13
   */
  class FlavourTagPacker
  {
  public:

    // These are required by the templated algorithms
    typedef LHCb::FlavourTag                    Data;
    typedef LHCb::PackedFlavourTag        PackedData;
    typedef LHCb::FlavourTags             DataVector;
    typedef LHCb::PackedFlavourTags PackedDataVector;
    static const std::string& packedLocation()   { return LHCb::PackedFlavourTagLocation::Default; }
    static const std::string& unpackedLocation() { return LHCb::FlavourTagLocation::Default; }

  private:

    /// Default Constructor hidden
    FlavourTagPacker() {}

  public:

    /// Default Constructor
    FlavourTagPacker( const GaudiAlgorithm * p ) : m_pack(p) {}

  public:

    /// Pack a single FlavourTag
    void pack( const Data & ft,
               PackedData & pft,
               PackedDataVector & pfts ) const;

    /// Pack FlavourTags
    void pack( const DataVector & fts,
               PackedDataVector & pfts ) const;

    /// Unpack a single FlavourTag
    void unpack( const PackedData       & pft,
                 Data                   & ft,
                 const PackedDataVector & pfts,
                 DataVector             & fts ) const;

    /// Unpack FlavourTags
    void unpack( const PackedDataVector & pfts,
                 DataVector             & fts ) const;

    /// Compare two FlavourTag vectors to check the packing -> unpacking performance
    StatusCode check( const DataVector & dataA,
                      const DataVector & dataB ) const;

    /// Compare two FlavourTags to check the packing -> unpacking performance
    StatusCode check( const Data & dataA,
                      const Data & dataB ) const;

  private:

    /// Access the parent algorithm
    const GaudiAlgorithm& parent() const { return *(m_pack.parent()); }

    /// Check if the given packing version is supported
    bool isSupportedVer( const char& ver ) const
    {
      const bool OK = ( 1 == ver || 0 == ver );
      if ( UNLIKELY(!OK) )
      {
        std::ostringstream mess;
        mess << "Unknown packed data version " << (int)ver;
        throw GaudiException( mess.str(), "FlavourTagPacker", StatusCode::FAILURE );
      }
      return OK;
    }

  private:

    /// Standard packing of quantities into integers ...
    StandardPacker m_pack;

  };

  // -----------------------------------------------------------------------

}

#endif // EVENT_PackedFlavourTag_H
