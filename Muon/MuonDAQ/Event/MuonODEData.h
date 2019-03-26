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
#ifndef MUONDAQ_MUONODEDATA_H
#define MUONDAQ_MUONODEDATA_H 1

#include <algorithm>
#include <array>

/** @class MuonODEData MuonODEData.h Event/MuonODEData.h
 *
 *
 *  @author Alessia Satta
 *  @date   2007-12-05
 */

class MuonODEData final {

  std::array<unsigned int, 35> m_data = {{0}};

public:
  void resetODEData() { *this = {}; };

  void setData( const std::array<unsigned int, 35>& d ) { m_data = d; }

  unsigned int getWord( int index ) const { return m_data[index]; };

  unsigned int getTDC( int index, int ch ) const {
    int shift_rigth = 32 - ( ch + 1 ) * 4;
    return ( m_data[index] << shift_rigth ) >> 28;
  }

  unsigned int getTDC( int ch ) const { return getTDC( ch / 8 + 1, ch % 8 ); }
};
#endif // MUONDAQ_MUONODEDATA_H
