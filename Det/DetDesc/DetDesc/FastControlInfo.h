#ifndef DETDESC_FASTCONTROLINFO_H
#define DETDESC_FASTCONTROLINFO_H 1

#include <string>

// Base classes
#include "ConditionInfo.h"
#include "DetDesc/IFastControl.h"

struct IDetectorElement;

///---------------------------------------------------------------------------
/** @class FastControlInfo FastControlInfo.h Det/DetDesc/FastControlInfo.h

    The simplest implementation of the IFastControl abstract interface,
    derived from the generic class ConditionInfo.

    @author Andrea Valassi 
    @date December 2001
*///--------------------------------------------------------------------------

struct FastControlInfo: ConditionInfo, virtual IFastControl
{
  /// Constructor
  FastControlInfo( IDetectorElement* de, const std::string& condition )
  : ConditionInfo( de, condition ) {}

};

#endif // DETDESC_FASTCONTROLINFO_H
