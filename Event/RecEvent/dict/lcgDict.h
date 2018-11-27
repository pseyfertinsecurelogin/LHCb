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
// $Id: lcgDict.h,v 1.2 2008-02-15 07:42:17 cattanem Exp $
#ifndef RECEVENT_LCGDICT_H 
#define RECEVENT_LCGDICT_H 1

// Additional classes to be added to automatically generated lcgdict

// begin include files
#include <vector>
#include "Event/RecVertex.h"
#include "Event/VertexBase.h"
#include "Event/TwoProngVertex.h"
// end include files

namespace 
{
  struct _Instantiations 
  {
    // begin instantiations
    std::vector<const LHCb::RecVertex*>  _i1;
    std::vector<const LHCb::VertexBase*> _i2;
    std::pair<int,float>                 _i3;
    std::vector<std::pair<int,float> >   _i4;
    Gaudi::SymMatrix9x9                  _i5;
    // end instantiations
  };
}

#endif // RECEVENT_LCGDICT_H
