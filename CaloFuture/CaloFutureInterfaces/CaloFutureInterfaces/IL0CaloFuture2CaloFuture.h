#ifndef CALOFUTUREINTERFACES_IL0CALOFUTURE2CALOFUTURE_H 
#define CALOFUTUREINTERFACES_IL0CALOFUTURE2CALOFUTURE_H 1

// Include files
// from STL
#include <vector>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// from LHCb
#include "Event/CaloCluster.h"
#include "Event/L0CaloCandidate.h"


/** @class IL0CaloFuture2CaloFuture IL0CaloFuture2CaloFuture.h CaloFutureInterfaces/IL0CaloFuture2CaloFuture.h
 *  
 * Tool to get a list of CaloFutureClusters (owned by TES) in the vicinity of the input L0CaloFutureCandidate(s),
 * if necessary invoking decoding and clusterization.
 *
 *  @author Dmitry Golubkov
 *  @date   2009-07-27
 */
struct IL0CaloFuture2CaloFuture : extend_interfaces<IAlgTool>
{
  // ==========================================================================
  /** obtain CaloFutureClusters corresponding to one L0CaloFutureCandidate
   *
   * Get a list of CaloFutureClusters in the vicinity of the L0CaloFutureCandidate,
   * if necessary invoke decoding and clusterization.
   * 
   * The obtained clusters are owned by TES.
   *
   * @param clusters (OUTPUT) vector of pointers of Calo clusters 
   * @param candidate(INPUT)  pointer to L0CaloCandidate
   * @param level    (INPUT)  number of neigbour levels around the cell for the ICaloFutureClusterization tool
   */
  virtual StatusCode clusterize
  ( std::vector<LHCb::CaloCluster*>&      clusters , 
    const LHCb::L0CaloCandidate*          candidate, 
    const unsigned int                    level     ) const = 0;
  // ==========================================================================
  /** obtain CaloFutureClusters corresponding to one L0CaloFutureCandidate
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
   * @param level     (INPUT)  number of neigbour levels around the cell for the ICaloFutureClusterization tool
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
   * @param level    (INPUT)  number of neigbour levels around the cell for the ICaloFutureClusterization tool
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
  DeclareInterfaceID(IL0CaloFuture2CaloFuture, 1, 1 );
};
#endif // CALOFUTUREINTERFACES_IL0CALOFUTURE2CALOFUTURE_H
