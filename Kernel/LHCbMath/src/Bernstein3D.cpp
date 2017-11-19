// ============================================================================
// Include files
// ============================================================================
// STD& STL
// ============================================================================
#include <array>
#include <climits>
#include <cassert>
#include <numeric>
// ============================================================================
// LHCbMath
// ============================================================================
#include "LHCbMath/LHCbMath.h"
#include "LHCbMath/Bernstein3D.h"
// ============================================================================
/** @file
 *  Implementation file for functions, related to Bernstein's polynomnials
 *
 *  @see http://en.wikipedia.org/wiki/Bernstein_polynomial
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2010-04-19
 */
// ============================================================================
namespace
{
  // ==========================================================================
  /// equality criteria for doubles
  const LHCb::Math::Equal_To<double> s_equal{} ; // equality criteria for doubles
  /// zero for doubles
  const LHCb::Math::Zero<double>     s_zero{}  ; // zero for doubles
  /// zero fo vectors
  const LHCb::Math::Zero< std::vector<double> > s_vzero{} ; // zero for vectors
  ///
  static_assert( std::numeric_limits<double>::is_specialized ,
                 "std::numeric_limits<double> is not specialized" ) ;
  static_assert( std::numeric_limits<long double>::is_specialized ,
                 "std::numeric_limits<long double> is not specialized" ) ;
  /// small value
  const LHCb::Math::Small<long double> s_small
  ( 2.0L * std::numeric_limits<double>::epsilon() ) ;
  // ==========================================================================
}
// ============================================================================
// constructor from the order
// ============================================================================
Gaudi::Math::Bernstein3D::Bernstein3D 
( const unsigned short       nX    ,
  const unsigned short       nY    ,
  const unsigned short       nZ    ,
  const double               xmin  ,
  const double               xmax  ,
  const double               ymin  ,
  const double               ymax  ,
  const double               zmin  ,
  const double               zmax  ) 
  : m_nx   ( nX )
  , m_ny   ( nY )
  , m_nz   ( nZ )
    //
  , m_pars ( ( nX + 1 ) * ( nY + 1 ) * ( nZ + 1 ) , 0.0 )
    //
  , m_xmin ( std::min ( xmin , xmax ) )
  , m_xmax ( std::max ( xmin , xmax ) )
  , m_ymin ( std::min ( ymin , ymax ) )
  , m_ymax ( std::max ( ymin , ymax ) )
  , m_zmin ( std::min ( zmin , zmax ) )
  , m_zmax ( std::max ( zmin , zmax ) )
    //
  , m_bx   ()
  , m_by   ()
  , m_bz   ()
{
  //
  typedef  Gaudi::Math::Bernstein::Basic BB ;
  for ( unsigned short ix = 0 ; ix <= nX ; ++ix )
  { m_bx.push_back ( Bernstein ( BB ( ix , nX ) , xmin , xmax ) ) ; }
  //
  for ( unsigned short iy = 0 ; iy <= nY ; ++iy )
  { m_by.push_back ( Bernstein ( BB ( iy , nY ) , ymin , ymax ) ) ; }
  //
  for ( unsigned short iz = 0 ; iz <= nZ ; ++iz )
  { m_bz.push_back ( Bernstein ( BB ( iz , nZ ) , zmin , zmax ) ) ; }
  //
}
// ============================================================================
// helper function to make calculations
// ============================================================================
double Gaudi::Math::Bernstein3D::calculate
( const std::vector<double>& fx , 
  const std::vector<double>& fy , 
  const std::vector<double>& fz ) const 
{
  double       result = 0 ;
  for  ( unsigned short ix = 0 ; ix <= m_nx ; ++ix )
  {
    for  ( unsigned short iy = 0 ; iy <= m_ny ; ++iy )
    { 
      for  ( unsigned short iz = 0 ; iz <= m_nz ; ++iz )
      { result += par ( ix , iy , iz ) * fx[ix] * fy[iy] * fz[iz]; }
    }
  }
  //
  const double scalex = ( m_nx + 1 ) / ( xmax() - xmin() ) ;
  const double scaley = ( m_ny + 1 ) / ( ymax() - ymin() ) ;
  const double scalez = ( m_nz + 1 ) / ( zmax() - zmin() ) ;
  //
  return result * scalex * scaley * scalez ;
}
// ============================================================================
// get the value
// ============================================================================
double Gaudi::Math::Bernstein3D::operator () ( const double x ,
                                               const double y , 
                                               const double z ) const
{
  /// the trivial cases
  if ( x < m_xmin || x > m_xmax ) { return 0.0        ; }
  if ( y < m_ymin || y > m_ymax ) { return 0.0        ; }
  if ( z < m_zmin || z > m_zmax ) { return 0.0        ; }
  //
  if      ( 0 == npars ()       ) { return 0.0        ; }
  else if ( 1 == npars ()       ) 
  { 
    const double scalex = ( m_nx + 1 ) / ( xmax() - xmin() ) ;
    const double scaley = ( m_ny + 1 ) / ( ymax() - ymin() ) ;
    const double scalez = ( m_nz + 1 ) / ( zmax() - zmin() ) ;
    //
    return m_pars [0] * scalex * scaley * scalez ;
  }
  ///
  std::vector<double> fx ( m_nx + 1 , 0 ) ;
  for  ( unsigned short i = 0 ; i <= m_nx ; ++i )
  { fx[i] = m_bx[i] ( x )  ; }
  //
  std::vector<double> fy ( m_ny + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_ny ; ++i )
  { fy[i] = m_by[i] ( y )  ; }
  //
  std::vector<double> fz ( m_nz + 1 , 0 ) ;
  for  ( unsigned short i = 0 ; i <= m_nz ; ++i )
  { fz[i] = m_bz[i] ( z )  ; }
  //
  return calculate ( fx , fy , fz ) ;
}

