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
#ifndef DETDESC_ALIGNMENTINFO_H
#define DETDESC_ALIGNMENTINFO_H 1

// Base classes
#include "ConditionInfo.h"
#include "DetDesc/IAlignment.h"

///---------------------------------------------------------------------------
/** @class AlignmentInfo AlignmentInfo.h Det/DetDesc/AlignmentInfo.h

    The simplest implementation of the IAlignment abstract interface,
    derived from the generic class ConditionInfo.

    @author Andrea Valassi 
    @date December 2001
*///--------------------------------------------------------------------------

struct AlignmentInfo : ConditionInfo, virtual IAlignment
{
  /// Constructor
  // ROOT dictionaries don't like
  // using ConditionInfo::ConditionInfo;
  AlignmentInfo( IDetectorElement* de, const std::string& condition ) 
  : ConditionInfo(de,condition) {}

};

#endif // DETDESC_ALIGNMENTINFO_H
