
//-----------------------------------------------------------------------------
/** @file RichGeomPhoton.cpp
 *
 *  Implementation file for class : RichGeomPhoton
 *
 *  @author  Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   2017-02-06
 */
//-----------------------------------------------------------------------------

// local
#include "RichFutureUtils/RichGeomPhoton.h"

std::ostream & Rich::Future::GeomPhoton::fillStream ( std::ostream& s ) const
{
  s << "{ "
    << " cherenkovTheta:\t" << CherenkovTheta() << std::endl
    << " cherenkovPhi:\t" << CherenkovPhi() << std::endl
    << " emissionPoint:\t" << emissionPoint() << std::endl
    << " detectionPoint:\t" << detectionPoint() << std::endl
    << " sphMirReflectionPoint:\t"<<sphMirReflectionPoint() << std::endl
    << " flatMirReflectionPoint:\t"<<flatMirReflectionPoint() << std::endl
    //<< " primaryMirror:\t" << primaryMirror() << std::endl
    //<< " secondaryMirror:\t" << secondaryMirror() << std::endl
    << " smartID:\t" << smartID() << std::endl
    << " activeSegmentFraction:\t" << activeSegmentFraction() << " } ";
  return s;
}
