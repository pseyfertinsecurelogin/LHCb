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
#ifndef CALODET_DECALORIMETER_H
#define CALODET_DECALORIMETER_H
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
#include <iostream>
#include <vector>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Plane3DTypes.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Transform3DTypes.h"
// ============================================================================
// Det/DetDesc
// ============================================================================
#include "DetDesc/DetectorElement.h"
#include "DetDesc/IGeometryInfo.h"
// ============================================================================
// Kernel/LHCbKernel
// ============================================================================
#include "Kernel/CaloCellID.h"
// ============================================================================
// Calo/CaloKernel
// ============================================================================
#include "CaloKernel/CaloException.h"
#include "CaloKernel/CaloVector.h"
// ============================================================================
// Det/CaloDet
// ============================================================================
#include "CaloDet/CLIDDeCalorimeter.h"
#include "CaloDet/CaloCardParams.h"
#include "CaloDet/CaloLed.h"
#include "CaloDet/CaloPin.h"
#include "CaloDet/CardParam.h"
#include "CaloDet/CellParam.h"
#include "CaloDet/Tell1Param.h"
// ============================================================================
/// forward declarations
// ============================================================================
class MsgStream;
class DeSubCalorimeter;
class DeSubSubCalorimeter;
// ============================================================================
/// definition of calorimeter planes
namespace CaloPlane {
  // ==========================================================================
  /// definition of calorimeter planes
  enum Plane { Front = 0, Middle, ShowerMax, Back };
  // ==========================================================================
} //                                                 end of namespace CaloPlane
// ============================================================================
/// Locations of DeCalorimeter detector elements in the TDS
namespace DeCalorimeterLocation {
  inline const std::string Spd  = "/dd/Structure/LHCb/DownstreamRegion/Spd";
  inline const std::string Prs  = "/dd/Structure/LHCb/DownstreamRegion/Prs";
  inline const std::string Ecal = "/dd/Structure/LHCb/DownstreamRegion/Ecal";
  inline const std::string Hcal = "/dd/Structure/LHCb/DownstreamRegion/Hcal";
} // namespace DeCalorimeterLocation
// ============================================================================
/** @class DeCalorimeter CaloDet/DeCalorimeter.h
 *
 *  Calorimeter detector element class.
 *
 *  @author Olivier Callot Olivier.Callot@cern.ch
 *  @author Vanya Belyaev  Ivan.Belyaev@itep.ru
 */
class DeCalorimeter : public DetectorElement {
public:
  using SubCalos = std::vector<const DeSubCalorimeter*>;
  ///  Constructors
  DeCalorimeter( const std::string& name = "" );
  ///  object identification
  static const CLID& classID() { return CLID_DeCalorimeter; }
  ///  object identification
  const CLID& clID() const override;
  ///  printout to std::ostream
  std::ostream& printOut( std::ostream& s = std::cerr ) const override;
  ///  printout to MsgStream
  MsgStream& printOut( MsgStream& ) const override;
  ///  initialization method
  StatusCode initialize() override;

  ///  if initialized in a proper way?
  inline bool isInitialized() const { return m_initialized; }

  // geometry setters (temp)
  void setZSize( const double zSize ) { m_zSize = zSize; }
  void setZOffset( const double zOffset ) { m_zOffset = zOffset; }

  //----------
  // getters
  //--------

  // general
  //--------
  const std::string& caloName() const { return m_caloDet; }
  /// calorimeter index, @see namespace CaloCellCode
  CaloCellCode::CaloIndex index() const { return m_caloIndex; }
  // accessing the geometry parameters
  //----------------------------------
  // get constant access to subcalorimeters
  const SubCalos& subCalos() const { return m_subCalos; }
  double          zSize() const { return m_zSize; };
  double          zOffset() const { return m_zOffset; };
  unsigned int    numberOfAreas() const { return m_nArea; };
  // reference plane in the global frame
  Gaudi::Plane3D        plane( const double zLocal ) const;
  inline Gaudi::Plane3D plane( const Gaudi::XYZPoint& point ) const;
  inline Gaudi::Plane3D plane( const CaloPlane::Plane pos ) const;

