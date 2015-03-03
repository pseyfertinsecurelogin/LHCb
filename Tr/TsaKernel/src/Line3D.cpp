
#include "TsaKernel/Line3D.h"

Tsa::Line3D Tsa::createLine3D(const Tsa::Line& xLine, const Tsa::Line& yLine, const double zRef){

  Gaudi::XYZVector direction =  Gaudi::XYZVector(xLine.m(), yLine.m(), 1.);
  direction = direction.Unit();
  Gaudi::XYZPoint point = Gaudi::XYZPoint(xLine.value(zRef),yLine.value(zRef),zRef);
  return Tsa::Line3D(point,direction);
}

