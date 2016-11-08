
// from Gaudi
#include "GaudiKernel/IDataManagerSvc.h" 

// from DetDesc
#include "DetDesc/DataStoreLoadAgent.h"

// local
#include "PreloadGeometryTool.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PreloadGeometryTool
//
// 2006-07-26 : Marco Cattaneo
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_TOOL_FACTORY( PreloadGeometryTool )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PreloadGeometryTool::PreloadGeometryTool( const std::string& type,
                                          const std::string& name,
                                          const IInterface* parent )
  : GaudiTool ( type, name , parent )
{
  declareInterface<IGenericTool>(this);
}

//=============================================================================
// Excution method
//=============================================================================
void PreloadGeometryTool::execute() 
{
  std::unique_ptr<DataStoreLoadAgent> loadAgent ( new DataStoreLoadAgent() );
  auto dataMgr = svc<IDataManagerSvc>("DetectorDataSvc", true);
  info() << "Preloading detector geometry..." << endmsg;
  const auto sc = dataMgr->traverseTree(loadAgent.get());
  if ( sc ) 
  {
    info() << "Loaded " << loadAgent->loadedObjects() << " objects to depth of "
           << loadAgent->maxDepth() << " levels" << endmsg;
  }
  else
  {
    error() << "Problem loading geometry" << endmsg;
  }
}
//=============================================================================
