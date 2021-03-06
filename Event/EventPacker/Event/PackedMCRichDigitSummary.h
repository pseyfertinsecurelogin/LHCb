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
#ifndef EVENT_PackedMCRichDigitSummary_H
#define EVENT_PackedMCRichDigitSummary_H 1

// STL
#include <string>

// Kernel
#include "Event/StandardPacker.h"

// Event
#include "Event/MCRichDigitSummary.h"

// Gaudi
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/StatusCode.h"

namespace LHCb {
  // -----------------------------------------------------------------------

  /** @struct PackedMCRichDigitSummary Event/PackedMCRichDigitSummary.h
   *
   *  Packed MCRichHit
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-13
   */
  struct PackedMCRichDigitSummary {
    int       history{0};
    int       richSmartID{0};
    long long mcParticle{-1};
  };

  // -----------------------------------------------------------------------

  constexpr CLID CLID_PackedMCRichDigitSummarys = 1527;

  /// Namespace for locations in TDS
  namespace PackedMCRichDigitSummaryLocation {
    inline const std::string Default = "pSim/Rich/DigitSummaries";
  }

  /** @class PackedMCRichDigitSummarys Event/PackedMCRichDigitSummary.h
   *
   *  Packed MCRichDigitSummarys
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-13
   */
  class PackedMCRichDigitSummarys : public DataObject {

  public:
    /// Vector of packed objects
    typedef std::vector<LHCb::PackedMCRichDigitSummary> Vector;

  public:
    /// Default Packing Version
    static char defaultPackingVersion() { return 1; }

  public:
    /// Class ID
    static const CLID& classID() { return CLID_PackedMCRichDigitSummarys; }

    /// Class ID
    const CLID& clID() const override { return PackedMCRichDigitSummarys::classID(); }

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

  /** @class MCRichDigitSummaryPacker Event/PackedMCRichDigitSummary.h
   *
   *  Utility class to handle the packing and unpacking of the MCRichDigitSummarys
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-13
   */
  class MCRichDigitSummaryPacker {
  public:
    // These are required by the templated algorithms
    typedef LHCb::MCRichDigitSummary        Data;
    typedef LHCb::PackedMCRichDigitSummary  PackedData;
    typedef LHCb::MCRichDigitSummarys       DataVector;
    typedef LHCb::PackedMCRichDigitSummarys PackedDataVector;
    static const std::string&               packedLocation() { return LHCb::PackedMCRichDigitSummaryLocation::Default; }
    static const std::string&               unpackedLocation() { return LHCb::MCRichDigitSummaryLocation::Default; }

  private:
    /// Default Constructor hidden
    MCRichDigitSummaryPacker() {}

  public:
    /// Default Constructor
    MCRichDigitSummaryPacker( const GaudiAlgorithm* p ) : m_pack( p ) {}

  public:
    /// Pack MCRichDigitSummarys
    void pack( const DataVector& hits, PackedDataVector& phits ) const;

    /// Unpack MCRichDigitSummarys
    void unpack( const PackedDataVector& phits, DataVector& hits ) const;

    /// Compare two MCRichDigitSummarys to check the packing -> unpacking performance
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
        throw GaudiException( mess.str(), "MCRichDigitSummaryPacker", StatusCode::FAILURE );
      }
      return OK;
    }

  private:
    /// Standard packing of quantities into integers ...
    StandardPacker m_pack;
  };

  // -----------------------------------------------------------------------

} // namespace LHCb

#endif // EVENT_PackedMCRichDigitSummary_H
