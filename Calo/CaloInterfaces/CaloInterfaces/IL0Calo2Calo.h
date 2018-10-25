/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef CALOINTERFACES_IL0CALO2CALO_H 
#define CALOINTERFACES_IL0CALO2CALO_H 1

// Include files
// from STL
#include <vector>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// from LHCb
#include "Event/CaloCluster.h"
#include "Event/L0CaloCandidate.h"


/** @class IL0Calo2Calo IL0Calo2Calo.h CaloInterfaces/IL0Calo2Calo.h
 *  
 * Tool to get a list of CaloClusters (owned by TES) in the vicinity of the input L0CaloCandidate(s),
 * if necessary invoking decoding and clusterization.
 *
 *  @author Dmitry Golubkov
 *  @date   2009-07-27
 */
struct IL0Calo2Calo : extend_interfaces<IAlgTool>
{
  // ==========================================================================
  /** obtain CaloClusters corresponding to one L0CaloCandidate
   *
   * Get a list of CaloClusters in the vicinity of the L0CaloCandidate,
   * if necessary invoke decoding and clusterization.
   * 
   * The obtained clusters are owned by TES.
   *
   * @param clusters (OUTPUT) vector of pointers of Calo clusters 
   * @param candidate(INPUT)  pointer to L0CaloCandidate
   * @param level    (INPUT)  number of neigbour levels around the cell for the ICaloClusterization tool
   */
  virtual StatusCode clusterize
  ( std::vector<LHCb::CaloCluster*>&      clusters , 
    const LHCb::L0CaloCandidate*          candidate, 
    const unsigned int                    level     ) const = 0;
  // ==========================================================================
  /** obtain CaloClusters corresponding to one L0CaloCandidate
   *
   * Get a list of CaloClusters in the vicinity of the L0CaloCandidate,
   * if necessary invoke decoding and clusterization.
   *
   * @param clusters (OUTPUT) vector of pointers of Calo clusters 
   * @param candidate(INPUT)  pointer to L0CaloCandidate
   */
  virtual StatusCode clusterize
  ( std::vector<LHCb::CaloCluster*>&      clusters, 
    const LHCb::L0CaloCandidate*          candidate ) const = 0;
  // ==========================================================================
  /** obtain CaloClusters corresponding to vector L0CaloCandidates
   *
   * Get a list of CaloClusters in the vicinity of a vector of L0CaloCandidates,
   * invoke decoding and clusterization if necessary.
   *
   * @param clusters  (OUTPUT) vector of pointers of Calo clusters 
   * @param candidates(INPUT)  container of pointers to L0CaloCandidate
   * @param level     (INPUT)  number of neigbour levels around the cell for the ICaloClusterization tool
   */
  virtual StatusCode clusterize
  ( std::vector<LHCb::CaloCluster*>&      clusters , 
    const LHCb::L0CaloCandidates*         candidate, 
    const unsigned int                    level     ) const = 0;
  // ==========================================================================
  /** obtain CaloClusters corresponding to vector of L0CaloCandidates
   *
   * Get a list of CaloClusters in the vicinity of a vector of L0CaloCandidates,
   * invoke decoding and clusterization if necessary.
   *
   * @param clusters  (OUTPUT) vector of pointers of Calo clusters 
   * @param candidates(INPUT)  container of pointers to L0CaloCandidate
   */
  virtual StatusCode clusterize
  ( std::vector<LHCb::CaloCluster*>&      clusters, 
    const LHCb::L0CaloCandidates*         candidate ) const = 0;
  // ==========================================================================
  /** obtain CaloClusters around a CaloCellID
   *
   * Get a list of CaloClusters in the vicinity of the CaloCellID,
   * if necessary invoke decoding and clusterization.
   * 
   * @param clusters (OUTPUT) vector of pointers of Calo clusters 
   * @param cellID   (INPUT)  pointer to CaloCellID
   * @param level    (INPUT)  number of neigbour levels around the cell for the ICaloClusterization tool
   */
  virtual StatusCode clusterize
  ( std::vector<LHCb::CaloCluster*>&      clusters , 
    const LHCb::CaloCellID&               cellID, 
    const unsigned int                    level     ) const = 0;
  // ==========================================================================
  /** obtain CaloClusters around a CaloCellID
   *
   * Get a list of CaloClusters in the vicinity of the CaloCellID,
   * if necessary invoke decoding and clusterization.
   * 
   * @param clusters (OUTPUT) vector of pointers of Calo clusters 
   * @param cellID   (INPUT)  pointer to CaloCellID
   */
  virtual StatusCode clusterize
  ( std::vector<LHCb::CaloCluster*>&      clusters, 
    const LHCb::CaloCellID&               cellID   ) const = 0;
  // ==========================================================================

  /** static interface identification
   *
   * \return the interface ID
   */
  DeclareInterfaceID(IL0Calo2Calo, 1, 1 );
};
#endif // CALOINTERFACES_IL0CALO2CALO_H