  // accessing the calibration parameters
  //-------------------------------------
  double maxEtInCenter( unsigned int reg = 0 ) const {
    return ( reg < m_maxEtInCenter.size() ) ? m_maxEtInCenter[reg] : m_maxEtInCenter[0];
  };
  double maxEtSlope( unsigned int reg = 0 ) const {
    return ( reg < m_maxEtSlope.size() ) ? m_maxEtSlope[reg] : m_maxEtSlope[0];
  };
  double pedestalShift() const { return m_pedShift; };
  double pinPedestalShift() const { return m_pinPedShift; };
  double L0EtGain() const { return m_l0Et; };
  double coherentNoise() const { return m_cNoise; };
  double incoherentNoise() const { return m_iNoise; };
  double stochasticTerm() const { return m_stoch; };
  double gainError() const { return m_gainE; };

  // for simulation only
  int    zSupMethod() const { return (int)m_zSupMeth; };
  double zSupThreshold() const { return m_zSup; };
  double spdThreshold( const LHCb::CaloCellID& id ) const {
    return ( mipDeposit() != 0 && caloName() == "SpdDet" ) ? cellGain( id ) / 2. / mipDeposit() : 0.;
  };
  double l0Threshold() const { return m_l0Thresh; };
  double mipDeposit() const { return m_mip; };
  double dynamicsSaturation() const { return m_dyn; };
  double fractionFromPrevious() const { return m_prev; };
  double numberOfPhotoElectrons( unsigned int area = 0 ) const { return ( area < m_phe.size() ) ? m_phe[area] : 0.; };
  double l0EtCorrection( unsigned int area = 0 ) const { return ( area < m_l0Cor.size() ) ? m_l0Cor[area] : 1.; };
  double activeToTotal() const { return m_activeToTotal; };

  // accessing the hardware parameter(s)
  // ---------------------------------
  unsigned int adcMax() const { return m_adcMax; };

  // accessing the reconstruction parameter(s)
  double zShowerMax() const { return m_zShowerMax; };

  // accessing readout parameters
  //-----------------------------
  unsigned int numberOfCells() const { return m_cells.size() - numberOfPins(); };
  unsigned int numberOfCards() const { return m_feCards.size(); };
  unsigned int numberOfTell1s() const { return m_tell1Boards.size(); };
  unsigned int numberOfPins() const { return m_pins.size(); };
  unsigned int numberOfLeds() const { return m_leds.size(); };
  unsigned int numberOfInvalidCells() const {
    return std::count_if( m_cells.begin(), m_cells.end(),
                          []( const CellParam& c ) { return !c.cellID().isPin() && !c.valid(); } );
  };
  unsigned int pinArea() const { return m_pinArea; };
  ///  Cell Parameters
  bool   valid( const LHCb::CaloCellID& ) const;
  double cellX( const LHCb::CaloCellID& ) const;
  double cellY( const LHCb::CaloCellID& ) const;
  double cellZ( const LHCb::CaloCellID& ) const;
  double cellSize( const LHCb::CaloCellID& ) const;
  double cellSine( const LHCb::CaloCellID& ) const;
  double cellGain( const LHCb::CaloCellID& ) const;
  // convert ADC to energy in MeV for a given cellID
  double cellEnergy( int adc, LHCb::CaloCellID id ) const {
    double offset = isPinId( id ) ? pinPedestalShift() : pedestalShift();
    return cellGain( id ) * ( (double)adc - offset );
  };
  // reverse operation : convert energy in MeV to ADC
  double cellADC( double energy, LHCb::CaloCellID id ) const {
    double offset = isPinId( id ) ? pinPedestalShift() : pedestalShift();
    double gain   = cellGain( id );
    return ( gain > 0 ) ? floor( energy / gain + offset + 0.5 ) : 0;
  };
  bool isSaturated( double energy, LHCb::CaloCellID id ) const { return cellADC( energy, id ) + 256 >= adcMax(); }

