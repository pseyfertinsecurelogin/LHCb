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
// $Id: lcg_selection.h,v 1.4 2010-03-17 21:30:50 raaij Exp $
#ifndef L0EVENT_LCGDICT_H 
#define L0EVENT_LCGDICT_H 1

// Additional classes to be added to automatically generated lcgdict

// begin include files
#include <map>
#include <string>
#include <utility>
#include "Event/L0DUChannel.h"
// end include files

template std::map<int,unsigned int>;
template std::map<std::string,LHCb::L0DUChannel*>;
template std::map<std::string,LHCb::L0DUTrigger*>;
template std::map<std::string,LHCb::L0DUChannel*>::iterator;
template std::pair<const std::string,LHCb::L0DUChannel*>;

#endif // L0EVENT_LCGDICT_H
