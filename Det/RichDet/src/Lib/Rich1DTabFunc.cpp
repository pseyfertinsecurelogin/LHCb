
//============================================================================
/** @file Rich1DTabFunc.cpp
 *
 *  Implementation file for class : Rich::TabulatedFunction1D
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2003-08-13
 */
//============================================================================

// STL
#include <sstream>
#include <cmath>

// GaudiKernel
#include "GaudiKernel/GaudiException.h"

// local
#include "RichDet/Rich1DTabFunc.h"

// boost
#include "boost/numeric/conversion/bounds.hpp"
#include "boost/limits.hpp"

using namespace Rich;

//============================================================================

bool TabulatedFunction1D::initInterpolator( const double x[],
                                            const double y[],
                                            const unsigned int size,
                                            const gsl_interp_type * interType )
{
  // copy data to temporary map
  Data data;
  for ( unsigned int i = 0; i < size; ++i ) { data[ x[i] ] = y[i]; }

  // initialise interpolation
  m_OK = initInterpolator( data, interType );

  // return final status
  return m_OK;
}

//============================================================================

bool TabulatedFunction1D::initInterpolator( const std::vector<double> & x,
                                            const std::vector<double> & y,
                                            const gsl_interp_type * interType )
{
  // Check on size of containers
  if ( x.size() != y.size() )
  {
    m_OK = false;
    initInterpolator();
    throw GaudiException( "Size of x vector != size of y vector",
                          "*Rich::TabulatedFunction1D*", StatusCode::FAILURE );
  }
  else
  {
    // copy data to temporary map
    Data data;
    for ( auto ix(x.begin()), iy(y.begin());
          ix != x.end(); ++ix, ++iy ) { data[*ix] = *iy; }

    // initialise interpolation
    m_OK = initInterpolator( data, interType );
  }

  // return final status
  return m_OK;
}

//============================================================================

bool
TabulatedFunction1D::initInterpolator( const std::vector< std::pair<double,double> > & data,
                                       const gsl_interp_type * interType )
{
  // copy data to temporary map
  Data data_map;
  for ( auto i = data.begin(); i != data.end(); ++i ) { data_map[i->first] = i->second; }

  // initialise interpolation
  m_OK = initInterpolator( data_map, interType );

  // return final status
  return m_OK;
}

//============================================================================

bool TabulatedFunction1D::initInterpolator( const std::map<double,double> & data,
                                            const gsl_interp_type * interType )
{

  // clean up first
  clearInterpolator();

  // Create the temporary GSL interpolator
  GSLSpline::Ptr gslSpline( gsl_spline_alloc( interType, data.size() ) );

  // Check number of points needed to work ...
  const auto min_points = gsl_interp_min_size(gslSpline->interp);
  if ( data.size() < min_points )
  {
    std::ostringstream mess;
    mess << "Error whilst initialising GSL interpolator : Type '" 
         << gsl_interp_name(gslSpline->interp)
         << "' requires a minimum of " << min_points << " data points. Only given "
         << data.size();
    initInterpolator();
    throw GaudiException( mess.str(), "*Rich::TabulatedFunction1D*", StatusCode::FAILURE );
    return false;
  }
  
  // find the min and max X values
  auto minX = boost::numeric::bounds<double>::highest();
  auto maxX = boost::numeric::bounds<double>::lowest();

  // Copy data to temporary GSL initialisation arrays
  std::unique_ptr<double[]> x ( new double[data.size()] );
  std::unique_ptr<double[]> y ( new double[data.size()] );
  unsigned int i = 0;
  for ( auto iD = data.begin(); iD != data.end(); ++iD, ++i )
  {
    x[i] = (*iD).first;
    y[i] = (*iD).second;
    // set min and max x
    if ( x[i] < minX ) { minX = x[i]; }
    if ( x[i] > maxX ) { maxX = x[i]; }
  }

  // find the min distance in x between consecutive (x,y) data points
  auto minXinc = boost::numeric::bounds<double>::highest();
  {
    auto iA = data.begin();
    auto iB = iA; ++iB;
    for ( ; iB != data.end(); ++iB, ++iA )
    {
      const auto diff = fabs( iA->first - iB->first );
      if ( diff < minXinc ) { minXinc = diff; }
    }
  }
  
  // Initialise the interpolators
  const auto err = gsl_spline_init ( gslSpline.get(), x.get(), y.get(), data.size() );
  if ( err )
  {
    initInterpolator();
    throw GaudiException( "Error whilst initialising GSL interpolators",
                          "*TabulatedFunction1D*", StatusCode::FAILURE );
    return false;
  }

  // Determine the number of sample points for the fast fixed binned interpolator
  const unsigned int maxSamples = 5000;
  const unsigned int nData      = data.size();
  // determine the minimum number of sample points based on the min X inc
  const unsigned int minXPts  = ( minXinc>0 ? std::lround((maxX-minX)/minXinc) : maxSamples );
  const unsigned int nsamples = std::max( nData, std::min(maxSamples,minXPts) ); 
  if ( nsamples < minXPts ) 
  {
    std::ostringstream mess;
    mess << "Sample points " << nsamples << " < min X diff points " << minXPts
         << "| nData=" << nData << " minXPts=" << minXPts << " nsamples=" << nsamples 
         << " minXinc=" << minXinc << " maxX=" << maxX << " minX=" << minX;
    initInterpolator();
    throw GaudiException( mess.str(), "*Rich::TabulatedFunction1D*", StatusCode::FAILURE );
    return false;
  }

  // Initialise the fast interpolator
  m_fastInterp.init( minX, maxX, gslSpline.get(), nsamples );

  // // check the interpolator is able to reproduce the inout (x,y) data points
  // const auto minDiff = 1e-6;
  // for ( const auto & d : data )
  // {
  //   const auto diff = fabs( d.second - value(d.first) );
  //   std::cout << "Diff = " << d.second << " " << diff << std::endl;
  // }

  // // printout
  // std::cout << "Input data :-" << std::endl;
  // for ( const auto d : data )
  // {
  //  std::cout << "  " << d.first << ", " << d.second << std::endl;
  // }
  // std::cout << "Interpolator scan :- " << std::endl;
  // const unsigned int nScans = 100;
  // const auto inc = ( maxX - minX ) / nScans;
  // double xx = minX;
  // for ( unsigned int i = 0; i < nScans; ++i )
  // {
  //  std::cout << "  " << xx << " = " << value(xx) << std::endl;
  //  xx += inc;
  // } 

  return true;
}