  double                cellTime( const LHCb::CaloCellID& ) const;
  const Gaudi::XYZPoint cellCenter( const LHCb::CaloCellID& ) const;
  const CaloNeighbors&  neighborCells( const LHCb::CaloCellID& ) const;
  const CaloNeighbors&  zsupNeighborCells( const LHCb::CaloCellID& ) const;
  bool                  hasQuality( const LHCb::CaloCellID&, CaloCellQuality::Flag flag ) const;
  bool                  isDead( const LHCb::CaloCellID& ) const;
  bool                  isNoisy( const LHCb::CaloCellID& ) const;
  bool                  isShifted( const LHCb::CaloCellID& ) const;
  bool                  hasDeadLED( const LHCb::CaloCellID& ) const;
  bool                  isVeryNoisy( const LHCb::CaloCellID& ) const;
  bool                  isVeryShifted( const LHCb::CaloCellID& ) const;
  // from cellId to  serial number and vice-versa
  int              cellIndex( const LHCb::CaloCellID& ) const;
  LHCb::CaloCellID cellIdByIndex( const unsigned int ) const;
  // from cell to FEB
  bool isReadout( const LHCb::CaloCellID& ) const;
  int  cardNumber( const LHCb::CaloCellID& ) const;
  int  cardRow( const LHCb::CaloCellID& ) const;
  int  cardColumn( const LHCb::CaloCellID& ) const;
  void cardAddress( const LHCb::CaloCellID& ID, int& card, int& row, int& col ) const;
  //  FEB card
  int                                  nCards() const { return m_feCards.size(); }
  int                                  downCardNumber( const int card ) const;
  int                                  leftCardNumber( const int card ) const;
  int                                  cornerCardNumber( const int card ) const;
  int                                  previousCardNumber( const int card ) const;
  void                                 cardNeighbors( const int card, int& down, int& left, int& corner ) const;
  int                                  validationNumber( const int card ) const;
  int                                  selectionType( const int card ) const;
  int                                  cardArea( const int card ) const;
  int                                  cardFirstRow( const int card ) const;
  int                                  cardLastColumn( const int card ) const;
  int                                  cardLastRow( const int card ) const;
  int                                  cardFirstColumn( const int card ) const;
  int                                  cardFirstValidRow( const int card ) const;
  int                                  cardLastValidRow( const int card ) const;
  int                                  cardFirstValidColumn( const int card ) const;
  int                                  cardLastValidColumn( const int card ) const;
  LHCb::CaloCellID                     firstCellID( const int card ) const;
  LHCb::CaloCellID                     lastCellID( const int card ) const;
  LHCb::CaloCellID                     cardCellID( const int card, const int row, const int col ) const;
  int                                  cardCrate( const int card ) const;
  int                                  cardSlot( const int card ) const;
  int                                  cardCode( const int card ) const;
  const std::vector<LHCb::CaloCellID>& cardChannels( const int card ) const;
  const std::vector<LHCb::CaloCellID>& pinChannels( const LHCb::CaloCellID ) const;
  const std::vector<LHCb::CaloCellID>& ledChannels( const int led ) const;
  const std::vector<int>&              pinLeds( const LHCb::CaloCellID ) const;
  const std::vector<int>&              cellLeds( const LHCb::CaloCellID ) const;
  int                                  cardIndexByCode( const int crate, const int slot ) const;
  int                                  cardToTell1( const int card ) const;
  // from validation to Hcal FEB
  int validationToHcalFEB( const int validation, const unsigned int slot ) const;
  // Tell1s
  int              nTell1s() const { return m_tell1Boards.size(); }
  std::vector<int> tell1ToCards( const int tell1 ) const;
  // CardParam/Tell1Param/CellParam
  const CardParam  cardParam( const int card ) const { return ( card < nCards() ) ? m_feCards[card] : CardParam(); }
  const Tell1Param tell1Param( const int tell1 ) const {
    return ( tell1 < nTell1s() ) ? m_tell1Boards[tell1] : Tell1Param( -1 );
  }
  const CellParam cellParam( const LHCb::CaloCellID& id ) const { return m_cells[id]; }
  const CaloPin   caloPin( const LHCb::CaloCellID& id ) const { return m_pins[id]; }
  const CaloLed   caloLed( const int led ) const { return ( led < (int)m_leds.size() ) ? m_leds[led] : -1; }
  //  More complex functions
  LHCb::CaloCellID Cell( const Gaudi::XYZPoint& point ) const;
  const CellParam* Cell_( const Gaudi::XYZPoint& point ) const;
  // Collections
  const CaloVector<CellParam>&   cellParams() const { return m_cells; }
  const CaloVector<CaloPin>&     caloPins() const { return m_pins; }
  const std::vector<CaloLed>&    caloLeds() const { return m_leds; }
  const std::vector<CardParam>&  cardParams() const { return m_feCards; }
  const std::vector<Tell1Param>& tell1Params() const { return m_tell1Boards; }
  // PIN flag
  bool isParasiticCard( const int card ) const { return ( card < nCards() ) ? m_feCards[card].isParasitic() : false; };
  bool isPmtCard( const int card ) const { return ( card < nCards() ) ? m_feCards[card].isPmtCard() : false; };
  bool isPinCard( const int card ) const { return ( card < nCards() ) ? m_feCards[card].isPinCard() : false; };
  bool isPinTell1( const int tell1 ) const { return ( tell1 < nTell1s() ) ? m_tell1Boards[tell1].readPin() : false; };
  bool isPinId( LHCb::CaloCellID id ) const { return ( (unsigned)m_pinArea == id.area() ) ? true : false; }
  // pileUp subtraction parameters
  int pileUpSubstractionMethod() const { return m_puMeth; }
  int pileUpSubstractionBin() const { return m_puBin; }
  int pileUpSubstractionMin() const { return m_puMin; }

private:
  ///  Initialization method for building the cells/cards/tell1/PIN layout
  StatusCode buildCells();
  StatusCode buildCards();
  StatusCode buildTell1s();
  StatusCode buildMonitoring();
  StatusCode getCalibration();
  StatusCode getPileUpOffset();
  StatusCode getL0Calibration();
  StatusCode getLEDReference();
  StatusCode getQuality();
  StatusCode getNumericGains();

private:
  DeCalorimeter( const DeCalorimeter& ) = delete;
  DeCalorimeter& operator=( const DeCalorimeter& ) = delete;

