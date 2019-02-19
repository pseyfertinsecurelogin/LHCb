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
#ifndef EVENT_MUONTELL1HEADER_H
#define EVENT_MUONTELL1HEADER_H 1

// Include files
#include <ostream>
/** @class MuonTell1Header MuonTell1Header.h Event/MuonTell1Header.h
 *
 *
 *  @author
 *  @date   2008-01-07
 */
class MuonTell1Header final {
  unsigned int m_data = 0;

public:
  /// Standard constructor
  MuonTell1Header( unsigned int data = 0 ) : m_data( data ) {}

  void         setWord( unsigned int word ) { m_data = word; };
  unsigned int getBCN() const {
    // first 8 bits
    return m_data & 0x00FF;
  };

  unsigned int getError() const {
    // second 8 bits
    return ( m_data & 0xFF00 ) >> 8;
  }

  unsigned int getEventError() const {
    // PP_ODE_err bit 0 provided by synch link
    return ( ( m_data >> 8 ) & (unsigned int)1 );
  }

  unsigned int getSYNCHDataErrorInODE() const {
    // PP_ODE_err bit 5
    return ( m_data >> 13 ) & ( (unsigned int)1 );
  }

  unsigned int getSYNCHEventCntErrorInODE() const {
    // PP_ODE_err bit 7
    return ( m_data >> 15 ) & ( (unsigned int)1 );
  }

  unsigned int getSYNCHBCNCntErrorInODE() const {
    // PP_ODE_err bit 6
    return ( m_data >> 14 ) & ( (unsigned int)1 );
  }

  unsigned int getOROfODEErrors() const {
    // PP_ODE_err bits 1-4
    return ( ( m_data >> 9 ) & 0x000F );
  }

  std::ostream& fillStream( std::ostream& s ) const {
    s << "{ ";
    s << "Muon Tell1 Header : "
      << " BCN " << getBCN() << " Tell1 error " << getEventError() << " OR of ODE errors " << getOROfODEErrors()
      << " SYNCH data consistency " << getSYNCHDataErrorInODE() << " SYNCH BC  consistency "
      << getSYNCHBCNCntErrorInODE() << " SYNCH Evt  consistency " << getSYNCHEventCntErrorInODE();

    return s << " }";
  }

  inline friend std::ostream& operator<<( std::ostream& str, const MuonTell1Header& obj ) {
    return obj.fillStream( str );
  }
};

#endif // EVENT_MUONTELL1HEADER_H
