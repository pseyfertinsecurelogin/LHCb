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
// ===========================================================================
#ifndef CALOFUTUREEVENT_DIGIT2CLUSTERSCONNECTOR_H
#define CALOFUTUREEVENT_DIGIT2CLUSTERSCONNECTOR_H 1
// ===========================================================================
// Include files
#include "Event/CaloCluster.h"
#include "GaudiKernel/Map.h"

/** @class  Digit2ClustersConnector Digit2ClustersConnector.h
 *
 *  Simple class to perform local "inversion"
 *  of cluster->digits relations.
 *  It behaves like "associator", but designed to be fast, efficient
 *  and LOCAL!
 *
 *  Usage:
 *  ....
 *  typdef  ObjectVector<CaloCluster>  Clusters;
 *  SmartDataPtr<Clusters>             clusters( .... );
 *  Digit2ClustersConnector  table;
 *  table.load( clurs->begin() , clusters->end() , 2 );
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   01/07/2001
 */

class Digit2ClustersConnector final {
public:
  /// useful typedefs
  using Clusters     = LHCb::CaloClusters;
  using EntryPointer = LHCb::CaloCluster::Entries::iterator;
  typedef std::pair<LHCb::CaloCluster*, EntryPointer> ClusterPair;
  using ClusterPairs = std::vector<ClusterPair>;

  typedef GaudiUtils::Map<const LHCb::CaloDigit*, ClusterPairs> Map;
  using It = Map::iterator;
  typedef std::pair<It, It> Pair;

public:
  /** Constructor (in case of corrupted data, throw CaloException)
   *  @param   Clus container of CaloCluster objects
   *  @param   CutOff for "shared" digits
   */
  Digit2ClustersConnector( Clusters* Clus = nullptr, const unsigned int CutOff = 0 );

  /// no copy  constructor!
  Digit2ClustersConnector( const Digit2ClustersConnector& ) = delete;
  /// no assignment!
  Digit2ClustersConnector& operator=( const Digit2ClustersConnector& ) = delete;

  /** Load data and build the inverse table
   *  TYPE - is any type which behaves like iterator, pointing
   *  to CaloCluster* object, e.g. iterator of any type of container
   *  of CaloCluster* objects.
   *
   *  @param first  iterator pointing to first element of CaloCluster sequence
   *  @param last   iterator pointing to last+1 of CaloCluster sequence
   *  @param Cut    cut parameter
   *  @result       status code
   */
  template <class TYPE>
  inline StatusCode load( TYPE first, TYPE last, const unsigned int Cut ) {
    StatusCode sc = StatusCode::SUCCESS;
    while ( first != last && sc.isSuccess() ) { sc = addCluster( *first++ ); }
    if ( sc.isSuccess() ) { sc = applyCutOff( Cut ); }
    return sc;
  }

  inline Map&                     map() { return m_map; };
  [[nodiscard]] inline const Map& map() const { return m_map; };

private:
  /**  Add cluster to the map
   *   @param    clu  pointer to CaloCluster
   *   @result   status code
   */
  StatusCode addCluster( LHCb::CaloCluster* clu );

  /**  Apply cut-off
   *    remove all digits which are connected toless then
   *   certain number of clusters
   *   @param    cut   value of cut-off
   *   @result   status code
   */
  StatusCode applyCutOff( const unsigned int cut );

private:
  Map          m_map; ///< table of relation
  unsigned int m_cut; ///< cut-off value
};

/// ===========================================================================
#endif // DIGIT2CLUSTERSCONNECTOR_H
