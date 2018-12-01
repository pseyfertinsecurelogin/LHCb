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
#ifndef EVENT_PackedCaloDigit_H
#define EVENT_PackedCaloDigit_H 1

// STL
#include <string>

// Kernel
#include "Event/StandardPacker.h"

// Event
#include "Event/CaloDigit.h"

// Gaudi
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/GaudiException.h"

namespace LHCb{
  // -----------------------------------------------------------------------


  /** @struct PackedCaloDigit Event/PackedCaloDigit.h
   *
   *  Packed LHCb::CaloDigit
   *
   *  @author Olivier Deschamps
   *  @date   2017-06-08
   */
  struct PackedCaloDigit{
    int key{0};
    int e{0};

    template<typename T>
    inline void save(T& buf) const {
      buf.io( key, e );
    }

    template<typename T>
    inline void load(T& buf, unsigned int /*version*/) {
      save(buf); // identical operation until version is incremented
    }
  };

  // -----------------------------------------------------------------------

  constexpr CLID CLID_PackedCaloDigits = 1542;

  /// Namespace for locations in TDS
  namespace PackedCaloDigitLocation
  {
    inline const std::string Default = "pRec/Calo/Digits";
    inline const std::string Ecal    = "pRec/Ecal/Digits";
    inline const std::string Hcal    = "pRec/Hcal/Digits";
    inline const std::string Prs     = "pRec/Prs/Digits";
    inline const std::string Spd     = "pRec/Spd/Digits";
  }

  /** @class PackedCaloDigits Event/PackedCaloDigit.h
   *
   *  Packed Calo Digits
   *
   *  @author Olivier Deschamps
   *  @date   2017-06-08
   */
  class PackedCaloDigits : public DataObject{
  public:
    /// Vector of PackedCaloDigit objects
    typedef std::vector<LHCb::PackedCaloDigit> CaloDigitVector;

  public:

    /// Default Packing Version
    static char defaultPackingVersion() { return 0; }

  public:

    /// Class ID
    static const CLID& classID() { return CLID_PackedCaloDigits; }

    /// Class ID
    const CLID& clID() const override { return PackedCaloDigits::classID(); }

  public:

    /// Write access to the data vector
    CaloDigitVector & data()                     { return m_digits; }

    /// Read access to the data vector
    const CaloDigitVector & data() const         { return m_digits; }

    /// Set the packing version
    void setPackingVersion( const char ver ) { m_packingVersion = ver; }

    /// Access the packing version
    char packingVersion() const { return m_packingVersion; }

    /// Describe serialization of object
    template<typename T>
    inline void save(T& buf) const {
      buf.template save<uint8_t>(m_packingVersion);
      buf.template save<uint8_t>(version());
      buf.save(m_digits);
    }

    /// Describe de-serialization of object
    template<typename T>
    inline void load(T& buf) {
      setPackingVersion(buf.template load<uint8_t>());
      setVersion(buf.template load<uint8_t>());
      if (m_packingVersion > defaultPackingVersion()) {
        throw std::runtime_error("PackedCaloDigits packing version is not supported: "
                                 + std::to_string(m_packingVersion));
      }
      buf.load(m_digits, m_packingVersion);
    }

  private:

    /// Data packing version
    char m_packingVersion{ defaultPackingVersion() };

    /// The packed data objects
    CaloDigitVector m_digits;

  };

  // -----------------------------------------------------------------------

  /** @class CaloDigitPacker Event/PackedCaloDigit.h
   *
   *  Utility class to handle the packing and unpacking of the Calo Digits
   *
   *  @author Olivier Deschamps
   *  @date   2017-06-08
   */
  class CaloDigitPacker{
  public:

    // These are required by the templated algorithms
    typedef LHCb::CaloDigit                     Data;
    typedef LHCb::PackedCaloDigit               PackedData;
    typedef LHCb::CaloDigits                    DataVector;
    typedef LHCb::PackedCaloDigits              PackedDataVector;
    static const std::string& packedLocation()  {return LHCb::PackedCaloDigitLocation::Default;}
    static const std::string& unpackedLocation(){return LHCb::CaloDigitLocation::Default;}

    /// Constructor
    CaloDigitPacker( const GaudiAlgorithm * p ) : m_pack(p) { }

  public:

    /// Pack Calo Digits
    void pack( const DataVector & dig, PackedDataVector & pdig ) const;

    /// Unpack Calo Digits
    void unpack( const PackedDataVector & pdig, DataVector             & digs ) const;

    /// Compare two Calo Digits containers to check the packing -> unpacking performance
    StatusCode check( const DataVector & dataA, const DataVector & dataB ) const;

    /// Compare two Calo Digits to check the packing -> unpacking performance
    StatusCode check( const Data & dataA, const Data & dataB ) const;

  public:

    /// Access the parent algorithm
    const GaudiAlgorithm& parent() const { return *(m_pack.parent()); }

    /// Check if the given packing version is supported
    bool isSupportedVer( const char& ver ) const{
      const bool OK = ( 0 == ver );
      if ( UNLIKELY(!OK) ){
        std::ostringstream mess;
        mess << "Unknown packed data version " << (int)ver;
        throw GaudiException( mess.str(), "CaloDigitPacker", StatusCode::FAILURE );
      }
      return OK;
    }

  private:

    /// Standard packing of quantities into integers ...
    StandardPacker m_pack;

  };

  // -----------------------------------------------------------------------

}

#endif // EVENT_PackedCaloDigit_H
