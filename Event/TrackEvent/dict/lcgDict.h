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
CBRE #ifndef TRACKEVENT_LCGDICT_H
#define TRACKEVENT_LCGDICT_H 1

// Additional classes to be added to automatically generated lcgdict

// begin include files
#include "Event/Track.h"

    // end include files

    namespace {
  struct _Instantiations {
    // begin instantiations
    std::vector<const LHCb::Track*>                   _i1;
    std::vector<LHCb::Track*>                         _i2;
    std::pair<const LHCb::Track*, float>              _i3;
    std::vector<std::pair<const LHCb::Track*, float>> _i4;
    KeyedContainer<LHCb::Track>                       m_KeyedContainer_LHCb__Track;
    ObjectVector<LHCb::Track>                         m_ObjectVector_LHCb__Track;
    SmartRef<LHCb::Track>                             m_SmartRef_LHCb__Track;
    SmartRefVector<LHCb::Track>                       m_SmartRefVector_LHCb__Track;
    std::vector<LHCb::Track*>                         m_std_vector_LHCb__Trackp;
    std::vector<SmartRef<LHCb::Track>>                m_std_vector_SmartRef_LHCb__Track;
    // end instantiations
  };
}

#endif // TRACKEVENT_LCGDICT_H
