
//=============================================================================
/** @file DeRichHPD.h
 *
 *  Header file for detector description class : DeRichHPD
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2006-09-19
 */
//=============================================================================

#pragma once

// STL
#include <cmath>
#include <array>
#include <vector>
#include <cstdint>

// DetDesc
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/IPVolume.h"

// LHCbKernel
#include "Kernel/ILHCbMagnetSvc.h"

// RichDet
#include "RichDet/DeRichPD.h"

// External declarations
extern const CLID CLID_DERichHPD;

//=============================================================================
/** @class DeRichHPD RichDet/DeRichHPD.h
 *
 * Class for generic info about HPDs
 *
 * @author Antonis Papanestis a.papanestis@rl.ac.uk
 * @date   2006-09-19
 */
//=============================================================================
class DeRichHPD : public DeRichPD
{

public:

  /**
   * Constructor for this class
   */
  explicit DeRichHPD( const std::string & name = "" ) : DeRichPD ( name ) { }

  /**
   * Default destructor
   */
  virtual ~DeRichHPD();

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

public:

  /**
   * Get the point on the centre of the HPD window on the inside surface
   * @return Inside window surface centre point
   */
  Gaudi::XYZPoint windowCentreIn() const
  {
    return geometry()->toGlobal(m_pvWindow->toMother({0,0,m_winInR}) );
  }

  /**
   * Get the point on the centre of the HPD window on the inside surface without misalignment
   * @return Inside window surface centre point without misalignment
   */
  Gaudi::XYZPoint windowCentreInIdeal() const
  {
    return ( geometry()->toGlobalMatrixNominal() *
             m_pvWindow->toMother({0,0,m_winInR}) );
  }

  /** Get the point on the centre of the HPD window on the inside surface in the mother
   *  (panel) coodinate system
   *  @return Inside window surface centre point in panel system
   */
  inline const Gaudi::XYZPoint hpdWinInsideCentreInMother() const noexcept
  {
    return m_windowInsideCentreMother;
  }

  /**
   * Get the point on the centre of the HPD window on the outside surface
   * @return Inside window surface centre point
   */
  Gaudi::XYZPoint windowCentreOut() const
  {
    return geometry()->toGlobal( m_pvWindow->toMother({0,0,m_winOutR}) );
  }

  // (Scalar) Converts a RichSmartID to a point in global coordinates.
  bool detectionPoint ( const LHCb::RichSmartID smartID,
                        Gaudi::XYZPoint& detectPoint,
                        bool photoCathodeSide = false ) const override final;

  // (SIMD) Converts RichSmartIDs to an SIMD point in global coordinates.
  DeRichPD::SIMDFP::MaskType 
  detectionPoint ( const SmartIDs& smartID,
                   SIMDPoint& detectPoint,
                   bool photoCathodeSide = false ) const override final;

  /** Converts a RichSmartID to a point on the anode in global coordinates.
   *  @param[in] smartID The RichSmartID for the HPD channel
   *  @return The detection point on the anode in global coordinates
   */
  inline Gaudi::XYZPoint detPointOnAnode( const LHCb::RichSmartID smartID ) const
  {
    return ( m_deSiSensor->geometry()->toGlobal( pointOnSilicon(smartID) ) );
  }

  /** Converts the given RichSmartID to the position on the silicon wafer,
   *  in the coordinate system of the wafer
   *  @param[in] smartID The RichSmartID channel identifier
   *  @return The position on the wafer
   */
  inline Gaudi::XYZPoint pointOnSilicon ( const LHCb::RichSmartID smartID ) const
  {
    return { xOnSilicon(smartID), yOnSilicon(smartID), 0.0 };
  }

  /** Get the tranformation from the HPD panel coordinate system to the HPD quartz
   *  @return The HPD panel to the HPD quartz tranformation
   */
  inline const Gaudi::Transform3D& fromPanelToHPDWindow ( ) const noexcept
  {
    return m_fromPanelToWindow;
  }

