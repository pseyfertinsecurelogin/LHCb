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
// ============================================================================
#ifndef LOKI_LOKICORE_DCT_H
#define LOKI_LOKICORE_DCT_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/HistoDef.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GaudiTool.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Dicts.h"
#include "LoKi/Functors.h"
#include "LoKi/HistoBook.h"
#include "LoKi/Print.h"
#include "LoKi/Services.h"
// ============================================================================
#include "LoKi/Assert.h"
#include "LoKi/Base.h"
#include "LoKi/Constants.h"
#include "LoKi/Context.h"
#include "LoKi/CoreEngine.h"
#include "LoKi/DecayChainBase.h"
#include "LoKi/Dump.h"
#include "LoKi/Geometry.h"
#include "LoKi/ICoreAntiFactory.h"
#include "LoKi/ICoreFactory.h"
#include "LoKi/IDecayNode.h"
#include "LoKi/Keeper.h"
#include "LoKi/KeeperBase.h"
#include "LoKi/Kinematics.h"
#include "LoKi/Listener.h"
#include "LoKi/MissingParticle.h"
#include "LoKi/Monitor.h"
#include "LoKi/Monitoring.h"
#include "LoKi/Param.h"
#include "LoKi/ParticleProperties.h"
#include "LoKi/PidFunctions.h"
#include "LoKi/Polarization.h"
#include "LoKi/Print.h"
#include "LoKi/RecStat.h"
#include "LoKi/Report.h"
#include "LoKi/TES.h"
#include "LoKi/Tensors.h"
#include "LoKi/Trees.h"
#include "LoKi/UniqueKeeper.h"
#include "LoKi/Welcome.h"
#include "LoKi/iTree.h"
// ============================================================================
namespace {
  // ==========================================================================
  struct _Interfaces {
    // ========================================================================
    LoKi::Interface<LoKi::ILoKiSvc>   m_1;
    LoKi::Interface<LoKi::IReporter>  m_2;
    LoKi::Interface<GaudiTool>        m_3;
    LoKi::Interface<GaudiAlgorithm>   m_4;
    LoKi::Interface<IDataProviderSvc> m_5;
    // fictive constructor
    _Interfaces();
    // ========================================================================
  };
  // ==========================================================================
} // namespace
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_LOKICORE_DCT_H
