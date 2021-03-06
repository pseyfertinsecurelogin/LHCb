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
#ifndef EVENT_PACKEDRICHPID_H
#define EVENT_PACKEDRICHPID_H 1

// STL
#include <sstream>
#include <string>

// Kernel
#include "Event/StandardPacker.h"

// Event
#include "Event/RichPID.h"

// Gaudi
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/StatusCode.h"

namespace LHCb {
  // -----------------------------------------------------------------------

  /** @struct PackedRichPID Event/PackedRichPID.h
   *
   *  Packed RichPID
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-13
   */
  struct PackedRichPID {
    int       pidResultCode{0};
    int       dllEl{0}, dllMu{0}, dllPi{0}, dllKa{0}, dllPr{0};
    long long track{-1};
    int       dllBt{0};
    long long key{0};
    int       dllDe{0};

    template <typename T>
    inline void save( T& buf ) const {
      buf.io( pidResultCode, dllEl, dllMu, dllPi, dllKa, dllPr, track, dllBt, key, dllDe );
    }

    template <typename T>
    inline void load( T& buf, unsigned int /*version*/ ) {
      buf.io( pidResultCode );
      buf.io( dllEl );
      buf.io( dllMu );
      buf.io( dllPi );
      buf.io( dllKa );
      buf.io( dllPr );
      buf.io( track );
      buf.io( dllBt );
      buf.io( key );
      buf.io( dllDe );

      // - Example 1, adding fields
      // if (version >= 5) buf.io(dllIon)

      // - Example 2, expanding (changing) field type
      // int -> long long pidResultCode
      // if (version >= 5) {
      //   buf.io(pidResultCode);
      // } else {
      //   int tmp;
      //   buf.io(tmp);
      //   pidResultCode = tmp;
      // }
    }
  };

  // -----------------------------------------------------------------------

  constexpr CLID CLID_PackedRichPIDs = 1561;

  /// Namespace for locations in TDS
  namespace PackedRichPIDLocation {
    inline const std::string Default  = "pRec/Rich/PIDs";
    inline const std::string InStream = "/pRec/Rich/CustomPIDs";
  } // namespace PackedRichPIDLocation

  /** @class PackedRichPIDs Event/PackedRichPID.h
   *
   *  Packed RichPIDs
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-13
   */
  class PackedRichPIDs : public DataObject {

  public:
    /// Vector of packed objects
    typedef std::vector<LHCb::PackedRichPID> Vector;

  public:
    /// Default Packing Version
    static char defaultPackingVersion() { return 4; }

  public:
    /// Class ID
    static const CLID& classID() { return CLID_PackedRichPIDs; }

    /// Class ID
    const CLID& clID() const override { return PackedRichPIDs::classID(); }

  public:
    /// Write access to the data vector
    Vector& data() { return m_vect; }

    /// Read access to the data vector
    const Vector& data() const { return m_vect; }

    /// Set the packing version
    void setPackingVersion( const char ver ) { m_packingVersion = ver; }

    /// Access the packing version
    char packingVersion() const { return m_packingVersion; }

    /// Describe serialization of object
    template <typename T>
    inline void save( T& buf ) const {
      buf.template save<uint8_t>( m_packingVersion );
      buf.template save<uint8_t>( version() );
      buf.save( m_vect );
    }

    /// Describe de-serialization of object
    template <typename T>
    inline void load( T& buf ) {
      setPackingVersion( buf.template load<uint8_t>() );
      setVersion( buf.template load<uint8_t>() );
      if ( m_packingVersion < 4 || m_packingVersion > defaultPackingVersion() ) {
        throw std::runtime_error( "PackedRichPIDs packing version is not supported: " +
                                  std::to_string( m_packingVersion ) );
      }
      buf.load( m_vect, m_packingVersion );
    }

  private:
    /// Data packing version
    char m_packingVersion{defaultPackingVersion()};

    /// The packed data objects
    Vector m_vect;
  };

  // -----------------------------------------------------------------------

  /** @class RichPIDPacker Event/PackedRichPID.h
   *
   *  Utility class to handle the packing and unpacking of the RichPIDs
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-13
   */
  class RichPIDPacker {

  public:
    // These are required by the templated algorithms
    typedef LHCb::RichPID        Data;
    typedef LHCb::PackedRichPID  PackedData;
    typedef LHCb::RichPIDs       DataVector;
    typedef LHCb::PackedRichPIDs PackedDataVector;
    static const std::string&    packedLocation() { return LHCb::PackedRichPIDLocation::Default; }
    static const std::string&    unpackedLocation() { return LHCb::RichPIDLocation::Default; }

  private:
    /// Default Constructor hidden
    RichPIDPacker() {}

  public:
    /// Constructor
    RichPIDPacker( const GaudiAlgorithm* parent ) : m_pack( parent ) {}

  public:
    /// Pack a RichPID
    void pack( const Data& pid, PackedData& ppid, PackedDataVector& ppids ) const;

    /// Pack RichPIDs
    void pack( const DataVector& pids, PackedDataVector& ppids ) const;

    /// Unpack a single RichPID
    void unpack( const PackedData& ppid, Data& pid, const PackedDataVector& ppids, DataVector& pids ) const;

    /// Unpack RichPIDs
    void unpack( const PackedDataVector& ppids, DataVector& pids ) const;

    /// Compare two RichPID containers to check the packing -> unpacking performance
    StatusCode check( const DataVector& dataA, const DataVector& dataB ) const;

    /// Compare two MuonPIDs to check the packing -> unpacking performance
    StatusCode check( const Data& dataA, const Data& dataB ) const;

  private:
    /// Access the parent algorithm
    const GaudiAlgorithm& parent() const { return *( m_pack.parent() ); }

    /// Check if the given packing version is supported
    bool isSupportedVer( const char& ver ) const {
      const bool OK = ( 0 <= ver && ver <= 4 );
      if ( UNLIKELY( !OK ) ) {
        std::ostringstream mess;
        mess << "Unknown packed data version " << (int)ver;
        throw GaudiException( mess.str(), "RichPIDPacker", StatusCode::FAILURE );
      }
      return OK;
    }

  private:
    /// Standard packing of quantities into integers ...
    StandardPacker m_pack;
  };

  // -----------------------------------------------------------------------

} // namespace LHCb

#endif // EVENT_PACKEDRICHPID_H
