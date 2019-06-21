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
// STD & STL
#include <algorithm>
#include <numeric>

/** GaudiKernel package */
#include "GaudiKernel/StatusCode.h"

/** DetDesc package */
#include "DetDesc/Solid.h"
#include "DetDesc/SolidBoolean.h"
#include "DetDesc/SolidChild.h"
#include "DetDesc/SolidException.h"

// ============================================================================
/** @file SolidBoolean.cpp
 *
 *  implementation file for class SolidBoolean
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   xx/xx/xxxx
 */
// ============================================================================

// ============================================================================
/** constructor - "main"("first") solid is mandatory!
 *  @param name name of the solid
 *  @param solid pointer to the "first"/"main" solid
 */
// ============================================================================
SolidBoolean::SolidBoolean( const std::string& name, std::unique_ptr<ISolid> solid )
    : SolidBase( name ), m_sb_first( std::move( solid ) ) {
  if ( !m_sb_first ) { throw SolidException( "SolidBoolean:: ISolid* points to NULL!" ); }
  /// set bounding parameters
  setBP();
}

// ============================================================================
/** constructor - "main"("first") solid is mandatory!
 *  @param name name of the solid
 */
// ============================================================================
SolidBoolean::SolidBoolean( const std::string& name ) : SolidBase( name ) {}

// ============================================================================
/// set bounding parameters
// ============================================================================
void SolidBoolean::setBP() {
  const SolidBase* base = dynamic_cast<SolidBase*>( m_sb_first.get() );
  if ( !base ) { throw SolidException( "SolidChild::setBP(): ISolid is not SolidBase!" ); }

  setXMin( base->xMin() );
  setYMin( base->yMin() );
  setZMin( base->zMin() );
  setXMax( base->xMax() );
  setYMax( base->yMax() );
  setZMax( base->zMax() );
  setRMax( base->rMax() );
  setRhoMax( base->rhoMax() );

  checkBP();
}
// ============================================================================

// ============================================================================
/** reset to the initial ("after constructor") state
 */
// ============================================================================
ISolid* SolidBoolean::reset() {
  SolidBase::reset();
  if ( m_sb_first ) { m_sb_first->reset(); }
  for ( auto& c : children() ) { c.reset(); }
  return this;
}

// ============================================================================
/** add child to daughter container
 *  @param child pointer to solid
 *  @param mtrx  pointer to transformation
 *  @return status code
 */
// ============================================================================
StatusCode SolidBoolean::addChild( std::unique_ptr<ISolid>   child, //
                                   const Gaudi::Transform3D* mtrx ) {
  if ( !child ) { return StatusCode::FAILURE; }
  auto pChild = std::make_unique<SolidChild>( std::move( child ), mtrx, "Child For " + name() );
  if ( !pChild ) { return StatusCode::FAILURE; }
  m_sb_childrens.push_back( std::move( pChild ) );
  checkTickContainerCapacity();
  return StatusCode::SUCCESS;
}

// ============================================================================
/** add child to daughter container
 *  @param child    pointer to solid
 *  @param position position
 *  @param rotation rotation
 */
// ============================================================================
StatusCode SolidBoolean::addChild( std::unique_ptr<ISolid>  child,    //
                                   const Gaudi::XYZPoint&   position, //
                                   const Gaudi::Rotation3D& rotation ) {
  if ( UNLIKELY( !child ) ) { return StatusCode::FAILURE; }
  auto pChild = std::make_unique<SolidChild>( std::move( child ), position, rotation, "Child For " + name() );
  if ( UNLIKELY( !pChild ) ) { return StatusCode::FAILURE; }
  m_sb_childrens.push_back( std::move( pChild ) );
  checkTickContainerCapacity();
  return StatusCode::SUCCESS;
}
// ============================================================================
unsigned int SolidBoolean::intersectionTicks( const Gaudi::XYZPoint&  Point,  //
                                              const Gaudi::XYZVector& Vector, //
                                              ISolid::Ticks&          ticks   //
                                              ) const {
  return intersectionTicksImpl( Point, Vector, ticks );
}
// ============================================================================
unsigned int SolidBoolean::intersectionTicks( const Gaudi::Polar3DPoint&  Point,  //
                                              const Gaudi::Polar3DVector& Vector, //
                                              ISolid::Ticks&              ticks   //
                                              ) const {
  return intersectionTicksImpl( Point, Vector, ticks );
}
// ============================================================================
unsigned int SolidBoolean::intersectionTicks( const Gaudi::RhoZPhiPoint&  Point,  //
                                              const Gaudi::RhoZPhiVector& Vector, //
                                              ISolid::Ticks&              ticks   //
                                              ) const {
  return intersectionTicksImpl( Point, Vector, ticks );
}
// ============================================================================
/** Calculate the maximum number of ticks that a straight line could make with this solid
 *  @return maximum number of ticks
 */
ISolid::Ticks::size_type SolidBoolean::maxNumberOfTicks() const {
  auto c = children();
  return std::accumulate( begin( c ), end( c ), first()->maxNumberOfTicks(),
                          []( auto sum, const auto& c ) { return sum + c.maxNumberOfTicks(); } );
}
// ============================================================================
unsigned int SolidBoolean::intersectionTicks( const Gaudi::XYZPoint&  Point,   //
                                              const Gaudi::XYZVector& Vector,  //
                                              const ISolid::Tick&     tickMin, //
                                              const ISolid::Tick&     tickMax, //
                                              ISolid::Ticks&          ticks    //
                                              ) const {
  return intersectionTicksImpl( Point, Vector, tickMin, tickMax, ticks );
}
// ============================================================================
unsigned int SolidBoolean::intersectionTicks( const Gaudi::Polar3DPoint&  Point,   //
                                              const Gaudi::Polar3DVector& Vector,  //
                                              const ISolid::Tick&         tickMin, //
                                              const ISolid::Tick&         tickMax, //
                                              ISolid::Ticks&              ticks    //
                                              ) const {
  return intersectionTicksImpl( Point, Vector, tickMin, tickMax, ticks );
}
// ============================================================================
unsigned int SolidBoolean::intersectionTicks( const Gaudi::RhoZPhiPoint&  Point,   //
                                              const Gaudi::RhoZPhiVector& Vector,  //
                                              const ISolid::Tick&         tickMin, //
                                              const ISolid::Tick&         tickMax, //
                                              ISolid::Ticks&              ticks    //
                                              ) const {
  return intersectionTicksImpl( Point, Vector, tickMin, tickMax, ticks );
}
// ============================================================================
/** printout to STD/STL stream
 *  @param os STD/STL stream
 *  @return reference to the stream
 */
// ============================================================================
std::ostream& SolidBoolean::printOut( std::ostream& os ) const {
  // printout the base class
  SolidBase::printOut( os );
  os << " ** 'Main' solid is \n";
  first()->printOut( os );
  for ( auto& child : children() ) {
    os << " ** 'Booled' with \n";
    child.printOut( os );
  }
  return os << std::endl;
}
// ============================================================================

// ============================================================================
/** printout to Gaudi  stream
 *  @param os Gaudi stream
 *  @return reference to the stream
 */
// ============================================================================
MsgStream& SolidBoolean::printOut( MsgStream& os ) const {
  // printout the base class
  SolidBase::printOut( os );
  os << " ** 'Main' solid is " << endmsg;
  first()->printOut( os );
  for ( auto& child : children() ) {
    os << " ** 'Booled' with " << endmsg;
    child.printOut( os );
  }
  return os << endmsg;
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
