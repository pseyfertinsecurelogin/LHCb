// Include files
// ============================================================================
// Include files 
// ============================================================================
// STD & STL 
// ============================================================================
#include <vector>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/ToStream.h"
// ============================================================================
// LHCbMath 
// ============================================================================
#include "LHCbMath/LHCbMath.h"
#include "LHCbMath/BSpline.h"
// ============================================================================
/** @file 
 *  Implementation file for class Gaudi::Math::BSpline 
 *  @date 2014-11-12 
 *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
 */  
// ============================================================================
namespace 
{
  // ==========================================================================
  const unsigned short s_ulps = LHCb::Math::mULPS_double + 200 ;
  // ==========================================================================
  /** @var s_zero 
   *  comparison with zero
   *  @code
   *  double a = ... ;
   *  if( s_zero ( a ) ) { ... }  
   *  @endcode 
   *  @see LHCb::Math::Zero 
   *  @see Gaudi::Math::Lomont
   */
  const LHCb::Math::Zero<double>     s_zero  ;
  // ==========================================================================
  /** @var s_equal
   *  comparison of double values 
   *  @code
   *  double a = ... ;
   *  double b = ... ;
   *  if( s_equal ( a , b ) ) { ... }  
   *  @endcode 
   *  @see LHCb::Math::Equal_To 
   *  @see Gaudi::Math::Lomont
   */
  const LHCb::Math::Equal_To<double> s_equal ;
  // ==========================================================================
  /** @var s_less 
   *  sorting criteria for splines 
   *  @see LHCb::Math::NumLess 
   *  @see LHCb::Math::Equal_To 
   */
  const LHCb::Math::NumLess<double>  s_less  ;
}
// ============================================================================
// de Boor-Cox 
// ============================================================================
namespace 
{
  // ==========================================================================
  // find an interval such      knot[i]<= x< knot[i+1]  
  template <class IT> 
  inline IT find_i ( IT first , IT last , const double value ) 
  {
    if ( 2 > std::distance ( first , last ) 
         || value < * first  
         || value > *(last-1) ) { return last ; }
    //
    IT l = std::upper_bound ( first , last , value  ) ; // , s_less ) ;
    //
    return 
      last  ==  l ?   l : 
      value <  *l ? --l : 
      value == *l ? ++l : l ;
  }
  // ==========================================================================
  /// get the access to the knots 
  inline double  knot ( const std::vector<double>& knots , const int index ) 
  {
    return 
      0     >         index        ? knots.front () :  
      index >=  (int) knots.size() ? knots.back  () : knots[index] ;
  }
  // ==========================================================================
  /// get the coefficient for de Boor-Cox algorithm
  inline double _sik   ( const unsigned short       i     , 
                         const unsigned short       k     ,
                         const double               x     , 
                         const std::vector<double>& knots ) 
  {
    const double ti   =  knot ( knots , i   ) ;
    const double tik  =  knot ( knots , i+k ) ;
    return ti < tik ?  ( x - ti ) / ( tik - ti )  : 0.0 ;
  }
  // ==========================================================================
  /** calculate the value of the basic spline 
   *  @param i INPUT spline number 
   *  @param k INPUT spline order 
   *  @param x INPUT the argument 
   *  @param know INPUT vector of knots (assumed to be ordered) 
   */
  inline double bspline 
  ( const          short       i     ,  // the spline number   
    const unsigned short       k     ,  // the spline order 
    const double               x     ,  // the spline argument 
    const std::vector<double>& knots )  // knots are assumed to be ordered 
  {
    // check the range 
    if ( 2 > knots.size ()                      ) { return 0 ; }
    if ( x < knots.front()  || x > knots.back() ) { return 0 ; }
    //
    if ( x < knot ( knots , i         ) ) { return 0 ; }
    if ( x > knot ( knots , i + k + 1 ) ) { return 0 ; }
    //
    const double ki = knot ( knots , i ) ;
    // stopping criteria for recursion
    if ( 0 == k ) { return  ki <= x && x < knot ( knots , i + 1 )  ? 1 : 0 ; }
    //
    const double s1 = _sik ( i     , k , x , knots ) ;
    const double s2 = _sik ( i + 1 , k , x , knots ) ;
    //
    const bool null1 = s_zero  (     s1 ) ;
    const bool unit2 = s_equal ( 1 , s2 ) ;
    //
    return 
      //
      null1 && unit2 ?   0.0 : 
      //
      null1          ? ( 1 - s2 ) * bspline ( i + 1 , k - 1 , x , knots ) :
      //
      unit2          ?       s1   * bspline ( i     , k - 1 , x , knots ) :
      //
      bspline ( i      , k - 1 , x , knots ) *       s1   +
      bspline ( i + 1  , k - 1 , x , knots ) * ( 1 - s2 ) ;
  }
  // ==========================================================================
  /** calculate the value of the M-spline 
   *  @param i INPUT spline number 
   *  @param k INPUT spline order 
   *  @param x INPUT the argument 
   *  @param know INPUT vector of knots (assumed to be ordered) 
   */
  inline double mspline 
  ( const          short       i     ,  // the spline number   
    const unsigned short       k     ,  // the spline order 
    const double               x     ,  // the spline argument 
    const std::vector<double>& knots )  // knots are assumed to be ordered 
  {
    // check the range 
    if ( 2 > knots.size ()                      ) { return 0 ; }
    if ( x < knots.front()  || x > knots.back() ) { return 0 ; }
    //
    const double ki  = knot ( knots , i ) ;
    if ( x < ki  ) { return 0 ; } 
    //
    const double kio = knot ( knots , i + k + 1 ) ;
    if ( x > kio ) { return 0 ; }
    //
    // stopping criteria for recursion
    if ( 0 == k  ) { return  1.0 / ( kio - ki ) ; }
    //
    return   
      ( ( x   - ki ) * mspline ( i     , k - 1 , x , knots ) + 
        ( kio -  x ) * mspline ( i + 1 , k - 1 , x , knots ) ) 
      * ( k + 1 ) / ( k * ( kio - ki ) );
  }
  // ==========================================================================
  /** calculate the value of the I-spline 
   *  @param i INPUT spline number 
   *  @param k INPUT spline order 
   *  @param x INPUT the argument 
   *  @param know INPUT vector of knots (assumed to be ordered) 
   */
  inline double ispline 
  ( const          short       i     ,  // the spline number   
    const unsigned short       k     ,  // the spline order 
    const double               x     ,  // the spline argument 
    const std::vector<double>& knots )  // knots are assumed to be ordered 
  {
    // check the range 
    if ( 2 > knots.size ()                      ) { return 0 ; }
    if ( x < knots.front()  || x > knots.back() ) { return 0 ; }
    //
    double result = 0 ;
    for ( int j = i ; knot ( knots , j ) <= x ; ++j ) 
    { result += bspline ( j , k , x , knots ) ; }
    //
    return result ;
  }
  // ==========================================================================
  /// de-boor-cox algorithm
  inline double deboor
  ( const unsigned short       k      , 
    const unsigned short       order  , 
    const unsigned short       i      , 
    const double               x      ,
    const std::vector<double>& knots  , 
    const std::vector<double>& pars   ) 
  {
    //
    if ( 0 == k ) { return pars[i] ; }
    //
    const double ti  = knot ( knots , i                 ) ;
    const double tip = knot ( knots , i + 1 + order - k ) ;
    //
    if ( s_equal ( ti , tip ) ) { return 0 ; }
    //
    const double tau = ( x - ti ) / ( tip - ti ) ;
    return 
      deboor ( k-1 , order , i - 1 , x , knots , pars ) * ( 1 - tau ) +
      deboor ( k-1 , order , i     , x , knots , pars ) *       tau   ;  
  }
}
// ============================================================================
// The  Basic spline 
// ============================================================================
/** constructor from the list of knots and the order 
 *  vector of parameters will be calculated automatically 
 *  @param knots  non-empty vector of poinst/knots 
 *  @param order  the order of splines 
 *  - vector of points is not requires to be ordered 
 *  - duplicated knots will be ignored
 *  - min/max value will be used as interval boundaries 
 *  - extra knots will added at the end of interval 
 */
// ============================================================================
Gaudi::Math::BSpline::BSpline 
( const std::vector<double>& knots  ,
  const unsigned short       order  ) 
  : std::unary_function<double,double>() 
  , m_knots   ( knots  ) 
  , m_pars    () 
  , m_order   ( order  ) 
  , m_inner   ( 0      )  
  , m_xmin    ( 0 ) 
  , m_xmax    ( 1 )
    //
  , m_jlast   ( 0      ) 
  , m_pars_i  ()
  , m_knots_i ()
{
  //
  std::stable_sort ( m_knots.begin() , m_knots.end() , s_less ) ; 
  std::vector<double>::iterator it = std::unique ( m_knots.begin() , m_knots.end() , s_equal );
  m_knots.erase ( it , m_knots.end() ) ;
  //
  if ( m_knots.size () < 2 ) { throw GaudiException 
      ("Vector of knots is too short", "Gaudi::Math::BSpline" , StatusCode(810) ) ; } 
  //
  m_inner = m_knots.size   () - 2 ;
  // specify vector of parameters 
  m_pars = std::vector<double>( m_inner + m_order + 1 , 0.0 ) ;
  //
  m_xmin = m_knots.front () ;
  m_xmax = m_knots.back  () ;
  //
  while ( m_knots.size() + 1 < m_pars.size() + m_order + 1 ) 
  {
    m_knots.insert ( m_knots.begin () , m_xmin ) ;
    m_knots.insert ( m_knots.end   () , m_xmax ) ;    
  }
  //
  // integration cache:
  m_pars_i .resize( m_pars.size  () + 1 ) ;
  m_knots_i.resize( m_knots.size () + 2 ) ;
  std::copy ( m_knots.begin() , m_knots.end() , m_knots_i.begin() + 1 ) ;
  m_knots_i.front () = m_xmin ;
  m_knots_i.back  () = m_xmax ;
}
// ======================================================================
/*  Constructor from the list of knots and list of parameters 
 *  The spline order will be calculated automatically 
 *  @param knots  non-empty vector of poinst/knots 
 *  @param pars   non-empty vector of parameters 
 *  - vector of points is not requires to be ordered 
 *  - min/max value will be used as interval boundaries 
 *  - duplicated knots will be ignored
 *  - extra knots will added at the end of interval 
 */
