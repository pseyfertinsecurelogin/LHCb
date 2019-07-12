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

// STL
#include <algorithm>
#include <array>
#include <cstdint>
#include <string>
#include <vector>

// Gaudi
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// DetDesc
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/ISolid.h"

// RichDet
#include "RichDet/DeRich.h"
#include "RichDet/DeRichLocations.h"
#include "RichDet/DeRichPDPanel.h"
#include "RichDet/DeRichPMTClassic.h"

/** @class DeRichPMTPanelClassic DeRichPMTPanelClassic.h RichDet/DeRichPMTPanelClassic.h
 *
 *  Detector element for PMT panels
 *
 *  @author Sajan Easo
 *  @date   2011-10-10
 */
class DeRichPMTPanelClassic : public DeRichPDPanel {

public:
  /// Standard constructor
  DeRichPMTPanelClassic( const std::string& name = "" );

  /// Destructor
  virtual ~DeRichPMTPanelClassic() = default;

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  const CLID& clID() const override final { return classID(); }

  /// return the class ID
  static const CLID& classID();

  /**
   * This is where most of the geometry is read and variables initialised
   *
   * @return Status of initialisation
   * @retval StatusCode::FAILURE Initialisation failed, program should
   * terminate
   */
  StatusCode initialize() override final;

  // Access the DeRichPD object for a given PD RichSmartID
  const DeRichPD* dePD( const LHCb::RichSmartID pdID ) const override final;

  // Returns the detector element for the given PD number
  const DeRichPD* dePD( const Rich::DAQ::PDPanelIndex PDNumber ) const override final;

  // Converts a Gaudi::XYZPoint in global coordinates to a RichSmartID.
  bool smartID( const Gaudi::XYZPoint& globalPoint, LHCb::RichSmartID& id ) const override final;

  // Returns the intersection point with the detector plane given a vector and a point.
  LHCb::RichTraceMode::RayTraceResult detPlanePoint( const Gaudi::XYZPoint&    pGlobal,     //
                                                     const Gaudi::XYZVector&   vGlobal,     //
                                                     Gaudi::XYZPoint&          hitPosition, //
                                                     LHCb::RichSmartID&        smartID,     //
                                                     const DeRichPD*&          pd,          //
                                                     const LHCb::RichTraceMode mode         //
                                                     ) const override final;

  // Returns the intersection point with an HPD window given a vector and a point.
  LHCb::RichTraceMode::RayTraceResult PDWindowPoint( const Gaudi::XYZPoint&    pGlobal,           //
                                                     const Gaudi::XYZVector&   vGlobal,           //
                                                     Gaudi::XYZPoint&          windowPointGlobal, //
                                                     LHCb::RichSmartID&        smartID,           //
                                                     const DeRichPD*&          pd,                //
                                                     const LHCb::RichTraceMode mode               //
                                                     ) const override final;

  // Returns the SIMD intersection point with an HPD window given a vector and a point.
  SIMDRayTResult::Results PDWindowPointSIMD( const SIMDPoint&          pGlobal,     //
                                             const SIMDVector&         vGlobal,     //
                                             SIMDPoint&                hitPosition, //
                                             SIMDRayTResult::SmartIDs& smartID,     //
                                             SIMDRayTResult::PDs&      PDs,         //
                                             const LHCb::RichTraceMode mode         //
                                             ) const override final;

  // Returns the SIMD intersection point with the detector plane given a vector and a point.
  SIMDRayTResult::Results detPlanePointSIMD( const SIMDPoint&          pGlobal,     //
                                             const SIMDVector&         vGlobal,     //
                                             SIMDPoint&                hitPosition, //
                                             SIMDRayTResult::SmartIDs& smartID,     //
                                             SIMDRayTResult::PDs&      PDs,         //
                                             const LHCb::RichTraceMode mode         //
                                             ) const override final;

  // Adds to the given vector all the available readout channels in this HPD panel
  bool readoutChannelList( LHCb::RichSmartID::Vector& readoutChannels ) const override final;

  /// Get tge sensitivevolumeID
  int sensitiveVolumeID( const Gaudi::XYZPoint& globalPoint ) const override final;

  /** Converts a RichSmartID to a point on the anode in global coordinates.
   *  @param[in] smartID The RichSmartID for the PMT channel
   *  @return The detection point on the anode in global coordinates
   */
  inline Gaudi::XYZPoint detPointOnAnode( const LHCb::RichSmartID smartID ) const {
    const auto* aPMT = dePMT( smartID );
    return ( aPMT ? aPMT->detPointOnAnode( smartID ) : Gaudi::XYZPoint( 0, 0, 0 ) );
  }

  /// Returns the PD number for the given RichSmartID
  Rich::DAQ::PDPanelIndex pdNumber( const LHCb::RichSmartID& smartID ) const override;

