/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
//
// Include files
// local
#include "Kernel/Particle2UnsignedInts.h"

// anonymous namespace to hide local symbols:
namespace 
{
   
  // ===========================================================================
  /** @var CLID_Candidate 
   *  Class ID definition 
   */
  static const CLID CLID_Particle2UnsignedInts = 882;
  
  // ==========================================================================
}
// ============================================================================
// Class ID 
// ============================================================================
const CLID& DaVinci::Map::Particle2UnsignedInts::classID() 
{
  return CLID_Particle2UnsignedInts;
}
//-----------------------------------------------------------------------------