// ============================================================================
/** get the integral over 3D-region
 *  \f[  x_{min} < x < x_{max}, y_{min}< y< y_{max} , z_{min} < z < z_{max}\f]
 */
// ============================================================================
double Gaudi::Math::Bernstein3D::integral() const
{ return std::accumulate ( m_pars.begin() , m_pars.end() , 0.0 ) ; }
// ============================================================================
/* get the integral over 3D-region
 *  \f[ \int_{x_{low}}^{x_{high}}
 *      \int_{y_{low}}^{y_{high}} 
 *      \int_{z_{low}}^{z_{high}} 
 *\mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\mathrm{d}z\f]
 *  @param xlow  low  edge in x
 *  @param xhigh high edge in x
 *  @param ylow  low  edge in y
 *  @param yhigh high edge in y
 *  @param zlow  low  edge in z
 *  @param zhigh high edge in z
 */
// ============================================================================
double Gaudi::Math::Bernstein3D::integral
( const double xlow , const double xhigh ,
  const double ylow , const double yhigh ,
  const double zlow , const double zhigh ) const
{
  if      ( s_equal ( xlow , xhigh ) ||
            s_equal ( ylow , yhigh ) ||
            s_equal ( zlow , zhigh ) ) { return 0 ; }
  //
  else if ( s_equal ( xlow  , m_xmin ) &&
            s_equal ( xhigh , m_xmax ) &&
            s_equal ( ylow  , m_ymin ) &&
            s_equal ( yhigh , m_ymax ) &&  
            s_equal ( zlow  , m_zmin ) &&
            s_equal ( zhigh , m_zmax ) )  { return integral () ; }
  //
  else if ( xlow  > xhigh ) 
  { return -1*integral ( xhigh , xlow  , ylow  , yhigh , zlow  , zhigh ) ; }
  else if ( ylow  > yhigh ) 
  { return -1*integral ( xlow  , xhigh , yhigh , ylow  , zlow  , zhigh ) ; }
  else if ( zlow  > zhigh ) 
  { return -1*integral ( xlow  , xhigh , ylow  , yhigh , zhigh , zlow  ) ; }
  //
  else if ( xhigh <  xmin () || xlow >  xmax() ) { return 0 ; }
  else if ( yhigh <  ymin () || ylow >  ymax() ) { return 0 ; }
  else if ( zhigh <  zmin () || zlow >  zmax() ) { return 0 ; }
  //
  const double  x_low  = std::max ( xmin() , xlow  ) ;
  const double  x_high = std::min ( xmax() , xhigh ) ;
  if ( x_low >= x_high ) { return 0 ; }
  //
  const double  y_low  = std::max ( ymin() , ylow  ) ;
  const double  y_high = std::min ( ymax() , yhigh ) ;
  if ( y_low >= y_high ) { return 0 ; }
  //
  const double  z_low  = std::max ( zmin() , zlow  ) ;
  const double  z_high = std::min ( zmax() , zhigh ) ;
  if ( z_low >= z_high ) { return 0 ; }
  //
  std::vector<double> fx ( m_nx + 1 , 0 ) ;
  for  ( unsigned short i = 0 ; i <= m_nx ; ++i )
  { fx[i] = m_bx[i].integral ( x_low , x_high ) ; }
  //
  std::vector<double> fy ( m_ny + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_ny ; ++i )
  { fy[i] = m_by[i].integral ( y_low , y_high ) ; }
  //
  std::vector<double> fz ( m_nz + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_nz ; ++i )
  { fz[i] = m_bz[i].integral ( z_low , z_high ) ; }
  //
  return calculate ( fx , fy , fz ) ;
}
// ============================================================================
/*  integral over x-dimension
 *  \f[ \int_{y_low}^{y_high} \mathcal{B}(x,y,z) \mathrm{d}y\f]
 *  @param y     variable
 *  @param z     variable
 *  @param ylow  low  edge in y
 *  @param yhigh high edge in y
 */