  // The maximum PD copy number for this panel
  Rich::DAQ::PDPanelIndex maxPdNumber() const override;

private:
  using Int        = std::int32_t;
  using IDeElemV   = std::vector<IDetectorElement*>;
  using IGeomInfoV = std::vector<const IGeometryInfo*>;
  using DRiPMTV    = std::vector<DeRichPMTClassic*>;
  // using ArraySetup     = std::array<Int,4>;
  using RowCol         = std::array<Int, 2>;
  using XYArray        = std::array<double, 2>;
  using XYArraySIMD    = std::array<SIMDFP, 2>;
  using ArraySetupSIMD = std::array<SIMDINT32, 4>;

private: // setup methods
  /// Update cached information on geometry changes
  StatusCode geometryUpdate();

  StatusCode getPanelGeometryInfo();

  void RichSetupMixedSizePmtModules();

  // Set the rich panel and side
  bool setRichAndSide();

private:
  /// Get the first RICH pointer
  const DetectorElement* getFirstDeRich() const;

  /// Returns the PD number for the given RichSmartID
  inline Rich::DAQ::PDPanelIndex _pdNumber( const LHCb::RichSmartID smartID ) const noexcept {
    // check for now. to be removed.
    if ( smartID.rich() != rich() || smartID.panel() != side() ) {
      error() << "_pdNumber RICH and side error " << smartID << endmsg;
    }
    // Should never get different rich or panel, so skip check
    // return Rich::DAQ::PDPanelIndex( smartID.rich() == rich() && smartID.panel() == side() ?
    //                                ( smartID.pdCol() * m_NumPmtInRichModule ) +
    //                                smartID.pdNumInCol() : nPDs() + 1 );
    return Rich::DAQ::PDPanelIndex( ( smartID.pdCol() * m_NumPmtInRichModule ) + smartID.pdNumInCol() );
  }

  const DeRichPMTClassic* dePMT( const Rich::DAQ::PDPanelIndex PmtNumber ) const;

  inline const DeRichPMTClassic* dePMT( const LHCb::RichSmartID pdID ) const {
    // get the lookup indices from the smart ID
    auto pdCol   = pdID.pdCol();
    auto pdInCol = pdID.pdNumInCol();

    // if need be correct the pdCol (for data when it was incorrectly filled)
    // this is temporary, can be removed when no longer needed...
    if ( UNLIKELY( pdCol >= m_DePMTs.size() ) ) {
      pdCol = PmtModuleNumInPanelFromModuleNumAlone( pdCol );
    } // return the pointer from the array
    return m_DePMTs[pdCol][pdInCol];

    // get the old way
    // return dePMT( _pdNumber( pdID ) );
  }

private:
  inline RowCol getPmtRowColFromPmtNum( const Int aPmtNum ) const noexcept {
    const Int aPRow = ( aPmtNum / m_NumPmtInRowCol[0] );
    return {aPRow, ( Int )( aPmtNum - ( aPRow * m_NumPmtInRowCol[0] ) )};
  }

  inline RowCol getGrandPmtRowColFromPmtNum( const Int aPmtNum ) const noexcept {
    const Int aPRow = ( aPmtNum / m_NumGrandPmtInRowCol[0] );
    return {aPRow, ( Int )( aPmtNum - ( aPRow * m_NumGrandPmtInRowCol[0] ) )};
  }

  inline Int PmtModuleNumInPanelFromModuleNum( const Int aMnum ) const noexcept {
    return aMnum - m_RichPmtModuleCopyNumBeginPanel[m_CurPanelNum];
  }

  inline Int PmtModuleNumInPanelFromModuleNumAlone( const Int aMnum ) const noexcept {
    return ( aMnum >= m_RichPmtModuleCopyNumBeginPanel[0] && aMnum <= m_RichPmtModuleCopyNumEndPanel[0]
                 ? aMnum - m_RichPmtModuleCopyNumBeginPanel[0]
                 : aMnum >= m_RichPmtModuleCopyNumBeginPanel[1] && aMnum <= m_RichPmtModuleCopyNumEndPanel[1]
                       ? aMnum - m_RichPmtModuleCopyNumBeginPanel[1]
                       : aMnum >= m_RichPmtModuleCopyNumBeginPanel[2] && aMnum <= m_RichPmtModuleCopyNumEndPanel[2]
                             ? aMnum - m_RichPmtModuleCopyNumBeginPanel[2]
                             : aMnum >= m_RichPmtModuleCopyNumBeginPanel[3] &&
                                       aMnum <= m_RichPmtModuleCopyNumEndPanel[3]
                                   ? aMnum - m_RichPmtModuleCopyNumBeginPanel[3]
                                   : -1 );
  }

