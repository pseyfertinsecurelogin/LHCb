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
#include "DetDesc/IGeometryInfo.h"

#include "UTDet/DeUTDetector.h"
#include "UTDet/DeUTLayer.h"
#include "UTDet/DeUTSector.h"
#include "UTDet/DeUTStation.h"
#include "UTDet/DeUTStave.h"
#include "UTDet/UTDetFun.h"

#include "Kernel/Trajectory.h"
#include "Kernel/UTNames.h"

#include "GaudiKernel/GaudiException.h"

// STL
#include <algorithm>
#include <cassert>
#include <numeric>

using namespace LHCb;

/** @file DeUTDetector.cpp
 *
 *  Implementation of class: DeUTDetector
 *
 *  All UT elements are modification for TT element classes
 *  that were originally written by Matt Needham)
 *
 *  @author Andy Beiter (based on code by Jianchun Wang, Matt Needham)
 *  @date   2018-09-04
 *
 */

DeUTDetector::DeUTDetector( const std::string& name ) : DetectorElement( name ) { m_sectors.clear(); }

const CLID& DeUTDetector::clID() const { return DeUTDetector::classID(); }

StatusCode DeUTDetector::initialize() {
  // init the base class
  StatusCode sc = DetectorElement::initialize();
  if ( sc.isFailure() ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
  } else {
    for ( auto iStation = this->childBegin(); this->childEnd() != iStation; ++iStation ) {
      DeUTStation* tStation = dynamic_cast<DeUTStation*>( *iStation );
      if ( tStation ) { m_stations.push_back( tStation ); }
    } // iStation

    if ( !m_stations.empty() ) {
      setFirstStation( m_stations.front()->id() );
    } else {
      // no stations - this is an error
      MsgStream msg( msgSvc(), name() );
      msg << MSG::ERROR << "No child elements !" << endmsg;
      sc = StatusCode::FAILURE;
    }
    // make a flat list of sectors
    flatten();
    setOffset();
    if ( !m_sectors.empty() ) {
      setNstrip( m_sectors.front()->nStrip() * m_sectors.size() );
      m_sMap.reserve( m_sectors.size() );
      for ( auto i : m_sectors ) { m_sMap.insert( i->elementID().uniqueSector(), i ); }
    }
  }
  return sc;
}

void DeUTDetector::setFirstStation( const unsigned int iStation ) { m_firstStation = iStation; }

int DeUTDetector::sensitiveVolumeID( const Gaudi::XYZPoint& point ) const {
  DeUTDetector* nonConstThis = const_cast<DeUTDetector*>( this );
  DeUTSector*   sector       = nonConstThis->findSector( point );
  if ( !sector ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "sensitiveVolumeID: no sensitive volume at " << point << endmsg;
    return -1;
  }
  return sector->elementID();
}

DeUTStation* DeUTDetector::findStation( const UTChannelID aChannel ) const {

  // return pointer to the station from channel
  auto iter = std::find_if( m_stations.begin(), m_stations.end(),
                            [&]( const DeUTStation* s ) { return s->contains( aChannel ); } );
  return iter != m_stations.end() ? *iter : nullptr;
}

DeUTStation* DeUTDetector::findStation( const Gaudi::XYZPoint& point ) const {

  // return pointer to the station from point in global system
  auto iter = std::find_if( m_stations.begin(), m_stations.end(),
                            [&]( const DeUTStation* s ) { return s->isInside( point ); } );
  return iter != m_stations.end() ? *iter : nullptr;
}

DeUTLayer* DeUTDetector::findLayer( const UTChannelID aChannel ) const {
  // return pointer to the layer from channel
  auto iter =
      std::find_if( m_layers.begin(), m_layers.end(), [&]( const DeUTLayer* l ) { return l->contains( aChannel ); } );
  return iter != m_layers.end() ? *iter : nullptr;
}

DeUTLayer* DeUTDetector::findLayer( const Gaudi::XYZPoint& point ) const {
  // return pointer to the layer from point
  auto iter =
      std::find_if( m_layers.begin(), m_layers.end(), [&]( const DeUTLayer* l ) { return l->isInside( point ); } );
  return iter != m_layers.end() ? *iter : nullptr;
}