// ============================================================================
double Gaudi::Math::Bernstein3D::integrateX
( const double y    ,
  const double z    ,
  const double xlow , const double xhigh ) const
{
  if      ( s_equal ( xlow , xhigh ) ) { return 0 ; }
  else if ( xlow  > xhigh  ) { return -1*integrateX ( y , z , xhigh , xlow ) ; }
  else if ( xhigh <= xmin () || xlow >= xmax() ) { return 0 ; }
  else if ( y     <  ymin () || y    >  ymax() ) { return 0 ; }
  else if ( z     <  zmin () || z    >  zmax() ) { return 0 ; }
  else if ( s_equal ( xlow  , m_xmin ) &&
            s_equal ( xhigh , m_xmax )         ) { return integrateX ( y ,  z ) ; }
  //
  const double  x_low  = std::max ( xmin() , xlow  ) ;
  const double  x_high = std::min ( xmax() , xhigh ) ;
  if ( x_low >= x_high ) { return 0 ; }
  //
  std::vector<double> fx ( m_nx + 1 , 0 ) ;
  for  ( unsigned short i = 0 ; i <= m_nx ; ++i )
  { fx[i] = m_bx[i].integral ( x_low , x_high ) ; }
  //
  std::vector<double> fy ( m_ny + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_ny ; ++i )
  { fy[i] = m_by[i] ( y ) ; }
  //
  std::vector<double> fz ( m_nz + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_nz ; ++i )
  { fz[i] = m_bz[i] ( z ) ; }
  //
  return calculate ( fx , fy , fz ) ;
}
// ============================================================================
/** integral over y-dimension
 *  \f[ \int_{x_low}^{x_high} \mathcal{B}(x,y,z) \mathrm{d}y\f]
 *  @param x     variable
 *  @param z     variable
 *  @param ylow  low  edge in y
 *  @param yhigh high edge in y
 */
