#ifndef DETDESC_SLOWCONTROLINFO_H
#define DETDESC_SLOWCONTROLINFO_H 1

#include <string>

// Base classes
#include "ConditionInfo.h"
#include "DetDesc/ISlowControl.h"

struct IDetectorElement;

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
