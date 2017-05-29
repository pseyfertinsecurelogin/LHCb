// Include files

#include <exception>
#include <functional>
#include <utility>
#include <vector>

#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiAlg/GaudiCommonImp.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/System.h"
#include "Kernel/CondDBNameTagPair.h"
// from LHCb
#include "Kernel/ICondDBInfo.h"
// local
#include "LoadDDDB.h"

class ISvcLocator;

//-----------------------------------------------------------------------------
// Implementation file for class : LoadDDDB
//
// 2005-10-14 : Marco Clemencic
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( LoadDDDB )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
LoadDDDB::LoadDDDB( const std::string& name,
                    ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty("Node", m_treeToLoad = "/dd*");
}
//=============================================================================
// Destructor
//=============================================================================
LoadDDDB::~LoadDDDB() {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode LoadDDDB::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << "==> Initialize" << endmsg;

  std::vector<LHCb::CondDBNameTagPair> tmp;
  svc<ICondDBInfo>( "XmlParserSvc", true )->defaultTags( tmp );

  for ( auto& item: tmp ) {
    info() << "Database " << item.first << " tag " << item.second << endmsg;
  }

  updMgrSvc(); // trigger the initialization of the Condition Update sub-system
  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode LoadDDDB::execute() {

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Execute" << endmsg;

  info() << "Loading the DDDB" << endmsg;

  try {
    detSvc()->addPreLoadItem(m_treeToLoad);
    detSvc()->preLoad();
  } catch (GaudiException &x) {
    fatal() << "Caught GaudiException" << endmsg;
    int i = 0;
    for ( GaudiException *ex = &x; 0 != ex; ex = ex->previous() ) {
      fatal() << std::string(i++,' ') << " ==> " << ex->what() << endmsg;
    }
    return StatusCode::FAILURE;
  } catch (std::exception &x) {
    fatal() << "Caught exception '" << System::typeinfoName(typeid(x)) << "'"
              << endmsg;
    fatal() << " ==> " << x.what() << endmsg;
    return StatusCode::FAILURE;
  } catch (...) {
    fatal() << "Caught unknown exception!!" << endmsg;
    return StatusCode::FAILURE;
  }
  info() << "done." << endmsg;
  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode LoadDDDB::finalize() {

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Finalize" << endmsg;
  return GaudiAlgorithm::finalize();  // must be called after all other actions
}

//=============================================================================
