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
// ============================================================================
// Include files
// ============================================================================
// STL & STD
// ============================================================================
#include <algorithm>
#include <functional>
#include <set>
// ============================================================================
/// from Gaudi
// ============================================================================
#include "GaudiKernel/IRndmGenSvc.h"
#include "GaudiKernel/RndmGenerators.h"
#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiKernel/Vector3DTypes.h"
#include "GaudiKernel/VectorsAsProperty.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiAlgorithm.h"
// ============================================================================
// DetDesc
// ============================================================================
#include "DetDesc/ILVolume.h"
#include "DetDesc/IPVolume.h"
#include "DetDesc/ISolid.h"
#include "DetDesc/ITransportSvc.h"
#include "DetDesc/IntersectionErrors.h"
#include "DetDesc/Material.h"
#include "DetDesc/SolidBox.h"
#include "DetDesc/VolumeIntersectionIntervals.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/format.hpp"
#include "boost/progress.hpp"
// ============================================================================
/** @file
 *  Implementation file for class  DetDesc::CheckOverlap
 *  @date 2007-12-12
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 */
// ============================================================================
namespace DetDesc {
  // ==========================================================================
  bool isOK( const ILVolume::Intersections& cnt ) {
    for ( auto i1 = cnt.begin(); cnt.end() != i1; ++i1 ) {
      const auto x1 = i1->first.first;
      const auto x2 = i1->first.second;
      for ( auto i2 = i1 + 1; cnt.end() != i2; ++i2 ) {
        if ( 0 == VolumeIntersectionIntervals::intersect( *i1, *i2 ) ) { return false; }

        const auto y1 = i2->first.first;
        const auto y2 = i2->first.second;
        if ( x2 < y1 || y2 < x1 ) { continue; }
        const auto z1 = std::max( x1, y1 );
        const auto z2 = std::min( x2, y2 );
        // ??
        if ( z1 < z2 ) { return false; }
      }
    }
    return true;
  }
  // ==========================================================================
  std::pair<std::pair<int, int>, double> notOK( const ILVolume::Intersections& cnt ) {
    for ( auto i1 = cnt.begin(); cnt.end() != i1; ++i1 ) {
      const auto x1 = i1->first.first;
      const auto x2 = i1->first.second;
      for ( auto i2 = i1 + 1; cnt.end() != i2; ++i2 ) {
        const auto y1 = i2->first.first;
        const auto y2 = i2->first.second;
        if ( x2 < y1 || y2 < x1 ) { continue; }
        const auto z1 = std::max( x1, y1 );
        const auto z2 = std::min( x2, y2 );
        if ( z1 < z2 ) {
          auto p = std::pair{i1 - cnt.begin(), i2 - cnt.begin()};
          return {p, z2 - z1};
        }
      }
    }
    return {};
  }
  // ==========================================================================
  /** @class CheckOverlap
   *  The helper algorithm to detect the various overlaps
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-12-12
   */
  class CheckOverlap : public GaudiAlgorithm {
  public:
    // ========================================================================
    /** standard algorithm initialization
     *  @see IAlgorithm
     *  @return status code
     */
    StatusCode initialize() override;
    // ========================================================================
    /** standard execution of algorithm
     *  @see IAlgorithm
     *  @return status code
     */
    StatusCode execute() override;
    // ========================================================================
    /** standard finalization of algorithm
     *  @see IAlgorithm
     *  @return status code
     */
    StatusCode finalize() override;
    // ========================================================================
    /** Standard constructor
     *  @param name name of the algorithm
     *  @param svcloc pointer to Service Locator
     */
    CheckOverlap( const std::string& name, ISvcLocator* svcloc );
    // ========================================================================
  private:
    // ========================================================================
    /// check one volume
    StatusCode checkVolume( const ILVolume* volume, const unsigned int level ) const;
    /// make all shoots
    StatusCode makeShots( const ILVolume* volume ) const;
    // ========================================================================
  private:
    // ========================================================================
    /// volume name
    Gaudi::Property<std::string> m_volumeName{this, "Volume", "Undefined Volume", "Volume name ot be checked"};
    /// volume itself
    const ILVolume* m_volume = nullptr;