  // Calo id
  std::string             m_caloDet;
  CaloCellCode::CaloIndex m_caloIndex = static_cast<CaloCellCode::CaloIndex>( -1 );

  // init flag
  bool m_initialized = false;

  // geometry
  double       m_YToXSizeRatio = std::numeric_limits<double>::signaling_NaN();
  double       m_zSize         = std::numeric_limits<double>::signaling_NaN();
  double       m_zOffset       = std::numeric_limits<double>::signaling_NaN();
  SubCalos     m_subCalos;
  unsigned int m_nArea{};

  // hardware
  int    m_adcMax{};
  int    m_coding{};
  int    m_centralHoleX{};
  int    m_centralHoleY{};
  int    m_maxRowCol{};
  int    m_firstRowUp{};
  double m_centerRowCol = std::numeric_limits<double>::signaling_NaN();

  // calibration
  std::vector<double> m_maxEtInCenter;
  std::vector<double> m_maxEtSlope;
  double              m_pedShift      = std::numeric_limits<double>::signaling_NaN();
  double              m_pinPedShift   = std::numeric_limits<double>::signaling_NaN();
  double              m_l0Et          = std::numeric_limits<double>::signaling_NaN();
  double              m_activeToTotal = std::numeric_limits<double>::signaling_NaN();
  double              m_stoch         = std::numeric_limits<double>::signaling_NaN();
  double              m_gainE         = std::numeric_limits<double>::signaling_NaN();
  double              m_cNoise        = std::numeric_limits<double>::signaling_NaN();
  double              m_iNoise        = std::numeric_limits<double>::signaling_NaN();
  double              m_zSupMeth      = std::numeric_limits<double>::signaling_NaN();
  double              m_zSup          = std::numeric_limits<double>::signaling_NaN();
  double              m_mip           = std::numeric_limits<double>::signaling_NaN();
  double              m_dyn           = std::numeric_limits<double>::signaling_NaN();
  double              m_prev          = std::numeric_limits<double>::signaling_NaN();
  double              m_l0Thresh      = std::numeric_limits<double>::signaling_NaN();
  std::vector<double> m_phe;
  std::vector<double> m_l0Cor;
  int                 m_puMeth = -1;
  int                 m_puBin  = 0;
  int                 m_puMin  = 0;

  // reconstruction
  double m_zShowerMax = std::numeric_limits<double>::signaling_NaN();

  // readout
  int m_pinArea = -1;

  ///  Collections
  std::vector<LHCb::CaloCellID>   m_empty;
  CaloVector<CellParam>           m_cells;
  CaloVector<CaloPin>             m_pins;
  std::vector<CardParam>          m_feCards;
  std::vector<Tell1Param>         m_tell1Boards;
  std::vector<CaloLed>            m_leds;
  std::vector<int>                m_pinTell1s;
  std::map<int, std::vector<int>> m_valCards;

  // conditions
  SmartRef<Condition> m_pileUp;
  SmartRef<Condition> m_calib;
  SmartRef<Condition> m_l0calib;
  SmartRef<Condition> m_gain;
  SmartRef<Condition> m_quality;
  SmartRef<Condition> m_LEDReference;
  SmartRef<Condition> m_reco;
  SmartRef<Condition> m_hardware;
  SmartRef<Condition> m_readout;
  SmartRef<Condition> m_monitor;
  SmartRef<Condition> m_numericGains;
  bool                loadCondition( SmartRef<Condition>& cond, std::string name, bool mandatory = false );

