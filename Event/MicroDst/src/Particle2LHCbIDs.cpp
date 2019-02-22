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
// $Id: Particle2LHCbIDs.cpp,v 1.1 2010/08/19 15:09:43 jpalac Exp $
// Include files
// local
#include "Kernel/Particle2LHCbIDs.h"

// anonymous namespace to hide local symbols:
namespace {

  // ===========================================================================
  /** @var CLID_Candidate
   *  Class ID definition
   */
  static const CLID CLID_Particle2LHCbIDs = 880;

  // ==========================================================================
} // namespace
// ============================================================================
// Class ID
// ============================================================================
const CLID& DaVinci::Map::Particle2LHCbIDs::classID() { return CLID_Particle2LHCbIDs; }
//-----------------------------------------------------------------------------