// ======================================================================
Gaudi::Math::BSpline::BSpline 
( const std::vector<double>& knots  ,
  const std::vector<double>& pars   ) 
  : std::unary_function<double,double>() 
    //
  , m_knots ( knots  ) 
  , m_pars  ( pars   ) 
  , m_order ( 1      ) 
  , m_inner ( 0      )  
  , m_xmin  ( 0      ) 
  , m_xmax  ( 1      )
  , m_jlast ( 0      ) 
    //
{
  //
  std::stable_sort ( m_knots.begin() , m_knots.end() , s_less ) ; 
  std::vector<double>::iterator it = std::unique ( m_knots.begin() , m_knots.end() , s_equal );
  m_knots.erase ( it , m_knots.end() ) ;
  //
  if      ( m_knots.size()     <  2              ) { throw GaudiException
      ("Vector of knots is too short", "Gaudi::Math::BSpline" , StatusCode(812) ) ; } 
  else if ( m_pars .size() + 1 <  m_knots.size() ) { throw GaudiException
      ("Vector of pars  is too short", "Gaudi::Math::BSpline" , StatusCode(813) ) ; }
  //
  m_inner = m_knots.size () - 2 ;
  m_order = m_pars.size  () - m_inner - 1 ;
  //
  m_xmin = m_knots.front () ;
  m_xmax = m_knots.back  () ;
  //
  while ( m_knots.size() + 1 < m_pars.size() + m_order + 1 ) 
  {
    m_knots.insert ( m_knots.begin () , m_xmin ) ;
    m_knots.insert ( m_knots.end   () , m_xmax ) ;    
  }
  //
  // integration cache:
  m_pars_i .resize ( m_pars.size  () + 1 ) ;
  m_knots_i.resize ( m_knots.size () + 2 ) ;
  std::copy ( m_knots.begin() , m_knots.end() , m_knots_i.begin() + 1 ) ;
  m_knots_i.front () = m_xmin ;
  m_knots_i.back  () = m_xmax ;
}
// ============================================================================
/* Constructor for uniform binning 
 *  @param xmin   low  edge of spline interval 
 *  @param xmax   high edge of spline interval 
 *  @param inner  number of inner points in   (xmin,xmax) interval
 *  @param order  the degree of spline 
 */
// ============================================================================
Gaudi::Math::BSpline::BSpline
( const double         xmin    ,  
  const double         xmax    , 
  const unsigned short inner   , // number of inner points  
  const unsigned short order   ) 
  : std::unary_function<double,double>() 
    //
  , m_knots () 
  , m_pars  ( order + inner + 1 , 0.0 ) 
  , m_order ( order )  
  , m_inner ( inner )  
  , m_xmin  ( std::min ( xmin  , xmax ) ) 
  , m_xmax  ( std::max ( xmin  , xmax ) ) 
  , m_jlast ( 0      ) 
{
  //
  const double dx = ( m_xmax - m_xmin ) ;
  for ( unsigned short i = 1 ; i <= inner ; ++i ) 
  { m_knots .push_back ( m_xmin + dx * double ( i )  / ( inner + 1 ) ) ; }
  //
  while ( m_knots.size() + 1 < m_pars.size() + m_order + 1 ) 
  {
    m_knots.insert ( m_knots.begin () , m_xmin ) ;
    m_knots.insert ( m_knots.end   () , m_xmax ) ;    
  }
  //
  // integration cache:
  m_pars_i .resize( m_pars.size  () + 1 ) ;
  m_knots_i.resize( m_knots.size () + 2 ) ;
  std::copy ( m_knots.begin() , m_knots.end() , m_knots_i.begin() + 1 ) ;
  m_knots_i.front () = m_xmin ;
  m_knots_i.back  () = m_xmax ;
}
// ============================================================================
// move constructor 
// ============================================================================
Gaudi::Math::BSpline::BSpline( Gaudi::Math::BSpline&&  right ) 
  : std::unary_function<double,double>( std::move ( right ) ) 
  , m_knots   ( std::move ( right.m_knots   ) ) 
  , m_pars    ( std::move ( right.m_pars    ) ) 
  , m_order   ( std::move ( right.m_order   ) ) 
  , m_inner   ( std::move ( right.m_inner   ) ) 
  , m_xmin    ( std::move ( right.m_xmin    ) ) 
  , m_xmax    ( std::move ( right.m_xmax    ) ) 
  , m_jlast   ( std::move ( right.m_jlast   ) ) 
  , m_pars_i  ( std::move ( right.m_pars_i  ) )  
  , m_knots_i ( std::move ( right.m_knots_i ) )  
{}
// ============================================================================
/// assignement move operator 
// ============================================================================
Gaudi::Math::BSpline& Gaudi::Math::BSpline::operator=
(       Gaudi::Math::BSpline&& right ) 
{
  if ( &right == this ) { return *this ; }
  //
  m_knots   = std::move ( right.m_knots   ) ;
  m_pars    = std::move ( right.m_pars    ) ;
  m_order   = std::move ( right.m_order   ) ;
  m_inner   = std::move ( right.m_inner   ) ;
  m_xmin    = std::move ( right.m_xmin    ) ;
  m_xmax    = std::move ( right.m_xmax    ) ;
  m_jlast   = std::move ( right.m_jlast   ) ;
  m_pars_i  = std::move ( right.m_pars_i  ) ;
  m_knots_i = std::move ( right.m_knots_i ) ;
  //
  return *this ;
}
// ============================================================================
// is it a increasing function?
// ============================================================================
bool Gaudi::Math::BSpline::increasing   () const 
{
  if ( m_pars.size() <= 1 ) { return true ; }
  for ( std::vector<double>::const_iterator it = m_pars.begin() + 1 ; 
        m_pars.end() != it ; ++it ) 
  { if (  (*(it-1)) > (*it) && !s_equal ( *(it-1) , *it ) ) { return false ; } }
  return true ;
}
// ============================================================================
// is it a decreasing function?
// ============================================================================
bool Gaudi::Math::BSpline::decreasing   () const 
{
  if ( m_pars.size() <= 1 ) { return true ; }
  for ( std::vector<double>::const_iterator it = m_pars.begin() + 1 ; 
        m_pars.end() != it ; ++it ) 
  { if (  (*(it-1)) < (*it) && !s_equal ( *(it-1) , *it ) ) { return false ; } }
  return true ;
}
// ============================================================================
// is it a constant function?
// ============================================================================
bool Gaudi::Math::BSpline::constant () const 
{
  //
  if ( m_pars.size() <= 1 ) { return true ; }
  for ( std::vector<double>::const_iterator it = m_pars.begin() + 1 ; 
        m_pars.end() != it ; ++it ) 
  { if ( !s_equal ( *(it-1) ,  *it ) ) { return false ; } }
  //
  return true ;
}
// ============================================================================
// simple  manipulations with bernstein polynoms: scale it! 
// ============================================================================
Gaudi::Math::BSpline&
Gaudi::Math::BSpline::operator*=( const double a ) 
{
  LHCb::Math::scale ( m_pars  , a ) ;
  return *this ;
}
// ============================================================================
// simple  manipulations with bernstein polynoms: scale it! 
// ============================================================================
Gaudi::Math::BSpline&
Gaudi::Math::BSpline::operator/=( const double a ) 
{
  LHCb::Math::scale ( m_pars  , 1.0/a ) ;
  return *this ;
}
// ============================================================================
// simple  manipulations with bernstein polynoms: shift it! 
// ============================================================================
Gaudi::Math::BSpline&
Gaudi::Math::BSpline::operator+=( const double a ) 
{
  LHCb::Math::shift ( m_pars  , a ) ;
  return *this ;
}
// ============================================================================
// simple  manipulations with bernstein polynoms: shift it! 
// ============================================================================
Gaudi::Math::BSpline&
Gaudi::Math::BSpline::operator-=( const double a ) 
{
  LHCb::Math::shift ( m_pars  , -a ) ;
  return *this ;
}
// ============================================================================
// set k-parameter
// ============================================================================
bool Gaudi::Math::BSpline::setPar ( const unsigned short k , const double value ) 
{
  if ( k >= m_pars.size()          ) { return false ; }
  if ( s_equal ( m_pars[k] , value ) ) { return false ; }
  //
  m_pars[k ] = value ;
  return true ;
}
// ============================================================================
// get the value of the B-spline  i at point x 
// ============================================================================
double Gaudi::Math::BSpline::bspline ( const short  i , 
                                       const double x )  const 
{ 
  return  
    x < m_xmin || x > m_xmax ? 0.0 :
    ::bspline ( i , m_order , x , m_knots ) ;
}
// ============================================================================
// get the value of the B-spline  (i,k) at point x
// ============================================================================
double Gaudi::Math::BSpline::bspline ( const          short i , 
                                       const unsigned short k , 
                                       const double         x )  const 
{ 
  return 
    x < m_xmin || x > m_xmax ? 0.0 :
    ::bspline ( i , k , x , m_knots ) ;
}
// ============================================================================
// get the value of the M-spline  i at point x
// ============================================================================
double Gaudi::Math::BSpline::mspline ( const short  i , 
                                       const double x )  const 
{ 
  return 
    x < m_xmin || x > m_xmax ? 0.0 :
    ::mspline ( i , m_order , x , m_knots ) ; 
}
// ============================================================================
// get the value of the M-spline  (i,k) at point x 
// ============================================================================
double Gaudi::Math::BSpline::mspline ( const          short i , 
                                       const unsigned short k , 
                                       const double         x )  const 
{ 
  return 
    x < m_xmin || x > m_xmax ? 0.0 :
    ::mspline ( i , k , x , m_knots ) ; 
}
// ============================================================================
// get the value of the I-spline  i at point x 
// ============================================================================
double Gaudi::Math::BSpline::ispline ( const short  i , 
                                       const double x )  const 
{
  return 
    x < m_xmin || m_xmax > x  ? 0.0 :
    ::ispline ( i , m_order , x , m_knots ) ; 
}
// ============================================================================
// get the value of the M-spline  (i,k) at point x 
// ============================================================================
double Gaudi::Math::BSpline::ispline ( const          short i , 
                                       const unsigned short k , 
                                       const double         x )  const 
{ 
  return 
    x < m_xmin || x > m_xmax ? 0.0 :
    ::ispline ( i , k , x , m_knots ) ; 
}
// ============================================================================
// get the value
// ============================================================================
double Gaudi::Math::BSpline::operator () ( const double x ) const
{
  if      ( x < m_xmin || x > m_xmax ) { return 0 ; }             // RETURN
  //
  // endpoints 
  //
  if      ( s_equal ( x , m_xmin ) ) { return m_pars.front () ; }
  else if ( s_equal ( x , m_xmax ) ) { return m_pars.back  () ; }
  //
  // const double arg =  
  //   !s_equal ( x , m_xmax ) ? x :
  //   0 <= m_xmax             ? 
  //   Gaudi::Math::next_double ( m_xmax , -s_ulps ) :
  //   Gaudi::Math::next_double ( m_xmax ,  s_ulps ) ;
  //
  const double arg = x ;
  //
  // find the proper "j"
  //
  // 1) try from jlast
  if ( x <  m_knots[ m_jlast ] || x >= m_knots[ m_jlast + 1 ] ) 
  {
    m_jlast = find_i ( m_knots.begin () , 
                       m_knots.end   () , arg ) - m_knots.begin() ;
  }
  //
  // straightforward calculations: 
  // double result = 0 ;
  // for ( unsigned short j = m_jlast - m_order ; j <= m_jlast ; ++j ) 
  // { result += bspline ( j , arg ) * m_pars[j] ; }
  //
  // use de Boor-Cox algorithm:
  return deboor ( m_order , m_order , m_jlast , arg , m_knots , m_pars ) ;
  //
}
// ============================================================================
namespace 
{
  // ==========================================================================
  inline double   
  _spline_integral_ 
  ( const std::vector<double>& pars  , 
    const std::vector<double>& knots ,
    const unsigned short       order ) 
  {
    double             result = 0 ;
    for ( unsigned short i = 0 ; i < pars.size() ; ++i ) 
    { result +=  pars[i] * ( knots[ i + order + 1 ] - knots [ i ] ) ; }
    //
    return result / ( order + 1 ) ;
  }
  // ==========================================================================
}
// ============================================================================
// get the integral between xmin and xmax
// ============================================================================
double Gaudi::Math::BSpline::integral () const
{
  //
  return _spline_integral_ ( m_pars , m_knots , m_order ) ;
  //
  // double             result = 0 ;
  // for ( unsigned short i = 0 ; i < m_pars.size() ; ++i ) 
  // { result +=  m_pars[i] * ( m_knots[ i + m_order + 1 ] - m_knots [ i ] ) ; }
  //
  // return result / ( m_order + 1 ) ;
}
// ============================================================================
// get the integral between low and high 
// ============================================================================
double Gaudi::Math::BSpline::integral 
( const double low  , 
  const double high ) const 
{
  //
  if      ( high < low                      ) { return - integral ( high   , low    ) ; }
  else if ( s_equal ( low , high )          ) { return 0 ; }
  else if ( low  <  m_xmin                  ) { return   integral ( m_xmin , high   ) ; }
  else if ( high >  m_xmax                  ) { return   integral ( low    , m_xmax ) ; }
  else if ( high <= m_xmin || low >= m_xmax ) { return 0 ; }
  else if ( s_equal ( low  , m_xmin ) && s_equal ( high , m_xmax ) ) { return integral() ; }
  //
  const double xhigh =
    !s_equal ( high , m_xmax ) ? high : 
    0 <= m_xmax                ? 
    Gaudi::Math::next_double ( m_xmax , -s_ulps ) :
    Gaudi::Math::next_double ( m_xmax ,  s_ulps ) ;
  //
  // make the integration:
  m_pars_i[0] = 0 ;
  for ( unsigned int i = 0 ; i < m_pars.size() ; ++i ) 
  { m_pars_i[i+1] = m_pars_i[i] + m_pars[i] * ( m_knots[ i + m_order + 1 ] - m_knots [ i ] ) ; }
  //
  const short  jL = find_i ( m_knots_i.begin () , m_knots_i.end () ,  low  ) - m_knots_i.begin() ;  
  const short  jH = find_i ( m_knots_i.begin () , m_knots_i.end () , xhigh ) - m_knots_i.begin() ;
  //
  const double rL = deboor ( m_order + 1 , m_order + 1 , jL ,  low  , m_knots_i , m_pars_i ) ;
  const double rH = deboor ( m_order + 1 , m_order + 1 , jH , xhigh , m_knots_i , m_pars_i ) ;
  //
  return  ( rH - rL ) / ( m_order + 1 ) ;
}
// ============================================================================
// get the integral   as function object 
// ============================================================================
Gaudi::Math::BSpline 
Gaudi::Math::BSpline::indefinite_integral ( const double C ) const 
{
  // create the object 
  BSpline result ( m_xmin , m_xmax  , m_inner  , m_order + 1 ) ;
  // fill it properly 
  std::copy ( m_knots.begin () , m_knots.end  () , result.m_knots  .begin() + 1 ) ;
  std::copy ( m_knots.begin () , m_knots.end  () , result.m_knots_i.begin() + 2 ) ;
  //
  *( result.m_knots  .begin()     ) = m_knots.front () ;
  *( result.m_knots  .end  () - 1 ) = m_knots.back  () ;
  *( result.m_knots_i.begin()     ) = m_knots.front () ;
  *( result.m_knots_i.begin() + 1 ) = m_knots.front () ;
  *( result.m_knots_i.end  () - 1 ) = m_knots.back  () ;
  *( result.m_knots_i.end  () - 2 ) = m_knots.back  () ;
  //
  result.m_knots_i [0] = m_knots[0] ;
  result.m_knots_i [1] = m_knots[0] ;
  //
  result.m_pars[0] = C ;
  for ( unsigned int i = 0 ; i < m_pars.size() ; ++i ) 
  { result.m_pars[i+1] = result.m_pars[i] 
      + m_pars[i] * ( m_knots[ i + m_order + 1 ] - m_knots [ i ] ) / ( m_order + 1 ) ; }
  //
  return result ;
}
// ============================================================================
// get the derivative at point "x" 
// ============================================================================
double Gaudi::Math::BSpline::derivative ( const double x   ) const 
{
  //
  if      ( x < m_xmin || x > m_xmax || 0 == m_order ) { return 0 ; }
  //
  const double arg = 
    !s_equal ( x , m_xmax ) ? x :
    0 <= m_xmax             ? 
    Gaudi::Math::next_double ( m_xmax , -s_ulps ) :
    Gaudi::Math::next_double ( m_xmax ,  s_ulps ) ;
  //
  // make the differentiation 
  //
  m_pars_i[0] = m_pars[0]  ;
  for ( unsigned int i = 1  ; i < m_pars.size() ; ++i ) 
  {m_pars_i[i] = ( m_pars[i] - m_pars[i-1] ) / ( m_knots [ i + m_order ] - m_knots [ i ] ) ; }
  //
  // try from jlast
  if ( arg <  m_knots[ m_jlast ] || arg >= m_knots[ m_jlast + 1 ] ) 
  { m_jlast = find_i ( m_knots.begin () , m_knots.end   () , arg ) - m_knots.begin() ; }
  //
  const double r = deboor  ( m_order - 1 , m_order - 1 , m_jlast , arg , m_knots , m_pars_i ) ;
  //
  return r * m_order ;
}
// ============================================================================
// get the derivative as function object 
// ============================================================================
Gaudi::Math::BSpline Gaudi::Math::BSpline::derivative () const 
{
  // create the object 
  BSpline result ( m_xmin , m_xmax , m_inner , 0 < m_order ? m_order -1 : 0  ) ;
  if ( 0 == m_order ) { return result ; }
  //
  std::copy ( m_knots.begin () + 1 , m_knots.end  () - 1  , result.m_knots . begin () ) ;
  //
  std::vector<double> _pars ( m_pars.size  () ) ;
  _pars[0] = m_pars[0] * m_order ;
  for ( unsigned int i = 1  ; i < m_pars.size() ; ++i ) 
  { _pars[i] = ( m_pars[i] - m_pars[i-1] ) * m_order / 
      ( m_knots [ i + m_order ] - m_knots [ i ] ) ; }
  //
  std::copy ( _pars.begin() + 1 , _pars.end() , result.m_pars.begin() ) ;
  //
  return result ;
}
// ============================================================================
// ============================================================================
// POSITIVE SPLINE 
// ============================================================================
// ============================================================================
/* constructor from the list of knots and the order 
 *  vector of parameters will be calculated automatically 
 *  @param points non-empty vector of poinst/knots 
 *  @param order  the order of splines 
 *  - vector of points is not requires to be ordered 
 *  - duplicated knots will be ignored
 *  - min/max value will be used as interval boundaries 
 */
