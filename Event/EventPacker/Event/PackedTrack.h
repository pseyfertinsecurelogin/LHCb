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
#pragma once

// STL
#include <string>
#include <vector>
#include <limits>
#include <cstdint>
#include <array>

// Kernel
#include "Event/StandardPacker.h"

// Event
#include "Event/Track.h"

// Gaudi
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/StatusCode.h"

namespace LHCb
{

  /** @class PackedTrack PackedTrack.h Event/PackedTrack.h
   *
   *  Packed description of a track
   *
   *  Version 2: Added new data members to PackedTrack to follow the upgraded Track.
   *
   *  @author Olivier Callot
   *  @date   2009-08-26
   */
  struct PackedTrack
  {

    long long key{0};
    int chi2PerDoF{0};
    int nDoF{0};
    unsigned int flags{0};
    int firstId{0};
    int lastId{0};
    int firstState{0};
    int lastState{0};
    int firstExtra{0};
    int lastExtra{0};
    //== Added for version 3, August 2009
    int likelihood{0};
    int ghostProba{0};

    //== Note that Nodes and Measurements on Track are transient only, an thus never stored.

    template<typename T>
    inline void save(T& buf) const
    {
      buf.io( key,
              chi2PerDoF, nDoF, flags,
              firstId, lastId,
              firstState, lastState,
              firstExtra, lastExtra,
              likelihood,
              ghostProba );
    }

    template<typename T>
    inline void load(T& buf, unsigned int version)
    {
      if (version == 4)
      {
        buf.io( key, chi2PerDoF, nDoF, flags );
        buf.template io<uint16_t>(firstId);
        buf.template io<uint16_t>(lastId);
        buf.template io<uint16_t>(firstState);
        buf.template io<uint16_t>(lastState);
        buf.template io<uint16_t>(firstExtra);
        buf.template io<uint16_t>(lastExtra);
        buf.io( likelihood, ghostProba );
      }
      else
      {
        save( buf ); // identical operation for the latest version
      }
    }
  };

  /** @class PackedState PackedState.h Event/PackedState.h
   *
   *  Describe a packed state
   *
   *  @author Olivier Callot
   *  @date   2008-11-07
   */
  struct PackedState
  {

    int flags{0};

    int x{0};
    int y{0};
    int z{0};
    int tx{0};
    int ty{0};
    int p{0};

    int cov_00{0};
    int cov_11{0};
    int cov_22{0};
    int cov_33{0};
    int cov_44{0};
    short int cov_10{0};
    short int cov_20{0};
    short int cov_21{0};
    short int cov_30{0};
    short int cov_31{0};
    short int cov_32{0};
    short int cov_40{0};
    short int cov_41{0};
    short int cov_42{0};
    short int cov_43{0};

    template<typename T>
    inline void save(T& buf) const
    {
      buf.io( flags,
              x, y, z, tx, ty, p,
              cov_00, cov_11, cov_22, cov_33, cov_44,
              cov_10, cov_20, cov_21, cov_30, cov_31,
              cov_32, cov_40, cov_41, cov_42, cov_43 );
    }

    template<typename T>
    inline void load(T& buf, unsigned int /*version*/)
    {
      save(buf); // identical operation until version is incremented
    }

  };

  static const CLID CLID_PackedTracks = 1550;

  // Namespace for locations in TDS
  namespace PackedTrackLocation
  {
    static const std::string& Default  =  "pRec/Track/Best";
    static const std::string& Muon     =  "pRec/Track/Muon";
    static const std::string& InStream = "/pRec/Track/Custom";
  }

  /** @class PackedTracks PackedTrack.h Event/PackedTrack.h
   *
   *  Container of packed Tracks
   *
   *  @author Olivier Callot
   *  @date   2009-08-26
   */

  class PackedTracks : public DataObject
  {

  public:

    /// Standard constructor
    PackedTracks( )
    {
      m_vect.reserve(500);
      m_state.reserve(2000);
      m_ids.reserve(2000);
      m_extra.reserve(5000);
    }

  public:

    const CLID& clID()  const override { return PackedTracks::classID(); }
    static  const CLID& classID()     { return CLID_PackedTracks;       }

  public:

    std::vector<PackedTrack>&       tracks()               { return m_vect; }
    const std::vector<PackedTrack>& tracks() const         { return m_vect; }

    std::vector<int>&       ids()                          { return m_ids; }
    const std::vector<int>& ids() const                    { return m_ids; }

    std::vector<PackedState>&       states()               { return m_state; }
    const std::vector<PackedState>& states() const         { return m_state; }

    std::vector<std::pair<int,int> >&       extras()       { return m_extra; }
    const std::vector<std::pair<int,int> >& extras() const { return m_extra; }

