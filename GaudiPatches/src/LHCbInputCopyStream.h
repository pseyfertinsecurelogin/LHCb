// $Id: LHCbInputCopyStream.h,v 1.1 2009-02-16 13:16:36 pkoppenb Exp $
#ifndef GAUDIPATCHES_LHCBINPUTCOPYSTREAM_H
#define GAUDIPATCHES_LHCBINPUTCOPYSTREAM_H


// Required for inheritance
#include "OutputStream.h"

/** A small to stream Data I/O.
 *    
 *   Copies the whole content of the DST.
 *
 *   Note : this will be replaced by InputCopyStream once the pathed version is 
 *   Released in Gaudi v21r0.
 *
 *   @author:  M.Frank, 
 *   @author:  P. Koppenburg, added optional TES locations
*/
class LHCbInputCopyStream : public OutputStream     {
  friend class AlgFactory<LHCbInputCopyStream>;
  friend class Factory<LHCbInputCopyStream,IAlgorithm* (std::string,ISvcLocator *)>;

public:
	/// Standard algorithm Constructor
	LHCbInputCopyStream(const std::string& name, ISvcLocator* pSvcLocator); 
  /// Standard Destructor
  virtual ~LHCbInputCopyStream();
  /// Collect leaves from input file
  virtual StatusCode collectLeaves(IRegistry* dir, int level);
  /// Collect all objects to be written tio the output stream
  virtual StatusCode collectObjects();

private:
  /// Allow optional items to be on TES instead of input file
  bool m_takeOptionalFromTES;
};

#endif // GAUDIPATCHES_LHCBINPUTCOPYSTREAM_H
