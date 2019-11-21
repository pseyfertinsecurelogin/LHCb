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
#pragma once
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <vector>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/PhysicalConstants.h"
#include "GaudiKernel/Point3DTypes.h"
// ============================================================================
// LHCbKernel
// ============================================================================
#include "Kernel/CaloCellID.h"
// ============================================================================
class DeCalorimeter;
// ============================================================================
using CaloNeighbors = LHCb::CaloCellID::Vector;
// ============================================================================
/** @class  CellParam CellParam.h CaloDet/CellParam.h
 *
 *   Auxilliary class to store cell's information inside
 *   the detector element
 *   The constructor creates a null cellID if no argument, as a default
 *   creator is needed for CaloVector...
 *
 */
// ============================================================================
namespace CaloCellQuality {
  // ==========================================================================
  enum Flag {
    OK                = 0,
    Dead              = 1,
    Noisy             = 2,
    Shifted           = 4,
    DeadLED           = 8,
    VeryNoisy         = 16,
    VeryShifted       = 32,
    LEDSaturated      = 64,
    BadLEDTiming      = 128,
    VeryBadLEDTiming  = 256,
    BadLEDRatio       = 512,
    BadLEDOpticBundle = 1024,
    UnstableLED       = 2048,
    StuckADC          = 4096,
    OfflineMask       = 8192
  };
  inline const int         Number       = 15;
  inline const std::string Name[Number] = {
      "OK",          "Dead",         "Noisy",        "Shifted",          "DeadLED",     "VeryNoisy",
      "VeryShifted", "LEDSaturated", "BadLEDTiming", "VeryBadLEDTiming", "BadLEDRatio", "BadLEDOpticBundle",
      "UnstableLED", "StuckADC",     "OfflineMask"};
  inline const std::string qName( int i ) {
    if ( i < Number && i >= 0 ) return Name[i];
    return std::string( "??" );
  }
  // ==========================================================================
} // namespace CaloCellQuality
// ============================================================================
class CellParam final {
  friend class DeCalorimeter;

public:
  /// standard constructor
  CellParam( LHCb::CaloCellID id = LHCb::CaloCellID() ) : m_cellID( std::move( id ) ) {}
  [[nodiscard]] bool                  valid() const { return m_valid; }
  [[nodiscard]] LHCb::CaloCellID      cellID() const { return m_cellID; }
  [[nodiscard]] double                x() const { return m_center.x(); }
  [[nodiscard]] double                y() const { return m_center.y(); }
  [[nodiscard]] double                z() const { return m_center.z() + m_zShower; }
  [[nodiscard]] const Gaudi::XYZPoint center() const { return Gaudi::XYZPoint( x(), y(), z() ); }
  [[nodiscard]] double                size() const { return m_size; }
  [[nodiscard]] double                sine() const { return m_sine; }
  [[nodiscard]] double                nominalGain() const { return m_gain; }
  [[nodiscard]] double                time() const { return m_time + m_dtime; }
  [[nodiscard]] int                   cardNumber() const { return m_cardNumber; }
  [[nodiscard]] int                   cardRow() const { return m_cardRow; }
  [[nodiscard]] int                   cardColumn() const { return m_cardColumn; }
  [[nodiscard]] double                deltaTime() const { return m_dtime; }
  [[nodiscard]] double                zShower() const { return m_zShower; }
  [[nodiscard]] int                   quality() const { return m_quality; }
  [[nodiscard]] int                   l0Constant() const { return m_l0constant; }
  [[nodiscard]] double                calibration() const {
    return m_calibration;
  } // absolute calibration from 'Calibration' condition (T0)
  [[nodiscard]] double ledDataRef() const { return m_ledDataRef; } // <LED> data from Calibration condition (Ref T0)
  [[nodiscard]] double ledMoniRef() const { return m_ledMoniRef; } // <LED/PIN> data from Calibration condition (Ref T0)
  [[nodiscard]] double ledData() const { return m_ledData; }       // <LED> data from Quality condition (current T)
  [[nodiscard]] double ledDataRMS() const { return m_ledDataRMS; } // RMS(LED) from Quality condition  (current T)
  [[nodiscard]] double ledMoni() const { return m_ledMoni; }       // <LED/PIN> data from Quality condition (current T)
  [[nodiscard]] double ledMoniRMS() const { return m_ledMoniRMS; } // RMS(LED/PIN) from Quality condition (current T)
  [[nodiscard]] double ledDataShift() const {
    return ( ledDataRef() > 0 && ledData() > 0 ) ? ledData() / ledDataRef() : 1;
  }
  [[nodiscard]] double gainShift() const {
    return ( ledMoniRef() > 0 && ledMoni() > 0 ) ? ledMoni() / ledMoniRef() : 1;
  }
  [[nodiscard]] double gain() const { return nominalGain() * calibration() / gainShift(); }
  [[nodiscard]] int    numericGain() const { return m_nGain; } // for Prs only
  [[nodiscard]] double pileUpOffset() const { return m_offset; }
  [[nodiscard]] double pileUpOffsetSPD() const { return m_offsetSPD; }
  [[nodiscard]] double pileUpOffsetRMS() const { return m_eoffset; }
  [[nodiscard]] double pileUpOffsetSPDRMS() const { return m_eoffsetSPD; }

