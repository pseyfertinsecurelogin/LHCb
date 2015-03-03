// $Id: InputCopyStream.cpp,v 1.4 2010-05-20 17:07:50 marcocle Exp $
#define GAUDISVC_PERSISTENCYSVC_INPUTCOPYSTREAM_CPP

// Framework include files
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IOpaqueAddress.h"
#include "GaudiKernel/DataStoreItem.h"
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/MsgStream.h"
#include "InputCopyStream.h"

#include "IDataStoreLeaves.h"

#include <set>

namespace LHCb {
// Define the algorithm factory for the standard output data writer
DECLARE_ALGORITHM_FACTORY(InputCopyStream)

// Standard Constructor
InputCopyStream::InputCopyStream(const std::string& name, ISvcLocator* pSvcLocator)
 : OutputStream(name, pSvcLocator), m_leavesTool(0)
{
  m_doPreLoad      = false;
  m_doPreLoadOpt   = false;
}

// Standard Destructor
InputCopyStream::~InputCopyStream()   {
}

// Initialize the instance.
StatusCode InputCopyStream::initialize() {
  StatusCode sc = OutputStream::initialize();
  if (sc.isFailure()) return sc;

  sc = toolSvc()->retrieveTool("DataSvcFileEntriesTool", "InputCopyStreamTool",
                               m_leavesTool);
  if (sc.isFailure()) return sc;

  return StatusCode::SUCCESS;
}

// Finalize the instance.
StatusCode InputCopyStream::finalize() {
  toolSvc()->releaseTool(m_leavesTool).ignore();
  m_leavesTool = 0;

  return OutputStream::finalize();
}

/// Collect all objects to be written to the output stream
StatusCode InputCopyStream::collectObjects()   {
  MsgStream log(msgSvc(), name());

  // Get the objects in the same file as the root node
  try {
    const IDataStoreLeaves::LeavesList & leaves = m_leavesTool->leaves();
    m_objects.assign(leaves.begin(), leaves.end());
  } catch (GaudiException &e) {
    log << MSG::ERROR << e.message() << endmsg;
    return StatusCode::FAILURE;
  }

  // Collect the other objects from the transient store (mandatory and optional)
  StatusCode sc = OutputStream::collectObjects();
  if (sc.isFailure()) return sc;

  // todo: this part should be moved to OutputStream::selectedObjects
  {
    // Remove duplicates from the list of objects
    std::set<DataObject*> unique;
    std::vector<DataObject*> tmp; // temporary vector with the reduced list
    tmp.reserve(m_objects.size());
    for (std::vector<DataObject*>::iterator o = m_objects.begin(); o != m_objects.end(); ++o) {
      if (!unique.count(*o)) {
        // if the pointer is not in the set, add it to both the set and the temporary vector
        unique.insert(*o);
        tmp.push_back(*o);
      }
    }
    m_objects.swap(tmp); // swap the data of the two vectors
  }

  return StatusCode::SUCCESS;
}

}
