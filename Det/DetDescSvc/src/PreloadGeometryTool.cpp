
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
: base_class ( type, name , parent )
{
  declareInterface<IGenericTool>(this);
}

//=============================================================================
// Excution method
//=============================================================================
void PreloadGeometryTool::execute()
{
  DataStoreLoadAgent loadAgent;
  auto dataMgr = service<IDataManagerSvc>("DetectorDataSvc", true);
  info() << "Preloading detector geometry..." << endmsg;
  const auto sc = dataMgr->traverseTree(&loadAgent);
  if ( sc ) {
    info() << "Loaded " << loadAgent.loadedObjects() << " objects to depth of "
           << loadAgent.maxDepth() << " levels" << endmsg;
  } else {
    error() << "Problem loading geometry" << endmsg;
  }
}
//=============================================================================
