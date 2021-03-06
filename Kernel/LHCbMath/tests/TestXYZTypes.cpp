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

// STL
#include <random>
#include <vector>

// Gaudi
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// Eigen
#include "LHCbMath/EigenTypes.h"

// VectorClass
//#include "LHCbMath/VectorClassTypes.h"

// VDT
#include "vdt/sqrt.h"

int main( int /*argc*/, char** /*argv*/ ) {

  // randomn generator
  static std::default_random_engine             gen;
  static std::uniform_real_distribution<double> x( -1000, 1000 );

  const unsigned int nTests = 10;

  std::cout << "Running " << nTests << " tests" << std::endl;
  for ( unsigned int i = 0; i < nTests; ++i ) {

    // Make a Gaudi point and vector
    const Gaudi::XYZPoint  gp( x( gen ), x( gen ), x( gen ) );
    const Gaudi::XYZVector gv( x( gen ), x( gen ), x( gen ) );

    // make some Eigen types
    const LHCb::Math::Eigen::XYZPoint<double>  ep( gp );
    const LHCb::Math::Eigen::XYZVector<double> ev( gv );

    // Test some planes
    const float a( x( gen ) ), b( x( gen ) ), c( x( gen ) ), d( x( gen ) );

    const Gaudi::Plane3D gplane( a, b, c, d );

    const LHCb::Math::Eigen::Plane3D<double> eplane( gplane );

    const Gaudi::Plane3D gplane2 = eplane;

    std::cout << "Distance Test = " << gplane.Distance( gp ) << " " << eplane.Distance( ep ) << " "
              << gplane2.Distance( gp ) << std::endl;

    std::cout << "Normal Test = " << gplane.Normal() << " " << eplane.Normal() << std::endl;
  }

  return 0;
}
