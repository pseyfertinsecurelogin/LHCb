
#pragma once

// Include files

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
  const DeRichPD* dePD( const Rich::DAQ::HPDCopyNumber PDNumber ) const override final;

  // Converts a Gaudi::XYZPoint in global coordinates to a RichSmartID.
  bool smartID( const Gaudi::XYZPoint& globalPoint,
                LHCb::RichSmartID& id ) const override final;

  // Returns the intersection point with the detector plane given a vector and a point.
  LHCb::RichTraceMode::RayTraceResult
  detPlanePoint( const Gaudi::XYZPoint& pGlobal,
                 const Gaudi::XYZVector& vGlobal,
                 Gaudi::XYZPoint& hitPosition,
                 LHCb::RichSmartID& smartID,
                 const LHCb::RichTraceMode mode ) const override final;

  // Returns the intersection point with an HPD window given a vector and a point.
  LHCb::RichTraceMode::RayTraceResult
  PDWindowPoint( const Gaudi::XYZVector& vGlobal,
                 const Gaudi::XYZPoint& pGlobal,
                 Gaudi::XYZPoint& windowPointGlobal,
                 LHCb::RichSmartID& smartID,
                 const LHCb::RichTraceMode mode ) const override final;

  // Adds to the given vector all the available readout channels in this HPD panel
  bool readoutChannelList( LHCb::RichSmartID::Vector& readoutChannels ) const override final;

  /// Get tge sensitivevolumeID
  int sensitiveVolumeID( const Gaudi::XYZPoint& globalPoint ) const override final;
  
  /** Converts a RichSmartID to a point on the anode in global coordinates.
   *  @param[in] smartID The RichSmartID for the PMT channel
   *  @return The detection point on the anode in global coordinates
   */
  Gaudi::XYZPoint detPointOnAnode( const LHCb::RichSmartID smartID ) const;

  /// Returns the PD number for the given RichSmartID
  Rich::DAQ::HPDCopyNumber pdNumber( const LHCb::RichSmartID& smartID ) const override;

  // The maximum PD copy number for this panel
  Rich::DAQ::HPDCopyNumber maxPdNumber() const override;

private:

  /// Returns the PD number for the given RichSmartID
  inline Rich::DAQ::HPDCopyNumber _pdNumber( const LHCb::RichSmartID& smartID ) const 
  {
    return Rich::DAQ::HPDCopyNumber( smartID.rich()  == rich() && 
                                     smartID.panel() == side() ?
                                     ( smartID.pdCol() * m_NumPmtInRichModule ) + smartID.pdNumInCol() :
                                     nPDs() + 1 );
  }

  const DeRichPMT* dePMT( const Rich::DAQ::HPDCopyNumber PmtCopyNumber ) const;

  std::vector<int> getPmtRowColFromPmtNum( const int aPmtNum );
  std::vector<int> getGrandPmtRowColFromPmtNum( const int aPmtNum );

  int PmtModuleNumInPanelFromModuleNum( const int aMnum ) const;
  int PmtModuleNumInPanelFromModuleNumAlone( const int aMnum ) const;
  std::vector<int> PmtModuleRowColFromModuleNumInPanel( const int aMnum );

  // Set the rich panel and side
  StatusCode setRichPanelAndSide();

  /// Update cached information on geometry changes
  StatusCode geometryUpdate();
  StatusCode getPanelGeometryInfo();
  int getPmtModuleNumFromRowCol(int MRow, int MCol ) const;
  std::vector<int> findPMTArraySetup(const Gaudi::XYZPoint& inPanel) const;
  StatusCode setRichPmtSmartID(const std::vector<int>& aPmtHitChannel,
                               LHCb::RichSmartID& ID ) const;
  DetectorElement* getFirstDeRich() const;
  int getNumModulesInThisPanel() ;
  int getPmtNumFromRowCol(int PRow, int PCol) const;
  int getGrandPmtNumFromRowCol(int PRow, int PCol) const;

  bool isInPmtAnodeLateralAcc(const Gaudi::XYZPoint& aPointInPmtAnode , const bool bFlagGrandPMT  ) const;
  bool isInPmt(const Gaudi::XYZPoint& aPointInPmt, const bool aFlagGrandPMT ) const;
  bool isInPmtPanel(const Gaudi::XYZPoint& aPointInPanel ) const;
  StatusCode getPanelInterSection ( const Gaudi::XYZPoint& pGlobal,
                                    const Gaudi::XYZVector& vGlobal ,
                                    Gaudi::XYZPoint& panelIntersection,
                                    Gaudi::XYZPoint& panelIntersectionGlobal ) const;

  /// Setup for Lens Flag
  void Rich1SetupPMTModulesWithLens();
  int getLensPmtNumFromRowCol(int PRow, int PCol ) const;
  /// setup flags for grand Modules
  int getModuleCopyNumber ( const std::string& aModuleName);
  void  RichSetupMixedSizePmtModules();

  bool isCurrentPmtModuleWithLens(const int aModuleNum);
  bool isCurrentPmtWithLens(const int aPMTNum) ;
  Gaudi::XYZPoint DemagnifyFromLens(const Gaudi::XYZPoint& aLensPoint) const ;
  bool ModuleIsWithGrandPMT(const int aModuleNum ) const 
  {
    return (( aModuleNum >=0 && aModuleNum < (int) m_ModuleIsWithGrandPMT.size() ) ?
            m_ModuleIsWithGrandPMT[aModuleNum] : false);
  }

