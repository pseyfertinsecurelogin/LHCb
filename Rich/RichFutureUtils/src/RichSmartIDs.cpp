
/*****************************************************************************\
* (c) Copyright 2000-2020 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

// local
#include "RichFutureUtils/RichSmartIDs.h"

using namespace Rich::Utils;

//=============================================================================
// Finds the average positions of a vector of clusters, in global LHCb
// coordinates on the PD entrance window
//=============================================================================
LHCb::STL::Vector<Gaudi::XYZPoint> RichSmartIDs::globalPositions( const Rich::PDPixelCluster::Vector& clusters,
                                                                  const bool ignoreClusters ) const {

  // allocate the vector
  LHCb::STL::Vector<Gaudi::XYZPoint> points;
  points.reserve( clusters.size() );

  // Do we need to take the full cluster info into account
  if ( UNLIKELY( !ignoreClusters ) ) {

    // scalar loop using full cluster info
    for ( const auto& clus : clusters ) {
      points.emplace_back();
      const auto ok = globalPosition( clus, points.back() );
      if ( UNLIKELY( !ok ) ) {
        // to do... Add MsgStream messaging here
        std::cout << "Failed to compute global position for " << clus << "\n";
      }
    }

  } else {
    // Speed things up using only the primary IDs and SIMD methods...

    // scalar FP type for SIMD objects
    using FP = Rich::SIMD::DefaultScalarFP;
    // SIMD float type
    using SIMDFP = Rich::SIMD::FP<FP>;
    // SIMD Point
    using SIMDPoint = Rich::SIMD::Point<FP>;
    // Type for SmartIDs container.
    using SmartIDs = Rich::SIMD::STDArray<LHCb::RichSmartID>;

    // working SIMD objects
    SmartIDs  ids;
    SIMDPoint point{0, 0, 0};
    // SIMD index
    std::size_t index = 0;
    // Last PD pointer
    const DeRichPD* lastPD{nullptr};

    // Save functor
    auto saveInfo = [&]() {
      // Compute SIMD position
      lastPD->detectionPoint( ids, point, m_hitPhotoCathSide );
      // save scalar info to container
      for ( std::size_t i = 0; i < index; ++i ) {
        points.emplace_back( point.X()[i], point.Y()[i], point.Z()[i] );
      } // reset SIMD index
      index  = 0;
      lastPD = nullptr;
    };

    // loop over the cluster info
    for ( const auto& clus : clusters ) {
      // If new PD, or SIMD data is full, save and reset.
      if ( lastPD && ( lastPD != clus.dePD() || index >= SIMDFP::Size ) ) { saveInfo(); }
      // Fill info
      lastPD     = clus.dePD();
      ids[index] = clus.primaryID();
      // increment index for next time
      ++index;
    }

    // Save the last one
    if ( lastPD && index > 0 ) { saveInfo(); }
  }

  // debug sanity check
  assert( points.size() == clusters.size() );

  // return
  return points;
}

//=============================================================================
// Returns the position of a RichSmartID cluster in global coordinates
// on the PD entrance window
//=============================================================================
bool RichSmartIDs::globalPosition( const Rich::PDPixelCluster& cluster, Gaudi::XYZPoint& detectPoint ) const {
  // Default return status is BAD
  bool sc = false;
  // cluster size
  const auto csize = cluster.size();
  if ( LIKELY( 1 == csize ) ) {
    // Handle single pixel clusters differently for speed
    sc = _globalPosition( cluster.primaryID(), cluster.dePD(), detectPoint );
  } else if ( csize > 1 ) {
    // start with primary ID position
    sc = _globalPosition( cluster.primaryID(), cluster.dePD(), detectPoint );
    // secondary IDs
    for ( const auto S : cluster.secondaryIDs() ) {
      Gaudi::XYZPoint tmpP;
      sc = sc && _globalPosition( S, cluster.dePD(), tmpP );
      detectPoint += (Gaudi::XYZVector)tmpP;
    }
    // normalise
    detectPoint /= (double)csize;
  }
  return sc;
}

//=============================================================================
// Returns the PD position (center of the silicon wafer)
//=============================================================================
bool RichSmartIDs::pdPosition( const LHCb::RichSmartID& pdid, Gaudi::XYZPoint& pdPoint ) const {
  // Create temporary RichSmartIDs for two corners of the PD wafer
  LHCb::RichSmartID id1( pdid ), id0( pdid );
  id0.setPixelRow( pdid.idType() == LHCb::RichSmartID::MaPMTID ? 1 : 10 );
  id0.setPixelCol( pdid.idType() == LHCb::RichSmartID::MaPMTID ? 1 : 10 );
  id1.setPixelRow( pdid.idType() == LHCb::RichSmartID::MaPMTID ? 6 : 21 );
  id1.setPixelCol( pdid.idType() == LHCb::RichSmartID::MaPMTID ? 6 : 21 );

  // get position of each of these pixels
  Gaudi::XYZPoint a, b;
  const auto      sc = _globalPosition( id0, a ) && _globalPosition( id1, b );

  // return average position (i.e. PD centre)
  pdPoint = ( sc ? Gaudi::XYZPoint( 0.5 * ( a.x() + b.x() ), 0.5 * ( a.y() + b.y() ), 0.5 * ( a.z() + b.z() ) )
                 : Gaudi::XYZPoint( 0, 0, 0 ) );
  return sc;
}

//=============================================================================
// Returns the SmartID for a given global position
//=============================================================================
bool RichSmartIDs::smartID( const Gaudi::XYZPoint& globalPoint, LHCb::RichSmartID& smartid ) const {
  // check to see if the smartID is set, and if PD is active
  if ( smartid.pdIsSet() && !m_richS->pdIsActive( smartid ) ) { return false; }

  try {
    if ( globalPoint.z() < 8000.0 ) {
      smartid.setRich( Rich::Rich1 );
      smartid.setPanel( globalPoint.y() > 0.0 ? Rich::top : Rich::bottom );
    } else {
      smartid.setRich( Rich::Rich2 );
      smartid.setPanel( globalPoint.x() > 0.0 ? Rich::left : Rich::right );
    }
    return panel( smartid )->smartID( globalPoint, smartid );
  }

  // Catch any GaudiExceptions thrown
  catch ( const GaudiException& ) {
    // reset smartid to a default one
    smartid = LHCb::RichSmartID();
    // return failure status
    return false;
  }
}

//=============================================================================
// Converts a point from the global frame to the detector panel frame
//=============================================================================
Gaudi::XYZPoint RichSmartIDs::globalToPDPanel( const Gaudi::XYZPoint& globalPoint ) const {
  return ( globalPoint.z() < 8000.0 ?
                                    // RICH1
               ( globalPoint.y() > 0.0 ? panel( Rich::Rich1, Rich::top )->globalToPDPanelMatrix() * globalPoint
                                       : panel( Rich::Rich1, Rich::bottom )->globalToPDPanelMatrix() * globalPoint )
                                    : // RICH2
               ( globalPoint.x() > 0.0 ? panel( Rich::Rich2, Rich::left )->globalToPDPanelMatrix() * globalPoint
                                       : panel( Rich::Rich2, Rich::right )->globalToPDPanelMatrix() * globalPoint ) );
}

//=============================================================================
// Converts a SIMD position in global coordinates to the local coordinate system
//=============================================================================
RichSmartIDs::SIMDPoint RichSmartIDs::globalToPDPanel( const Rich::DetectorType rich,
                                                       const SIMDPoint&         globalPoint ) const {

  using namespace LHCb::SIMD;

  // mask for top/left
  const auto isTopLeft = ( Rich::Rich1 == rich ? globalPoint.y() > SIMDFP::Zero() : globalPoint.x() > SIMDFP::Zero() );
  if ( all_of( isTopLeft ) ) {
    // all top/left
    return panel( rich, Rich::top )->globalToPDPanelMatrixSIMD() * globalPoint;
  } else if ( none_of( isTopLeft ) ) {
    // all bottom/right
    return panel( rich, Rich::bottom )->globalToPDPanelMatrixSIMD() * globalPoint;
  } else {
    // need a mixture

    // top/left
    const auto pTL = panel( rich, Rich::top )->globalToPDPanelMatrixSIMD() * globalPoint;
    // bottom/right
    const auto pBR = panel( rich, Rich::bottom )->globalToPDPanelMatrixSIMD() * globalPoint;
    // get bottom/right X,Y,Z
    auto X = pBR.X();
    auto Y = pBR.Y();
    auto Z = pBR.Z();
    // set top/left as required
    X( isTopLeft ) = pTL.X();
    Y( isTopLeft ) = pTL.Y();
    Z( isTopLeft ) = pTL.Z();
    // return
    return {X, Y, Z};
  }
}

//=============================================================================
// Returns a list with all valid smartIDs
//=============================================================================
LHCb::RichSmartID::Vector RichSmartIDs::readoutChannelList() const {

  // the list to return
  LHCb::RichSmartID::Vector readoutChannels;

  // Reserve rough size ( RICH1 + RICH2 )
  readoutChannels.reserve( 400000 );

  bool sc = true;

  // Fill for RICH1
  sc = sc && panel( Rich::Rich1, Rich::top )->readoutChannelList( readoutChannels );
  sc = sc && panel( Rich::Rich1, Rich::bottom )->readoutChannelList( readoutChannels );

  // Fill for RICH2
  sc = sc && panel( Rich::Rich2, Rich::left )->readoutChannelList( readoutChannels );
  sc = sc && panel( Rich::Rich2, Rich::right )->readoutChannelList( readoutChannels );

  if ( !sc ) { throw Rich::Exception( "Problem reading readout channel lists from DeRichPDPanels" ); }

  // Sort the list
  SmartIDSorter::sortByRegion( readoutChannels );

  return readoutChannels;
}

//=============================================================================