// ============================================================================
double Gaudi::Math::Bernstein3D::integrateY
( const double x    ,
  const double z    ,
  const double ylow , const double yhigh ) const
{
  if      ( s_equal ( ylow  , yhigh ) ) { return 0 ; }
  else if ( ylow  >  yhigh ) { return -1*integrateY ( x , z , yhigh , ylow  ) ; }
  else if ( x     <  xmin () || x    >  xmax() ) { return 0 ; }
  else if ( z     <  zmin () || z    >  zmax() ) { return 0 ; }
  else if ( yhigh <= ymin () || ylow >= ymax() ) { return 0 ; }
  else if ( s_equal ( ylow  , m_ymin ) &&
            s_equal ( yhigh , m_ymax )         ) { return integrateY ( x , z ) ; }
  //
  const double  y_low  = std::max ( ymin() , ylow  ) ;
  const double  y_high = std::min ( ymax() , yhigh ) ;
  if ( y_low >= y_high ) { return 0 ; }
  //
  std::vector<double> fx ( m_nx + 1 , 0 ) ;
  for  ( unsigned short i = 0 ; i <= m_nx ; ++i )
  { fx[i] = m_bx[i] ( x ) ; }
  //
  std::vector<double> fy ( m_ny + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_ny ; ++i )
  { fy[i] = m_by[i].integral ( y_low , y_high ) ; }
  //
  std::vector<double> fz ( m_nz + 1 , 0 ) ;
  for  ( unsigned short i = 0 ; i <= m_nz ; ++i )
  { fz[i] = m_bz[i] ( z ) ; }
  //
  return calculate ( fx , fy , fz ) ;
}
// ============================================================================
/** integral over z-dimension
 *  \f[ \int_{z_low}^{z_high} \mathcal{B}(x,y,z) \mathrm{d}z\f]
 *  @param x     variable
 *  @param y     variable
 *  @param zlow  low  edge in z
 *  @param zhigh high edge in z
 */
// ============================================================================
double Gaudi::Math::Bernstein3D::integrateZ
( const double x    ,
  const double y    ,
  const double zlow , const double zhigh ) const
{
  if      ( s_equal ( zlow  , zhigh ) ) { return 0 ; }
  else if ( zlow  >  zhigh ) { return -1*integrateZ ( x , y , zhigh , zlow  ) ; }
  else if ( x     <  xmin () || x    >  xmax() ) { return 0 ; }
  else if ( y     <  ymin () || y    >  ymax() ) { return 0 ; }
  else if ( zhigh <= zmin () || zlow >= zmax() ) { return 0 ; }
  else if ( s_equal ( zlow  , m_zmin ) &&
            s_equal ( zhigh , m_zmax )         ) { return integrateZ ( x , y ) ; }
  //
  const double  z_low  = std::max ( zmin() , zlow  ) ;
  const double  z_high = std::min ( zmax() , zhigh ) ;
  if ( z_low >= z_high ) { return 0 ; }
  //
  std::vector<double> fx ( m_nx + 1 , 0 ) ;
  for  ( unsigned short i = 0 ; i <= m_nx ; ++i )
  { fx[i] = m_bx[i] ( x ) ; }
  //
  std::vector<double> fy ( m_ny + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_ny ; ++i )
  { fy[i] = m_by[i] ( y ) ; }
  //
  std::vector<double> fz ( m_nz + 1 , 0 ) ;
  for  ( unsigned short i = 0 ; i <= m_nz ; ++i ) 
  { fz[i] = m_bz[i].integral ( z_low , z_high ) ; }
  //
  return calculate ( fx , fy , fz ) ;
}


