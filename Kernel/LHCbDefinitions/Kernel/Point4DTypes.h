// $Id: Point4DTypes.h,v 1.1 2005-12-02 16:59:36 cattanem Exp $
#ifndef KERNEL_POINT4DTYPES_H 
#define KERNEL_POINT4DTYPES_H 1

// Include files
#include "Kernel/Vector4DTypes.h"

/** @namespace Gaudi Point4DTypes.h Kernel/Point4DTypes.h
 *  
 *
 *  @author Marco Cattaneo
 *  @date   2005-11-29
 */

namespace Gaudi {
  typedef LorentzVector XYZTPoint; ///< 4D point (space-time) @todo use Point4D from MathCore
}

#endif // KERNEL_POINT4DTYPES_H
