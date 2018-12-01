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
#define CALOFUTUREUTILS_DIGIT2CLUSTERSCONNECTOR_CPP 1 
// ===========================================================================
#include "CaloKernel/CaloException.h"
#include "Event/CaloDigit.h"
#include "Event/CaloCluster.h"
// local 
#include "CaloFutureUtils/Digit2ClustersConnector.h"

// ============================================================================
/** @file Digit2ClustersConnector.cpp 
 *
 * Implementation file for class : Digit2ClusterConnector
 * 
 * @date 01/07/2001 
 * @author Vanya Belyaev Ivan.Belyaev@itep.ru
 */
// ============================================================================

// ============================================================================
/** Constructor
 *   @param   Clus container of CaloFutureCluster objects  
 *   @param   CutOff for "shared" digits 
 */
Digit2ClustersConnector::Digit2ClustersConnector( Clusters*    clus         ,
                                                  const unsigned int CutOff ) 
  : m_cut( CutOff )
{
  if( clus ) { 
      StatusCode sc = load( clus->begin() , 
                            clus->end  () , 
                            CutOff        ); 
      if( sc.isFailure() )
        { throw CaloException( "Digit2ClustersConnector! corrupted data") ; }
  };  
}
// ============================================================================

// ============================================================================
/// addCluster method 
StatusCode Digit2ClustersConnector::addCluster( LHCb::CaloCluster* cluster )
{
  // check arguments 
  if( !cluster ) { return StatusCode::SUCCESS ;}
  // loop over cluster entries 
  for( auto entry = cluster->entries().begin();
       cluster->entries().end() != entry ; ++entry )
    {
      const LHCb::CaloDigit* digit = entry->digit();
      if( digit ) m_map[digit].push_back( ClusterPair( cluster, entry ) );
    }
  ///   
  return StatusCode::SUCCESS;
  ///
}
// ============================================================================

// ============================================================================
/// addCluster method 
StatusCode Digit2ClustersConnector::applyCutOff( const unsigned int CutOff )
{
  // check the value of cut-off 
  const unsigned int cut = ( 0 != CutOff ) ? CutOff : m_cut ;
  if( 0 == cut ) { return StatusCode::SUCCESS ; }
  // loop over the map and erase some entries 
  auto it = m_map.begin();
  while( it != m_map.end() )
    {
      if( it->second.size() >= cut ) {        ++it  ; }
      else { m_map.erase( it ) ; it = m_map.begin() ; }
    }
  ///
  return StatusCode::SUCCESS;
  ///
}
//  ===========================================================================