  inline SIMDINT32 PmtModuleNumInPanelFromModuleNumAlone( const SIMDINT32& aMnum ) const noexcept {
    SIMDINT32 res = -SIMDINT32::One();
    res( aMnum >= m_RichPmtModuleCopyNumBeginPanelSIMD[0] && aMnum <= m_RichPmtModuleCopyNumEndPanelSIMD[0] ) =
        aMnum - m_RichPmtModuleCopyNumBeginPanelSIMD[0];
    res( aMnum >= m_RichPmtModuleCopyNumBeginPanelSIMD[1] && aMnum <= m_RichPmtModuleCopyNumEndPanelSIMD[1] ) =
        aMnum - m_RichPmtModuleCopyNumBeginPanelSIMD[1];
    res( aMnum >= m_RichPmtModuleCopyNumBeginPanelSIMD[2] && aMnum <= m_RichPmtModuleCopyNumEndPanelSIMD[2] ) =
        aMnum - m_RichPmtModuleCopyNumBeginPanelSIMD[2];
    res( aMnum >= m_RichPmtModuleCopyNumBeginPanelSIMD[3] && aMnum <= m_RichPmtModuleCopyNumEndPanelSIMD[3] ) =
        aMnum - m_RichPmtModuleCopyNumBeginPanelSIMD[3];
    return res;
  }

  inline RowCol PmtModuleRowColFromModuleNumInPanel( const Int aMnum ) const noexcept {
    RowCol rc   = {-1, -1};
    auto&  MRow = rc[0];
    auto&  MCol = rc[1];

    if ( Rich::Rich1 == rich() ) {
      MRow = ( Int )( aMnum / m_RichPmtNumModulesInRowCol[0] );
      MCol = aMnum - MRow * m_RichPmtNumModulesInRowCol[0];
    } else // if ( rich() == Rich::Rich2 || rich() == Rich::Rich )
    {
      MCol = ( Int )( aMnum / m_RichPmtNumModulesInRowCol[3] );
      MRow = aMnum - MCol * m_RichPmtNumModulesInRowCol[3];
    }

    return rc;
  }

  inline SIMDINT32 getPmtModuleNumFromRowCol( SIMDINT32 MRow, SIMDINT32 MCol ) const {
    // set the closest Row Col.
    // This means if the row col exceeds the edges set them to those at the closest edge.

    MRow.setZero( MRow < SIMDINT32::Zero() );
    MCol.setZero( MCol < SIMDINT32::Zero() );

    SIMDINT32 aMNum = m_RichPmtModuleCopyNumBeginPanelSIMD[m_CurPanelNum];

    if ( Rich::Rich1 == rich() ) {
      MRow( MRow >= m_RichPmtNumModulesInRowColSIMD[1] ) = m_RichPmtNumModulesInRowColSIMD[1] - SIMDINT32::One();
      MCol( MCol >= m_RichPmtNumModulesInRowColSIMD[0] ) = m_RichPmtNumModulesInRowColSIMD[0] - SIMDINT32::One();
      aMNum += MCol + ( MRow * m_RichPmtNumModulesInRowColSIMD[0] );
    } else // if ( rich() == Rich::Rich2 || rich() == Rich::Rich )
    {
      MRow( MRow >= m_RichPmtNumModulesInRowColSIMD[3] ) = m_RichPmtNumModulesInRowColSIMD[3] - SIMDINT32::One();
      MCol( MCol >= m_RichPmtNumModulesInRowColSIMD[2] ) = m_RichPmtNumModulesInRowColSIMD[2] - SIMDINT32::One();
      aMNum += MRow + ( MCol * m_RichPmtNumModulesInRowColSIMD[3] );
    }

    return aMNum;
  }

  /// setup flags for grand Modules
  Int getModuleCopyNumber( const std::string& aModuleName );

  ArraySetupSIMD findPMTArraySetupSIMD( const SIMDPoint& aLocalPoint ) const;

private:
  // Simple struct to store module numbers
  struct ModuleNumbersSIMD {
    SIMDINT32 aModuleCol        = -SIMDINT32::One();
    SIMDINT32 aModuleRow        = -SIMDINT32::One();
    SIMDINT32 aModuleNum        = -SIMDINT32::One();
    SIMDINT32 aModuleNumInPanel = -SIMDINT32::One();
  };

  /// Function pointer for the getModuleNums method to use depending on settings
  void ( DeRichPMTPanelClassic::*m_getModuleNumsSIMD )( const SIMDFP& x, const SIMDFP& y,
                                                        ModuleNumbersSIMD& nums ) const noexcept = nullptr;

  inline void getModuleNumsSIMD( const SIMDFP& x, const SIMDFP& y, ModuleNumbersSIMD& nums ) const noexcept {
    ( this->*m_getModuleNumsSIMD )( x, y, nums );
  }