    // volume limits (for assemblies)
    Gaudi::Property<double> m_minx{this, "MinX", -10 * Gaudi::Units::m};
    Gaudi::Property<double> m_maxx{this, "MaxX", 10 * Gaudi::Units::m};
    Gaudi::Property<double> m_miny{this, "MinY", -10 * Gaudi::Units::m};
    Gaudi::Property<double> m_maxy{this, "MaxY", 10 * Gaudi::Units::m};
    Gaudi::Property<double> m_minz{this, "MinZ", -10 * Gaudi::Units::m};
    Gaudi::Property<double> m_maxz{this, "MaxZ", 10 * Gaudi::Units::m};

    // number of shots
    Gaudi::Property<int> m_shots{this, "Shots", 10000};

    // point of shooting for sphere
    Gaudi::Property<Gaudi::XYZPoint>  m_vertex{this, "Null"};
    mutable std::set<const ILVolume*> m_checked;

    mutable Gaudi::Accumulators::Counter<> m_volumesCnt{this, "#volumes"};
    mutable Gaudi::Accumulators::Counter<> m_assemblyCnt{this, "#assembly"};
  };
  // ==========================================================================
} // namespace DetDesc
// ============================================================================
/*  Standard constructor
 *  @param name name of the algorithm
 *  @param svcloc pointer to Service Locator
 */
// ============================================================================
DetDesc::CheckOverlap::CheckOverlap( const std::string& name, ISvcLocator* svcloc ) : GaudiAlgorithm( name, svcloc ) {}
// ============================================================================
/*  standard algorithm initialization
 *  @see IAlgorithm
 *  @return status code
 */
// ============================================================================
StatusCode DetDesc::CheckOverlap::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize();
  if ( sc.isFailure() ) { return sc; }

  Assert( randSvc(), "randSvc() points to NULL!" );
  Assert( detSvc(), "detSvc()  points to NULL!" );

  if ( !exist<ILVolume>( detSvc(), m_volumeName ) && exist<ILVolume>( detSvc(), "/dd/Geometry/" + m_volumeName ) ) {
    m_volumeName = "/dd/Geometry/" + m_volumeName;
  }

  m_volume = getDet<ILVolume>( m_volumeName );

  if ( !m_volume->isAssembly() && 0 != m_volume->solid() ) {
    const auto top = m_volume->solid()->coverTop();
    if ( !top ) { return Error( "CoverTop* points to NULL!" ); }
    const auto box = dynamic_cast<const SolidBox*>( top );
    if ( !box ) { return Error( "SolidBox* points to NULL!" ); }

    m_minx = -1 * box->xHalfLength() * 1.05;
    m_maxx = box->xHalfLength() * 1.05;
    m_miny = -1 * box->yHalfLength() * 1.05;
    m_maxy = box->yHalfLength() * 1.05;
    m_minz = -1 * box->zHalfLength() * 1.05;
    m_maxz = box->zHalfLength() * 1.05;
  }

  /// check for Transport Service
  svc<ITransportSvc>( "TransportSvc", true );

  always() << " ATTENTION! THIS ALGORITHM DOES DESTROY THE GEOMETRY TREE" << endmsg;
  always() << " ATTENTION! NEVER USED IT IN THE REAL JOB OR RELY ON THE " << endmsg;
  always() << " ATTENTION! RESULT OF ANY OTHER 'GEOMETRY' TOOL/ALGORITHM" << endmsg;

  return StatusCode::SUCCESS;
}
// ============================================================================
/*  standard execution of algorithm
 *  @see IAlgorithm
 *  @return status code
 */
