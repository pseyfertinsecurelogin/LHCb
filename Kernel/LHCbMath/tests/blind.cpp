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
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <cmath>
#include <fstream>
#include <iostream>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/StatEntity.h"
#include "GaudiKernel/System.h"
// ============================================================================
// LHCbMath
// ============================================================================
#include "LHCbMath/Blind.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/assert.hpp"
#include "boost/static_assert.hpp"
// ============================================================================
/** @file
 *  Set of simple tests for 'blind' function
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2010-10-10
 */
// ============================================================================
int main() {
  using namespace Gaudi::Math;

  typedef std::vector<std::string> Files;

  Files files;
  files.push_back( "/GaudiKernel/IInterface.h" );
  files.push_back( "/GaudiKernel/Parsers.h" );
  files.push_back( "/GaudiKernel/Grammars.h" );
  files.push_back( "/GaudiKernel/Algorithm.h" );
  files.push_back( "/GaudiKernel/Property.h" );
  files.push_back( "/GaudiKernel/NTuple.h" );
  files.push_back( "/GaudiKernel/IHistogramSvc.h" );

  const std::string directory = System::getEnv( "GAUDIKERNELROOT" );

  StatEntity cnt;
  for ( Files::const_iterator ifile = files.begin(); files.end() != ifile; ++ifile ) {

    std::string   from = directory + ( *ifile );
    std::ifstream input( from.c_str() );

    //
    std::string value;
    while ( input >> value ) { cnt += Gaudi::Math::blind( value, 0, 1 ); }
  }
  //
  std::cout << " Counter entries      : " << cnt.nEntries() << std::endl
            << "         mean         : " << cnt.mean() << " +- " << cnt.meanErr() << std::endl
            << "         RMS*sqrt(12) : " << cnt.rms() * std::sqrt( 12.0 ) << std::endl
            << "         min/max      : " << cnt.min() << " / " << cnt.max() << std::endl;

  // ==========================================================================
  exit( 0 ); // EXIT
  // ==========================================================================
}
// ============================================================================
// The END
// ============================================================================
