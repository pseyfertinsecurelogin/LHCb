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
#include "GaudiKernel/Kernel.h"
#include "Kernel/CaloCellCode.h"
#include <ostream>
#include <set>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class CaloCellID CaloCellID.h
   *
   * Simple class to represent cell identifier for calorimeter devices * * * The
   * class represents the unique cell identifier for all calorimeter devices * *
   *
   * @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *
   */

  class CaloCellID final {
  public:
    /// Representation type (presumably 32bits)
    typedef CaloCellCode::ContentType ContentType;
    /// vector of cell IDs
    typedef std::vector<LHCb::CaloCellID> Vector;
    /// the ordered set of unique of cell IDs
    typedef std::set<LHCb::CaloCellID> Set;

    /// Default/empty constructor
    CaloCellID() : m_all( 0 ) {}

    /// Explicit constructor from Calo, Area, Row and Column
    CaloCellID( const CaloCellCode::CaloIndex& Calo, unsigned int Area, unsigned int Row, unsigned int Column );

    /// Explicit constructor from Calo, Area, Row and Column
    CaloCellID( const CaloCellCode::CaloIndex& Calo, const std::string& Area, unsigned int Row, unsigned int Column );

    /// Explicit constructor from 32 bit representation
    explicit CaloCellID( const ContentType& all );

    /// Decoding function to extract index
    unsigned int index() const;

    /// Decoding function to extract calorimeter identifier
    CaloCellCode::CaloIndex calo() const;

    /// Decoding function to extract calorimeter area identifier
    unsigned int area() const;

    /// Decoding function to extract calorimeter row identifier
    unsigned int row() const;

    /// Decoding function to extract calorimeter column identifier
    unsigned int col() const;

    /// Comparison operator using 'index()' representation
    bool operator<( const CaloCellID& ID ) const;

    /// Equality operator using 'contents()' representation
    bool operator==( const CaloCellID& ID ) const;

    /// return true if the cellID corresponds to a PIN-diode readout channel
    bool isPin() const;

    /// printOut to standard STD/STL output streams
    std::ostream& fillStream( std::ostream& os ) const;

    /// bit pattern as string
    std::string bits( char del ) const;

    /// bit pattern as string
    std::string bits() const;

    /// Update Calorimeter identifier
    CaloCellID& setCalo( CaloCellCode::CaloIndex Calo );

    /// hash-function for python
    std::size_t __hash__() const;

    /// hash-function
    std::size_t hash() const;

    /// convert to string representation
    std::string toString() const;

    /// get the name of Calorimeter
    const std::string& caloName() const;

    /// get the name of Calorimeteter detector area
    const std::string& areaName() const;

    /// Retrieve const  all significant bits representation of CellID (32bits)
    const ContentType& all() const;

    /// Update  all significant bits representation of CellID (32bits)
    void setAll( const ContentType& value );

    friend std::ostream& operator<<( std::ostream& str, const CaloCellID& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Update Calorimeter Area identifier
    CaloCellID& setArea( unsigned int Area );

    /// Update Calorimeter Area identifier
    CaloCellID& setArea( unsigned int Calo, const std::string& Area );

    /// Update Calorimeter Row identifier
    CaloCellID& setRow( unsigned int Row );

    /// Update Calorimeter Column identifier
    CaloCellID& setCol( unsigned int Col );

    /// Update Index field
    CaloCellID& setIndex( unsigned int Index );

    /// set the active field
    CaloCellID& set( const ContentType& Value, unsigned int Shift, unsigned int Mask );

    /// Extract 'the rest' - must be 0!
    ContentType rest() const;

    /// Extract the full content
    ContentType contents() const;

    ContentType m_all; ///< all significant bits representation of CellID (32bits)

  }; // class CaloCellID

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::CaloCellID::CaloCellID( const CaloCellCode::CaloIndex& Calo, unsigned int Area, unsigned int Row,
                                     unsigned int Column )
    : m_all( 0 ) {

  setCalo( Calo );
  setArea( Area );
  setRow( Row );
  setCol( Column );
}

inline LHCb::CaloCellID::CaloCellID( const CaloCellCode::CaloIndex& Calo, const std::string& Area, unsigned int Row,
                                     unsigned int Column )
    : m_all( 0 ) {

  setCalo( Calo );
  setArea( calo(), Area );
  setRow( Row );
  setCol( Column );
}

inline LHCb::CaloCellID::CaloCellID( const ContentType& all ) : m_all( 0 ) { m_all = all; }

inline const LHCb::CaloCellID::ContentType& LHCb::CaloCellID::all() const { return m_all; }

inline void LHCb::CaloCellID::setAll( const ContentType& value ) { m_all = value; }

inline unsigned int LHCb::CaloCellID::index() const {

  return ( contents() & CaloCellCode::MaskIndex ) >> CaloCellCode::ShiftIndex;
}

inline CaloCellCode::CaloIndex LHCb::CaloCellID::calo() const {

  return static_cast<CaloCellCode::CaloIndex>( ( contents() & CaloCellCode::MaskCalo ) >> CaloCellCode::ShiftCalo );
}

inline unsigned int LHCb::CaloCellID::area() const {

  return ( contents() & CaloCellCode::MaskArea ) >> CaloCellCode::ShiftArea;
}

inline unsigned int LHCb::CaloCellID::row() const {

  return ( contents() & CaloCellCode::MaskRow ) >> CaloCellCode::ShiftRow;
}

inline unsigned int LHCb::CaloCellID::col() const {

  return ( contents() & CaloCellCode::MaskCol ) >> CaloCellCode::ShiftCol;
}

inline bool LHCb::CaloCellID::operator<( const CaloCellID& ID ) const { return index() < ID.index(); }

inline bool LHCb::CaloCellID::operator==( const CaloCellID& ID ) const { return contents() == ID.contents(); }

inline bool LHCb::CaloCellID::isPin() const { return CaloCellCode::isPinArea( calo(), area() ); }

inline std::string LHCb::CaloCellID::bits() const { return bits( ',' ); }

inline LHCb::CaloCellID& LHCb::CaloCellID::setCalo( CaloCellCode::CaloIndex Calo ) {

  return set( Calo, CaloCellCode::ShiftCalo, CaloCellCode::MaskCalo );
}

inline std::size_t LHCb::CaloCellID::__hash__() const { return hash(); }

inline std::size_t LHCb::CaloCellID::hash() const { return index(); }

inline const std::string& LHCb::CaloCellID::caloName() const { return CaloCellCode::caloName( calo() ); }

inline const std::string& LHCb::CaloCellID::areaName() const { return CaloCellCode::caloArea( calo(), area() ); }

inline LHCb::CaloCellID& LHCb::CaloCellID::setArea( unsigned int Area ) {

  return set( Area, CaloCellCode::ShiftArea, CaloCellCode::MaskArea );
}

inline LHCb::CaloCellID& LHCb::CaloCellID::setRow( unsigned int Row ) {

  return set( Row, CaloCellCode::ShiftRow, CaloCellCode::MaskRow );
}

inline LHCb::CaloCellID& LHCb::CaloCellID::setCol( unsigned int Col ) {

  return set( Col, CaloCellCode::ShiftCol, CaloCellCode::MaskCol );
}

inline LHCb::CaloCellID& LHCb::CaloCellID::setIndex( unsigned int Index ) {

  return set( Index, CaloCellCode::ShiftIndex, CaloCellCode::MaskIndex );
}

inline LHCb::CaloCellID& LHCb::CaloCellID::set( const ContentType& Value, unsigned int Shift, unsigned int Mask ) {

  m_all = CaloCellCode::setField( contents(), Value, Shift, Mask );
  return *this;
}

inline LHCb::CaloCellID::ContentType LHCb::CaloCellID::rest() const {

  return ( contents() & CaloCellCode::MaskRest ) >> CaloCellCode::ShiftRest;
}

inline LHCb::CaloCellID::ContentType LHCb::CaloCellID::contents() const { return m_all; }
