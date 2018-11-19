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
#ifndef EVENT_PackedCaloAdc_H
#define EVENT_PackedCaloAdc_H 1

// STL
#include <string>

// Kernel
#include "Event/StandardPacker.h"

// Event
#include "Event/CaloAdc.h"

// Gaudi
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/GaudiException.h"

namespace LHCb{
  // -----------------------------------------------------------------------


  /** @struct PackedCaloAdc Event/PackedCaloAdc.h
   *
   *  Packed LHCb::CaloAdc
   *
   *  @author Olivier Deschamps
   *  @date   2017-06-08
   */
  struct PackedCaloAdc{
    int key{0};
    int adc{0};

    template<typename T>
    inline void save(T& buf) const {
      buf.io( key, adc );
    }

    template<typename T>
    inline void load(T& buf, unsigned int /*version*/) {
      save(buf); // identical operation until version is incremented
    }
  };

  // -----------------------------------------------------------------------

  static const CLID CLID_PackedCaloAdcs = 1543;

  /// Namespace for locations in TDS
  namespace PackedCaloAdcLocation
  {
    static const std::string& Ecal    = "pRec/Ecal/Adcs";
    static const std::string& Hcal    = "pRec/Hcal/Adcs";
    static const std::string& Prs     = "pRec/Prs/Adcs";
    static const std::string& Spd     = "pRec/Spd/Adcs";
  }

  /** @class PackedCaloAdcs Event/PackedCaloAdc.h
   *
   *  Packed Calo ADCs
   *
   *  @author Olivier Deschamps
   *  @date   2017-06-08
   */
  class PackedCaloAdcs : public DataObject{
  public:
    /// Vector of PackedCaloAdc objects
    typedef std::vector<LHCb::PackedCaloAdc> CaloAdcVector;

  public:

    /// Default Packing Version
    static char defaultPackingVersion() { return 0; }

  public:

    /// Class ID
    static const CLID& classID() { return CLID_PackedCaloAdcs; }

    /// Class ID
    const CLID& clID() const override { return PackedCaloAdcs::classID(); }

  public:

    /// Write access to the data vector
    CaloAdcVector & data()                     { return m_adcs; }

    /// Read access to the data vector
    const CaloAdcVector & data() const         { return m_adcs; }

    /// Set the packing version
    void setPackingVersion( const char ver ) { m_packingVersion = ver; }

    /// Access the packing version
    char packingVersion() const { return m_packingVersion; }

    /// Describe serialization of object
    template<typename T>
    inline void save(T& buf) const {
      buf.template save<uint8_t>(m_packingVersion);
      buf.template save<uint8_t>(version());
      buf.save(m_adcs);
    }

    /// Describe de-serialization of object
    template<typename T>
    inline void load(T& buf) {
      setPackingVersion(buf.template load<uint8_t>());
      setVersion(buf.template load<uint8_t>());
      if (m_packingVersion > defaultPackingVersion()) {
        throw std::runtime_error("PackedCaloAdcs packing version is not supported: "
                                 + std::to_string(m_packingVersion));
      }
      buf.load(m_adcs, m_packingVersion);
    }

  private:

    /// Data packing version
    char m_packingVersion{ defaultPackingVersion() };

    /// The packed data objects
    CaloAdcVector m_adcs;

  };

  // -----------------------------------------------------------------------

  /** @class CaloAdcPacker Event/PackedCaloAdc.h
   *
   *  Utility class to handle the packing and unpacking of the Calo ADCs
   *
   *  @author Olivier Deschamps
   *  @date   2017-06-08
   */
  class CaloAdcPacker{
  public:

    // These are required by the templated algorithms
    typedef LHCb::CaloAdc                     Data;
    typedef LHCb::PackedCaloAdc               PackedData;
    typedef LHCb::CaloAdcs                    DataVector;
    typedef LHCb::PackedCaloAdcs              PackedDataVector;
    static const std::string& packedLocation()  {return LHCb::PackedCaloAdcLocation::Ecal;}
    static const std::string& unpackedLocation(){return LHCb::CaloAdcLocation::Ecal;}

    /// Constructor
    CaloAdcPacker( const GaudiAlgorithm * p ) : m_pack(p) { }

  public:

    /// Pack Calo ADCs
    void pack( const DataVector & adc, PackedDataVector & padc ) const;

    /// Unpack Calo ADCs
    void unpack( const PackedDataVector & padc, DataVector             & adcs ) const;

    /// Compare two Calo ADCs containers to check the packing -> unpacking performance
    StatusCode check( const DataVector & dataA, const DataVector & dataB ) const;

    /// Compare two Calo ADCs to check the packing -> unpacking performance
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
        throw GaudiException( mess.str(), "CaloAdcPacker", StatusCode::FAILURE );
      }
      return OK;
    }

  private:

    /// Standard packing of quantities into integers ...
    StandardPacker m_pack;

  };

  // -----------------------------------------------------------------------

}

#endif // EVENT_PackedCaloAdc_H