  [[nodiscard]] const std::vector<LHCb::CaloCellID>& pins() const { return m_pins; }
  [[nodiscard]] const std::vector<int>&              leds() const { return m_leds; }
  [[nodiscard]] const CaloNeighbors&                 neighbors() const { return m_neighbors; }
  [[nodiscard]] const CaloNeighbors&                 zsupNeighbors() const { return m_zsupNeighbors; }

  // ** To initialize the cell: Geometry, neighbours, gain

  void setValid( bool valid ) { m_valid = valid; }

  void setCenterSize( const Gaudi::XYZPoint& point, double S ) {
    m_center = point;
    m_size   = S;
    //    m_sine   = sqrt( (point.x()*point.x() + point.y()*point.y()) /
    //                 point.mag2() );
    m_sine = 0;
    if ( point.R() != 0 ) m_sine = point.Rho() / point.R();        // MathCore methods (Cartesian3D)
    m_time = point.R() / Gaudi::Units::c_light * Gaudi::Units::ns; // R=sqrt(Mag2)
  }
  void setDeltaTime( double dtime ) { m_dtime = dtime; }
  void setZshower( double dz ) { m_zShower = dz; }

  void addZsupNeighbor( const LHCb::CaloCellID& ID ) { m_zsupNeighbors.push_back( ID ); }
  void addNeighbor( const LHCb::CaloCellID& ID ) { m_neighbors.push_back( ID ); }
  void setNominalGain( const double gain ) { m_gain = gain; }
  void setFeCard( const int num, const int relCol, const int relRow ) {
    m_cardNumber = num;
    m_cardColumn = relCol;
    m_cardRow    = relRow;
  }
  void addPin( LHCb::CaloCellID id ) { m_pins.push_back( id ); }
  void addLed( int id ) { m_leds.push_back( id ); }
  void resetPins() { m_pins.clear(); }
  void resetLeds() { m_leds.clear(); }

  // Calibration & quality
  void setQualityFlag( int quality ) { m_quality = quality; }
  void addQualityFlag( int quality ) { m_quality = m_quality | quality; }
  void setLedData( double ledData, double ledDataRMS ) {
    m_ledData    = ledData;
    m_ledDataRMS = ledDataRMS;
  }
  void setLedMoni( double ledMoni, double ledMoniRMS ) {
    m_ledMoni    = ledMoni;
    m_ledMoniRMS = ledMoniRMS;
  }
  void setLedDataRef( double ledDataRef, double ledMoniRef ) {
    m_ledDataRef = ledDataRef;
    m_ledMoniRef = ledMoniRef;
  }
  void setCalibration( double calib ) { m_calibration = calib; }
  void setL0Constant( int cte ) { m_l0constant = cte; }
  void setNumericGain( int ng ) { m_nGain = ng; } // for Prs only
  void setPileUpOffset( double off, double eoff = 0 ) {
    m_offset  = off;
    m_eoffset = eoff;
  }
  void setPileUpOffsetSPD( double off, double eoff = 0 ) {
    m_offsetSPD  = off;
    m_eoffsetSPD = eoff;
  }

  std::string cellStatus() {
    if ( (CaloCellQuality::Flag)m_quality == CaloCellQuality::OK ) return CaloCellQuality::qName( CaloCellQuality::OK );
    std::ostringstream s( "" );
    s << "| ";
    int q = m_quality;
    int d = 1;
    while ( q > 0 ) {
      if ( ( q & 0x1 ) == 1 ) s << CaloCellQuality::qName( d ) << " | ";
      d += 1;
      q /= 2;
    }
    return s.str();
  }

  bool operator==( const CellParam& c2 ) const { return center() == c2.center() && size() == c2.size(); }

private:
  LHCb::CaloCellID              m_cellID;    ///< ID of the cell
  double                        m_size{0.0}; ///< Cell size
  Gaudi::XYZPoint               m_center{-99999.0 * Gaudi::Units::meter, -99999.0 * Gaudi::Units::meter,
                           0.0 * Gaudi::Units::meter}; ///< Cell centre
  double                        m_sine{0.0};                         ///< To transform E to Et
  double                        m_gain{0.0};                         ///< MeV per ADC count
  double                        m_time{0.0};                         ///< Nominal time of flight from Vertex (ns)
  int                           m_cardNumber{-1};                    ///< Front-end card number
  int                           m_cardRow{-1};                       ///< card row and column
  int                           m_cardColumn{-1};
  CaloNeighbors                 m_neighbors{};     ///< List of neighbors
  CaloNeighbors                 m_zsupNeighbors{}; ///< List of neighbors in same area
  bool                          m_valid{false};
  std::vector<LHCb::CaloCellID> m_pins;
  std::vector<int>              m_leds;
  double                        m_dtime{0.0};
  double                        m_zShower{0.0};
  int                           m_quality{CaloCellQuality::OK};
  double                        m_calibration{1.0};
  int                           m_l0constant{0};
  double                        m_ledDataRef{-1.0};
  double                        m_ledMoniRef{-1.0};
  double                        m_ledData{-1.0};
  double                        m_ledMoni{-1.0};
  double                        m_ledDataRMS{0.0};
  double                        m_ledMoniRMS{0.0};
  int                           m_nGain{0}; // numeric gains (for Prs only)
  double                        m_offset{0.0};
  double                        m_eoffset{0.0};
  double                        m_offsetSPD{0.0};
  double                        m_eoffsetSPD{0.0};
};

// ============================================================================
// The END
// ============================================================================
