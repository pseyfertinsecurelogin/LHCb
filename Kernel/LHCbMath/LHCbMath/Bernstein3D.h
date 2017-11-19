// ================================================================================
#ifndef LHCBMATH_BERNSTEIN3D_H 
#define LHCBMATH_BERNSTEIN3D_H 1
// ============================================================================
// Include files
// ============================================================================
// LHCbMath
// ============================================================================
#include "LHCbMath/LHCbMath.h"
#include "LHCbMath/BernsteinPoly.h"
#include "LHCbMath/NSphere.h"
// ============================================================================
/** @file LHCbMath/Bernstein3D.h
 *  Collection of files related to 3D-moodels, based on Bernstein polynomials 
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
    /** @class Bernstein3D
     *  Generic 3D-polynomial of order defined as 
     *  \f[ P(x,y,z) = \sum_{i,j,k} a_{ijk}B^{n_x}_i(x) B^{n_y}_j(y) B^{n_z}_k(z)\f] 
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2017-11-14
     */
    class GAUDI_API Bernstein3D
    {
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the order
      Bernstein3D ( const unsigned short       nX    =  1 ,
                    const unsigned short       nY    =  1 ,
                    const unsigned short       nZ    =  1 ,
                    const double               xmin  =  0 ,
                    const double               xmax  =  1 ,
                    const double               ymin  =  0 ,
                    const double               ymax  =  1 ,
                    const double               zmin  =  0 ,
                    const double               zmax  =  1 ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double operator () ( const double x ,
                           const double y , 
                           const double z ) const ;
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
                          const unsigned short n     ,
                          const double         value ) ;
      /// set (l,m)-parameter
      bool setParameter ( const unsigned short l     ,
                          const unsigned short m     ,
                          const unsigned short n     ,
                          const double         value )
      { return setPar   ( l , m  , n , value ) ; }
      // ======================================================================
    public: // getters
      // ======================================================================
      /// get (l,m,n)-parameter
      double  par       ( const unsigned short l ,
                          const unsigned short m ,
                          const unsigned short n ) const ;
      /// get (l,m,n)-parameter
      double  parameter ( const unsigned short l ,
                          const unsigned short m ,
                          const unsigned short n ) const { return par (  l , m , n ) ; }
      /// get k-parameter
      double  par       ( const unsigned int k ) const
      { return k < m_pars.size() ? m_pars[k] : 0.0 ; }
      /// get k-parameter
      double  parameter ( const unsigned int k ) const { return par ( k ) ; }
      /// get all parameters at once
      const std::vector<double>& pars() const { return m_pars ; }
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
      /// get lower edge
      double zmin () const { return m_zmin ; }
      /// get upper edge
      double zmax () const { return m_zmax ; }
      /// get the polynomial order (X)
      unsigned short nX () const { return m_nx ; }
      /// get the polynomial order (Y)
      unsigned short nY () const { return m_ny ; }
      /// get the polynomial order (Y)
      unsigned short nZ () const { return m_nz ; }
      // ======================================================================
    public:  // transformations
      // ======================================================================
      double x  ( const double tx ) const
      { return xmin ()  + ( xmax () - xmin () ) * tx ; }
      double y  ( const double ty ) const
      { return ymin ()  + ( ymax () - ymin () ) * ty ; }
      double z  ( const double tz ) const
      { return zmin ()  + ( zmax () - zmin () ) * tz ; }
      double tx ( const double x ) const
      { return  ( x - xmin () ) / ( xmax () - xmin () )      ; }
      double ty ( const double y ) const
      { return  ( y - ymin () ) / ( ymax () - ymin () )      ; }
      double tz ( const double z ) const
      { return  ( z - zmin () ) / ( zmax () - zmin () )      ; }
      // ======================================================================
    public: // general integration
      // ======================================================================
      /** get the integral over 2D-region
       *  \f[ \int_{x_{low}}^{x_{high}} 
       *      \int_{y_{low}}^{y_{high}}
       *      \int_{z_{low}}^{z_{high}}
       *      \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\mathrm{d}z\f]
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       *  @param zlow  low  edge in z
       *  @param zhigh high edge in z
       */
      double integral   ( const double xlow , const double xhigh ,
                          const double ylow , const double yhigh ,
                          const double zlow , const double zhigh ) const ;
      // ======================================================================
      /** integral over x-dimension
       *  \f[ \int_{x_{low}}^{x_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\f]
       *  @param x     variable
       *  @param z     variable
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateX ( const double y    ,
                          const double z    ,                          
                          const double xlow , const double xhigh ) const ;
      /** integral over y-dimension
       *  \f[ \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y,z) \mathrm{d}y\f]
       *  @param y     variable
       *  @param z     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateY ( const double x    ,
                          const double z    ,
                          const double ylow , const double yhigh ) const ;
      /** integral over z-dimension
       *  \f[ \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}z\f]
       *  @param x     variable
       *  @param y     variable
       *  @param zlow  low  edge in z
       *  @param zhigh high edge in z
       */
      double integrateZ ( const double x    ,
                          const double y    ,
                          const double zlow , const double zhigh ) const ;
      // ======================================================================
      /** integral over x&y-dimensions
       *  \f[ \int_{x_{low}}^{x_{high}}
       *      \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
       *  @param z     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateXY ( const double z    ,                          
                           const double xlow , const double xhigh ,
                           const double ylow , const double yhigh ) const ;
      /** integral over x&z-dimensions
       *  \f[ \int_{x_{low}}^{x_{high}}
       *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
       *  @param y     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param zlow  low  edge in y
       *  @param zhigh high edge in y
       */
      double integrateXZ ( const double y    ,                          
                           const double xlow , const double xhigh ,
                           const double zlow , const double zhigh ) const ;      
      /** integral over y&z-dimensions
       *  \f[ \int_{y_{low}}^{y_{high}}
       *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
       *  @param x     variable
       *  @param ylow  low  edge in x
       *  @param yhigh high edge in x
       *  @param zlow  low  edge in y
       *  @param zhigh high edge in y
       */
      double integrateYZ ( const double x    ,                          
                           const double ylow , const double yhigh ,
                           const double zlow , const double zhigh ) const ;
      // ======================================================================      
    public: // special cases
      // ======================================================================
      /** get the integral over 3D-region
       *  \f[  x_{min} < x < x_{max}, 
       *       y_{min} < y < y_{max},
       *       z_{min} < z < z_{max}\f]
       */
      double integral   () const ;
      /** integral over x-dimension
       *  \f[ \int_{x_{min}}^{x_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\f]
       *  @param y     variable
       *  @param z     variable
       */
      double integrateX ( const double y , 
                          const double z ) const ;
      /** integral over y-dimension
       *  \f[ \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y,z) \mathrm{d}y\f]
       *  @param x     variable
       *  @param z     variable
       */
      double integrateY ( const double x , 
                          const double z ) const ;
      /** integral over z-dimension
       *  \f[ \int_{z_{min}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}z\f]
       *  @param x     variable
       *  @param y     variable
       */
      double integrateZ ( const double x , 
                          const double y ) const ;

      // ======================================================================
    public: // special cases
      // ======================================================================
      /** integral over x&y-dimensions
       *  \f[ \int_{x_{min}}^{x_{max}}
       *      \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
       *  @param z     variable
       */
      double integrateXY ( const double z    ) const ;
      /** integral over x&z-dimensions
       *  \f[ \int_{x_{min}}^{x_{min}}
       *      \int_{z_{max}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
       *  @param y     variable
       */
      double integrateXZ ( const double y    ) const ;
      /** integral over y&z-dimensions
       *  \f[ \int_{y_{min}}^{y_{max}}
       *      \int_{z_{min}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
       *  @param x     variable
       */
      double integrateYZ ( const double x    ) const ;
      // ======================================================================
    public: // few helper functions to expose internals
      // ======================================================================
      /// evaluate the basic polynomials
      double basicX ( const unsigned short i , const double         x ) const
      { return ( i > m_nx || x < m_xmin || x < m_xmax ) ? 0.0 : m_bx[i](x) ; }
      /// evaluate the basic polynomials
      double basicY ( const unsigned short i , const double         y ) const
      { return ( i > m_ny || y < m_ymin || y < m_ymax ) ? 0.0 : m_by[i](y) ; }
      /// evaluate the basic polynomials
      double basicZ ( const unsigned short i , const double         z ) const
      { return ( i > m_nz || z < m_zmin || z < m_zmax ) ? 0.0 : m_bz[i](z) ; }
      /// expose some internals
      const Bernstein& basicX ( const unsigned short i ) const { return m_bx[i] ; }
      /// expose some internals
      const Bernstein& basicY ( const unsigned short i ) const { return m_by[i] ; }
      /// expose some internals
      const Bernstein& basicZ ( const unsigned short i ) const { return m_bz[i] ; }
      // ======================================================================
    private: // helper functions to make calculations
      // ======================================================================
      /// helper function to make calculations
      double calculate ( const std::vector<double>& fx , 
                         const std::vector<double>& fy , 
                         const std::vector<double>& fz ) const ;
      // ======================================================================
    private:
      // ======================================================================
      // polynom order in x-dimension
      unsigned short m_nx ; // polynom order in x-dimension
      // polynom order in y-dimension
      unsigned short m_ny ; // polynom order in y-dimension
      // polynom order in z-dimension
      unsigned short m_nz ; // polynom order in z-dimension
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
      /// the left edge of interval
      double m_zmin  ;                             // the left edge of interval
      /// the right edge of interval
      double m_zmax  ;                             // the right edge of interval
      // ======================================================================
    private:
      // ======================================================================
      ///  vectors of basic  Bernstein polynomials
      typedef std::vector<Bernstein>  VB ;
      ///  vector  of basic  Bernstein polynomials
      VB m_bx ; //  vector  of basic  Bernstein polynomials
      ///  vector  of basic  Bernstein polynomials
      VB m_by ; //  vector  of basic  Bernstein polynomials
      ///  vector  of basic  Bernstein polynomials
      VB m_bz ; //  vector  of basic  Bernstein polynomials
      // ======================================================================
    } ;

    // ========================================================================
    /** @class Positive3D
     *  The 3D-polynomial of order Nx*Ny*Nz, that is constrained 
     *  to be non-negative over the  defined range      
     *  \f[  P(x,y,z) = \sum_{i,j,k} a_{ijk}B^{n_x}_i(x) B^{n_y}_j(y) B^{n_z}_k(z)\f] 
     *  where all coefficients \f$a_{ijk}\f$ are non-negative and 
     *  \f$ \sum_{i,j,k} a_{ijk}=1 \f$ 
     *  @author Vanya BELYAEV Ivan.Belayev@itep.ru
     *  @date 2017-11-14
     */
    class GAUDI_API Positive3D 
    {
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the order
      Positive3D ( const unsigned short       Nx    =  1 ,
                   const unsigned short       Ny    =  1 ,
                   const unsigned short       Nz    =  1 ,
                   const double               xmin  =  0 ,
                   const double               xmax  =  1 ,
                   const double               ymin  =  0 ,
                   const double               ymax  =  1 ,
                   const double               zmin  =  0 ,
                   const double               zmax  =  1 ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double operator () ( const double x , 
                           const double y , 
                           const double z ) const
      { return m_bernstein ( x , y , z ) ; }
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
      double  par       ( const unsigned int k ) const ;
      /// get the parameter value
      double  parameter ( const unsigned int k ) const { return par ( k ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get lower/upper edges
      double         xmin () const { return m_bernstein.xmin () ; }
      double         xmax () const { return m_bernstein.xmax () ; }
      double         ymin () const { return m_bernstein.ymin () ; }
      double         ymax () const { return m_bernstein.ymax () ; }
      double         zmin () const { return m_bernstein.zmin () ; }
      double         zmax () const { return m_bernstein.zmax () ; }
      // polynom order
      unsigned short nX   () const { return m_bernstein.nX   () ; }
      unsigned short nY   () const { return m_bernstein.nY   () ; }
      unsigned short nZ   () const { return m_bernstein.nZ   () ; }
      // ======================================================================
    public:
      // ======================================================================
      // transform variables
      double tx ( const double  x ) const { return m_bernstein.tx (  x ) ; }
      double ty ( const double  y ) const { return m_bernstein.ty (  y ) ; }
      double tz ( const double  z ) const { return m_bernstein.tz (  z ) ; }
      double  x ( const double tx ) const { return m_bernstein. x ( tx ) ; }
      double  y ( const double ty ) const { return m_bernstein. y ( ty ) ; }
      double  z ( const double tz ) const { return m_bernstein. z ( tz ) ; }
      // ======================================================================
    public: // integrals 
      // ======================================================================
      /** get the integral over 3D-region
       *  \f[ \int_{x_{low}}^{x_{high}} 
       *      \int_{y_{low}}^{y_{high}}
       *      \int_{z_{low}}^{z_{high}}
       *      \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\mathrm{d}z\f]
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       *  @param zlow  low  edge in z
       *  @param zhigh high edge in z
       */
      double integral   ( const double xlow , const double xhigh ,
                          const double ylow , const double yhigh ,
                          const double zlow , const double zhigh ) const ;
      // ======================================================================
    public: //  partial integrals 
      // ======================================================================
      /** integral over x-dimension
       *  \f[ \int_{x_{low}}^{x_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\f]
       *  @param x     variable
       *  @param z     variable
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateX ( const double y    ,
                          const double z    ,                          
                          const double xlow , const double xhigh ) const 
      { return m_bernstein.integrateX ( y ,  z , xlow , xhigh ) ; }
      /** integral over y-dimension
       *  \f[ \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y,z) \mathrm{d}y\f]
       *  @param y     variable
       *  @param z     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateY ( const double x    ,
                          const double z    ,
                          const double ylow , const double yhigh ) const 
      { return m_bernstein.integrateY ( x ,  z , ylow , yhigh ) ; }
      /** integral over z-dimension
       *  \f[ \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}z\f]
       *  @param x     variable
       *  @param y     variable
       *  @param zlow  low  edge in z
       *  @param zhigh high edge in z
       */
      double integrateZ ( const double x    ,
                          const double y    ,
                          const double zlow , const double zhigh ) const 
      { return m_bernstein.integrateZ ( x ,  y , zlow , zhigh ) ; }
      // ======================================================================
      /** integral over x&y-dimensions
       *  \f[ \int_{x_{low}}^{x_{high}}
       *      \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
       *  @param z     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateXY ( const double z    ,                          
                           const double xlow , const double xhigh ,
                           const double ylow , const double yhigh ) const 
      { return m_bernstein.integrateXY ( z , xlow , xhigh , ylow , yhigh ) ; }
      /** integral over x&z-dimensions
       *  \f[ \int_{x_{low}}^{x_{high}}
       *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
       *  @param y     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param zlow  low  edge in y
       *  @param zhigh high edge in y
       */
      double integrateXZ ( const double y    ,                          
                           const double xlow , const double xhigh ,
                           const double zlow , const double zhigh ) const 
      { return m_bernstein.integrateXZ ( y , xlow , xhigh , zlow , zhigh ) ; }
      /** integral over y&z-dimensions
       *  \f[ \int_{y_{low}}^{y_{high}}
       *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
       *  @param x     variable
       *  @param ylow  low  edge in x
       *  @param yhigh high edge in x
       *  @param zlow  low  edge in y
       *  @param zhigh high edge in y
       */
      double integrateYZ ( const double x    ,                          
                           const double ylow , const double yhigh ,
                           const double zlow , const double zhigh ) const 
      { return m_bernstein.integrateYZ ( x , ylow , yhigh , zlow , zhigh ) ; }
      // ======================================================================      
    public: // Integrals: special cases
      // ======================================================================
      /** get the integral over 3D-region
       *  \f[  x_{min} < x < x_{max}, 
       *       y_{min} < y < y_{max},
       *       z_{min} < z < z_{max} \f]
       */
      double integral   () const ;
      /** integral over x-dimension
       *  \f[ \int_{x_{min}}^{x_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\f]
       *  @param y     variable
       *  @param z     variable
       */
      double integrateX ( const double y , 
                          const double z ) const 
      { return m_bernstein.integrateX ( y , z ) ; }
      /** integral over y-dimension
       *  \f[ \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y,z) \mathrm{d}y\f]
       *  @param x     variable
       *  @param z     variable
       */
      double integrateY ( const double x , 
                          const double z ) const 
      { return m_bernstein.integrateY ( x , z ) ; }
      /** integral over z-dimension
       *  \f[ \int_{z_{min}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}z\f]
       *  @param x     variable
       *  @param y     variable
       */
      double integrateZ ( const double x , 
                          const double y ) const 
      { return m_bernstein.integrateZ ( x , y ) ; }
      // ======================================================================
    public: // Integrals: special cases
      // ======================================================================
      /** integral over x&y-dimensions
       *  \f[ \int_{x_{min}}^{x_{max}}
       *      \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
       *  @param z     variable
       */
      double integrateXY ( const double z    ) const 
      { return m_bernstein.integrateXY ( z ) ; }
      /** integral over x&z-dimensions
       *  \f[ \int_{x_{min}}^{x_{min}}
       *      \int_{z_{max}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
       *  @param y     variable
       */
      double integrateXZ ( const double y    ) const 
      { return m_bernstein.integrateXZ ( y ) ; }
      /** integral over y&z-dimensions
       *  \f[ \int_{y_{min}}^{y_{max}}
       *      \int_{z_{min}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
       *  @param x     variable
       */
      double integrateYZ ( const double x    ) const 
      { return m_bernstein.integrateYZ ( x ) ; }
      // ======================================================================
    public: // ingeredients
      // =====================================================================
      // get the bernstein polinomial in 2D
      const  Gaudi::Math::Bernstein3D& bernstein () const
      { return m_bernstein ; }
      /// get the parameter sphere
      const  Gaudi::Math::NSphere&     sphere    () const
      { return m_sphere ; }
      // ======================================================================
    private:
      // ======================================================================
      /// update bernstein coefficients
      bool updateBernstein () ;
      // ======================================================================
    private:
      // ======================================================================
      /// the actual bernstein polynomial
      Gaudi::Math::Bernstein3D m_bernstein ; // the actual bernstein polynomial
      /// the external parameter sphere
      Gaudi::Math::NSphere     m_sphere    ;
      // ======================================================================
    } ;
    // ========================================================================
  } //                                         The end of namespace Gaudi::Math
  // ==========================================================================
} //                                                The end of namespace  Gaudi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LHCBMATH_BERNSTEIN3D_H
// ============================================================================
