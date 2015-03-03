// $Id: TrackTypes.h,v 1.9 2006-06-15 09:31:53 cattanem Exp $
#ifndef KERNEL_TRACKTYPES_H
#define KERNEL_TRACKTYPES_H 1
 
// Include files
#include "Kernel/GenericVectorTypes.h"
#include "Kernel/GenericMatrixTypes.h"
#include "Kernel/SymmetricMatrixTypes.h"

/** @namespace Gaudi TrackTypes.h Kernel/TrackTypes.h
 *
 *
 *  @author M. Needham
 *  @date   2005-1-09
 */

namespace Gaudi {

  typedef Vector5 TrackVector;
  typedef SymMatrix5x5 TrackSymMatrix;
  typedef Matrix5x5 TrackMatrix;
  typedef Matrix1x5 TrackProjectionMatrix;
 
};
 
#endif //  KERNEL_TRACKTYPES_H
