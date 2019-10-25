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
#ifndef CALODET_DESUBSUBCALORIMETER_H
#define CALODET_DESUBSUBCALORIMETER_H
#include "CaloDet/CLIDDeSubSubCalorimeter.h"
#include "DetDesc/DetectorElement.h"
#include "GaudiKernel/MsgStream.h"

class MsgStream;

/** @class DeSubSubCalorimeter DeSubSubCalorimeter.h "CaloDet/DeSubSubCalorimeter.h"
 *
 *  specific detector element class (calo areas)
 *
 *  @author Olivier Callot Olivier.Callot@cern.ch
 *  @author Vanya Belyaev  Ivan.Belyaev@itep.ru
 *
 */

class DeSubSubCalorimeter : public DetectorElement {
public:
  /// Constructors
  DeSubSubCalorimeter( const std::string& name = "" ) : DetectorElement( name ) {}
  /// object identification
  static const CLID& classID() { return CLID_DeSubSubCalorimeter; }
  /// printout to standard STL stream
  std::ostream& printOut( std::ostream& os = std::cerr ) const override;
  /// printout to standard Gaudi stream
  MsgStream& printOut( MsgStream& ) const override;
  /// object identification
  const CLID& clID() const override;
  /// standard initialization
  StatusCode initialize() override;
  /// get/set subSubcalo  size
  inline double xSize() const { return m_xSize; }
  inline double ySize() const { return m_ySize; }
  inline void   setXSize( const double x ) { m_xSize = x; }
  inline void   setYSize( const double y ) { m_ySize = y; }
  /// get/set cell  size
  inline double cellSize() const { return m_cellSize; }
  inline void   setCellSize( const double s ) { m_cellSize = s; }
  /// get/set area ID
  inline int  area() const { return m_area; };
  inline void setArea( const int a ) { m_area = a; }
  ///
  friend std::ostream& operator<<( std::ostream& os, const DeSubSubCalorimeter& de ) { return de.printOut( os ); }
  friend MsgStream&    operator<<( MsgStream& os, const DeSubSubCalorimeter& de ) { return de.printOut( os ); }

private:
  double m_cellSize{0.0}; ///< Cell dimension
  double m_xSize{0.0};
  double m_ySize{0.0};
  int    m_area{-1}; ///< Calo Area ID
};
// ============================================================================
inline std::ostream& operator<<( std::ostream& os, const DeSubSubCalorimeter* de ) {
  return de ? ( os << *de ) : ( os << " DeSubSubCalorimeter* points to NULL!" << std::endl );
}
// ============================================================================
// ============================================================================
inline MsgStream& operator<<( MsgStream& os, const DeSubSubCalorimeter* de ) {
  return de ? ( os << *de ) : ( os << " DeSubSubCalorimeter* points to NULL!" << endmsg );
}
// ============================================================================

// ============================================================================
#endif //   CALODET_DESUSUBBCALORIMETER_H