DeUTSector* DeUTDetector::findSector( const std::string& nickname ) const {
  // return pointer to the sector from the nickname
  auto iter = std::find_if( m_sectors.begin(), m_sectors.end(), UTDetFun::equal_by_name<DeUTSector*>( nickname ) );
  return iter != m_sectors.end() ? *iter : nullptr;
}

DeUTLayer* DeUTDetector::findLayer( const std::string& nickname ) const {
  // return pointer to the sector from the nickname
  auto iter = std::find_if( m_layers.begin(), m_layers.end(), UTDetFun::equal_by_name<DeUTLayer*>( nickname ) );
  return iter != m_layers.end() ? *iter : nullptr;
}

DeUTStation* DeUTDetector::findStation( const std::string& nickname ) const {
  // return pointer to the sector from the nickname
  auto iter = std::find_if( m_stations.begin(), m_stations.end(), UTDetFun::equal_by_name<DeUTStation*>( nickname ) );
  return iter != m_stations.end() ? *iter : nullptr;
}

DeUTDetector::Sectors DeUTDetector::disabledSectors() const {
  Sectors     disabled;
  const auto& vec = sectors();
  std::copy_if( vec.begin(), vec.end(), std::back_inserter( disabled ),
                []( const DeUTSector* s ) { return s->sectorStatus() == DeUTSector::ReadoutProblems; } );
  return disabled;
}

std::vector<LHCb::UTChannelID> DeUTDetector::disabledBeetles() const {

  std::vector<LHCb::UTChannelID> disabledBeetles;
  for ( auto s : sectors() ) {
    auto bStatus = s->beetleStatus();
    for ( unsigned int i = 0; i < bStatus.size(); ++i ) {
      if ( bStatus[i] == DeUTSector::ReadoutProblems ) {
        const unsigned int firstStripOnBeetle = ( i * LHCbConstants::nStripsInBeetle ) + 1;
        disabledBeetles.push_back( s->stripToChan( firstStripOnBeetle ) );
      }
    } // i
  }
  return disabledBeetles;
}

LHCb::LineTraj<double> DeUTDetector::trajectory( const LHCb::LHCbID& id, const double offset ) const {
  // look up the trajectory
  if ( !id.isUT() ) {
    throw GaudiException( "The LHCbID is not of UT type!", "DeUTDetector.cpp", StatusCode::FAILURE );
  }
  DeUTSector* aSector = findSector( id.utID() );
  if ( !aSector ) { throw GaudiException( "Failed to find sector", "DeUTDetector.cpp", StatusCode::FAILURE ); }
  return aSector->trajectory( id.utID(), offset );
}

LHCb::LineTraj<double> DeUTDetector::trajectoryFirstStrip( const LHCb::LHCbID& id ) const {
  if ( !id.isUT() ) {
    throw GaudiException( "The LHCbID is not of UT type!", "DeUTDetector.cpp", StatusCode::FAILURE );
  }

  DeUTSector* aSector = findSector( id.utID() );
  if ( !aSector ) { throw GaudiException( "Failed to find sector", "DeUTDetector.cpp", StatusCode::FAILURE ); }
  return aSector->trajectoryFirstStrip();
}

LHCb::LineTraj<double> DeUTDetector::trajectoryLastStrip( const LHCb::LHCbID& id ) const {
  if ( !id.isUT() ) {
    throw GaudiException( "The LHCbID is not of UT type!", "DeUTDetector.cpp", StatusCode::FAILURE );
  }
  DeUTSector* aSector = findSector( id.utID() );
  if ( !aSector ) { throw GaudiException( "Failed to find sector", "DeUTDetector.cpp", StatusCode::FAILURE ); }
  return aSector->trajectoryLastStrip();
}

double DeUTDetector::fractionActive() const {
  return std::accumulate( m_sectors.begin(), m_sectors.end(), 0.0,
                          [&]( double fA, const DeUTSector* s ) { return fA + s->fractionActive(); } ) /
         double( m_sectors.size() );
}

