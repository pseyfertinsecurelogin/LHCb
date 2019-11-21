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
/// ===========================================================================
#include "Kernel/CaloCellID.h"
#include <vector>

// Include files
class DeCalorimeter;

/** @class  CardParam CardParam.h CaloDet/CardParam.h
 *
 *
 */

class CardParam final {

  friend class DeCalorimeter;

public:
  enum Mapping { None = 0, Prs64Top = 1, Prs64Bottom = 2, Prs32Top = 3, Prs32Bottom = 4 };

  /** constructor
      @param iArea  calorimeter area
      @param iRow   row
      @param iCol   column
      @param ilRow
      @param ilCol
      @param iNum
      @param iCrate
      @param iSlot
  */
  CardParam( int iArea = 0, int iRow = 0, int iCol = 0, int ilRow = 0, int ilCol = 0, int iNum = 0, int iCrate = 0,
             int iSlot = 0 );

  [[nodiscard]] int     downNumber() const { return m_DownNumber; }
  [[nodiscard]] int     leftNumber() const { return m_LeftNumber; }
  [[nodiscard]] int     cornerNumber() const { return m_CornerNumber; }
  [[nodiscard]] int     previousNumber() const { return m_PreviousNumber; }
  [[nodiscard]] int     validationNumber() const { return m_validationNumber; }
  [[nodiscard]] int     area() const { return m_Area; }
  [[nodiscard]] int     firstRow() const { return m_Row; }
  [[nodiscard]] int     firstColumn() const { return m_Col; }
  [[nodiscard]] int     lastRow() const { return m_lRow; }
  [[nodiscard]] int     lastColumn() const { return m_lCol; }
  [[nodiscard]] bool    isPinCard() const { return m_isPin; }
  [[nodiscard]] int     tell1() const { return m_tell1; }
  [[nodiscard]] int     number() const { return m_number; }
  [[nodiscard]] int     crate() const { return m_crate; }
  [[nodiscard]] int     slot() const { return m_slot; }
  [[nodiscard]] int     code() const { return 16 * m_crate + m_slot; }
  [[nodiscard]] int     selectionType() const { return m_selectionType; }
  [[nodiscard]] Mapping mapping() const { return m_mapping; }
  [[nodiscard]] bool    isParasitic() const {
    return ( ( m_Row + m_lRow + m_Col + m_lCol ) == 0 );
  } // special FEBs (no channelID associated)
  [[nodiscard]] bool isPmtCard() const { return ( !isPinCard() && !isParasitic() ); }

  [[nodiscard]] const std::vector<LHCb::CaloCellID>& ids() const { return m_ids; }
  void                                               addID( LHCb::CaloCellID id ) { m_ids.push_back( id ); }
  void                                               setTell1( int tell1 ) { m_tell1 = tell1; }
  void                                               setValidationNumber( int num ) { m_validationNumber = num; }
  void setNeighboringCards( const int down, const int left, const int corner, const int previous ) {
    m_DownNumber     = down;
    m_LeftNumber     = left;
    m_CornerNumber   = corner;
    m_PreviousNumber = previous;
  }
  void setIsPin( bool pin ) { m_isPin = pin; }
  void setSelectionType( int num ) { m_selectionType = num; }
  void setMapping( Mapping typ ) { m_mapping = typ; }

private:
  int                           m_Area;
  int                           m_Row;
  int                           m_Col;
  int                           m_lRow;
  int                           m_lCol;
  int                           m_DownNumber       = -1;
  int                           m_LeftNumber       = -1;
  int                           m_CornerNumber     = -1;
  int                           m_PreviousNumber   = -1;
  int                           m_validationNumber = -1;
  bool                          m_isPin            = false;
  int                           m_number;
  std::vector<LHCb::CaloCellID> m_ids;
  int                           m_crate;
  int                           m_slot;
  int                           m_tell1 = -1;
  int     m_selectionType = -1; // [e,g,pi0L,pi0G] = -1 ; [hadronMaster] = 0 ; [hadronSlave1]=1 ; [hadronSlave2]=2
  Mapping m_mapping       = CardParam::None; // see CardParam::Mapping  enum
};

/// ===========================================================================
