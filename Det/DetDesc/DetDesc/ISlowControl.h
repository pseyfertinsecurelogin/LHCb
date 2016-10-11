#ifndef DETDESC_ISLOWCONTROL_H
#define DETDESC_ISLOWCONTROL_H 1

// Base class
#include "DetDesc/IConditionInfo.h"

///---------------------------------------------------------------------------
/** @class ISlowControl ISlowControl.h Det/DetDesc/ISlowControl.h

    An abstract interface to get the slow control information of
    a detector element.

    @author Andrea Valassi 
    @date December 2001
*///--------------------------------------------------------------------------

struct  ISlowControl : virtual IConditionInfo
{

};

#endif // DETDESC_ISLOWCONTROL_H




