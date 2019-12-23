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
    // ========================================================================
    DataObjectReadHandle<DataObject> m_dataLocation{this, "DataLocation", ""};
    // ========================================================================
  public:
    // ========================================================================
    /// standard constructor
    using GaudiAlgorithm::GaudiAlgorithm;
    // ========================================================================
    /// the only one essential method
    StatusCode execute() override {
      if ( !m_dataLocation.objKey().empty() ) {
        if ( DataObject* data = m_dataLocation.getIfExists(); data ) {
          return evtSvc()->unregisterObject( data ).andThen( [&] { delete data; } );
        }
      }
      return StatusCode::SUCCESS;
    }
    // ========================================================================
  }; // end of class DataRemove
  // ==========================================================================
  DECLARE_COMPONENT( DataRemove )
} //                                                     end of namespace Gaudi
// ============================================================================
