/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef LINKER_LINKEDFROMKEY_H 
#define LINKER_LINKEDFROMKEY_H 1

// Include files
#include "Linker/LinkedFrom.h"

/** @class LinkedFromKey LinkedFromKey.h Linker/LinkedFromKey.h
 *  Simple helper class to handle LinkedFrom with no source, i.e. key linker
 *
 *  @author Olivier Callot
 *  @date   2008-03-14
 */

template <class TARGET, class KEY=int> 
using LinkedFromKey = LinkedFrom<ContainedObject, TARGET, KEY>;

#endif // LINKER_LINKEDFROMKEY_H
