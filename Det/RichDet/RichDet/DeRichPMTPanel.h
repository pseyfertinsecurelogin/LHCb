
#pragma once

// STL
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <cstdint>

// Gaudi
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// DetDesc
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/ISolid.h"

// RichDet
#include "RichDet/DeRichPDPanel.h"
#include "RichDet/DeRich.h"
#include "RichDet/DeRichPMT.h"
#include "RichDet/DeRichLocations.h"

/** @class DeRichPMTPanel DeRichPMTPanel.h RichDet/DeRichPMTPanel.h
 *
 *  Detector element for PMT panels
 *
 *  @author Sajan Easo
 *  @date   2011-10-10
 */
class DeRichPMTPanel : public DeRichPDPanel
{

public:

  /// Standard constructor
  DeRichPMTPanel( const std::string & name = ""  );

  virtual ~DeRichPMTPanel( ) = default; ///< Destructor

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  const CLID& clID() const override final { return classID(); }

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
  bool smartID( const Gaudi::XYZPoint& globalPoint,
                LHCb::RichSmartID& id ) const override final;

  // Returns the intersection point with the detector plane given a vector and a point.
  LHCb::RichTraceMode::RayTraceResult
  detPlanePoint( const Gaudi::XYZPoint& pGlobal,
                 const Gaudi::XYZVector& vGlobal,
                 Gaudi::XYZPoint& hitPosition,
                 LHCb::RichSmartID& smartID,
                 const DeRichPD*& pd,
                 const LHCb::RichTraceMode mode ) const override final;

  // Returns the intersection point with an HPD window given a vector and a point.
  LHCb::RichTraceMode::RayTraceResult
  PDWindowPoint( const Gaudi::XYZPoint& pGlobal,
                 const Gaudi::XYZVector& vGlobal,
                 Gaudi::XYZPoint& windowPointGlobal,
                 LHCb::RichSmartID& smartID,
                 const DeRichPD*& pd,
                 const LHCb::RichTraceMode mode ) const override final;

  // Returns the SIMD intersection point with an HPD window given a vector and a point.
  SIMDRayTResult::Results
  PDWindowPointSIMD( const Rich::SIMD::Point<FP> & pGlobal,
                     const Rich::SIMD::Vector<FP> & vGlobal,
                     Rich::SIMD::Point<FP> & hitPosition,
                     SIMDRayTResult::SmartIDs& smartID,
                     SIMDRayTResult::PDs& PDs,
                     const LHCb::RichTraceMode mode ) const override final;

  // Returns the SIMD intersection point with the detector plane given a vector and a point.
  SIMDRayTResult::Results
  detPlanePointSIMD( const Rich::SIMD::Point<FP> & pGlobal,
                     const Rich::SIMD::Vector<FP> & vGlobal,
                     Rich::SIMD::Point<FP> & hitPosition,
                     SIMDRayTResult::SmartIDs& smartID,
                     SIMDRayTResult::PDs& PDs,
                     const LHCb::RichTraceMode mode ) const override final;

  // Adds to the given vector all the available readout channels in this HPD panel
  bool readoutChannelList( LHCb::RichSmartID::Vector& readoutChannels ) const override final;

  /// Get tge sensitivevolumeID
  int sensitiveVolumeID( const Gaudi::XYZPoint& globalPoint ) const override final;
  
  /** Converts a RichSmartID to a point on the anode in global coordinates.
   *  @param[in] smartID The RichSmartID for the PMT channel
   *  @return The detection point on the anode in global coordinates
   */
  inline Gaudi::XYZPoint detPointOnAnode( const LHCb::RichSmartID smartID ) const
  {
    const auto * aPMT = dePMT( smartID );
    return ( aPMT ? aPMT->detPointOnAnode(smartID) : Gaudi::XYZPoint(0,0,0) );
  }

  /// Returns the PD number for the given RichSmartID
  Rich::DAQ::PDPanelIndex pdNumber( const LHCb::RichSmartID& smartID ) const override;

  // The maximum PD copy number for this panel
  Rich::DAQ::PDPanelIndex maxPdNumber() const override;

private: 

  using Int        = std::int32_t;
  using IDeElemV   = std::vector<IDetectorElement*>;
  using DRiPMTV    = std::vector<DeRichPMT*>;
  using ArraySetup = std::array<Int,4>;
  using RowCol     = std::array<Int,2>;
  using XYArray    = std::array<double,2>;
  