// ============================================================================
Gaudi::Math::PositiveSpline::PositiveSpline 
( const std::vector<double>& points ,
  const unsigned short       order  ) 
  : std::unary_function<double,double> () 
  , m_bspline ( points , order ) 
  , m_sphere  ( 1 ) 
{
  //
  if ( 2 > m_bspline.npars() ) { throw GaudiException
      ( "At least two spline parameters are required" , 
        "Gaudi::Math::PositiveSpline"                 , StatusCode(814,true) ) ; }
  //
  m_sphere = Gaudi::Math::NSphere( m_bspline.npars() - 1 , true ) ;  
  updateCoefficients () ;
}
// ============================================================================
/*  Constructor from the list of knots and list of parameters 
 *  The spline order will be calculated automatically 
 *  @param points non-empty vector of poinst/knots 
 *  @param pars   non-empty vector of parameters 
 *  - vector of points is not requires to be ordered 
 *  - duplicated knots will be ignored
 *  - min/max value will be used as interval boundaries 
 */
// ============================================================================
Gaudi::Math::PositiveSpline::PositiveSpline 
( const std::vector<double>& points    ,
  const std::vector<double>& pars      ) 
  : std::unary_function<double,double> () 
  , m_bspline ( points , std::vector<double>( pars.size() + 1 , 0 )  ) 
  , m_sphere  ( pars   , true  ) 
{
  //
  if ( 2 > m_bspline.npars() ) { throw GaudiException
      ( "At least two spline parameters are required" , 
        "Gaudi::Math::PositiveSpline"                 , StatusCode(814,true) ) ; }
  //
  m_sphere = Gaudi::Math::NSphere( m_bspline.npars() - 1 , true ) ;  
  updateCoefficients () ;
}
// ============================================================================
/*  Constructor for uniform binning 
 *  @param xmin   low  edge of spline interval 
 *  @param xmax   high edge of spline interval 
 *  @param inner  number of inner points in   (xmin,xmax) interval
 *  @param order  the degree of splline 
 */
