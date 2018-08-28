
//----------------------------------------------------------------------------
/** @file DeRichHPDPanel.h
 *
 *  Header file for detector description class : DeRichHPDPanel
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2004-06-18
 */
//----------------------------------------------------------------------------

#pragma once

// STL
#include <array>
#include <sstream>

// Gaudi
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// DetDesc
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/ISolid.h"

// RichDet
#include "RichDet/DeRich.h"
#include "RichDet/DeRichHPD.h"
#include "RichDet/DeRichLocations.h"
#include "RichDet/DeRichPDPanel.h"

//----------------------------------------------------------------------------
/** @class DeRichHPDPanel RichDet/DeRichHPDPanel.h
 *
 * DeRichHPDPanel provides geometry information for the HPD panels, converts
 * RichSmartIDs to space points and finds intersections with the HPDs
 *
 * @author Antonis Papanestis a.papanestis@rl.ac.uk
 *
 * @todo See if the HPD planes in DeRichHPDPanel::detPlanePoint and
 *       DeRichHPDPanel::PDWindowPoint could be made the same. At the moment the
 *       hits obtained from these methods are on slightly different planes.
 */
//----------------------------------------------------------------------------
class DeRichHPDPanel : public DeRichPDPanel
{

public:

  /**
   * Constructor for this class
   */
  DeRichHPDPanel( const std::string &name = "" );

  /**
   * Default destructor
   */
  virtual ~DeRichHPDPanel() = default;

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  const CLID &clID() const override final { return classID(); }

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID &classID();

  /**
   * This is where most of the geometry is read and variables initialised
   *
   * @return Status of initialisation
   * @retval StatusCode::FAILURE Initialisation failed, program should
   * terminate
   */
  StatusCode initialize() override final;

public:

  // Converts a Gaudi::XYZPoint in global coordinates to a RichSmartID.
  bool smartID( const Gaudi::XYZPoint &globalPoint, LHCb::RichSmartID &id ) const override final;

  /** Converts a RichSmartID to a point on the anode in global coordinates.
   *  @param[in] smartID      The HPD channel ID
   *  @return The detection anode point in global coordinates
   */
  Gaudi::XYZPoint detPointOnAnode( const LHCb::RichSmartID &smartID ) const;

  // Returns the scalar intersection point with an HPD window given a vector and a point.
  LHCb::RichTraceMode::RayTraceResult
  PDWindowPoint( const Gaudi::XYZPoint &   pGlobal,
                 const Gaudi::XYZVector &  vGlobal,
                 Gaudi::XYZPoint &         windowPointGlobal,
                 LHCb::RichSmartID &       smartID,
                 const DeRichPD *&         pd,
                 const LHCb::RichTraceMode mode ) const override final;

  // Returns the scalar intersection point with the detector plane given a vector and a point.
  LHCb::RichTraceMode::RayTraceResult
  detPlanePoint( const Gaudi::XYZPoint &   pGlobal,
                 const Gaudi::XYZVector &  vGlobal,
                 Gaudi::XYZPoint &         hitPosition,
                 LHCb::RichSmartID &       smartID,
                 const DeRichPD *&         pd,
                 const LHCb::RichTraceMode mode ) const override final;

  // Returns the SIMD intersection point with an HPD window given a vector and a point.
  SIMDRayTResult::Results PDWindowPointSIMD( const SIMDPoint &         pGlobal,
                                             const SIMDVector &        vGlobal,
                                             SIMDPoint &               hitPosition,
                                             SIMDRayTResult::SmartIDs &smartID,
                                             SIMDRayTResult::PDs &     PDs,
                                             const LHCb::RichTraceMode mode ) const override final;

  // Returns the SIMD intersection point with the detector plane given a vector and a point.
  SIMDRayTResult::Results detPlanePointSIMD( const SIMDPoint &         pGlobal,
                                             const SIMDVector &        vGlobal,
                                             SIMDPoint &               hitPosition,
                                             SIMDRayTResult::SmartIDs &smartID,
                                             SIMDRayTResult::PDs &     PDs,
                                             const LHCb::RichTraceMode mode ) const override final;

  /// Access the DeRichPD object for a given PD RichSmartID
  const DeRichPD *dePD( const LHCb::RichSmartID pdID ) const override final;

  /// Returns the detector element for the given PD number
  const DeRichPD *dePD( const Rich::DAQ::PDPanelIndex PDNumber ) const override final;

  /// Returns the detector element for the given PD number
  inline const DeRichHPD *deHPD( const Rich::DAQ::PDPanelIndex HPDNumber ) const
  {
    const DeRichHPD *deHPD = ( HPDNumber.data() < nPDs() ? m_DeHPDs[HPDNumber.data()] : nullptr );
#ifndef NDEBUG
    if ( !deHPD )
    {
      std::ostringstream mess;
      mess << "Inappropriate HPDNumber : " << HPDNumber;
      throw GaudiException( mess.str(), "*DeRichHPDPanel*", StatusCode::FAILURE );
    }
#endif
    return deHPD;
  }

