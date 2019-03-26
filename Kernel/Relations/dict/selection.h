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
// ====================================================================
// Include file
// ====================================================================
// STD & STL
// ====================================================================
#include <string>
#include <vector>
// ====================================================================
// Relations
// ====================================================================
#include "Relations/RelationsDict.h"
// ====================================================================
// Other include files: put your own includes here:
// ====================================================================
//#include ...
//#include ...
//#include ...
// ====================================================================

/** @file
 *
 *  Helper file for building the simplest Reflex dictionaries
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-02-05
 */

namespace RelationsDict {
  struct __Instantiations {
    GaudiDict::Relation1DDict<int, int>         _1;
    GaudiDict::Relation2DDict<std::string, int> _2;
  };
} // namespace RelationsDict

// ====================================================================
// The END
// ====================================================================
