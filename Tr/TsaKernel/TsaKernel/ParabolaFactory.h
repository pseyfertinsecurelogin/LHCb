#ifndef TsaParabolaFactory_H
#define TsaParabolaFactory_H

#include "Parabola.h"


namespace Tsa{
  namespace ParabolaFactory{

    // from two things (x,tx,z)
    template <typename TYPE>
      Tsa::Parabola create(TYPE point1,
                      TYPE point2);

    // from 3 things (x,z)
    template <typename TYPE>
      Tsa::Parabola create(TYPE point1,
                          TYPE point2,
                          TYPE point3);
  };
};

template <typename TYPE>
inline Tsa::Parabola Tsa::ParabolaFactory::create(TYPE point1,
                                  TYPE point2){
  // create from two
  double a = 0.5*(point1->tX()-point2->tX())/(point1->z()-point2->z());
  double b = point1->tX() - (2.0*a*point1->z());
  double c = 0.5*(( point1->x() - (point1->z()*((a*point1->z()) + b)))
          +  (point2->x() - (point2->z()*((a*point2->z()) + b))));
  return Tsa::Parabola(a,b,c);
}


template <typename TYPE>
inline Tsa::Parabola Tsa::ParabolaFactory::create(TYPE point1,
                                  TYPE point2,
                                  TYPE point3){
  // create from three
  
  // curvature
  double num = ((point2->z() - point1->z())*(point3->x()-point2->x()))
    - ((point3->z() - point2->z())*(point2->x()-point1->x()));
  double dem = (point2->z() - point1->z())*(point3->z() - point2->z())*(point3->z()-point1->z());
  double a =  num/dem;

  // b
  double b = ((point2->x()-point1->x())/(point2->z()-point1->z())) 
             - (a*(point1->z()+point2->z()));

  //  double c =  point1->x() -(a*gsl_pow_2(point1->z())) - (b*point1->z());
  double c = point1->x() - (point1->z()*((a*point1->z()) + b));

  return Tsa::Parabola(a,b,c);
}

#endif
