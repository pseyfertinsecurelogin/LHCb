#ifndef DETDESC_IREADOUT_H
#define DETDESC_IREADOUT_H 1

// Base class
#include "DetDesc/IConditionInfo.h"

///---------------------------------------------------------------------------
/** @class IReadOut IReadOut.h Det/DetDesc/IReadOut.h

    An abstract interface to get the readout information of
    a detector element.

    @author Andrea Valassi 
    @date December 2001
*///--------------------------------------------------------------------------

struct IReadOut : virtual IConditionInfo
{

};

#endif // DETDESC_IREADOUT_H




