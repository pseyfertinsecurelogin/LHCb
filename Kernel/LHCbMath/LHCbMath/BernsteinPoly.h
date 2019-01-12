/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// ============================================================================
#ifndef LHCBMATH_BERNSTEINPOLY_H 
#define LHCBMATH_BERNSTEINPOLY_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
#include <vector>
#include <array>
#include <complex>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
// ============================================================================
// LHCbMath
// ============================================================================
#include "LHCbMath/LHCbMath.h"
#include "LHCbMath/Polynomials.h"
// ============================================================================
/** @file LHCbMath/BernsteinPoly.h
 *  Bernstein polynomials
 *  @see http://en.wikipedia.org/wiki/Bernstein_polynomial
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2010-04-19
 */
// ============================================================================
namespace Gaudi
{
  // ==========================================================================
  namespace Math
  {
    // ========================================================================
    /// forward declaration
    class LegendreSum  ; // forward declaration
    class ChebyshevSum ; // forward declaration
    class Polynomial   ; // forward declaration
    // ========================================================================
    /** @class Bernstein
     *  The sum of bernstein's polynomial of order N
     *  \f$f(x) = \sum_i a_i B^n_i(x)\f$, where
     *  \f$ B^n_k(x) = C^n_k x^k(1-x)^{n-k}\f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     */
    class GAUDI_API Bernstein : public Gaudi::Math::PolySum
    {
      // ======================================================================
    public:
      // ======================================================================
      /** helper structure to denote the basic Bernstein polynomials B(k,N)
       *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
       */
      class GAUDI_API Basic
      {
      public:
        // ====================================================================
        explicit
          Basic ( const unsigned short k = 0 ,
                  const unsigned short N = 0 )
          : m_k ( k )
          , m_N ( N )
        {}
        // ====================================================================
      public :
        // ====================================================================
        unsigned short k () const { return m_k ; }
        unsigned short N () const { return m_N ; }
        // ====================================================================
      private:
        // ====================================================================
        unsigned short m_k ;
        unsigned short m_N ;
        // ====================================================================
      } ;
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the order
      Bernstein ( const unsigned short        N     = 0 ,
                  const double                xmin  = 0 ,
                  const double                xmax  = 1 ) ;
      // ======================================================================
      /// constructor from N+1 coefficients
      Bernstein ( const std::vector<double>&  pars      ,
                  const double                xmin  = 0 ,
                  const double                xmax  = 1 ) ;
      // ======================================================================
      /// construct the basic bernstein polinomial  B(k,N)
      Bernstein  ( const Basic&              basic     ,
                   const double              xmin  = 0 ,
                   const double              xmax  = 1 ) ;
      // ======================================================================
      /// template constructor from sequence of parameters
      template <class ITERATOR>
        Bernstein ( ITERATOR                 first ,
                    ITERATOR                 last  ,
                    const double             xmin  ,
                    const double             xmax  )
        : Gaudi::Math::PolySum ( first , last )
        , m_xmin ( std::min ( xmin, xmax ) )
        , m_xmax ( std::max ( xmin, xmax ) )
      {}
      // ======================================================================
      /// constructor  from Bernstein polynomial from *different* domain
      Bernstein ( const Bernstein& poly ,
                  const double     xmin ,
                  const double     xmax ) ;
      // ======================================================================
      /** construct Bernstein interpolant
       *  @param x    vector of abscissas
       *  @param y    vector of function values
       *  @param xmin low  edge for Bernstein polynomial
       *  @param xmin high edge for Bernstein polynomial
       *  - if vector of y is longer  than vector x, extra values are ignored
       *  - if vector of y is shorter than vector x, missing entries are assumed to be zero
       *  It relies on Newton-Bernstein algorithm
       *  @see http://arxiv.org/abs/1510.09197
       *  @see Mark Ainsworth and Manuel A. Sanches,
       *       "Computing of Bezier control points of Largangian interpolant
       *       in arbitrary dimension", arXiv:1510.09197 [math.NA]
       *  @see http://adsabs.harvard.edu/abs/2015arXiv151009197A
       */
      Bernstein ( const std::vector<double>& x         ,
                  const std::vector<double>& y         ,
                  const double               xmin  = 0 ,
                  const double               xmax  = 1 ) ;
      // ======================================================================
      /** construct Bernstein interpolant
       *  @param x    vector of abscissas
       *  @param y    vector of function values
       *  @param xmin low  edge for Bernstein polynomial
       *  @param xmin high edge for Bernstein polynomial
       *  - if vector of y is longer  than vector x, extra values are ignored
       *  - if vector of y is shorter than vector x, missing entries are assumed to be zero
       *  It relies on Newton-Bernstein algorithm
       *  @see http://arxiv.org/abs/1510.09197
       *  @see Mark Ainsworth and Manuel A. Sanches,
       *       "Computing of Bezier control points of Largangian interpolant
       *       in arbitrary dimension", arXiv:1510.09197 [math.NA]
       *  @see http://adsabs.harvard.edu/abs/2015arXiv151009197A
       */
      template <class XITERATOR,class YITERATOR>
        Bernstein ( XITERATOR    xbegin ,
                    XITERATOR    xend   ,
                    YITERATOR    ybegin ,
                    YITERATOR    yend   ,
                    const double xmin   ,
                    const double xmax   );
      // ======================================================================
      /** construct Bernstein polynomial from its roots
       *
       *  Polinomial has a form
       *  \f$ B(x) = \prod_i (x-r_i) \prod_j (x-c_i)(x-c_i^*) \f$
       *
       *  @param xmin low  edge for Bernstein polynomial
       *  @param xmax high edge for Bernstein polynomial
       *  @param r  the list of real  roots of the polinomial
       *  @param c  the list of complex roots (only one root from cc-pair is needed)
       */
      Bernstein 
      ( const double xmin , 
        const double xmax , 
        const std::vector<double>&                 r = 
        std::vector<double> () , 
        const std::vector<std::complex<double> > & c = 
        std::vector<std::complex<double> > ()  );
      // ======================================================================
      /** construct Bernstein polynomial from its roots
       *
       *  Polinomial has a form
       *  \f$ B(x) = \prod_i (x-r_i) \prod_j (x-c_i)(x-c_i^*) \f$
       *
       *  @param xmin low  edge for Bernstein polynomial
       *  @param xmax high edge for Bernstein polynomial
       *  @param c  the list of complex roots (only one root from cc-pair is needed)
       *  @param r  the list of real  roots of the polinomial
       */
      Bernstein 
      ( const double xmin , 
        const double xmax , 
        const std::vector<std::complex<double> > & c ,
        const std::vector<double>&                 r = 
        std::vector<double> () ) ;
      // ======================================================================
      /// copy
      Bernstein ( const Bernstein&  ) = default ;
      /// move
      Bernstein (       Bernstein&& ) = default ;
      // ======================================================================
      /** constructor from Legendre polynomial
       *  @see http://www.sciencedirect.com/science/article/pii/S0377042700003769 eq.20
       */
      explicit Bernstein ( const LegendreSum&  poly ) ;
      // ======================================================================
      /// constructor from Chebyshev polynomial
      explicit Bernstein ( const ChebyshevSum& poly ) ;
      // ======================================================================
      /// constructor from simple monomial form
      explicit Bernstein ( const Polynomial&   poly ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value of polynomial
      double evaluate ( const double x ) const ;
      // ======================================================================
      /// get the value
      double operator () ( const double x ) const
      { return x < m_xmin ? 0 : x > m_xmax ? 0 : evaluate ( x ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get lower edge
      double xmin () const { return m_xmin ; }
      /// get upper edge
      double xmax () const { return m_xmax ; }
      // ======================================================================
    public:
      // ======================================================================
      /// all coefficients are so small that  P(x) + c == c ? 
      bool   small         ( const double c = 1.0 ) const ;
      /// is it a decreasing function?
      bool   decreasing    () const ;
      /// is it a increasing function?
      bool   increasing    () const ;
      /// is it a monothonical function?
      bool   monothonic    () const { return increasing() || decreasing() ; }
      /// is it a constant function?
      bool   constant      () const ;
      // ======================================================================
    public: // convert from local to global variables
      // ======================================================================
      double x ( const double t ) const
      { return       m_xmin   + ( m_xmax - m_xmin ) * t ; }
      double t ( const double x ) const
      { return ( x - m_xmin ) / ( m_xmax - m_xmin )     ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get the integral between xmin and xmax
      double    integral   () const ;
      /// get the integral between low and high
      double    integral   ( const double low , const double high ) const ;
      /** get indefinite integral  as function object
       *  \f$ I(x) = \int^{x}_{x_{min}} B(t) dt + C \f$
       *  @param C the integration constant
       */
      Bernstein indefinite_integral ( const double C = 0 ) const ;
      /// get the derivative at point "x"
      double    derivative          ( const double x     ) const ;
      /// get derivative as function object
      Bernstein derivative          () const ;
      // ======================================================================
    public :
      // ======================================================================
      /** elevate it:
       *  represent as Bernstein polynomial of order N+r
       *  @param r  INPUT increase of degree
       *  @return new polynomial of order N+r
       */
      Bernstein elevate  ( const unsigned short r ) const ;
      // ======================================================================
      /** reduce it:
       *  represent as Bernstein polynomial of order N-r
       *  @param r  INPUT decrease of degree
       *  @return new polynomial of order N-r
       */
      Bernstein reduce  ( const unsigned short r ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /** calculate ``nearest'' polynomial (in the sense of q-norm) of lower degree,
       *  where q-norm is defined as:
       *  \f$ \left| f \right|_{q} = \left( \sum_i \left|c_i\right|^q\right)^{\frac{1}{q}} \f$
       *
       *  - q_inv = 0.0 ->  \f$ max_k    \left|c_k\right|  \f$
       *  - q_inv = 0.5 ->  \f$ \sqrt{ \sum_k  c_k^2 }     \f$
       *  - q_inv = 1.0 ->  \f$ \sum_k \left| c_k \right|  \f$
       *  @see  N.Rezvani and R.M. Corless,
       *       "The Nearest Polynomial With A Given Zero, Revisited"
       *        ACM SIGSAM Bulletin, Vol. 39, No. 3, September 2005
       *  @see http://dl.acm.org/citation.cfm?doid=1113439.1113442
       */
      Bernstein nearest ( const double q_inv = 0 ) const ;
      // ======================================================================
      /** calculate q-norm of the polynomial
       *  where q-norm is defined as:
       *  \f$ \left| f \right|_{q} = \left( \sum_i \left|c_i\right|^q\right)^{\frac{1}{q}} \f$
       *
       *  - q_inv = 0.0 ->  \f$ max_k    \left|c_k\right|  \f$
       *  - q_inv = 0.5 ->  \f$ \sqrt{ \sum_k  c_k^2 }     \f$
       *  - q_inv = 1.0 ->  \f$ \sum_k \left| c_k \right|  \f$
       *
       *  q-norm for Bernstein polynomials is "equivalent" to common
       *  \f$L^2\f$-norm for generic functions,
       *  in the sense that
       *  \f$   \gamma_n \left| f \right|_{\inf}
       *             \le \left| f \right|_{\int}
       *             \le \left| f \right|_{\inf} \f$,
       *  where \f$  \left| f \right|_{\int}^2 = \int  \left| f \right|^2 dx \f$ and
       *  \f$ \gamma_n = \min_{j} \int \left|  B^n_j(x) \right|^2 dx
       *            \sim  (\pi n)^{-\frac{3}{4}} \f$
       *  similarly \f$ \left| f \right|_I \le \left| f \right|_1 \f$,
       */
      double    norm   ( const double q_inv = 0 ) const ;
      // ======================================================================
      /** filter out very small terms
       *  the term is considered to be very small if
       *   - it is numerically zero
       *   - or if epsilon > 0,
       *          abs ( c(k) ) < epsilon
       *   - or if scale   > 0  , 
       *           scale + par ==  scale 
       *   - or if scale   <= 0 ,
       *           norm  + pars == norm    
       *  Since the maximum value for each term of
       *  \f$ c_k C^n_k \frac{ k^k (n-k)^{n-k}}{ n^n}\f$
       *  @param  epsilon  parameter to define "smalness" of terms
       *  @param  scale    parameter to define "smalness" of terms
       *  @return number of nullified terms
       */
      unsigned short remove_noise ( const double epsilon = 0 , 
                                    const double scale   = 0 ) ;
      // ======================================================================
      /** how close are two polynomials in q-norm?
       *  where q-norm is defined as:
       *  \f$ \left| f \right|_{q} = \left( \sum_i \left|c_i\right|^q\right)^{\frac{1}{q}} \f$
       *
       *  - q_inv = 0.0 -> \f$ max_k    \left|c_k\right|  \f$
       *  - q_inv = 0.5 -> \f$ \sqrt{ \sum_k  c_k^2 }     \f$
       *  - q_inv = 1.0 -> \f$ \sum_k \left| c_k \right|  \f$
       */
      double distance ( const Bernstein& other , const double q_inv = 0 ) const ;
      // ======================================================================
    public:  // polynomial division
      // ======================================================================
      /** polynomial division
       *  \f$  f(x) = q(z)*g(x) + r(x) \f$
       *  @return the pair q(x),r(x)
       */
      std::pair<Bernstein,Bernstein> divmod   ( const Bernstein& g ) const ;
      // ======================================================================
      /** polynomial division
       *  \f$  f(x) = q(z)*g(x) + r(x) \f$
       *  @return the quotient q(x)
       */
      // ======================================================================
      Bernstein                      quotient ( const Bernstein& g ) const ;
      /** polynomial division
       *  \f$  f(x) = q(z)*g(x) + r(x) \f$
       *  @return the reminder r(x)
       */
      Bernstein                      reminder ( const Bernstein& g ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /** get the leading power coefficient
       *  \f$ f(x) = hx^n + ....\f$
       *  @return the coefficient at x^n
       */
      double    head  () const ;
      // ======================================================================
    public:
      // ======================================================================
      /// simple  manipulations with polynoms: shift it!
      Bernstein& operator += ( const double a ) ;
      /// simple  manipulations with polynoms: shift it!
      Bernstein& operator -= ( const double a ) ;
      /// simple  manipulations with polynoms: scale it!
      Bernstein& operator *= ( const double a ) ;
      /// simple  manipulations with polynoms: scale it!
      Bernstein& operator /= ( const double a ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// negate it!
      Bernstein  operator-() const ;
      // ======================================================================
    public: // a bit of operators for python
      // ======================================================================
      /// Sum of Bernstein polynomial and a constant
      Bernstein __add__   ( const double value ) const ;
      /// Sum of Bernstein polynomial and a constant
      Bernstein __radd__  ( const double value ) const ;
      /// Product of Bernstein polynomial and a constant
      Bernstein __mul__   ( const double value ) const ;
      /// Product of Bernstein polynomial and a constant
      Bernstein __rmul__  ( const double value ) const ;
      /// Subtract a constant from Benrstein polynomial
      Bernstein __sub__   ( const double value ) const ;
      /// Constant minus Bernstein polynomial
      Bernstein __rsub__  ( const double value ) const ;
      /// Divide Benrstein polynomial by a constant
      Bernstein __div__   ( const double value ) const ;
      /// Negate Bernstein polynomial
      Bernstein __neg__   () const ;
      // ======================================================================
    public:
      // ======================================================================
      /// Sum of   Bernstein polynomials
      Bernstein __add__   ( const Bernstein& other ) const ;
      /// Subtract Bernstein polynomials
      Bernstein __sub__   ( const Bernstein& other ) const ;
      /// Multiply Bernstein polynomials
      Bernstein __mul__   ( const Bernstein& other ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the underlying Bernstein polynomial  (self here)
      const Gaudi::Math::Bernstein& bernstein () const { return *this ; }
      // ======================================================================
    public:
      // ======================================================================
      /// the sum two Bernstein polynomials
      Bernstein  sum      ( const Bernstein&        other ) const ;
      /// subtract Bernstein polynomials
      Bernstein  subtract ( const Bernstein&        other ) const ;
      /// multiply Bernstein polynomials
      Bernstein  multiply ( const Bernstein&        other ) const ;
      /// multiply Bernstein polynomials with the basic bernstein polynomial
      Bernstein  multiply ( const Bernstein::Basic& other ) const ;
      /** multiply Bernstein polynomial with
       *  \f$ (x-x_{min})^i(x_{max}-x)^j \f$
       */
      Bernstein  multiply ( const unsigned short i ,
                            const unsigned short j ) const ;
      /// power function
      Bernstein  pow      ( const unsigned short i ) const ;
      /// scale  all coefficients with 2**i 
      Bernstein  ldexp    ( const short i )  const ;
      // ======================================================================
    public:  // various assignements
      // ======================================================================
      /// copy assignement
      Bernstein& operator=( const Bernstein&  right ) ;
      /// move assignement
      Bernstein& operator=(       Bernstein&& right ) ;
      /// assignement from the constant
      Bernstein& operator=( const double      right ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// swap  two Bernstein polynimials 
      void swap ( Bernstein& right ) ;      
      // ======================================================================
    private:  // internal data
      // ======================================================================
      /// the left edge of interval
      double m_xmin  ;                             // the left edge of interval
      /// the right edge of interval
      double m_xmax  ;                             // the right edge of interval
      // ======================================================================
    };
    // ========================================================================
    /** construct Bernstein interpolant
     *  @param x    vector of abscissas
     *  @param y    vector of function values
     *  @param xmin low  edge for Bernstein polynomial
     *  @param xmin high edge for Bernstein polynomial
     *  - if vector of y is longer  than vector x, extra values are ignored
     *  - if vector of y is shorter than vector x, missing entries are assumed to be zero
     *  It relies on Newton-Bernstein algorithm
     *  @see http://arxiv.org/abs/1510.09197
     *  @see Mark Ainsworth and Manuel A. Sanches,
     *       "Computing of Bezier control points of Largangian interpolant
     *       in arbitrary dimension", arXiv:1510.09197 [math.NA]
     *  @see http://adsabs.harvard.edu/abs/2015arXiv151009197A
     */
    template <class XITERATOR,class YITERATOR>
    Bernstein::Bernstein ( XITERATOR    xbegin ,
                           XITERATOR    xend   ,
                           YITERATOR    ybegin ,
                           YITERATOR    yend   ,
                           const double xmin   ,
                           const double xmax   )
      : Gaudi::Math::PolySum ( xbegin == xend ? 0 : std::distance ( xbegin , xend  ) -1 )
      , m_xmin ( std::min ( xmin , xmax ) )
      , m_xmax ( std::max ( xmin , xmax ) )
    {
      const unsigned int N  = std::distance ( xbegin , xend ) ;
      const unsigned int NY = std::distance ( ybegin , yend ) ;
      std::vector<long double> _t ( std::max ( N , 1u ) ) ;
      //
      std::transform ( xbegin , xend   , _t.begin() ,
                       [this]( const double v ) { return this->t(v) ; } ) ;
      //
      std::vector<long double> _f ( N ) ;
      YITERATOR ylast = std::next ( ybegin , std::min ( N , NY ) ) ;
      std::copy ( ybegin, ylast , _f.begin() ) ;
      //
      std::vector<long double>  w ( N , 0.0 ) ;
      std::vector<long double>  c ( N , 0.0 ) ;
      //
      w[0] =  1.0  ;
      c[0] = _f[0] ;
      //
      for ( unsigned int s = 1 ; s < N ; ++s )
      {
        /// calculate the divided differences
        for ( unsigned int k = N - 1 ; s <= k ; --k )
        {
          const long double fk  = _f[k  ] ;
          const long double fk1 = _f[k-1] ;
          const long double xk  = _t[k  ] ;
          const long double xks = _t[k-s] ;
          _f[k] = ( fk - fk1 ) / ( xk - xks ) ;
        }
        //
        const long double xs1 = _t[s-1] ;
        for ( unsigned int j = s ; 1 <= j ; --j )
        {
          w[j] =  j * w[j-1] * ( 1 - xs1 ) / s  - ( s - j ) * xs1 * w[j] / s ;
          c[j] =  j * c[j-1]               / s  + ( s - j )       * c[j] / s  + w[j] * _f[s] ;
        }
        w[0]  = -w[0] *   xs1 ;
        c[0] +=  w[0] * _f[s] ;
      }
      ///  finally set parameters
      for ( unsigned short i = 0 ; i < N ; ++i ) { setPar ( i , c[i] ) ; }
    }
    // ======================================================================
    ///  Bernstein plus      constant
    inline Bernstein operator+( const Bernstein& p , const double v )
    { return Bernstein ( p ) += v ; } //  Bernstein plus constant
    ///  Bernstein multiply  constant
    inline Bernstein operator*( const Bernstein& p , const double v )
    { return Bernstein ( p ) *= v ; } //  Bernstein plus constant
    ///  Bernstein minus constant
    inline Bernstein operator-( const Bernstein& p , const double v )
    { return Bernstein ( p ) -= v ; } //  Bernstein plus constant
    ///  Bernstein divide constant
    inline Bernstein operator/( const Bernstein& p , const double v )
    { return Bernstein ( p ) /= v ; } //  Bernstein plus constant
    ///  Constant plus  Bernstein
    inline Bernstein operator+( const double v , const Bernstein& p ) { return p +   v  ; }
    ///  Constant times Bernstein
    inline Bernstein operator*( const double v , const Bernstein& p ) { return p *   v  ; }
    ///  Constant minus Bernstein
    inline Bernstein operator-( const double v , const Bernstein& p ) { return v + (-p) ; }
    // ========================================================================
    ///  Bernstein plus     Bernstein
    inline Bernstein operator+( const Bernstein& a , const Bernstein& b  )
    { return a.sum      ( b ) ; } //  Bernstein plus     Bernstein
    ///  Bernstein minus    Bernstein
    inline Bernstein operator-( const Bernstein& a , const Bernstein& b  )
    { return a.subtract ( b ) ; } //  Bernstein subtract Bernstein
    ///  Bernstein multiply Bernstein
    inline Bernstein operator*( const Bernstein& a , const Bernstein& b  )
    { return a.multiply ( b ) ; } //  Bernstein multiply  Bernstein
    ///  polynomial division: quotient
    inline Bernstein operator/( const Bernstein& a , const Bernstein& b  )
    { return a.quotient ( b ) ; } // polynomial division: quotient
    ///  polynomial division: reminder
    inline Bernstein operator%( const Bernstein& a , const Bernstein& b  )
    { return a.reminder ( b ) ; } // polynomial division: reminder
    // ========================================================================
    inline void swap ( Bernstein& a , Bernstein& b ) { a.swap ( b ) ; }
    // ========================================================================
    /** polynomial division
     *  Return pair of polynomials q and r, such as
     *  \f$ a = qb + r \f$
     *  @param a
     *  @param b
     */
    inline
    std::pair<Bernstein,Bernstein> divmod
    ( const Bernstein& a ,
      const Bernstein& b ) { return a.divmod ( b ) ; }
    // ========================================================================
    /** Dual basic Bernstein function
     *  The dual basic functions \f$ d^n_j(x)\f$ are defined as
     *   \f$  \int_{x_{min}}^{x_{max}}   b^n_k(x) d^n_j(x) = \delta_{kj}\f$,
     *   where \f$b^n_k(x)\f$ is basic Bernstein polynomial
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date 2016-07-03
     */
    class GAUDI_API BernsteinDualBasis
    {
      // ======================================================================
    public :
      // ======================================================================
      unsigned short k () const { return m_k                  ; }
      unsigned short N () const { return m_bernstein.degree() ; }
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the order
      BernsteinDualBasis ( const unsigned short N     = 0 ,
                           const unsigned short k     = 0 ) ;
      /// copy constructor
      BernsteinDualBasis  ( const BernsteinDualBasis&  right ) ;
      /// cconstructor
      BernsteinDualBasis  (       BernsteinDualBasis&& right ) ;
      /// destructor
      ~BernsteinDualBasis () = default;
      // ======================================================================
    public:
      // ======================================================================
      /// calculate the value of dual bernstein function
      double operator() ( const double x ) const
      { return m_k <= N() ? m_bernstein ( x ) : 0.0 ; }
      // ======================================================================
    public:
      // ======================================================================
      ///  get the parameters
      double par ( const unsigned short i ) const { return m_bernstein.par( i ) ; }
      ///  get all parameters
      const std::vector<double>& pars()     const { return m_bernstein.pars() ; }
      // ======================================================================
    private:
      // ======================================================================
      /// the index
      unsigned short   m_k         ; // the index
      /// the actual bernstein polynomial
      Bernstein        m_bernstein ; // the actual bernstein polynomial
      // ======================================================================
    };
    // ========================================================================
  } //                                             end of namespace Gaudi::Math
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
namespace LHCb
{
  // ==========================================================================
  namespace Math
  {
    // ========================================================================
    /// specialization: is Bernstein polynomial close to zero?
    template <>
    struct Zero<Gaudi::Math::Bernstein> 
    {
    public:
      // ======================================================================
      // is Bernstein polynomial almost to zero ?
      inline bool operator () ( const Gaudi::Math::Bernstein& b ) const
      { return m_zero ( b.pars() ) ; }
    private:
      // ======================================================================
      /// the actual comparator 
      Zero< std::vector<double> > m_zero ;
      // ======================================================================      
    };
    // ========================================================================
    /// specialization: is Bernstein polynomial small enough ?
    template <>
    struct Tiny<Gaudi::Math::Bernstein> 
    {
    public:
      // ======================================================================
      Tiny ( const double n ) : m_tiny ( std::abs ( n ) ) {}
      Tiny () = delete ;
      // is Bernstein polynomial sufficiently small 
      inline bool operator () ( const Gaudi::Math::Bernstein& b ) const
      { return m_tiny ( b.norm() ) ; }
      // ======================================================================
    private:
      // ======================================================================
      Tiny<double> m_tiny ;
      // ======================================================================      
    };
    // ========================================================================
    /** scale all coefficients with 2**i
     *  @param  b (INPUT) Berstein polynomial
     *  @param  i (INPUT) the scaling binary exponent
     *  @return the scaled polynomial
     */
    inline 
    Gaudi::Math::Bernstein 
    ldexp ( const Gaudi::Math::Bernstein& b , 
            const short                   i ) { return b.ldexp ( i ) ; }
    // ========================================================================
  } 
  // ==========================================================================
}
// ============================================================================
namespace Gaudi
{
  // ==========================================================================
  namespace Math
  {  
    // ========================================================================
    /** get the integral between low and high for a product of Bernstein
     *  polynom and the exponential function with the exponent tau
     *  \f[  \int_{a}^{b} \mathcal{B} e^{\tau x } \mathrm{d}x \f]
     *  @param poly  bernstein polynomial
     *  @param tau   slope parameter for exponential
     *  @param a     low  integration range
     *  @param b     high integration range
     */
    GAUDI_API
    double integrate
    ( const Gaudi::Math::Bernstein& poly ,
      const double                  tau  ,
      const double                  a    ,
      const double                  b    ) ;
    // ========================================================================
    /** get the integral between 0 and 1 for a product of basic  Bernstein
     *  polynom and the exponential function with the exponent tau
     *  \f[  \int_{0}^{1} \mathcal{B} e^{\tau x } \mathrm{d}x \f]
     *  @param b     basic bernstein polynomial
     *  @param tau   slope parameter for exponential
     */
    GAUDI_API
    double integrate
    ( const Gaudi::Math::Bernstein::Basic& b    ,
      const double                         tau  ) ;
    // =======================================================================
    /** get the integral between \f$x_{min}\f$ and \f$x_{max}\f$ for
     *  a product of Bernstein polynom and the exponential function
     *   with the exponent tau
     *  \f[  \int_{x_{min}}^{x_{max}} \mathcal{B} e^{\tau x } \mathrm{d}x \f]
     *  @param poly  bernstein polynomial
     *  @param tau   slope parameter for exponential
     */
    GAUDI_API
    double integrate
    ( const Gaudi::Math::Bernstein& poly ,
      const double                  tau  ) ;
    // ========================================================================
    /** get the integral between 0 and 1 for a product of basic  Bernstein
     *  polynom and monomial or degree m
     *  \f[  \int_{0}^{1} \mathcal{B} \frac{x^m}{m!} \mathrm{d}x \f]
     *  @param b     basic bernstein polynomial
     *  @param m     degree of monomial
     */
    GAUDI_API
    double integrate_poly
    ( const Gaudi::Math::Bernstein::Basic& b ,
      const unsigned short                 m ) ;
    // =======================================================================
    /** get the integral between xmin and xmax Bernstein
     *  polynom and monomial or degree m
     *  \f[  \int_{x_min}^{x_max} \mathcal{B} \frac{(x-x_min)^m}{m!} \mathrm{d}x \f]
     *  @param b     basic bernstein polynomial
     *  @param m     degree of monomial
     */
    GAUDI_API
    double integrate_poly
    ( const Gaudi::Math::Bernstein& b ,
      const unsigned short          m ) ;
    // ========================================================================
    /** get the integral between xmin and xmax Bernstein
     *  polynom and monomial or degree m
     *  \f[  \int_{low}^{high} \mathcal{B} \frac{(x-x_min)^m}{m!} \mathrm{d}x \f]
     *  @param b     basic bernstein polynomial
     *  @param m     degree of monomial
     *  @param low   low  integration limit
     *  @param high  high integtation limit
     */
    GAUDI_API
    double integrate_poly
    ( const Gaudi::Math::Bernstein& b    ,
      const unsigned short          m    ,
      const double                  low  ,
      const double                  high ) ;
    // =======================================================================
    /** de Casteljau algorithm for summation of Bernstein polynomials
     *  \f$ f(x) = \sum_i p_i B_ik(x) \f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-02-10
     */
    GAUDI_API double casteljau
    ( const std::vector<double>& pars ,
      const double               x    ) ;
    // ======================================================================== 
    /** deflate Bernstein polynomial at  <code>x=xmin</code>
     *  \f$ b(x)-b(x_{min})=(x-x_{min})*d(x)\f$      
     *  @param  b  berntein polynomial to be deflated 
     *  @return deflated polinomial "d"
     */ 
    Gaudi::Math::Bernstein
    deflate_left ( const Gaudi::Math::Bernstein& b ) ;    
    // ========================================================================
    /** deflate Bernstein polynomial at  <code>x=xmax</code>
     *  \f$ b(x)-b(x_{max})=(x-x_{max})*d(x)\f$      
     *  @param  b  berntein polynomial to be deflated 
     *  @return deflated polinomial "d"
     */ 
    Gaudi::Math::Bernstein
    deflate_right ( const Gaudi::Math::Bernstein& b ) ;
    // ========================================================================
    /** deflate Bernstein polynomial at  <code>x=x0</code>
     *  \f$ b(x)-b(x_{0})=(x-x_{0})*d(x)\f$      
     *  @param  b  berntein polynomial to be deflated 
     *  @param  x0 the point 
     *  @return deflated polinomial "d"
     */ 
    Gaudi::Math::Bernstein
    deflate       ( const Gaudi::Math::Bernstein& b , const double x0 ) ;
    // ========================================================================
    /** get abscissas of crosssing point of the control polygon 
     *  for Bernstein polynomial
     *  @param  b bernstein polynomial
     *  @reutrn abscissas of crossing points of the control  polygon
     */
    std::vector<double> 
    crossing_points  ( const Gaudi::Math::Bernstein& b ) ;
    // ========================================================================    
    /** get number of (strickt) sign changes in trhe sequnce of coefficients
     *  for Bernstein polynomial 
     *  if  N is number of sign changes, then the number of real roots R is 
     *  \f$ R = N - 2K\f$, where K is non-negative integer
     */
    unsigned short 
    sign_changes ( const Gaudi::Math::Bernstein& b ) ;
    // ========================================================================
    /** get the most left crossing  point of convex hull with  x-axis 
     *  (it is a step  towards finding the most left root, if any 
     *  if convex hull does not cross the x-axis, xmax is returned      
     */
    double left_line_hull ( const Gaudi::Math::Bernstein& b  ) ;
    // ========================================================================
    /** get the most right rossing  point of convex hull with  x-axis 
     *  (it is a step  towards finding the most right root, if any 
     *  if convex hull does not cross the x-axis, xmin is returned      
     */
    double right_line_hull ( const Gaudi::Math::Bernstein& b ) ;
  } 
  // ==========================================================================
}
// ============================================================================
// add couple of functions into Gaudi::Math::Interpolation namespace
// ============================================================================
namespace Gaudi
{
  // ==========================================================================
  namespace Math
  {
    // ========================================================================
    namespace Interpolation
    {
      // ======================================================================
      /** construct interpolation polynomial (in Bernstein form)
       *  @param xbegin   begin-iterator for vector of abscissas
       *  @param xend     end-iterator for vector of abscissas
       *  @param ybegin   begin-iterator for vector of function
       *  @param yend     end-iterator for vector of function
       *  @param xmin     low  edge for Bernstein polynomial
       *  @param xmax     high edge for Bernstein polynomial
       *  - if vector of y is longer  than vector x, extra values are ignored
       *  - if vector of y is shorter than vector x, missing entries are assumed to be zero
       *  It relies on Newton-Bernstein algorithm
       *  @see http://arxiv.org/abs/1510.09197
       *  @see Mark Ainsworth and Manuel A. Sanches,
       *       "Computing of Bezier control points of Largangian interpolant
       *       in arbitrary dimension", arXiv:1510.09197 [math.NA]
       *  @see http://adsabs.harvard.edu/abs/2015arXiv151009197A
       *  @see Gaudi::Math::Bernstein
       *  @code
       *  std::array<double,5> x = ... ; // abscissas
       *  std::vector<double,> f = ... ; // function values
       *  Gaudi::Math::Bernstein p = bernstein ( x.begin() , x.end() ,
       *                                           f.begin() , f.end() , -1 , 1 );
       *  std::cout << " interpolant at x=0.1 is " << p(0.1) << std::endl ;
       *  std::cout << " interpolant at x=0.2 is " << p(0.2) << std::endl ;
       *  @endcode
       */
      template <class XITERATOR, class YITERATOR>
      inline
      Gaudi::Math::Bernstein
      bernstein ( XITERATOR    xbegin ,
                  XITERATOR    xend   ,
                  YITERATOR    ybegin ,
                  YITERATOR    yend   ,
                  const double xmin   ,
                  const double xmax   )
      {
        return Gaudi::Math::Bernstein ( xbegin , xend ,
                                        ybegin , yend ,
                                        xmin   , xmax ) ;
      }
      // ======================================================================
      /** construct interpolation polynomial (in Bernstein form)
       *  @param func     the function
       *  @param xbegin   begin-iterator for vector of abscissas
       *  @param xend     end-iterator for vector of abscissas
       *  @param xmin low  edge for Bernstein polynomial
       *  @param xmax high edge for Bernstein polynomial
       *  - if vector of y is longer  than vector x, extra values are ignored
       *  - if vector of y is shorter than vector x, missing entries are assumed to be zero
       *  It relies on Newton-Bernstein algorithm
       *  @see http://arxiv.org/abs/1510.09197
       *  @see Mark Ainsworth and Manuel A. Sanches,
       *       "Computing of Bezier control points of Largangian interpolant
       *       in arbitrary dimension", arXiv:1510.09197 [math.NA]
       *  @see http://adsabs.harvard.edu/abs/2015arXiv151009197A
       *  @see Gaudi::Math::Bernstein
       *  @code
       *  auto f = [] ( double t ) { return std::sin ( t ) ; }
       *  std::array<double,5> x = ... ; // abscissas
       *  Gaudi::Math::Bernstein p = interpolate ( f , x.begin() , x.end() , -1 , 1 );
       *  std::cout << " interpolant at x=0.1 is " << p(0.1) << std::endl ;
       *  std::cout << " interpolant at x=0.2 is " << p(0.2) << std::endl ;
       *  @endcode
       */
      template <class XITERATOR, class FUNCTION>
      inline
      Gaudi::Math::Bernstein
      bernstein ( FUNCTION     func   ,
                  XITERATOR    xbegin ,
                  XITERATOR    xend   ,
                  const double xmin   ,
                  const double xmax   )
      {
        const unsigned int N = std::distance ( xbegin , xend ) ;
        std::vector<double> f ( N ) ;
        std::transform ( xbegin , xend , f.begin () , func ) ;
        return Gaudi::Math::Bernstein ( xbegin    , xend    ,
                                        f.begin() , f.end() ,
                                        xmin      , xmax    ) ;
      }
      // ================================================================================
      /** construct interpolation polynomial (in Bernstein form) using Gauss-Lobatto grid,
       *  that minimises Runge's effect.
       *  @param func      the function
       *  @param N         the interpolation  degree
       *  @param xmin low  edge for Bernstein polynomial
       *  @param xmax high edge for Bernstein polynomial
       *  - if vector of y is longer  than vector x, extra values are ignored
       *  - if vector of y is shorter than vector x, missing entries are assumed to be zero
       *  It relies on Newton-Bernstein algorithm
       *  @see http://arxiv.org/abs/1510.09197
       *  @see Mark Ainsworth and Manuel A. Sanches,
       *       "Computing of Bezier control points of Largangian interpolant
       *       in arbitrary dimension", arXiv:1510.09197 [math.NA]
       *  @see http://adsabs.harvard.edu/abs/2015arXiv151009197A
       *  @see Gaudi::Math::Bernstein
       *  @code
       *  auto f = [] ( double t ) { return std::sin ( t ) ; }
       *  Gaudi::Math::Bernstein p = lobatto ( f , 5 , -1 , 1 );
       *  std::cout << " interpolant at x=0.1 is " << p(0.1) << std::endl ;
       *  std::cout << " interpolant at x=0.2 is " << p(0.2) << std::endl ;
       *  @endcode
       */
      template <class FUNCTION>
      inline
      Gaudi::Math::Bernstein
      lobatto  ( FUNCTION             func ,
                 const unsigned short N    ,
                 const double         xmin ,
                 const double         xmax )
      {
        // trivial case:
        if ( 0 == N )
        {
          //  trivial grid
          const double x = 0.5 * ( xmin + xmax ) ;
          const double y = func ( x ) ;
          return Gaudi::Math::Bernstein ( &x   , &x + 1 ,
                                          &y   , &y + 1 ,
                                          xmin , xmax   ) ;
        }
        //
        std::vector<double> x ( N + 1 ) ;
        //
        const double x_min = std::min ( xmin , xmax ) ;
        const double x_max = std::max ( xmin , xmax ) ;
        //
        const double xhs = 0.5 * ( x_min + x_max ) ;
        const double xhd = 0.5 * ( x_max - x_min ) ;
        //
        const long double pi_N1 = M_PIl / ( N  - 1 ) ;
        auto _xi_ = [xhs,xhd,pi_N1] ( const unsigned short k )
          { return xhs - std::cos ( pi_N1 * k ) * xhd  ; } ;
        x.front() = x_min ;
        x.back () = x_max ;
        for ( unsigned short i = 1 ; i + 1 < N ; ++i ) { x[i] =  _xi_ ( i )  ; }
        //
        return bernstein ( func ,
                           x.begin () , x.end () ,
                           xmin       , xmax     ) ;
      }
      // ================================================================================
      /** construct interpolation polynomial (in Bernstein form) using Gauss-Lobatto grid,
       *  that minimises Runge's effect.
       *  @param func      the function
       *  @param N         the interpolation  degree
       *  @param xmin low  edge for Bernstein polynomial
       *  @param xmax high edge for Bernstein polynomial
       *  - if vector of y is longer  than vector x, extra values are ignored
       *  - if vector of y is shorter than vector x, missing entries are assumed to be zero
       *  It relies on Newton-Bernstein algorithm
       *  @see http://arxiv.org/abs/1510.09197
       *  @see Mark Ainsworth and Manuel A. Sanches,
       *       "Computing of Bezier control points of Largangian interpolant
       *       in arbitrary dimension", arXiv:1510.09197 [math.NA]
       *  @see http://adsabs.harvard.edu/abs/2015arXiv151009197A
       *  @see Gaudi::Math::Bernstein
       *  @code
       *  auto f = [] ( double t ) { return std::sin ( t ) ; }
       *  Gaudi::Math::Bernstein p = lobatto<5> ( f , -1 , 1 );
       *  std::cout << " interpolant at x=0.1 is " << p(0.1) << std::endl ;
       *  std::cout << " interpolant at x=0.2 is " << p(0.2) << std::endl ;
       *  @endcode
       */
      template <unsigned short N, class FUNCTION>
      inline
      Gaudi::Math::Bernstein
      lobatto  ( FUNCTION             func ,
                 const double         xmin ,
                 const double         xmax )
      {
        // trivial case:
        if ( 0 == N )
        {
          //  trivial grid
          const double x = 0.5 * ( xmin + xmax ) ;
          const double y = func ( x ) ;
          return Gaudi::Math::Bernstein ( &x   , &x + 1 ,
                                          &y   , &y + 1 ,
                                          xmin , xmax   ) ;
        }
        //
        std::array<double,N+1> x ;
        //
        const double x_min = std::min ( xmin , xmax ) ;
        const double x_max = std::max ( xmin , xmax ) ;
        //
        const double xhs = 0.5 * ( x_min + x_max ) ;
        const double xhd = 0.5 * ( x_max - x_min ) ;
        //
        const long double pi_N1 = M_PIl / ( N  - 1 ) ;
        auto _xi_ = [xhs,xhd,pi_N1] ( const unsigned short k )
          { return xhs - std::cos ( pi_N1 * k ) * xhd  ; } ;
        x.front () = x_min ;
        x.back  () = x_max ;
        for ( unsigned short i = 1 ; i + 1 < N ; ++i )
        { x[i] =  _xi_ ( i )  ; }
        //
        return bernstein ( func ,
                           x.begin () , x.end () ,
                           xmin       , xmax     ) ;
      }
      // ======================================================================
      /** construct interpolation polynomial (in Bernstein form)
       *  @param x       vector of abscissas
       *  @param y       vector of function values
       *  @param xmin low  edge for Bernstein polynomial
       *  @param xmax high edge for Bernstein polynomial
       *  - if vector of y is longer  than vector x, extra values are ignored
       *  - if vector of y is shorter than vector x, missing entries are assumed to be zero
       *  It relies on Newton-Bernstein algorithm
       *  @see http://arxiv.org/abs/1510.09197
       *  @see Mark Ainsworth and Manuel A. Sanches,
       *       "Computing of Bezier control points of Largangian interpolant
       *       in arbitrary dimension", arXiv:1510.09197 [math.NA]
       *  @see http://adsabs.harvard.edu/abs/2015arXiv151009197A
       *  @see Gaudi::Math::Bernstein
       *  @code
       *  std::vector<double> x = ... ; // abscissas
       *  std::vector<double> y = ... ; // functionvalues
       *  Gaudi::Math::Bernstein p = interpolate ( x , y , -1 , 1 );
       *  std::cout << " interpolant at x=0.1 is " << p(0.1) << std::endl ;
       *  std::cout << " interpolant at x=0.2 is " << p(0.2) << std::endl ;
       *  @endcode
       */
      GAUDI_API
      Gaudi::Math::Bernstein
      bernstein ( const std::vector<double>& x    ,
                  const std::vector<double>& y    ,
                  const double               xmin ,
                  const double               xmax );
      // ======================================================================
      /** construct interpolation polynomial (in Bernstein form)
       *  @param func    the function
       *  @param x       vector of abscissas
       *  @param xmin low  edge for Bernstein polynomial
       *  @param xmax high edge for Bernstein polynomial
       *  - if vector of y is longer  than vector x, extra values are ignored
       *  - if vector of y is shorter than vector x, missing entries are assumed to be zero
       *  It relies on Newton-Bernstein algorithm
       *  @see http://arxiv.org/abs/1510.09197
       *  @see Mark Ainsworth and Manuel A. Sanches,
       *       "Computing of Bezier control points of Largangian interpolant
       *       in arbitrary dimension", arXiv:1510.09197 [math.NA]
       *  @see http://adsabs.harvard.edu/abs/2015arXiv151009197A
       *  @see Gaudi::Math::Bernstein
       *  @code
       *  auto f = [] ( double t ) { return std::sin ( t ) ; }
       *  std::vector<double> x = ... ; // abscissas
       *  Gaudi::Math::Bernstein p = interpolate ( f , x , -1 , 1 );
       *  std::cout << " interpolant at x=0.1 is " << p(0.1) << std::endl ;
       *  std::cout << " interpolant at x=0.2 is " << p(0.2) << std::endl ;
       *  @endcode
       */
      GAUDI_API
      Gaudi::Math::Bernstein
      bernstein ( std::function<double(double)> func ,
                  const std::vector<double>&    x    ,
                  const double                  xmin ,
                  const double                  xmax ) ;
      // ======================================================================
      /** construct interpolation polynomial (in Bernstein form) using Gauss-Lobatto grid,
       *  that minimises Runge's effect.
       *  @param func      the function
       *  @param N         the interpolation  degree
       *  @param xmin low  edge for Bernstein polynomial
       *  @param xmax high edge for Bernstein polynomial
       *  - if vector of y is longer  than vector x, extra values are ignored
       *  - if vector of y is shorter than vector x, missing entries are assumed to be zero
       *  It relies on Newton-Bernstein algorithm
       *  @see http://arxiv.org/abs/1510.09197
       *  @see Mark Ainsworth and Manuel A. Sanches,
       *       "Computing of Bezier control points of Largangian interpolant
       *       in arbitrary dimension", arXiv:1510.09197 [math.NA]
       *  @see http://adsabs.harvard.edu/abs/2015arXiv151009197A
       *  @see Gaudi::Math::Bernstein
       *  @code
       *  auto f = [] ( double t ) { return std::sin ( t ) ; }
       *  Gaudi::Math::Bernstein p = bernstein ( f , 5 , -1 , 1 );
       *  std::cout << " interpolant at x=0.1 is " << p(0.1) << std::endl ;
       *  std::cout << " interpolant at x=0.2 is " << p(0.2) << std::endl ;
       *  @endcode
       */
      GAUDI_API
      Gaudi::Math::Bernstein
      bernstein ( std::function<double(double)> func ,
                  const unsigned short          N    ,
                  const double                  xmin ,
                  const double                  xmax ) ;
      // ======================================================================
    } //                            end of namespace Gaudi::Math::Interpolation
    // ========================================================================
  } //                                             end of namespace Gaudi::Math
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================



// ============================================================================
//                                                                      The END  
// ============================================================================
#endif // LHCBMATH_BERNSTEINPOLY_H