  void getModuleNums_R1Up_NoLens_SIMD( const SIMDFP& x, const SIMDFP& y, ModuleNumbersSIMD& nums ) const noexcept {
    using namespace LHCb::SIMD;
    nums.aModuleCol = simd_cast<SIMDINT32>( abs( x - m_PmtModulePlaneHalfSizeR1SIMD[0] ) * m_PmtModulePitchInvSIMD );
    nums.aModuleRow = simd_cast<SIMDINT32>( abs( y - m_PmtModulePlaneHalfSizeR1SIMD[1] ) * m_PmtModulePitchInvSIMD );
    nums.aModuleNum = getPmtModuleNumFromRowCol( nums.aModuleRow, nums.aModuleCol );
    nums.aModuleNumInPanel = nums.aModuleNum - m_RichPmtModuleCopyNumBeginPanelSIMD[0];
  }

  void getModuleNums_R1Dn_NoLens_SIMD( const SIMDFP& x, const SIMDFP& y, ModuleNumbersSIMD& nums ) const noexcept {
    using namespace LHCb::SIMD;
    nums.aModuleCol = simd_cast<SIMDINT32>( abs( x - m_PmtModulePlaneHalfSizeR1SIMD[2] ) * m_PmtModulePitchInvSIMD );
    nums.aModuleRow = simd_cast<SIMDINT32>( abs( y - m_PmtModulePlaneHalfSizeR1SIMD[3] ) * m_PmtModulePitchInvSIMD );
    nums.aModuleNum = getPmtModuleNumFromRowCol( nums.aModuleRow, nums.aModuleCol );
    nums.aModuleNumInPanel = nums.aModuleNum - m_RichPmtModuleCopyNumBeginPanelSIMD[1];
  }

  void getModuleNums_R2Le_Small_SIMD( const SIMDFP& x, const SIMDFP& y, ModuleNumbersSIMD& nums ) const noexcept {
    using namespace LHCb::SIMD;
    nums.aModuleCol = simd_cast<SIMDINT32>( abs( x - m_PmtModulePlaneHalfSizeR2SIMD[0] ) * m_PmtModulePitchInvSIMD );
    nums.aModuleRow = simd_cast<SIMDINT32>( abs( y - m_PmtModulePlaneHalfSizeR2SIMD[1] ) * m_PmtModulePitchInvSIMD );
    nums.aModuleNum = getPmtModuleNumFromRowCol( nums.aModuleRow, nums.aModuleCol );
    nums.aModuleNumInPanel = nums.aModuleNum - m_RichPmtModuleCopyNumBeginPanelSIMD[2];
  }

  void getModuleNums_R2Ri_Small_SIMD( const SIMDFP& x, const SIMDFP& y, ModuleNumbersSIMD& nums ) const noexcept {
    using namespace LHCb::SIMD;
    nums.aModuleCol = simd_cast<SIMDINT32>( abs( x - m_PmtModulePlaneHalfSizeR2SIMD[2] ) * m_PmtModulePitchInvSIMD );
    nums.aModuleRow = simd_cast<SIMDINT32>( abs( y - m_PmtModulePlaneHalfSizeR2SIMD[3] ) * m_PmtModulePitchInvSIMD );
    nums.aModuleNum = getPmtModuleNumFromRowCol( nums.aModuleRow, nums.aModuleCol );
    nums.aModuleNumInPanel = nums.aModuleNum - m_RichPmtModuleCopyNumBeginPanelSIMD[3];
  }

  void getModuleNums_R2Le_Grand_SIMD( const SIMDFP& x, const SIMDFP& y, ModuleNumbersSIMD& nums ) const noexcept {
    using namespace LHCb::SIMD;
    nums.aModuleCol =
        simd_cast<SIMDINT32>( abs( x - m_GrandPmtModulePlaneHalfSizeR2SIMD[0] ) * m_GrandPmtModulePitchInvSIMD );
    nums.aModuleRow =
        simd_cast<SIMDINT32>( abs( y - m_GrandPmtModulePlaneHalfSizeR2SIMD[1] ) * m_GrandPmtModulePitchInvSIMD );
    nums.aModuleNum        = getPmtModuleNumFromRowCol( nums.aModuleRow, nums.aModuleCol );
    nums.aModuleNumInPanel = nums.aModuleNum - m_RichPmtModuleCopyNumBeginPanelSIMD[2];
  }

  void getModuleNums_R2Ri_Grand_SIMD( const SIMDFP& x, const SIMDFP& y, ModuleNumbersSIMD& nums ) const noexcept {
    using namespace LHCb::SIMD;
    nums.aModuleCol =
        simd_cast<SIMDINT32>( abs( x - m_GrandPmtModulePlaneHalfSizeR2SIMD[2] ) * m_GrandPmtModulePitchInvSIMD );
    nums.aModuleRow =
        simd_cast<SIMDINT32>( abs( y - m_GrandPmtModulePlaneHalfSizeR2SIMD[3] ) * m_GrandPmtModulePitchInvSIMD );
    nums.aModuleNum        = getPmtModuleNumFromRowCol( nums.aModuleRow, nums.aModuleCol );
    nums.aModuleNumInPanel = nums.aModuleNum - m_RichPmtModuleCopyNumBeginPanelSIMD[3];
  }

