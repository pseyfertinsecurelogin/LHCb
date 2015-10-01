// $Id:$ 
// ============================================================================
#ifndef LHCBMATH_BPLINE_H 
#define LHCBMATH_BPLINE_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <vector>
#include <functional>
// ============================================================================
// LHCbMath
// ============================================================================
#include "LHCbMath/NSphere.h"
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
    /** @class BSpline
     *  The basic spline   ("B-spline")
     *  @see http://en.wikipedia.org/wiki/B-spline
     *  @see http://link.springer.com/chapter/10.1007%2F978-3-0348-7692-6_6
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     */
    class GAUDI_API BSpline : public std::unary_function<double,double>
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
      BSpline ( const std::vector<double>& points    ,
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
      BSpline ( const std::vector<double>& points    ,
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
    public:
      // ======================================================================
      /// get the value
      double operator () ( const double x ) const ;
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
      unsigned short order () const { return m_order ; }
      // number of inner knots 
      unsigned short inner () const { return m_inner ; }
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
      /// get minimal value of the function on (xmin,xmax) interval 
      double fun_min       () const ;
      /// get maximal value of the function on (xmin,xmax) interval 
      double fun_max       () const ;
      // ======================================================================
    public:
      // ======================================================================
      /// positive      function ?
      bool   positive      () const ;
      /// negative      function ?
      bool   negative      () const ; 
      /// non-positive  function ?
      bool   nonpositive   () const ; 
      /// non-negative  function ?
      bool   nonnegative   () const ; 
      ///  has roots at [xmin,xmax] ? 
      bool   hasroots      () const { return   !positive () &&   !negative () ; }
      ///  keep sign at [xmin,xmax] ? 
      bool   keepsign      () const { return nonpositive () || nonnegative () ; }
      /// is it a decreasing function?
      bool   decreasing    () const ;
      /// is it a increasing function?
      bool   increasing    () const ;
      /// is it a monothonical function?
      bool   monothonic    () const { return increasing() || decreasing() ; }
      /// is it a constant function?
      bool   constant      () const ;
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
    public: // simple  manipulations with bernstein polynoms 
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
    class GAUDI_API PositiveSpline : public std::unary_function<double,double>
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
      virtual ~PositiveSpline() ;
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
      /// get minimal value of the function on (xmin,xmax) interval 
      double fun_min       () const { return m_bspline.fun_min () ; }
      /// get maximal value of the function on (xmin,xmax) interval 
      double fun_max       () const { return m_bspline.fun_max () ; }
      // ======================================================================
      /// positive      function ?
      bool    positive     () const { return m_bspline.positive()    ; }
      /// negative      function ?
      bool   negative      () const { return false ; }      
      /// non-positive  function ?
      bool   nonpositive   () const { return false ; }      
      /// non-negative  function ?
      bool   nonnegative   () const { return true  ; }
      ///  keep sign at [xmin,xmax] ? 
      bool   keepsign      () const { return true ; }
      ///  has roots at [xmin,xmax] ? 
      bool   hasroots      () const { return !positive () ; }
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
      /// destructor  
      virtual ~ConvexOnlySpline() ;
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
      virtual bool updateCoefficients  () ;
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
      /// destructor  
      virtual ~MonothonicSpline() ;
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
      virtual bool updateCoefficients  () ;
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
      /// destructor  
      virtual ~ConvexSpline() ;
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
      virtual bool updateCoefficients  () ;
      // ======================================================================
    protected:
      // ======================================================================
      /// convex function?
      bool m_convex ;  // convex function?
      // ======================================================================
    } ;
    // =========================================================================
    /** @class Spline2D
     *  Non-negative spline in 2D 
     */
    class GAUDI_API  Spline2D : public std::binary_function<double,double,double>
    {
      // ======================================================================
    public:
      // ======================================================================
      Spline2D ( const BSpline&        xspline , 
                 const BSpline&        yspline ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double operator () ( const double x , const double y ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get number of parameters
      std::size_t npars () const { return m_sphere.nPhi () ; }
      /// set k-parameter
      bool setPar       ( const unsigned int k , const double value ) 
      { return m_sphere.setPhase ( k , value ) ; }
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
      /// get the parameter sphere  
      const  Gaudi::Math::NSphere& sphere  () const { return m_sphere ; }  
      // ======================================================================
    private:
      // ======================================================================
      /// X-spline 
      mutable Gaudi::Math::BSpline m_xspline ; // x-spline 
      /// Y-spline 
      mutable Gaudi::Math::BSpline m_yspline ; // y-spline 
      /// parameter sphere 
      Gaudi::Math::NSphere m_sphere  ; // parameter sphere
      // ======================================================================
    private:
      // ======================================================================
      mutable   std::vector<double>  m_xcache ; // x-cache 
      mutable   std::vector<double>  m_ycache ; // y-cache
      // ======================================================================
    };
    // =========================================================================
    /** @class Spline2DSym
     *  Non-negative symmetric spline in 2D 
     */
    class GAUDI_API  Spline2DSym : public std::binary_function<double,double,double>
    {
      // ======================================================================
    public:
      // ======================================================================
      Spline2DSym ( const BSpline&        xspline ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double operator () ( const double x , const double y ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get number of parameters
      std::size_t npars () const { return m_sphere.nPhi () ; }
      /// set k-parameter
      bool setPar       ( const unsigned int k , const double value ) 
      { return m_sphere.setPhase ( k , value ) ; }
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
      double         xmin   () const { return m_spline.xmin  () ; }
      double         xmax   () const { return m_spline.xmax  () ; }
      double         ymin   () const { return xmin  () ; }
      double         ymax   () const { return xmax  () ; }
      // spline order 
      double         xorder () const { return m_spline.order () ; }
      double         yorder () const { return xorder () ; }
      // inner knots 
      double         xinner () const { return m_spline.inner () ; }
      double         yinner () const { return xinner () ; }
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
    public: // specific integration 
      // ======================================================================
      /** get the integral over 2D-region 
       *  \f[ x_{min}< x < x_{max}, y_{min}<y<y_{max}\f]
       */
      double integral   () const ; 
      /** get the integral over X  for given Y
       *  @param y  (INPU) y-value 
       */  
      double integrateX ( const double y ) const ;
      /** get the integral over Y  for given X
       *  @param x  (INPU) y-value 
       */  
      double integrateY ( const double x ) const ;
      // ======================================================================
    public: // ingeredients 
      // =====================================================================
      // get x-spline 
      const  Gaudi::Math::BSpline& xspline () const { return m_spline   ; }
      const  Gaudi::Math::BSpline& yspline () const { return xspline () ; }
      /// get the parameter sphere  
      const  Gaudi::Math::NSphere& sphere  () const { return m_sphere ; }  
      // ======================================================================
    private:
      // ======================================================================
      /// X-spline 
      mutable Gaudi::Math::BSpline m_spline ; // x-spline 
      /// parameter sphere 
      Gaudi::Math::NSphere         m_sphere  ; // parameter sphere
      // ======================================================================
    private:
      // ======================================================================
      mutable   std::vector<double>  m_xcache ; // x-cache 
      mutable   std::vector<double>  m_ycache ; // y-cache
      // ======================================================================
    }; 
    // ========================================================================
  } //                                             end of namespace Gaudi::Math
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // LHCBMATH_SPLINES_H
// ============================================================================