  using XYArraySIMD = std::array<SIMDFP,2>;

private: // setup methods

  /// Update cached information on geometry changes
  StatusCode geometryUpdate();

  StatusCode getPanelGeometryInfo();

  /// Setup for Lens Flag
  void Rich1SetupPMTModulesWithLens();

  void RichSetupMixedSizePmtModules();

  // Set the rich panel and side
  bool setRichAndSide();

private:

  /// Returns the PD number for the given RichSmartID
  inline Rich::DAQ::PDPanelIndex _pdNumber( const LHCb::RichSmartID smartID ) const noexcept
  {
    // check for now. to be removed.
    if ( smartID.rich() != rich() || smartID.panel() != side() )
    { 
      error() << "_pdNumber RICH and side error " << smartID << endmsg;
    }
    // Should never get different rich or panel, so skip check
    //return Rich::DAQ::PDPanelIndex( smartID.rich() == rich() && smartID.panel() == side() ?
    //                                ( smartID.pdCol() * m_NumPmtInRichModule ) + smartID.pdNumInCol() :
    //                                nPDs() + 1 );
    return Rich::DAQ::PDPanelIndex( ( smartID.pdCol() * m_NumPmtInRichModule ) + smartID.pdNumInCol() );
  }

  const DeRichPMT* dePMT( const Rich::DAQ::PDPanelIndex PmtNumber ) const;

  inline const DeRichPMT* dePMT( const LHCb::RichSmartID pdID ) const
  {
    // get the lookup indices from the smart ID
    auto pdCol   = pdID.pdCol();
    auto pdInCol = pdID.pdNumInCol();  
    
    // if need be correct the pdCol (for data when it was incorrectly filled)
    // this is temporary, can be removed when no longer needed...
    if ( pdCol >= m_DePMTs.size() ) { pdCol = PmtModuleNumInPanelFromModuleNumAlone(pdCol); }
    
    // return the pointer from the array
    return m_DePMTs[pdCol][pdInCol];

    // get the old way
    //return dePMT( _pdNumber( pdID ) );
  }

  inline RowCol getPmtRowColFromPmtNum( const Int aPmtNum ) const noexcept
  {
    const Int aPRow = (aPmtNum/m_NumPmtInRowCol[0]);
    return { aPRow, (Int)( aPmtNum - (aPRow*m_NumPmtInRowCol[0]) ) };
  }

  inline RowCol getGrandPmtRowColFromPmtNum( const Int aPmtNum ) const noexcept
  {
    const Int aPRow = (aPmtNum/m_NumGrandPmtInRowCol[0]);
    return { aPRow, (Int)( aPmtNum - (aPRow*m_NumGrandPmtInRowCol[0]) ) };
  }

  inline Int PmtModuleNumInPanelFromModuleNum( const Int aMnum ) const noexcept
  {
    return aMnum - m_RichPmtModuleCopyNumBeginPanel[m_CurPanelNum];
  }

  inline Int PmtModuleNumInPanelFromModuleNumAlone( const Int aMnum ) const noexcept
  {
    //info() << "PmtModuleNumInPanelFromModuleNumAlone Begin " << m_RichPmtModuleCopyNumBeginPanel << endmsg;
    //info() << "PmtModuleNumInPanelFromModuleNumAlone End   " << m_RichPmtModuleCopyNumEndPanel   << endmsg;
    // const auto i = ( aMnum >= m_RichPmtModuleCopyNumBeginPanel[0] &&
    //                  aMnum <= m_RichPmtModuleCopyNumEndPanel[0]   ? 0 :
    //                  aMnum >= m_RichPmtModuleCopyNumBeginPanel[1] &&
    //                  aMnum <= m_RichPmtModuleCopyNumEndPanel[1]   ? 1 :
    //                  aMnum >= m_RichPmtModuleCopyNumBeginPanel[2] &&
    //                  aMnum <= m_RichPmtModuleCopyNumEndPanel[2]   ? 2 :
    //                  aMnum >= m_RichPmtModuleCopyNumBeginPanel[3] &&
    //                  aMnum <= m_RichPmtModuleCopyNumEndPanel[3]   ? 3 :
    //          -1 );
    //info() << "PmtModuleNumInPanelFromModuleNumAlone " << aMnum << " i = " << i << endmsg;

    return ( aMnum >= m_RichPmtModuleCopyNumBeginPanel[0] &&
             aMnum <= m_RichPmtModuleCopyNumEndPanel[0]   ? aMnum - m_RichPmtModuleCopyNumBeginPanel[0] :
             aMnum >= m_RichPmtModuleCopyNumBeginPanel[1] &&
             aMnum <= m_RichPmtModuleCopyNumEndPanel[1]   ? aMnum - m_RichPmtModuleCopyNumBeginPanel[1] :
             aMnum >= m_RichPmtModuleCopyNumBeginPanel[2] &&
             aMnum <= m_RichPmtModuleCopyNumEndPanel[2]   ? aMnum - m_RichPmtModuleCopyNumBeginPanel[2] :
             aMnum >= m_RichPmtModuleCopyNumBeginPanel[3] &&
             aMnum <= m_RichPmtModuleCopyNumEndPanel[3]   ? aMnum - m_RichPmtModuleCopyNumBeginPanel[3] :
             -1 );
  }