// ============================================================================
double Gaudi::Math::Bernstein3D::integrateX ( const double y , 
                                              const double z ) const
{
  if      ( y < ymin () || y > ymax() ) { return 0 ; }
  else if ( z < zmin () || z > zmax() ) { return 0 ; }
  //
  const std::vector<double> fx ( m_nx + 1 , ( xmax() - xmin () ) / ( m_nx + 1 ) ) ;
  //
  std::vector<double> fy ( m_ny + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_ny ; ++i )
  { fy[i] = m_by[i] ( y ) ; }
  //
  std::vector<double> fz ( m_nz + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_nz ; ++i )
  { fz[i] = m_bz[i] ( z ) ; }
  //
  return calculate ( fx , fy , fz ) ;
}
// ============================================================================
double Gaudi::Math::Bernstein3D::integrateY ( const double x , 
                                              const double z ) const
{
  if      ( x < xmin () || x > xmax() ) { return 0 ; }
  else if ( z < zmin () || z > zmax() ) { return 0 ; }
  //
  std::vector<double> fx ( m_nx + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_nx ; ++i )
  { fx[i] = m_bx[i] ( x ) ; }
  //
  const std::vector<double> fy ( m_ny + 1 , ( ymax() - ymin () ) / ( m_ny + 1 ) ) ;
  //
  std::vector<double> fz ( m_nz + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_nz ; ++i )
  { fz[i] = m_bz[i] ( z ) ; }
  //
  return calculate ( fx , fy , fz ) ;
}
// ============================================================================
double Gaudi::Math::Bernstein3D::integrateZ ( const double x , 
                                              const double y ) const
{
  if      ( x < xmin () || x > xmax() ) { return 0 ; }
  else if ( y < ymin () || y > ymax() ) { return 0 ; }
  //
  std::vector<double> fx ( m_nx + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_nx ; ++i )
  { fx[i] = m_bx[i] ( x ) ; }
  //
  std::vector<double> fy ( m_ny + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_ny ; ++i )
  { fy[i] = m_by[i] ( y ) ; }
  //
  const std::vector<double> fz ( m_nz + 1 , ( zmax() - zmin () ) / ( m_nz + 1 ) ) ;
  //
  return calculate ( fx , fy , fz ) ;
}

// ============================================================================
/*  integral over x&y-dimensions
 *  \f[ \int_{x_{low}}^{x_{high}}
 *      \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
 *  @param z     variable
 *  @param xlow  low  edge in x
 *  @param xhigh high edge in x
 *  @param ylow  low  edge in y
 *  @param yhigh high edge in y
 */
// ============================================================================
double Gaudi::Math::Bernstein3D::integrateXY
( const double z    ,                          
  const double xlow , const double xhigh ,
  const double ylow , const double yhigh ) const 
{
  if      ( s_equal ( xlow  , xhigh ) ) { return 0 ; }
  else if ( s_equal ( ylow  , yhigh ) ) { return 0 ; }
  else if ( xlow  >  xhigh ) { return -1*integrateXY ( z , xhigh , xlow  , ylow  , yhigh ) ; }
  else if ( ylow  >  yhigh ) { return -1*integrateXY ( z , xlow  , xhigh , yhigh , ylow  ) ; }
  else if ( z     <  zmin () || z    >  zmax() ) { return 0 ; }
  else if ( s_equal ( xlow  , m_xmin ) &&
            s_equal ( xhigh , m_xmax ) &&
            s_equal ( ylow  , m_ymin ) &&
            s_equal ( yhigh , m_ymax ) ) { return integrateXY ( z ) ; }
  //  //
  const double  x_low  = std::max ( xmin() , xlow  ) ;
  const double  x_high = std::min ( xmax() , xhigh ) ;
  if ( x_low >= x_high ) { return 0 ; }
  //
  const double  y_low  = std::max ( ymin() , ylow  ) ;
  const double  y_high = std::min ( ymax() , yhigh ) ;
  if ( y_low >= y_high ) { return 0 ; }
  //
  std::vector<double> fx ( m_nx + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_nx ; ++i )
  { fx[i] = m_bx[i].integral ( x_low , x_high ) ; }
  //
  std::vector<double> fy ( m_ny + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_ny ; ++i )
  { fy[i] = m_by[i].integral ( y_low , y_high ) ; }
  //
  std::vector<double> fz ( m_nz + 1 , 0 ) ;
  for  ( unsigned short i = 0 ; i <= m_nz ; ++i )
  { fz[i] = m_bz[i] ( z ) ; }
  //
  return calculate ( fx , fy , fz ) ;
}
// ============================================================================
/** integral over x&z-dimensions
 *  \f[ \int_{x_{low}}^{x_{high}}
 *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
 *  @param y     variable
 *  @param xlow  low  edge in x
 *  @param xhigh high edge in x
 *  @param zlow  low  edge in y
 *  @param zhigh high edge in y
 */
