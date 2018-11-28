/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// ============================================================================
#ifndef LHCBMATH_BPLINE_H
#define LHCBMATH_BPLINE_H 1

#include "LHCbMath/NSphere.h"
#include "LHCbMath/LHCbMath.h"

#include "GaudiKernel/StatusCode.h"

#include <vector>

// ============================================================================
/** @file LHCbMath/BSpline.h
 *  Simple implementation of (B,M,I)-splines and related stuff
 *  @see http://en.wikipedia.org/wiki/B-spline
 *  @see http://en.wikipedia.org/wiki/M-spline
 *  @see http://en.wikipedia.org/wiki/I-spline
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2014-11-27
 */
// ============================================================================
namespace Gaudi
{
  // ==========================================================================
  namespace Math
  {
    // ========================================================================
    class Bernstein ;
    // ========================================================================
    /** @class BSpline
     *  The basic spline   ("B-spline")
     *  @see http://en.wikipedia.org/wiki/B-spline
     *  @see http://link.springer.com/chapter/10.1007%2F978-3-0348-7692-6_6
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     */
    class GAUDI_API BSpline
    {
      // ======================================================================
    public:
      // ======================================================================
      /** constructor from the list of knots and the order
       *  vector of parameters will be calculated automatically
       *  @param knots  non-empty vector of poinst/knots
       *  @param order  the order of splines
       *  - vector of knots is not required to be ordered
       *  - duplicated knots will be ignored
       *  - min/max value will be used as interval boundaries
       *  - extra knots will added at the end of interval
       */
      BSpline ( const std::vector<double>& knots     ,
                const unsigned short       order = 3 ) ;
      // ======================================================================
      /** Constructor from the list of knots and list of parameters
       *  The spline order will be calculated automatically
       *  @param knots  non-empty vector of poinst/knots
       *  @param pars   non-empty vector of parameters
       *  - vector of knots  is not required to be ordered
       *  - min/max value will be used as interval boundaries
       *  - duplicated knots will be ignored
       *  - extra knots will added at the end of interval
       */
      BSpline ( const std::vector<double>& knots     ,
                const std::vector<double>& pars      ) ;
      // ======================================================================
      /** Constructor for uniform binning
       *  @param xmin   low  edge of spline interval
       *  @param xmax   high edge of spline interval
       *  @param inner  number of inner points in   (xmin,xmax) interval
       *  @param order  the degree of splline
       */
      BSpline ( const double         xmin   = 0 ,
                const double         xmax   = 1 ,
                const unsigned short inner  = 3 ,   // number of inner points
                const unsigned short order  = 3 ) ;
      // ======================================================================
      /** constructor from another spline with the different range
       *  @param b b-spline
       *  @param xmin minimal x-value
       *  @param xmax maximal y-value
       */
      BSpline ( const BSpline& b     ,
                const double   xmin  ,
                const double   xmax  ) ;
      // ======================================================================
      /** constructor from Bernstein polynomial
       *  @param b Bernstein polynomial
       */
      BSpline ( const Gaudi::Math::Bernstein& b ) ;
      // ======================================================================
      /// copy constructor
      BSpline ( const BSpline& ) = default ;
      /// move constructor
      BSpline (       BSpline&& right ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double operator () ( const double x ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// Greville's abscissa
      double              greville_abscissa  ( const unsigned short i ) const ;
      // ======================================================================
      /// get the vector of Greville's abscissas
      std::vector<double> greville_abscissas () const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get number of parameters
      std::size_t npars  () const { return m_pars.size() ; }
      /// are all parameters zero?
      // bool        zero   () const ;
      /// set k-parameter
      bool setPar        ( const unsigned short k , const double value ) ;
      /// set k-parameter
      bool setParameter  ( const unsigned short k , const double value )
      { return setPar    ( k , value ) ; }
      /// get the parameter value
      double  par        ( const unsigned short k ) const
      { return ( k < m_pars.size() ) ? m_pars[k] : 0.0 ; }
      /// get the parameter value
      double  parameter ( const unsigned short k ) const { return par ( k ) ; }
      /// get lower edge
      double xmin () const { return m_xmin ; }
      /// get upper edge
      double xmax () const { return m_xmax ; }
      /// get all parameters:
      const std::vector<double>& pars  () const { return m_pars  ; }
      /// get all knots
      const std::vector<double>& knots () const { return m_knots ; }
      /// the spline order
      unsigned short order  () const { return m_order ; }
      /// the spline order
      unsigned short degree () const { return m_order ; }
      // number of inner knots
      unsigned short inner  () const { return m_inner ; }
      // ======================================================================
    public: // technical: get the effective position for knot "index"
      // ======================================================================
      double knot_i ( const int index ) const
      {
        return
          index <  0                   ? m_knots.front() :
          index < (int) m_knots.size() ? m_knots[index]  : m_knots.back() ;
      }
      // ======================================================================
    public:
      // ======================================================================
      /// get the integral between xmin and xmax
      double  integral   () const ;
      /// get the integral between low and high
      double  integral   ( const double low , const double high ) const ;
      /// get the derivative at point "x"
      double  derivative ( const double x   ) const ;
      /** get integral   as function object
       *  @param C integration constant
       */
      BSpline indefinite_integral ( const double C = 0 ) const ;
      /// get derivative as function object
      BSpline derivative          () const ;
      // ======================================================================
    public:
      // ======================================================================
      /// is it a decreasing function?
      bool   decreasing    () const ;
      /// is it a increasing function?
      bool   increasing    () const ;
      /// is it a monothonical function?
      bool   monothonic    () const { return increasing() || decreasing() ; }
      /// is it a constant function?
      bool   constant      () const ;
      // ======================================================================
    public:
      // ======================================================================
      /** insert new (unique) knot into the list of knots
       *  @param t new knot  to be inserted
       *  @return true if knot is indeed inserted
       */
      bool insert ( const double t ) ;
      // ======================================================================
    public:
      // ======================================================================
      /**  calculate q-norm of the spline
       *  where q-norm is defined as:
       *  \f$ \left| f \right|_{q} = \left( \sum_i \left|c_i\right|^q\right)^{\frac{1}{q}} \f$
       *
       *  - q_inv = 0.0 ->  \f$ max_k    \left|c_k\right|  \f$
       *  - q_inv = 0.5 ->  \f$ \sqrt{ \sum_k  c_k^2 }     \f$
       *  - q_inv = 1.0 ->  \f$ \sum_k \left| c_k \right|  \f$
       */
      double   norm ( const double q_inv = 0 ) const ;
      /// scale  all coefficients with 2**i
      BSpline ldexp ( const short i )  const ;
      // ======================================================================
    public: // B-splines
      // ======================================================================
      /// get the value of the B-spline  i at point x
      double bspline ( const          short i , const double x )  const ;
      /// get the value of the B-spline  (i,k) at point x
      double bspline ( const          short i ,
                       const unsigned short k , const double x )  const ;
      // ======================================================================
    public: // M-splines
      // ======================================================================
      /// get the value of the M-spline  i at point x
      double mspline ( const          short i , const double x )  const ;
      /// get the value of the M-spline  (i,k) at point x
      double mspline ( const          short i ,
                       const unsigned short k , const double x )  const ;
      // ======================================================================
    public: // I-splines
      // ======================================================================
      /// get the value of the I-spline  i at point x
      double ispline ( const          short i , const double x )  const ;
      /// get the value of the I-spline  (i,k) at point x
      double ispline ( const          short i ,
                       const unsigned short k , const double x )  const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the underlying spline
      const Gaudi::Math::BSpline& bspline () const { return *this ; }
      // ======================================================================
    public: // simple  manipulations with  B-splines
      // ======================================================================
      /// simple  manipulations with spline: scale it!
      BSpline& operator *= ( const double a ) ;     // scale it!
      /// simple  manipulations with spline: scale it!
      BSpline& operator /= ( const double a ) ;     // scale it!
      /// simple  manipulations with spline: shift it!
      BSpline& operator += ( const double a ) ;     // shift it!
      /// simple  manipulations with spline: shift it!
      BSpline& operator -= ( const double a ) ;     // shift it!
      // ======================================================================
    public:
      // ======================================================================
      /// negate it!
      BSpline operator-() const ;
      // ======================================================================
    public:
      // ======================================================================
      /// assignement      operator
      BSpline& operator=( const BSpline& ) = default ;
      /// assignement move operator
      BSpline& operator=(       BSpline&& right ) ;
      // ======================================================================
    public: // operators for python
      // ======================================================================
      /// Sum of B-spline and a constant
      BSpline __add__   ( const double value ) const ;
      /// Sum of B-spline and a constant
      BSpline __radd__  ( const double value ) const ;
      /// Product B-spline and a constant
      BSpline __mul__   ( const double value ) const ;
      /// Product B-spline and a constant
      BSpline __rmul__  ( const double value ) const ;
      /// Subtract a constant from Benrstein polynomial
      BSpline __sub__   ( const double value ) const ;
      /// Constant minus B-spline
      BSpline __rsub__  ( const double value ) const ;
      /// Divide B-spline by a constant
      BSpline __div__   ( const double value ) const ;
      /// Negate B-spline
      BSpline __neg__   () const ;
      // ======================================================================
    private:
      // ======================================================================
      /// the list of knots
      std::vector<double>  m_knots  ;              // the list of knots
      /// the list of parameters
      std::vector<double>  m_pars   ;              // the list of parameters
      /// order of polynomial
      unsigned short       m_order  ;              // order of polynomial
      unsigned short       m_inner  ;              // number of inner points
      /// the left edge of interval
      double m_xmin  ;                             // the left edge of interval
      /// the right edge of interval
      double m_xmax  ;                             // the right edge of interval
      // ======================================================================
    private: // some caching for efficiency
      // ======================================================================
      /// the last active index
      mutable  unsigned short       m_jlast   ;     // the last active index
      /// parameters for integration
      mutable  std::vector<double>  m_pars_i  ;     // for integration
      /// extended list of knots for integration
      std::vector<double>  m_knots_i ;              // the list of knots
      // ======================================================================
    };
    // ========================================================================
    ///  B-spline plus      constant
    inline BSpline operator+( const BSpline& p , const double v )
    { return BSpline ( p ) += v ; }
    ///  B-spline multiply  constant
    inline BSpline operator*( const BSpline& p , const double v )
    { return BSpline ( p ) *= v ; }
    ///  B-spline minus constant
    inline BSpline operator-( const BSpline& p , const double v )
    { return BSpline ( p ) -= v ; }
    ///  B-spline divide constant
    inline BSpline operator/( const BSpline& p , const double v )
    { return BSpline ( p ) /= v ; }
    ///  Constant plus  B-spline
    inline BSpline operator+( const double v , const BSpline& p ) { return p +   v  ; }
    ///  Constant times B-spline
    inline BSpline operator*( const double v , const BSpline& p ) { return p *   v  ; }
    ///  Constant minus B-spline
    inline BSpline operator-( const double v , const BSpline& p ) { return v + (-p) ; }
    // ========================================================================
    /** @class PositiveSpline
     *  The special spline for non-negative function
     *  Actually it is a sum of M-splines with non-negative coefficients
     *  \f$ f(x) = \sum_i \alpha_i * M_i^k(x) \f$,
     *  with constraints  \f$  \sum_i \alpha_i=1\f$
     *  and \f$ 0 \le \alpha_i\f$.
     *  @see http://en.wikipedia.org/wiki/M-spline
     *  @see http://en.wikipedia.org/wiki/B-spline
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     */
    class GAUDI_API PositiveSpline
    {
      // ======================================================================
    public:
      // ======================================================================
      /** constructor from the list of knots and the order
       *  vector of parameters will be calculated automatically
       *  @param points non-empty vector of poinst/knots
       *  @param order  the order of splines
       *  - vector of points is not requires to be ordered
       *  - duplicated knots will be ignored
       *  - min/max value will be used as interval boundaries
       */
      PositiveSpline ( const std::vector<double>& points    ,
                       const unsigned short       order = 3 ) ;
      // ======================================================================
      /** Constructor from the list of knots and list of parameters
       *  The spline order will be calculated automatically
       *  @param points non-empty vector of poinst/knots
       *  @param pars   non-empty vector of parameters
       *  - vector of points is not requires to be ordered
       *  - duplicated knots will be ignored
       *  - min/max value will be used as interval boundaries
       */
      PositiveSpline ( const std::vector<double>& points    ,
                       const std::vector<double>& pars      ) ;
      // ======================================================================
      /** Constructor for uniform binning
       *  @param xmin   low  edge of spline interval
       *  @param xmax   high edge of spline interval
       *  @param inner  number of inner points in   (xmin,xmax) interval
       *  @param order  the degree of splline
       */
      PositiveSpline ( const double         xmin   = 0 ,
                       const double         xmax   = 1 ,
                       const unsigned short inner  = 3 ,   // number of inner points
                       const unsigned short order  = 3 ) ;
      /// constructor from the basic spline
      PositiveSpline ( const BSpline& spline ) ;
      /// destructor
      virtual ~PositiveSpline() = default;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double operator () ( const double x ) const { return m_bspline ( x ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get number of parameters
      std::size_t npars  () const { return m_sphere.nPhi() ; }
      /// set k-parameter
      bool setPar        ( const unsigned short k , const double value ) ;
      /// set k-parameter
      bool setParameter  ( const unsigned short k , const double value )
      { return setPar    ( k , value ) ; }
      /// get the parameter value
      double  par        ( const unsigned short k ) const
      { return m_sphere.par ( k ) ; }
      /// get the parameter value
      double  parameter ( const unsigned short k ) const { return par ( k ) ; }
      /// get lower edge
      double xmin () const { return m_bspline.xmin() ; }
      /// get upper edge
      double xmax () const { return m_bspline.xmax() ; }
      /// get all parameters:
      const std::vector<double>& pars  () const { return m_sphere.pars  () ; }
      /// get all knots
      const std::vector<double>& knots () const { return m_bspline.knots() ; }
      /// the spline order
      unsigned short             order () const { return m_bspline.order() ; }
      // ======================================================================
    public: // technical: get the effective position for knot "index"
      // ======================================================================
      double knot_i ( const int index ) const { return m_bspline.knot_i ( index ) ; }
      // ======================================================================
    public:    public:
      // ======================================================================
      /// is it a decreasing function?
      bool   decreasing    () const { return m_bspline.decreasing () ; }
      /// is it a increasing function?
      bool   increasing    () const { return m_bspline.decreasing () ; }
      /// is it a monothonical function?
      bool   monothonic    () const { return increasing() || decreasing() ; }
      /// is it a constant function?
      bool   constant      () const { return m_bspline.constant  () ; }
      // ======================================================================
     public:
      // ======================================================================
      /// get the parameter sphere
      const Gaudi::Math::NSphere& sphere  () const { return m_sphere  ; }
      /// get the underlying spline
      const Gaudi::Math::BSpline& bspline () const { return m_bspline ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get the integral between xmin and xmax
      double  integral   () const ;
      /// get the integral between low and high
      double  integral   ( const double low , const double high ) const ;
      /// get the derivative at point "x"
      double  derivative ( const double x   ) const
      { return m_bspline.derivative ( x          ) ; }
      // ======================================================================
    public: // operators for python
      // ======================================================================
      /// Sum of spline and a constant
      BSpline __add__   ( const double value ) const { return m_bspline+ value ; }
      /// Sum of spline and a constant
      BSpline __radd__  ( const double value ) const { return m_bspline + value ; }
      /// Product of spline and a constant
      BSpline __mul__   ( const double value ) const { return m_bspline * value ; }
      /// Product of spline and a constant
      BSpline __rmul__  ( const double value ) const { return m_bspline * value ; }
      /// Subtract a constant from spline
      BSpline __sub__   ( const double value ) const { return m_bspline - value ; }
      /// Constant minus spline
      BSpline __rsub__  ( const double value ) const { return value - m_bspline ; }
      /// Divide spline by a constant
      BSpline __div__   ( const double value ) const { return m_bspline / value ; }
      /// Negate spline
      BSpline __neg__   () const { return -m_bspline; }
      // ======================================================================
    protected:
      // ======================================================================
      /// update coefficients
      virtual bool updateCoefficients  () ;
      // ======================================================================
    protected:
      // ======================================================================
      /// the underlying B-spline
      Gaudi::Math::BSpline  m_bspline ;  // the underlying B-spline
      /// the N-sphere of parameters
      Gaudi::Math::NSphere m_sphere   ; // the N-sphere of parameters
      // ======================================================================
    } ;
    // ========================================================================
    ///  Positive plus      constant
    inline BSpline operator+( const PositiveSpline& p , const double v )
    { return p.bspline () + v ; }
    ///  Positive multiply  constant
    inline BSpline operator*( const PositiveSpline& p , const double v )
    { return p.bspline () * v ; }
    ///  Positive minus     constant
    inline BSpline operator-( const PositiveSpline& p , const double v )
    { return p.bspline () - v ; }
    ///  Positive divide constant
    inline BSpline operator/( const PositiveSpline& p , const double v )
    { return p.bspline () / v ; }
    ///  Constant plus  Positive
    inline BSpline operator+( const double v , const PositiveSpline& p ) { return p + v  ; }
    ///  Constant times Positive
    inline BSpline operator*( const double v , const PositiveSpline& p ) { return p * v  ; }
    ///  Constant minus Positive
    inline BSpline operator-( const double v , const PositiveSpline& p )
    { return v - p.bspline() ; }
    // ========================================================================
    /** @class ConvexOnlySpline
     *  The special spline for non-negative function,
     *  with a fixed sign of second dervative (convex or concave)
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-09-20
     */
    class GAUDI_API ConvexOnlySpline : public PositiveSpline
    {
      // ======================================================================
    public:
      // ======================================================================
      /** constructor from the list of knots and the order
       *  vector of parameters will be calculated automatically
       *  @param points non-empty vector of poinst/knots
       *  @param order  the order of splines
       *  - vector of points is not requires to be ordered
       *  - duplicated knots will be ignored
       *  - min/max value will be used as interval boundaries
       */
      ConvexOnlySpline
        ( const std::vector<double>& points            ,
          const unsigned short       order      = 3    ,
          const bool                 convex     = true ) ;
      // ======================================================================
      /** Constructor from the list of knots and list of parameters
       *  The spline order will be calculated automatically
       *  @param points non-empty vector of poinst/knots
       *  @param pars   non-empty vector of parameters
       *  - vector of points is not requires to be ordered
       *  - duplicated knots will be ignored
       *  - min/max value will be used as interval boundaries
       */
      ConvexOnlySpline
        ( const std::vector<double>& points         ,
          const std::vector<double>& pars           ,
          const bool                 Connvex = true ) ;
      // ======================================================================
      /** Constructor for uniform binning
       *  @param xmin   low  edge of spline interval
       *  @param xmax   high edge of spline interval
       *  @param inner  number of inner points in   (xmin,xmax) interval
       *  @param order  the degree of splline
       */
      ConvexOnlySpline
        ( const double         xmin       = 0    ,
          const double         xmax       = 1    ,
          const unsigned short inner      = 2    ,   // number of inner points
          const unsigned short order      = 3    ,
          const bool           convex     = true ) ;
      /// constructor from positive spline
      ConvexOnlySpline ( const PositiveSpline& spline     ,
                         const bool            increasing ) ;
      /// constructor from the basic spline
      ConvexOnlySpline ( const BSpline&        spline     ,
                         const bool            increasing ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// convex  function ?
      bool convex    () const { return m_convex    ; }
      /// concave function ?
      bool concave   () const { return  !convex () ; }
      // ======================================================================
    protected:
      // ======================================================================
      /// update coefficients
      bool updateCoefficients  ()  override;
      // ======================================================================
    protected:
      // ======================================================================
      /// increasing function?
      bool m_convex ;  // convex function?
      // ======================================================================
    } ;
    // ========================================================================
    /** @class MonothonicSpline
     *  The special spline for non-negative increasing function,
     *  (well, actually non-decreasing)
     *  Actually it is a sum of B-splines with
     *  non-decreasing coefficients
     *  \f$ f(x) = \sum_i \alpha_i * B_i^k(x) \f$,
     *  with constraint \f$ 0 \le \alpha_{i} \le \alpha_{i+1}\f$ and
     *  normalization is\f$ f(x_{max}=1\f$
     *  @see http://en.wikipedia.org/wiki/I-spline
     *  @see http://en.wikipedia.org/wiki/B-spline
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     */
    class GAUDI_API MonothonicSpline : public PositiveSpline
    {
      // ======================================================================
    public:
      // ======================================================================
      /** constructor from the list of knots and the order
       *  vector of parameters will be calculated automatically
       *  @param points non-empty vector of poinst/knots
       *  @param order  the order of splines
       *  - vector of points is not requires to be ordered
       *  - duplicated knots will be ignored
       *  - min/max value will be used as interval boundaries
       */
      MonothonicSpline
        ( const std::vector<double>& points            ,
          const unsigned short       order      = 3    ,
          const bool                 increasing = true ) ;
      // ======================================================================
      /** Constructor from the list of knots and list of parameters
       *  The spline order will be calculated automatically
       *  @param points non-empty vector of poinst/knots
       *  @param pars   non-empty vector of parameters
       *  - vector of points is not requires to be ordered
       *  - duplicated knots will be ignored
       *  - min/max value will be used as interval boundaries
       */
      MonothonicSpline
        ( const std::vector<double>& points            ,
          const std::vector<double>& pars              ,
          const bool                 increasing = true ) ;
      // ======================================================================
      /** Constructor for uniform binning
       *  @param xmin   low  edge of spline interval
       *  @param xmax   high edge of spline interval
       *  @param inner  number of inner points in   (xmin,xmax) interval
       *  @param order  the degree of splline
       */
      MonothonicSpline
        ( const double         xmin       = 0    ,
          const double         xmax       = 1    ,
          const unsigned short inner      = 2    ,   // number of inner points
          const unsigned short order      = 3    ,
          const bool           increasing = true ) ;
      /// constructor from positive spline
      MonothonicSpline ( const PositiveSpline& spline     ,
                         const bool            increasing ) ;
      /// constructor from the basic spline
      MonothonicSpline ( const BSpline&        spline     ,
                         const bool            increasing ) ;
      // ======================================================================
    public:
      // ======================================================================
      bool increasing () const { return m_increasing    ; }
      bool decreasing () const { return  !increasing () ; }
      bool monothonic () const { return true ; }
      // ======================================================================
    protected:
      // ======================================================================
      /// update coefficients
      bool updateCoefficients  ()  override;
      // ======================================================================
    protected:
      // ======================================================================
      /// increasing function?
      bool m_increasing ;  // increasing function?
      // ======================================================================
    } ;
    // ========================================================================
    /** @class ConvexSpline
     *  The special spline with following properties :
     *   - it is positive
     *   - it it monothonic (increasing or decreasing)
     *   - it is eitehr convex or concave
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     */
    class GAUDI_API ConvexSpline : public MonothonicSpline
    {
      // ======================================================================
    public:
      // ======================================================================
      /** constructor from the list of knots and the order
       *  vector of parameters will be calculated automatically
       *  @param points non-empty vector of poinst/knots
       *  @param order  the order of splines
       *  - vector of points is not requires to be ordered
       *  - duplicated knots will be ignored
       *  - min/max value will be used as interval boundaries
       */
      ConvexSpline
        ( const std::vector<double>& points            ,
          const unsigned short       order      = 3    ,
          const bool                 increasing = true ,
          const bool                 convex     = true ) ;
      /** Constructor from the list of knots and list of parameters
       *  The spline order will be calculated automatically
       *  @param points non-empty vector of poinst/knots
       *  @param pars   non-empty vector of parameters
       *  - vector of points is not requires to be ordered
       *  - duplicated knots will be ignored
       *  - min/max value will be used as interval boundaries
       */
      ConvexSpline
        ( const std::vector<double>& points            ,
          const std::vector<double>& pars              ,
          const bool                 increasing = true ,
          const bool                 convex     = true ) ;
      /** Constructor for uniform binning
       *  @param xmin   low  edge of spline interval
       *  @param xmax   high edge of spline interval
       *  @param inner  number of inner points in   (xmin,xmax) interval
       *  @param order  the degree of splline
       */
      ConvexSpline
        ( const double            xmin       = 0    ,
          const double            xmax       = 1    ,
          const unsigned short    inner      = 2    ,   // number of inner points
          const unsigned short    order      = 3    ,
          const bool              increasing = true ,
          const bool              convex     = true ) ;
      /// constructor from positive spline
      ConvexSpline
        ( const PositiveSpline&   spline     ,
          const bool              increasing ,
          const bool              convex      ) ;
      /// constructor from basic spline
      ConvexSpline
        ( const BSpline&          spline     ,
          const bool              increasing ,
          const bool              convex      ) ;
      /// constructor from monothonic spline
      ConvexSpline
        ( const MonothonicSpline& spline     ,
          const bool              convex      ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// convex?
      bool convex   () const { return m_convex    ; } // convex?
      /// concave?
      bool concave  () const { return  !convex () ; } // concave?
      // ======================================================================
    protected:
      // ======================================================================
      /// update coefficients
      bool updateCoefficients  ()  override;
      // ======================================================================
    protected:
      // ======================================================================
      /// convex function?
      bool m_convex ;  // convex function?
      // ======================================================================
    } ;
    // =========================================================================
    /** @class GenericSpline2D
     *  GenericSpline in 2D
     */
    class GAUDI_API  GenericSpline2D
    {
      // ======================================================================
    public:
      // ======================================================================
      GenericSpline2D ( const BSpline& xspline = BSpline() ,
                        const BSpline& yspline = BSpline() ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double operator () ( const double x , const double y ) const
      { return evaluate  ( x , y ) ; }
      /// get the value
      double evaluate    ( const double x , const double y ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get number of parameters
      std::size_t npars () const { return m_pars.size() ; }
      /// set k-parameter
      bool setPar       ( const unsigned int k , const double value )
      { return setParameter ( k , value ) ; }
      /// set k-parameter
      bool setParameter ( const unsigned int k , const double value ) ;
      /// set (i,j) paramter
      bool setPar       ( const unsigned short i ,
                          const unsigned short j , const double value )
      { return setPar ( index ( i , j ) , value ) ; }
      /// get the parameter value
      double  par       ( const unsigned int k ) const
      { return k < m_pars.size() ? m_pars[k] : 0.0 ; }
      /// get the parameter value
      double  parameter ( const unsigned int k ) const { return par ( k ) ; }
      /// get (i,j)-parameter
      double  par       ( const unsigned short i ,
                          const unsigned short j ) const
      { return par ( index ( i  , j ) ) ; }
      // ======================================================================
      // get all parameters
      const std::vector<double>& pars() const { return m_pars ; }
      // ======================================================================
    private:
      // ======================================================================
      unsigned int index ( const unsigned short i ,
                           const unsigned short j ) const
      {
        const unsigned short nx = m_xspline.npars() ;
        const unsigned short ny = m_yspline.npars() ;
        return
          i >= nx ? -1 :
          j >= ny ? -1 :
          1u * i * ny  + j ;
      }
      // ======================================================================
    public:
      // ======================================================================
      /// get lower/upper edges
      double         xmin   () const { return m_xspline.xmin  () ; }
      double         xmax   () const { return m_xspline.xmax  () ; }
      double         ymin   () const { return m_yspline.xmin  () ; }
      double         ymax   () const { return m_yspline.xmax  () ; }
      // spline order
      double         xorder () const { return m_xspline.order () ; }
      double         yorder () const { return m_yspline.order () ; }
      // inner knots
      double         xinner () const { return m_xspline.inner () ; }
      double         yinner () const { return m_yspline.inner () ; }
      // ======================================================================
    public: // few useful methdod
      // ======================================================================
      /// simple  manipulations with polynoms: shift it!
      GenericSpline2D& operator += ( const double a ) ;
      /// simple  manipulations with polynoms: shift it!
      GenericSpline2D& operator -= ( const double a ) ;
      /// simple  manipulations with polynoms: scale it!
      GenericSpline2D& operator *= ( const double a ) ;
      /// simple  manipulations with polynoms: scale it!
      GenericSpline2D& operator /= ( const double a ) ;
      // negate it
      GenericSpline2D  operator-()  const ;
      // ======================================================================
    public:  // for python
      // ======================================================================
      /// Sum of GenericSpline polynomial and a constant
      GenericSpline2D __add__   ( const double value ) const ;
      /// Sum of GenericSpline polynomial and a constant
      GenericSpline2D __radd__  ( const double value ) const ;
      /// Product of GenericSpline polynomial and a constant
      GenericSpline2D __mul__   ( const double value ) const ;
      /// Product of GenericSpline polynomial and a constant
      GenericSpline2D __rmul__  ( const double value ) const ;
      /// Subtract a constant from Benrstein polynomial
      GenericSpline2D __sub__   ( const double value ) const ;
      /// Constant minus GenericSpline polynomial
      GenericSpline2D __rsub__  ( const double value ) const ;
      /// Divide Benrstein polynomial by a constant
      GenericSpline2D __div__   ( const double value ) const ;
      /// Negate GenericSpline polynomial
      GenericSpline2D __neg__   () const ;
     // ======================================================================
    public: // generic integrals
      // ======================================================================
      /** get the integral over 2D-region
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integral   ( const double xlow , const double xhigh ,
                          const double ylow , const double yhigh ) const ;
      /** get the integral over X  for given Y
       *  @param y  (INPU) y-value
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateX
        ( const double y    ,
          const double xlow , const double xhigh ) const ;
      /** get the integral over Y  for given X
       *  @param x  (INPU) y-value
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateY
        ( const double x    ,
          const double ylow , const double yhigh ) const ;
      // ======================================================================
    public: // specific integrals
      // ======================================================================
      /** get the integral over 2D-region
       *  \f[ x_{min}<x<x_{max}, y_{min}<y<y_{max}\f]
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integral   () const ;
      /** get the integral over X  for given Y
       *  @param y  (INPU) y-value
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateX ( const double y ) const ;
      /** get the integral over Y  for given X
       *  @param x  (INPU) y-value
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateY ( const double x ) const ;
      // ======================================================================
    public: // ingredients
      // =====================================================================
      // get x-spline
      const  Gaudi::Math::BSpline& xspline () const { return m_xspline ; }
      const  Gaudi::Math::BSpline& yspline () const { return m_yspline ; }
      // ======================================================================
    private:
      // ======================================================================
      // make the calcualtions
      double calculate ( const std::vector<double>& fx ,
                         const std::vector<double>& fy ) const ;
      // ======================================================================
    private:
      // ======================================================================
      /// X-spline
      mutable Gaudi::Math::BSpline m_xspline ; // x-spline
      /// Y-spline
      mutable Gaudi::Math::BSpline m_yspline ; // y-spline
      /// parameters
      std::vector<double>  m_pars ;
      // ======================================================================
    };
    // ========================================================================
    ///  Spline plus      constant
    inline GenericSpline2D operator+( const GenericSpline2D& p , const double v )
    { return GenericSpline2D ( p ) += v ; } //  GenericSpline plus constant
    ///  Spline multiply  constant
    inline GenericSpline2D operator*( const GenericSpline2D& p , const double v )
    { return GenericSpline2D ( p ) *= v ; } //  GenericSpline plus constant
    ///  Spline minus constant
    inline GenericSpline2D operator-( const GenericSpline2D& p , const double v )
    { return GenericSpline2D ( p ) -= v ; } //  GenericSpline plus constant
    ///  Spline divide constant
    inline GenericSpline2D operator/( const GenericSpline2D& p , const double v )
    { return GenericSpline2D ( p ) /= v ; } //  GenericSpline plus constant
    ///  Spline plus  GenericSpline
    inline GenericSpline2D operator+( const double v , const GenericSpline2D& p ) { return p +   v  ; }
    ///  Spline times GenericSpline
    inline GenericSpline2D operator*( const double v , const GenericSpline2D& p ) { return p *   v  ; }
    ///  Constant minus GenericSpline
    inline GenericSpline2D operator-( const double v , const GenericSpline2D& p ) { return v + (-p) ; }
    // =========================================================================
    /** @class GenericSpline2DSym
     *  GenericSpline in 2D
     */
    class GAUDI_API GenericSpline2DSym
    {
      // ======================================================================
    public:
      // ======================================================================
      GenericSpline2DSym ( const BSpline& xspline = BSpline() ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double operator () ( const double x , const double y ) const
      { return evaluate  ( x , y ) ; }
      /// get the value
      double evaluate    ( const double x , const double y ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get number of parameters
      std::size_t npars () const { return m_pars.size() ; }
      /// set k-parameter
      bool setPar       ( const unsigned int k , const double value )
      { return setParameter ( k , value ) ; }
      /// set k-parameter
      bool setParameter ( const unsigned int k , const double value ) ;
      /// set (i,j) paramter
      bool setPar       ( const unsigned short i ,
                          const unsigned short j , const double value )
      { return setPar ( index ( i , j ) , value ) ; }
      /// get the parameter value
      double  par       ( const unsigned int k ) const
      { return k < m_pars.size() ? m_pars[k] : 0.0 ; }
      /// get the parameter value
      double  parameter ( const unsigned int k ) const { return par ( k ) ; }
      /// get (i,j)-parameter
      double  par       ( const unsigned short i ,
                          const unsigned short j ) const
      { return par ( index ( i  , j ) ) ; }
      // ======================================================================
      // get all parameters
      const std::vector<double>& pars() const { return m_pars ; }
      // ======================================================================
    private :
      // ======================================================================
      ///  convert (l,m)-index into single k-index
      unsigned int index ( const unsigned short l ,
                           const unsigned short m ) const
      {
        const unsigned short n = m_spline.npars() ;
        return
          m >  l ? index ( m , l )  :
          l >= n ? -1               :  // NB !!
          1u * l * ( l + 1 ) / 2 + m ;
      }
      // ======================================================================
    public:
      // ======================================================================
      /// get lower/upper edges
      double         xmin   () const { return m_spline.xmin  () ; }
      double         xmax   () const { return m_spline.xmax  () ; }
      double         ymin   () const { return m_spline.xmin  () ; }
      double         ymax   () const { return m_spline.xmax  () ; }
      // spline order
      double         xorder () const { return m_spline.order () ; }
      double         yorder () const { return m_spline.order () ; }
      // inner knots
      double         xinner () const { return m_spline.inner () ; }
      double         yinner () const { return m_spline.inner () ; }
      // ======================================================================
    public: // few useful methdod
      // ======================================================================
      /// simple  manipulations with polynoms: shift it!
      GenericSpline2DSym& operator += ( const double a ) ;
      /// simple  manipulations with polynoms: shift it!
      GenericSpline2DSym& operator -= ( const double a ) ;
      /// simple  manipulations with polynoms: scale it!
      GenericSpline2DSym& operator *= ( const double a ) ;
      /// simple  manipulations with polynoms: scale it!
      GenericSpline2DSym& operator /= ( const double a ) ;
      // negate it
      GenericSpline2DSym  operator-()  const ;
      // ======================================================================
    public:  // for python
      // ======================================================================
      /// Sum of GenericSpline polynomial and a constant
      GenericSpline2DSym __add__   ( const double value ) const ;
      /// Sum of GenericSpline polynomial and a constant
      GenericSpline2DSym __radd__  ( const double value ) const ;
      /// Product of GenericSpline polynomial and a constant
      GenericSpline2DSym __mul__   ( const double value ) const ;
      /// Product of GenericSpline polynomial and a constant
      GenericSpline2DSym __rmul__  ( const double value ) const ;
      /// Subtract a constant from Benrstein polynomial
      GenericSpline2DSym __sub__   ( const double value ) const ;
      /// Constant minus GenericSpline polynomial
      GenericSpline2DSym __rsub__  ( const double value ) const ;
      /// Divide Benrstein polynomial by a constant
      GenericSpline2DSym __div__   ( const double value ) const ;
      /// Negate GenericSpline polynomial
      GenericSpline2DSym __neg__   () const ;
     // ======================================================================
    public: // generic integrals
      // ======================================================================
      /** get the integral over 2D-region
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integral   ( const double xlow , const double xhigh ,
                          const double ylow , const double yhigh ) const ;
      /** get the integral over X  for given Y
       *  @param y  (INPU) y-value
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateX
        ( const double y    ,
          const double xlow , const double xhigh ) const ;
      /** get the integral over Y  for given X
       *  @param x  (INPU) y-value
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateY
        ( const double x    ,
          const double ylow , const double yhigh ) const ;
      // ======================================================================
    public: // specific integrals
      // ======================================================================
      /** get the integral over 2D-region
       *  \f[ x_{min}<x<x_{max}, y_{min}<y<y_{max}\f]
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integral   () const ;
      /** get the integral over X  for given Y
       *  @param y  (INPU) y-value
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateX ( const double y ) const ;
      /** get the integral over Y  for given X
       *  @param x  (INPU) y-value
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateY ( const double x ) const ;
      // ======================================================================
    public: // ingredients
      // =====================================================================
      // get x-spline
      const  Gaudi::Math::BSpline& xspline () const { return m_spline ; }
      const  Gaudi::Math::BSpline& yspline () const { return m_spline ; }
      // ======================================================================
    private:
      // ======================================================================
      // make the calcualtions
      double calculate ( const std::vector<double>& fx ,
                         const std::vector<double>& fy ) const ;
      // ======================================================================
    private:
      // ======================================================================
      /// X-spline
      mutable Gaudi::Math::BSpline m_spline ; // x-spline
      /// parameters
      std::vector<double>  m_pars ;
      // ======================================================================
    };
    // ========================================================================
    ///  Spline plus      constant
    inline GenericSpline2DSym operator+( const GenericSpline2DSym& p , const double v )
    { return GenericSpline2DSym ( p ) += v ; } //  GenericSpline plus constant
    ///  Spline multiply  constant
    inline GenericSpline2DSym operator*( const GenericSpline2DSym& p , const double v )
    { return GenericSpline2DSym ( p ) *= v ; } //  GenericSpline plus constant
    ///  Spline minus constant
    inline GenericSpline2DSym operator-( const GenericSpline2DSym& p , const double v )
    { return GenericSpline2DSym ( p ) -= v ; } //  GenericSpline plus constant
    ///  Spline divide constant
    inline GenericSpline2DSym operator/( const GenericSpline2DSym& p , const double v )
    { return GenericSpline2DSym ( p ) /= v ; } //  GenericSpline plus constant
    ///  Spline plus  GenericSpline
    inline GenericSpline2DSym operator+( const double v , const GenericSpline2DSym& p ) { return p +   v  ; }
    ///  Spline times GenericSpline
    inline GenericSpline2DSym operator*( const double v , const GenericSpline2DSym& p ) { return p *   v  ; }
    ///  Constant minus GenericSpline
    inline GenericSpline2DSym operator-( const double v , const GenericSpline2DSym& p ) { return v + (-p) ; }
    // =========================================================================
    /** @class Spline2D
     *  Non-negative spline in 2D
     */
    class GAUDI_API  Spline2D
    {
      // ======================================================================
    public:
      // ======================================================================
      Spline2D ( const BSpline& xspline = BSpline() ,
                 const BSpline& yspline = BSpline() ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double operator () ( const double x , const double y ) const
      { return evaluate ( x , y ) ; }
      double evaluate   ( const double x , const double y ) const
      { return m_spline ( x , y ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get number of parameters
      std::size_t npars () const { return m_sphere.nPhi () ; }
      /// set k-parameter
      bool setPar       ( const unsigned int k , const double value )
      { return m_sphere.setPhase ( k , value ) ? updateSpline() : false ; }
      /// set k-parameter
      bool setParameter ( const unsigned int k , const double value )
      { return setPar   ( k , value ) ; }
      /// get the parameter value
      double  par       ( const unsigned int k ) const
      { return m_sphere.par ( k ) ; }
      /// get the parameter value
      double  parameter ( const unsigned int k ) const { return par ( k ) ; }
      // ======================================================================
      /// get all parameters (phases on sphere)
      const std::vector<double>& pars  () const { return m_sphere.pars () ; }
      /// get bernstein coefficients
      const std::vector<double>& bpars () const { return m_spline.pars () ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get lower/upper edges
      double         xmin   () const { return m_spline.xmin   () ; }
      double         xmax   () const { return m_spline.xmax   () ; }
      double         ymin   () const { return m_spline.ymin   () ; }
      double         ymax   () const { return m_spline.ymax   () ; }
      // spline order
      double         xorder () const { return m_spline.xorder () ; }
      double         yorder () const { return m_spline.yorder () ; }
      // inner knots
      double         xinner () const { return m_spline.xinner () ; }
      double         yinner () const { return m_spline.yinner () ; }
      // ======================================================================
    public: // generic integrals
      // ======================================================================
      /** get the integral over 2D-region
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integral   ( const double xlow , const double xhigh ,
                          const double ylow , const double yhigh ) const
      { return m_spline.integral ( xlow , xhigh , ylow , yhigh ) ; }
      /** get the integral over X  for given Y
       *  @param y  (INPU) y-value
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateX
        ( const double y     ,
          const double xlow  ,
          const double xhigh ) const
      { return m_spline.integrateX ( y , xlow , xhigh ) ; }
      /** get the integral over Y  for given X
       *  @param x  (INPU) y-value
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateY
        ( const double x     ,
          const double ylow  ,
          const double yhigh ) const
      { return m_spline.integrateY ( x , ylow , yhigh ) ; }
      // ======================================================================
    public: // specific integrals
      // ======================================================================
      /** get the integral over 2D-region
       *  \f[ x_{min}<x<x_{max}, y_{min}<y<y_{max}\f]
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integral   () const { return 1 ; }
      /** get the integral over X  for given Y
       *  @param y  (INPU) y-value
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateX ( const double y ) const
      { return m_spline.integrateX ( y  ) ; }
      /** get the integral over Y  for given X
       *  @param x  (INPU) y-value
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateY ( const double x ) const
      { return m_spline.integrateY ( x  ) ; }
      // ======================================================================
    public: // ingredients
      // =====================================================================
      /// get 2D-spline
      const  Gaudi::Math::GenericSpline2D&  spline () const { return m_spline ; }
      /// get 2D-spline
      const  Gaudi::Math::GenericSpline2D& bspline () const { return m_spline ; }
      /// get x-spline
      const  Gaudi::Math::BSpline&         xspline () const { return m_spline.xspline() ; }
      /// get y-spline
      const  Gaudi::Math::BSpline&         yspline () const { return m_spline.yspline() ; }
      /// get the parameter sphere
      const  Gaudi::Math::NSphere& sphere  () const { return m_sphere ; }
      // ======================================================================
    private:
      // ======================================================================
      /// update spline coefficients
      bool updateSpline () ;
      // ======================================================================
    private:
      // ======================================================================
      /// the 2D-spline itself
      Gaudi::Math::GenericSpline2D m_spline ;
      /// parameter sphere
      Gaudi::Math::NSphere         m_sphere ; // parameter sphere
      // ======================================================================
    };
    // =========================================================================
    /** @class Spline2DSym
     *  Non-negative symmetric spline in 2D
     */
    class GAUDI_API  Spline2DSym
    {
      // ======================================================================
    public:
      // ======================================================================
      Spline2DSym ( const BSpline& xspline = BSpline() ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double operator () ( const double x , const double y ) const
      { return evaluate  ( x , y ) ; }
      double evaluate    ( const double x , const double y ) const
      { return  m_spline ( x , y ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get number of parameters
      std::size_t npars () const { return m_sphere.nPhi () ; }
      /// set k-parameter
      bool setPar       ( const unsigned int k , const double value )
      { return m_sphere.setPhase ( k , value ) ? updateSpline() : false ; }
      /// set k-parameter
      bool setParameter ( const unsigned int k , const double value )
      { return setPar   ( k , value ) ; }
      /// get the parameter value
      double  par       ( const unsigned int k ) const
      { return m_sphere.par ( k ) ; }
      /// get the parameter value
      double  parameter ( const unsigned int k ) const { return par ( k ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get lower/upper edges
      double         xmin   () const { return m_spline.xmin   () ; }
      double         xmax   () const { return m_spline.xmax   () ; }
      double         ymin   () const { return m_spline.ymin   () ; }
      double         ymax   () const { return m_spline.ymax   () ; }
      // spline order
      double         xorder () const { return m_spline.xorder () ; }
      double         yorder () const { return m_spline.yorder () ; }
      // inner knots
      double         xinner () const { return m_spline.xinner () ; }
      double         yinner () const { return m_spline.yinner () ; }
      // ======================================================================
    public: // generic integration
      // ======================================================================
      /** get the integral over 2D-region
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integral   ( const double xlow , const double xhigh ,
                          const double ylow , const double yhigh ) const
      { return m_spline.integral ( xlow , xhigh , ylow , yhigh ) ; }
      /** get the integral over X  for given Y
       *  @param y  (INPU) y-value
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateX
        ( const double y     ,
          const double xlow  ,
          const double xhigh ) const
      { return m_spline.integrateX ( y , xlow , xhigh ) ; }
      /** get the integral over Y  for given X
       *  @param x  (INPU) y-value
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateY
        ( const double x     ,
          const double ylow  ,
          const double yhigh ) const
      { return m_spline.integrateY ( x , ylow , yhigh ) ; }
      // ======================================================================
    public: // specific integration
      // ======================================================================
      /** get the integral over 2D-region
       *  \f[ x_{min}< x < x_{max}, y_{min}<y<y_{max}\f]
       */
      double integral   () const { return  1 ; }
      /** get the integral over X  for given Y
       *  @param y  (INPU) y-value
       */
      double integrateX ( const double y ) const
      { return m_spline.integrateX ( y ) ; }
      /** get the integral over Y  for given X
       *  @param x  (INPU) y-value
       */
      double integrateY ( const double x ) const
      { return m_spline.integrateY ( x ) ; }
      // ======================================================================
    public: // ingeredients
      // =====================================================================
      // get x-spline
      const  Gaudi::Math::BSpline& xspline () const { return m_spline.xspline() ; }
      // get y-spline
      const  Gaudi::Math::BSpline& yspline () const { return m_spline.yspline() ; }
      // get 2D-spline
      const  Gaudi::Math::GenericSpline2DSym&  spline () const { return m_spline  ; }
      // get 2D-spline
      const  Gaudi::Math::GenericSpline2DSym& bspline () const { return m_spline  ; }
      /// get the parameter sphere
      const  Gaudi::Math::NSphere& sphere  () const { return m_sphere ; }
      // ======================================================================
    private:
      // ======================================================================
      ///  update spline  coefficients
      bool updateSpline () ;
      // ======================================================================
    private:
      // ======================================================================
      /// 2D-spline
      Gaudi::Math::GenericSpline2DSym m_spline ; // 2D-spline
      /// parameter sphere
      Gaudi::Math::NSphere            m_sphere  ; // parameter sphere
      // ======================================================================
    };
    // ========================================================================
  } //                                             end of namespace Gaudi::Math
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
namespace  LHCb
{
  // ==========================================================================
  namespace Math
  {
    // ========================================================================
    /// specialization: is B-spline close to zero?
    template <>
    struct Zero<Gaudi::Math::BSpline>
    {
    public:
      // ======================================================================
      // is B-spline almost to zero ?
      inline bool operator () ( const Gaudi::Math::BSpline& b ) const
      { return m_zero ( b.pars() ) ; }
    private:
      // ======================================================================
      /// the actual comparator
      Zero< std::vector<double> > m_zero ;
      // ======================================================================
    };
    // ========================================================================
    /// specialization: is B-spline small enough ?
    template <>
    struct Tiny<Gaudi::Math::BSpline>
    {
    public:
      // ======================================================================
      Tiny ( const double n ) : m_tiny ( std::abs ( n ) ) {}
      Tiny () = delete ;
      // is B-spline sufficiently small ?
      inline bool operator () ( const Gaudi::Math::BSpline& b ) const
      { return m_tiny ( b.norm() ) ; }
      // ======================================================================
    private:
      // ======================================================================
      Tiny<double> m_tiny ;
      // ======================================================================
    };
    // ========================================================================
    /** scale all coefficients with 2**i
     *  @param  b (INPUT) B-spline
     *  @param  i (INPUT) the scaling binary exponent
     *  @return the scaled B-spline
     */
    inline
    Gaudi::Math::BSpline
    ldexp ( const Gaudi::Math::BSpline& b ,
            const short                 i ) { return b.ldexp ( i ) ; }
    // ========================================================================
  } //                                              end of namespace LHCb::Math
  // ==========================================================================
}//                                                       end of namespace LHCb
// ============================================================================
namespace  Gaudi
{
  // ==========================================================================
  namespace Math
  {
    // ========================================================================
    class Bernstein ;
    // ========================================================================
    /** calculate the upper convex hull for Bernstein Polynomial
     *  \f$ B(x) \le U (x) \f$
     *  @param p  bernstein Polynomial
     *  @return   the spline object that represents upper convex hull
     */
    GAUDI_API
    Gaudi::Math::BSpline
    upper_convex_hull ( const Gaudi::Math::Bernstein& p ) ;
    // ========================================================================
    /** calculate the lower convex hull for Bernstein Polynomial
     *  \f$ B(x)  \ge  L (x) \f$
     *  @param p  bernstein Polynomial
     *  @return   the spline object that represents lower convex hull
     */
    GAUDI_API
    Gaudi::Math::BSpline
    lower_convex_hull ( const Gaudi::Math::Bernstein& p ) ;
    // ========================================================================
    /** get control polygon  for Bernstein polynomial
     *  @param p  bernstein Polynomial
     *  @return   the spline object that represents the control polygon
     */
    GAUDI_API
    Gaudi::Math::BSpline
    control_polygon   ( const Gaudi::Math::Bernstein& p ) ;
    // ========================================================================
    /** get control polygon  for Basic spline
     *  @param p  basic spline
     *  @return   the spline object that represents the control polygon
     */
    GAUDI_API
    Gaudi::Math::BSpline
    control_polygon   ( const Gaudi::Math::BSpline& p ) ;
    // ========================================================================
    /** get abscissas of crossing points of the control polygon with x-axis
     *  @param  b     (INPUT) bernstein polynomial
     *  @param formal (INPUT) get all formal crossing-points
     *  @reutrn abscissas of crossing points of the control  polygon
     */
    GAUDI_API
    std::vector<double>
    crossing_points  ( const Gaudi::Math::BSpline& b              ,
                       const bool                  formal = false ) ;
    // ========================================================================
    /** calculate the value of spline defined by vector of knot and vector of
     *  points using de-boor-cox algorithm
     *  @see https://en.wikipedia.org/wiki/De_Boor%27s_algorithm
     *  @param x     (INPUT) value of x
     *  @param order (INPUT) the order of spline
     *  @param knots (INPUT) the vector of knots
     *  @param pars  (INPUT) the vector of control points
     *  @return the valeu of b-spline at point x
     */
    GAUDI_API
    double deboor
    ( const double               x     ,
      const unsigned short       order ,
      const std::vector<double>& knots ,
      const std::vector<double>& pars  ) ;
    // ========================================================================
        /** insert new knot at position x in the spline, defined by
     *  knot vector knots, vector of control points pars and the order
     *  Boehm's algorithm is used
     *  @see W.Boehm, ``Inserting new knots into B-spline curves'',
     *       Computer-Aided Design, 12, no.4, (1980) 199
     *  @see http://dx.doi.org/10.1016/0010-4485(80)90154-2
     *  @see http://www.sciencedirect.com/science/article/pii/0010448580901542
     *  @param x     (INPUT)  position of new knot
     *  @param knots (UPDATE) vector of knots
     *  @param pars  (UPDATE) vector of control points
     *  @param order (INPUT)  degree/order of spline
     *  @param num   (INPUT)  insert new knot "num"-times
     *  @return multiplicity of inserted knot
     */
    GAUDI_API
    unsigned short
    boehm ( const double         x       ,
            std::vector<double>& knots   ,
            std::vector<double>& pars    ,
            const unsigned short order   ,
            const unsigned short num = 1 ) ;
    // ========================================================================
  } //                                             end of namespace Gaudi::Math
  // ==========================================================================
} //                                                     end of namespace Gaudi
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
      /** define parameters for the interpolation spline
       *  @param xy (INPUT)   vector of data
       *  @param bs (UPDATE) the spline
       *  @return status code
       */
      GAUDI_API
      StatusCode
      bspline
      ( std::vector< std::pair<double,double> >  xy ,
        Gaudi::Math::BSpline&                    bs ) ;
      // ======================================================================
      /** create the interpolation spline
       *  @param xy (INPUT)   vector of data
       *  @param bs (UPDATE) the spline
       *  @return status code
       */
      GAUDI_API
      StatusCode
      bspline
      ( const std::vector<double>& x  ,
        const std::vector<double>& y  ,
        Gaudi::Math::BSpline&      bs ) ;
      // ======================================================================
      // /** interpolate function <code>func</code> using  its value at x
      //  *  @param func  (INPPUT) the function
      //  *  @param x     (INPUT)  vector of points/abscissas
      //  *  @param order (INPUT)  the spline order
      //  *  @return B-spline object that interpolates the function
      //  */
      // template <class FUNCTION>
      // inline
      // Gaudi::Math::BSpline
      // spline_interpolate
      // ( FUNCTION                   func      ,
      //   const std::vector<double>& x         ,
      //   const unsigned short       order = 3 )
      // {
      //   /// get some reasonable knots from  proposed  vector of abscissas
      //   std::vector<double> knots = knots_from_abscissas ( x , order ) ;
      //   /// create the spline
      //   Gaudi::Math::BSpline result (  knots , order ) ;
      //   std::vector<double> f ( x.size() , 0  ) ;
      //   const unsigned short N = x.size() ;
      //   for ( unsigned short i = 0 ; i < N ; ++i ) { f[i] = func ( x[i] ) ; }
      //   //
      //   StatusCode sc = bspline ( x  , f , result ) ;
      //   if (  sc.isFailure() )
      //   { throw GaudiException ( "Can't interpolate" ,
      //                            "Gaudi::Math::spline_interpolate", sc ) ; }
      //   //
      //   return result ;
      // }
      // =====================================================================
      /** Create variation diminishing spline approximation for the given function
       *  @param func the function
       *  @param knots vector of knots
       *  @param order the order of approximation spline
       *  @return constructed VDS approximating spline
       */
      template <class FUNCTION>
      inline
      Gaudi::Math::BSpline
      spline_approximate
      ( FUNCTION                   func      ,
        const std::vector<double>& knots     ,
        const unsigned short       order = 3 )
      {
        // constuct spline
        Gaudi::Math::BSpline      bs ( knots , order ) ;
        // get greville abscissas
        const std::vector<double> ga ( bs.greville_abscissas() ) ;
        unsigned short i  = 0 ;
        // fill vector of parameters
        for ( double t : ga ) { bs.setPar ( i , func( t )  ); ++i ; }
        return bs ;
      }
      // ======================================================================
      /** define parameters for the interpolation spline
       *  @param func (INPUT) the function
       *  @param x    (INPUT) vector of abscissas
       *  @param bs   (UPDATE) the spline
       *  @return status code
       */
      template <class FUNCTION>
      inline
      StatusCode
      bspline
      ( FUNCTION                   func ,
        const std::vector<double>& x    ,
        Gaudi::Math::BSpline&      bs   )
      {
        std::vector< std::pair<double,double> >  xy (  x.size() ) ;
        std::transform
          ( x.begin() , x.end  () , xy.begin() ,
            [&func]( const double a ) { return std::pair{  a , func ( a ) } ; } ) ;
        return bspline (  xy , bs ) ;
      }
      // ======================================================================
    } //                            end of namespace Gaudi::Math::Interpolation
    // ========================================================================
  } //                                             end of namespace Gaudi::Math
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LHCBMATH_SPLINES_H
// ============================================================================
