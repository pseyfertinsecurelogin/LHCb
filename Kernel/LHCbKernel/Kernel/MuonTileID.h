/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

// Include files
#include "Kernel/MuonBase.h"
#include "Kernel/MuonLayout.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class MuonTileID MuonTileID.h
   *
   * Universal identifier for the Muon System entities
   *
   * @author Andrei Tsaregorodtsev
   *
   */

  class MuonTileID final {
  public:
    /// Default constructor
    MuonTileID() : m_muonid( 0 ) {}

    /// Constructor with all the arguments
    MuonTileID( int station, const IMuonLayout& lay, int region, int quarter, int x, int y );

    /// Constructor from similar MuonTileID
    MuonTileID( const MuonTileID& id, const unsigned int region, const unsigned int quarter, const unsigned int x,
                const unsigned int y );

    /// Constructor from relative position in the containing MuonTile MuonTileID
    MuonTileID( const MuonTileID& id, const IMuonLayout& layout, const unsigned int x, const unsigned int y );

    /// Constructor taking a long needed for the keyed container
    MuonTileID( const long int muonid );

    /// Constructor from a string representation of the MuonTileID
    MuonTileID( const std::string& strid );

    /// convert the MuonTileID to an int for use as a key
    operator long int() const;

    /// Function to extract station
    unsigned int station() const;

    /// Function to extract region
    unsigned int region() const;

    /// Function to extract quarter
    unsigned int quarter() const;

    /// Function to extract layout
    MuonLayout layout() const;

    /// Function to extract index in x
    unsigned int nX() const;

    /// Function to extract index in y
    unsigned int nY() const;

    /// Function to extract lower part of the identifier
    unsigned int index() const;

    /// Function to extract lower part of the identifier with station information
    unsigned int key() const;

    /// comparison operator using key.
    bool operator<( const MuonTileID& id ) const;

    /// equality operator using key.
    bool operator==( const MuonTileID& id ) const;

    /// non-equality operator using key.
    bool operator!=( const MuonTileID& id ) const;

    /// Find the MuonTileID which is an interception of two MuonTileID's
    MuonTileID intercept( const MuonTileID& id ) const;

    /// Find the MuonTileID which is an interception of two MuonTileID's
    MuonTileID interceptSameRegion( const MuonTileID& id ) const;

    /// Find the MuonTileID of a Tile of a given layout containing this pad
    MuonTileID containerID( const IMuonLayout& layout ) const;

    /// Find the MuonTileID of a Tile which is a neighbour of this pad
    MuonTileID neighbourID( int dirX, int dirY ) const;

    /// Get local offset X with respect to the container defined by the given IMuonLayout
    int localX( const IMuonLayout& layout ) const;

    /// Get local offset y with respect to the container defined by the given IMuonLayout
    int localY( const IMuonLayout& layout ) const;

    /// Check that the MuonTileID is consistent in terms of its layout
    bool isValid() const;

    /// Check that the MuonTileID is defined. It means that its code is not 0
    bool isDefined() const;

    /// presents the MuonTileID as a readable string
    std::string toString() const;

    /// update station identifier
    void setStation( const unsigned int station );

    /// update region identifier
    void setRegion( const unsigned int region );

    /// update quarter identifier
    void setQuarter( const unsigned int quarter );

    /// update index in x
    void setX( const unsigned int x );

    /// update index in y
    void setY( const unsigned int y );

    /// update layout identifier
    void setLayout( const MuonLayout& layout );

    /// modify index in x
    void deltaX( int dx );

    /// modify index in y
    void deltaY( int dy );

    /// Print this MuonTileID in a human readable way
    std::ostream& fillStream( std::ostream& s ) const;

    friend std::ostream& operator<<( std::ostream& str, const MuonTileID& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// modify index in y
    void set( const unsigned int Value, const unsigned int Shift, const unsigned int Mask );

    unsigned int m_muonid; ///< muon tile id

  }; // class MuonTileID

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::MuonTileID::MuonTileID( int station, const IMuonLayout& lay, int region, int quarter, int x, int y )
    : m_muonid( 0 ) {

  setStation( station );
  setRegion( region );
  setQuarter( quarter );
  setX( x );
  setY( y );
  setLayout( MuonLayout( lay.grid( *this ) ) );
}

inline LHCb::MuonTileID::MuonTileID( const MuonTileID& id, const unsigned int region, const unsigned int quarter,
                                     const unsigned int x, const unsigned int y )
    : m_muonid( id.m_muonid ) {

  setQuarter( quarter );
  setRegion( region );
  setX( x );
  setY( y );
}

inline LHCb::MuonTileID::MuonTileID( const long int muonid ) { m_muonid = static_cast<unsigned int>( muonid ); }

inline LHCb::MuonTileID::MuonTileID( const std::string& strid ) : m_muonid( 0 ) {

  const char* str = strid.c_str() + 1;
  char*       endptr;

  long sta     = strtol( str, &endptr, 0 );
  str          = endptr + 1;
  long mlx     = strtol( str, &endptr, 0 );
  str          = endptr + 1;
  long mly     = strtol( str, &endptr, 0 );
  str          = endptr + 2;
  long quarter = strtol( str, &endptr, 0 );
  str          = endptr + 2;
  long region  = strtol( str, &endptr, 0 );
  str          = endptr + 1;
  long nx      = strtol( str, &endptr, 0 );
  str          = endptr + 1;
  long ny      = strtol( str, &endptr, 0 );

  setStation( sta );
  setLayout( MuonLayout( mlx, mly ) );
  setQuarter( quarter );
  setRegion( region );
  setX( nx );
  setY( ny );
}

inline LHCb::MuonTileID::operator long int() const { return static_cast<long int>( m_muonid ); }

inline unsigned int LHCb::MuonTileID::station() const {
  return ( m_muonid & MuonBase::MaskStation ) >> MuonBase::ShiftStation;
}

inline unsigned int LHCb::MuonTileID::region() const {
  return ( m_muonid & MuonBase::MaskRegion ) >> MuonBase::ShiftRegion;
}

inline unsigned int LHCb::MuonTileID::quarter() const {
  return ( m_muonid & MuonBase::MaskQuarter ) >> MuonBase::ShiftQuarter;
}

inline MuonLayout LHCb::MuonTileID::layout() const {

  int xg = ( m_muonid & MuonBase::MaskLayoutX ) >> MuonBase::ShiftLayoutX;
  int yg = ( m_muonid & MuonBase::MaskLayoutY ) >> MuonBase::ShiftLayoutY;
  return MuonLayout( xg, yg );
}

inline unsigned int LHCb::MuonTileID::nX() const { return ( m_muonid & MuonBase::MaskX ) >> MuonBase::ShiftX; }

inline unsigned int LHCb::MuonTileID::nY() const { return ( m_muonid & MuonBase::MaskY ) >> MuonBase::ShiftY; }

inline unsigned int LHCb::MuonTileID::index() const {
  return ( m_muonid & MuonBase::MaskIndex ) >> MuonBase::ShiftIndex;
}

inline unsigned int LHCb::MuonTileID::key() const { return ( m_muonid & MuonBase::MaskKey ) >> MuonBase::ShiftKey; }

inline bool LHCb::MuonTileID::operator<( const MuonTileID& id ) const { return key() < id.key(); }

inline bool LHCb::MuonTileID::operator==( const MuonTileID& id ) const { return m_muonid == id.m_muonid; }

inline bool LHCb::MuonTileID::operator!=( const MuonTileID& id ) const { return !( id == *this ); }

inline bool LHCb::MuonTileID::isDefined() const { return m_muonid != 0; }

inline void LHCb::MuonTileID::setStation( const unsigned int station ) {
  set( station, MuonBase::ShiftStation, MuonBase::MaskStation );
}

inline void LHCb::MuonTileID::setRegion( const unsigned int region ) {
  set( region, MuonBase::ShiftRegion, MuonBase::MaskRegion );
}

inline void LHCb::MuonTileID::setQuarter( const unsigned int quarter ) {
  set( quarter, MuonBase::ShiftQuarter, MuonBase::MaskQuarter );
}

inline void LHCb::MuonTileID::setX( const unsigned int x ) { set( x, MuonBase::ShiftX, MuonBase::MaskX ); }

inline void LHCb::MuonTileID::setY( const unsigned int y ) { set( y, MuonBase::ShiftY, MuonBase::MaskY ); }

inline void LHCb::MuonTileID::setLayout( const MuonLayout& layout ) {

  unsigned int lx, ly;
  lx = layout.xGrid();
  ly = layout.yGrid();
  set( lx, MuonBase::ShiftLayoutX, MuonBase::MaskLayoutX );
  set( ly, MuonBase::ShiftLayoutY, MuonBase::MaskLayoutY );
}

inline void LHCb::MuonTileID::deltaX( int dx ) { setX( nX() + dx ); }

inline void LHCb::MuonTileID::deltaY( int dy ) { setY( nY() + dy ); }

inline void LHCb::MuonTileID::set( const unsigned int Value, const unsigned int Shift, const unsigned int Mask ) {

  unsigned int tmp1, tmp2;
  tmp1     = ( Value << Shift ) & Mask;
  tmp2     = m_muonid & ~Mask;
  m_muonid = ( tmp1 | tmp2 );
}
