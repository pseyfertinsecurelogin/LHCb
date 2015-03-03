// $Id: IWriteSelResult.h,v 1.5 2007-09-07 14:01:40 cattanem Exp $
#ifndef KERNEL_IWRITESELRESULT_H 
#define KERNEL_IWRITESELRESULT_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// Forward declaration
namespace LHCb {
  class SelResult;
};

static const InterfaceID IID_IWriteSelResult ( "IWriteSelResult", 1, 0 );

/** @class IWriteSelResult IWriteSelResult.h Kernel/IWriteSelResult.h
 *  
 *  Writes SelResult to SelResult container. 
 *
 *  @author Patrick Koppenburg
 *  @date   2007-09-07
 */
class IWriteSelResult : virtual public IAlgTool {
public: 

  // Return the interface ID
  static const InterfaceID& interfaceID() { return IID_IWriteSelResult; }

  /// write a SelResult
  virtual StatusCode write(LHCb::SelResult&) = 0;
  
  /// create andf write a SelResult from algo name and result
  virtual StatusCode write(std::string name, bool result, std::string decay = "") = 0;

protected:

private:

};
#endif // KERNEL_IWRITESELRESULT_H