private:

  using IDeElemV = std::vector<IDetectorElement*>;
  using IDRiPMTV = std::vector<DeRichPMT*>;

  /// Container for the PMT Modules as Det Elements
  IDeElemV m_DePMTModules{1,nullptr};

  ///< Container for the PMTs
  std::vector<IDRiPMTV> m_DePMTs{1,IDRiPMTV(2,nullptr)};

  /// Container for the PMTAnodes as Det Elements
  std::vector<IDeElemV> m_DePMTAnodes{1,IDeElemV(2,nullptr)};

  std::vector<double> m_PmtModulePlaneHalfSizeR1;
  std::vector<double> m_PmtModulePlaneHalfSizeR2;
  double m_PmtModulePitch{0};
  double m_PmtPitch{0};
  std::vector<int> m_RichPmtNumModulesInRowCol{4,0};
  std::vector<int> m_RichPmtModuleCopyNumBeginPanel{4,0};
  std::vector<int> m_RichPmtModuleCopyNumEndPanel{4,0};
  std::vector<double> m_RichPmtModuleActiveAreaHalfSize{2,0};
  std::vector<int> m_NumPmtInRowCol{2,0};
  std::vector<int> m_NumPmtModuleInRich{4,0};
  int m_NumPmtInRichModule{0};
  int m_totNumPmtModuleInRich{0};
  double m_PmtAnodeXSize{0};
  double m_PmtAnodeYSize{0};
  double m_PmtPixelGap{0};
  int m_PmtPixelsInRow{0};
  int m_PmtPixelsInCol{0};
  double m_PmtAnodeXEdge{0};
  double m_PmtAnodeYEdge{0};
  double m_AnodeXPixelSize{0};
  double m_AnodeYPixelSize{0};
  double m_PmtAnodeEffectiveXPixelSize{0};
  double m_PmtAnodeEffectiveYPixelSize{0};
  double m_PmtMasterLateralSize{0};
  double m_RichPmtQuartzThickness{0};
  double m_RichPmtQuartzLocalZInPmt{0};

  int m_Rich1PmtLensPresence{0};
  std::vector<int>  m_Rich1PmtLensModuleCol;
  std::vector<bool> m_RichPmtModuleLensFlag;
  int m_totNumPmtModuleInRich1{0};

  Gaudi::Plane3D m_localPlane;
  Gaudi::XYZVector m_localPlaneNormal;
  // Gaudi::Plane3D m_localPlane2;
  int m_Rich1TotNumPmts{0};
  int m_Rich2TotNumPmts{0};
  Gaudi::Plane3D m_detectionPlane_exterior;
  // Access info related to PMT Lens flag

  double m_PmtMasterWithLensLateralSize{0};
  double m_PmtModuleWithLensPitch{0};
  double m_PmtLensPitch{0};
  std::vector<double> m_Rich1PmtPanelWithLensXSize{2,0};
  std::vector<double> m_Rich1PmtPanelWithLensYSize{2,0};
  std::vector<int>    m_Rich1PmtPanelWithLensColSize{2,0};
  std::vector<int>    m_RichNumLensPmtinModuleRowCol{2,0};
  double m_Rich1LensMagnificationFactor{0};

  bool  m_Rich2UseGrandModule{false};
  int   m_Rich2ArrayConfig{0};
  bool  m_Rich2UseMixedModule{false};

  std::vector<double> m_GrandPmtModulePlaneHalfSizeR2;
  std::vector<double> m_MixedPmtModulePlaneHalfSizeR2;
  std::vector<double> m_MixedStdPmtModulePlaneHalfSizeR2;
  double m_GrandPmtModulePitch{0};
  std::vector<double> m_RichGrandPmtModuleActiveAreaHalfSize{2,0};
  double m_GrandPmtPitch{0};
  double m_GrandPmtAnodeXSize{0};
  double m_GrandPmtAnodeYSize{0};
  double m_GrandPmtPixelGap{0};
  int  m_GrandPmtPixelsInRow{0};
  int  m_GrandPmtPixelsInCol{0};
  double m_GrandPmtAnodeXEdge{0};
  double m_GrandPmtAnodeYEdge{0};
  double m_GrandAnodeXPixelSize{0};
  double m_GrandAnodeYPixelSize{0};
  double m_GrandPmtAnodeEffectiveXPixelSize{0};
  double m_GrandPmtAnodeEffectiveYPixelSize{0};
  double m_GrandPmtMasterLateralSize{0};
  int m_GrandNumPmtInRichModule{0};
  std::vector<int> m_NumGrandPmtInRowCol{2,0};
  int m_Rich2TotNumGrandPmts{0};
  int m_Rich2TotNumStdPmts{0};
  int m_Rich2TotNumGrandModules{0};
  int m_Rich2TotNumStdModules{0};

  std::vector<int> m_Rich2MixedModuleArrayColumnSize{3,0};
  std::vector<bool> m_ModuleIsWithGrandPMT;

  Rich::DAQ::HPDCopyNumber m_maxPDCopyN{0};

};