// ============================================================================
Gaudi::Math::PositiveSpline::PositiveSpline 
( const double         xmin  ,  
  const double         xmax  ,  
  const unsigned short inner ,   // number of inner points 
  const unsigned short order ) 
  : std::unary_function<double,double> ()
  , m_bspline ( xmin , xmax , inner , order ) 
  , m_sphere  ( 1 , false ) 
{
  //
  if ( 2 > m_bspline.npars() ) { throw GaudiException
      ( "At least two spline parameters are required" , 
        "Gaudi::Math::PositiveSpline"                 , StatusCode(814,true) ) ; }
  //
  m_sphere = Gaudi::Math::NSphere( m_bspline.npars() - 1 , true ) ;  
  updateCoefficients() ;
}
// ============================================================================
// constructor fomr the basic spline 
// ============================================================================
Gaudi::Math::PositiveSpline::PositiveSpline 
( const Gaudi::Math::BSpline& spline ) 
  : std::unary_function<double,double> ()
  , m_bspline ( spline    ) 
  , m_sphere  ( 1 , false ) 
{
  //
  if ( 2 > m_bspline.npars() ) { throw GaudiException
      ( "At least two spline parameters are required" , 
        "Gaudi::Math::PositiveSpline"                 , StatusCode(814,true) ) ; }
  //
  m_sphere = Gaudi::Math::NSphere( m_bspline.npars() - 1 , true ) ;  
  updateCoefficients() ;
}
// ============================================================================
Gaudi::Math::PositiveSpline::~PositiveSpline(){}
// ============================================================================
// update coefficients  
// ============================================================================
bool Gaudi::Math::PositiveSpline::updateCoefficients  () 
{
  //
  bool   update = false ;
  //
  // get sphere coefficients 
  std::vector<double> v( m_sphere.nX() ) ;
  for ( unsigned short ix = 0 ; ix < m_sphere.nX() ; ++ix ) 
  {  v [ix] = m_sphere.x2 ( ix ) ; }
  //
  const double isum = 1.0 / 
    _spline_integral_ ( v , m_bspline.knots() , m_bspline.order() ) ;
  //
  for ( unsigned short ix = 0 ; ix < m_sphere.nX() ; ++ix ) 
  { 
    const bool updated = m_bspline.setPar ( ix , v [ix] * isum ) ; 
    update = updated || update ;
  }
  //
  return update ;
}
// ============================================================================
// set k-parameter
// ============================================================================
bool Gaudi::Math::PositiveSpline::setPar 
( const unsigned short k , const double value ) 
{
  //
  const bool update = m_sphere.setPhase ( k , value ) ;
  if ( !update ) { return false ; }   // no actual change 
  //
  return updateCoefficients () ;
}
// ============================================================================
// get the integral between xmin and xmax
// ============================================================================
double  Gaudi::Math::PositiveSpline::integral   () const { return 1 ; }
// ============================================================================
// get the integral between low and high 
// ============================================================================
double  Gaudi::Math::PositiveSpline::integral   
( const double low  , 
  const double high ) const 
{ 
  return 
    s_equal ( low  , xmin() ) && s_equal ( high , xmax() ) ? 1 :
    m_bspline.integral   ( low , high ) ; 
}
// ============================================================================


// ============================================================================
// MONOTONIC SPLINE 
// ============================================================================
// ============================================================================
/* constructor from the list of knots and the order 
 *  vector of parameters will be calculated automatically 
 *  @param points non-empty vector of poinst/knots 
 *  @param order  the order of splines 
 *  - vector of points is not requires to be ordered 
 *  - duplicated knots will be ignored
 *  - min/max value will be used as interval boundaries 
 */
// ============================================================================
Gaudi::Math::MonothonicSpline::MonothonicSpline
( const std::vector<double>& points      ,
  const unsigned short       order       , 
  const bool                 increasing  )
  : Gaudi::Math::PositiveSpline ( points , order ) 
  , m_increasing                ( increasing ) 
{
  updateCoefficients () ;
}
// ============================================================================
/*  Constructor from the list of knots and list of parameters 
 *  The spline order will be calculated automatically 
 *  @param points non-empty vector of poinst/knots 
 *  @param pars   non-empty vector of parameters 
 *  - vector of points is not requires to be ordered 
 *  - duplicated knots will be ignored
 *  - min/max value will be used as interval boundaries 
 */
// ============================================================================
Gaudi::Math::MonothonicSpline::MonothonicSpline
( const std::vector<double>& points     ,
  const std::vector<double>& pars       ,
  const bool                 increasing ) 
  : Gaudi::Math::PositiveSpline ( points , pars ) 
  , m_increasing                ( increasing    ) 
{
  updateCoefficients () ;
}
// ============================================================================
/*  Constructor for uniform binning 
 *  @param xmin   low  edge of spline interval 
 *  @param xmax   high edge of spline interval 
 *  @param inner  number of inner points in   (xmin,xmax) interval
 *  @param order  the degree of splline 
 */
// ============================================================================
Gaudi::Math::MonothonicSpline::MonothonicSpline
( const double         xmin       ,  
  const double         xmax       , 
  const unsigned short inner      ,   // number of inner points 
  const unsigned short order      , 
  const bool           increasing ) 
  : Gaudi::Math::PositiveSpline ( xmin , xmax  , inner ,order ) 
  , m_increasing                ( increasing    ) 
{
  updateCoefficients () ;
}
// ============================================================================
// constructor from the basic spline 
// ============================================================================
Gaudi::Math::MonothonicSpline::MonothonicSpline
( const Gaudi::Math::PositiveSpline& spline     , 
  const bool                         increasing ) 
  : Gaudi::Math::PositiveSpline ( spline        ) 
  , m_increasing                ( increasing    ) 
{
  updateCoefficients () ;
}
// ============================================================================
// constructor from the basic spline 
// ============================================================================
Gaudi::Math::MonothonicSpline::MonothonicSpline
( const Gaudi::Math::BSpline&        spline     , 
  const bool                         increasing ) 
  : Gaudi::Math::PositiveSpline ( spline        ) 
  , m_increasing                ( increasing    ) 
{
  updateCoefficients () ;
}
// ============================================================================
// destructor
// ============================================================================
Gaudi::Math::MonothonicSpline::~MonothonicSpline(){}
// ============================================================================
// update coefficients  
// ============================================================================
bool Gaudi::Math::MonothonicSpline::updateCoefficients  () 
{
  //
  bool   update = false ;
  //
  // get sphere coefficients 
  std::vector<double> v ( m_sphere.nX() ) ;
  for ( unsigned short ix = 0 ; ix < m_sphere.nX() ; ++ix ) 
  { v[ix] = m_sphere.x2 ( ix ) ; }
  //
  // integrate them and to get new coefficients
  if   ( m_increasing ) { std::partial_sum ( v. begin() , v. end() ,  v. begin() ) ; }
  else                  { std::partial_sum ( v.rbegin() , v.rend() ,  v.rbegin() ) ; }
  //
  const double isum = 1.0 / 
    _spline_integral_ ( v , m_bspline.knots() , m_bspline.order() ) ;
  //
  for ( unsigned short ix = 0 ; ix < m_sphere.nX() ; ++ix ) 
  { 
    const bool updated = m_bspline.setPar ( ix , v [ix] * isum ) ; 
    update = updated || update ;
  }
  //
  return update ;
}
// ============================================================================
// ============================================================================
// COVEX ONLY  SPLINE 
// ============================================================================
// ============================================================================
/* constructor from the list of knots and the order 
 *  vector of parameters will be calculated automatically 
 *  @param points non-empty vector of poinst/knots 
 *  @param order  the order of splines 
 *  - vector of points is not requires to be ordered 
 *  - duplicated knots will be ignored
 *  - min/max value will be used as interval boundaries 
 */
// ============================================================================
Gaudi::Math::ConvexOnlySpline::ConvexOnlySpline
( const std::vector<double>& points      ,
  const unsigned short       order       , 
  const bool                 convex      )
  : Gaudi::Math::PositiveSpline ( points , order ) 
  , m_convex                    ( convex ) 
{
  if ( 2 > this->order() ) { throw GaudiException
      ( "Degree of spline must be at least 2" , 
        "Gaudi::Math::ConvexOnlySpline"       , StatusCode(815,true) ) ; }
  updateCoefficients () ;
}
// ============================================================================
/*  Constructor from the list of knots and list of parameters 
 *  The spline order will be calculated automatically 
 *  @param points non-empty vector of poinst/knots 
 *  @param pars   non-empty vector of parameters 
 *  - vector of points is not requires to be ordered 
 *  - duplicated knots will be ignored
 *  - min/max value will be used as interval boundaries 
 */
// ============================================================================
Gaudi::Math::ConvexOnlySpline::ConvexOnlySpline
( const std::vector<double>& points     ,
  const std::vector<double>& pars       ,
  const bool                 convex     ) 
  : Gaudi::Math::PositiveSpline ( points , pars ) 
  , m_convex                    ( convex    ) 
{
  if ( 2 > order() ) { throw GaudiException
      ( "Degree of spline must be at least 2" , 
        "Gaudi::Math::ConvexOnlySpline"       , StatusCode(815,true) ) ; }
  updateCoefficients () ;
}
// ============================================================================
/*  Constructor for uniform binning 
 *  @param xmin   low  edge of spline interval 
 *  @param xmax   high edge of spline interval 
 *  @param inner  number of inner points in   (xmin,xmax) interval
 *  @param order  the degree of splline 
 */