  /** Get the tranformation from the HPD quartz window to the HPD
   *  @return The HPD window to HPD tranformation
   */
  inline const Gaudi::Transform3D& fromHPDWindowToHPD ( ) const noexcept
  {
    return m_fromWindowToHPD;
  }

  /** Get the tranformation from the HPD quartz window to the global LHCb coordinate system
   *  @return The HPD window to LHCb global tranformation
   */
  inline const Gaudi::Transform3D& fromHPDWindowToGlobal ( ) const noexcept
  {
    return m_fromWindowToGlobal;
  }

  /** Get the tranformation from the HPD coordinate system to the panel coordinate
   *  system including misalignment
   *  @return The HPD to panel tranformation
   */
  inline const Gaudi::Transform3D& fromHPDToPanel ( ) const noexcept
  {
    return m_fromHPDToPanel;
  }

  /** Get the solid for the HPD window (for intersection purposes)
   *  @return The quartz window solid
   */
  inline const ISolid* HPDWindowSolid ( ) const noexcept
  {
    return m_windowSolid;
  }

  /** Test for shadowing by the kapton shield of the given DeRichHPD
   *  @param[in] pInPanel Position in the HPD panel coordinate system
   *  @param[in] vInPanel Direction vedctor in the HPD panel coordinate system
   *  @return boolean indicating if the given direction intersects the HPD kapton shield
   *  @retval TRUE  Does intersect
   *  @retval FALSE Does not intersect
   */
  bool testKaptonShadowing( const Gaudi::XYZPoint&  pInPanel,
                            const Gaudi::XYZVector& vInPanel ) const;

  /** Intersect a given direction, from a given point, with the outer surface
   *  of the HPD window in the global LHCb frame
   *
   *  @param[in]  position      The start point to use for the ray tracing
   *  @param[in]  direction     The direction to ray trace from the start point
   *  @param[out] intersection  The intersection point of the direction with the plane
   *
   *  @return Boolean indicating the status of the ray tracing
   *  @retval true  Ray tracing was successful
   *  @retval false Ray tracing was unsuccessful
   */
  bool intersectEntryWindow ( const Gaudi::XYZPoint& position,
                              const Gaudi::XYZVector& direction,
                              Gaudi::XYZPoint& intersection ) const;

public: // types

  /// Field polarity
  enum FieldPolarity : uint8_t { Down = 0, Up = 1 };

  /// Number of field polarities (just for explicitness...)
  static const uint8_t NPolarities = 2;

public:

  /** Retrieves the demagnification interpolation function for the HPD R coordinate.
   *  For a given R on the HPD cathode returns the R on the anode.
   *  Only valid values for field are 0 (DOWN) or 1 (UP)
   *  @return A pointer to the demagnification function for R(R)
   */
  inline const Rich::TabulatedFunction1D*
  demagnification_RtoR( const FieldPolarity field = FieldPolarity::Down ) const noexcept
  {
    return &m_demagMapR[ field ];
  }

  /** Retrieves the demagnification interpolation function for the HPD phi coordinate.
   *  For a given R on the HPD cathode returns the phi on the anode.
   *  Only valid values for field are 0 (DOWN) or 1 (UP)
   *  @return A pointer to the demagnification function for phi(R)
   */
  inline const Rich::TabulatedFunction1D*
  demagnification_RtoPhi( const FieldPolarity field = FieldPolarity::Down ) const noexcept
  {
    return &m_demagMapPhi[ field ];
  }

  /** Retrieves the magnification interpolation function for the HPD R coordinate.
   *  For a given R on the HPD anode returns the R on the cathode.
   *  Only valid values for field are 0 (DOWN) or 1 (UP)
   *  @return A pointer to the magnification function for R(R)
   */
  inline const Rich::TabulatedFunction1D*
  magnification_RtoR( const FieldPolarity field = FieldPolarity::Down ) const noexcept
  {
    return &m_magMapR[ field ];
  }

