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
#ifndef CALODET_DESUBCALORIMETER_H
#define CALODET_DESUBCALORIMETER_H
#include "CaloDet/CLIDDeSubCalorimeter.h"
#include "DetDesc/DetectorElement.h"
#include "GaudiKernel/MsgStream.h"
#include "Kernel/CaloCellID.h"

class MsgStream;
class DeSubSubCalorimeter;

/** @class DeSubCalorimeter DeSubCalorimeter.h "CaloDet/DeSubCalorimeter.h"
 *
 *  specific detector element class (calo halves)
 *
 *  @author Olivier Deschamps odescham@in2p3.fr
 *
 */

class DeSubCalorimeter : public DetectorElement {
public:
  enum class Side { C = -1, A = +1 };
  /// Constructors
  DeSubCalorimeter( const std::string& name = "" );
  /// object identification
  static const CLID& classID() { return CLID_DeSubCalorimeter; }
  /// printout to standard STL stream
  std::ostream& printOut( std::ostream& os = std::cerr ) const override;
  /// printout to standard Gaudi stream
  MsgStream& printOut( MsgStream& ) const override;
  /// object identification
  const CLID& clID() const override;
  /// standard initialization
  StatusCode initialize() override;
  /// get side
  Side        side() const { return m_side; };
  std::string sideName() const {
    switch ( m_side ) {
    case Side::A:
      return "A-side";
    case Side::C:
      return "C-side";
    default:
      return "??"; // dark side??
    }
  };
  /// set side (0=C=Right / 1=A=Left)
  void setSide( Side s ) { m_side = s; };
  ///
  const auto& subSubCalos() const { return m_subSubCalos; };

  friend std::ostream& operator<<( std::ostream& os, const DeSubCalorimeter& de ) { return de.printOut( os ); }
  friend MsgStream&    operator<<( MsgStream& os, const DeSubCalorimeter& de ) { return de.printOut( os ); }

private:
  std::vector<const DeSubSubCalorimeter*> m_subSubCalos;
  Side                                    m_side = Side::C; ///< Calo side (0=C-side=Right , 1=A-side=Left)
};
// ============================================================================
inline std::ostream& operator<<( std::ostream& os, const DeSubCalorimeter* de ) {
  return de ? ( os << *de ) : ( os << " DeSubCalorimeter* points to NULL!" << std::endl );
}
inline MsgStream& operator<<( MsgStream& os, const DeSubCalorimeter* de ) {
  return de ? ( os << *de ) : ( os << " DeSubCalorimeter* points to NULL!" << endmsg );
}
// ============================================================================
#endif //   CALODET_DESUBCALORIMETER_H