  void getModuleNums_R2Le_Mixed_SIMD( const SIMDFP& x, const SIMDFP& y, ModuleNumbersSIMD& nums ) const noexcept {
    using namespace LHCb::SIMD;
    nums.aModuleCol =
        simd_cast<SIMDINT32>( abs( x - m_MixedPmtModulePlaneHalfSizeR2SIMD[0] ) * m_GrandPmtModulePitchInvSIMD );

    // const auto m1 = simd_cast<SIMDINT32::mask_type>( y < m_MixedStdPmtModulePlaneHalfSizeR2SIMD[1] );
    nums.aModuleRow /*( m1 )*/ =
        simd_cast<SIMDINT32>( abs( y - m_MixedPmtModulePlaneHalfSizeR2SIMD[1] ) * m_GrandPmtModulePitchInvSIMD );

    const auto m2         = simd_cast<SIMDINT32::mask_type>( y >= abs( m_MixedStdPmtModulePlaneHalfSizeR2SIMD[1] ) );
    nums.aModuleRow( m2 ) = simd_cast<SIMDINT32>( abs( ( y - ( abs( m_MixedStdPmtModulePlaneHalfSizeR2SIMD[1] ) ) ) *
                                                       m_GrandPmtModulePitchInvSIMD ) ) +
                            m_Rich2MixedModuleArrayColumnSizeSIMD[0] + m_Rich2MixedModuleArrayColumnSizeSIMD[1];

    const auto m3 = simd_cast<SIMDINT32::mask_type>( abs( y ) < abs( m_MixedStdPmtModulePlaneHalfSizeR2SIMD[1] ) );
    nums.aModuleCol( m3 ) =
        simd_cast<SIMDINT32>( abs( x - m_MixedStdPmtModulePlaneHalfSizeR2SIMD[0] ) * m_PmtModulePitchInvSIMD );
    nums.aModuleRow( m3 ) =
        simd_cast<SIMDINT32>( abs( y - m_MixedStdPmtModulePlaneHalfSizeR2SIMD[1] ) * m_PmtModulePitchInvSIMD ) +
        m_Rich2MixedModuleArrayColumnSizeSIMD[0];

    nums.aModuleNum        = getPmtModuleNumFromRowCol( nums.aModuleRow, nums.aModuleCol );
    nums.aModuleNumInPanel = nums.aModuleNum - m_RichPmtModuleCopyNumBeginPanelSIMD[2];
  }

  void getModuleNums_R2Ri_Mixed_SIMD( const SIMDFP& x, const SIMDFP& y, ModuleNumbersSIMD& nums ) const noexcept {
    using namespace LHCb::SIMD;
    nums.aModuleCol =
        simd_cast<SIMDINT32>( abs( x - m_MixedPmtModulePlaneHalfSizeR2SIMD[2] ) * m_GrandPmtModulePitchInvSIMD );

    // const auto m1 = simd_cast<SIMDINT32::mask_type>( y > m_MixedStdPmtModulePlaneHalfSizeR2SIMD[3] );
    nums.aModuleRow /*( m1 )*/ =
        simd_cast<SIMDINT32>( abs( y - m_MixedPmtModulePlaneHalfSizeR2SIMD[3] ) * m_GrandPmtModulePitchInvSIMD );

    const auto m2         = simd_cast<SIMDINT32::mask_type>( y <= -m_MixedStdPmtModulePlaneHalfSizeR2SIMD[3] );
    nums.aModuleRow( m2 ) = simd_cast<SIMDINT32>( abs( ( y + m_MixedStdPmtModulePlaneHalfSizeR2SIMD[3] ) *
                                                       m_GrandPmtModulePitchInvSIMD ) ) +
                            m_Rich2MixedModuleArrayColumnSizeSIMD[0] + m_Rich2MixedModuleArrayColumnSizeSIMD[1];

    const auto m3 = simd_cast<SIMDINT32::mask_type>( abs( y ) < abs( m_MixedStdPmtModulePlaneHalfSizeR2SIMD[3] ) );
    nums.aModuleCol( m3 ) =
        simd_cast<SIMDINT32>( abs( x - m_MixedStdPmtModulePlaneHalfSizeR2SIMD[2] ) * m_PmtModulePitchInvSIMD );
    nums.aModuleRow( m3 ) =
        simd_cast<SIMDINT32>( abs( y - m_MixedStdPmtModulePlaneHalfSizeR2SIMD[3] ) * m_PmtModulePitchInvSIMD ) +
        m_Rich2MixedModuleArrayColumnSizeSIMD[0];

    nums.aModuleNum        = getPmtModuleNumFromRowCol( nums.aModuleRow, nums.aModuleCol );
    nums.aModuleNumInPanel = nums.aModuleNum - m_RichPmtModuleCopyNumBeginPanelSIMD[3];
  }

private:
  template <typename TYPE>
  inline void setRichPmtSmartIDPix( const TYPE         pixCol, //
                                    const TYPE         pixRow, //
                                    LHCb::RichSmartID& id      //
                                    ) const noexcept {
    id.setPixelCol_PMT( pixCol );
    id.setPixelRow_PMT( pixRow );
  }

