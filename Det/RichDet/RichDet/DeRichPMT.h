
#pragma once

// STL
#include <cmath>
#include <cstdint>
#include <array>

// DetDesc
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/IPVolume.h"

// LHCbKernel
#include "Kernel/ILHCbMagnetSvc.h"
#include "Kernel/RichSmartID.h"

// RichDet
#include "RichDet/DeRichPD.h"

// GSL
#include "gsl/gsl_math.h"

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
  DeRichPMT ( const std::string & name = "" );

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

  /** Converts a RichSmartID to a point on the anode in global coordinates.
   *  @param[in] smartID The RichSmartID for the PMT channel
   *  @return The detection point on the anode in global coordinates
   */
  Gaudi::XYZPoint detPointOnAnode ( const LHCb::RichSmartID& smartID ) const;

  inline int pmtModuleNumber() const noexcept { return m_moduleNum; }

  inline int pmtCopyNumber() const noexcept { return m_copyNum; }

  inline void setPmtLensFlag( const bool aflag ) noexcept { m_PmtLensFlag = aflag; }

  inline bool PmtLensFlag() const noexcept { return m_PmtLensFlag; }
  
  void setPmtIsGrandFlag( const bool isGrand );

  inline bool PmtIsGrand() const noexcept { return m_PmtIsGrand; }

  /// Set the PD ID
  inline void setPDSmartID( const LHCb::RichSmartID id ) noexcept { m_pdSmartID = id; }

  /// Access the PD ID
  inline LHCb::RichSmartID pdSmartID() const noexcept { return m_pdSmartID; }

private:

  // CRJ Use float not double to reduce memory footprint
  //using FType = double;
  using FType = float;

  using IPix = LHCb::RichSmartID::DataType;

private:

  DetectorElement * getFirstRich();

  StatusCode getPMTParameters();
  StatusCode initPMTQuantumEff();
  StatusCode updateGeometry();

private:

  inline Gaudi::XYZPoint getAnodeHitCoordFromPixelNum( const IPix fracPixelCol,
                                                       const IPix fracPixelRow ) const
  {
    const auto xh = ( (FType)fracPixelCol - m_PmtNumPixColFrac ) * m_PmtEffectivePixelXSize;
    const auto yh = ( (FType)fracPixelRow - m_PmtNumPixRowFrac ) * m_PmtEffectivePixelYSize;
    return { xh, yh, m_PmtAnodeHalfThickness };
  }
  
  inline  Gaudi::XYZPoint getAnodeHitCoordFromGrandPixelNum( const IPix fracPixelCol,
                                                             const IPix fracPixelRow ) const
  {
    const auto aXEffPixel = ( fracPixelCol==0 || fracPixelCol==(m_PmtNumPixCol-1) ?  
                              m_GrandPmtEdgePixelXSize : m_GrandPmtEffectivePixelXSize );
    const auto aYEffPixel = ( fracPixelRow==0 || fracPixelRow==(m_PmtNumPixRow-1) ? 
                              m_GrandPmtEdgePixelYSize : m_GrandPmtEffectivePixelYSize ); 
    const auto xh = ( (FType)fracPixelCol - m_PmtNumPixColFrac ) * aXEffPixel;
    const auto yh = ( (FType)fracPixelRow - m_PmtNumPixRowFrac ) * aYEffPixel;
    return { xh, yh, m_GrandPmtAnodeHalfThickness };
  }
  
  inline Gaudi::XYZPoint 
  getAnodeHitCoordFromMultTypePixelNum( const IPix fracPixelCol,
                                        const IPix fracPixelRow,
                                        const Rich::DetectorType rich ) const
  {
    return ( rich == Rich::Rich2 && PmtIsGrand() ?
             getAnodeHitCoordFromGrandPixelNum( fracPixelCol, fracPixelRow ) :
             getAnodeHitCoordFromPixelNum     ( fracPixelCol, fracPixelRow ) );
  }
  
  Gaudi::XYZPoint RichPmtLensReconFromPhCath( const Gaudi::XYZPoint & aPhCathCoord ) const;
  
private:

  // CRJ - To minimise memory footprint do not define data members
  //       that are not needed (outside of getPMTParameters() setup)
  //       Also, data members should be arranged so most commonly 
  //       accessed are listed first.

  FType m_zShift{0};

  FType m_PmtQwZSize{0};

  bool m_PmtLensFlag{false};
  FType m_PmtLensRoc2{0};
  FType m_PmtLensMagRatio{0};

  int m_moduleNum{0}; ///< Module number
  int m_copyNum{0};   ///< Copy number

  FType m_PmtEffectivePixelXSize{0};
  FType m_PmtEffectivePixelYSize{0};
  FType m_PmtAnodeHalfThickness{0};
  unsigned int m_PmtNumPixCol{0};
  unsigned int m_PmtNumPixRow{0};
  FType m_PmtNumPixColFrac{0};
  FType m_PmtNumPixRowFrac{0};

  FType m_GrandPmtEdgePixelXSize{0};
  FType m_GrandPmtEdgePixelYSize{0};
  FType m_GrandPmtEffectivePixelXSize{0};
  FType m_GrandPmtEffectivePixelYSize{0};
  FType m_GrandPmtAnodeHalfThickness{0};
  bool m_Rich2UseGrandPmt{false};
  bool m_Rich2UseMixedPmt{false};

  bool m_PmtIsGrand{false};

  /// The PMT Anode detector element
  IDetectorElement * m_dePmtAnode = nullptr;

  Rich::DetectorType m_rich = Rich::InvalidDetector;

  /// PD SmartID
  LHCb::RichSmartID m_pdSmartID;

};
