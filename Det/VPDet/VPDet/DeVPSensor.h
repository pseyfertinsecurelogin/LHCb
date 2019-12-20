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
#ifndef VPDET_DEVPSENSOR_H
#define VPDET_DEVPSENSOR_H 1

#include <array>
#include <atomic>

// Gaudi
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Point3DTypes.h"

// Det/Desc
#include "DetDesc/DetectorElement.h"
#include "DetDesc/IGeometryInfo.h"
// Kernel/LHCbKernel
#include "Kernel/VPChannelID.h"
#include "Kernel/VPConstants.h"

// Unique class identifier
static const CLID CLID_DeVPSensor = 1008205;

/** @class DeVPSensor DeVPSensor.h VPDet/DeVPSensor.h
 *
 *  Detector element class for a single VP sensor
 *  @author Victor Coco
 *  @date   2009-05-14
 */

class DeVPSensor : public DetectorElement {

public:
  /// Constructor
  using DetectorElement::DetectorElement;

  /// Object identification
  static const CLID& classID() { return CLID_DeVPSensor; }
  const CLID&        clID() const override;

  /// Initialise the sensor.
  StatusCode initialize() override;

  /// Calculate the nearest channel to a given point.
  bool pointToChannel( const Gaudi::XYZPoint& point, const bool local, LHCb::VPChannelID& channel ) const;

  /// Return the pixel size.
  std::pair<double, double> pixelSize( const LHCb::VPChannelID channel ) const;

  /// Return true if the pixel is elongated.
  bool isLong( const LHCb::VPChannelID channel ) const;

  /// Determine if a local 3-d point is inside the sensor active area.
  bool isInActiveArea( const Gaudi::XYZPoint& point ) const;

  /// Determine whether a point is within the sensor
  bool isInsideSensor( const double x, const double y ) const {
    return isInActiveArea( globalToLocal( Gaudi::XYZPoint{x, y, z()} ) );
  }

  /// Convert local position to global position
  Gaudi::XYZPoint localToGlobal( const Gaudi::XYZPoint& point ) const { return m_geometry->toGlobal( point ); }
  /// Convert global position to local position
  Gaudi::XYZPoint globalToLocal( const Gaudi::XYZPoint& point ) const { return m_geometry->toLocal( point ); }
  /// Convert local direction to global direction
  Gaudi::XYZVector localToGlobal( const Gaudi::XYZVector& point ) const { return m_geometry->toGlobal( point ); }
  /// Convert global direction to local direction
  Gaudi::XYZVector globalToLocal( const Gaudi::XYZVector& point ) const { return m_geometry->toLocal( point ); }

  /// Return the z position of the sensor in the global frame
  double z() const { return m_z; }

  /// Return the sensor number
  unsigned int sensorNumber() const { return m_sensorNumber; }
  /// Return the module number
  unsigned int module() const { return m_module; }
  /// Return station number (station comprises left and right module)
  unsigned int station() const { return m_module >> 1; }

  /// Return true for x < 0 side of the detector
  bool isRight() const { return !m_isLeft; }
  /// Return true for x > 0 side of the detector
  bool isLeft() const { return m_isLeft; }

  /// Return sensor thickness in mm.
  double siliconThickness() const { return m_cache.thickness; }

  /// Return array of cached local x-coordinates by column
  const std::array<double, VP::NSensorColumns>& xLocal() const { return m_cache.local_x; }

  /// Return array of cached x pitches by column
  const std::array<double, VP::NSensorColumns>& xPitch() const { return m_cache.x_pitch; }

  /// Calculate the local position of a given pixel.
  Gaudi::XYZPoint channelToLocalPoint( const LHCb::VPChannelID channel ) const {
    const double x = m_cache.local_x[channel.scol()];
    const double y = ( channel.row() + 0.5 ) * m_cache.pixelSize;
    return Gaudi::XYZPoint( x, y, 0.0 );
  }
  /// Calculate the global position of a given pixel.
  Gaudi::XYZPoint channelToGlobalPoint( const LHCb::VPChannelID channel ) const {
    return localToGlobal( channelToLocalPoint( channel ) );
  }
  /// Calculate the position of a given pixel.
  Gaudi::XYZPoint channelToPoint( const LHCb::VPChannelID& channel, const bool local ) const {
    const double          x = m_cache.local_x[channel.scol()];
    const double          y = ( channel.row() + 0.5 ) * m_cache.pixelSize;
    const Gaudi::XYZPoint point( x, y, 0.0 );
    return ( local ? point : localToGlobal( point ) );
  }
  double       chipSize() const { return m_cache.chipSize; }
  double       interChipDist() const { return m_cache.interChipDist; }
  double       pixelSize() const { return m_cache.pixelSize; }
  double       interChipPixelSize() const { return m_cache.interChipPixelSize; }
  unsigned int numChips() const { return m_cache.nChips; }
  unsigned int numColumns() const { return m_cache.nCols; }
  unsigned int numRows() const { return m_cache.nRows; }

private:
  IGeometryInfo* m_geometry = nullptr;

  unsigned int m_sensorNumber;
  unsigned int m_module;
  bool         m_isLeft;

  /// Global Z position
  double m_z;

  struct common_t {
    common_t() = default;
    /// Dimensions of the sensor active area
    double sizeX;
    double sizeY;
    double thickness;
    /// Number of chips per ladder
    unsigned int nChips;
    /// Length of chip active area
    double chipSize;
    /// Distance between two chips
    double interChipDist;
    /// Number of columns and rows
    unsigned int nCols;
    unsigned int nRows;
    /// Cell size of pixels
    double pixelSize;
    /// Cell size in column direction of elongated pixels
    double interChipPixelSize;
    /// Cache of local x-cooordinates
    std::array<double, VP::NSensorColumns> local_x;
    /// Cache of x-pitch
    std::array<double, VP::NSensorColumns> x_pitch;
    /// Cache validity, so we create it only once on startup
    std::atomic<bool> common_cache_valid{false};
  };

  static common_t m_cache;

  /// Output level flag
  bool m_debug = false;

  /// Message stream
  mutable std::unique_ptr<MsgStream> m_msg;
  /// On-demand access to message stream
  MsgStream& msg() const {
    if ( UNLIKELY( !m_msg ) ) m_msg.reset( new MsgStream( msgSvc(), "DeVPSensor" ) );
    return *m_msg;
  }

  /// Update geometry cache when the alignment changes
  StatusCode updateGeometryCache();

  friend void init_cache( common_t& );
};
#endif