  template <typename TYPE>
  inline void setRichPmtSmartID( const TYPE         pdCol,   //
                                 const TYPE         pdInCol, //
                                 const TYPE         pixCol,  //
                                 const TYPE         pixRow,  //
                                 LHCb::RichSmartID& id       //
                                 ) const noexcept {
    id.setPD_PMT( pdCol, pdInCol );
    setRichPmtSmartIDPix( pixCol, pixRow, id );
  }

private:
  inline Int getNumModulesInThisPanel() const noexcept { return m_NumPmtModuleInRich[m_CurPanelNum]; }

  inline SIMDINT32 getPmtNumFromRowCol( SIMDINT32 PRow, SIMDINT32 PCol ) const noexcept {
    // for values outside the range, set the closest value to the
    // corresponding edges.
    PRow.setZero( PRow < SIMDINT32::Zero() );
    PRow( PRow >= m_NumPmtInRowColSIMD[1] ) = m_NumPmtInRowCol[1] - SIMDINT32::One();
    PCol.setZero( PCol < SIMDINT32::Zero() );
    PCol( PCol >= m_NumPmtInRowColSIMD[0] ) = m_NumPmtInRowCol[0] - SIMDINT32::One();
    return ( PCol + ( PRow * m_NumPmtInRowColSIMD[0] ) );
  }

  inline SIMDINT32 getGrandPmtNumFromRowCol( SIMDINT32 PRow, SIMDINT32 PCol ) const noexcept {
    PRow.setZero( PRow < SIMDINT32::Zero() );
    PRow( PRow >= m_NumGrandPmtInRowColSIMD[1] ) = m_NumGrandPmtInRowColSIMD[1] - SIMDINT32::One();
    PCol.setZero( PCol < SIMDINT32::Zero() );
    PCol( PCol >= m_NumGrandPmtInRowColSIMD[0] ) = m_NumGrandPmtInRowColSIMD[0] - SIMDINT32::One();
    return ( PCol + ( PRow * m_NumGrandPmtInRowColSIMD[0] ) );
  }

  inline bool isInPmtAnodeLateralAcc( const Gaudi::XYZPoint& aPointInPmtAnode, //
                                      const bool             aFlagGrandPMT     //
                                      ) const noexcept {
    const auto xp = fabs( aPointInPmtAnode.x() );
    const auto yp = fabs( aPointInPmtAnode.y() );
    return ( aFlagGrandPMT && rich() == Rich::Rich2
                 ? ( ( xp < fabs( m_GrandPmtAnodeXEdge ) ) && ( yp < fabs( m_GrandPmtAnodeYEdge ) ) )
                 : ( ( xp < fabs( m_PmtAnodeXEdge ) ) && ( yp < fabs( m_PmtAnodeYEdge ) ) ) );
  }

  inline decltype( auto ) isInPmtAnodeLateralAcc( const SIMDFP            X,            //
                                                  const SIMDFP            Y,            //
                                                  const SIMDFP::mask_type aFlagGrandPMT //
                                                  ) const noexcept {
    SIMDFP gxe( m_PmtAnodeXEdge );
    SIMDFP gye( m_PmtAnodeYEdge );
    gxe( aFlagGrandPMT ) = SIMDFP( m_GrandPmtAnodeXEdge );
    gye( aFlagGrandPMT ) = SIMDFP( m_GrandPmtAnodeYEdge );

    return ( X < abs( gxe ) && Y < abs( gye ) );
  }

  inline bool isInPmt( const Gaudi::XYZPoint& aPointInPmt,  //
                       const bool             aFlagGrandPMT //
                       ) const noexcept {
    const auto aPmtH = ( ( aFlagGrandPMT && rich() == Rich::Rich2 ) ? ( m_GrandPmtMasterLateralSize * 0.5 )
                                                                    : ( m_PmtMasterLateralSize * 0.5 ) );
    return ( fabs( aPointInPmt.x() ) < aPmtH && fabs( aPointInPmt.y() ) < aPmtH );
  }

  inline decltype( auto ) isInPmt( const SIMDFP            X,            //
                                   const SIMDFP            Y,            //
                                   const SIMDFP::mask_type aFlagGrandPMT //
                                   ) const noexcept {
    SIMDFP aPmtH( m_PmtMasterLateralSize * 0.5 );
    aPmtH( aFlagGrandPMT ) = SIMDFP( m_GrandPmtMasterLateralSize * 0.5 );
    return ( X < aPmtH && Y < aPmtH );
  }

