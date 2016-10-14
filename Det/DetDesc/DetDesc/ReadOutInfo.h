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
