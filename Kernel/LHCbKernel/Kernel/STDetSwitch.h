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
#ifndef _STDetSwitch_H
#define _STDetSwitch_H

#include <string>

/** 
* Switch the type around (IT - OT and vice versa)
* @param type to switch to
* @param string to switch
*/
namespace STDetSwitch{

void flip(std::string aType, std::string& aString);

}
#endif
