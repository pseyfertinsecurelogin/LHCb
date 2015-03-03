// $Id: LineTypes.h,v 1.1 2006-11-01 11:02:28 jpalac Exp $
#ifndef KERNEL_LINETYPES_H 
#define KERNEL_LINETYPES_H 1

// Include files
#include "Kernel/Point3DTypes.h"
#include "Kernel/Vector3DTypes.h"
#include "LHCbMath/Line.h"
/** @namespace Gaudi LineTypes.h Kernel/LineTypes.h
 *  
 *  Namespace containing definitions of a few Line types.
 *
 *  @author Juan PALACIOS
 *  @date   2006-11-01
 */
namespace Gaudi 
{
  typedef Line<XYZPoint, XYZVector>                 XYZLine;
  typedef Line<Polar3DPoint, Polar3DVector>         Polar3DLine;
  typedef Line<RhoEtaPhiPoint, RhoEtaPhiVector>     RhoEtaPhiLine;
  typedef Line<RhoZPhiPoint, RhoZPhiVector>         RhoZPhiLine;

  typedef Line<XYZPointF, XYZVectorF>               XYZLineF;
  typedef Line<Polar3DPointF, Polar3DVectorF>       Polar3DLineF;
  typedef Line<RhoEtaPhiPointF, RhoEtaPhiVectorF>   RhoEtaPhiLineF;
  typedef Line<RhoZPhiPointF, RhoZPhiVectorF>       RhoZPhiLineF;
  
}

#endif // KERNEL_LINETYPES_H