  /// Adds to the given vector all the available readout channels in this HPD panel
  bool readoutChannelList( LHCb::RichSmartID::Vector &readoutChannels ) const override final;

  /// sensitive volume identifier
  int sensitiveVolumeID( const Gaudi::XYZPoint &globalPoint ) const override final;

  /// Returns the number for the given RichSmartID
  Rich::DAQ::PDPanelIndex pdNumber( const LHCb::RichSmartID &smartID ) const override final;

  /// The maximum PD number for this panel
  Rich::DAQ::PDPanelIndex maxPdNumber() const override final;

private: // methods

  /// Returns the PD number for the given RichSmartID
  inline Rich::DAQ::PDPanelIndex _pdNumber( const LHCb::RichSmartID &smartID ) const
  {
    return Rich::DAQ::PDPanelIndex( smartID.rich() == rich() && smartID.panel() == side() ?
                                      smartID.pdCol() * nPDsPerCol() + smartID.pdNumInCol() :
                                      nPDs() + 1 );
  }

  /** Finds the HPD row and column that corresponds to the x,y coordinates
   *  of a point in the panel. The row and column are retuned in the smartID.
   *
   * @return Status of request
   * @retval true   HPD is found
   * @retval false  The point is outside the coverage of the HPDs.
   */
  bool findHPDColAndPos( const Gaudi::XYZPoint &inPanel, LHCb::RichSmartID &id ) const;

  /// Check HPD panel acceptance
  LHCb::RichTraceMode::RayTraceResult checkPanelAcc( const Gaudi::XYZPoint &point ) const;

  /// Update cached information on geometry changes
  StatusCode geometryUpdate();

private: // data

  double m_HPDPitch { 0 };           ///< distance between HPDs
  double m_HPDColPitch { 0 };        ///< distance between HPD columns
  double m_OneOverHPDPitch { 0 };    ///< 1 / distance between HPDs (cached for speed)
  double m_OneOverHPDColPitch { 0 }; ///< 1 / distance between HPD columns (cached for speed)

  unsigned int m_pixelRows { 0 };    ///< Number of pixel rows
  unsigned int m_pixelColumns { 0 }; ///< Number of pixel columns

  /// The active HPD window radius (photocathode coverage) Squared
  double m_activeRadiusSq { 0 };

  double m_pixelSize { 0 };          ///< The pixel size on the silicon sensor
  double m_subPixelSize { 0 };       ///< The size of the subpixel (Alice mode)
  double m_siliconHalfLengthX { 0 }; ///< Half size (x) of silicon sensor
  double m_siliconHalfLengthY { 0 }; ///< Half size (y) of silicon sensor

  Gaudi::Plane3D   m_localPlane;       ///< detection plane in PDPanel coordinates
  Gaudi::XYZVector m_localPlaneNormal; ///< The normal vector of det plane in local coordinates

  /** Plane2 is defined going through all HPDs at the edge of photocathode coverage on
   *  HPD window. It is used for HPD row/column purposes
   *  This plane is parrallel to m_localPlane, thus share the same normal vector
   */
  Gaudi::Plane3D m_localPlane2;
  double         m_localPlaneZdiff { 0 }; ///< Shift in Z between localPlane2 and localPlane

  double                  m_panelColumnSideEdge { 0 }; ///< Edge of the panel along the columns
  std::array< double, 2 > m_panelStartColPosEvenOdd = {
    { 0, 0 }
  }; ///< Bottom/Start point of the [even,odd] HPD columns
  double m_panelStartColPos {
    0
  }; ///< abs max of even and odd start points used as the edge across columns

  std::vector< IDetectorElement * > m_DeSiSensors; ///< Container for the Si sensors as Det Elements

  std::vector< DeRichHPD * > m_DeHPDs; ///< Container for the HPDs as Det Elements

  /// DeRichSystem pointer
  DeRichSystem *m_deRichS = nullptr;
};

//=========================================================================
//  convert a SmartID to a point on the anode (global coord system)
//=========================================================================
inline Gaudi::XYZPoint
DeRichHPDPanel::detPointOnAnode( const LHCb::RichSmartID &smartID ) const
{
  return deHPD( _pdNumber( smartID ) )->detPointOnAnode( smartID );
}

//=========================================================================
// Check HPD panel acceptance
//=========================================================================
inline LHCb::RichTraceMode::RayTraceResult
DeRichHPDPanel::checkPanelAcc( const Gaudi::XYZPoint &point ) const
{
  const auto uv = ( Rich::Rich1 == rich() ? std::make_pair( point.y(), point.x() ) :
                                            std::make_pair( point.x(), point.y() ) );
  return ( ( fabs( uv.first ) >= fabs( m_panelColumnSideEdge ) ||
             fabs( uv.second ) >= m_panelStartColPos ) ?
             LHCb::RichTraceMode::RayTraceResult::OutsidePDPanel :
             LHCb::RichTraceMode::RayTraceResult::InPDPanel );
}
