#ifndef KERNEL_IBDLTOOL_H
#define KERNEL_IBDLTOOL_H 1

// Include files
#include "GaudiKernel/IAlgTool.h"


/** @class IBdlTool IBdlTool.h
 *  Interface class for field integral tool
 *
 *  @author Marco Cattaneo
 *  @author Mariusz Witek
 *  @date   2005-09-06
 *
*/
struct IBdlTool : extend_interfaces<IAlgTool>
{
  // Return the interface ID
  DeclareInterfaceID(IBdlTool, 2, 0);

  /// Actual operator function
  virtual double bdlIntegral( double ySlopeVelo,
                              double zOrigin,
                              double zVelo ) = 0;
  virtual double zBdlMiddle(  double ySlopeVelo,
                              double zOrigin,
                              double zVelo ) = 0;

};

#endif // KERNEL_IBDLTOOL_H