  inline RowCol PmtModuleRowColFromModuleNumInPanel( const Int aMnum ) const noexcept
  {
    RowCol rc = {-1,-1};
    auto & MRow = rc[0];
    auto & MCol = rc[1];
    
    if ( Rich::Rich1 == rich() )
    {
      MRow = (Int) (aMnum/m_RichPmtNumModulesInRowCol[0]);
      MCol = aMnum - MRow*m_RichPmtNumModulesInRowCol[0];
    }
    else //if ( rich() == Rich::Rich2 || rich() == Rich::Rich )
    {
      MCol = (Int) (aMnum/m_RichPmtNumModulesInRowCol[3]);
      MRow = aMnum - MCol*m_RichPmtNumModulesInRowCol[3];
    }
    
    return rc;
  }

  Int getPmtModuleNumFromRowCol( Int MRow, Int MCol ) const;

  inline ArraySetup findPMTArraySetup( const Gaudi::XYZPoint& aGlobalPoint ) const
  {
    const auto inPanel = geometry()->toLocalMatrix() * aGlobalPoint;
    return findPMTArraySetup( aGlobalPoint, inPanel );
  } 

  ArraySetup findPMTArraySetup( const Gaudi::XYZPoint& aGlobalPoint,
                                const Gaudi::XYZPoint& aLocalPoint ) const;

  struct ModuleNumbers
  {
    Int  aModuleCol        = -1;
    Int  aModuleRow        = -1;
    Int  aModuleNum        = -1;
    Int  aModuleNumInPanel = -1;
    bool aModuleWithLens   =  false;
  };
  void getModuleNums( const double x, const double y, ModuleNumbers& nums ) const;

  template< typename TYPE >
  inline void setRichPmtSmartID( const TYPE pdCol,
                                 const TYPE pdInCol,
                                 const TYPE pixCol,
                                 const TYPE pixRow,
                                 LHCb::RichSmartID& id ) const noexcept
  {
    id.setPD(pdCol,pdInCol); 
    id.setPixelCol(pixCol);
    id.setPixelRow(pixRow);
  }
  
  const DetectorElement * getFirstDeRich() const;
  
  inline Int getNumModulesInThisPanel() const noexcept
  {
    return m_NumPmtModuleInRich[m_CurPanelNum];
  }
  
  inline Int getPmtNumFromRowCol( Int PRow, Int PCol ) const noexcept
  {
    // for values outside the range, set the closest value to the
    // corresponding edges. 
    if      ( PRow < 0                    ) { PRow = 0; }
    else if ( PRow >= m_NumPmtInRowCol[1] ) { PRow = m_NumPmtInRowCol[1]-1; }
    if      ( PCol < 0                    ) { PCol = 0; }
    else if ( PCol >= m_NumPmtInRowCol[0] ) { PCol = m_NumPmtInRowCol[0]-1; }
    return ( PCol + ( PRow*m_NumPmtInRowCol[0] ) );
  }

