
/** @file DeRichBeamPipe.h
 *
 *  Header file for detector description class : DeRichBeamPipe
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2006-11-27
 */

#pragma once

// STL
#include <array>
#include <memory>

// Utils
#include "RichUtils/RichSIMDTypes.h"

// DetDesc
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/ISolid.h"
#include "DetDesc/SolidCons.h"

// Local
#include "RichDet/DeRichBase.h"

// External declarations
extern const CLID CLID_DERichBeamPipe;

/** @class DeRichBeamPipe RichDet/DeRichBeamPipe.h
 *
 * Class for generic description of the BeamPipe (equivalent) in the Rich
 *
 * @author Antonis Papanestis a.papanestis@rl.ac.uk
 * @date   2006-11-27
 */
class DeRichBeamPipe : public DeRichBase
{

private:

  /// Internal representation of a line
  // using LINE = Gaudi::Math::Line<Gaudi::XYZPoint,Gaudi::XYZVector>;

public:

  /// Enum describing the various possible types of intersection
  enum BeamPipeIntersectionType
  {
    NoIntersection = 0, ///< Did not intersect the beam pipe at all
    FrontAndBackFace,   ///< Entered via the front face and left via the back face (i.e. totally
                        ///< inside the beampipe)
    FrontFaceAndCone,   ///< Entered via the front face and left via the cone surface
    BackFaceAndCone,    ///< Entered via the cone surafece and left via the backface
    ConeOnly            ///< Entered via the cone surafece and left via the cone surface
  };

public:

  /**
   * Constructor for this class
   */
  DeRichBeamPipe( const std::string &name = "" );

  /**
   * Default destructor
   */
  virtual ~DeRichBeamPipe() = default;

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  const CLID &clID() const override { return classID(); }

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID &classID();

  /**
   * This is where most of the geometry is read and variables initialised
   * @return Status of initialisation
   * @retval StatusCode::FAILURE Initialisation failed, program should
   * terminate
   */
  StatusCode initialize() override;

public:

  /**
   *  Finds the entry and exit points of the beam pipe equivalent (central
   *  tube in Rich2). For boolean solids
   *  this is the first and last intersection point.
   *
   *  @param[in]  start The start point of the vector to test
   *  @param[in]  end   The end point of the vector to test
   *  @param[out] entryPoint The entry point to the beam-pipe volume
   *  @param[out] exitPoint  The exit point from the beam-pipe volume
   *
   *  @return Enum describing the status of the intersection
   */
  BeamPipeIntersectionType intersectionPoints( const Gaudi::XYZPoint &start,
                                               const Gaudi::XYZPoint &end,
                                               Gaudi::XYZPoint &      entryPoint,
                                               Gaudi::XYZPoint &      exitPoint ) const;
  /**
   *  Test if a given direction intersects the beam-pipe volume at all.
   *  Faster than intersectionPoints since it does not compute the intersection points
   *  in global coordinates.
   *
   *  Scalar implementation
   *
   *  @param[in]  start The start point of the vector to test
   *  @param[in]  end   The end point of the vector to test
   *
   *  @return boolean indicating if the beam pipe was intersected or not
   *  @retval true  The beam pipe was intersected
   *  @retval false The beam pipe was NOT intersected
   */
  template < typename POINT,
             typename std::enable_if< std::is_arithmetic< typename POINT::Scalar >::value >::type
               * = nullptr >
  inline bool testForIntersection( const POINT &start, const POINT &end ) const
  {

    // fast test on if the start and end points are close enough to the
    // the beam axis in global coords
    auto inter = isCloseBy( start, end );

    // If close enough, run full test
    if ( UNLIKELY( inter ) )
    {
      // get point and direction in local coordinates
      const auto pLocal = geometry()->toLocalMatrix() * start;
      const auto vLocal = geometry()->toLocalMatrix() * ( end - start );
      // run full intersection test
      inter = m_localCone->testForIntersection( pLocal, vLocal );
    }

    // return
    return inter;
  }

  /**
   *  Test if a given direction intersects the beam-pipe volume at all.
   *  Faster than intersectionPoints since it does not compute the intersection points
   *  in global coordinates.
   *
   *  SIMD implementation
   *
   *  @param[in]  start The start point of the vector to test
   *  @param[in]  end   The end point of the vector to test
   *
   *  @return boolean indicating if the beam pipe was intersected or not
   *  @retval true  The beam pipe was intersected
   *  @retval false The beam pipe was NOT intersected
   */
  template < typename POINT,
             typename std::enable_if< !std::is_arithmetic< typename POINT::Scalar >::value >::type
               * = nullptr >
  inline decltype( auto ) testForIntersection( const POINT &                     start,
                                               const POINT &                     end,
                                               typename POINT::Scalar::mask_type mask ) const
  {

    // fast test on if the start and end points are close enough to the
    // the beam axis in global coords
    mask &= isCloseBy( start, end );

    // If close enough, run full test
    if ( UNLIKELY( any_of( mask ) ) )
    {
      // get point and direction in local coordinates
      const auto pL = m_toLocalMatrixSIMD * start;
      const auto vL = m_toLocalMatrixSIMD * ( end - start );
      // run full intersection test
      // For the moment run this scalar ... Vectorising SolidCons is for later on ...
      for ( std::size_t i = 0; i < POINT::Scalar::Size; ++i )
      {
        if ( mask[ i ] )
        {
          mask[ i ] = m_localCone->testForIntersection(
            Gaudi::XYZPoint { pL.X()[ i ], pL.Y()[ i ], pL.Z()[ i ] },
            Gaudi::XYZVector { vL.X()[ i ], vL.Y()[ i ], vL.Z()[ i ] } );
        }
      }
    }

    // return
    return mask;
  }

public:

