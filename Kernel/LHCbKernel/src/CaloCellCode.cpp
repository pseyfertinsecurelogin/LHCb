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
// Include files
// ============================================================================
#include <type_traits>
// ============================================================================
// LHCbKernel
// ============================================================================
#include "Kernel/CaloCellCode.h"
#include "Kernel/CaloCellID.h"
// ============================================================================
/** @file
 *  trivial assertion for CaloCellCode::ContentType size
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2009-09-29
 */
// ============================================================================
namespace CaloCellCode {

  // ==========================================================================
  /** @enum CaloArea
   *  The actual enumeration for the calorimeter areas
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-09-28
   */
  enum CaloAreas {
    Outer = 0,
    Middle, // NB: Inner for Hcal
    Inner,
    /// total number of Calorimeter areas
    CaloAreaNums = 4 // total number of Calorimeter areas
  };
  // ==========================================================================
} // namespace CaloCellCode
// ============================================================================
namespace {
  // ==========================================================================
  /** @var AreaName
   *  The array of calorimeter area names
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-09-28
   */
  static const std::array<std::string, CaloCellCode::CaloAreaNums> s_AreaName = {
      {"Outer", "Middle", "Inner", "PinArea"}};
  // ==========================================================================
  /// get the area index from name
  inline int _area( const std::string& area ) {
    auto it = std::find( s_AreaName.begin(), s_AreaName.end(), area );
    return s_AreaName.end() == it ? -1 : ( it - s_AreaName.begin() );
  }
  // ==========================================================================
} // namespace
// ============================================================================

namespace {
  // ==========================================================================
  typedef CaloCellCode::ContentType     _Type1;
  typedef LHCb::CaloCellID::ContentType _Type2;
  // ==========================================================================
} // namespace

static_assert( std::numeric_limits<_Type1>::is_specialized && std::is_integral<_Type1>::value &&
                   std::numeric_limits<_Type1>::is_integer && std::numeric_limits<_Type1>::digits == 32u &&
                   !std::is_signed<_Type1>::value,
               "_Type1 not valid" );

static_assert( std::numeric_limits<_Type2>::is_specialized && std::is_integral<_Type2>::value &&
                   std::numeric_limits<_Type1>::is_integer && std::numeric_limits<_Type2>::digits == 32u &&
                   !std::is_signed<_Type1>::value,
               "_Type2 not valid" );

static_assert( CaloCellCode::BitsTotal == 32, "32 bits" );

static_assert( CaloCellCode::SpdCalo < CaloCellCode::CaloNums && CaloCellCode::PrsCalo < CaloCellCode::CaloNums &&
                   CaloCellCode::EcalCalo < CaloCellCode::CaloNums && CaloCellCode::HcalCalo < CaloCellCode::CaloNums,
               "Det < CaloNums" );

static_assert( CaloCellCode::Outer < CaloCellCode::CaloAreaNums && CaloCellCode::Middle < CaloCellCode::CaloAreaNums &&
                   CaloCellCode::Inner < CaloCellCode::CaloAreaNums,
               "Region < AreaNums" );

static_assert( (unsigned int)CaloCellCode::PinArea < (unsigned int)CaloCellCode::CaloAreaNums,
               "PinArea < CaloAreaNums" );

namespace CaloCellCode {
  // ============================================================================
  /* get the area name from calorimeter index and number
   * @attention function make heavy use of hadcoded structure of Calorimeter!
   * @warning   different convention for Hcal
   * @param  calo (INPUT) calorimeter index
   * @param  area (INPUT) area index
   * @return name for the area
   * @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   * @date 2009-09-28
   */
  // ============================================================================
  const std::string& caloArea( const int calo, const int area ) {
    switch ( calo ) {
    case SpdCalo:
    case PrsCalo:
      switch ( area ) {
      case 0:
        return s_AreaName[Outer]; // Outer
      case 1:
        return s_AreaName[Middle]; // Middle
      case 2:
        return s_AreaName[Inner]; // Inner
      default:
        return s_BadName;
      }
    case EcalCalo:
      switch ( area ) {
      case 0:
        return s_AreaName[Outer]; // Outer
      case 1:
        return s_AreaName[Middle]; // Middle
      case 2:
        return s_AreaName[Inner]; // Inner
      case 3:
        return s_AreaName[PinArea]; // Pin-Area
      default:
        return s_BadName;
      }
    case HcalCalo:
      switch ( area ) {
      case 0:
        return s_AreaName[Outer]; // Outer
      case 1:
        return s_AreaName[Inner]; // Middle, return as Inner ! ATTENTION!
      case 3:
        return s_AreaName[PinArea]; // Pin-Area
      default:
        return s_BadName;
      }
    default:
      return s_BadName;
    }
  }
  // ============================================================================
  /*  get the area index from calorimeter index and name
   *  @attention function make heavy use of hadcoded structure of Calorimeter!
   *  @warning   the different convention for Hcal
   *  @param  calo (INPUT) calorimeter index
   *  @param  area (INPUT) area name
   *  @return indx for the area
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-09-28
   */
  // ============================================================================
  int caloArea( const int calo, const std::string& area ) {
    //
    switch ( calo ) {
    case HcalCalo:
      switch ( _area( area ) ) {
      case 0:
        return Outer; // 0
      case 1:
        return -1; // ATTENTION!!!
      case 2:
        return Middle; // 1        // ATTENTION!!!
      case 3:
        return PinArea; // 3
      default:
        return -1;
      }
    case EcalCalo:
      return _area( area );
    case SpdCalo:
    case PrsCalo:
      switch ( _area( area ) ) {
      case 0:
        return Outer; // 0
      case 1:
        return Middle; // 1
      case 2:
        return Inner; // 2
      default:
        return -1;
      }
    default:
      return -1;
    }
  }
  // ============================================================================
  /*  Is the given area is Pin-diod area ?
   *  @attention It must be coherent with CaloCellCode::caloArea
   *  @see CaloCellCode::caloArea
   *  @param calo (INPUT) calorimeetr index
   *  @param area (INPUT) calorimeter index
   *  @reutrn true if the area is Pin-diod area
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-09-28
   */
  // ============================================================================
  bool isPinArea( CaloIndex calo, const int area ) {
    return (int)PinArea == area && ( CaloIndex::EcalCalo == calo || CaloIndex::HcalCalo == calo );
  }
  std::string toString( const CaloIndex& i ) { return caloName( i ); }
  StatusCode  parse( CaloIndex& i, const std::string& s ) {
    i = caloNum( s );
    return i == static_cast<CaloIndex>( -1 ) ? StatusCode::FAILURE : StatusCode::SUCCESS;
  }

} // namespace CaloCellCode
// ============================================================================
// The END
// ============================================================================