  StatusCode updHardware();
  StatusCode updGeometry();
  StatusCode updGain();
  StatusCode updCalib();
  StatusCode updL0Calib();
  StatusCode updLEDReference();
  StatusCode updQuality();
  StatusCode updReadout();
  StatusCode updMonitor();
  StatusCode updReco();
  StatusCode updNumGains();
  StatusCode updPileUp();
  // ===========================================================================
  /** ouput operator for class DeCalorimeter
   *  @see DeCalorimeter
   *  @param os reference to standard STL/STD output stream
   *  @param de reference to DeCalorimeter object
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   */
  friend std::ostream& operator<<( std::ostream& os, const DeCalorimeter& de ) { return de.printOut( os ); }
  // ===========================================================================
  // ===========================================================================
  /** ouput operator for class DeCalorimeter
   *  @see DeCalorimeter
   *  @see MsgStream
   *  @param os reference to Gaudi message output stream
   *  @param de reference to DeCalorimeter object
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   */
  friend MsgStream& operator<<( MsgStream& os, const DeCalorimeter& de ) { return de.printOut( os ); }
  // ===========================================================================
};

// ===========================================================================
/** ouput operator for class DeCalorimeter
 *  @see DeCalorimeter
 *  @param os reference to standard STL/STD output stream
 *  @param de pointer to DeCalorimeter object
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 */
inline std::ostream& operator<<( std::ostream& os, const DeCalorimeter* de ) {
  return de ? ( os << *de ) : ( os << " DeCalorimeter* points to NULL!" << std::endl );
}
// ===========================================================================

// ===========================================================================
/** ouput operator for class DeCalorimeter
 *  @see DeCalorimeter
 *  @see MsgStream
 *  @param os reference to Gaudi message output stream
 *  @param de pointer to DeCalorimeter object
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 */
inline MsgStream& operator<<( MsgStream& os, const DeCalorimeter* de ) {
  return de ? ( os << *de ) : ( os << " DeCalorimeter* points to NULL!" << endmsg );
}
// ===========================================================================

// ===========================================================================
//  validity flag for the cell
// ===========================================================================
inline bool DeCalorimeter::valid( const LHCb::CaloCellID& ID ) const { return m_cells[ID].valid(); }
// ===========================================================================
//  Quality flag for the cell
// ===========================================================================
inline bool DeCalorimeter::hasQuality( const LHCb::CaloCellID& ID, CaloCellQuality::Flag flag ) const {
  int quality = m_cells[ID].quality();
  return flag == CaloCellQuality::OK ? quality == 0 : ( ( quality & flag ) != 0 );
}
inline bool DeCalorimeter::isDead( const LHCb::CaloCellID& ID ) const {
  return hasQuality( ID, CaloCellQuality::Dead );
}
inline bool DeCalorimeter::isNoisy( const LHCb::CaloCellID& ID ) const {
  return hasQuality( ID, CaloCellQuality::Noisy );
}
inline bool DeCalorimeter::isShifted( const LHCb::CaloCellID& ID ) const {
  return hasQuality( ID, CaloCellQuality::Shifted );
}
inline bool DeCalorimeter::hasDeadLED( const LHCb::CaloCellID& ID ) const {
  return hasQuality( ID, CaloCellQuality::DeadLED );
}
inline bool DeCalorimeter::isVeryNoisy( const LHCb::CaloCellID& ID ) const {
  return hasQuality( ID, CaloCellQuality::VeryNoisy );
}
inline bool DeCalorimeter::isVeryShifted( const LHCb::CaloCellID& ID ) const {
  return hasQuality( ID, CaloCellQuality::VeryShifted );
}
// ===========================================================================
//  x-position of center of the cell
// ===========================================================================
inline double DeCalorimeter::cellX( const LHCb::CaloCellID& ID ) const { return m_cells[ID].x(); }
//  ===========================================================================
//  y-position of center of the cell
//  ===========================================================================
inline double DeCalorimeter::cellY( const LHCb::CaloCellID& ID ) const { return m_cells[ID].y(); }