// ============================================================================
Gaudi::Math::ConvexOnlySpline::ConvexOnlySpline
( const double         xmin      ,  
  const double         xmax      , 
  const unsigned short inner     ,   // number of inner points 
  const unsigned short order     , 
  const bool           convex    ) 
  : Gaudi::Math::PositiveSpline ( xmin , xmax  , inner , order ) 
  , m_convex                    ( convex    ) 
{
  if ( this->order() < 2 ) { throw GaudiException
      ( "Degree of spline must be at least 2" , 
        "Gaudi::Math::ConvexOnlySpline"       , StatusCode(815,true) ) ; }
  updateCoefficients () ;
}
// ============================================================================
// constructor from the basic spline 
// ============================================================================
Gaudi::Math::ConvexOnlySpline::ConvexOnlySpline
( const Gaudi::Math::PositiveSpline& spline     , 
  const bool                         convex     ) 
  : Gaudi::Math::PositiveSpline ( spline        ) 
  , m_convex                    ( convex    ) 
{
  if ( 2 > order() ) { throw GaudiException
      ( "Degree of spline must be at least 2" , 
        "Gaudi::Math::ConvexOnlySpline"       , StatusCode(815,true) ) ; }
  updateCoefficients () ;
}
// ============================================================================
// constructor from the basic spline 
// ============================================================================
Gaudi::Math::ConvexOnlySpline::ConvexOnlySpline
( const Gaudi::Math::BSpline&        spline     , 
  const bool                         convex     ) 
  : Gaudi::Math::PositiveSpline ( spline        ) 
  , m_convex                    ( convex    ) 
{
  if ( 2 > order() ) { throw GaudiException
      ( "Degree of spline must be at least 2" , 
        "Gaudi::Math::ConvexOnlySpline"       , StatusCode(815,true) ) ; }
  updateCoefficients () ;
}
// ============================================================================
// destructor
// ============================================================================
Gaudi::Math::ConvexOnlySpline::~ConvexOnlySpline(){}
// ============================================================================
// update coefficients  
// ============================================================================
bool Gaudi::Math::ConvexOnlySpline::updateCoefficients  () 
{
  //
  if  ( order() < 2 ) { return Gaudi::Math::PositiveSpline::updateCoefficients() ; }
  //
  bool   update = false ;
  //
  // get sphere coefficients 
  std::vector<double>  v ( m_sphere.nX() ) ;
  const unsigned short vs = v.size()     ;
  const unsigned short o  = order() ;  
  //
  if ( !m_convex ) 
  {
    const std::array<double,2> a = { { m_sphere.x2(0) , m_sphere.x2(1) } };
    for ( unsigned short ix = 2 ; ix < vs ; ++ix ) { v[ix] = m_sphere.x2 ( ix ) ; }
    //
    // integrate them and to get new coefficients
    std::partial_sum ( v.  begin() + 2 , v.  end() , v.  begin() + 2 ) ; 
    //
    for ( unsigned int i = 1 ; i < v.size()  ; ++i ) 
    { v[i] = v[i-1] + v[i] * ( knot_i (  i + o  + 1 ) - knot_i ( i )  ) / o ; }
    //
    const double last = v.back() ;
    for ( unsigned int i = 0 ; i < v.size() ; ++i ) { v[i] = last - v[i] ; }
    //
    const double v1 = a[0] - v.front() ;
    const double v2 = a[1] - v.back()  ;
    //
    for ( unsigned int j = 0 ; j < v.size()  ; ++j ) 
    { 
      double vj = 0 ;
      for ( unsigned short i = j + 1 ; i < j + o + 1 ; ++i ) { vj += knot_i ( i ) ; }
      v[j] += v1 + vj * ( v2 - v1 ) / o ;
    }
  }
  else 
  {
    const std::array<double,3> a = { { m_sphere.x2(0) , 
                                       m_sphere.x2(1) , 
                                       m_sphere.x2(2) } };
    for ( unsigned short ix = 3 ; ix < vs ; ++ix ) { v[ix] = m_sphere.x2 ( ix ) ; }
    // integrate them and to get new coefficients
    std::partial_sum ( v.  begin() + 3 , v.  end() , v.  begin() + 3 ) ; 
    //
    for ( unsigned int i = 3 ; i < v.size()  ; ++i ) 
    { v[i] = v[i-1] + v[i] * ( knot_i (  i + o  + 1 ) - knot_i ( i )  ) / o ; }
    //
    const double a0 = a[0] ;
    const double a2 = a[2] ;
    const double a1_min = -1*std::sqrt ( a0 * a2 ) ;
    const double a1_max = 0.5 * ( a0 + a2 ) ;
    //
    const double a1 = a1_min + a[1] * ( a1_max - a1_min ) ;
    //
    const double c0 = a0         ;
    const double c1 = 2*(a1-a0)  ;
    const double c2 = a0+a2-2*a1 ; 
    //
    for ( unsigned int j = 0 ; j < v.size()  ; ++j ) 
    { 
      double v1 = 0 ;
      for ( unsigned short   i = j + 1 ; i < j + o + 1 ; ++i ) { v1 += knot_i ( i ) ; }
      double v2 = 0 ;
      for ( unsigned short   i = j + 1 ; i < j + o     ; ++i ) 
      { for ( unsigned short k = i + 1 ; k < j + o + 1 ; ++k ) 
        { v2 += knot_i ( i ) * knot_i ( k ) ; } }
      v[j] += c0 + c1 * v1 / o + 2 * c2 * v2  / ( o * ( o - 1 ) ) ;
    }
  }
  //
  // normalize it! 
  //
  const double isum = 1.0 / 
    _spline_integral_ ( v , m_bspline.knots() , m_bspline.order() ) ;
  //
  for ( unsigned short ix = 0 ; ix < m_sphere.nX() ; ++ix ) 
  { 
    const bool updated = m_bspline.setPar ( ix , v [ix] * isum ) ; 
    update = updated || update ;
  }
  //
  return update ;
}
// ============================================================================
// ============================================================================
// convex SPLINE 
// ============================================================================
// ============================================================================
/* constructor from the list of knots and the order 
 *  vector of parameters will be calculated automatically 
 *  @param points non-empty vector of poinst/knots 
 *  @param order  the order of splines 
 *  - vector of points is not requires to be ordered 
 *  - duplicated knots will be ignored
 *  - min/max value will be used as interval boundaries 
 */
// ============================================================================
Gaudi::Math::ConvexSpline::ConvexSpline
( const std::vector<double>& points      ,
  const unsigned short       order       , 
  const bool                 increasing  ,
  const bool                 convex      )
  : Gaudi::Math::MonothonicSpline ( points , order , increasing ) 
  , m_convex                      ( convex ) 
{
  if ( 2 > this->order() ) { throw GaudiException
      ( "Degree of spline must be at least 2" , 
        "Gaudi::Math::ConvexSpline"           , StatusCode(816,true) ) ; }
  updateCoefficients () ;
}
// ============================================================================
/*  Constructor from the list of knots and list of parameters 
 *  The spline order will be calculated automatically 
 *  @param points non-empty vector of poinst/knots 
 *  @param pars   non-empty vector of parameters 
 *  - vector of points is not requires to be ordered 
 *  - duplicated knots will be ignored
 *  - min/max value will be used as interval boundaries 
 */
// ============================================================================
Gaudi::Math::ConvexSpline::ConvexSpline
( const std::vector<double>& points     ,
  const std::vector<double>& pars       ,
  const bool                 increasing ,
  const bool                 convex     ) 
  : Gaudi::Math::MonothonicSpline ( points , pars , increasing ) 
  , m_convex                      ( convex ) 
{
  if ( 2 > order() ) { throw GaudiException
      ( "Degree of spline must be at least 2" , 
        "Gaudi::Math::ConvexSpline"           , StatusCode(816,true) ) ; }
  updateCoefficients () ;
}
// ============================================================================
/*  Constructor for uniform binning 
 *  @param xmin   low  edge of spline interval 
 *  @param xmax   high edge of spline interval 
 *  @param inner  number of inner points in   (xmin,xmax) interval
 *  @param order  the degree of splline 
 */
// ============================================================================
Gaudi::Math::ConvexSpline::ConvexSpline
( const double            xmin       ,  
  const double            xmax       , 
  const unsigned short    inner      ,   // number of inner points 
  const unsigned short    order      , 
  const bool              increasing ,
  const bool              convex     ) 
  : Gaudi::Math::MonothonicSpline ( xmin , xmax , inner , order , increasing ) 
  , m_convex                      ( convex ) 
{
  if ( 2 > this->order() ) { throw GaudiException
      ( "Degree of spline must be at least 2" , 
        "Gaudi::Math::ConvexSpline"           , StatusCode(816,true) ) ; }
  updateCoefficients () ;
}
// ============================================================================
// constructor from positive spline 
// ============================================================================
Gaudi::Math::ConvexSpline::ConvexSpline
( const PositiveSpline&   spline      , 
  const bool              increasing  ,
  const bool              convex      ) 
  : Gaudi::Math::MonothonicSpline ( spline , increasing ) 
  , m_convex                      ( convex ) 
{
  if ( 2 > order() ) { throw GaudiException
      ( "Degree of spline must be at least 2" , 
        "Gaudi::Math::ConvexSpline"           , StatusCode(816,true) ) ; }
  updateCoefficients () ;
}
// ============================================================================
// constructor from basic spline 
// ============================================================================
Gaudi::Math::ConvexSpline::ConvexSpline
( const BSpline&          spline     , 
  const bool              increasing ,
  const bool              convex     ) 
  : Gaudi::Math::MonothonicSpline ( spline , increasing ) 
  , m_convex                      ( convex ) 
{
  if ( 2 > order() ) { throw GaudiException
      ( "Degree of spline must be at least 2" , 
        "Gaudi::Math::ConvexSpline"           , StatusCode(816,true) ) ; }
  updateCoefficients () ;
}
// ============================================================================
// constructor from monothonic spline 
// ============================================================================
Gaudi::Math::ConvexSpline::ConvexSpline
( const MonothonicSpline& spline , 
  const bool              convex ) 
  : Gaudi::Math::MonothonicSpline ( spline ) 
  , m_convex                      ( convex ) 
{
  if ( 2 > order() ) { throw GaudiException
      ( "Degree of spline must be at least 2" , 
        "Gaudi::Math::ConvexSpline"           , StatusCode(816,true) ) ; }
  updateCoefficients () ;
}
// ============================================================================
// destructor 
// ============================================================================
Gaudi::Math::ConvexSpline::~ConvexSpline(){}
// ============================================================================
// update coefficients  
// ============================================================================
bool Gaudi::Math::ConvexSpline::updateCoefficients  () 
{
  //
  bool   update = false ;
  //
  // get sphere coefficients  (all but 0 ) : NOTE INDICES HERE! 
  std::vector<double> v ( m_sphere.nX() - 1 ) ;
  for ( unsigned short ix = 1 ; ix < m_sphere.nX() ; ++ix ) 
  { v[ix-1] = m_sphere.x2 ( ix ) * ( ix + 1 ) ; }
  //
  // integrate them and to get new coefficients
  if   ( m_convex ) { std::partial_sum ( v. begin() , v. end() ,  v. begin() ) ; }
  else              { std::partial_sum ( v.rbegin() , v.rend() ,  v.rbegin() ) ; }
  //

  // Actual algorithm: "Safe"
  // BSpline aux1 ( m_bspline.knots() , v  )     ;
  // BSpline aux2 = aux1.indefinite_integral ( m_sphere.x2(0) ) ;  
  // v    = aux2.pars()    ;
  
  // in place: 
  //   the second integration: 
  std::vector<double> v2 ( m_sphere.nX() ) ;
  v2[0] = m_sphere.x2(0)  ;
  const unsigned short o = order() ;
  for ( unsigned int i = 0 ; i < v.size() ; ++i ) 
  { v2[i+1] = v2[i] + v[i] * ( knot_i (  i + o + 1 ) - knot_i ( i + 1 )  ) / o ; }
  //
  // revert, if needed 
  if ( !m_increasing ) { std::reverse ( v2.begin() , v2.end () ) ; }
  
  //
  const double isum = 1.0 / 
    _spline_integral_ ( v2 , m_bspline.knots() , m_bspline.order() ) ;
  //
  for ( unsigned short ix = 0 ; ix < m_sphere.nX() ; ++ix ) 
  { 
    const bool updated = m_bspline.setPar ( ix , v2 [ix] * isum ) ; 
    update = updated || update ;
  }
  //
  return update ;
}
// ============================================================================
// ============================================================================
// 2D-spline 
// ============================================================================
// ============================================================================
Gaudi::Math::Spline2D::Spline2D
( const Gaudi::Math::BSpline& xspline , 
  const Gaudi::Math::BSpline& yspline ) 
  : std::binary_function<double,double,double> () 
    //
  , m_xspline ( xspline        ) 
  , m_yspline ( yspline        ) 
  , m_sphere  ( xspline.npars() * yspline.npars() - 1 ) 
  , m_xcache  ( xspline.npars() , 0 ) 
  , m_ycache  ( yspline.npars() , 0 ) 
{
  for ( unsigned i = 0 ; i < m_xspline.npars() ; ++i ) { m_xspline.setPar ( i , 0 ) ; }
  for ( unsigned i = 0 ; i < m_yspline.npars() ; ++i ) { m_yspline.setPar ( i , 0 ) ; }
}
// ===========================================================================
// get the value
// ============================================================================
double Gaudi::Math::Spline2D::operator () ( const double x , const double y ) const 
{
  //
  if ( x < xmin() || y < ymin() || x > xmax() || y > ymax() ) { return 0 ; }
  //
  const double xarg =  
    !s_equal ( x , xmax ()) ? x :
    0 <= xmax ()            ? 
    Gaudi::Math::next_double ( xmax() , -s_ulps ) :
    Gaudi::Math::next_double ( xmax() ,  s_ulps ) ;
  //
  const double yarg =  
    !s_equal ( y , ymax ()) ? y :
    0 <= ymax ()            ?
    Gaudi::Math::next_double ( ymax() , -s_ulps ) :
    Gaudi::Math::next_double ( ymax() ,  s_ulps ) ;
  //
  // fill x-cache 
  for ( unsigned short ix = 0 ; ix < m_xcache.size() ; ++ix )
  {
    m_xspline.setPar ( ix , 1.0 ) ;
    //
    double resx  = m_xspline ( xarg ) ;
    if ( 0 < resx ) 
    {
      const double ti  = knot ( m_xspline.knots() , ix                         ) ;
      const double tip = knot ( m_xspline.knots() , ix + m_xspline.order() + 1 ) ;
      resx /= ( tip - ti ) ; 
    }
    //
    m_xcache[ix] = resx ;
    //
    m_xspline.setPar ( ix , 0.0 ) ;
  }
  //
  // fill y-cache 
  for ( unsigned short iy = 0 ; iy < m_ycache.size() ; ++iy )
  {
    m_yspline.setPar ( iy , 1.0 ) ;
    //
    double resy  = m_yspline ( yarg ) ;
    if ( 0 < resy ) 
    {
      const double ti  = knot ( m_yspline.knots() , iy                         ) ;
      const double tip = knot ( m_yspline.knots() , iy + m_yspline.order() + 1 ) ;
      resy /= ( tip - ti ) ; 
    }
    //
    m_ycache[iy] = resy ;
    //
    m_yspline.setPar ( iy , 0.0 ) ;
  }
  //
  double         result = 0 ;
  for ( unsigned short ix = 0 ; ix < m_xcache.size() ; ++ix ) 
  {
    const double vx   = m_xcache[ix] ;
    if ( s_zero ( vx ) ) { continue ; }       // CONTINUE 
    //
    for ( unsigned short iy = 0 ; iy < m_ycache.size() ; ++iy ) 
    {
      const double vy = m_ycache[iy] ;
      if ( s_zero ( vy ) ) { continue ; }     // CONTINUE 
      //
      const unsigned k = ix * m_ycache.size() + iy ;
      result += m_sphere.x2 ( k ) * vx * vy ;
    }
  }
  //
  return result * ( m_xspline.order() + 1 ) * ( m_yspline.order() + 1 ) ;
}
// ============================================================================
/*  get the integral over 2D-region 
 *  @param xlow  low  edge in x 
 *  @param xhigh high edge in x 
 *  @param ylow  low  edge in y 
 *  @param yhigh high edge in y 
 */
