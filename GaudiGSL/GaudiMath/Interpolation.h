/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef GAUDIMATH_INTERPOLATION_H
#define GAUDIMATH_INTERPOLATION_H 1
// ============================================================================
// include files
// ============================================================================
#include <iosfwd>
#include <string>

class StatusCode;

namespace GaudiMath {
  namespace Interpolation {
    enum Type {
      Linear = 0,       // linear interpolation
      Polynomial,       // polinomial interpolation
      Cspline,          // Cubic spline with natural  boundary conditions
      Cspline_Periodic, // Cubic spline with periodic boundary conditions
      Akima,            // Akima spline with natural  boundary conditions
      Akima_Periodic    // Akima spline with periodic boundary conditions
    };

    StatusCode           parse( Type&, const std::string& );
    std::string          toString( const Type& );
    std::ostream&        toStream( const Type&, std::ostream& );
    inline std::ostream& operator<<( std::ostream& os, const Type& t ) { return toStream( t, os ); }

  } // end of namespace Interpolation
} // end of namespace GaudiMath

#endif // GAUDIMATH_INTERPOLATION_H