  /** Retrieves the magnification interpolation function for the HPD phi coordinate.
   *  For a given R on the HPD anode returns the phi on the cathode.
   *  Only valid values for field are 0 (DOWN) or 1 (UP)
   *  @return A pointer to the magnification function for phi(R)
   */
  inline const Rich::TabulatedFunction1D*
  magnification_RtoPhi( const FieldPolarity field = FieldPolarity::Down ) const noexcept
  {
    return &m_magMapPhi[ field ];
  }

private: // functions

  /** Returns the silicon x coordinate for the given RichSmartID
   *  @param smartID The RichSmartID
   *  @return The x coordinate
   */
  inline double xOnSilicon( const LHCb::RichSmartID smartID ) const
  {
    return ( ( (double)smartID.pixelCol() + 0.5 ) * m_pixelSize ) - m_siliconHalfLengthX;
  }

  /** Returns the silicon y coordinate for the given RichSmartID
   *  @param smartID The RichSmartID
   *  @return The y coordinate
   */
  inline double yOnSilicon( const LHCb::RichSmartID smartID ) const
  {
    return m_siliconHalfLengthY - ( ( (double)smartID.pixelRow() + 0.5 ) * m_pixelSize );
  }

  /// parameterised extra radius for the defraction in the HPD window
  inline double extraRadiusForRefraction( const double r ) const
  {
    return ( m_refPs[0] + 
             ( ( ( ( m_refPs[3] * r ) + m_refPs[2] ) * r + m_refPs[1] ) * r ) );
  }

  /// Get parameters from Rich1
  StatusCode getParameters();

  /// Update the cached parameters based on geometry
  StatusCode updateGeometry();

  /// Update the magnification and demagnification information
  StatusCode updateDemagProperties();

  /// Update the cached field parameters
  StatusCode updateFieldParams();

  /// go from a point on silicon to a point on the photo-cathode with magnet ON
  bool magnifyToGlobalMagnetON( Gaudi::XYZPoint& detectPoint,
                                const bool photoCathodeSide ) const;

  /// go from a point on silicon to a point on the photo-cathode with magnet OFF
  bool magnifyToGlobalMagnetOFF( Gaudi::XYZPoint& detectPoint,
                                 const bool photoCathodeSide ) const;

  /// Initialise the interpolators for demagnification (cathode to anode)
  StatusCode fillHpdDemagTable( const FieldPolarity field );

  /// Initialise the interpolators for magnification (anode to cathode)
  StatusCode fillHpdMagTable( const FieldPolarity field );

  /// Initialise the HPD quantum eff function
  StatusCode initHpdQuantumEff();

  /// Access magnetic field service on demand
  inline ILHCbMagnetSvc * magSvc() const noexcept { return m_magFieldSvc; }

  /// Load the Magnetic field service
  void loadMagSvc();

  /** It returns the rotation angle \Delta\phi [rad] as a function of the
   * radial entrance
   * window coordinate r [mm] and the axial magnetic flux density B [G].
   * first calculate the coefficients a, b, c from the
   * value of the axial magnetic flux density
   * these are called \Delta\phi_0, \Delta\phi_2, \Delta\phi_3 in the
   * referenced thesis at the beginning
   */
  double Delta_Phi( const double r, const double B );

  /** function mag returns the r coordinate given rho and B as arguments
   * coefficients of the fits for the dependence of the mag law
   * from the magnetic field
   */
  double mag( const double rho, const double B );

  /**
   * Radial mapping function and rotation function
   * from Gianluca Aglieri Rinella and Ann Van Lysebetten
   * for the description and correction of the magnetic distortions
   * induced in the axially symmetric case on the HPD detector.
   * See for explanation and reference
   * CERN-THESIS-2005-060 on pages 92-93
   * http://cdsweb.cern.ch/search.py?recid=916449
   *
   * function demag returns the rho coordinate given r and B as arguments
   * r [mm] is the radial coordinate of the led on the entrance window
   * B [G] is the magnetic flux density field (axial)
   * rho [mm] is the radial coordinate on the anode plane from the centre of
   * the cathode image
   */
  double demag( const double r, const double B );

private: // data

