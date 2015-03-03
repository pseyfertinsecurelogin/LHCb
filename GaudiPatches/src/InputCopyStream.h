// $Id: InputCopyStream.h,v 1.4 2010-05-20 17:07:50 marcocle Exp $
#ifndef GAUDISVC_PERSISTENCYSVC_INPUTCOPYSTREAM_H
#define GAUDISVC_PERSISTENCYSVC_INPUTCOPYSTREAM_H


// Required for inheritance
#include "OutputStream.h"

class IDataStoreLeaves;

namespace LHCb {

/** Extension of OutputStream to copy the content of the main input file.
 *
 *  On top of the standard behavior of OutputStream, this class takes also all
 *  the entries that come from the same file as the root entry in the data
 *  service.
 *
 *  @author:  M. Frank
 *  @author:  P. Koppenburg
 *  @author:  M. Clemencic
*/
class InputCopyStream : public OutputStream {
public:
  /// Standard algorithm Constructor
  InputCopyStream(const std::string& name, ISvcLocator* pSvcLocator); 
  /// Standard Destructor
  virtual ~InputCopyStream();

  /// Initialize the instance.
  virtual StatusCode initialize();

  /// Finalize the instance.
  virtual StatusCode finalize();

  /// Collect all objects to be written to the output stream
  virtual StatusCode collectObjects();

private:
  /// Allow optional items to be on TES instead of input file
  bool m_takeOptionalFromTES;

  /// Pointer to the (public) tool used to retrieve the objects in a file.
  IDataStoreLeaves *m_leavesTool;
};

}

#endif // GAUDISVC_PERSISTENCYSVC_INPUTCOPYSTREAM_H
