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
#ifndef _STClustersOnBoard_H
#define _STClustersOnBoard_H

/** @class STClustersOnBoard STClustersOnBoard.h
 *
 *  Helper class for keeping track of clusters...
 *
 *  @author M Needham
 *  @date   2004-08-01
 */

#include <algorithm>
#include <array>
#include <utility>
#include <vector>

#include "Event/STCluster.h"
#include "Kernel/STDAQDefinitions.h"

class STClustersOnBoard final {

public:
  STClustersOnBoard( unsigned int nMax );

  typedef std::pair<LHCb::STCluster*, unsigned int> boardPair;
  typedef std::vector<boardPair>                    ClusterVector;

  void addCluster( LHCb::STCluster* aCluster );

  ClusterVector clusters() const;

  bool inOverflow() const;

  bool inOverflow( const unsigned int ppx ) const;

  void clear();

private:
  unsigned int                m_maxClustersPerPPx;
  mutable ClusterVector       m_clusCont;
  std::array<unsigned int, 4> m_ppxCount;
};

inline STClustersOnBoard::STClustersOnBoard( unsigned int nMax ) : m_maxClustersPerPPx( nMax ) {
  // constructer
  m_clusCont.reserve( 200 );
  clear();
}

inline STClustersOnBoard::ClusterVector STClustersOnBoard::clusters() const {
  std::sort( m_clusCont.begin(), m_clusCont.end(),
             []( const boardPair& obj1, const boardPair& obj2 ) { return obj1.second < obj2.second; } );
  return m_clusCont;
}

inline void STClustersOnBoard::addCluster( LHCb::STCluster* aCluster ) {

  const unsigned int daqChan = aCluster->tell1Channel();
  const unsigned int ppx     = daqChan / STDAQ::nStripPerPPx;
  if ( !inOverflow( ppx ) ) {
    m_clusCont.emplace_back( aCluster, daqChan );
    ++m_ppxCount[ppx];
  } else {
    // data went into the void
  }
}

inline bool STClustersOnBoard::inOverflow( const unsigned int ppx ) const {
  return m_ppxCount[ppx] >= m_maxClustersPerPPx;
}

inline bool STClustersOnBoard::inOverflow() const {
  auto iter = std::find_if( m_ppxCount.begin(), m_ppxCount.end(),
                            [&]( unsigned int ppx ) { return ppx >= m_maxClustersPerPPx; } );
  return iter != m_ppxCount.end();
}

inline void STClustersOnBoard::clear() {
  m_clusCont.clear();
  m_ppxCount.fill( 0 );
}

#endif // ClustersOnBoard
