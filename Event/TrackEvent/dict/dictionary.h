/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

// begin include files
#include "Event/ChiSquare.h"
#include "Event/State.h"
#include "Event/StateVector.h"
#include "Event/Track.h"
#include "Event/Track_v1.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include <vector>

// end include files

namespace {
  struct TrackEvent_Instantiations {
    // begin instantiations
    KeyedContainer<LHCb::Event::Track>                m_KeyedContainer_LHCb__Event__Track;
    KeyedContainer<LHCb::Track>                       m_KeyedContainer_LHCb__Track;
    ObjectVector<LHCb::Event::Track>                  m_ObjectVector_LHCb__Event__Track;
    ObjectVector<LHCb::Track>                         m_ObjectVector_LHCb__Track;
    SmartRef<LHCb::Event::Track>                      m_SmartRef_LHCb__Event__Track;
    SmartRef<LHCb::Track>                             m_SmartRef_LHCb__Track;
    SmartRefVector<LHCb::Event::Track>                m_SmartRefVector_LHCb__Event__Track;
    SmartRefVector<LHCb::Track>                       m_SmartRefVector_LHCb__Track;
    std::pair<const LHCb::Track*, float>              _i3;
    std::vector<LHCb::Event::Track*>                  m_std_vector_LHCb__Event__Trackp;
    std::vector<LHCb::State*>                         m_std_vector_LHCb__Statep;
    std::vector<LHCb::StateVector*>                   m_std_vector_LHCb__StateVectorp;
    std::vector<LHCb::Track*>                         _i2;
    std::vector<LHCb::Track*>                         m_std_vector_LHCb__Trackp;
    std::vector<SmartRef<LHCb::Event::Track>>         m_std_vector_SmartRef_LHCb__Event__Track;
    std::vector<SmartRef<LHCb::Track>>                m_std_vector_SmartRef_LHCb__Track;
    std::vector<const LHCb::Track*>                   _i1;
    std::vector<std::pair<const LHCb::Track*, float>> _i4;
    // end instantiations
  };
} // namespace
