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
#ifndef _MCHitMonitor_H
#define _MCHitMonitor_H

#include "GaudiAlg/GaudiHistoAlg.h"

#include <string>
#include <vector>

namespace LHCb {
  class MCHit;
  class MCParticle;
} // namespace LHCb

namespace AIDA {
  class IHistogram1D;
  class IHistogram2D;
} // namespace AIDA

struct IMCParticleSelector;

/** class MCHitMonitor, package MCHitMonitor
 *  Top Level Algorithm that manages MCHits digitization code
 *
 *  @author M.Needham
 *  @date 04/12/2008 [revised]
 *  @date   21/10/2000
 */

class MCHitMonitor : public GaudiHistoAlg {

public:
  /// Constructer
  MCHitMonitor( const std::string& name, ISvcLocator* pSvcLocator );

  /// intialize
  StatusCode initialize() override;

  /// execute
  StatusCode execute() override;

  /// finalize
  StatusCode finalize() override;

private:
  /// init histograms
  void initHistograms();

  /// fill histograms
  StatusCode fillHistograms( const LHCb::MCHit* aHit ) const;

  /// station number for a given z
  int getStationID( const double z ) const;

  // selector
  std::string          m_selectorName;
  IMCParticleSelector* m_selector = nullptr;

  std::vector<IHistogram1D*> m_timeOfFlightHistos;
  std::vector<IHistogram2D*> m_XvsYHistos;
  std::vector<IHistogram1D*> m_EnergyLossHistos;

  mutable std::vector<double> m_energyVec;

  // job Options
  std::string         m_MCHitPath;
  std::vector<double> m_Zstations;
  double              m_Xmax;
  double              m_Ymax;
  double              m_TMax;
  double              m_TMin;
  double              m_EMax;
  double              m_MaxPath;
  double              m_ZTolerance;
  double              m_minPathLength;
  unsigned int        m_nToCollect;

  // counters
  mutable Gaudi::Accumulators::AveragingCounter<>             m_betaGamma_counter{this, "betaGamma"};
  mutable Gaudi::Accumulators::Counter<>                      m_DeltaRay_counter{this, "DeltaRay"};
  mutable Gaudi::Accumulators::AveragingCounter<unsigned int> m_numberHits_counter{this, "numberHits"};
};

#endif // TrMCHitMonitor_H
