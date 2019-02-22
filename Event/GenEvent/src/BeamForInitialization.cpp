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
// local
#include "GenEvent/BeamForInitialization.h"

// Include files
#include "Event/BeamParameters.h"

//-----------------------------------------------------------------------------
// Implementation file for class : BeamForInitialization
//
// 2011-05-27 : Patrick Robbe
//-----------------------------------------------------------------------------

LHCb::BeamParameters* BeamForInitialization::s_initialBeamParameters = nullptr;

LHCb::BeamParameters*& BeamForInitialization::getInitialBeamParameters() { return s_initialBeamParameters; }
