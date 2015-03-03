// $Id: ClusFun.h,v 1.2 2006-07-24 15:37:31 cattanem Exp $
#ifndef _ClusFun_H_
#define _ClusFun_H_

#include <functional>
#include "Kernel/Plane3DTypes.h"
#include "Kernel/Point3DTypes.h"
#include "Kernel/Vector3DTypes.h"

namespace Tsa{
  class Cluster;
};

namespace Tsa{

  namespace ClusFun{

    Gaudi::Plane3D plane(const Tsa::Cluster* cluster1, 
                         const Tsa::Cluster* cluster2);

    bool intersection(const Tsa::Cluster* cluster,
                      const Gaudi::Plane3D& plane, 
                      Gaudi::XYZPoint& intersect);

    bool poca(const Tsa::Cluster* cluster1,
              const Tsa::Cluster* cluster2, 
              Gaudi::XYZPoint& aPoint);


    Gaudi::XYZPoint poca(const Tsa::Cluster* cluster1,
                         const Gaudi::XYZPoint& intersect);


    double dxdz(const Tsa::Cluster* cluster1,
                const Tsa::Cluster* cluster2);
  

  template <class TYPE1, class TYPE2 = TYPE1 >
   class Less_by_z
     : public std::binary_function<TYPE1,TYPE2,bool>
   {
   public:

     /** compare the z of one object with the
     *  Station  of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    inline bool operator() ( TYPE1 obj1 , TYPE2 obj2 ) const
    {
      return
        ( !obj1 ) ? true  :
      ( !obj2 ) ? false :  obj1->midPoint().z()  < obj2->midPoint().z();
    }
    ///
   };

   template <class TYPE1, class TYPE2 = TYPE1 >
   class Greater_by_z
     : public std::binary_function<TYPE1,TYPE2,bool>
   {
   public:

     /** compare the z of one object with the
     *  Station  of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    inline bool operator() ( TYPE1 obj1 , TYPE2 obj2 ) const
    {
      return
        ( !obj1 ) ? true  :
        ( !obj2 ) ? false : obj1->midPoint().z() > obj2->midPoint().z();
    }
    ///
   };


  };
};

#include "TsaKernel/Cluster.h"
#include "TsaKernel/Line3D.h"
#include "LHCbMath/GeomFun.h"

inline Gaudi::XYZPoint Tsa::ClusFun::poca(const Tsa::Cluster* cluster1,
                                   const Gaudi::XYZPoint& aPoint){

  Tsa::Line3D tLine(cluster1->beginPoint(), cluster1->direction() );
  return Gaudi::Math::closestPoint( aPoint,tLine);
}

inline bool Tsa::ClusFun::intersection(const Tsa::Cluster* cluster,
                                       const Gaudi::Plane3D& plane,
                                       Gaudi::XYZPoint& intersect){

  // intersect cluster == line and plane
  Tsa::Line3D tLine(cluster->beginPoint(), cluster->direction() );
  double mu = 0.0;
  return Gaudi::Math::intersection(tLine, plane, intersect, mu);
}


inline Gaudi::Plane3D Tsa::ClusFun::plane(const Tsa::Cluster* cluster1, 
                                          const Tsa::Cluster* cluster2){
 
  return Gaudi::Plane3D(cluster1->midPoint(), 
                        cluster2->beginPoint(),
                        cluster2->endPoint());
}

inline double Tsa::ClusFun::dxdz(const Tsa::Cluster* cluster1,
                                 const Tsa::Cluster* cluster2) {

  /** slope in x  of 2 clusters */
  const Gaudi::XYZPoint point1 = cluster1->midPoint(); 
  const Gaudi::XYZPoint point2 = cluster2->midPoint(); 
  return((point1.x() - point2.z())/(point1.z() - point2.z()));
};

#endif


