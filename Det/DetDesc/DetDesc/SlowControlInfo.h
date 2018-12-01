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
#ifndef DETDESC_SLOWCONTROLINFO_H
#define DETDESC_SLOWCONTROLINFO_H 1

#include <string>

// Base classes
#include "ConditionInfo.h"
#include "DetDesc/ISlowControl.h"

///---------------------------------------------------------------------------
/** @class SlowControlInfo SlowControlInfo.h Det/DetDesc/SlowControlInfo.h

    The simplest implementation of the ISlowControl abstract interface,
    derived from the generic class ConditionInfo.

    @author Andrea Valassi 
    @date December 2001
*///--------------------------------------------------------------------------

struct SlowControlInfo: ConditionInfo, virtual ISlowControl
{
  /// Constructor
  SlowControlInfo( IDetectorElement* de,
		   const std::string& condition )
  : ConditionInfo( de, condition ) {}

};

#endif // DETDESC_SLOWCONTROLINFO_H
