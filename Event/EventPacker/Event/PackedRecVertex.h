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
#ifndef EVENT_PACKEDRECVERTEX_H
#define EVENT_PACKEDRECVERTEX_H 1

// STL
#include <string>
#include <vector>

// Gaudi
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/GaudiException.h"

// Kernel
#include "Event/StandardPacker.h"

// Event
#include "Event/RecVertex.h"

namespace LHCb
{

  // -----------------------------------------------------------------------

  /** @class PackedRecVertex Event/PackedRecVertex.h
   *
   *  Structure to describe a reconstructed vertex
   *
   *  @author Olivier Callot
   *  @date   2008-11-14
   */
  struct PackedRecVertex
  {
    int key{0};
    int technique{0};
    int chi2{0};
    int nDoF{0};
    int x{0};
    int y{0};
    int z{0};
    int cov00{0};
    int cov11{0};
    int cov22{0};
    short int cov10{0};
    short int cov20{0};
    short int cov21{0};
    unsigned short int firstTrack{0},  lastTrack{0};
    unsigned short int firstInfo{0},   lastInfo{0};
    int container{0};

    template<typename T>
    inline void save(T& buf) const {
      buf.io(
        key, technique, chi2, nDoF,
        x, y, z,
        cov00, cov11, cov22, cov10, cov20, cov21,
        firstTrack, lastTrack,
        firstInfo, lastInfo,
        container
      );
    }

    template<typename T>
    inline void load(T& buf, unsigned int version) {
      if (version == 1) {
        buf.io(
          key, technique, chi2, nDoF,
          x, y, z,
          cov00, cov11, cov22, cov10, cov20, cov21,
          firstTrack, lastTrack,
          firstInfo, lastInfo
        );
        // in packing version 1, container was not serialized!
        container = 0;  // initialize "container" with a well defined value
                        // as it is not serialized
      } else {
        save(buf); // identical operation for the latest version
      }
    }
  };

  // -----------------------------------------------------------------------

  constexpr CLID CLID_PackedRecVertices = 1553;

  // Namespace for locations in TDS
  namespace PackedRecVertexLocation
  {
    inline const std::string Primary = "pRec/Vertex/Primary";
    inline const std::string InStream = "/pPhys/RecVertices";
  }

  /** @class PackedRecVertices Event/PackedRecVertex.h
   *
   *  Container of packed RecVertex objects
   *
   *  @author Olivier Callot
   *  @date   2008-11-14
   */

  class PackedRecVertices : public DataObject
  {

  public:

    /// Default Packing Version
    static char defaultPackingVersion() { return 2; }

  public:

    /// Vector of packed objects
    typedef std::vector<LHCb::PackedRecVertex> Vector;

  public:

    /// Standard constructor
    PackedRecVertices( )
    {
      m_vect.reserve    ( 5   );
      m_refs.reserve    ( 100 );
      m_weights.reserve ( 100 );
      m_extra.reserve   ( 250 );
    }

  public:

    const CLID& clID()  const override { return PackedRecVertices::classID(); }
    static  const CLID& classID()     { return CLID_PackedRecVertices;       }

  public:

    std::vector<PackedRecVertex>& vertices()                   { return m_vect; }
    const std::vector<PackedRecVertex>& vertices() const       { return m_vect; }

    std::vector<long long>& refs()                         { return m_refs; }
    const std::vector<long long>& refs() const             { return m_refs; }

    void addExtra( const int a, const int b ) { m_extra.emplace_back( std::make_pair(a,b) ); }
    std::vector<std::pair<int,int> >& extras()             { return m_extra; }
    const std::vector<std::pair<int,int> >& extras() const { return m_extra; }

    std::vector<short int>& weights()             { return m_weights; }
    const std::vector<short int>& weights() const { return m_weights; }

  public:

    /// Set the packing version
    void setPackingVersion( const char ver ) { m_packingVersion = ver; }

    /// Access the packing version
    char packingVersion() const { return m_packingVersion; }

    /// Describe serialization of object
    template<typename T>
    inline void save(T& buf) const {
      buf.template save<uint8_t>(m_packingVersion);
      buf.template save<uint8_t>(version());
      buf.save(m_vect);
      buf.save(m_refs);
      buf.save(m_extra);
      buf.save(m_weights);
    }

    /// Describe de-serialization of object
    template<typename T>
    inline void load(T& buf) {
      setPackingVersion(buf.template load<uint8_t>());
      setVersion(buf.template load<uint8_t>());
      if (m_packingVersion < 1 || m_packingVersion > defaultPackingVersion()) {
        throw std::runtime_error("RecVertices packing version is not supported: "
                                 + std::to_string(m_packingVersion));
      }
      buf.load(m_vect, m_packingVersion);
      buf.load(m_refs);
      buf.load(m_extra, m_packingVersion);
      buf.load(m_weights);
    }

  private:

    std::vector<PackedRecVertex>     m_vect;
    std::vector<long long>           m_refs;
    std::vector<std::pair<int,int> > m_extra;
    std::vector<short int>           m_weights;

    /// Data packing version
    char m_packingVersion{0};

  };

  // -----------------------------------------------------------------------

  /** @class RecVertexPacker Event/PackedRecVertex.h
   *
   *  Utility class to handle the packing and unpacking of the RecVertices
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-13
   */
  class RecVertexPacker
  {

  public:

    // These are required by the templated algorithms
    typedef LHCb::RecVertex                     Data;
    typedef LHCb::PackedRecVertex         PackedData;
    typedef LHCb::RecVertices             DataVector;
    typedef LHCb::PackedRecVertices PackedDataVector;
    static const std::string& packedLocation()   { return LHCb::PackedRecVertexLocation::Primary; }
    static const std::string& unpackedLocation() { return LHCb::RecVertexLocation::Primary; }

  private:

    /// Default Constructor hidden
    RecVertexPacker() {}

  public:

    /// Constructor
    RecVertexPacker( const GaudiAlgorithm * parent ) : m_pack(parent) {}

  public:

    /// Pack a Vertex
    void pack( const Data & vert,
               PackedData & pvert,
               const DataVector & verts,
               PackedDataVector & pverts ) const;

    /// Pack Vertices
    void pack( const DataVector & verts,
               PackedDataVector & pverts ) const;

    /// Unpack a Vertex
    void unpack( const PackedData       & pvert,
                 Data                   & vert,
                 const PackedDataVector & pverts,
                 DataVector             & verts ) const;

    /// Unpack Vertices
    void unpack( const PackedDataVector & pverts,
                 DataVector             & verts ) const;

  private:

    /// Access the parent algorithm
    inline const GaudiAlgorithm& parent() const { return *(m_pack.parent()); }

    /// Safe sqrt ...
    inline double safe_sqrt( const double x ) const
    { return ( x > 0 ? std::sqrt(x) : 0.0 ); }

    /// Check if the given packing version is supported
    bool isSupportedVer( const char& ver ) const
    {
      const bool OK = ( 2 == ver || 1 == ver || 0 == ver );
      if ( UNLIKELY(!OK) )
      {
        std::ostringstream mess;
        mess << "Unknown packed data version " << (int)ver;
        throw GaudiException( mess.str(), "RecVertexPacker", StatusCode::FAILURE );
      }
      return OK;
    }

  private:

    /// Standard packing of quantities into integers ...
    StandardPacker m_pack;

  };

  // -----------------------------------------------------------------------

}

#endif // EVENT_PACKEDRECVERTEX_H
