// $Id: OldFun.h,v 1.1.1.1 2006-07-24 14:56:20 mneedham Exp $
#ifndef _OldFun_H_
#define _OldFun_H_

#include <functional>
#include <algorithm>

// all the fun you could want with Clusters !
namespace ClusFun{

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
        ( !obj2 ) ? false : obj1->zMeas() < obj2->zMeas();
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
        ( !obj2 ) ? false : obj1->zMeas() > obj2->zMeas();
    }
    ///
   };

  template <typename TYPE >
  inline double dxdz(TYPE c1, 
                     TYPE c2) {

    /** slope in x  of 2 clusters */
    return((c1->measuredVal()-c2->measuredVal())/(c1->zMeas()-c2->zMeas()));
  }

  template <typename TYPE >
  inline double dxdz(TYPE aCluster, 
                     const double& xPair, 
                     const double& zPair) {

     /** slope in x  of cluster plus pair */
     return (aCluster->measuredVal()-xPair)/(aCluster->zMeas()-zPair);
  }

  template <typename TYPE >
  inline double xPred(TYPE aCluster, const double& zTarget, const double& tX){
    /** swim cluster measurement to z Targt */
    return(aCluster->measuredVal()+(tX*(zTarget-aCluster->zMeas())));
  }

  template <typename TYPE >
  inline double yPred(TYPE aCluster, const double& x) {
    /** pred y given prediction of x */
    return((aCluster->measuredVal()-(x*aCluster->cosAngle()))/aCluster->sinAngle());
  }

  template <typename TYPE >
  inline void toXY(double& xPair, double& yPair,
                   TYPE u, TYPE v) {
  /** convert from u,v to x,y*/
  double norm = (v->sinAngle()*u->cosAngle())-(u->sinAngle()*v->cosAngle());
  xPair = (v->sinAngle()*u->measuredVal() - u->sinAngle()*v->measuredVal())/norm;
  yPair = (v->measuredVal()* u->cosAngle() - u->measuredVal()* v->cosAngle() )/norm; 
}



};




#endif // _ClusFun_H
