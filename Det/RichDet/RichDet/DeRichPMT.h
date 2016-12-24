
#pragma once

// Include files
// DetDesc
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/IPVolume.h"

// LHCbKernel
#include "Kernel/ILHCbMagnetSvc.h"

// RichDet
#include "RichDet/DeRichPD.h"

// GSL
#include "gsl/gsl_math.h"

// STL
#include <cmath>

// External declarations
extern const CLID CLID_DERichPMT;

//=============================================================================
/** @class DeRichPMT DeRichPMT.h RichDet/DeRichPMT.h
 *
 *  PMT Photodetector
 *
 *  @author Sajan Easo
 *  @date   2011-10-10
 */
//=============================================================================

class DeRichPMT : public DeRichPD
{

public:

  /// Standard constructor
  DeRichPMT(const std::string & name = "" );

  virtual ~DeRichPMT( ) = default; ///< Destructor

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  const CLID& clID() const override final { return classID(); }

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID& classID();

  /**
   * This is where most of the geometry is read and variables initialised
   * @return Status of initialisation
   * @retval StatusCode::FAILURE Initialisation failed, program should
   * terminate
   */
  StatusCode initialize() override final;

  // @brief Converts a RichSmartID to a point in global coordinates.
  bool detectionPoint ( const LHCb::RichSmartID smartID,
                        Gaudi::XYZPoint& detectPoint,
                        bool photoCathodeSide = false ) const override final;

  // Converts an x,y point from the anode to the photocathode in the
  // coordinate system of the PMT.
  bool detectionPoint ( const double fracPixelCol,
                        const double fracPixelRow,
                        Gaudi::XYZPoint& detectPoint,
                        const bool photoCathodeSide = true ) const override final;

  /** Converts a RichSmartID to a point on the anode in global coordinates.
   *  @param[in] smartID The RichSmartID for the PMT channel
   *  @return The detection point on the anode in global coordinates
   */
  Gaudi::XYZPoint detPointOnAnode ( const LHCb::RichSmartID& smartID ) const;

  void setPmtLensFlag( bool aflag )  { m_PmtLensFlag = aflag; }
  int pmtCopyNumber() const noexcept {  return m_number;}
  bool PmtLensFlag() const noexcept { return m_PmtLensFlag;}

  void setPmtIsGrandFlag(bool aflagG ) {m_PmtIsGrand  = aflagG ;}
  bool PmtIsGrand() const noexcept { return m_PmtIsGrand; }

private:

  StatusCode getPMTParameters();
  StatusCode initPMTQuantumEff();
  StatusCode updateGeometry();
  Gaudi::XYZPoint getAnodeHitCoordFromPixelNum( const double fracPixelCol,
                                                const double fracPixelRow ) const;
  
  Gaudi::XYZPoint getAnodeHitCoordFromGrandPixelNum( const double fracPixelCol,
                                                     const double fracPixelRow ) const;
  
  Gaudi::XYZPoint getAnodeHitCoordFromMultTypePixelNum( const double fracPixelCol,
                                                        const double fracPixelRow,
                                                        const LHCb::RichSmartID& smartID ) const;
  
  Gaudi::XYZPoint RichPmtLensReconFromPhCath( const Gaudi::XYZPoint & aPhCathCoord ) const;
  
private:
  
  IDetectorElement* m_dePmtAnode = nullptr; ///< The PMT Anode detector element
  int m_number{0};                    ///<Pmt number (should be the same as copy number)
  std::string  m_PmtQELocation;

  double m_PmtAnodeXSize{0};
  double m_PmtAnodeYSize{0};
  double m_PmtAnodeZSize{0};

  double m_PmtAnodeLocationInPmt{0};
  double m_PmtPixelXSize{0};
  double m_PmtPixelYSize{0};
  double m_PmtPixelGap{0};
  double m_PmtEffectivePixelXSize{0};
  double m_PmtEffectivePixelYSize{0};
  double m_PmtAnodeHalfThickness{0};
  double m_PmtNumPixCol{0};
  double m_PmtNumPixRow{0};
  double m_PmtQwZSize{0};
  double m_QwToAnodeZDist{0};
  bool m_PmtLensFlag{false};
  double m_PmtLensMagnificationRatio{0};
  double m_PmtLensRoc{0};

  double m_GrandPmtAnodeXSize{0};
  double m_GrandPmtAnodeYSize{0};
  double m_GrandPmtAnodeZSize{0};
  double m_GrandPmtPixelXSize{0};
  double m_GrandPmtPixelYSize{0};
  double m_GrandPmtPixelGap{0};
  double m_GrandPmtEdgePixelXSize{0};
  double m_GrandPmtEdgePixelYSize{0};
  double m_GrandPmtEffectivePixelXSize{0};
  double m_GrandPmtEffectivePixelYSize{0};
  double m_GrandPmtEdgePixelXDiff{0};
  double m_GrandPmtEdgePixelYDiff{0};
  double m_GrandPmtAnodeHalfThickness{0};
  bool m_Rich2UseGrandPmt{false};
  bool m_Rich2UseMixedPmt{false};
  double m_Rich1Rich2ZDivideLimit{0};
  int m_Rich2PmtArrayConfig{0};
  bool m_PmtIsGrand{false};

};