// ===========================================================================
//  z-position of center of the cell
// ===========================================================================
inline double DeCalorimeter::cellZ( const LHCb::CaloCellID& ID ) const { return m_cells[ID].z(); }

// ===========================================================================
//  cell size
// ===========================================================================
inline double DeCalorimeter::cellSize( const LHCb::CaloCellID& ID ) const { return m_cells[ID].size(); }

// ===========================================================================
//  sine function for given cell
// ===========================================================================
inline double DeCalorimeter::cellSine( const LHCb::CaloCellID& ID ) const { return m_cells[ID].sine(); }

// ===========================================================================
//  PM gain for given cell
// ===========================================================================
inline double DeCalorimeter::cellGain( const LHCb::CaloCellID& ID ) const { return m_cells[ID].gain(); }

// ===========================================================================
//  Timing for for given cell
// ===========================================================================
inline double DeCalorimeter::cellTime( const LHCb::CaloCellID& ID ) const { return m_cells[ID].time(); }

// ===========================================================================
//  cell center
// ===========================================================================
inline const Gaudi::XYZPoint DeCalorimeter::cellCenter( const LHCb::CaloCellID& ID ) const {
  return m_cells[ID].center();
}

// ============================================================================
//  list of neighbour cells
// ============================================================================
inline const CaloNeighbors& DeCalorimeter::neighborCells( const LHCb::CaloCellID& ID ) const {
  return m_cells[ID].neighbors();
}

// ============================================================================
//  list of neighbour cells
// ============================================================================
inline const CaloNeighbors& DeCalorimeter::zsupNeighborCells( const LHCb::CaloCellID& ID ) const {
  return m_cells[ID].zsupNeighbors();
}

// ============================================================================
//  From ID to cell serial number and vice-versa
// ============================================================================
inline int DeCalorimeter::cellIndex( const LHCb::CaloCellID& ID ) const { return m_cells.index( ID ); }

// ============================================================================
//  From ID to cell serial number and vice-versa
// ============================================================================
inline LHCb::CaloCellID DeCalorimeter::cellIdByIndex( const unsigned int num ) const {
  return ( ( num < m_cells.size() ) ? ( m_cells.begin() + num )->cellID() : LHCb::CaloCellID() );
}

inline bool DeCalorimeter::isReadout( const LHCb::CaloCellID& ID ) const {
  return ( m_cells[ID].cardNumber() >= 0 ) ? true : false;
}
// ============================================================================
//  card number
// ============================================================================
inline int DeCalorimeter::cardNumber( const LHCb::CaloCellID& ID ) const { return m_cells[ID].cardNumber(); }
// ============================================================================
//  card row
// ============================================================================
inline int DeCalorimeter::cardRow( const LHCb::CaloCellID& ID ) const { return m_cells[ID].cardRow(); }
// ============================================================================
//  card column
// ============================================================================
inline int DeCalorimeter::cardColumn( const LHCb::CaloCellID& ID ) const { return m_cells[ID].cardColumn(); }
// ============================================================================
//  card address
// ============================================================================
inline void DeCalorimeter::cardAddress( const LHCb::CaloCellID& ID, int& card, int& row, int& column ) const {
  card   = m_cells[ID].cardNumber();
  row    = m_cells[ID].cardRow();
  column = m_cells[ID].cardColumn();
}

// ============================================================================
//  down card number
// ============================================================================
inline int DeCalorimeter::downCardNumber( const int card ) const {
  return ( card < nCards() ) ? m_feCards[card].downNumber() : -1;
}

// ============================================================================
//  left card number
// ============================================================================
inline int DeCalorimeter::leftCardNumber( const int card ) const {
  return ( card < nCards() ) ? m_feCards[card].leftNumber() : -1;
}

// ============================================================================
//  corder card number
// ============================================================================
inline int DeCalorimeter::cornerCardNumber( const int card ) const {
  return ( card < nCards() ) ? m_feCards[card].cornerNumber() : -1;
}

// ============================================================================
//  previous card number
// ============================================================================
inline int DeCalorimeter::previousCardNumber( const int card ) const {
  return ( card < nCards() ) ? m_feCards[card].previousNumber() : -1;
}

// ============================================================================
//  validation card number
// ============================================================================
inline int DeCalorimeter::validationNumber( const int card ) const {
  return ( card < nCards() ) ? m_feCards[card].validationNumber() : -1;
}

// ============================================================================
//  selection board type
// ============================================================================
inline int DeCalorimeter::selectionType( const int card ) const {
  return ( card < nCards() ) ? m_feCards[card].selectionType() : -1;
}