// ============================================================================
double Gaudi::Math::Bernstein3D::integrateXZ
( const double y    ,                          
  const double xlow , const double xhigh ,
  const double zlow , const double zhigh ) const 
{
  if      ( s_equal ( xlow  , xhigh ) ) { return 0 ; }
  else if ( s_equal ( zlow  , zhigh ) ) { return 0 ; }
  else if ( xlow  >  xhigh ) { return -1*integrateXZ ( y , xhigh , xlow  , zlow  , zhigh ) ; }
  else if ( zlow  >  zhigh ) { return -1*integrateXZ ( y , xlow  , xhigh , zhigh , zlow  ) ; }
  else if ( y     <  ymin () || y    >  ymax() ) { return 0 ; }
  else if ( s_equal ( xlow  , m_xmin ) &&
            s_equal ( xhigh , m_xmax ) &&
            s_equal ( zlow  , m_zmin ) &&
            s_equal ( zhigh , m_zmax ) ) { return integrateXZ ( y ) ; }
  //
  const double  x_low  = std::max ( xmin() , xlow  ) ;
  const double  x_high = std::min ( xmax() , xhigh ) ;
  if ( x_low >= x_high ) { return 0 ; }
  //
  const double  z_low  = std::max ( zmin() , zlow  ) ;
  const double  z_high = std::min ( zmax() , zhigh ) ;
  if ( z_low >= z_high ) { return 0 ; }
  //
  std::vector<double> fx ( m_nx + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_nx ; ++i )
  { fx[i] = m_bx[i].integral ( x_low , x_high ) ; }
  //
  std::vector<double> fy ( m_ny + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_ny ; ++i )
  { fy[i] = m_by[i] ( y ) ; }
  //
  std::vector<double> fz ( m_nz + 1 , 0 ) ;
  for  ( unsigned short i = 0 ; i <= m_nz ; ++i )
  { fz[i] = m_bz[i].integral ( z_low , z_high ) ; }
  //
  return calculate ( fx , fy , fz ) ;
}
// ============================================================================
/** integral over y&z-dimensions
 *  \f[ \int_{y_{low}}^{y_{high}}
 *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
 *  @param x     variable
 *  @param ylow  low  edge in x
 *  @param yhigh high edge in x
 *  @param zlow  low  edge in y
 *  @param zhigh high edge in y
 */
// ============================================================================ 
double Gaudi::Math::Bernstein3D::integrateYZ
( const double x    ,                          
  const double ylow , const double yhigh ,
  const double zlow , const double zhigh ) const 
{
  if      ( s_equal ( ylow  , yhigh ) ) { return 0 ; }
  else if ( s_equal ( zlow  , zhigh ) ) { return 0 ; }
  else if ( ylow  >  yhigh ) { return -1*integrateYZ ( x , yhigh , ylow  , zlow  , zhigh ) ; }
  else if ( zlow  >  zhigh ) { return -1*integrateYZ ( x , ylow  , yhigh , zhigh , zlow  ) ; }
  else if ( x     <  xmin () || x    >  xmax() ) { return 0 ; }
  else if ( s_equal ( ylow  , m_ymin ) &&
            s_equal ( yhigh , m_ymax ) &&
            s_equal ( zlow  , m_zmin ) &&
            s_equal ( zhigh , m_zmax ) ) { return integrateYZ ( x ) ; }
  //
  const double  y_low  = std::max ( ymin() , ylow  ) ;
  const double  y_high = std::min ( ymax() , yhigh ) ;
  if ( y_low >= y_high ) { return 0 ; }
  //
  const double  z_low  = std::max ( zmin() , zlow  ) ;
  const double  z_high = std::min ( zmax() , zhigh ) ;
  if ( z_low >= z_high ) { return 0 ; }
  //
  std::vector<double> fx ( m_nx + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_nx ; ++i )
  { fx[i] = m_bx[i] ( x ) ; }
  //
  std::vector<double> fy ( m_ny + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_ny ; ++i )
  { fy[i] = m_by[i].integral ( y_low , y_high ) ; }
  //
  std::vector<double> fz ( m_nz + 1 , 0 ) ;
  for  ( unsigned short i = 0 ; i <= m_nz ; ++i )
  { fz[i] = m_bz[i].integral ( z_low , z_high ) ; }
  //
  return calculate ( fx , fy , fz ) ;
}
// ============================================================================
/*  integral over x&y-dimensions
 *  \f[ \int_{x_{min}}^{x_{max}}
 *      \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
 *  @param z     variable
 */
