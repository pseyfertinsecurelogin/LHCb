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
#include <mutex>
// Gaudi
#include "GaudiKernel/IUpdateManagerSvc.h"

// LHCb
// Det/DetDesc
#include "DetDesc/Condition.h"

// Local
#include "VPDet/DeVPSensor.h"
#include "getOutputLevel.h"

DeVPSensor::common_t DeVPSensor::m_cache{};

void init_cache( DeVPSensor::common_t& cache ) {
  // Calculate the active area.
  cache.sizeX = cache.nChips * cache.chipSize + ( cache.nChips - 1 ) * cache.interChipDist;
  cache.sizeY = cache.chipSize;

  for ( unsigned int col = 0; col < VP::NSensorColumns; ++col ) {
    // Calculate the x-coordinate of the pixel centre and the pitch.
    const double x0    = ( col / VP::NColumns ) * ( cache.chipSize + cache.interChipDist );
    double       x     = x0 + ( col % VP::NColumns + 0.5 ) * cache.pixelSize;
    double       pitch = cache.pixelSize;
    switch ( col ) {
    case 256:
    case 512:
      // right of chip border
      x -= 0.5 * ( cache.interChipPixelSize - cache.pixelSize );
      pitch = 0.5 * ( cache.interChipPixelSize + cache.pixelSize );
      break;
    case 255:
    case 511:
      // left of chip border
      x += 0.5 * ( cache.interChipPixelSize - cache.pixelSize );
      pitch = cache.interChipPixelSize;
      break;
    case 254:
    case 510:
      // two left of chip border
      pitch = 0.5 * ( cache.interChipPixelSize + cache.pixelSize );
      break;
    }
    cache.local_x[col] = x;
    cache.x_pitch[col] = pitch;
  }
}

//==============================================================================
// Object identification
//==============================================================================
const CLID& DeVPSensor::clID() const { return DeVPSensor::classID(); }

//==============================================================================
// Initialisation method
//==============================================================================
StatusCode DeVPSensor::initialize() {

  auto sc = initOutputLevel( msgSvc(), "DeVPSensor" );
  if ( !sc ) return sc;

  const auto lvl = msgSvc()->outputLevel( "DeVPSensor" );
  m_debug        = lvl <= MSG::DEBUG;

  sc = DetectorElement::initialize();
  if ( !sc.isSuccess() ) {
    msg() << MSG::ERROR << "Cannot initialise DetectorElement" << endmsg;
    return sc;
  }
  // Get the information we need from the DDDB.
  // Some of these parameters are common to all sensors
  // and will be stored in statics.

  // sensor specifics
  //
  m_sensorNumber   = param<int>( "SensorNumber" );
  m_module         = param<int>( "Module" );
  std::string side = param<std::string>( "Side" );
  m_isLeft         = side.find( "Left" ) == 0;

  // commons
  //
  if ( !m_cache.common_cache_valid ) {
    static std::mutex            mtx;
    std::unique_lock<std::mutex> lock( mtx );
    // create the local cache if no-one else did it yet
    if ( !m_cache.common_cache_valid ) {
      m_cache.thickness          = param<double>( "Thickness" );
      m_cache.nChips             = param<int>( "NChips" );
      m_cache.chipSize           = param<double>( "ChipSize" );
      m_cache.interChipDist      = param<double>( "InterChipDist" );
      m_cache.nCols              = param<int>( "NColumns" );
      m_cache.nRows              = param<int>( "NRows" );
      m_cache.pixelSize          = param<double>( "PixelSize" );
      m_cache.interChipPixelSize = param<double>( "InterChipPixelSize" );
      init_cache( m_cache );
      m_cache.common_cache_valid = true;
    }
  }

  m_geometry = geometry();
  // Register geometry conditions.
  updMgrSvc()->registerCondition( this, this->m_geometry, &DeVPSensor::updateGeometryCache );
  sc = updMgrSvc()->update( this );
  if ( !sc.isSuccess() ) {
    msg() << MSG::ERROR << "Failed to update geometry cache." << endmsg;
    return sc;
  }

  return StatusCode::SUCCESS;
}

//==============================================================================
// Calculate the nearest pixel to a point in the global frame.
//==============================================================================
bool DeVPSensor::pointToChannel( const Gaudi::XYZPoint& point, const bool local, LHCb::VPChannelID& channel ) const {

  Gaudi::XYZPoint localPoint = local ? point : globalToLocal( point );
  // Check if the point is in the active area of the sensor.
  if ( !isInActiveArea( localPoint ) ) return false;
  // Set the sensor number.
  channel.setSensor( m_sensorNumber );
  double x0 = 0.;
  for ( unsigned int i = 0; i < m_cache.nChips; ++i ) {
    const double x = localPoint.x() - x0;
    if ( x < m_cache.chipSize + 0.5 * m_cache.interChipDist ) {
      // Set the chip number.
      channel.setChip( i );
      // Set the row and column.
      unsigned int col = 0;
      unsigned int row = 0;
      if ( x > 0. ) {
        col = int( x / m_cache.pixelSize );
        if ( col >= m_cache.nCols ) col = m_cache.nCols - 1;
      }
      if ( localPoint.y() > 0. ) {
        row = int( localPoint.y() / m_cache.pixelSize );
        if ( row >= m_cache.nRows ) row = m_cache.nRows - 1;
      }
      channel.setCol( col );
      channel.setRow( row );
      break;
    }
    x0 += m_cache.chipSize + m_cache.interChipDist;
  }
  return true;
}

//==============================================================================
// Check if a local point is inside the active area of the sensor.
//==============================================================================
bool DeVPSensor::isInActiveArea( const Gaudi::XYZPoint& point ) const {

  if ( point.x() < 0. || point.x() > m_cache.sizeX ) return false;
  if ( point.y() < 0. || point.y() > m_cache.sizeY ) return false;
  return true;
}

//==============================================================================
// Return the size of a pixel with given channel ID.
//==============================================================================
std::pair<double, double> DeVPSensor::pixelSize( const LHCb::VPChannelID channel ) const {

  return {isLong( channel ) ? m_cache.interChipPixelSize : m_cache.pixelSize, m_cache.pixelSize};
}

//==============================================================================
// Return true if a pixel with given channel ID is an elongated pixel.
//==============================================================================
bool DeVPSensor::isLong( const LHCb::VPChannelID channel ) const {
  const unsigned int chip = channel.chip();
  const unsigned int col  = channel.col();
  if ( ( col == 0 && chip > 0 ) || ( col == m_cache.nCols - 1 && chip < m_cache.nChips - 1 ) ) { return true; }
  return false;
}

//==============================================================================
// Cache geometry parameters
//==============================================================================
StatusCode DeVPSensor::updateGeometryCache() {

  m_z = m_geometry->toGlobal( Gaudi::XYZPoint( 0, 0, 0 ) ).z();
  return StatusCode::SUCCESS;
}