    /// Describe serialization of object
    template<typename T>
    inline void save(T& buf) const
    {
      buf.template save<uint8_t>( version() );
      buf.save(m_vect);
      buf.save(m_state);
      buf.save(m_ids);
      buf.save(m_extra);
    }

    /// Describe de-serialization of object
    template<typename T>
    inline void load(T& buf)
    {
      setVersion( buf.template load<uint8_t>() );
      if ( version() < 4 || version() > 5 )
      {
        throw std::runtime_error("PackedTracks packing version is not supported: "
                                 + std::to_string(version()));
      }
      buf.load(m_vect, version());
      buf.load(m_state, version());
      buf.load(m_ids);
      buf.load(m_extra, version());
    }

  private:

    std::vector<PackedTrack>         m_vect;
    std::vector<PackedState>         m_state;
    std::vector<int>                 m_ids;
    std::vector<std::pair<int,int> > m_extra;

  };

  /** @class TrackPacker Event/PackedTrack.h
   *
   *  Utility class to handle the packing and unpacking of Tracks
   *
   *  @author Christopher Rob Jones
   *  @date   05/04/2012
   */
  class TrackPacker
  {

  public:

    typedef LHCb::Track                    Data;
    typedef LHCb::PackedTrack        PackedData;
    typedef LHCb::Tracks             DataVector;
    typedef LHCb::PackedTracks PackedDataVector;

  public:

    /// Default Constructor
    TrackPacker( const GaudiAlgorithm * p ) : m_pack(p) { }

  public:

    /// Pack a Track
    void pack( const Data & track,
               PackedData & ptrack,
               PackedDataVector & ptracks ) const;

    /// Pack Tracks
    void pack( const DataVector & tracks,
               PackedDataVector & ptracks ) const;

    /// Unpack a single Track
    void unpack( const PackedData       & ptrack,
                 Data                   & track,
                 const PackedDataVector & ptracks,
                 DataVector             & tracks ) const;

    /// Unpack Tracks
    void unpack( const PackedDataVector & ptracks,
                 DataVector             & tracks ) const;

    /// Compare two Track containers to check the packing -> unpacking performance
    StatusCode check( const DataVector & dataA,
                      const DataVector & dataB ) const;

    /// Compare two Tracks to check the packing -> unpacking performance
    StatusCode check( const Data & dataA,
                      const Data & dataB ) const;

  private:

    /// Convert a state to a packed state
    void convertState( const LHCb::State& state,
                       PackedDataVector & ptracks ) const;

    /// Convert a packed state to a state in a track
    void convertState( const LHCb::PackedState& pSta,
                       LHCb::Track & tra ) const;

    void compareStates( const LHCb::State& oSta,
                        const LHCb::State& tSta ) const;

  private:

    /// Access the parent algorithm
    inline const GaudiAlgorithm& parent() const { return *(m_pack.parent()); }

    /// Safe sqrt ...
    template< typename TYPE >
    inline TYPE safe_sqrt( const TYPE x ) const
    { 
      return ( x > TYPE(0) ? std::sqrt(x) : TYPE(0) );
    }

    /// Safe divide ...
    template< typename TYPE >
    inline TYPE safe_divide( const TYPE a, const TYPE b ) const
    { 
      return ( b != TYPE(0) ? a/b : 9e9 );
    }

    /// Check if the given packing version is supported
    bool isSupportedVer( const char ver ) const
    {
      const bool OK = ( 0 <= ver && ver <= 5 );
      if ( UNLIKELY(!OK) )
      {
        std::ostringstream mess;
        mess << "Unknown packed data version " << (int)ver;
        throw GaudiException( mess.str(), "TrackPacker", StatusCode::FAILURE );
      }
      return OK;
    }

  public:

    /// Reset wraping bug counts
    inline void resetWrappingCounts() const
    {
      m_firstIdHigh     = 0;
      m_lastIdHigh      = 0;
      m_firstStateHigh  = 0;
      m_lastStateHigh   = 0;
      m_firstExtraHigh  = 0;
      m_lastExtraHigh   = 0;
      m_lastPackedDataV = nullptr;
    }

  private:

    /// Standard packing of quantities into integers ...
    StandardPacker m_pack;

  private:

    // cached data to handle wrapped ID numbers ...
    mutable int m_firstIdHigh{0};
    mutable int m_lastIdHigh{0};
    mutable int m_firstStateHigh{0};
    mutable int m_lastStateHigh{0};
    mutable int m_firstExtraHigh{0};
    mutable int m_lastExtraHigh{0};

    // Cache the pointers to the last packed and unpacked containers
    mutable const PackedDataVector * m_lastPackedDataV = nullptr;

  };

}  // End of LHCb namespace
