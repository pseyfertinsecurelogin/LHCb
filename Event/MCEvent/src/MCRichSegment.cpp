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

//-----------------------------------------------------------------------------
/** @file MCRichSegment.cpp
 *
 *  Implementation file for class : MCRichSegment
 *
 *  @author  Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date    2004-03-23
 */
//-----------------------------------------------------------------------------

// Gaudi
#include "GaudiKernel/GaudiException.h"

// local
#include "Event/MCRichSegment.h"

Gaudi::XYZPoint
LHCb::MCRichSegment::bestPoint( const double fraction ) const
{
  // Check data is OK
  if ( trajectoryPoints().size() < 2 ) 
  {
    throw GaudiException( "Too few trajectory data points", 
                          "MCRichSegment", StatusCode::FAILURE );
  }

  // If only two state points, then calculation is simple
  if ( 2 == trajectoryPoints().size() ) 
  {
    // CRJ _ Need to decide which way is neatest ...
    //return exitPoint()*fraction + Gaudi::XYZVector(entryPoint()*(1-fraction));
    return Gaudi::XYZPoint( exitPoint().x()*fraction + entryPoint().x()*(1-fraction),
                            exitPoint().y()*fraction + entryPoint().y()*(1-fraction),
                            exitPoint().z()*fraction + entryPoint().z()*(1-fraction) );
  }
  else 
  { // Need to find which two state points to use

    // Get the z coordinate of the point on the segment
    const double zPoint = fraction*exitPoint().z() + (1-fraction)*entryPoint().z();

    // Find the position points either side of the z point
    auto iF = trajectoryPoints().begin();
    auto iS = iF; 
    ++iS; // iterate once so iS is one ahead of iF
    for ( ; iS != trajectoryPoints().end(); ++iF, ++iS ) 
    {
      if ( (*iF).z() < zPoint && (*iS).z() > zPoint ) break;
    }

    if ( iS != trajectoryPoints().end() ) 
    {
      // finally, interpolate between the two points either side of required z
      const auto stepLength = (*iS).z() - (*iF).z();
      const auto firstF = ( stepLength>0 ? ((*iS).z()-zPoint)/stepLength : 0 );
      // CRJ _ Need to decide which way is neatest ...
      // return firstF*(*iF) + Gaudi::XYZVector((1-firstF)*(*iS));
      return Gaudi::XYZPoint ( firstF*(*iF).x() + (1-firstF)*(*iS).x(),
                               firstF*(*iF).y() + (1-firstF)*(*iS).y(),
                               firstF*(*iF).z() + (1-firstF)*(*iS).z() );
    } 
    else 
    {
      // gone beyond the end of the list of points, so just use the first/last point
      return ( zPoint < entryPoint().z() ? entryPoint() : exitPoint() );
    }

  }

}

Gaudi::XYZVector 
LHCb::MCRichSegment::bestMomentum( const double fraction ) const
{

  // Check data is OK
  if ( trajectoryPoints().size() < 2 || 
       trajectoryMomenta().size() != trajectoryPoints().size() ) 
  {
    throw GaudiException( "Too few trajectory data points", 
                          "MCRichSegment", StatusCode::FAILURE );
  }

  // If only two state points, then calculation is simple
  if ( 2 == trajectoryMomenta().size() ) 
  {
    return fraction*exitMomentum() + (1-fraction)*entryMomentum();
  } 
  else 
  { // Need to find which two state points to use

    // Get the z coordinate of the point on the segment
    const auto zPoint = fraction*exitPoint().z() + (1-fraction)*entryPoint().z();

    // Find the position points either side of the z point
    auto iF = trajectoryPoints().begin();
    auto iS = iF; 
    ++iS; // iterate once so iS is one ahead of iF
    auto iMom = trajectoryMomenta().begin();
    for ( ; iS != trajectoryPoints().end(); ++iF, ++iS, ++iMom ) 
    {
      if ( (*iF).z() < zPoint && (*iS).z() > zPoint ) break;
    }

    if ( iS != trajectoryPoints().end() ) 
    {
      // interpolate between the two points either side of required z
      const auto stepLength = (*iS).z() - (*iF).z();
      const auto firstF = ( stepLength>0 ? ((*iS).z()-zPoint)/stepLength : 0.0 );
      return ( firstF * (*iMom) ) + ( (1-firstF) * (*(++iMom)) );
    } 
    else 
    {
      // gone beyond the end of the list of points, so just use the first/last point
      return ( zPoint < entryPoint().z() ? entryMomentum() : exitMomentum() );
    }

  }

}

Gaudi::XYZVector 
LHCb::MCRichSegment::averageMomentum( const unsigned int nsamples ) const
{
  Gaudi::XYZVector avgMom(0,0,0);
  for ( unsigned int iSample = 0; iSample < nsamples+1; ++iSample ) 
  {
    avgMom += bestMomentum( static_cast<double>(iSample)/static_cast<double>(nsamples) );
  }
  return ( avgMom /= static_cast<double>(nsamples+1) );
}
