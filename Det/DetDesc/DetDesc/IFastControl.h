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
#ifndef DETDESC_IFASTCONTROL_H
#define DETDESC_IFASTCONTROL_H 1

// Base class
#include "DetDesc/IConditionInfo.h"

///---------------------------------------------------------------------------
/** @class IFastControl IFastControl.h Det/DetDesc/IFastControl.h

    An abstract interface to get the slow control information of
    a detector element.

    @author Andrea Valassi
    @date December 2001
*///--------------------------------------------------------------------------

struct IFastControl : virtual IConditionInfo {};

#endif // DETDESC_IFASTCONTROL_H
