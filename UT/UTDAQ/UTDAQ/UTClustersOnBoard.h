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
#ifndef _UTClustersOnBoard_H
#define _UTClustersOnBoard_H

/** @class UTClustersOnBoard UTClustersOnBoard.h
 *
 *  Helper class for keeping track of clusters...
 *
 *  @author A Beiter (based on code by M Needham)
 *  @date   2018-09-04
 */

#include <algorithm>
#include <array>
#include <utility>
#include <vector>

#include "Event/UTCluster.h"
#include "Kernel/UTDAQDefinitions.h"

class UTClustersOnBoard final {

public:
  UTClustersOnBoard( unsigned int nMax );

  typedef std::pair<LHCb::UTCluster*, unsigned int> boardPair;
  typedef std::vector<boardPair>                    ClusterVector;

  void addCluster( LHCb::UTCluster* aCluster );

  ClusterVector clusters() const;

  bool inOverflow() const;

  bool inOverflow( const unsigned int ppx ) const;

  void clear();

private:
  unsigned int                m_maxClustersPerPPx;
  mutable ClusterVector       m_clusCont;
  std::array<unsigned int, 4> m_ppxCount;
};

inline UTClustersOnBoard::UTClustersOnBoard( unsigned int nMax ) : m_maxClustersPerPPx( nMax ) {
  // constructer
  m_clusCont.reserve( 200 );
  clear();
}

inline UTClustersOnBoard::ClusterVector UTClustersOnBoard::clusters() const {
  std::sort( m_clusCont.begin(), m_clusCont.end(),
             []( const boardPair& obj1, const boardPair& obj2 ) { return obj1.second < obj2.second; } );
  return m_clusCont;
}

inline void UTClustersOnBoard::addCluster( LHCb::UTCluster* aCluster ) {

  const unsigned int daqChan = aCluster->tell1Channel();
  const unsigned int ppx     = daqChan / UTDAQ::nStripPerPPx;
  if ( !inOverflow( ppx ) ) {
    m_clusCont.emplace_back( aCluster, daqChan );
    ++m_ppxCount[ppx];
  } else {
    // data went into the void
  }
}

inline bool UTClustersOnBoard::inOverflow( const unsigned int ppx ) const {
  return m_ppxCount[ppx] >= m_maxClustersPerPPx;
}

inline bool UTClustersOnBoard::inOverflow() const {
  auto iter = std::find_if( m_ppxCount.begin(), m_ppxCount.end(),
                            [&]( unsigned int ppx ) { return ppx >= m_maxClustersPerPPx; } );
  return iter != m_ppxCount.end();
}

inline void UTClustersOnBoard::clear() {
  m_clusCont.clear();
  m_ppxCount.fill( 0 );
}

#endif // ClustersOnBoard
