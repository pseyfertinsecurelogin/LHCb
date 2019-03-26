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
// Calculate the pixel and fraction corresponding to a global point.
//==============================================================================
bool DeVPSensor::pointToChannel( const Gaudi::XYZPoint& point, const bool local, LHCb::VPChannelID& channel,
                                 std::pair<double, double>& fraction ) const {

  Gaudi::XYZPoint localPoint = local ? point : globalToLocal( point );
  // Check if the point is in the active area of the sensor.
  if ( !isInActiveArea( localPoint ) ) return false;
  fraction.first  = 0.;
  fraction.second = 0.;
  // Set the sensor number.
  channel.setSensor( m_sensorNumber );
  const double step = m_cache.chipSize + m_cache.interChipDist;
  double       x0   = 0.;
  for ( unsigned int i = 0; i < m_cache.nChips; ++i ) {
    if ( localPoint.x() < x0 + step ) {
      // Set the chip number.
      channel.setChip( i );
      // Calculate the column number.
      const double       x    = localPoint.x() - x0;
      const double       fcol = x / m_cache.pixelSize - 0.5;
      const unsigned int icol = fcol > 0. ? int( fcol ) : 0;
      // Set column and inter-pixel fraction.
      if ( icol <= 0 ) {
        channel.setCol( 0 );
        if ( 0 == i ) {
          if ( fcol > 0. ) fraction.first = fcol;
        } else {
          // First column has elongated pixels.
          const double pitch = 0.5 * ( m_cache.pixelSize + m_cache.interChipPixelSize );
          fraction.first     = x / pitch;
        }
      } else if ( icol >= m_cache.nCols - 1 ) {
        channel.setCol( m_cache.nCols - 1 );
        if ( i == m_cache.nChips - 1 ) {
          fraction.first = fcol - icol;
        } else {
          // Last column has elongated pixels.
          if ( x < m_cache.chipSize ) {
            // This point is assigned to the last but one pixel.
            channel.setCol( m_cache.nCols - 2 );
            const double pitch = 0.5 * ( m_cache.pixelSize + m_cache.interChipPixelSize );
            fraction.first     = 1. - ( m_cache.chipSize - x ) / pitch;
          } else {
            // Point is in inter-chip region.
            fraction.first = ( x - m_cache.chipSize ) / m_cache.interChipPixelSize;
          }
        }
      } else {
        channel.setCol( icol );
        fraction.first = fcol - icol;
        if ( icol == m_cache.nCols - 2 && i < m_cache.nChips - 1 ) {
          fraction.first *= m_cache.pixelSize / m_cache.interChipPixelSize;
        }
      }
      // Set the row and inter-pixel fraction.
      const double       frow = localPoint.y() / m_cache.pixelSize - 0.5;
      const unsigned int irow = frow > 0. ? int( frow ) : 0;
      if ( irow <= 0 ) {
        channel.setRow( 0 );
        if ( frow > 0. ) fraction.second = frow;
      } else {
        channel.setRow( irow );
        fraction.second = frow - irow;
      }
      break;
    }
    x0 += step;
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
