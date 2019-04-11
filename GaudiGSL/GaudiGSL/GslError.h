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
#ifndef GAUDIGSL_GSLERROR_H
#define GAUDIGSL_GSLERROR_H 1
// Include files
#include "GaudiKernel/Kernel.h"
// STD & STL
#include <string>
#include <tuple>

/** @struct GslError GslError.h
 *
 *  Helper class to represent GSL errors
 *
 *  @author Vanya Belyaev Ivan.Belyaev
 *  @date   29/04/2002
 */
class GAUDI_API GslError {
public:
  /// error message ('reason')
  std::string reason;
  /// file name
  std::string file;
  /// line number
  int line;
  /// error code (GSL)
  int code;
  /// constructor
  GslError( std::string r = "", std::string f = "", const int l = 0, const int c = 0 )
      : reason( std::move( r ) ), file( std::move( f ) ), line( l ), code( c ){};
  /// comparison (ordering) criteria
  friend bool operator<( const GslError& left, const GslError& right ) {
    return std::tie( left.code, left.reason, left.file, left.line ) <
           std::tie( right.code, right.reason, right.file, right.line );
  };
};

// ============================================================================
// The END
// ============================================================================
#endif // GSLERROR_H
