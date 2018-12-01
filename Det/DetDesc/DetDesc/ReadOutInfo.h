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
#ifndef DETDESC_READOUTINFO_H
#define DETDESC_READOUTINFO_H 1

// Base classes
#include "ConditionInfo.h"
#include "DetDesc/IReadOut.h"

///---------------------------------------------------------------------------
/** @class ReadOutInfo ReadOutInfo.h Det/DetDesc/ReadOutInfo.h

    The simplest implementation of the IReadOut abstract interface,
    derived from the generic class ConditionInfo.

    @author Andrea Valassi 
    @date December 2001
*///--------------------------------------------------------------------------

struct ReadOutInfo: ConditionInfo, virtual IReadOut
{

  /// Constructor
  ReadOutInfo( IDetectorElement* de, const std::string& condition )
  : ConditionInfo( de, condition ) {}

};

#endif // DETDESC_READOUTINFO_H
