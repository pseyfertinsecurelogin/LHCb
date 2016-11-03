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
