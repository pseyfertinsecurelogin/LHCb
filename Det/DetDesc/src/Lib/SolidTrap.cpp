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
// ===========================================================================

// Units
#include "GaudiKernel/SystemOfUnits.h"

/**  DetDesc package */
#include "DetDesc/SolidBase.h"
#include "DetDesc/SolidBox.h"
#include "DetDesc/SolidException.h"
#include "DetDesc/SolidTicks.h"
#include "DetDesc/SolidTrap.h"
#include "DetDesc/SolidTrd.h"

// ============================================================================
/** @file SolidTrap.cpp
 *
 *  implementation of class SolidTrap
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 */
// ============================================================================

// ============================================================================
/** constructor
 *  @param Name                   name of trap solid
 *  @param ZHalfLength            half length in z
 *  @param Theta                  theta angle
 *  @param Phi                    phi angle
 *  @param DyAtMinusZ             delta y at bottom face
 *  @param DxAtMinusZMinusY       delta x at bottom face , minus y
 *  @param DxAtMinusZPlusY        delta x at bottom face , plus y
 *  @param AlphaAtMinusZ          alpha angle at bottom face
 *  @param DyAtPlusZ              delta y at top face
 *  @param DxAtPlusZMinusY        delta x at top face, minus y
 *  @param DxAtPlusZPlusY         delta x at top face, plus y
 *  @param AlphaAtPlusZ           alpha angle at top face
 *  @exception SolidException wrong parameters range
 */
// ============================================================================
SolidTrap::SolidTrap( const std::string& Name, const double ZHalfLength, const double Theta, const double Phi,
                      const double DyAtMinusZ, const double DxAtMinusZMinusY, const double DxAtMinusZPlusY,
                      const double AlphaAtMinusZ, const double DyAtPlusZ, const double DxAtPlusZMinusY,
                      const double DxAtPlusZPlusY, const double AlphaAtPlusZ )
    : SolidBase( Name )
    , SolidPolyHedronHelper( Name )
    , m_trap_zHalfLength( ZHalfLength )
    , m_trap_theta( Theta )
    , m_trap_phi( Phi )
    , m_trap_dyAtMinusZ( DyAtMinusZ )
    , m_trap_dxAtMinusZMinusY( DxAtMinusZMinusY )
    , m_trap_dxAtMinusZPlusY( DxAtMinusZPlusY )
    , m_trap_alphaAtMinusZ( AlphaAtMinusZ )
    , m_trap_dyAtPlusZ( DyAtPlusZ )
    , m_trap_dxAtPlusZMinusY( DxAtPlusZMinusY )
    , m_trap_dxAtPlusZPlusY( DxAtPlusZPlusY )
    , m_trap_alphaAtPlusZ( AlphaAtPlusZ ) {
  if ( 0 >= ZHalfLength ) { throw SolidException( "SolidTrap::ZHalfLength is not positive!" ); }
  if ( 0 >= DyAtMinusZ ) { throw SolidException( "SolidTrap::dyAtMinusZ is not positive!" ); }
  if ( 0 >= DxAtMinusZMinusY ) { throw SolidException( "SolidTrap::dxAtMinusZMinusY is not positive!" ); }
  if ( 0 >= DxAtMinusZPlusY ) { throw SolidException( "SolidTrap::dxAtMinusZPlusY  is not positive!" ); }
  if ( 0 >= DyAtPlusZ ) { throw SolidException( "SolidTrap::dyAtMinusZ is not positive!" ); }
  if ( 0 >= DxAtPlusZMinusY ) { throw SolidException( "SolidTrap::dxAtMinusZMinusY is not positive!" ); }
  if ( 0 >= DxAtPlusZPlusY ) { throw SolidException( "SolidTrap::dxAtMinusZPlusY is not positive!" ); }
  ///
  makeAll();
  /// set bounding parameters
  setBP();
  ///
  checkTickContainerCapacity();
  createCover();
}

// ============================================================================
/** initialize the polihedron base
 *  @exception SolidException wrong parameters
 */
// ============================================================================
void SolidTrap::makeAll() {
  /// clear vertices
  m_ph_vertices.clear();
  m_ph_planes.clear();
  SolidPolyHedronHelper::reset();
  {
    /** construct points (vertoces)
     * code is copied from G4
     */
    double fDz         = zHalfLength();
    double fTthetaCphi = tan( theta() ) * cos( phi() );
    double fTthetaSphi = tan( theta() ) * sin( phi() );
    double fDy1        = dyAtMinusZ();
    double fDx1        = dxAtMinusZMinusY();
    double fDx2        = dxAtMinusZPlusY();
    double fTalpha1    = tan( alphaAtMinusZ() );
    double fDy2        = dyAtPlusZ();
    double fDx3        = dxAtPlusZMinusY();
    double fDx4        = dxAtPlusZPlusY();
    double fTalpha2    = tan( alphaAtPlusZ() );
    ///
    m_ph_vertices.emplace_back( -fDz * fTthetaCphi - fDy1 * fTalpha1 - fDx1, -fDz * fTthetaSphi - fDy1, -fDz );
    m_ph_vertices.emplace_back( -fDz * fTthetaCphi - fDy1 * fTalpha1 + fDx1, -fDz * fTthetaSphi - fDy1, -fDz );
    m_ph_vertices.emplace_back( -fDz * fTthetaCphi + fDy1 * fTalpha1 - fDx2, -fDz * fTthetaSphi + fDy1, -fDz );
    m_ph_vertices.emplace_back( -fDz * fTthetaCphi + fDy1 * fTalpha1 + fDx2, -fDz * fTthetaSphi + fDy1, -fDz );
    m_ph_vertices.emplace_back( +fDz * fTthetaCphi - fDy2 * fTalpha2 - fDx3, +fDz * fTthetaSphi - fDy2, +fDz );
    m_ph_vertices.emplace_back( +fDz * fTthetaCphi - fDy2 * fTalpha2 + fDx3, +fDz * fTthetaSphi - fDy2, +fDz );
    m_ph_vertices.emplace_back( +fDz * fTthetaCphi + fDy2 * fTalpha2 - fDx4, +fDz * fTthetaSphi + fDy2, +fDz );
    m_ph_vertices.emplace_back( +fDz * fTthetaCphi + fDy2 * fTalpha2 + fDx4, +fDz * fTthetaSphi + fDy2, +fDz );
    ///
  }
  if ( 8 != m_ph_vertices.size() ) { throw SolidException( "SolidTrap:: wrong vertex #!" ); }
  /// make faces
  addFace( point( 0 ), point( 4 ), point( 5 ), point( 1 ) );
  addFace( point( 2 ), point( 3 ), point( 7 ), point( 6 ) );
  addFace( point( 0 ), point( 2 ), point( 6 ), point( 4 ) );
  addFace( point( 1 ), point( 5 ), point( 7 ), point( 3 ) );
  addFace( point( 0 ), point( 1 ), point( 3 ), point( 2 ) ); /// bottom face
  addFace( point( 5 ), point( 4 ), point( 6 ), point( 7 ) ); /// top    face
  ///
  if ( 6 != planes().size() ) { throw SolidException( "SolidTrap::wrong face # " ); }
  ///
}