  inline decltype( auto ) isInPmtPanel( const SIMDPoint& aPointInPanel ) const noexcept {
    return ( abs( aPointInPanel.x() ) < m_xyHalfSizeSIMD[0] && abs( aPointInPanel.y() ) < m_xyHalfSizeSIMD[1] );
  }

  inline decltype( auto ) checkPDAcceptance( SIMDFP                  X,            //
                                             SIMDFP                  Y,            //
                                             const SIMDFP::mask_type aFlagGrandPMT //
                                             ) const noexcept {
    X = abs( X );
    Y = abs( Y );
    return ( isInPmt( X, Y, aFlagGrandPMT ) && isInPmtAnodeLateralAcc( X, Y, aFlagGrandPMT ) );
  }

  inline bool checkPDAcceptance( const Gaudi::XYZPoint& aPointInPmt,  //
                                 const bool             aFlagGrandPMT //
                                 ) const noexcept {
    return ( isInPmt( aPointInPmt, aFlagGrandPMT ) && isInPmtAnodeLateralAcc( aPointInPmt, aFlagGrandPMT ) );
  }

private:
  /// Gets the intersection with the panel (SIMD) in global panel coordinates
  inline decltype( auto ) getPanelInterSection( const SIMDPoint&  pGlobal,          //
                                                const SIMDVector& vGlobal,          //
                                                SIMDPoint&        panelIntersection //
                                                ) const noexcept {

    // find the intersection with the detection plane
    auto scalar = vGlobal.Dot( m_detectionPlaneNormalSIMD );

    // check norm
    const auto sc = abs( scalar ) > SIMDFP( 1e-5 );

    // Protect against /0
    if ( !all_of( sc ) ) { scalar( !sc ) = SIMDFP::One(); }

    // get panel intersection point
    const auto distance = m_detectionPlaneSIMD.Distance( pGlobal ) / scalar;
    panelIntersection   = pGlobal - ( distance * vGlobal );

    // return
    return sc;
  }

  inline bool ModuleIsWithGrandPMT( const Int aModuleNum ) const noexcept {
    // Only RICH2 has grand PMTs
    return ( rich() == Rich::Rich2 &&   //
                     aModuleNum >= 0 && //
                     aModuleNum < (Int)m_ModuleIsWithGrandPMT.size()
                 ? m_ModuleIsWithGrandPMT[aModuleNum]
                 : false );
  }

  inline SIMDINT32::mask_type ModuleIsWithGrandPMT( const SIMDINT32& aModuleNum ) const noexcept {
    // Only RICH2 has grand PMTs
    if ( rich() == Rich::Rich1 ) {
      return SIMDINT32::mask_type::Zero();
    } else {
      auto m = ( aModuleNum >= SIMDINT32::Zero() && //
                 aModuleNum < SIMDINT32( m_ModuleIsWithGrandPMT.size() ) );
      GAUDI_LOOP_UNROLL( SIMDINT32::Size )
      for ( std::size_t i = 0; i < SIMDINT32::Size; ++i ) {
        // if ( m[i] )
        m[i] = m_ModuleIsWithGrandPMT[aModuleNum[i]];
      }
      return m;
    }
  }

private:
  /// utility method to convert a vector to an array of the same size.
  template <typename OUTTYPE, std::size_t N, typename INTYPE = OUTTYPE>
  decltype( auto ) toarray( const std::vector<INTYPE>& v ) const {
    if ( UNLIKELY( v.size() != N ) ) {
      throw GaudiException( "Vector to Array Size Error", "DeRichPMTPanelClassic", StatusCode::FAILURE );
    }
    std::array<OUTTYPE, N> a;
    std::copy( v.begin(), v.end(), a.begin() );
    return a;
  }

private:
  // SIMD caches of quantities

  /// SIMD To global transform
  Rich::SIMD::Transform3D<Rich::SIMD::DefaultScalarFP> m_toGlobalMatrixSIMD;

  /// SIMD 'toLocal' transformation
  Rich::SIMD::Transform3D<Rich::SIMD::DefaultScalarFP> m_toLocalMatrixSIMD;

  /// SIMD detection plane normal (global coordis)
  Rich::SIMD::Vector<Rich::SIMD::DefaultScalarFP> m_detectionPlaneNormalSIMD;

  /// SIMD detection plane
  Rich::SIMD::Plane<Rich::SIMD::DefaultScalarFP> m_detectionPlaneSIMD;

  /// (X,Y) panel half sizes for this panel
  XYArraySIMD m_xyHalfSizeSIMD = {{}};

  SIMDFP m_PmtModulePitchInvSIMD = SIMDFP::Zero();

  std::array<SIMDFP, 4> m_PmtModulePlaneHalfSizeR1SIMD = {{}};
  std::array<SIMDFP, 4> m_PmtModulePlaneHalfSizeR2SIMD = {{}};

  std::array<SIMDINT32, 4> m_RichPmtNumModulesInRowColSIMD = {{}};

