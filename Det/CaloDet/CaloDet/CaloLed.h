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
#pragma once

// Include files
#include "Kernel/CaloCellCode.h"
#include "Kernel/CaloCellID.h"
#include <sstream>

/** @class  CaloLed CaloLed.h CaloDet/CaloLed.h
 *
 *
 */

class CaloLed final {

public:
  /** constructors
   */
  CaloLed() = default;
  CaloLed( int id ) : m_id( id ) {}

  // getters
  [[nodiscard]] int                                  number() const { return m_id; }
  [[nodiscard]] const std::vector<LHCb::CaloCellID>& cells() const { return m_cells; }
  [[nodiscard]] LHCb::CaloCellID                     pin() const { return m_pin; }

  [[nodiscard]] std::vector<int> firstRows() const { return m_fRow; }
  [[nodiscard]] std::vector<int> lastRows() const { return m_lRow; }
  [[nodiscard]] std::vector<int> firstColumns() const { return m_fCol; }
  [[nodiscard]] std::vector<int> lastColumns() const { return m_lCol; }
  [[nodiscard]] std::vector<int> areas() const { return m_area; }
  [[nodiscard]] int              firstRow() const { return *( m_fRow.begin() ); }
  [[nodiscard]] int              lastRow() const { return *( m_lRow.begin() ); }
  [[nodiscard]] int              firstColumn() const { return *( m_fCol.begin() ); }
  [[nodiscard]] int              lastColumn() const { return *( m_lCol.begin() ); }
  [[nodiscard]] int              area() const { return *( m_area.begin() ); }
  [[nodiscard]] int              index() const { return m_index; }

  // setters
  void setNumber( int number ) { m_id = number; }
  void addCell( const LHCb::CaloCellID cell ) { m_cells.push_back( cell ); }
  void setPin( const LHCb::CaloCellID pin ) { m_pin = pin; }
  void setIndex( const int index ) { m_index = index; }
  void addCaloRegion( int area, int fCol, int fRow, int lCol, int lRow ) {
    m_area.push_back( area );
    m_fRow.push_back( fRow );
    m_fCol.push_back( fCol );
    m_lCol.push_back( lCol );
    m_lRow.push_back( lRow );
    std::stringstream s( " " );
    s << m_region << " += [" << area << ", " << fCol << "," << fRow << "," << lCol << "," << lRow << "] ";
    m_region = s.str();
  }
  friend std::ostream& operator<<( std::ostream& str, CaloLed obj ) { return obj.fillStream( str ); }
  std::ostream&        fillStream( std::ostream& str ) {
    return str << " Led : " << number() << " index " << index() << " : " << m_region << " readout by  PINId" << pin();
  }

private:
  int                           m_id{-1};
  std::string                   m_region = "Region ";
  LHCb::CaloCellID              m_pin    = {};
  std::vector<int>              m_area;
  std::vector<int>              m_fCol;
  std::vector<int>              m_fRow;
  std::vector<int>              m_lCol;
  std::vector<int>              m_lRow;
  int                           m_index = -1;
  std::vector<LHCb::CaloCellID> m_cells;
};