// ============================================================================
double Gaudi::Math::Spline2D::integral 
( const double xlow  , 
  const double xhigh , 
  const double ylow  , 
  const double yhigh ) const 
{
  //
  if      ( xhigh < xlow ) { return - integral ( xhigh , xlow  , ylow  , yhigh ) ; }
  else if ( yhigh < ylow ) { return - integral ( xlow  , xhigh , yhigh , ylow  ) ; }
  // boundaries 
  else if ( xhigh < xmin () || yhigh < ymin () ) { return 0 ; }
  else if ( xlow  > xmax () || ylow  > ymax () ) { return 0 ; }
  else if ( s_equal ( xlow , xhigh ) || s_equal ( ylow , yhigh ) ) { return 0 ; }
  //
  else if ( s_equal ( xlow  , xmin() ) &&  
            s_equal ( xhigh , xmax() ) &&  
            s_equal ( ylow  , ymin() ) &&  
            s_equal ( yhigh , ymax() ) ) { return integral() ; }
  // adjust 
  else if ( xlow  < xmin () ) { return integral ( xmin() , xhigh   , ylow   , yhigh  ) ; }
  else if ( xhigh > xmax () ) { return integral ( xlow   , xmax () , ylow   , yhigh  ) ; }
  else if ( ylow  < ymin () ) { return integral ( xlow   , xhigh   , ymin() , yhigh  ) ; }
  else if ( yhigh > ymax () ) { return integral ( xlow   , xhigh   , ylow   , ymax() ) ; }
  //
  //  
  const double xarg =  
    !s_equal ( xhigh , xmax ()) ? xhigh :
    0 <= xmax ()                ? 
    Gaudi::Math::next_double ( xmax() , -s_ulps ) :
    Gaudi::Math::next_double ( xmax() ,  s_ulps ) ;
  //
  const double yarg =  
    !s_equal ( yhigh , ymax ()) ? yhigh :
    0 <= ymax ()                ? 
    Gaudi::Math::next_double ( ymax() , -s_ulps ) :
    Gaudi::Math::next_double ( ymax() ,  s_ulps ) ;
  //
  // fill x-cache 
  for ( unsigned short ix = 0 ; ix < m_xcache.size() ; ++ix )
  {
    m_xspline.setPar ( ix , 1.0 ) ;
    //
    double resx  = m_xspline.integral ( xlow , xarg ) ;
    if ( 0 < resx ) 
    {
      const double ti  = knot ( m_xspline.knots() , ix                         ) ;
      const double tip = knot ( m_xspline.knots() , ix + m_xspline.order() + 1 ) ;
      resx /= ( tip - ti ) ; 
    }
    //
    m_xcache[ix] = resx ;
    //
    m_xspline.setPar ( ix , 0.0 ) ;
  }
  //
  // fill y-cache 
  for ( unsigned short iy = 0 ; iy < m_ycache.size() ; ++iy )
  {
    m_yspline.setPar ( iy , 1.0 ) ;
    //
    double resy  = m_yspline.integral ( ylow ,  yarg ) ;
    if ( 0 < resy ) 
    {
      const double ti  = knot ( m_yspline.knots() , iy                         ) ;
      const double tip = knot ( m_yspline.knots() , iy + m_yspline.order() + 1 ) ;
      resy /= ( tip - ti ) ; 
    }
    //
    m_ycache[iy] = resy ;
    //
    m_yspline.setPar ( iy , 0.0 ) ;
  }
  //
  double         result = 0 ;
  for ( unsigned short ix = 0 ; ix < m_xcache.size() ; ++ix ) 
  {
    const double vx   = m_xcache[ix] ;
    if ( s_zero ( vx ) ) { continue ; }       // CONTINUE 
    //
    for ( unsigned short iy = 0 ; iy < m_ycache.size() ; ++iy ) 
    {
      const double vy = m_ycache[iy] ;
      if ( s_zero ( vy ) ) { continue ; }     // CONTINUE 
      //
      const unsigned k = ix * m_ycache.size () + iy ;
      result += m_sphere.x2 ( k ) * vx * vy ;
    }
  }
  //
  return result * ( m_xspline.order() + 1 ) * ( m_yspline.order() + 1 ) ;
}
// ============================================================================
/*  get the integral over X  for given Y
 *  @param x  (INPUT) x-value 
 *  @param ylow  low  edge in y 
 *  @param yhigh high edge in y 
 */  
// ============================================================================
double Gaudi::Math::Spline2D::integrateY 
( const double x    , 
  const double ylow , const double yhigh ) const 
{
  //
  if      ( x < xmin() || x > xmax()           ) { return 0 ; }
  else if ( s_equal ( ylow  , ymin() ) && 
            s_equal ( yhigh , ymax() ) ) { return integrateY ( x ) ; }
  else if ( yhigh <  ylow ) { return - integrateY ( x , yhigh , ylow ) ; }
  else if ( s_equal ( ylow , yhigh )           ) { return 0 ; }
  else if ( yhigh <  ymin() ||  ylow >  ymax() ) { return 0 ; }
  else if ( ylow  <  ymin() ) { return integrateY ( x , ymin() , yhigh  ) ; }
  else if ( yhigh >  ymax() ) { return integrateY ( x , ylow   , ymax() ) ; }
  //
  const double xarg =  
    !s_equal ( x     , xmax ()) ? x :
    0 <= xmax ()                ? 
    Gaudi::Math::next_double ( xmax() , -s_ulps ) :
    Gaudi::Math::next_double ( xmax() ,  s_ulps ) ;
  //
  const double yarg =  
    !s_equal ( yhigh , ymax ()) ? yhigh :
    0 <= ymax ()                ? 
    Gaudi::Math::next_double ( ymax() , -s_ulps ) :
    Gaudi::Math::next_double ( ymax() ,  s_ulps ) ;
  //
  // fill x-cache 
  for ( unsigned short ix = 0 ; ix < m_xcache.size() ; ++ix )
  {
    m_xspline.setPar ( ix , 1.0 ) ;
    //
    double resx  = m_xspline ( xarg ) ;
    if ( 0 < resx ) 
    {
      const double ti  = knot ( m_xspline.knots() , ix                         ) ;
      const double tip = knot ( m_xspline.knots() , ix + m_xspline.order() + 1 ) ;
      resx /= ( tip - ti ) ; 
    }
    //
    m_xcache[ix] = resx ;
    //
    m_xspline.setPar ( ix , 0.0 ) ;
  }
  //
  // fill y-cache 
  for ( unsigned short iy = 0 ; iy < m_ycache.size() ; ++iy )
  {
    m_yspline.setPar ( iy , 1.0 ) ;
    //
    double resy  = m_yspline.integral ( ylow ,  yarg ) ;
    if ( 0 < resy ) 
    {
      const double ti  = knot ( m_yspline.knots() , iy                         ) ;
      const double tip = knot ( m_yspline.knots() , iy + m_yspline.order() + 1 ) ;
      resy /= ( tip - ti ) ; 
    }
    //
    m_ycache[iy] = resy ;
    //
    m_yspline.setPar ( iy , 0.0 ) ;
  }
  //
  double         result = 0 ;
  for ( unsigned short ix = 0 ; ix < m_xcache.size() ; ++ix ) 
  {
    const double vx   = m_xcache[ix] ;
    if ( s_zero ( vx ) ) { continue ; }       // CONTINUE 
    //
    for ( unsigned short iy = 0 ; iy < m_ycache.size() ; ++iy ) 
    {
      const double vy = m_ycache[iy] ;
      if ( s_zero ( vy ) ) { continue ; }     // CONTINUE 
      //
      const unsigned k = ix * m_ycache.size () + iy ;
      result += m_sphere.x2 ( k ) * vx * vy ;
    }
  }
  //
  return result * ( m_xspline.order() + 1 ) * ( m_yspline.order() + 1 ) ;
}
// ============================================================================
/*  get the integral over Y  for given C
 *  @param y  (INPUT) y-value 
 *  @param xlow  low  eadge in x 
 *  @param xhigh high edge in x 
 */  
