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

#ifndef EVENT_PACKEDCALOHYPO_H
#define EVENT_PACKEDCALOHYPO_H 1

#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/StatusCode.h"

// Kernel
#include "Event/StandardPacker.h"

// Event
#include "Event/CaloHypo.h"

#include <string>
#include <vector>

namespace LHCb {

  /** @struct PackedCaloHypo Event/PackedCaloHypo.h
   *  Packed description of a CaloHypo
   *
   *  @author Olivier Callot
   *  @date   2008-11-10
   */
  struct PackedCaloHypo {
    int key{0};
    int hypothesis{0};
    int lh{0};
    // from CaloPosition
    int z{0};
    // position (3) + 3x3 symmetric covariance matrix
    int posX{0}, posY{0}, posE{0};

    int       cov00{0}, cov11{0}, cov22{0};
    short int cov10{0}, cov20{0}, cov21{0};
    short int cerr10{0}; // non diagonal terms of the x,y spread matrix.
    // center in x,y + 2x2 symetric covariance matrix
    int centX{0};
    int centY{0};
    int cerr00{0};
    int cerr11{0};

    unsigned short int firstDigit{0};
    unsigned short int lastDigit{0};
    unsigned short int firstCluster{0};
    unsigned short int lastCluster{0};
    unsigned short int firstHypo{0};
    unsigned short int lastHypo{0};

    template <typename T>
    inline void save( T& buf ) const {
      buf.io( key, hypothesis, lh, z, posX, posY, posE, cov00, cov11, cov22, cov10, cov20, cov21, cerr10, centX, centY,
              cerr00, cerr11, firstDigit, lastDigit, firstCluster, lastCluster, firstHypo, lastHypo );
    }

    template <typename T>
    inline void load( T& buf, unsigned int /*version*/ ) {
      save( buf ); // identical operation until version is incremented
    }
  };

  constexpr CLID CLID_PackedCaloHypos = 1551;

  // Namespace for locations in TDS
  namespace PackedCaloHypoLocation {
    inline const std::string Photons      = "pRec/Calo/Photons";
    inline const std::string Electrons    = "pRec/Calo/Electrons";
    inline const std::string MergedPi0s   = "pRec/Calo/MergedPi0s";
    inline const std::string SplitPhotons = "pRec/Calo/SplitPhotons";
  } // namespace PackedCaloHypoLocation

  /** @class PackedCaloHypos Event/PackedCaloHypo.h
   *
   *  Vector of packed CaloHypos
   *
   *  @author Olivier Callot
   *  @date   2008-11-10
   */

  class PackedCaloHypos : public DataObject {

  public:
    /// Default Packing Version
    static char defaultPackingVersion() { return 1; }

  public:
    /// Standard constructor
    PackedCaloHypos() {
      m_vect.reserve( 100 );
      m_refs.reserve( 1000 );
    }

  public:
    const CLID&        clID() const override { return PackedCaloHypos::classID(); }
    static const CLID& classID() { return CLID_PackedCaloHypos; }

  public:
    std::vector<PackedCaloHypo>&       hypos() { return m_vect; }
    const std::vector<PackedCaloHypo>& hypos() const { return m_vect; }

    std::vector<long long>&       refs() { return m_refs; }
    const std::vector<long long>& refs() const { return m_refs; }

  public:
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
      buf.save( m_refs );
    }

    /// Describe de-serialization of object
    template <typename T>
    inline void load( T& buf ) {
      setPackingVersion( buf.template load<uint8_t>() );
      setVersion( buf.template load<uint8_t>() );
      if ( m_packingVersion < 1 || m_packingVersion > defaultPackingVersion() ) {
        throw std::runtime_error( "PackedCaloHypos packing version is not supported: " +
                                  std::to_string( m_packingVersion ) );
      }
      buf.load( m_vect, m_packingVersion );
      buf.load( m_refs );
    }

  private:
    std::vector<PackedCaloHypo> m_vect;
    std::vector<long long>      m_refs;

    /// Data packing version
    char m_packingVersion{0};
  };

  /** @class CaloHypoPacker Event/PackedCaloHypo.h
   *
   *  Utility class to handle the packing and unpacking of CaloHypos
   *
   *  @author Christopher Rob Jones
   *  @date   05/04/2012
   */
  class CaloHypoPacker {

  public:
    typedef LHCb::CaloHypo        Data;
    typedef LHCb::PackedCaloHypo  PackedData;
    typedef LHCb::CaloHypos       DataVector;
    typedef LHCb::PackedCaloHypos PackedDataVector;

    /// Constructor
    CaloHypoPacker( const GaudiAlgorithm* p ) : m_pack( p ) {}

  public:
    /// Pack CaloHypos
    void pack( const DataVector& hypos, PackedDataVector& phypos ) const;

    /// Unpack CaloHypos
    void unpack( const PackedDataVector& phypos, DataVector& hypos ) const;

    /// Compare two CaloHypos to check the packing -> unpacking performance
    StatusCode check( const DataVector& dataA, const DataVector& dataB ) const;

  private:
    /// Access the parent algorithm
    const GaudiAlgorithm& parent() const { return *( m_pack.parent() ); }

    /// Safe sqrt ...
    inline double safe_sqrt( const double x ) const { return ( x > 0 ? std::sqrt( x ) : 0.0 ); }

    /// Check if the given packing version is supported
    bool isSupportedVer( const char& ver ) const {
      const bool OK = ( 1 == ver || 0 == ver );
      if ( !OK ) {
        std::ostringstream mess;
        mess << "Unknown packed data version " << (int)ver;
        throw GaudiException( mess.str(), "CaloHypoPacker", StatusCode::FAILURE );
      }
      return OK;
    }

  private:
    /// Standard packing of quantities into integers ...
    StandardPacker m_pack;
  };

} // namespace LHCb

#endif // EVENT_PACKEDCALOHYPO_H
