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
#ifndef EVENT_PackedMCRichTrack_H
#define EVENT_PackedMCRichTrack_H 1

// STL
#include <string>
#include <vector>

// Kernel
#include "Event/StandardPacker.h"

// Event
#include "Event/MCRichTrack.h"

// Gaudi
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/GaudiException.h"

namespace LHCb
{
  // -----------------------------------------------------------------------

  /** @struct PackedMCRichTrack Event/PackedMCRichTrack.h
   *
   *  Packed MCRichTrack
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-13
   */
  struct PackedMCRichTrack
  {
    int key{0};
    std::vector<long long> mcSegments;
    long long mcParticle{-1};
  };

  // -----------------------------------------------------------------------

  constexpr CLID CLID_PackedMCRichTracks = 1524;

  /// Namespace for locations in TDS
  namespace PackedMCRichTrackLocation
  {
    inline const std::string Default = "pSim/Rich/Tracks";
  }

  /** @class PackedMCRichTracks Event/PackedMCRichTrack.h
   *
   *  Packed MCRichTracks
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-13
   */
  class PackedMCRichTracks : public DataObject
  {

  public:

    /// Vector of packed objects
    typedef std::vector<LHCb::PackedMCRichTrack> Vector;

  public:

    /// Default Packing Version
    static char defaultPackingVersion() { return 1; }

  public:

    /// Class ID
    static const CLID& classID() { return CLID_PackedMCRichTracks; }

    /// Class ID
    const CLID& clID() const  override { return PackedMCRichTracks::classID(); }

  public:

    /// Write access to the data vector
    Vector & data()             { return m_vect; }

    /// Read access to the data vector
    const Vector & data() const { return m_vect; }

    /// Set the packing version
    void setPackingVersion( const char ver ) { m_packingVersion = ver; }

    /// Access the packing version
    char packingVersion() const { return m_packingVersion; }

  private:

    /// Data packing version
    char   m_packingVersion{ defaultPackingVersion() };

    /// The packed data objects
    Vector m_vect;

  };

  // -----------------------------------------------------------------------

  /** @class MCRichTrackPacker Event/PackedMCRichTrack.h
   *
   *  Utility class to handle the packing and unpacking of the MCRichTracks
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-13
   */
  class MCRichTrackPacker
  {
  public:

    // These are required by the templated algorithms
    typedef LHCb::MCRichTrack                    Data;
    typedef LHCb::PackedMCRichTrack        PackedData;
    typedef LHCb::MCRichTracks             DataVector;
    typedef LHCb::PackedMCRichTracks PackedDataVector;
    static const std::string& packedLocation()   { return LHCb::PackedMCRichTrackLocation::Default; }
    static const std::string& unpackedLocation() { return LHCb::MCRichTrackLocation::Default; }

  private:

    /// Default Constructor
    MCRichTrackPacker() {}

  public:

    /// Constructor
    MCRichTrackPacker( const GaudiAlgorithm * p ) : m_pack(p) {}

  public:

    /// Pack MCRichTracks
    void pack( const DataVector & tracks,
               PackedDataVector & ptracks ) const;

    /// Unpack MCRichTracks
    void unpack( const PackedDataVector & ptracks,
                 DataVector             & tracks ) const;

    /// Compare two MCRichHits to check the packing -> unpacking performance
    StatusCode check( const DataVector & dataA,
                      const DataVector & dataB ) const;

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
        throw GaudiException( mess.str(), "MCRichTrackPacker", StatusCode::FAILURE );
      }
      return OK;
    }

  private:

    /// Standard packing of quantities into integers ...
    StandardPacker m_pack;

  };

  // -----------------------------------------------------------------------

}

#endif // EVENT_PackedMCRichTrack_H