// ============================================================================
double Gaudi::Math::Spline2D::integrateX
( const double y    , 
  const double xlow , const double xhigh ) const 
{
  //
  if      ( y < ymin() || y > ymax()           ) { return 0 ; }
  else if ( s_equal ( xlow  , xmin() ) && 
            s_equal ( xhigh , xmax() ) ) { return integrateX ( y ) ; }
  else if ( xhigh <  xlow ) { return - integrateX ( y , xhigh , xlow ) ; }
  else if ( s_equal ( xlow , xhigh )           ) { return 0 ; }
  else if ( xhigh <= xmin() ||  xlow > xmax()  ) { return 0 ; }
  else if ( xlow  <  xmin() ) { return integrateX ( y , xmin() , xhigh  ) ; }
  else if ( xhigh >  xmax() ) { return integrateX ( y , xlow   , xmax() ) ; }
  //
  const double xarg =  
    !s_equal ( xhigh , xmax ()) ? xhigh :
    0 <= xmax ()                ? 
    Gaudi::Math::next_double ( xmax() , -s_ulps ) :
    Gaudi::Math::next_double ( xmax() ,  s_ulps ) ;
  //
  const double yarg =  
    !s_equal ( y     , ymax ()) ? y     :
    0 <= ymax ()                ? 
    Gaudi::Math::next_double ( ymax() , -s_ulps ) :
    Gaudi::Math::next_double ( ymax() ,  s_ulps ) ;
  //
  // fill x-cache 
  for ( unsigned short ix = 0 ; ix < m_xcache.size() ; ++ix )
  {
    m_xspline.setPar ( ix , 1.0 ) ;
    //
    double resx  = m_xspline.integral ( xlow , xarg ) ;
    if ( 0 < resx ) 
    {
      const double ti  = knot ( m_xspline.knots() , ix                         ) ;
      const double tip = knot ( m_xspline.knots() , ix + m_xspline.order() + 1 ) ;
      resx /= ( tip - ti ) ; 
    }
    //
    m_xcache[ix] = resx ;
    //
    m_xspline.setPar ( ix , 0.0 ) ;
  }
  //
  // fill y-cache 
  for ( unsigned short iy = 0 ; iy < m_ycache.size() ; ++iy )
  {
    m_yspline.setPar ( iy , 1.0 ) ;
    //
    double resy  = m_yspline( yarg ) ;
    if ( 0 < resy ) 
    {
      const double ti  = knot ( m_yspline.knots() , iy                         ) ;
      const double tip = knot ( m_yspline.knots() , iy + m_yspline.order() + 1 ) ;
      resy /= ( tip - ti ) ; 
    }
    //
    m_ycache[iy] = resy ;
    //
    m_yspline.setPar ( iy , 0.0 ) ;
  }
  //
  double         result = 0 ;
  for ( unsigned short ix = 0 ; ix < m_xcache.size() ; ++ix ) 
  {
    const double vx   = m_xcache[ix] ;
    if ( s_zero ( vx ) ) { continue ; }       // CONTINUE 
    //
    for ( unsigned short iy = 0 ; iy < m_ycache.size() ; ++iy ) 
    {
      const double vy = m_ycache[iy] ;
      if ( s_zero ( vy ) ) { continue ; }     // CONTINUE 
      //
      const unsigned k = ix * m_ycache.size () + iy ;
      result += m_sphere.x2 ( k ) * vx * vy ;
    }
  }
  //
  return result * ( m_xspline.order() + 1 ) * ( m_yspline.order() + 1 ) ;
}
// ============================================================================
/*  get the integral over 2D-region 
 *  \f[ x_{min}<x<x_{max}, y_{min}<y<y_{max}\f] 
 *  @param xlow  low  edge in x 
 *  @param xhigh high edge in x 
 *  @param ylow  low  edge in y 
 *  @param yhigh high edge in y 
 */
// ============================================================================
double Gaudi::Math::Spline2D::integral   () const { return 1 ; }
// ============================================================================
/*  get the integral over X  for given Y
 *  @param x  (INPUT) x-value 
 */  
// ============================================================================
double Gaudi::Math::Spline2D::integrateY 
( const double x ) const 
{
  //
  if      ( x < xmin() || x > xmax()           ) { return 0 ; }
  //
  const double xarg =  
    !s_equal ( x     , xmax ()) ? x :
    0 <= xmax ()                ? 
    Gaudi::Math::next_double ( xmax() , -s_ulps ) :
    Gaudi::Math::next_double ( xmax() ,  s_ulps ) ;
  //
  // fill x-cache 
  for ( unsigned short ix = 0 ; ix < m_xcache.size() ; ++ix )
  {
    m_xspline.setPar ( ix , 1.0 ) ;
    //
    double resx  = m_xspline ( xarg ) ;
    if ( 0 < resx ) 
    {
      const double ti  = knot ( m_xspline.knots() , ix                         ) ;
      const double tip = knot ( m_xspline.knots() , ix + m_xspline.order() + 1 ) ;
      resx /= ( tip - ti ) ; 
    }
    //
    m_xcache[ix] = resx ;
    //
    m_xspline.setPar ( ix , 0.0 ) ;
  }
  //
  // fill y-cache 
  //
  std::fill ( m_ycache.begin() , m_ycache.end() , 1 ) ;
  //
  double         result = 0 ;
  for ( unsigned short ix = 0 ; ix < m_xcache.size() ; ++ix ) 
  {
    const double vx   = m_xcache[ix] ;
    if ( s_zero ( vx ) ) { continue ; }       // CONTINUE 
    //
    for ( unsigned short iy = 0 ; iy < m_ycache.size() ; ++iy ) 
    {
      const double vy = m_ycache[iy] ;
      if ( s_zero ( vy ) ) { continue ; }     // CONTINUE 
      //
      const unsigned k = ix * m_ycache.size () + iy ;
      result += m_sphere.x2 ( k ) * vx * vy ;
    }
  }
  //
  return result * ( m_xspline.order() + 1 ) ;
}
// ============================================================================
/*  get the integral over X  for given Y
 *  @param y  (INPUT) y-value 
 */  
// ============================================================================
double Gaudi::Math::Spline2D::integrateX
( const double y    ) const 
{
  //
  if      ( y < ymin() || y > ymax()           ) { return 0 ; }
  //
  const double yarg =  
    !s_equal ( y     , ymax ()) ? y     :
    0 <= ymax ()                ? 
    Gaudi::Math::next_double ( ymax() , -s_ulps ) :
    Gaudi::Math::next_double ( ymax() ,  s_ulps ) ;
  //
  // fill x-cache 
  std::fill ( m_xcache.begin() , m_xcache.end() , 1 ) ;
  //
  // fill y-cache 
  for ( unsigned short iy = 0 ; iy < m_ycache.size() ; ++iy )
  {
    m_yspline.setPar ( iy , 1.0 ) ;
    //
    double resy  = m_yspline( yarg ) ;
    if ( 0 < resy ) 
    {
      const double ti  = knot ( m_yspline.knots() , iy                         ) ;
      const double tip = knot ( m_yspline.knots() , iy + m_yspline.order() + 1 ) ;
      resy /= ( tip - ti ) ; 
    }
    //
    m_ycache[iy] = resy ;
    //
    m_yspline.setPar ( iy , 0.0 ) ;
  }
  //
  double         result = 0 ;
  for ( unsigned short ix = 0 ; ix < m_xcache.size() ; ++ix ) 
  {
    const double vx   = m_xcache[ix] ;
    if ( s_zero ( vx ) ) { continue ; }       // CONTINUE 
    //
    for ( unsigned short iy = 0 ; iy < m_ycache.size() ; ++iy ) 
    {
      const double vy = m_ycache[iy] ;
      if ( s_zero ( vy ) ) { continue ; }     // CONTINUE 
      //
      const unsigned k = ix * m_ycache.size () + iy ;
      result += m_sphere.x2 ( k ) * vx * vy ;
    }
  }
  //
  return result * ( m_yspline.order() + 1 ) ;
}
// ============================================================================

// ============================================================================
// symmetric  2D-spline 
// ============================================================================
Gaudi::Math::Spline2DSym::Spline2DSym
( const Gaudi::Math::BSpline&  spline )
  : std::binary_function<double,double,double> () 
    //
  , m_spline ( spline             ) 
  , m_sphere ( spline.npars() * ( spline.npars()  + 1 ) / 2 - 1  ) 
  , m_xcache ( spline.npars() , 0 ) 
  , m_ycache ( spline.npars() , 0 ) 
{
  for ( unsigned i = 0 ; i < m_spline.npars() ; ++i ) { m_spline.setPar ( i , 0 ) ; }
}
// ===========================================================================
// get the value
// ============================================================================
double Gaudi::Math::Spline2DSym::operator () ( const double x , const double y ) const 
{
  //
  if ( x < xmin() || y < ymin() || x > xmax() || y > ymax() ) { return 0 ; }
  //
  const double xarg =  
    !s_equal ( x , xmax ()) ? x :
    0 <= xmax ()            ? 
    Gaudi::Math::next_double ( xmax() , -s_ulps ) :
    Gaudi::Math::next_double ( xmax() ,  s_ulps ) ;
  //
  const double yarg =  
    !s_equal ( y , ymax ()) ? y :
    0 <= ymax ()            ?
    Gaudi::Math::next_double ( ymax() , -s_ulps ) :
    Gaudi::Math::next_double ( ymax() ,  s_ulps ) ;
  //
  // fill x&y-caches  
  for ( unsigned short i = 0 ; i < m_xcache.size() ; ++i )
  {
    m_spline.setPar ( i , 1.0 ) ;
    //
    double resx  = m_spline ( xarg ) ;
    double resy  = m_spline ( yarg ) ;
    //
    if ( 0 < resx || 0 < resy ) 
    {
      const double ti  = knot ( m_spline.knots() , i                        ) ;
      const double tip = knot ( m_spline.knots() , i + m_spline.order() + 1 ) ;
      //
      if ( 0 < resx ) { resx /= ( tip - ti ) ; }
      if ( 0 < resy ) { resy /= ( tip - ti ) ; }
      //
    }
    //
    m_xcache[i] = resx ;
    m_ycache[i] = resy ;
    //
    m_spline.setPar ( i , 0.0 ) ;
  }
  //
  // get the final value 
  double         result = 0 ;
  for ( unsigned short ix = 0 ; ix < m_xcache.size() ; ++ix ) 
  {
    const double vx   = m_xcache[ix] ;
    if ( s_zero ( vx ) ) { continue ; }       // CONTINUE 
    //
    for ( unsigned short iy = 0 ; iy < m_ycache.size() ; ++iy ) 
    {
      const double vy = m_ycache[iy] ;
      if ( s_zero ( vy ) ) { continue ; }     // CONTINUE 
      //
      const unsigned int k = 
        ( ix < iy ) ? ( iy * ( iy + 1 ) / 2 + ix ) : ( ix * ( ix + 1 ) / 2 + iy ) ;
      //
      result += ( ix == iy ) ? 
        m_sphere.x2 ( k ) * vx * vy       : 
        m_sphere.x2 ( k ) * vx * vy * 0.5 ;
    }
  }
  //
  return result * ( ( m_spline.order() + 1 ) * ( m_spline.order() + 1 ) ) ;
}
// ============================================================================
/*  get the integral over 2D-region 
 *  @param xlow  low  edge in x 
 *  @param xhigh high edge in x 
 *  @param ylow  low  edge in y 
 *  @param yhigh high edge in y 
 */