// ============================================================================
/** - creates the pointer to "simplified" solid - "cover"
 *    -# the cover for Trap is Trd
 */
// ============================================================================
void SolidTrap::createCover() {
  double ymx1 = fabs( point( 0 ).y() );
  double xmx1 = fabs( point( 0 ).x() );
  double ymx2 = fabs( point( 4 ).y() );
  double xmx2 = fabs( point( 4 ).x() );
  ///
  for ( std::size_t i = 1; i < 4; ++i ) {
    xmx1 = fabs( point( i ).x() ) > xmx1 ? fabs( point( i ).x() ) : xmx1;
    ymx1 = fabs( point( i ).y() ) > ymx1 ? fabs( point( i ).y() ) : ymx1;
  }
  for ( std::size_t i1 = 4; i1 < 8; ++i1 ) {
    xmx2 = fabs( point( i1 ).x() ) > xmx2 ? fabs( point( i1 ).x() ) : xmx2;
    ymx2 = fabs( point( i1 ).y() ) > ymx2 ? fabs( point( i1 ).y() ) : ymx2;
  }
  ///
  m_cover = std::make_unique<SolidTrd>( "Cover for " + name(), zHalfLength(), xmx1, ymx1, xmx2, ymx2 );
}

// ============================================================================
/** - printout to STD/STL stream
 *  - implementation  of ISolid abstract interface
 *  - reimplementation of SolidBase::printOut( std::ostream& )
 *  @see SolidBase
 *  @see ISolid
 *  @param os STD/STL stream
 *  @return reference to the stream
 */
// ============================================================================
std::ostream& SolidTrap::printOut( std::ostream& os ) const {
  SolidBase::printOut( os );
  return os << "["
            << " sizeZ[mm]=" << zHalfLength() / Gaudi::Units::millimeter
            << " theta[deg]=" << theta() / Gaudi::Units::degree << " phi[deg]=" << phi() / Gaudi::Units::degree
            << " sizeY1[mm]=" << dyAtMinusZ() / Gaudi::Units::millimeter
            << " sizeX1[mm]=" << dxAtMinusZMinusY() / Gaudi::Units::millimeter
            << " sizeX2[mm]=" << dxAtMinusZPlusY() / Gaudi::Units::millimeter
            << " alp1[deg]=" << alphaAtMinusZ() / Gaudi::Units::degree
            << " sizeY2[mm]=" << dyAtPlusZ() / Gaudi::Units::millimeter
            << " sizeX3[mm]=" << dxAtPlusZMinusY() / Gaudi::Units::millimeter
            << " sizeX4[mm]=" << dxAtPlusZPlusY() / Gaudi::Units::millimeter
            << " alp2[deg]=" << alphaAtPlusZ() / Gaudi::Units::degree << "]";
}

// ============================================================================
/** - printout to Gaudi MsgStream stream
 *  - implementation  of ISolid abstract interface
 *  - reimplementation of SolidBase::printOut( MsgStream& )
 *  @see SolidBase
 *  @see ISolid
 *  @param os Gaudi MsgStream  stream
 *  @return reference to the stream
 */
// ============================================================================
MsgStream& SolidTrap::printOut( MsgStream& os ) const {
  SolidBase::printOut( os );
  return os << "["
            << " sizeZ[mm]=" << zHalfLength() / Gaudi::Units::millimeter
            << " theta[deg]=" << theta() / Gaudi::Units::degree << " phi[deg]=" << phi() / Gaudi::Units::degree
            << " sizeY1[mm]=" << dyAtMinusZ() / Gaudi::Units::millimeter
            << " sizeX1[mm]=" << dxAtMinusZMinusY() / Gaudi::Units::millimeter
            << " sizeX2[mm]=" << dxAtMinusZPlusY() / Gaudi::Units::millimeter
            << " alp1[deg]=" << alphaAtMinusZ() / Gaudi::Units::degree
            << " sizeY2[mm]=" << dyAtPlusZ() / Gaudi::Units::millimeter
            << " sizeX3[mm]=" << dxAtPlusZMinusY() / Gaudi::Units::millimeter
            << " sizeX4[mm]=" << dxAtPlusZPlusY() / Gaudi::Units::millimeter
            << " alp2[deg]=" << alphaAtPlusZ() / Gaudi::Units::degree << "]";
}
// ============================================================================