DeUTSector* DeUTDetector::findSector( const Gaudi::XYZPoint& aPoint ) const {
  DeUTSector*  aSector  = nullptr;
  DeUTStation* tStation = findStation( aPoint );
  if ( tStation ) {
    DeUTStation* aStation = dynamic_cast<DeUTStation*>( tStation );
    DeUTLayer*   aLayer   = aStation->findLayer( aPoint );
    if ( aLayer ) {
      DeUTStave* aStave = aLayer->findStave( aPoint );
      if ( aStave ) { aSector = aStave->findSector( aPoint ); }
    }
  }
  return aSector;
}

DeUTBaseElement* DeUTDetector::findTopLevelElement( const std::string& nickname ) const {
  if ( nickname.find( "UT" ) == std::string::npos ) return nullptr;

  const UTChannelID chan = UTNames().stringToChannel( nickname );
  if ( chan.sector() != 0 ) return this->DeUTDetector::findSector( chan );
  if ( chan.layer() != 0 ) return findLayer( chan );
  if ( chan.station() != 0u ) return findStation( chan );

  return nullptr;
}

void DeUTDetector::flatten() {
  m_sectors.reserve( 1200 );
  for ( auto ptrstation : m_stations ) {
    for ( auto ptrlayer : dynamic_cast<DeUTStation*>( ptrstation )->layers() ) {
      m_layers.push_back( ptrlayer );
      const auto& vecptrsectors = ptrlayer->sectors();
      m_sectors.insert( m_sectors.end(), vecptrsectors.begin(), vecptrsectors.end() );
    }
  }
}

void DeUTDetector::setOffset() {

  bool beginit     = true;
  uint curr_region = 0;
  uint topoffset   = 0;

  assert( m_stations.size() == NBSTATION && "setOffset of UTDetector assumes a wrong number of station" );

  // add one offset for each region of each stave of each layer of each station
  std::size_t ir = 0;
  for ( auto ptrstation : m_stations ) {
    for ( auto ptrlayer : dynamic_cast<DeUTStation*>( ptrstation )->layers() ) {

      assert( dynamic_cast<DeUTStation*>( ptrstation )->layers().size() == NBLAYER &&
              "setOffset of UTDetector assumes a wrong number of layers per station" );
#ifndef NDEBUG
      const auto curr_nbreg = ir;
#endif
      for ( const auto& ptrstave : ptrlayer->staves() ) {
        const auto& vecptrsectors = ptrstave->sectors();
        // add an offset only if we are on a new region
        if ( beginit || curr_region != ptrstave->detRegion() ) {
          curr_region = ptrstave->detRegion();
          beginit     = false;
          assert( ir < m_offset.size() );
          m_offset[ir] = topoffset;
          ++ir;
        }

        // move the total to the last offset
        topoffset += vecptrsectors.size();
      }
      assert( ( ir - curr_nbreg ) == NBREGION && "setOffset of UTDetector assumes a wrong number of region per layer" );
    }
  }
}

DeUTSector* DeUTDetector::getSector( unsigned int station, //
                                     unsigned int layer,   //
                                     unsigned int region,  //
                                     unsigned int sector,  //
                                     unsigned int uniqueSector ) const {

  DeUTSector* res = nullptr;

  // helper to get index according to station/layer/region
  constexpr std::array<std::array<std::array<uint, NBREGION>, NBLAYER>, NBSTATION> get_idx_offset{
      {{{{0, 1, 2}, {3, 4, 5}}}, {{{6, 7, 8}, {9, 10, 11}}}}};

  // form array indices.
  const unsigned int ista( station - 1u ), ilay( layer - 1u ), ireg( region - 1u ), isec( sector - 1u );

  // check for valid info...
  if ( ista < NBSTATION && ilay < NBLAYER && ireg < NBREGION ) {

    // get index offset corresponding to the station/layer/region we want
    auto idx_offset = ( ( get_idx_offset[ista] )[ilay] )[ireg];

    const auto i = getOffset( idx_offset ) + isec;
    // assert( i < m_sectors.size() );
    res = ( i < m_sectors.size() ? m_sectors[i] : nullptr );

    // debug check to be sure we find the same sector as findSector
    assert( [&]() {
      auto goodsector = m_sMap.find( uniqueSector )->second;
      return ( goodsector == res );
    }() && "getSector was not able to find the same UT Sector as findSector" );
    // avoid warning in non debug mode
    (void)uniqueSector;
  }

  return res;
}
