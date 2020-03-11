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
// STD&STL
// ============================================================================
#include <iostream>
// ============================================================================
// Relations
// ============================================================================
#include "Relations/Relation1D.h"
#include "Relations/RelationWeighted1D.h"
#include "Relations/RelationsDict.h"
// ============================================================================
/** @file
 *
 *  Auxillary test-file for the package Kernel/Relations
 *  to detect the obvious problems
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2009-04-09
 */
// ============================================================================
int main() {
  LHCb::Relation1D<int, float>                 t1;
  LHCb::Relation2D<std::string, int>           t2;
  LHCb::RelationWeighted1D<int, float, double> t3;
  LHCb::RelationWeighted2D<int, float, double> t4;

  t1.relate( 0, 0.0 ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
  t1.relate( 1, 1.0 ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
  t1.relate( 2, 2.0 ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );

  t2.relate( "0", 0 ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
  t2.relate( "1", 1 ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
  t2.relate( "2", 2 ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );

  t3.relate( 0, 0.f, 0.0 ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
  t3.relate( 1, 1.f, 1.0 ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
  t3.relate( 2, 2.f, 2.0 ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );

  t4.relate( 0, 0.f, 0.0 ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
  t4.relate( 1, 1.f, 1.0 ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
  t4.relate( 2, 2.f, 2.0 ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );

  std::cout << " Table size: "
            << " t1 " << t1.relations().size() << " t2 " << t2.relations().size() << " t3 " << t3.relations().size()
            << " t4 " << t4.relations().size() << std::endl;
}
// ============================================================================
// The END
// ============================================================================
