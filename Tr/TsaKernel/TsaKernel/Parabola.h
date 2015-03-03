#ifndef _TsaParabola_H
#define _TsaParabola_H

#include "TsaKernel/Line.h"


namespace Tsa{
 class Parabola {

  public:
 
    Parabola(const double a, const double b, const double c);
  
    ~Parabola(){}

    double value(const double z) const;

    double derivative(const double z) const;

    double curvature() const;

    Tsa::Line tangent(const double z) const;

    double b() const;

    double c() const;

  private:  

    double m_a;
    double m_b;
    double m_c;

 };
};

inline Tsa::Parabola::Parabola(const double a, 
                                const double b, 
                                const double c):
m_a(a),
m_b(b),
m_c(c)
{
  // constructer
}
       		
inline double Tsa::Parabola::value(const double z) const{
  return z*((m_a*z)+m_b)+m_c;
}

inline Tsa::Line Tsa::Parabola::tangent(const double z) const{
  const double slope = derivative(z);
  return Tsa::Line(slope,value(z) - (slope*z));
}
 
inline double Tsa::Parabola::derivative(const double z) const{
 return ((2.0*m_a*z) +m_b);
}
			
inline double Tsa::Parabola::curvature() const{
 return m_a;
}

inline double Tsa::Parabola::b() const{
 return m_b;
}

inline double Tsa::Parabola::c() const{
 return m_c;
}


#endif // _TsaParabola_H