//============================================================================

void TabulatedFunction1D::initInterpolator()
{
  // remove any existing interpolators
  clearInterpolator();
}

//============================================================================

double
TabulatedFunction1D::rms( const double from,
                          const double to,
                          const unsigned int samples,
                          const TabulatedFunction1D * weightFunc ) const
{
  if ( samples < 2 )
  {
    throw GaudiException( "rms() : samples must be > 1",
                          "*Rich::TabulatedFunction1D*", StatusCode::FAILURE );
  }

  // x increment
  const auto xInc = (to-from)/(double)(samples-1);

  double rms(0), X(from);
  for ( unsigned int i = 0; i < samples; ++i, X += xInc )
  {
    const double Y = value(X) * ( weightFunc ? weightFunc->value(X) : 1.0 );
    if ( Y > 0 ) { rms += Y * Y; }
  }
  rms /= (double)samples;

  return std::sqrt(rms);
}

//============================================================================

double
TabulatedFunction1D::standardDeviation( const double from,
                                        const double to,
                                        const unsigned int samples,
                                        const TabulatedFunction1D * weightFunc ) const
{
  if ( samples < 2 )
  {
    throw GaudiException( "standardDeviation() : samples must be > 1",
                          "*Rich::TabulatedFunction1D*", StatusCode::FAILURE );
  }

  // mean value
  const auto avgX = meanX(from,to);

  // x increment
  const auto xInc = (to-from)/(double)(samples-1);

  double sd(0), sum(0), X(from);
  for ( unsigned int i = 0; i < samples; ++i, X += xInc )
  {
    const auto Y = value(X) * ( weightFunc ? weightFunc->value(X) : 1.0 );
    if ( Y > 0 )
    {
      sd  += Y * std::pow(X-avgX,2);
      sum += Y;
    }
  }
  sd /= sum;

  return std::sqrt(sd);
}

//============================================================================

std::unique_ptr<TabulatedFunction1D> 
TabulatedFunction1D::combine( const ConstVector & funcs,
                              const unsigned int samples,
                              const gsl_interp_type * interType )
{
  if ( samples < 2 )
  {
    throw GaudiException( "combine() : samples must be > 1",
                          "*Rich::TabulatedFunction1D*", StatusCode::FAILURE );
  }
  
  // Default to a nullptr pointer. Filled later on.
  std::unique_ptr<TabulatedFunction1D> combFunc;

  // Get global min and max range of function
  auto maxX(boost::numeric::bounds<double>::highest());
  auto minX(boost::numeric::bounds<double>::lowest());
  for ( const auto F : funcs )
  {
    if ( F->minX() > minX ) { minX = F->minX(); }
    if ( F->maxX() < maxX ) { maxX = F->maxX(); }
  }

  // Check all is OK
  if ( minX < maxX )
  {

    // x increment
    const auto xInc = (maxX-minX)/(double)(samples-1);

    // Create the data points
    Data mergedData;
    double X(minX);
    for ( unsigned int i = 0; i < samples; ++i, X += xInc )
    {
      double Y = 1.0;
      for ( const auto func : funcs ) { Y *= func->value(X); }
      mergedData[X] = Y;
    }

    // Create the new interpolated function
    combFunc = std::make_unique<TabulatedFunction1D>(mergedData,interType);

  }

  // return
  return combFunc;
}

//============================================================================

double
TabulatedFunction1D::rangeWarning( const double x, const double retx ) const
{
  std::cerr << "Rich::TabulatedFunction1D : WARNING : Out-Of-Range x = " << x
            << " Valid Range = " << minX() << " to " << maxX() << std::endl;
  return retx;
}

//============================================================================