  // cached field parameters

  /// Field polarity
  FieldPolarity m_field{Down};

  /// Is field ON or OFF
  bool m_isFieldON{true};

private:

  /// version of MDMS corrections, for both field polarities
  std::array<int,NPolarities> m_MDMS_version = {{0,0}};

  IDetectorElement* m_deSiSensor = nullptr;  ///< The silicon sensor detector element

  const IPVolume* m_pvWindow = nullptr;      ///< The pv for the HPD quartz window
  const ISolid* m_windowSolid = nullptr;     ///< The HPD window solid

  const ISolid* m_kaptonSolid = nullptr;     ///< Pointer to the kapton solid

  int m_number{0};               ///< HPD number (should be the same as copy number)

  double m_winInR{0};            ///< Inner radius of HPD window squared
  double m_winOutR{0};           ///< Outter radius of HPD window squared
  double m_winInRsq{0};          ///< Inner radius of HPD window squared
  double m_winOutRsq{0};         ///< Outter radius of HPD window squared

  /// The active HPD window radius (photocathode coverage)
  double m_activeRadius{0};

  double m_pixelSize{0};           ///< The pixel size on the silicon sensor
  double m_siliconHalfLengthX{0};  ///< Si sensor half size in x
  double m_siliconHalfLengthY{0};  ///< Si sensor half size in y
  double m_siAnodeRCheck{0};       ///< Max Anode radius on Si sensor

  /// The demagnification factor of the HPD. Element [0] is the linear
  /// term, and element[1] the non-linear term for small corrections.
  std::array<double,2> m_deMagFactor = {{0,0}};
  double m_magnificationCoef1{0};
  double m_magnificationCoef2{0};

  /// Interpolated function for HPD R for demagnification
  std::array< Rich::TabulatedFunction1D, NPolarities > m_demagMapR;
  /// Interpolated function for HPD phi for demagnification
  std::array< Rich::TabulatedFunction1D, NPolarities > m_demagMapPhi;
  /// Interpolated function for HPD R for magnification
  std::array< Rich::TabulatedFunction1D, NPolarities > m_magMapR;
  /// Interpolated function for HPD phi for magnification
  std::array< Rich::TabulatedFunction1D, NPolarities > m_magMapPhi;

  /// Demagnification parameters condition
  std::vector< SmartRef<Condition> > m_demagConds;

  std::vector<double> m_refPs = {0,0,0,0}; ///< refraction parameters for quartz window

  /// Flag to indicate the full treatment of magnetic distortions should be performed
  bool m_UseHpdMagDistortions{true};

  /// Turns on the use of a test field map
  bool m_UseBFieldTestMap{false};

  /// magnitude of the longitudinal B field
  double m_LongitudinalBField{0};

  // Cached parameters for speed reasons.
  Gaudi::Transform3D m_SiSensorToHPDMatrix; ///< silicon to HPD transform
  Gaudi::Transform3D m_fromWindowToGlobal;  ///< window to global coord system transform
  Gaudi::Transform3D m_fromPanelToWindow;   ///< HPD panel to HPD window coord system transform
  Gaudi::Transform3D m_fromPanelToKapton;   ///< HPD panel to kapton coord system transform
  Gaudi::Transform3D m_fromWindowToHPD;     ///< window to global coord system transform
  Gaudi::Transform3D m_fromHPDToPanel;      ///< HPD to HPD Panel transform

  /// The centre of the HPD window (inside) in the mother (panel) coordinate system
  Gaudi::XYZPoint m_windowInsideCentreMother;

  /// Rotional centre for MDMS corrections
  Gaudi::XYZVector m_MDMSRotCentre;

  /// Centre of curvature of entrance window outer surface in global frame
  Gaudi::XYZVector m_windowCoCglobal;

  /// pointer to the magnetic field service
  ILHCbMagnetSvc * m_magFieldSvc = nullptr;

};
