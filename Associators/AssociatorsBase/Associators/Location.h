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
#ifndef ASSOCIATORS_LOCATION_H
#define ASSOCIATORS_LOCATION_H

namespace Links {
inline std::string location(const std::string& location)
{
   return "Link/" + (location.compare(0, 7, "/Event/") == 0 ? location.substr(7) : location);
}
}

#endif