// ============================================================================
//  card neighbours
// ============================================================================
inline void DeCalorimeter::cardNeighbors( const int card, int& down, int& left, int& corner ) const {
  down   = -1;
  left   = -1;
  corner = -1;
  if ( card < nCards() ) {
    down   = m_feCards[card].downNumber();
    left   = m_feCards[card].leftNumber();
    corner = m_feCards[card].cornerNumber();
  }
}

// ============================================================================
//  card area
// ============================================================================
inline int DeCalorimeter::cardArea( const int card ) const { return ( card < nCards() ) ? m_feCards[card].area() : -1; }

// ============================================================================
//  card first row
// ============================================================================
inline int DeCalorimeter::cardFirstRow( const int card ) const {
  return ( card < nCards() ) ? m_feCards[card].firstRow() : -1;
}

// ============================================================================
//  card first column
// ============================================================================
inline int DeCalorimeter::cardFirstColumn( const int card ) const {
  return ( card < nCards() ) ? m_feCards[card].firstColumn() : -1;
}

// ============================================================================
//  card last row
// ============================================================================
inline int DeCalorimeter::cardLastRow( const int card ) const {
  return ( card < nCards() ) ? m_feCards[card].lastRow() : -1;
}

// ============================================================================
//  card last column
// ============================================================================
inline int DeCalorimeter::cardLastColumn( const int card ) const {
  return ( card < nCards() ) ? m_feCards[card].lastColumn() : -1;
}

// ============================================================================
//  valid boundaries :  first/last col/row with a valid cell
// ============================================================================
inline int DeCalorimeter::cardFirstValidRow( const int card ) const {
  LHCb::CaloCellID cell = firstCellID( card );
  if ( valid( cell ) ) return cell.row();

  for ( int irow = cardFirstRow( card ); irow <= cardLastRow( card ); ++irow ) {
    for ( int icol = cardFirstColumn( card ); icol <= cardLastColumn( card ); ++icol ) {
      LHCb::CaloCellID test( cell.calo(), cell.area(), irow, icol );
      if ( valid( test ) ) return irow;
    }
  }
  return -1;
}

inline int DeCalorimeter::cardFirstValidColumn( const int card ) const {
  LHCb::CaloCellID cell = firstCellID( card );
  if ( valid( cell ) ) return cell.col();
  for ( int irow = cardFirstRow( card ); irow <= cardLastRow( card ); ++irow ) {
    for ( int icol = cardFirstColumn( card ); icol <= cardLastColumn( card ); ++icol ) {
      LHCb::CaloCellID test( cell.calo(), cell.area(), irow, icol );
      if ( valid( test ) ) return icol;
    }
  }
  return -1;
}

inline int DeCalorimeter::cardLastValidRow( const int card ) const {
  LHCb::CaloCellID cell = lastCellID( card );
  if ( valid( cell ) ) return cell.row();

  for ( int irow = cardLastRow( card ); irow >= cardFirstRow( card ); --irow ) {
    for ( int icol = cardLastColumn( card ); icol >= cardFirstColumn( card ); --icol ) {
      LHCb::CaloCellID test( cell.calo(), cell.area(), irow, icol );
      if ( valid( test ) ) return irow;
    }
  }
  return -1;
}
inline int DeCalorimeter::cardLastValidColumn( const int card ) const {
  LHCb::CaloCellID cell = lastCellID( card );
  if ( valid( cell ) ) return cell.col();

  for ( int irow = cardLastRow( card ); irow >= cardFirstRow( card ); --irow ) {
    for ( int icol = cardLastColumn( card ); icol >= cardFirstColumn( card ); --icol ) {
      LHCb::CaloCellID test( cell.calo(), cell.area(), irow, icol );
      if ( valid( test ) ) return icol;
    }
  }
  return -1;
}

// ===========================================================================
//  ID of the bottom left cell, of the top right cell, of the specified cell
// ===========================================================================
inline LHCb::CaloCellID DeCalorimeter::firstCellID( const int card ) const {

  return ( card < nCards() ) ? LHCb::CaloCellID( m_caloIndex, m_feCards[card].area(), m_feCards[card].firstRow(),
                                                 m_feCards[card].firstColumn() )
                             : LHCb::CaloCellID();
}

