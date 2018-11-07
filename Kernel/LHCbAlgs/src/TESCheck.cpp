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
// ============================================================================
// Include files
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiAlgorithm.h"
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/DataObject.h"
// ============================================================================

// ============================================================================
/** @file
 *
 *  Definition and implementation of class TESCheck
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2004-09-16
 */
// ============================================================================
/** @class TESCheck
 *  Simple class to check the existence of
 *  objects in Gaudi Transient stores
 *  The actual object type is not checked.
 *
 *  Also the algorithm  could be used to "preload" objects.
 *  For MSG::DEBUG level the printout of object location and the actula type
 *  is performed
 *
 *  Algorithm properties:
 *
 *   @li <tt>Inputs</tt>  vector of locations to be checked,
 *                        the default value is empty vector
 *
 *   @li <tt>Store</tt>   numerical identifier for Gaudi Transient store,
 *                        0 : Event Store ( default )
 *                        1 : Detector Store
 *                        2 : Histogram Store
 *
 *   @li <tt>Stop</tt>    flag which dictates to return error code if
 *                        data is not available (default) or just
 *                        print a warning message. The second options
 *                        can be useful for interactive environment
 *
 *  Usage:
 *
 *  @code
 *
 *  //
 *  ApplicationMgr.TopAlg += [ 'TESCheck/EvtCheck' ]
 *  EvtCheck.Inputs = { "MC/Calo" } ;
 *  EvtCheck.Inputs = { "MC/Velo" } ;
 *  //
 *  // check detector element
 *  ApplicationMgr.TopAlg += [ 'TESCheck/DetCheck' ]
 *  DetCheck.Inputs = { "/dd/Structure/LHCb" } ;
 *  // SWITCH to use Detector Data Store instead of Event Data Store
 *  DetCheck.DettectorStore = true ;
 *
 *  @endcode
 *
 *  Indeed it is a bit updated version of
 *     CaloCheckObjectsAlg from Calo/CaloReco package
 */
// ============================================================================
class TESCheck final : public GaudiAlgorithm
{
 public:

  using GaudiAlgorithm::GaudiAlgorithm;

  /// execute the algorithm itself
  StatusCode execute()  override;


 private:
  enum Stores { EvtStore = 0 , DetStore, HstStore };

  StringArrayProperty   m_inputs { this, "Inputs" ,{} };
  Gaudi::Property<int>  m_store  { this, "Store"  , Stores::EvtStore };
  Gaudi::Property<bool> m_stop   { this, "Stop", true };

};
// ============================================================================

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( TESCheck )

// ============================================================================
/// execute the algorithm itself
// ============================================================================
StatusCode TESCheck::execute()
{
  for ( const auto & address : m_inputs.value() )
  {

    SmartIF<IDataProviderSvc> dp;
    switch(m_store) {
    case Stores::DetStore:
      dp = detSvc();
      break;
    case Stores::HstStore:
      dp = histoSvc();
      break;
    case Stores::EvtStore:
    default:
      dp = evtSvc();
    }

    SmartDataPtr<DataObject> obj( dp , address ) ;
    DataObject * o = obj ;
    if ( !o  )
    {
      if ( m_stop.value() ) { return Error ( "Check failed for '" + address + "'" ) ; }
      if ( msgLevel ( MSG::WARNING ) )
        Warning ( "Check failed for '" + address  + "'", StatusCode::SUCCESS, 1 ).ignore() ;
      setFilterPassed(false) ;
    }
    else {
      setFilterPassed(true) ;
      if ( msgLevel ( MSG::DEBUG ) )
      {
        debug() << "Object address/type : "
                << "'" << address << "'/'"
                << System::typeinfoName( typeid( *o ) ) << "'" << endmsg ;
      }
    }

  }

  return StatusCode::SUCCESS;
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