// ============================================================================
StatusCode DetDesc::CheckOverlap::execute() {
  StatusCode sc = checkVolume( m_volume, 0 );

  always() << " ATTENTION! THIS ALGORITHM DOES DESTROY THE GEOMETRY TREE" << endmsg;
  always() << " ATTENTION! NEVER USED IT IN THE REAL JOB OR RELY ON THE " << endmsg;
  always() << " ATTENTION! RESULT OF ANY OTHER 'GEOMETRY' TOOL/ALGORITHM" << endmsg;

  return sc;
}
// ============================================================================
// perform the actual checking of the volume
// ============================================================================
StatusCode DetDesc::CheckOverlap::checkVolume( const ILVolume* volume, const unsigned int level ) const {

  if ( !volume ) { return Error( "Invalid pointer to ILVolume!" ); }

  boost::format     fmt( "%-3d" );
  const std::string lev = ( fmt % level ).str();

  // not checked yet ?
  if ( m_checked.end() != m_checked.find( volume ) ) { return StatusCode::SUCCESS; }

  always() << lev << std::string( 2 * level, ' ' ) << "Checking: " << volume->name() << endmsg;

  // ==========================================================================
  // loop over all daughter volumes and
  for ( const auto pv : volume->pvolumes() ) {
    if ( !pv ) { return Error( "IPVolume* points to NULL!" ); } // RETURN
    const auto lv = pv->lvolume();
    if ( !lv ) { return Error( "ILVolume* points to NULL!" ); } // RETURN
    // check the daughter volume
    const auto sc = checkVolume( lv, level + 1 );
    if ( sc.isFailure() ) { return sc; } // RETURN
  }
  // ==========================================================================
  /*  here all daughter volumes are OK and are CLEARED!
   *  and one can start to make the own  shoots (MUST be efficient!)
   */
  const auto result = makeShots( volume );
  //
  // ATTENTION!!!
  // clear daugher volumes :
  const_cast<ILVolume*>( volume )->clearVolumes();

  // ==========================================================================
  always() << lev << std::string( 2 * level, ' ' ) << "Checked:  " << volume->name() << endmsg;

  ++m_volumesCnt;
  if ( volume->isAssembly() ) { ++m_assemblyCnt; }

  m_checked.insert( volume );

  return result;
}
// ============================================================================
// make the random shoots
// ============================================================================
StatusCode DetDesc::CheckOverlap::makeShots( const ILVolume* volume ) const {

  if ( !volume ) { return Error( "Invalid pointer to ILVolume!" ); }

  // reset the counter of errors
  DetDesc::IntersectionErrors::setCode( StatusCode::SUCCESS, volume );

  std::vector<Gaudi::XYZVector> vcts;
  const int                     max_vectors = 7;
  vcts.reserve( max_vectors );

  //
  vcts.emplace_back( 0, 0, 1 );
  vcts.emplace_back( 0, 1, 0 );
  vcts.emplace_back( 1, 0, 0 );

  vcts.emplace_back( 1, 1, 0 );
  vcts.emplace_back( 1, -1, 0 );
  vcts.emplace_back( 0, 1, 1 );
  vcts.emplace_back( 0, -1, 1 );

  auto xmin = m_minx;
  auto xmax = m_maxx;
  auto ymin = m_miny;
  auto ymax = m_maxy;
  auto zmin = m_minz;
  auto zmax = m_maxz;

  if ( !volume->isAssembly() && 0 != volume->solid() ) {
    const auto top = volume->solid()->coverTop();
    if ( !top ) { return Error( "CoverTop* points to NULL!" ); }
    const auto box = dynamic_cast<const SolidBox*>( top );
    if ( !box ) { return Error( "SolidBox* points to NULL!" ); }

    xmin = -1 * box->xHalfLength() * 1.01;
    xmax = box->xHalfLength() * 1.01;
    ymin = -1 * box->yHalfLength() * 1.01;
    ymax = box->yHalfLength() * 1.01;
    zmin = -1 * box->zHalfLength() * 1.01;
    zmax = box->zHalfLength() * 1.01;
  }

  // get the number of shoots
  auto nShots = m_shots;

  // check the simplest cases
  if ( !volume->isAssembly() && volume->pvolumes().empty() ) {
    const auto solid = volume->solid();
    if ( solid && dynamic_cast<const SolidBox*>( solid ) ) {
      // nothing to check, the case is just trivial
      nShots = 0;
      return StatusCode::SUCCESS; // RETURN
    }
  }

  { //
    boost::format fmt( "Shooting #%8d:" );
    info() << ( fmt % nShots ).str() << volume->name() << " #pv=" << volume->pvolumes().size() << endmsg;
  }

  // display the progress
  boost::progress_display progress( nShots );

  // get the random number generator
  Rndm::Numbers flat( randSvc(), Rndm::Flat( 0.0, 1.0 ) );
  Rndm::Numbers flat1( randSvc(), Rndm::Flat( -1.0, 1.0 ) );

  for ( int iShoot = 0; iShoot < nShots; ++iShoot ) {

    const auto x1 = xmin + flat.shoot() * ( xmax - xmin );
    const auto y1 = ymin + flat.shoot() * ( ymax - ymin );
    const auto z1 = zmin + flat.shoot() * ( zmax - zmin );

    const auto x2 = xmin + flat.shoot() * ( xmax - xmin );
    const auto y2 = ymin + flat.shoot() * ( ymax - ymin );
    const auto z2 = zmin + flat.shoot() * ( zmax - zmin );

    const Gaudi::XYZPoint point( x1, y1, z1 );
    const Gaudi::XYZPoint p2( x2, y2, z2 );

    vcts.emplace_back( p2 - point );
    vcts.emplace_back( m_vertex.value() - point );
    vcts.emplace_back( m_vertex.value() - point );

    vcts.emplace_back( flat1(), flat1(), flat1() );
    vcts.emplace_back( flat1(), flat1(), flat1() );
    vcts.emplace_back( flat1(), flat1(), flat1() );

    vcts.emplace_back( 0, 0, 1 + 0.1 * flat1() );
    vcts.emplace_back( 0, 1 + 0.1 * flat1(), 0 );
    vcts.emplace_back( 1 + 0.1 * flat1(), 0, 0 );

    vcts.emplace_back( 0, 0, 1 + 0.1 * flat1() );
    vcts.emplace_back( 0, 1 + 0.1 * flat1(), 0 );
    vcts.emplace_back( 1 + 0.1 * flat1(), 0, 0 );

    for ( const auto v : vcts ) {

      // reset the counter of errors
      DetDesc::IntersectionErrors::setCode( StatusCode::SUCCESS, volume );

      ILVolume::Intersections intersections;
      volume->intersectLine( point, v, intersections, 0 );

      // get the status
      auto sc = DetDesc::IntersectionErrors::code();

      if ( sc.isFailure() ) {

        error() << "Problem is detected with volume " << volume->name() << " With P/V=" << point << "/" << v << endmsg;

        DetDesc::IntersectionErrors::inspect( volume, point, v, intersections );

        return Error( "Intersection problems with " + volume->name(), sc );
      }
    } // vectors
    // remove last vectors
    while ( max_vectors < vcts.size() ) { vcts.pop_back(); }
    // show the progress
    ++progress;
  } // shoots

  return StatusCode::SUCCESS;
}
// ========================================================================
/*  standard finalization of algorithm
 *  @see IAlgorithm
 *  @return status code
 */
// ========================================================================
StatusCode DetDesc::CheckOverlap::finalize() {
  always() << " ATTENTION! THIS ALGORITHM DOES DESTROY THE GEOMETRY TREE" << endmsg;
  always() << " ATTENTION! NEVER USED IT IN THE REAL JOB OR RELY ON THE " << endmsg;
  always() << " ATTENTION! RESULT OF ANY OTHER 'GEOMETRY' TOOL/ALGORITHM" << endmsg;

  return GaudiAlgorithm::finalize();
}
// ============================================================================
/// the factory, nesessary for instantiation
DECLARE_COMPONENT( DetDesc::CheckOverlap )
// ============================================================================

// ============================================================================
// The END
// ============================================================================
