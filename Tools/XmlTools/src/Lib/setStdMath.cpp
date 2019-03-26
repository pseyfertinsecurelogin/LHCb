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
#include "XmlTools/Evaluator.h"

#include <cmath> // for sqrt and pow

using namespace std;

namespace XmlTools {

  void Evaluator::setStdMath() {

    //   S E T   S T A N D A R D   C O N S T A N T S

    setVariable( "pi", 3.14159265358979323846 );
    setVariable( "e", 2.7182818284590452354 );
    setVariable( "gamma", 0.577215664901532861 );
    setVariable( "radian", 1.0 );
    setVariable( "rad", 1.0 );
    setVariable( "degree", 3.14159265358979323846 / 180. );
    setVariable( "deg", 3.14159265358979323846 / 180. );

    //   S E T   S T A N D A R D   F U N C T I O N S

    setFunction( "abs", fabs );
    setFunction( "min", fmin );
    setFunction( "max", fmax );
    setFunction( "sqrt", sqrt );
    setFunction( "pow", pow );
    setFunction( "sin", sin );
    setFunction( "cos", cos );
    setFunction( "tan", tan );
    setFunction( "asin", asin );
    setFunction( "acos", acos );
    setFunction( "atan", atan );
    setFunction( "atan2", atan2 );
    setFunction( "sinh", sinh );
    setFunction( "cosh", cosh );
    setFunction( "tanh", tanh );
    setFunction( "exp", exp );
    setFunction( "log", log );
    setFunction( "log10", log10 );
  }

} // namespace XmlTools