// ============================================================================
double Gaudi::Math::Spline2DSym::integral 
( const double xlow  , 
  const double xhigh , 
  const double ylow  , 
  const double yhigh ) const 
{
  //
  if      ( xhigh < xlow ) { return - integral ( xhigh , xlow  , ylow  , yhigh ) ; }
  else if ( yhigh < ylow ) { return - integral ( xlow  , xhigh , yhigh , ylow  ) ; }
  // boundaries 
  else if ( xhigh < xmin () || yhigh < ymin () ) { return 0 ; }
  else if ( xlow  > xmax () || ylow  > ymax () ) { return 0 ; }
  else if ( s_equal ( xlow , xhigh ) || s_equal ( ylow , yhigh ) ) { return 0 ; }
  // adjust 
  else if ( xlow  < xmin () ) { return integral ( xmin() , xhigh   , ylow   , yhigh  ) ; }
  else if ( xhigh > xmax () ) { return integral ( xlow   , xmax () , ylow   , yhigh  ) ; }
  else if ( ylow  < ymin () ) { return integral ( xlow   , xhigh   , ymin() , yhigh  ) ; }
  else if ( yhigh > ymax () ) { return integral ( xlow   , xhigh   , ylow   , ymax() ) ; }
  //  
  const double xarg =  
    !s_equal ( xhigh , xmax ()) ? xhigh :
    0 <= xmax ()                ? 
    Gaudi::Math::next_double ( xmax() , -s_ulps ) :
    Gaudi::Math::next_double ( xmax() ,  s_ulps ) ;
  //
  const double yarg =  
    !s_equal ( yhigh , ymax ()) ? yhigh :
    0 <= ymax ()                ? 
    Gaudi::Math::next_double ( ymax() , -s_ulps ) :
    Gaudi::Math::next_double ( ymax() ,  s_ulps ) ;
  //
  // fill x&y-caches 
  for ( unsigned short i = 0 ; i < m_xcache.size() ; ++i )
  {
    m_spline.setPar ( i , 1.0 ) ;
    //
    double resx  = m_spline.integral ( xlow , xarg ) ;
    double resy  = m_spline.integral ( ylow , yarg ) ;
    //
    if ( 0 < resx || 0 < resy ) 
    {
      //
      const double ti  = knot ( m_spline.knots() , i                        ) ;
      const double tip = knot ( m_spline.knots() , i + m_spline.order() + 1 ) ;
      //
      if ( 0 < resx ) { resx /= ( tip - ti ) ; }
      if ( 0 < resy ) { resy /= ( tip - ti ) ; }
    }
    //
    m_xcache[i] = resx ;
    m_ycache[i] = resy ;
    //
    m_spline.setPar ( i , 0.0 ) ;
  }
  //
  // get the final value 
  double         result = 0 ;
  for ( unsigned short ix = 0 ; ix < m_xcache.size() ; ++ix ) 
  {
    const double vx   = m_xcache[ix] ;
    if ( s_zero ( vx ) ) { continue ; }       // CONTINUE 
    //
    for ( unsigned short iy = 0 ; iy < m_ycache.size() ; ++iy ) 
    {
      const double vy = m_ycache[iy] ;
      if ( s_zero ( vy ) ) { continue ; }     // CONTINUE 
      //
      const unsigned int k = 
        ( ix < iy ) ? ( iy * ( iy + 1 ) / 2 + ix ) : ( ix * ( ix + 1 ) / 2 + iy ) ;
      //
      result += ( ix == iy ) ? 
        m_sphere.x2 ( k ) * vx * vy       : 
        m_sphere.x2 ( k ) * vx * vy * 0.5 ;
    }
  }
  //
  return result * ( ( m_spline.order() + 1 ) * ( m_spline.order() + 1 ) )  ;
}
// ============================================================================
/*  get the integral over X  for given Y
 *  @param x  (INPUT) x-value 
 *  @param ylow  low  edge in y 
 *  @param yhigh high edge in y 
 */  
// ============================================================================
double Gaudi::Math::Spline2DSym::integrateY 
( const double x    , 
  const double ylow , const double yhigh ) const 
{
  //
  if      ( x < xmin() || x > xmax()           ) { return 0 ; }
  else if ( yhigh <  ylow ) { return - integrateY ( x , yhigh , ylow ) ; }
  else if ( s_equal ( ylow , yhigh )           ) { return 0 ; }
  else if ( yhigh <= ymin() ||  ylow > ymax()  ) { return 0 ; }
  else if ( ylow  <  ymin() ) { return integrateY ( x , ymin() , yhigh  ) ; }
  else if ( yhigh >  ymax() ) { return integrateY ( x , ylow   , ymax() ) ; }
  //
  const double xarg =  
    !s_equal ( x     , xmax ()) ? x :
    0 <= xmax ()                ? 
    Gaudi::Math::next_double ( xmax() , -s_ulps ) :
    Gaudi::Math::next_double ( xmax() ,  s_ulps ) ;
  //
  const double yarg =  
    !s_equal ( yhigh , ymax ()) ? yhigh :
    0 <= ymax ()                ? 
    Gaudi::Math::next_double ( ymax() , -s_ulps ) :
    Gaudi::Math::next_double ( ymax() ,  s_ulps ) ;
  //
  // fill x&y-caches 
  for ( unsigned short i = 0 ; i < m_xcache.size() ; ++i )
  {
    m_spline.setPar ( i , 1.0 ) ;
    //
    double resx  = m_spline          (        xarg ) ;
    double resy  = m_spline.integral ( ylow , yarg ) ;
    if ( 0 < resx || 0 < resy ) 
    {
      const double ti  = knot ( m_spline.knots() , i                        ) ;
      const double tip = knot ( m_spline.knots() , i + m_spline.order() + 1 ) ;
      if ( 0 < resx ) { resx /= ( tip - ti ) ; }
      if ( 0 < resy ) { resy /= ( tip - ti ) ; }
    }
    //
    m_xcache[i] = resx ;
    m_ycache[i] = resy ;
    //
    m_spline.setPar ( i , 0.0 ) ;
  }
  //
  //
  double         result = 0 ;
  for ( unsigned short ix = 0 ; ix < m_xcache.size() ; ++ix ) 
  {
    const double vx   = m_xcache[ix] ;
    if ( s_zero ( vx ) ) { continue ; }       // CONTINUE 
    //
    for ( unsigned short iy = 0 ; iy < m_ycache.size() ; ++iy ) 
    {
      const double vy = m_ycache[iy] ;
      if ( s_zero ( vy ) ) { continue ; }     // CONTINUE 
      //
      const unsigned int k = 
        ( ix < iy ) ? ( iy * ( iy + 1 ) / 2 + ix ) : ( ix * ( ix + 1 ) / 2 + iy ) ;
      //
      result += ( ix == iy ) ? 
        m_sphere.x2 ( k ) * vx * vy       : 
        m_sphere.x2 ( k ) * vx * vy * 0.5 ;
    }
  }
  //
  return result * ( m_spline.order() + 1 ) * ( m_spline.order() + 1 ) ;
}
// ============================================================================
/*  get the integral over Y  for given Y
 *  @param y  (INPUT) y-value 
 *  @param xlow  low  eadge in x 
 *  @param xhigh high edge in x 
 */  
// ============================================================================
double Gaudi::Math::Spline2DSym::integrateX
( const double y    , 
  const double xlow , const double xhigh ) const 
{ return integrateY ( y , xlow , xhigh ) ; }
// ============================================================================
//  get the integral over 2D-region 
// ============================================================================
double Gaudi::Math::Spline2DSym::integral () const { return 1 ; }
// ============================================================================


// ============================================================================
/*  get the integral over Y  for given X
 *  @param x  (INPUT) x-value 
 */  
// ============================================================================
double Gaudi::Math::Spline2DSym::integrateY 
( const double x ) const 
{
  //
  if      ( x < xmin() || x > xmax()           ) { return 0 ; }
  //
  const double xarg =  
    !s_equal ( x     , xmax ()) ? x :
    0 <= xmax ()                ? 
    Gaudi::Math::next_double ( xmax() , -s_ulps ) :
    Gaudi::Math::next_double ( xmax() ,  s_ulps ) ;
  //
  // fill x&y-caches 
  for ( unsigned short i = 0 ; i < m_xcache.size() ; ++i )
  {
    m_spline.setPar ( i , 1.0 ) ;
    //
    double resx  = m_spline          (        xarg ) ;
    if ( 0 < resx ) 
    {
      const double ti  = knot ( m_spline.knots() , i                        ) ;
      const double tip = knot ( m_spline.knots() , i + m_spline.order() + 1 ) ;
      if ( 0 < resx ) { resx /= ( tip - ti ) ; }
    }
    //
    m_xcache[i] = resx ;
    m_ycache[i] = 1    ;
    //
    m_spline.setPar ( i , 0.0 ) ;
  }
  //
  //
  double         result = 0 ;
  for ( unsigned short ix = 0 ; ix < m_xcache.size() ; ++ix ) 
  {
    const double vx   = m_xcache[ix] ;
    if ( s_zero ( vx ) ) { continue ; }       // CONTINUE 
    //
    for ( unsigned short iy = 0 ; iy < m_ycache.size() ; ++iy ) 
    {
      const double vy = m_ycache[iy] ;
      if ( s_zero ( vy ) ) { continue ; }     // CONTINUE 
      //
      const unsigned int k = 
        ( ix < iy ) ? ( iy * ( iy + 1 ) / 2 + ix ) : ( ix * ( ix + 1 ) / 2 + iy ) ;
      //
      result += ( ix == iy ) ? 
        m_sphere.x2 ( k ) * vx * vy       : 
        m_sphere.x2 ( k ) * vx * vy * 0.5 ;
    }
  }
  //
  return result * ( m_spline.order() + 1 ) ;
}
// ============================================================================
/*  get the integral over X  for given Y
 *  @param x  (INPUT) x-value 
 */  
// ============================================================================
double Gaudi::Math::Spline2DSym::integrateX ( const double y ) const 
{ return integrateY ( y ) ; }
// ============================================================================



  
// ============================================================================
// The END 
// ============================================================================