  inline Int getGrandPmtNumFromRowCol( Int PRow, Int PCol ) const noexcept
  {
    // for values outside the range, set the closest value to the
    // corresponding edges.
    if      ( PRow < 0                         ) { PRow = 0; }
    else if ( PRow >= m_NumGrandPmtInRowCol[1] ) { PRow = m_NumGrandPmtInRowCol[1] - 1; }
    if      ( PCol < 0                         ) { PCol = 0; }
    else if ( PCol >= m_NumGrandPmtInRowCol[0] ) { PCol = m_NumGrandPmtInRowCol[0] - 1; }
    return ( PCol + ( PRow*m_NumGrandPmtInRowCol[0] ) );
  }

  inline bool isInPmtAnodeLateralAcc( const Gaudi::XYZPoint& aPointInPmtAnode,
                                      const bool aFlagGrandPMT ) const noexcept
  {
    const auto xp = fabs(aPointInPmtAnode.x());
    const auto yp = fabs(aPointInPmtAnode.y());
    return ( aFlagGrandPMT && rich() == Rich::Rich2 ?
             ( ( xp < fabs( m_GrandPmtAnodeXEdge ) ) &&
               ( yp < fabs( m_GrandPmtAnodeYEdge ) ) ) :
             ( ( xp < fabs( m_PmtAnodeXEdge ) ) &&
               ( yp < fabs( m_PmtAnodeYEdge ) ) ) );
  }

  inline bool isInPmt( const Gaudi::XYZPoint& aPointInPmt, 
                       const bool aFlagGrandPMT ) const noexcept
  {
    const auto aPmtH = ( ( aFlagGrandPMT && rich() == Rich::Rich2 ) ?
                         ( m_GrandPmtMasterLateralSize*0.5) : (m_PmtMasterLateralSize*0.5) );
    return ( fabs(aPointInPmt.x()) < aPmtH && 
             fabs(aPointInPmt.y()) < aPmtH );
  }
  
  inline bool isInPmtPanel( const Gaudi::XYZPoint& aPointInPanel ) const noexcept
  {
    return ( fabs(aPointInPanel.x()) < m_xyHalfSize[0] &&
             fabs(aPointInPanel.y()) < m_xyHalfSize[1] );
  }

  inline decltype(auto) isInPmtPanel( const Rich::SIMD::Point<FP>& aPointInPanel ) const noexcept
  {
    return ( abs(aPointInPanel.x()) < m_xyHalfSizeSIMD[0] &&
             abs(aPointInPanel.y()) < m_xyHalfSizeSIMD[1] ); 
  }
  
  /// Gets the intercestion with the panel (scalar)
  inline bool getPanelInterSection ( const Gaudi::XYZPoint& pGlobal,
                                     const Gaudi::XYZVector& vGlobal ,
                                     Gaudi::XYZPoint& panelIntersection ) const
  {
    // transform to the panel
    const auto vInPanel = geometry()->toLocalMatrix() * vGlobal;
    // find the intersection with the detection plane
    const auto scalar = vInPanel.Dot(m_localPlaneNormal);
    // check norm
    const auto sc = fabs(scalar) > 1e-5;
    if ( sc )
    {
      // transform point to the PMTPanel coordsystem.
      const auto pInPanel = geometry()->toLocalMatrix() * pGlobal;
      // get panel intersection point
      const auto distance = -m_localPlane.Distance(pInPanel) / scalar;
      panelIntersection = pInPanel + ( distance * vInPanel );
    }
    // return
    return sc;
  }

  /// Gets the intercestion with the panel (SIMD)
  inline decltype(auto) getPanelInterSection ( const Rich::SIMD::Point<FP>& pGlobal,
                                               const Rich::SIMD::Vector<FP>& vGlobal ,
                                               Rich::SIMD::Point<FP>& panelIntersection ) const
  {
    // transform to the panel
    const auto vInPanel = m_toLocalMatrixSIMD * vGlobal;
    // find the intersection with the detection plane
    auto scalar = vInPanel.Dot(m_localPlaneNormalSIMD);
    // check norm
    const auto sc = abs(scalar) > SIMDFP(1e-5);
    // Protect against /0
    scalar(!sc) = SIMDFP::One();
    // transform point to the PMTPanel coordsystem.
    const auto pInPanel = m_toLocalMatrixSIMD * pGlobal;
    // get panel intersection point
    const auto distance = -m_localPlaneSIMD.Distance(pInPanel) / scalar;
    panelIntersection = pInPanel + ( distance * vInPanel );
    // return
    return sc;
  }

