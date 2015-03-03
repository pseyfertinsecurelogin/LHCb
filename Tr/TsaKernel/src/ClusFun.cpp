
#include "TsaKernel/ClusFun.h"
#include "LHCbMath/GeomFun.h"
#include "TsaKernel/Cluster.h"
#include "gsl/gsl_math.h"
#include "TsaKernel/Line3D.h"

bool Tsa::ClusFun::poca(const Tsa::Cluster* cluster1,
                        const Tsa::Cluster* cluster2,
                        Gaudi::XYZPoint& aPoint){
   
  Tsa::Line3D line1( cluster1->trajectory().position(0.),
                     cluster1->trajectory().direction(0.));
 
  Tsa::Line3D line2( cluster2->trajectory().position(0.),
                     cluster2->trajectory().direction(0.));

  Gaudi::XYZPoint pos1;  
  Gaudi::XYZPoint pos2;
  bool canCalculate = Gaudi::Math::closestPoints(line1,line2,pos1, pos2);
  if (canCalculate == true){
    aPoint.SetXYZ(0.5*(pos1.x() + pos2.x()),
                  0.5*(pos1.y() + pos2.y()),
                  0.5*(pos1.z() + pos2.z()));
  }

  return canCalculate;
}

