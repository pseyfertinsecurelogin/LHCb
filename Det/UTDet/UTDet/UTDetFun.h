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
#ifndef _UTDetFun_H_
#define _UTDetFun_H_

#include "UTDet/DeUTBaseElement.h"
#include <string>

namespace UTDetFun {

  inline const auto SortByY = []( const DeUTBaseElement* obj1, const DeUTBaseElement* obj2 ) {
    return ( !obj1 ) ? true : ( !obj2 ) ? false : obj1->globalCentre().y() < obj2->globalCentre().y();
  };

  /// equal by (nick)name
  inline const auto equal_by_name = []( std::string testname ) {
    return [n = std::move( testname )]( const auto& obj ) { return obj->nickname() == n; };
  };

} // namespace UTDetFun

#endif // _UTDataFunctor_H_