  inline Int getLensPmtNumFromRowCol( Int PRow, Int PCol ) const noexcept
  {
    // for values outside the range, set the closest value to the
    // corresponding edges.
    if      ( PRow < 0                                  ) { PRow = 0; }
    else if ( PRow >= m_RichNumLensPmtinModuleRowCol[1] ) { PRow = m_RichNumLensPmtinModuleRowCol[1] - 1; }
    if      ( PCol < 0                                  ) { PCol = 0; }
    else if ( PCol >= m_RichNumLensPmtinModuleRowCol[0] ) { PCol = m_RichNumLensPmtinModuleRowCol[0] - 1; }
    return ( PCol + ( PRow*m_RichNumLensPmtinModuleRowCol[0] ) );
  }

  /// setup flags for grand Modules
  Int getModuleCopyNumber( const std::string& aModuleName );

  inline bool isCurrentPmtModuleWithLens(const Int aModuleNum) const noexcept
  {
    return ( aModuleNum < m_totNumPmtModuleInRich1 ?
             m_RichPmtModuleLensFlag[aModuleNum] : false );
  }
  
  inline bool isCurrentPmtWithLens( const Int aPMTNum ) const noexcept
  {
    const Int aModuleNum = aPMTNum/m_NumPmtInRichModule;
    return isCurrentPmtModuleWithLens(aModuleNum);
  }

  inline Gaudi::XYZPoint DemagnifyFromLens( const Gaudi::XYZPoint& aLensPoint ) const noexcept
  {
    return { aLensPoint.x() * m_Rich1LensDemagnificationFactor,
             aLensPoint.y() * m_Rich1LensDemagnificationFactor,
             aLensPoint.z() };
  }

  inline bool ModuleIsWithGrandPMT( const Int aModuleNum ) const noexcept
  {
    return ( aModuleNum >= 0 && 
             aModuleNum <  (Int) m_ModuleIsWithGrandPMT.size() ?
             m_ModuleIsWithGrandPMT[aModuleNum] : false );
  }

private:

  /// utility method to convert a vector to an array of the same size.
  template< typename OUTTYPE, std::size_t N, typename INTYPE = OUTTYPE >
  decltype(auto) toarray( const std::vector<INTYPE>& v ) const
  {
    if ( v.size() != N ) 
    { throw GaudiException("Vector to Array Size Error","DeRichPMTPanel",StatusCode::FAILURE); }
    std::array<OUTTYPE,N> a;
    std::copy( v.begin(), v.end(), a.begin() );
    return a;
  }

private:

  // SIMD caches of quantities

  /// SIMD To global transform
  Rich::SIMD::Transform3D<Rich::SIMD::DefaultScalarFP> m_toGlobalMatrixSIMD;
  
  /// SIMD 'toLocal' transformation
  Rich::SIMD::Transform3D<Rich::SIMD::DefaultScalarFP> m_toLocalMatrixSIMD;

  /// SIMD local plane normal
  Rich::SIMD::Vector<Rich::SIMD::DefaultScalarFP> m_localPlaneNormalSIMD;

  /// SIMD local plane
  Rich::SIMD::Plane<Rich::SIMD::DefaultScalarFP> m_localPlaneSIMD;

  /// (X,Y) panel half sizes for this panel
  XYArraySIMD m_xyHalfSizeSIMD = {{}};

private:

  /// SmartID for this panel
  LHCb::RichSmartID m_panelID;

  /// Index for this panel
  std::int8_t m_CurPanelNum{-1};

  /// Container for the PMT Modules as Det Elements
  IDeElemV m_DePMTModules{1,nullptr};

  ///< Container for the PMTs, sorted by panel
  std::vector<DRiPMTV> m_DePMTs{1,DRiPMTV(2,nullptr)};

  /// Container for the PMTAnodes as Det Elements
  std::vector<IDeElemV> m_DePMTAnodes{1,IDeElemV(2,nullptr)};

  /// Total number of PMT
  unsigned int m_totNumPMTs{0};

  /// (X,Y) panel half sizes for this panel
  XYArray m_xyHalfSize = {{}};