// ============================================================================
double Gaudi::Math::Bernstein3D::integrateXY ( const double z    ) const 
{
  if ( z < zmin () || z > zmax() ) { return 0 ; }
  //
  const std::vector<double> fx ( m_nx + 1 , ( xmax() - xmin () ) / ( m_nx + 1 ) ) ;
  const std::vector<double> fy ( m_ny + 1 , ( ymax() - ymin () ) / ( m_ny + 1 ) ) ;
  //
  std::vector<double> fz ( m_nz + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_nz ; ++i )
  { fz[i] = m_bz[i] ( z ) ; }
  //
  return calculate ( fx , fy , fz ) ;
}
// ============================================================================
/*  integral over x&z-dimensions
 *  \f[ \int_{x_{min}}^{x_{min}}
 *      \int_{z_{max}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
 *  @param y     variable
 */
// ============================================================================
double Gaudi::Math::Bernstein3D::integrateXZ ( const double y    ) const 
{
  if ( y < ymin () || y > ymax() ) { return 0 ; }
  //
  const std::vector<double> fx ( m_nx + 1 , ( xmax() - xmin () ) / ( m_nx + 1 ) ) ;
  //
  std::vector<double> fy ( m_ny + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_ny ; ++i )
  { fy[i] = m_by[i] ( y ) ; }
  //
  const std::vector<double> fz ( m_nz + 1 , ( zmax() - zmin () ) / ( m_nz + 1 ) ) ;
  //
  return calculate ( fx , fy , fz ) ;
}
// ============================================================================
/* integral over y&z-dimensions
 *  \f[ \int_{y_{min}}^{y_{max}}
 *      \int_{z_{min}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
 *  @param x     variable
 */
// ============================================================================
double Gaudi::Math::Bernstein3D::integrateYZ ( const double x    ) const 
{
  if ( x < xmin () || x > xmax() ) { return 0 ; }
  //
  std::vector<double> fx ( m_nx + 1 , 0 ) ;
  for ( unsigned short i = 0 ; i <= m_nx ; ++i )
  { fx[i] = m_bx[i] ( x ) ; }
  //
  const std::vector<double> fy ( m_ny + 1 , ( ymax() - ymin () ) / ( m_ny + 1 ) ) ;
  //
  const std::vector<double> fz ( m_nz + 1 , ( zmax() - zmin () ) / ( m_nz + 1 ) ) ;
  //
  return calculate ( fx , fy , fz ) ;
}
// ============================================================================
// set (l,m,n)-parameter
// ============================================================================
bool Gaudi::Math::Bernstein3D::setPar
( const unsigned short l     ,
  const unsigned short m     ,
  const unsigned short n     ,
  const double         value )
{
  if ( l > m_nx || m > m_ny || n > m_nz ) { return false ; }
  //
  // ix*(ny+1)*(nz+1)+iy*(nz+1)+iz
  const unsigned int nzp1 =  m_nz + 1 ;
  const unsigned int k    =
    nzp1 * l * ( m_ny + 1 ) + 
    nzp1 * m                + 
    n ;
  //
  return setPar ( k , value ) ;
}
// ============================================================================
// set k-parameter
// ============================================================================
bool Gaudi::Math::Bernstein3D::setPar
( const unsigned int   k     ,
  const double         value )
{
  if ( k >= npars() )                     { return false ; }
  if ( s_equal ( m_pars [ k ] , value ) ) { return false ; }
  m_pars [ k ] = value ;
  return true ;
}
// ============================================================================
// get (l,m)-parameter
// ============================================================================
double  Gaudi::Math::Bernstein3D::par
( const unsigned short l ,
  const unsigned short m ,
  const unsigned short n ) const
{
  if ( l > m_nx || m > m_ny || n > m_nz ) { return 0 ; }
  //
  // ix*(ny+1)*(nz+1)+iy*(nz+1)+iz
  const unsigned int nzp1 =  m_nz + 1 ;
  const unsigned int k    =
    nzp1 * l * ( m_ny + 1 ) + 
    nzp1 * m                + 
    n ;
  //
  return par ( k ) ;
}
// ============================================================================

