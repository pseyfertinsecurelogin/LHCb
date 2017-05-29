// $Id: $
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include <functional>
#include <string>

#include "Gaudi/Details/PluginServiceDetails.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;

// ============================================================================
namespace Gaudi
{
  // ==========================================================================
  /** @class DataRemove
   *  Trivial algorithm to remove data in the TES
   *  @author Chris Jones Christopher.Rob.Jones@cern.ch
   *  @date 2012-11-08
   */
  // ==========================================================================
  class DataRemove final : public GaudiAlgorithm
  {
    // ========================================================================
    friend class AlgFactory<Gaudi::DataRemove> ;
    // ========================================================================
  public:
    // ========================================================================
    /// the only one essential method
    StatusCode execute ()  override;
    // ========================================================================
  protected:
    // ========================================================================
    /// standard constructor
    DataRemove ( const std::string& name ,
                 ISvcLocator*       pSvc ) ;
    /// virtual destructor
    virtual ~DataRemove() = default ;
    // ========================================================================
  private:
    std::string m_dataLocation;
    // ========================================================================
  } ; // end of class DataRemove
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
Gaudi::DataRemove::DataRemove( const std::string& name ,
                               ISvcLocator*       pSvc )
  : GaudiAlgorithm ( name , pSvc )
{
  declareProperty( "DataLocation", m_dataLocation = "" );
}
// ========================================================================
// the main method
// ========================================================================
StatusCode Gaudi::DataRemove::execute()
{
  if ( !m_dataLocation.empty() )
  {
    DataObject * data = getIfExists<DataObject>( m_dataLocation );
    if ( data )
    {
      const StatusCode sc = evtSvc()->unregisterObject( data );
      if ( sc.isSuccess() )
      {
        delete data;
        data = nullptr;
      }
      else
      {
        return Error( "Failed to delete input data " + m_dataLocation, sc );
      }
    }
  }
  return StatusCode::SUCCESS;
}
// ============================================================================
DECLARE_NAMESPACE_ALGORITHM_FACTORY(Gaudi,DataRemove)
// ============================================================================
// The END
// ============================================================================