  std::array<double,4> m_PmtModulePlaneHalfSizeR1 = {{}};
  std::array<double,4> m_PmtModulePlaneHalfSizeR2 = {{}};
  double m_PmtModulePitchInv{0};
  double m_PmtPitchInv{0};
  std::array<Int,4> m_RichPmtNumModulesInRowCol = {{}};
  std::array<Int,4> m_RichPmtModuleCopyNumBeginPanel = {{}};
  std::array<Int,4> m_RichPmtModuleCopyNumEndPanel = {{}};
  std::array<double,2> m_RichPmtModuleActiveAreaHalfSize = {{}};
  std::array<Int,2> m_NumPmtInRowCol = {{}};
  std::array<Int,4> m_NumPmtModuleInRich = {{}};
  Int m_NumPmtInRichModule{0};
  Int m_totNumPmtModuleInRich{0};
  double m_PmtAnodeXSize{0};
  double m_PmtAnodeYSize{0};
  double m_PmtPixelGap{0};
  Int m_PmtPixelsInRow{0};
  Int m_PmtPixelsInCol{0};
  double m_PmtAnodeXEdge{0};
  double m_PmtAnodeYEdge{0};
  double m_AnodeXPixelSize{0};
  double m_AnodeYPixelSize{0};
  double m_PmtAnodeEffectiveXPixelSizeInv{0};
  double m_PmtAnodeEffectiveYPixelSizeInv{0};
  double m_PmtMasterLateralSize{0};
  double m_RichPmtQuartzThickness{0};
  double m_RichPmtQuartzLocalZInPmt{0};

  Int m_Rich1PmtLensPresence{0};
  std::vector<int>  m_Rich1PmtLensModuleCol;
  std::vector<bool> m_RichPmtModuleLensFlag;
  Int m_totNumPmtModuleInRich1{0};

  Gaudi::Plane3D m_localPlane;
  Gaudi::XYZVector m_localPlaneNormal;
 
  Gaudi::Plane3D m_detectionPlane_exterior;
  // Access info related to PMT Lens flag

  double m_PmtMasterWithLensLateralSize{0};
  double m_PmtModuleWithLensPitchInv{0};
  double m_PmtLensPitch{0};
  std::array<double,2> m_Rich1PmtPanelWithLensXSize = {{}};
  std::array<double,2> m_Rich1PmtPanelWithLensYSize = {{}};
  std::array<Int,2>    m_Rich1PmtPanelWithLensColSize = {{}};
  std::array<Int,2>    m_RichNumLensPmtinModuleRowCol = {{}};
  double m_Rich1LensDemagnificationFactor{0};

  bool  m_Rich2UseGrandModule{false};
  Int   m_Rich2ArrayConfig{0};
  bool  m_Rich2UseMixedModule{false};

  std::array<double,4> m_GrandPmtModulePlaneHalfSizeR2 = {{}};
  std::array<double,4> m_MixedPmtModulePlaneHalfSizeR2 = {{}};
  std::array<double,4> m_MixedStdPmtModulePlaneHalfSizeR2 = {{}};
  double m_GrandPmtModulePitchInv{0};
  std::array<double,2> m_RichGrandPmtModuleActiveAreaHalfSize = {{}};
  double m_GrandPmtPitchInv{0};
  double m_GrandPmtAnodeXSize{0};
  double m_GrandPmtAnodeYSize{0};
  double m_GrandPmtPixelGap{0};
  Int    m_GrandPmtPixelsInRow{0};
  Int    m_GrandPmtPixelsInCol{0};
  double m_GrandPmtAnodeXEdge{0};
  double m_GrandPmtAnodeYEdge{0};
  double m_GrandAnodeXPixelSize{0};
  double m_GrandAnodeYPixelSize{0};
  double m_GrandPmtAnodeEffectiveXPixelSizeInv{0};
  double m_GrandPmtAnodeEffectiveYPixelSizeInv{0};
  double m_GrandPmtMasterLateralSize{0};
  Int m_GrandNumPmtInRichModule{0};
  std::array<Int,2> m_NumGrandPmtInRowCol = {{}};
  Int m_Rich2TotNumGrandPmts{0};
  Int m_Rich2TotNumStdPmts{0};
  Int m_Rich2TotNumGrandModules{0};
  Int m_Rich2TotNumStdModules{0};

  std::vector<int> m_Rich2MixedModuleArrayColumnSize{3,0};
  std::vector<bool> m_ModuleIsWithGrandPMT;

  Rich::DAQ::PDPanelIndex m_maxPDCopyN{0};

};
