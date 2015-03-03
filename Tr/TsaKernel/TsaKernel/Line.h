#ifndef _TsaLine_H
#define _TsaLine_H

/** @class Tsa::Line Line.h TsaKernel/Line.h
 *
 *  y = mx + c !
 *
 *  @author M.Needham
 *  @date   31/05/2004
 */

namespace Tsa{
  class Line{

  public:

    Line(const double y1, const double y2,
          const double x1, const double x2);

    Line(const double m , const double c);

    Line(const double m , const double y, const double x);

    ~Line(){}

    double value(const double x) const;

    double m() const;

    double c() const;

  private:

    double m_m;
    double m_c;
  };
}; 

inline Tsa::Line::Line(const double m, const double c):
m_m(m),
m_c(c){
  // constructor from slope and constatn
}

inline Tsa::Line::Line(const double y1, const double y2,
                        const double x1, const double x2){

  m_m = (y2-y1)/(x2-x1);
  m_c = y1 - (m_m*x1);
  
}

inline Tsa::Line::Line(const double m, const double y, const double z):
m_m(m),
m_c(y - (m*z)){
  // constructor from slope and constatn
}

inline double Tsa::Line::value(const double x) const{
  return (m_c+(m_m*x));
}

inline double Tsa::Line::m() const{
  return m_m;
}

inline double Tsa::Line::c() const{
  return m_c;
}

#endif // _TsaLine_H