// ===========================================================================
//  ID of the bottom left cell, of the top right cell, of the specified cell
// ===========================================================================
inline LHCb::CaloCellID DeCalorimeter::lastCellID( const int card ) const {
  return ( card < nCards() )
             ? LHCb::CaloCellID( m_caloIndex, m_feCards[card].area(), m_feCards[card].firstRow() + nRowCaloCard - 1,
                                 m_feCards[card].firstColumn() + nColCaloCard - 1 )
             : LHCb::CaloCellID();
}

// ===========================================================================
//  ID of the bottom left cell, of the top right cell, of the specified cell
// ===========================================================================
inline LHCb::CaloCellID DeCalorimeter::cardCellID( const int card, const int row, const int col ) const {
  return ( card < nCards() ) ? LHCb::CaloCellID( m_caloIndex, m_feCards[card].area(), m_feCards[card].firstRow() + row,
                                                 m_feCards[card].firstColumn() + col )
                             : LHCb::CaloCellID();
}

// ============================================================================
//  FEcard -> Tell1
// ============================================================================
inline int DeCalorimeter::cardToTell1( const int card ) const {
  return ( card < nCards() ) ? m_feCards[card].tell1() : -1;
}
// ============================================================================
//  Validation -> Hcal FEB
// ============================================================================
inline int DeCalorimeter::validationToHcalFEB( const int validation, const unsigned int slot ) const {
  auto it = m_valCards.find( validation );
  if ( it != m_valCards.end() ) {
    const auto& hcalFe = it->second;
    if ( slot < hcalFe.size() ) return hcalFe[slot];
  }
  return -1;
}
// ============================================================================
//  Tell1 -> FECards
// ============================================================================
inline std::vector<int> DeCalorimeter::tell1ToCards( const int tell1 ) const {
  return ( tell1 >= 0 && tell1 < nTell1s() ) ? m_tell1Boards[tell1].feCards() : std::vector<int>();
}

// ============================================================================
//  card Id & content
// ============================================================================
inline int DeCalorimeter::cardCrate( const int card ) const {
  return ( card < nCards() ) ? m_feCards[card].crate() : -1;
}

inline int DeCalorimeter::cardSlot( const int card ) const { return ( card < nCards() ) ? m_feCards[card].slot() : -1; }

inline int DeCalorimeter::cardCode( const int card ) const { return ( card < nCards() ) ? m_feCards[card].code() : -1; }

inline const std::vector<LHCb::CaloCellID>& DeCalorimeter::cardChannels( const int card ) const {
  return card < nCards() ? m_feCards[card].ids() : m_empty;
}

inline const std::vector<LHCb::CaloCellID>& DeCalorimeter::pinChannels( const LHCb::CaloCellID id ) const {
  return m_pins[id].cells();
}

inline const std::vector<LHCb::CaloCellID>& DeCalorimeter::ledChannels( const int led ) const {
  return led < (int)m_leds.size() ? m_leds[led].cells() : m_empty;
}

inline const std::vector<int>& DeCalorimeter::pinLeds( const LHCb::CaloCellID id ) const { return m_pins[id].leds(); }

inline const std::vector<int>& DeCalorimeter::cellLeds( const LHCb::CaloCellID id ) const { return m_cells[id].leds(); }

inline int DeCalorimeter::cardIndexByCode( const int crate, const int slot ) const {
  auto i = std::find_if( m_feCards.begin(), m_feCards.end(),
                         [&]( const CardParam& c ) { return c.crate() == crate && c.slot() == slot; } );
  return i != m_feCards.end() ? i - m_feCards.begin() : -1;
}
// ============================================================================
/// Return a reference (tilted) plane
// ============================================================================
inline Gaudi::Plane3D DeCalorimeter::plane( const CaloPlane::Plane pos ) const {
  switch ( pos ) {
  case CaloPlane::Front:
    return plane( m_zOffset - m_zSize / 2. );
  case CaloPlane::ShowerMax:
    return plane( m_zShowerMax );
  case CaloPlane::Middle:
    return plane( m_zOffset );
  case CaloPlane::Back:
    return plane( m_zOffset + m_zSize / 2. );
  default:
    return plane( m_zOffset );
  }
}
// ============================================================================
/// return a 3D-plane, which contain the given 3D-point in the global system
// ============================================================================
inline Gaudi::Plane3D DeCalorimeter::plane( const Gaudi::XYZPoint& global ) const {
  return plane( geometry()->toLocal( global ).Z() );
}

#endif //    CALODET_DECALORIMETER_H
