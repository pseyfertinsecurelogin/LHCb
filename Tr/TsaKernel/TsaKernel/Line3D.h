#ifndef _TsaLine3D_H
#define _TsaLine3D_H

/* * typedef for a line in 3-D
 *
 *  @author M.Needham
 *  @date   31/05/2004
 */

// from Kernel/LHCbDefinitions
#include "Kernel/Point3DTypes.h"
#include "Kernel/Vector3DTypes.h"
#include "LHCbMath/Line.h"
#include "TsaKernel/Line.h"


namespace Tsa{

  typedef Gaudi::Line<Gaudi::XYZPoint,Gaudi::XYZVector > Line3D;

  Line3D createLine3D(const Tsa::Line& xLine, const Tsa::Line& yLine, double zRef);

};



#endif
