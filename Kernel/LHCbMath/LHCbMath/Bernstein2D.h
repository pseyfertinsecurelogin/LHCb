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
#ifndef LHCBMATH_BERNSTEIN2D_H 
#define LHCBMATH_BERNSTEIN2D_H 1
// ============================================================================
// Include files
// ============================================================================
// LHCbMath
// ============================================================================
#include "LHCbMath/LHCbMath.h"
#include "LHCbMath/BernsteinPoly.h"
#include "LHCbMath/NSphere.h"
// ============================================================================
/** @file LHCbMath/Bernstein2D.h
 *  Collection of files related to 2D-moodels, based on Bernstein polynomials 
 *  @author Vanya Belyaev
 *  @date   2017-11-18
 */
// ============================================================================
namespace Gaudi
{
  // ==========================================================================
  namespace Math
  {
    // ========================================================================
    /** @class Bernstein2D
     *  Generic 2D-polynomial of order nX*nY, defined as 
     *  \f[ P(x,y) = \sum_{i,j} a_{ij}B^{n_x}_i(x) B^{n_y}_j(y)\f] 
     */
    class GAUDI_API Bernstein2D
    {
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the order
      Bernstein2D ( const unsigned short       nX    =  1 ,
                    const unsigned short       nY    =  1 ,
                    const double               xmin  =  0 ,
                    const double               xmax  =  1 ,
                    const double               ymin  =  0 ,
                    const double               ymax  =  1 ) ;
      // ======================================================================
      /// copy 
      Bernstein2D ( const Bernstein2D&  right ) =   default ;
      /// move 
      Bernstein2D (       Bernstein2D&& right ) ;
      // ======================================================================
    public:
      // ======================================================================
      ///  get the  value 
      double evaluate    ( const double x , const double y ) const ;
      /// get the value
      double operator () ( const double x , const double y ) const 
      { return evaluate ( x , y ) ; }
      // ======================================================================
    public: // setters
      // ======================================================================
      /// set k-parameter
      bool setPar       ( const unsigned int   k     ,
                          const double         value ) ;
      /// set k-parameter
      bool setParameter ( const unsigned int   k     ,
                          const double         value )
      { return ( k < m_pars.size() ) && setPar ( k , value ) ; }
      /// set (l,m)-parameter
      bool setPar       ( const unsigned short l     ,
                          const unsigned short m     ,
                          const double         value ) 
      {
        const unsigned int k = index ( l , m ) ;
        return ( k < m_pars.size() ) && setPar ( k , value ) ;
      }
      /// set (l,m)-parameter
      bool setParameter ( const unsigned short l     ,
                          const unsigned short m     ,
                          const double         value )
      { return setPar   ( l , m  , value ) ; }
      // ======================================================================
    public: // getters
      // ======================================================================
      /// get (l,m)-parameter
      double  par       ( const unsigned short l ,
                          const unsigned short m ) const 
      { return par ( index ( l , m ) ) ; }      
      /// get (l,m)-parameter
      double  parameter ( const unsigned short l ,
                          const unsigned short m ) const { return par (  l , m  ) ; }
      /// get k-parameter
      double  par       ( const unsigned int k ) const
      { return k < m_pars.size() ? m_pars[k] : 0.0 ; }
      /// get k-parameter
      double  parameter ( const unsigned int k ) const { return par ( k ) ; }
      /// get all parameters at once
      const std::vector<double>& pars() const { return m_pars ; }
      // ======================================================================
    private:
      // ======================================================================
      ///  convert (l,m)-index into single k-index
      unsigned int index ( const unsigned short l , 
                           const unsigned short m ) const 
      {
        return
          l > m_nx ? -1 :
          m > m_ny ? -1 :
          1u * l * ( m_ny + 1 ) + m ;  
      }
      // ======================================================================
    public:
      // ======================================================================
      /// get the actual number of parameters
      std::size_t npars () const { return m_pars.size() ; }
      /// get lower edge
      double xmin () const { return m_xmin ; }
      /// get upper edge
      double xmax () const { return m_xmax ; }
      /// get lower edge
      double ymin () const { return m_ymin ; }
      /// get upper edge
      double ymax () const { return m_ymax ; }
      /// get the polynomial order (X)
      unsigned short nX () const { return m_nx ; }
      /// get the polynomial order (Y)
      unsigned short nY () const { return m_ny ; }
      // ======================================================================
    public:  // transformations
      // ======================================================================
      double x  ( const double tx ) const
      { return xmin ()  + ( xmax () - xmin () ) * tx ; }
      double y  ( const double ty ) const
      { return ymin ()  + ( ymax () - ymin () ) * ty ; }
      double tx ( const double x ) const
      { return  ( x - xmin () ) / ( xmax () - xmin () )      ; }
      double ty ( const double y ) const
      { return  ( y - ymin () ) / ( ymax () - ymin () )      ; }
      // ======================================================================
    public: // general integration
      // ======================================================================
      /** get the integral over 2D-region
       *  \f[ \int_{x_{low}}^{x_{high}}\int_{y_{low}}^{y_{high}}
       *      \mathcal{B}(x,y) \mathrm{d}x\mathrm{d}y \f]
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integral   ( const double xlow , const double xhigh ,
                          const double ylow , const double yhigh ) const ;
      /** integral over x-dimension
       *  \f[ \int_{x_{low}}^{x_{high}} \mathcal{B}(x,y) \mathrm{d}x\f]
       *  @param x     variable
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateX ( const double y    ,
                          const double xlow , const double xhigh ) const ;
      /** integral over y-dimension
       *  \f[ \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y) \mathrm{d}y\f]
       *  @param y     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateY ( const double x    ,
                          const double ylow , const double yhigh ) const ;
      // ======================================================================
    public: // special cases
      // ======================================================================
      /** get the integral over 2D-region
       *  \f[  x_min < x < x_max, y_min< y< y_max\f]
       */
      double integral   () const ;
      /** integral over x-dimension
       *  \f[ \int_{x_{min}}^{x_{max}} \mathcal{B}(x,y) \mathrm{d}x\f]
       *  @param x     variable
       */
      double integrateX ( const double y    ) const ;
      /** integral over y-dimension
       *  \f[ \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y) \mathrm{d}y\f]
       *  @param y     variable
       */
      double integrateY ( const double x    ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// simple  manipulations with polynoms: shift it!
      Bernstein2D& operator += ( const double a ) ;
      /// simple  manipulations with polynoms: shift it!
      Bernstein2D& operator -= ( const double a ) ;
      /// simple  manipulations with polynoms: scale it!
      Bernstein2D& operator *= ( const double a ) ;
      /// simple  manipulations with polynoms: scale it!
      Bernstein2D& operator /= ( const double a ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// negate it!
      Bernstein2D  operator-() const ;
      // ======================================================================
    public:
      // ======================================================================
      /// Sum of Bernstein polynomial and a constant
      Bernstein2D __add__   ( const double value ) const ;
      /// Sum of Bernstein polynomial and a constant
      Bernstein2D __radd__  ( const double value ) const ;
      /// Product of Bernstein polynomial and a constant
      Bernstein2D __mul__   ( const double value ) const ;
      /// Product of Bernstein polynomial and a constant
      Bernstein2D __rmul__  ( const double value ) const ;
      /// Subtract a constant from Benrstein polynomial
      Bernstein2D __sub__   ( const double value ) const ;
      /// Constant minus Bernstein polynomial
      Bernstein2D __rsub__  ( const double value ) const ;
      /// Divide Benrstein polynomial by a constant
      Bernstein2D __div__   ( const double value ) const ;
      /// Negate Bernstein polynomial
      Bernstein2D __neg__   () const ;
      // ======================================================================
    public: // few helper functions to expose internals
      // ======================================================================
      /// evaluate the basic polynomials
      double basicX ( const unsigned short i , const double         x ) const
      { return ( i > m_nx || x < m_xmin || x < m_xmax ) ? 0.0 : m_bx[i](x) ; }
      /// evaluate the basic polynomials
      double basicY ( const unsigned short i , const double         y ) const
      { return ( i > m_ny || y < m_ymin || y < m_ymax ) ? 0.0 : m_by[i](y) ; }
      /// expose some internals
      const Bernstein& basicX ( const unsigned short i ) const { return m_bx[i] ; }
      /// expose some internals
      const Bernstein& basicY ( const unsigned short i ) const { return m_by[i] ; }
      // ======================================================================
    public:
      // ======================================================================
      /// swap  two 2D-polynomials 
      void swap ( Bernstein2D&  right ) ;
      // ======================================================================
    private: // helper functions to make the calculations
      // ======================================================================
      /// helper function to make calculations
      double calculate ( const std::vector<double>& fx , 
                         const std::vector<double>& fy ) const ;
      // ======================================================================
    private:
      // ======================================================================
      // polynom order in x-dimension
      unsigned short m_nx ; // polynom order in x-dimension
      // polynom order in y-dimension
      unsigned short m_ny ; // polynom order in y-dimension
      /// the list of parameters
      std::vector<double>  m_pars ;                // the list of parameters
      /// the left edge of interval
      double m_xmin  ;                             // the left edge of interval
      /// the right edge of interval
      double m_xmax  ;                             // the right edge of interval
      /// the left edge of interval
      double m_ymin  ;                             // the left edge of interval
      /// the right edge of interval
      double m_ymax  ;                             // the right edge of interval
      // ======================================================================
    private:
      // ======================================================================
      ///  vectors of basic  Bernstein polynomials
      typedef std::vector<Bernstein>  VB ;
      ///  vector  of basic  Bernstein polynomials
      VB m_bx ; //  vector  of basic  Bernetin polynomials
      ///  vector  of basic  Bernstein polynomials
      VB m_by ; //  vector  of basic  Bernetin polynomials
      // ======================================================================
    } ;
    // ========================================================================
    ///  Bernstein plus      constant
    inline Bernstein2D operator+( const Bernstein2D& p , const double v )
    { return Bernstein2D ( p ) += v ; } //  Bernstein plus constant
    ///  Bernstein multiply  constant
    inline Bernstein2D operator*( const Bernstein2D& p , const double v )
    { return Bernstein2D ( p ) *= v ; } //  Bernstein plus constant
    ///  Bernstein minus constant
    inline Bernstein2D operator-( const Bernstein2D& p , const double v )
    { return Bernstein2D ( p ) -= v ; } //  Bernstein plus constant
    ///  Bernstein divide constant
    inline Bernstein2D operator/( const Bernstein2D& p , const double v )
    { return Bernstein2D ( p ) /= v ; } //  Bernstein plus constant
    ///  Constant plus  Bernstein
    inline Bernstein2D operator+( const double v , const Bernstein2D& p ) { return p +   v  ; }
    ///  Constant times Bernstein
    inline Bernstein2D operator*( const double v , const Bernstein2D& p ) { return p *   v  ; }
    ///  Constant minus Bernstein
    inline Bernstein2D operator-( const double v , const Bernstein2D& p ) { return v + (-p) ; }
    // ========================================================================
    /// swap  two 2D-polynomials 
    inline void  swap ( Bernstein2D& a  , Bernstein2D& b ) { a.swap ( b ) ; }
    // ========================================================================
    /** @class Positive2D
     *  The 2D-polynomial of order Nx*Ny, that is constrained 
     *  to be non-negative over the  defined range      
     *  \f[  P(x,y) = \sum_{i,j} a_{ij}B^{n_x}_i(x) B^{n_y}_j(y)\f] 
     *  where all coefficients \f$a_{ij}\f$ are non-negative and 
     *  \f$ \sum_{i,j} a_{ij}=1 \f$ 
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     */
    class GAUDI_API Positive2D
    {
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the order
      Positive2D ( const unsigned short       Nx    =  1 ,
                   const unsigned short       Ny    =  1 ,
                   const double               xmin  =  0 ,
                   const double               xmax  =  1 ,
                   const double               ymin  =  0 ,
                   const double               ymax  =  1 ) ;
      // ======================================================================
      /// copy 
      Positive2D ( const Positive2D&  right ) = default ;
      /// move 
      Positive2D (       Positive2D&& right ) ;
      // ======================================================================
    public:
      // ======================================================================
      ///  get the  value 
      double evaluate    ( const double x , const double y ) const 
      { return m_bernstein ( x , y ) ; }        
      /// get the value
      double operator () ( const double x , const double y ) const
      { return evaluate    ( x , y ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get number of parameters
      std::size_t npars () const { return m_sphere.nPhi () ; }
      /// set k-parameter
      bool setPar       ( const unsigned int k , const double value ) ;
      /// set k-parameter
      bool setParameter ( const unsigned int k , const double value )
      { return setPar   ( k , value ) ; }
      /// get the parameter value
      double  par       ( const unsigned int k ) const 
      { return m_sphere.phase ( k ) ; }
      /// get the parameter value
      double  parameter ( const unsigned int k ) const { return par ( k ) ; }
      // ======================================================================
      /// get all parameters (phases on sphere)
      const std::vector<double>& pars  () const { return m_sphere   .pars () ; }
      /// get bernstein coefficients
      const std::vector<double>& bpars () const { return m_bernstein.pars () ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get lower/upper edges
      double         xmin () const { return m_bernstein.xmin () ; }
      double         xmax () const { return m_bernstein.xmax () ; }
      double         ymin () const { return m_bernstein.ymin () ; }
      double         ymax () const { return m_bernstein.ymax () ; }
      // polynom order
      unsigned short nX   () const { return m_bernstein.nX   () ; }
      unsigned short nY   () const { return m_bernstein.nY   () ; }
      // ======================================================================
    public:
      // ======================================================================
      // transform variables
      double tx ( const double  x ) const { return m_bernstein.tx (  x ) ; }
      double ty ( const double  y ) const { return m_bernstein.ty (  y ) ; }
      double  x ( const double tx ) const { return m_bernstein. x ( tx ) ; }
      double  y ( const double ty ) const { return m_bernstein. y ( ty ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /** get the integral over 2D-region
       *  \f[ \int_{x_low}^{x_high}\int_{y_low}^{y_high} \mathcal{B}(x,y) \mathrm{d}x\mathrm{d}y\f]
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integral   ( const double xlow , const double xhigh ,
                          const double ylow , const double yhigh ) const ;
      /** integral over x-dimension
       *  \f[ \int_{x_{low}}^{x_{high}} \mathcal{B}(x,y) \mathrm{d}x\f]
       *  @param x     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateX ( const double y    ,
                          const double xlow , const double xhigh ) const
      { return m_bernstein.integrateX ( y , xlow , xhigh ) ; }
      /** integral over y-dimension
       *  \f[ \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y) \mathrm{d}y\f]
       *  @param x     variable
       *  @param ylow  low  edge in x
       *  @param yhigh high edge in x
       */
      double integrateY ( const double x    ,
                          const double ylow , const double yhigh ) const
      { return m_bernstein.integrateY ( x , ylow , yhigh ) ; }
      // ======================================================================
    public: // specific
      // ======================================================================
      /** get the integral over 2D-region
       *  \f[ \int_{x_{min}}^{x_{max}}\int_{y_{min}}^{y_{max}}
       *        \mathcal{B}(x,y) \mathrm{d}x\mathrm{d}y\f]
       */
      double integral   () const ;
      /** integral over x-dimension
       *  \f[ \int_{x_{min}}^{x_{max}} \mathcal{B}(x,y) \mathrm{d}x\f]
       *  @param x     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateX ( const double y    ) const
      { return m_bernstein.integrateX ( y ) ; }
      /** integral over y-dimension
       *  \f[ \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y) \mathrm{d}y\f]
       *  @param x     variable
       *  @param ylow  low  edge in x
       *  @param yhigh high edge in x
       */
      double integrateY ( const double x    ) const
      { return m_bernstein.integrateY ( x ) ; }
      // ======================================================================
    public: // ingeredients
      // =====================================================================
      // get the bernstein polinomial in 2D
      const  Gaudi::Math::Bernstein2D& bernstein () const
      { return m_bernstein ; }
      /// get the parameter sphere
      const  Gaudi::Math::NSphere&     sphere    () const
      { return m_sphere ; }
      // ======================================================================
    public:
      // ======================================================================
      /// swap  two 2D-polynomials 
      void swap ( Positive2D&  right ) ;
      // ======================================================================
    private:
      // ======================================================================
      /// update bernstein coefficients
      bool updateBernstein () ;
      // ======================================================================
    private:
      // ======================================================================
      /// the actual bernstein polynomial
      Gaudi::Math::Bernstein2D m_bernstein ; // the actual bernstein polynomial
      /// the external parameter sphere
      Gaudi::Math::NSphere     m_sphere    ;
      // ======================================================================
    } ;
    // ========================================================================
    /// swap  two 2D-polynomials 
    inline void  swap ( Positive2D& a  , Positive2D& b ) { a.swap ( b ) ; }
    // ========================================================================
    /** @class Bernstein2DSym
     *  The symmetric Bernstein's polynomial of order N*N
     */
    class GAUDI_API Bernstein2DSym
    {
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the order
      Bernstein2DSym ( const unsigned short       n     =  1 ,
                       const double               xmin  =  0 ,
                       const double               xmax  =  1 ) ;
      // ======================================================================
      /// copy 
      Bernstein2DSym ( const Bernstein2DSym&  right ) =   default ;
      /// move 
      Bernstein2DSym (       Bernstein2DSym&& right ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double evaluate    ( const double x , const double y ) const ;
      /// get the value
      double operator () ( const double x , const double y ) const 
      { return evaluate ( x , y ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get number of parameters
      std::size_t npars () const { return m_pars.size() ; }
      /// set k-parameter
      bool setPar       ( const unsigned int   k     ,
                          const double         value ) ;
      /// set k-parameter
      bool setParameter ( const unsigned int   k     ,
                          const double         value )
      { return ( k < m_pars.size() ) && setPar ( k , value ) ; }
      /// set (l,m)-parameter
      bool setPar       ( const unsigned short l     ,
                          const unsigned short m     ,
                          const double         value ) 
      {
        const  unsigned int k = index ( l , m ) ;
        return ( k < m_pars.size() ) && setPar ( k , value ) ;
      }
      /// set (l,m)-parameter
      bool setParameter ( const unsigned short l     ,
                          const unsigned short m     ,
                          const double         value )
      { return setPar   ( l , m  , value ) ; }
      /// get (l,m)-parameter
      double  par       ( const unsigned short l ,
                          const unsigned short m ) const 
      { return par ( index ( l , m ) ) ; }
      /// get (l,m)-parameter value
      double  parameter ( const unsigned short l ,
                          const unsigned short m ) const { return par (  l , m  ) ; }
      /// get k-parameter
      double  par       ( const unsigned int   k ) const
      { return k < m_pars.size() ? m_pars [k] : 0.0 ; }
      /// get k-parameter
      double  parameter ( const unsigned int   k ) const { return par ( k ) ; }
      /// get all parameters at once
      const std::vector<double>& pars() const { return m_pars ; }
      // ======================================================================
    private:
      // ======================================================================
      ///  convert (l,m)-index into single k-index
      unsigned int index ( const unsigned short l , 
                           const unsigned short m ) const 
      {
        return
          m > l   ? index ( m , l )  :
          l > m_n ? -1               :  // NB !!
          1u * l * ( l + 1 ) / 2 + m ;
      }
      // ======================================================================
    public:
      // ======================================================================
      /// get lower edge
      double xmin () const { return m_xmin    ; }
      /// get upper edge
      double xmax () const { return m_xmax    ; }
      /// get lower edge
      double ymin () const { return   xmin () ; }
      /// get upper edge
      double ymax () const { return   xmax () ; }
      // ======================================================================
      unsigned short n  () const { return m_n  ; }
      unsigned short nX () const { return n () ; }
      unsigned short nY () const { return n () ; }
      // ======================================================================
    public:
      // ======================================================================
      double x  ( const double tx ) const
      { return xmin ()  + ( xmax () - xmin () ) * tx ; }
      double y  ( const double ty ) const
      { return ymin ()  + ( ymax () - ymin () ) * ty ; }
      double tx ( const double x ) const
      { return  ( x - xmin () ) / ( xmax () - xmin () ) ; }
      double ty ( const double y ) const
      { return  ( y - ymin () ) / ( ymax () - ymin () ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /// simple  manipulations with polynoms: shift it!
      Bernstein2DSym& operator += ( const double a ) ;
      /// simple  manipulations with polynoms: shift it!
      Bernstein2DSym& operator -= ( const double a ) ;
      /// simple  manipulations with polynoms: scale it!
      Bernstein2DSym& operator *= ( const double a ) ;
      /// simple  manipulations with polynoms: scale it!
      Bernstein2DSym& operator /= ( const double a ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// negate it!
      Bernstein2DSym  operator-() const ;
      // ======================================================================
    public:
      // ======================================================================
      /// Sum of Bernstein polynomial and a constant
      Bernstein2DSym __add__   ( const double value ) const ;
      /// Sum of Bernstein polynomial and a constant
      Bernstein2DSym __radd__  ( const double value ) const ;
      /// Product of Bernstein polynomial and a constant
      Bernstein2DSym __mul__   ( const double value ) const ;
      /// Product of Bernstein polynomial and a constant
      Bernstein2DSym __rmul__  ( const double value ) const ;
      /// Subtract a constant from Benrstein polynomial
      Bernstein2DSym __sub__   ( const double value ) const ;
      /// Constant minus Bernstein polynomial
      Bernstein2DSym __rsub__  ( const double value ) const ;
      /// Divide Benrstein polynomial by a constant
      Bernstein2DSym __div__   ( const double value ) const ;
      /// Negate Bernstein polynomial
      Bernstein2DSym __neg__   () const ;
      // ======================================================================
    public: // generic integrals
      // ======================================================================
      /** get the integral over 2D-region
       *  \f[ \int_{x_{low}}^{x_{high}}\int_{y_{low}}^{y_{high}}
       *  \mathcal{B}(x,y) \mathrm{d}x\mathrm{d}y\f]
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integral   ( const double xlow , const double xhigh ,
                          const double ylow , const double yhigh ) const ;
      /** integral over x-dimension
       *  \f[ \int_{x_{low}}^{x_{high}} \mathcal{B}(x,y) \mathrm{d}x\f]
       *  @param x     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateX ( const double y    ,
                          const double xlow , const double xhigh ) const ;
      /** integral over y-dimension
       *  \f[ \int_{y_{low}}^{x_{high}} \mathcal{B}(x,y) \mathrm{d}y\f]
       *  @param y     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateY ( const double x    ,
                          const double ylow , const double yhigh ) const ;
      // ======================================================================
    public: // specific integrals
      // ======================================================================
      /** get the integral over 2D-region
       *  \f[ \int_{x_{min}}^{x_{max}}\int_{y_{min}}^{y_{max}}
       *  \mathcal{B}(x,y) \mathrm{d}x\mathrm{d}y\f]
       */
      double integral   () const ;
      /** integral over x-dimension
       *  \f[ \int_{x_{min}}^{x_{max}} \mathcal{B}(x,y) \mathrm{d}x\f]
       *  @param x     variable
       */
      double integrateX ( const double y ) const ;
      /** integral over y-dimension
       *  \f[ \int_{y_{min}}^{x_{max}} \mathcal{B}(x,y) \mathrm{d}y\f]
       *  @param y     variable
       */
      double integrateY ( const double x ) const ;
      // ======================================================================
    public: // few helper functions to expose internals
      // ======================================================================
      /// evaluate the basic polynomials
      double basic  ( const unsigned short i , const double         x ) const
      { return ( i > m_n || x < m_xmin || x < m_xmax ) ? 0.0 : m_b[i](x) ; }
      /// expose some internals
      const Bernstein& basic ( const unsigned short i ) const { return m_b[i] ; }
      // ======================================================================
    public:
      // ======================================================================
      /// swap  two 2D-polynomials 
      void swap ( Bernstein2DSym&  right ) ;
      // ======================================================================
    private: // helper functions to make the calculations
      // ======================================================================
      /// helper function to make calculations
      double calculate ( const std::vector<double>& fx , 
                         const std::vector<double>& fy ) const ;
      // ======================================================================
    private:
      // ======================================================================
      // polynom order
      unsigned short m_n  ; // polynom order in x-dimension
      /// the list of parameters
      std::vector<double>  m_pars ;                // the list of parameters
      /// the left edge of interval
      double m_xmin  ;                             // the left edge of interval
      /// the right edge of interval
      double m_xmax  ;                             // the right edge of interval
      // ======================================================================
    private:
      // ======================================================================
      ///  vectors of basic  Bernstein polynomials
      typedef std::vector<Bernstein>  VB ;
      ///  vector  of basic  Bernetin polynomials
      VB m_b  ; //  vector  of basic  Bernstein polynomials
      // ======================================================================
    } ;
    // ========================================================================
    ///  Bernstein plus      constant
    inline Bernstein2DSym operator+( const Bernstein2DSym& p , const double v )
    { return Bernstein2DSym ( p ) += v ; } //  Bernstein plus constant
    ///  Bernstein multiply  constant
    inline Bernstein2DSym operator*( const Bernstein2DSym& p , const double v )
    { return Bernstein2DSym ( p ) *= v ; } //  Bernstein plus constant
    ///  Bernstein minus constant
    inline Bernstein2DSym operator-( const Bernstein2DSym& p , const double v )
    { return Bernstein2DSym ( p ) -= v ; } //  Bernstein plus constant
    ///  Bernstein divide constant
    inline Bernstein2DSym operator/( const Bernstein2DSym& p , const double v )
    { return Bernstein2DSym ( p ) /= v ; } //  Bernstein plus constant
    ///  Constant plus  Bernstein
    inline Bernstein2DSym operator+( const double v , const Bernstein2DSym& p ) { return p +   v  ; }
    ///  Constant times Bernstein
    inline Bernstein2DSym operator*( const double v , const Bernstein2DSym& p ) { return p *   v  ; }
    ///  Constant minus Bernstein
    inline Bernstein2DSym operator-( const double v , const Bernstein2DSym& p ) { return v + (-p) ; }
    // ========================================================================
    /// swap  two 2D-polynomials 
    inline void  swap ( Bernstein2DSym& a  , Bernstein2DSym& b ) { a.swap ( b ) ; }
    // ========================================================================
    /** @class Positive2DSym
     *  The "positive" symmetrical polynomial of order Nx*Ny
     *  Actually it is a sum of basic bernstein 2D-polynomials with
     *  non-negative coefficients
     */
    class GAUDI_API Positive2DSym
    {
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the order
      Positive2DSym ( const unsigned short       Nx    =  1 ,
                      const double               xmin  =  0 ,
                      const double               xmax  =  1 ) ;
      // ======================================================================
      /// copy 
      Positive2DSym ( const Positive2DSym&  right ) =   default ;
      /// move 
      Positive2DSym (       Positive2DSym&& right ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double evaluate   ( const double x , const double y ) const ;
      /// get the value
      double operator () ( const double x , const double y ) const 
      { return evaluate ( x , y ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get number of parameters
      std::size_t npars () const { return m_sphere.nPhi () ; }
      /// set k-parameter
      bool setPar       ( const unsigned int k , const double value ) ;
      /// set k-parameter
      bool setParameter ( const unsigned int k , const double value )
      { return setPar   ( k , value ) ; }
      /// get the parameter value
      double  par       ( const unsigned int k ) const 
      { return m_sphere.phase ( k ) ; }
      /// get the parameter value
      double  parameter ( const unsigned int k ) const { return par ( k ) ; }
      // ======================================================================
      /// get all parameters (phases on sphere)
      const std::vector<double>& pars  () const { return m_sphere   .pars () ; }
      /// get bernstein coefficients
      const std::vector<double>& bpars () const { return m_bernstein.pars () ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get lower/upper edges
      double         xmin () const { return m_bernstein.xmin () ; }
      double         xmax () const { return m_bernstein.xmax () ; }
      double         ymin () const { return m_bernstein.ymin () ; }
      double         ymax () const { return m_bernstein.ymax () ; }
      // polynom order
      unsigned short n    () const { return m_bernstein.n    () ; }
      unsigned short nX   () const { return m_bernstein.nX   () ; }
      unsigned short nY   () const { return m_bernstein.nY   () ; }
      // ======================================================================
    public:
      // ======================================================================
      double tx ( const double  x ) const { return m_bernstein.tx (  x ) ; }
      double ty ( const double  y ) const { return m_bernstein.ty (  y ) ; }
      double  x ( const double tx ) const { return m_bernstein. x ( tx ) ; }
      double  y ( const double ty ) const { return m_bernstein. y ( ty ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /** get the integral over 2D-region
       *  \f[ \int_{x_low}^{x_high}\int_{y_low}^{y_high}
       *   \mathcal{B}(x,y) \mathrm{d}x\mathrm{d}y \f]
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integral   ( const double xlow , const double xhigh ,
                          const double ylow , const double yhigh ) const ;
      // ======================================================================
      /** integral over x-dimension
       *  \f[ \int_{y_low}^{y_high} \mathcal{B}(x,y) \mathrm{d}y\f]
       *  @param x     variable
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateX ( const double y    ,
                          const double xlow , const double xhigh ) const ;
      // ======================================================================
      /** integral over x-dimension
       *  \f[ \int_{x_low}^{x_high} \mathcal{B}(x,y) \mathrm{d}x\f]
       *  @param y     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateY ( const double x    ,
                          const double ylow , const double yhigh ) const ;
      // ======================================================================
    public: // specific
      // ======================================================================
      /** get the integral over 2D-region
       *  \f[ \int_{x_{min}}^{x_{max}}\int_{y_{min}}^{y_{max}}
       *   \mathcal{B}(x,y) \mathrm{d}x\mathrm{d}y \f]
       */
      double integral   () const ;
      /** integral over x-dimension
       *  \f[ \int_{x_{min}}^{x_{max}} \mathcal{B}(x,y) \mathrm{d}x\f]
       *  @param x     variable
       */
      double integrateX ( const double y ) const ;
      /** integral over y-dimension
       *  \f[ \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y) \mathrm{d}y\f]
       *  @param y     variable
       */
      double integrateY ( const double x ) const ;
      // ======================================================================
    public:
      // ======================================================================
      // get the bernstein 2D polynom
      const Gaudi::Math::Bernstein2DSym& bernstein() const
      { return m_bernstein ; }
      /// get the parameter sphere
      const  Gaudi::Math::NSphere&       sphere   () const
      { return m_sphere ; }
      // ======================================================================
    public:
      // ======================================================================
      /// swap  two 2D-polynomials 
      void swap ( Positive2DSym&  right ) ;
      // ======================================================================
    private:
      // ======================================================================
      /// update bernstein coefficients
      bool updateBernstein () ;
      // ======================================================================
    private:
      // ======================================================================
      /// the actual bernstein polynomial
      Gaudi::Math::Bernstein2DSym m_bernstein ; // the actual bernstein polynomial
      /// Parameter sphere
      Gaudi::Math::NSphere        m_sphere ;
      // ======================================================================
    } ;
    // ========================================================================
  } //                                        The end of namespace  Gaudi::Math
  // ==========================================================================
} //                                                 The end of namespace Gaudi
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // LHCBMATH_BERNSTEIN2D_H
