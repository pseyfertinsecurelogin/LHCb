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
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/DataObject.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiAlgorithm.h"
// ============================================================================
namespace Gaudi {
  // ==========================================================================
  /** @class DataRemove
   *  Trivial algorithm to remove data in the TES
   *  @author Chris Jones Christopher.Rob.Jones@cern.ch
   *  @date 2012-11-08
   */
  // ==========================================================================
  class DataRemove final : public GaudiAlgorithm {
  public:
    // ========================================================================
    /// the only one essential method
    StatusCode execute() override;
    // ========================================================================
    /// standard constructor
    DataRemove( const std::string& name, ISvcLocator* pSvc );
    // ========================================================================
  private:
    std::string m_dataLocation;
    // ========================================================================
  }; // end of class DataRemove
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
Gaudi::DataRemove::DataRemove( const std::string& name, ISvcLocator* pSvc ) : GaudiAlgorithm( name, pSvc ) {
  declareProperty( "DataLocation", m_dataLocation = "" );
}
// ========================================================================
// the main method
// ========================================================================
StatusCode Gaudi::DataRemove::execute() {
  if ( !m_dataLocation.empty() ) {
    DataObject* data = getIfExists<DataObject>( m_dataLocation );
    if ( data ) {
      const StatusCode sc = evtSvc()->unregisterObject( data );
      if ( sc.isSuccess() ) {
        delete data;
        data = nullptr;
      } else {
        return Error( "Failed to delete input data " + m_dataLocation, sc );
      }
    }
  }
  return StatusCode::SUCCESS;
}
// ============================================================================
DECLARE_COMPONENT( Gaudi::DataRemove )
// ============================================================================
// The END
// ============================================================================