// ============================================================================
// 2D-POSITIVE 
// ============================================================================
// ============================================================================
// constructor from the order
// ============================================================================
Gaudi::Math::Positive3D::Positive3D 
( const unsigned short       Nx    ,
  const unsigned short       Ny    ,
  const unsigned short       Nz    ,
  const double               xmin  ,
  const double               xmax  ,
  const double               ymin  ,
  const double               ymax  ,
  const double               zmin  ,
  const double               zmax  )
  : m_bernstein (   Nx , Ny , Nz,  xmin , xmax , ymin , ymax , zmin , zmax ) 
  , m_sphere    ( ( Nx + 1 ) * ( Ny + 1 ) * ( Nz + 1 ) - 1 )
{
  updateBernstein () ;
}
// ============================================================================
// set k-parameter
// ============================================================================
bool Gaudi::Math::Positive3D::setPar 
( const unsigned int k     , 
  const double       value )
{
  //
  const bool update = m_sphere.setPhase ( k , value ) ;
  if ( !update ) { return false ; }   // no actual change 
  //
  return updateBernstein () ;
}
// =============================================================================
// update bernstein coefficients
// =============================================================================
bool Gaudi::Math::Positive3D::updateBernstein ()
{
  //
  bool update = false ;
  for ( unsigned int ix = 0 ; ix < m_sphere.nX() ; ++ix ) 
  { 
    const bool updated = m_bernstein.setPar ( ix , m_sphere.x2 ( ix ) ) ;
    update = updated || update ;  
  }
  //
  return update ;
}
// ============================================================================
// get the parameter value
// ============================================================================
double Gaudi::Math::Positive3D::par ( const unsigned int k ) const 
{ return m_sphere.phase ( k ) ; }
// ============================================================================
/*  get the integral over 3D-region           
 *  \f[ \int_{x_{min}}^{x_{max}}
 *      \int_{y_{min}}^{y_{max}} 
 *      \int_{z_{min}}^{z_{max}} 
 *        \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\mathrm{d}z\f] 
 */
// ============================================================================
double  Gaudi::Math::Positive3D::integral   () const { return 1 ; }
// ============================================================================
/* get the integral over 3D-region 
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
// ============================================================================
double Gaudi::Math::Positive3D::integral   
( const double xlow , const double xhigh , 
  const double ylow , const double yhigh ,
  const double zlow , const double zhigh ) const 
{ 
  return
    s_equal ( xlow  , xmin() ) && 
    s_equal ( xhigh , xmax() ) && 
    s_equal ( ylow  , ymin() ) && 
    s_equal ( yhigh , ymax() ) && 
    s_equal ( zlow  , zmin() ) && 
    s_equal ( zhigh , zmax() )  ? 1.0 : 
    m_bernstein.integral ( xlow , xhigh , ylow , yhigh , zlow , zhigh ) ; 
}
// ============================================================================



// ============================================================================
//                                                                      The END
// ============================================================================
