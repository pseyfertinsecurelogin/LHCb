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

#include "RichFutureUtils/RichTabulatedRefIndex.h"

// boost
#include "boost/limits.hpp"
#include "boost/numeric/conversion/bounds.hpp"

using namespace Rich::Utils;

float TabulatedRefIndex::refractiveIndex( const Rich::RadIntersection::Vector& intersections,
                                          const float                          energy ) const {
  // loop over all radiator intersections and calculate the weighted average
  // according to the path length in each radiator
  float refIndex( 0 ), totPathL( 0 );
  for ( const auto& R : intersections ) {
    const auto pLength = R.pathLength();
    refIndex += pLength * R.radiator()->refractiveIndex( energy, m_hltMode );
    totPathL += pLength;
  }
  return ( totPathL > 0 ? refIndex / totPathL : refIndex );
}

float TabulatedRefIndex::refractiveIndex( const Rich::RadIntersection::Vector& intersections ) const {
  // loop over all radiator intersections and calculate the weighted average
  // according to the path length in each radiator
  float refIndex( 0 ), totPathL( 0 );
  for ( const auto& R : intersections ) {
    const auto energy  = meanPhotonEnergy( R.radiator()->radiatorID() );
    const auto pLength = R.pathLength();
    refIndex += pLength * R.radiator()->refractiveIndex( energy, m_hltMode );
    totPathL += pLength;
  }
  return ( totPathL > 0 ? refIndex / totPathL : refIndex );
}

float TabulatedRefIndex::refractiveIndexRMS( const Rich::RadIntersection::Vector& intersections ) const {
  // loop over all radiator intersections and calculate the weighted average
  // according to the path length in each radiator
  float refIndexRMS( 0 ), totPathL( 0 );
  for ( const auto& R : intersections ) {
    const auto  pLength = R.pathLength();
    const auto* index   = R.radiator()->refIndex( m_hltMode );
    refIndexRMS += pLength * index->rms( index->minX(), index->maxX(), 100 );
    totPathL += pLength;
  }
  return ( totPathL > 0 ? refIndexRMS / totPathL : refIndexRMS );
}

float TabulatedRefIndex::refractiveIndexSD( const Rich::RadIntersection::Vector& intersections ) const {
  // loop over all radiator intersections and calculate the weighted average
  // according to the path length in each radiator
  float refIndexSD( 0 ), totPathL( 0 );
  for ( const auto& R : intersections ) {
    const auto  pLength = R.pathLength();
    const auto* index   = R.radiator()->refIndex( m_hltMode );
    refIndexSD += pLength * index->standardDeviation( index->minX(), index->maxX(), 100 );
    totPathL += pLength;
  }
  return ( totPathL > 0 ? refIndexSD / totPathL : refIndexSD );
}

float                                                                //
TabulatedRefIndex::thresholdMomentum( const Rich::ParticleIDType id, //
                                      const Rich::RadiatorType   rad ) const {
  auto thresP = boost::numeric::bounds<float>::highest();
  if ( Rich::BelowThreshold != id ) {
    const auto index = refractiveIndex( rad );
    thresP           = ( index > 1.0 //
                   ? m_particleMass[id] / std::sqrt( ( index * index ) - 1.0 )
                   : boost::numeric::bounds<float>::highest() );
  }
  return thresP;
}

float                                                                   //
TabulatedRefIndex::thresholdMomentum( const Rich::ParticleIDType    id, //
                                      const LHCb::RichTrackSegment& trSeg ) const {
  auto thresP = boost::numeric::bounds<float>::highest();
  if ( Rich::BelowThreshold != id ) {
    const auto index = refractiveIndex( trSeg.radIntersections(), trSeg.avPhotonEnergy() );
    thresP           = ( index > 1.0 //
                   ? m_particleMass[id] / std::sqrt( ( index * index ) - 1.0 )
                   : boost::numeric::bounds<float>::highest() );
  }
  return thresP;
}
