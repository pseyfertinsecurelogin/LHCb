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
#include "Event/FlavourTag.h"
#include "Event/Particle.h"
#include "Event/RelatedInfoMap.h"
#include "Event/Tagger.h"
#include "Event/Vertex.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include <vector>

// end include files

namespace {
  struct PhysEvent_Instantiations {
    // begin instantiations
    //
    Gaudi::NamedRange_<LHCb::Particle::ConstVector> _i5;
    Gaudi::Range_<LHCb::Particle::ConstVector>      _i6;
    KeyedContainer<LHCb::FlavourTag>                m_KeyedContainer_LHCb__FlavourTag;
    KeyedContainer<LHCb::Particle>                  m_KeyedContainer_LHCb__Particle;
    KeyedContainer<LHCb::Vertex>                    m_KeyedContainer_LHCb__Vertex;
    LHCb::Particle::Selection                       _i4;
    LHCb::RelatedInfoMap                            _i7;
    LHCb::RelatedInfoMap::MAP                       _i8;
    ObjectVector<LHCb::FlavourTag>                  m_ObjectVector_LHCb__FlavourTag;
    ObjectVector<LHCb::Particle>                    m_ObjectVector_LHCb__Particle;
    ObjectVector<LHCb::Vertex>                      m_ObjectVector_LHCb__Vertex;
    SmartRef<LHCb::FlavourTag>                      m_SmartRef_LHCb__FlavourTag;
    SmartRef<LHCb::Particle>                        m_SmartRef_LHCb__Particle;
    SmartRef<LHCb::Vertex>                          m_SmartRef_LHCb__Vertex;
    SmartRefVector<LHCb::FlavourTag>                m_SmartRefVector_LHCb__FlavourTag;
    SmartRefVector<LHCb::Particle>                  m_SmartRefVector_LHCb__Particle;
    SmartRefVector<LHCb::Vertex>                    m_SmartRefVector_LHCb__Vertex;
    std::vector<LHCb::FlavourTag*>                  m_std_vector_LHCb__FlavourTagp;
    std::vector<LHCb::FlavourTag>                   m_std_vector_LHCb__FlavourTag;
    std::vector<LHCb::Particle*>                    m_std_vector_LHCb__Particlep;
    std::vector<LHCb::Tagger>                       m_std_vector_LHCb__Tagger;
    std::vector<LHCb::Vertex*>                      m_std_vector_LHCb__Vertexp;
    std::vector<SmartRef<LHCb::FlavourTag>>         m_std_vector_SmartRef_LHCb__FlavourTag;
    std::vector<SmartRef<LHCb::Particle>>           m_std_vector_SmartRef_LHCb__Particle;
    std::vector<SmartRef<LHCb::Vertex>>             m_std_vector_SmartRef_LHCb__Vertex;
    std::vector<const LHCb::Particle*>              _i1;
    std::vector<const LHCb::Particle*>::iterator    _i2;
    std::vector<const LHCb::Vertex*>                _i3;
    // end instantiations
  };
} // namespace
