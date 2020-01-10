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
// ============================================================================
#ifndef KERNEL_CALOCELLCODE_H
#define KERNEL_CALOCELLCODE_H 1
// ============================================================================
// Include files
// ============================================================================
// STD and STL
// ============================================================================
#include <algorithm>
#include <array>
#include <iomanip>
#include <set>
#include <string>
#include <vector>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/StatusCode.h"
// ============================================================================
// Boost
// ============================================================================
#ifdef __INTEL_COMPILER            // Disable ICC warning from Boost
#  pragma warning( disable : 279 ) // controlling expression is constant
#endif
// ============================================================================
/** @class CaloCellCode CaloCellCode.h Kernel/CaloCellCode.h
 *
 *  Namespace for all code/decode rules of CaloCellID class
 *
 *  @todo CaloCellCode.h: switch from C-string to std::string
 *  @todo CaloCellCode.h: make code more robust & CPU-efficient
 *  @todo CaloCellCode.h: remove many redundant & confusing "static"
 *  @todo CaloCellCode.h: return by reference
 *
 *  @author  Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date    19/02/2001
 */
// ============================================================================
/** @namespace CaloCellCode
 *
 *  Namespace for all code/decode rules of CaloCellID class
 *
 *  @author  Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date    19/02/2001
 */
namespace CaloCellCode {
  // ==========================================================================
  /** @var s_BadName
   *  representation of "bad-name"
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-09-28
   */
  inline const std::string s_BadName = "????";
  // ==========================================================================
  /** @typedef ContentType
   *  the actual type for 32 bits representation of internal data
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-09-28
   */
  using ContentType = unsigned int;
  // ==========================================================================
  // Coding of CaloCellID objects : Lengths, basic
  // ==========================================================================
  const unsigned int BitsCol  = 6;
  const unsigned int BitsRow  = 6;
  const unsigned int BitsArea = 2;
  const unsigned int BitsCalo = 2;
  // ==========================================================================
  // Coding of CaloCellID objects : Lengths, extra
  // ==========================================================================
  const unsigned int BitsIndex = BitsCol + BitsRow + BitsArea;
  const unsigned int BitsAll   = BitsCol + BitsRow + BitsArea + BitsCalo;
  const unsigned int BitsTotal = 32;
  const unsigned int BitsRest  = BitsTotal - BitsAll;
  // ==========================================================================
  // Coding of CaloCellID objects : Shifts, basic
  // ==========================================================================
  const unsigned int ShiftCol  = 0;
  const unsigned int ShiftRow  = ShiftCol + BitsCol;
  const unsigned int ShiftArea = ShiftRow + BitsRow;
  const unsigned int ShiftCalo = ShiftArea + BitsArea;
  // ==========================================================================
  // Coding of CaloCellID objects : Shifts, extra
  // ==========================================================================
  const unsigned int ShiftIndex = ShiftCol;
  const unsigned int ShiftAll   = ShiftCol;
  const unsigned int ShiftRest  = ShiftCalo + BitsCalo;
  // ==========================================================================
  // Coding of CaloCellID objects : Masks, basic
  // ==========================================================================
  const ContentType MaskCol  = ( ( ( (ContentType)1 ) << BitsCol ) - 1 ) << ShiftCol;
  const ContentType MaskRow  = ( ( ( (ContentType)1 ) << BitsRow ) - 1 ) << ShiftRow;
  const ContentType MaskArea = ( ( ( (ContentType)1 ) << BitsArea ) - 1 ) << ShiftArea;
  const ContentType MaskCalo = ( ( ( (ContentType)1 ) << BitsCalo ) - 1 ) << ShiftCalo;
  // ==========================================================================
  // Coding of CaloCellID objects : Masks, extra
  // ==========================================================================
  const ContentType MaskIndex = ( ( ( (ContentType)1 ) << BitsIndex ) - 1 ) << ShiftIndex;
  const ContentType MaskAll   = ( ( ( (ContentType)1 ) << BitsAll ) - 1 ) << ShiftAll;
  const ContentType MaskRest  = ( ( ( (ContentType)1 ) << BitsRest ) - 1 ) << ShiftRest;
  // ==========================================================================
  /** set the active fields
   *  @param Where (INPUT) the initial value
   *  @param Value (INPUT) value to be set
   *  @param Shift (INPUT) shift
   *  @param Mask  (INPUT) mask
   *  @return new Field
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-09-28
   */
  inline ContentType setField( const ContentType Field, const ContentType Value, const ContentType Shift,
                               const ContentType Mask ) {
    ContentType tmp1 = ( Value << Shift ) & Mask;
    ContentType tmp2 = Field & ~Mask;
    return tmp1 | tmp2;
  }
  // ==========================================================================
  /** @enum CaloIndex
   *  the indices for  Calorimeter Detectors (to extract the coding of the "calo" )
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-09-28
   */
  enum CaloIndex {
    Undefined = -1,
    SpdCalo   = 0,
    PrsCalo,
    EcalCalo,
    HcalCalo,
    /// total number of NAMED Calorimeters
    CaloNums = 4 // total number of NAMED Calorimeters
  };
  std::string          toString( const CaloIndex& );
  StatusCode           parse( CaloIndex&, const std::string& );
  inline std::ostream& toStream( const CaloIndex& ci, std::ostream& os ) {
    return os << std::quoted( toString( ci ), '\'' );
  }
  // ==========================================================================
  /** @var CaloNames
   *  The actual list of Calorimter names
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-09-28
   */
  inline const std::array<std::string, CaloNums> CaloNames = {{"Spd", "Prs", "Ecal", "Hcal"}};
  // ==========================================================================
  /// the calorimeter names:
  inline const std::string_view SpdName  = {CaloNames[SpdCalo]};  //  Spd
  inline const std::string_view PrsName  = {CaloNames[PrsCalo]};  //  Prs
  inline const std::string_view EcalName = {CaloNames[EcalCalo]}; // Ecal
  inline const std::string_view HcalName = {CaloNames[HcalCalo]}; // Hcal
  // ==========================================================================
  /** simple function to get the calorimeter name from number
   *  @param num (INPUT) calorimeter index
   *  @return calorimeter name
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-09-28
   */
  inline const std::string& caloName( CaloIndex num ) {
    return num < 0 ? s_BadName : num < CaloNums ? CaloNames[num] : s_BadName;
  }
  // ==========================================================================
  /** get the calorimeter index from name, returns -1 for wrong name!
   *  @param name (INPUT) the calorimeter name (can be long string)
   *  @return calorimeter index from name, returns -1 for wrong name!
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-09-28
   */
  inline CaloIndex caloNum( std::string_view name ) {
    auto begin = CaloNames.begin();
    auto end   = CaloNames.end();
    auto m     = std::find_if( begin, end, [=]( const auto& i ) { return name.find( i ) != std::string_view::npos; } );
    return CaloIndex( m != end ? CaloCellCode::CaloIndex( m - begin ) : CaloCellCode::CaloIndex::Undefined );
  }
  // =========================================================================
  /** get the calorimeter index from name, returns -1 for wrong name!
   *  @paran name (INPUT) the calorimeter name (can be long string)
   *  @return calorimeter index from name, returns -1 for wrong name!
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-09-28
   */
  inline CaloIndex CaloNumFromName( std::string_view name ) { return caloNum( name ); }
  // ==========================================================================
  /** get the area name from calorimeter index and number
   *  @attention function make heavy use of hadcoded structure of Calorimeter!
   *  @warning   the different convention for Hcal
   *  @param  calo (INPUT) calorimeter index
   *  @param  area (INPUT) area index
   *  @return name for the area
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-09-28
   */
  GAUDI_API
  const std::string& caloArea( const int calo, const int area );
  // =========================================================================
  /** get the area index from calorimeter index and name
   *  @attention function make heavy use of hadcoded structure of Calorimeter!
   *  @warning   the different convention for Hcal
   *  @param  calo (INPUT) calorimeter index
   *  @param  area (INPUT) area name
   *  @return indx for the area
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-09-28
   */
  GAUDI_API
  int caloArea( const int calo, const std::string& area );
  // ==========================================================================
  /** Is the given area is Pin-diod area ?
   *  @attention It must be coherent with CaloCellCode::caloArea
   *  @see CaloCellCode::caloArea
   *  @param calo (INPUT) calorimeetr index
   *  @param area (INPUT) calorimeter index
   *  @reutrn true if the area is Pin-diod area
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-09-28
   */
  GAUDI_API
  bool isPinArea( CaloIndex calo, const int area );
  // ==========================================================================
  /** @enum CaloArea
   *  The actual enumeration for the calorimeter areas
   *  @attention some code lines explicitly rely on the actual value of PinArea,
   *             therefore this symbol is the pubilc one.
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-09-28
   */
  enum CaloArea {
    // ======================================================================
    /// some code lines explicitly rely on the alue of PinArea
    PinArea = 3 // some code lines explicitly rely on this value
                // ======================================================================
  };
  // ==========================================================================
} // namespace CaloCellCode

// ============================================================================
// The End
// ============================================================================
#endif //   CALOKERNEL_CALOCELLCODE_H