  std::array<SIMDFP, 4> m_GrandPmtModulePlaneHalfSizeR2SIMD = {{}};
  std::array<SIMDFP, 4> m_MixedPmtModulePlaneHalfSizeR2SIMD = {{}};

  SIMDFP m_GrandPmtModulePitchInvSIMD = SIMDFP::Zero();

  std::array<SIMDFP, 4> m_MixedStdPmtModulePlaneHalfSizeR2SIMD = {{}};

  std::array<SIMDFP, 2> m_RichGrandPmtModuleActiveAreaHalfSizeSIMD = {{}};

  std::array<SIMDINT32, 2> m_NumGrandPmtInRowColSIMD = {{}};

  SIMDFP m_GrandPmtPitchInvSIMD = SIMDFP::Zero();

  std::array<SIMDFP, 2> m_RichPmtModuleActiveAreaHalfSizeSIMD = {{}};

  SIMDFP m_PmtPitchInvSIMD = SIMDFP::Zero();

  std::array<SIMDINT32, 2> m_NumPmtInRowColSIMD = {{}};

  SIMDFP m_GrandPmtAnodeXEdgeSIMD = SIMDFP::Zero();
  SIMDFP m_GrandPmtAnodeYEdgeSIMD = SIMDFP::Zero();

  SIMDFP m_GrandPmtAnodeEffectiveXPixelSizeInvSIMD = SIMDFP::Zero();
  SIMDFP m_GrandPmtAnodeEffectiveYPixelSizeInvSIMD = SIMDFP::Zero();

  SIMDINT32 m_GrandPmtPixelsInRowSIMD = SIMDINT32::Zero();
  SIMDINT32 m_GrandPmtPixelsInColSIMD = SIMDINT32::Zero();

  SIMDFP m_PmtAnodeXEdgeSIMD = SIMDFP::Zero();
  SIMDFP m_PmtAnodeYEdgeSIMD = SIMDFP::Zero();

  SIMDFP m_PmtAnodeEffectiveXPixelSizeInvSIMD = SIMDFP::Zero();
  SIMDFP m_PmtAnodeEffectiveYPixelSizeInvSIMD = SIMDFP::Zero();

  SIMDINT32 m_PmtPixelsInRowSIMD = SIMDINT32::Zero();
  SIMDINT32 m_PmtPixelsInColSIMD = SIMDINT32::Zero();

  std::array<SIMDINT32, 4> m_RichPmtModuleCopyNumBeginPanelSIMD = {{}};
  std::array<SIMDINT32, 4> m_RichPmtModuleCopyNumEndPanelSIMD   = {{}};

  Rich::SIMD::STDVector<SIMDINT32> m_Rich2MixedModuleArrayColumnSizeSIMD;

private:
  /// Index for this panel
  Int m_CurPanelNum{-1};

  /// Container for the PMT Module geometry() pointers
  IGeomInfoV m_DePMTModules{1, nullptr};

  /// Position of module local {0,0,0} in panel local coordinates
  std::vector<Gaudi::XYZPoint> m_DePMTModulesZeroPtn;

  ///< Container for the PMTs, sorted by panel
  std::vector<DRiPMTV> m_DePMTs{1, DRiPMTV( 2, nullptr )};

  /// Container for the PMTAnodes geometry() pointers
  std::vector<IGeomInfoV> m_DePMTAnodes{1, IGeomInfoV( 2, nullptr )};

  /// Position of anode local {0,0,0} in panel local coordinates
  std::vector<std::vector<Gaudi::XYZPoint>> m_DePMTAnodesZeroPtn;

  /// Total number of PMT
  unsigned int m_totNumPMTs{0};

  std::array<Int, 4> m_RichPmtNumModulesInRowCol      = {{}};
  std::array<Int, 4> m_RichPmtModuleCopyNumBeginPanel = {{}};
  std::array<Int, 4> m_RichPmtModuleCopyNumEndPanel   = {{}};
  std::array<Int, 2> m_NumPmtInRowCol                 = {{}};
  std::array<Int, 4> m_NumPmtModuleInRich             = {{}};
  Int                m_NumPmtInRichModule{0};
  Int                m_totNumPmtModuleInRich{0};

  double m_PmtAnodeXEdge{0};
  double m_PmtAnodeYEdge{0};

  double m_PmtMasterLateralSize{0};

  bool m_Rich2UseGrandModule{false};
  Int  m_Rich2ArrayConfig{0};
  bool m_Rich2UseMixedModule{false};

  double             m_GrandPmtAnodeXEdge{0};
  double             m_GrandPmtAnodeYEdge{0};
  double             m_GrandPmtMasterLateralSize{0};
  std::array<Int, 2> m_NumGrandPmtInRowCol = {{}};

  std::vector<int>  m_Rich2MixedModuleArrayColumnSize{3, 0};
  std::vector<bool> m_ModuleIsWithGrandPMT;

  Rich::DAQ::PDPanelIndex m_maxPDCopyN{0};
};