  /// Convert the enum to text for easy reading
  static std::string text( const DeRichBeamPipe::BeamPipeIntersectionType &type );

private:

  /// Returns the 'average' of two points
  template < typename POINT >
  inline POINT average( const POINT &p1, const POINT &p2 ) const
  {
    return POINT( 0.5 * ( p1.x() + p2.x() ), 0.5 * ( p1.y() + p2.y() ), 0.5 * ( p1.z() + p2.z() ) );
  }

  /// Test if the given start and end points are 'close' to the beampipe or not
  template < typename POINT >
  inline decltype( auto ) isCloseBy( const POINT &start, const POINT &end ) const
  {
    if constexpr ( std::is_arithmetic< typename POINT::Scalar >::value )
    {
      // scalar
      return ( isCloseBy( start ) || isCloseBy( end ) );
    }
    else
    {
      // SIMD
      auto mask = isCloseBy( start );
      if ( !all_of( mask ) ) { mask |= isCloseBy( end ); }
      return mask;
    }
  }

  /// Test if the given point is 'close' to the beampipe or not
  template < typename POINT >
  inline decltype( auto ) isCloseBy( const POINT &p ) const
  {
    if constexpr ( std::is_arithmetic< typename POINT::Scalar >::value )
    {
      // Scalar - Get the closest z coord in the beam pipe
      const auto beamz =
        ( p.z() > m_endPGlo.z() ? m_endPGlo.z() :
                                  p.z() < m_startPGlo.z() ? m_startPGlo.z() : p.z() );
      // Get (beam pipe axis (x,y) and R^2 at this point in z position
      const auto beamx  = ( m_m[ 0 ] * beamz ) + m_c[ 0 ];
      const auto beamy  = ( m_m[ 1 ] * beamz ) + m_c[ 1 ];
      const auto beamR2 = ( m_m[ 2 ] * beamz ) + m_c[ 2 ];
      const auto dx     = beamx - p.x();
      const auto dy     = beamy - p.y();
      const auto dz     = beamz - p.z();
      const auto dist2  = ( ( dx * dx ) + ( dy * dy ) + ( dz * dz ) );
      return ( dist2 < beamR2 );
    }
    else
    {
      // SIMD - Get the closest z coord in the beam pipe
      auto beamz                           = p.z();
      beamz( beamz > m_endPGloSIMD.z() )   = m_endPGloSIMD.z();
      beamz( beamz < m_startPGloSIMD.z() ) = m_startPGloSIMD.z();
      // Get beam pipe axis (x,y) and R^2 at this point in z position
      const auto beamx  = ( m_mSIMD[ 0 ] * beamz ) + m_cSIMD[ 0 ];
      const auto beamy  = ( m_mSIMD[ 1 ] * beamz ) + m_cSIMD[ 1 ];
      const auto beamR2 = ( m_mSIMD[ 2 ] * beamz ) + m_cSIMD[ 2 ];
      const auto dx     = beamx - p.x();
      const auto dy     = beamy - p.y();
      const auto dz     = beamz - p.z();
      const auto dist2  = ( ( dx * dx ) + ( dy * dy ) + ( dz * dz ) );
      return ( dist2 < beamR2 );
    }
  }

private: // data

  /// SIMD Global position on the z axis for the start of the beampipe
  Rich::SIMD::Point< Rich::SIMD::DefaultScalarFP > m_startPGloSIMD;

  /// SIMD Global position on the z axis for the end of the beampipe
  Rich::SIMD::Point< Rich::SIMD::DefaultScalarFP > m_endPGloSIMD;

  /// SIMD parameters for y = mx +c scaling of cone axis (x,y) and R^2 as a function of z
  std::array< Rich::SIMD::FP< Rich::SIMD::DefaultScalarFP >, 3 > m_mSIMD, m_cSIMD;

  /// SIMD 'toLocal' transformation
  Rich::SIMD::Transform3D< Rich::SIMD::DefaultScalarFP > m_toLocalMatrixSIMD;

private: // data

  /// The RICH detector
  Rich::DetectorType m_rich = Rich::InvalidDetector;

  /// A copy of the beam pipe cone that is solid (not hollow)
  std::unique_ptr< SolidCons > m_localCone;

  /// Global position on the z axis for the start of the beampipe
  Gaudi::XYZPoint m_startPGlo;

  /// Global position on the z axis for the end of the beampipe
  Gaudi::XYZPoint m_endPGlo;

  // parameters for y = mx +c scaling of cone axis (x,y) and R^2 as a function of z
  std::array< double, 3 > m_m, m_c;
};

//==============================================================================

inline std::ostream &
operator<<( std::ostream &s, const DeRichBeamPipe::BeamPipeIntersectionType &type )
{
  return s << DeRichBeamPipe::text( type );
}

//==============================================================================
