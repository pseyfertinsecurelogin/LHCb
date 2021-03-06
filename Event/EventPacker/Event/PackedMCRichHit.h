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
#ifndef EVENT_PACKEDMCRICHHIT_H
#define EVENT_PACKEDMCRICHHIT_H 1

// STL
#include <string>

// Kernel
#include "Event/StandardPacker.h"

// Event
#include "Event/MCRichHit.h"

// Gaudi
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/StatusCode.h"

namespace LHCb {
  // -----------------------------------------------------------------------

  /** @struct PackedMCRichHit Event/PackedMCRichHit.h
   *
   *  Packed MCRichHit
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-13
   */
  struct PackedMCRichHit {
    int       x{0}, y{0}, z{0};
    int       energy{0};
    int       tof{0};
    int       sensDetID{0};
    int       history{0};
    long long mcParticle{-1};
  };

  // -----------------------------------------------------------------------

  constexpr CLID CLID_PackedMCRichHits = 1521;

  /// Namespace for locations in TDS
  namespace PackedMCRichHitLocation {
    inline const std::string Default = "pSim/Rich/Hits";
  }

  /** @class PackedMCRichHits Event/PackedMCRichHit.h
   *
   *  Packed MCRichHits
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-13
   */
  class PackedMCRichHits : public DataObject {

  public:
    /// Vector of packed objects
    typedef std::vector<LHCb::PackedMCRichHit> Vector;

  public:
    /// Default Packing Version
    static char defaultPackingVersion() { return 1; }

  public:
    /// Class ID
    static const CLID& classID() { return CLID_PackedMCRichHits; }

    /// Class ID
    const CLID& clID() const override { return PackedMCRichHits::classID(); }

  public:
    /// Write access to the data vector
    Vector& data() { return m_vect; }

    /// Read access to the data vector
    const Vector& data() const { return m_vect; }

    /// Set the packing version
    void setPackingVersion( const char ver ) { m_packingVersion = ver; }

    /// Access the packing version
    char packingVersion() const { return m_packingVersion; }

  private:
    /// Data packing version
    char m_packingVersion{defaultPackingVersion()};

    /// The packed data objects
    Vector m_vect;
  };

  // -----------------------------------------------------------------------

  /** @class MCRichHitPacker Event/PackedMCRichHit.h
   *
   *  Utility class to handle the packing and unpacking of the MCRichHits
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-13
   */
  class MCRichHitPacker {
  public:
    // These are required by the templated algorithms
    typedef LHCb::MCRichHit        Data;
    typedef LHCb::PackedMCRichHit  PackedData;
    typedef LHCb::MCRichHits       DataVector;
    typedef LHCb::PackedMCRichHits PackedDataVector;
    static const std::string&      packedLocation() { return LHCb::PackedMCRichHitLocation::Default; }
    static const std::string&      unpackedLocation() { return LHCb::MCRichHitLocation::Default; }

  private:
    /// Default Constructor hidden
    MCRichHitPacker() {}

  public:
    /// Constructor
    MCRichHitPacker( const GaudiAlgorithm* p ) : m_pack( p ) {}

  public:
    /// Pack MCRichHits
    void pack( const DataVector& hits, PackedDataVector& phits ) const;

    /// Unpack MCRichHits
    void unpack( const PackedDataVector& phits, DataVector& hits ) const;

    /// Compare two MCRichHits to check the packing -> unpacking performance
    StatusCode check( const DataVector& dataA, const DataVector& dataB ) const;

  private:
    /// Access the parent algorithm
    const GaudiAlgorithm& parent() const { return *( m_pack.parent() ); }

    /// Check if the given packing version is supported
    bool isSupportedVer( const char& ver ) const {
      const bool OK = ( 1 == ver || 0 == ver );
      if ( UNLIKELY( !OK ) ) {
        std::ostringstream mess;
        mess << "Unknown packed data version " << (int)ver;
        throw GaudiException( mess.str(), "MCRichHitPacker", StatusCode::FAILURE );
      }
      return OK;
    }

  private:
    /// Standard packing of quantities into integers ...
    StandardPacker m_pack;
  };

  // -----------------------------------------------------------------------

} // namespace LHCb

#endif // EVENT_PACKEDMCRICHHIT_H
