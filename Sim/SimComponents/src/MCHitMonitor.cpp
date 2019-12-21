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

#include "AIDA/IHistogram1D.h"
#include "AIDA/IHistogram2D.h"
#include "Event/MCHit.h"
#include "Event/MCParticle.h"
#include "GaudiAlg/GaudiHistoAlg.h"
#include "GaudiKernel/SystemOfUnits.h"
#include "LHCbMath/ModeFunctions.h"
#include "MCInterfaces/IMCParticleSelector.h"
#include <string>
#include <vector>

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
  ToolHandle<IMCParticleSelector> m_selector{this, "selectorName", "MCParticleSelector"};

  std::vector<IHistogram1D*> m_timeOfFlightHistos;
  std::vector<IHistogram2D*> m_XvsYHistos;
  std::vector<IHistogram1D*> m_EnergyLossHistos;

  mutable std::vector<double> m_energyVec;

  // job Options
  DataObjectReadHandle<LHCb::MCHits>   m_mcHits{this, "mcPathString", ""};
  Gaudi::Property<std::vector<double>> m_Zstations{this, "zStations", {}};
  Gaudi::Property<double>              m_Xmax{this, "xMax", 400.0 * Gaudi::Units::cm};
  Gaudi::Property<double>              m_Ymax{this, "yMax", 400.0 * Gaudi::Units::cm};
  Gaudi::Property<double>              m_TMax{this, "tMax", 100.0 * Gaudi::Units::ns};
  Gaudi::Property<double>              m_TMin{this, "tMin", 0.0 * Gaudi::Units::ns};
  Gaudi::Property<double>              m_EMax{this, "eMax", 1.0 * Gaudi::Units::MeV};
  Gaudi::Property<double>              m_MaxPath{this, "maxPath", 5.0 * Gaudi::Units::cm};
  Gaudi::Property<double>              m_ZTolerance{this, "zTolerance", 20.0 * Gaudi::Units::cm};
  Gaudi::Property<double>              m_minPathLength{this, "minPathLength", 0.1 * Gaudi::Units::mm};
  Gaudi::Property<unsigned int>        m_nToCollect{this, "nToCollect", 20000};

  // counters
  mutable Gaudi::Accumulators::AveragingCounter<>             m_betaGamma_counter{this, "betaGamma"};
  mutable Gaudi::Accumulators::Counter<>                      m_DeltaRay_counter{this, "DeltaRay"};
  mutable Gaudi::Accumulators::AveragingCounter<unsigned int> m_numberHits_counter{this, "numberHits"};
};

//
// This File contains the implementation of the MCHitMonitor
//
//   Author:Matthew Needham
//   Created: 18-05-1999

// Needed for the creation of MCHitMonitor objects.
DECLARE_COMPONENT( MCHitMonitor )

//_________________________________________________
/// MCHitMonitor
///
/// An MCHitMonitor is a Gaudi top level Algorithm that checks the
/// output of the event tracks fitter

MCHitMonitor::MCHitMonitor( const std::string& name, ISvcLocator* pSvcLocator ) : GaudiHistoAlg( name, pSvcLocator ) {
  /// MCHitMonitor constructor
}

StatusCode MCHitMonitor::initialize() {
  // initialize

  return GaudiHistoAlg::initialize().andThen( [&] {
    // initialize histograms
    initHistograms();

    m_energyVec.reserve( m_nToCollect );
  } );
}

void MCHitMonitor::initHistograms() {
  /// Intialize histograms
  std::string tPath = histoPath() + "/";

  // make histograms per stations
  IHistogram1D* aHisto1D = nullptr;
  IHistogram2D* aHisto2D = nullptr;

  int nStation = m_Zstations.size();
  for ( int iStation = 0; iStation < nStation; ++iStation ) {

    aHisto1D = book( 100 + iStation, "time of Flight" + std::to_string( 100 + iStation ), m_TMin, m_TMax, 200 );
    m_timeOfFlightHistos.push_back( aHisto1D );

    // x vs y plots
    aHisto2D = histoSvc()->book( tPath + std::to_string( 200 + iStation ), "x vs y" + std::to_string( 200 + iStation ),
                                 200, -m_Xmax / Gaudi::Units::cm, m_Xmax / Gaudi::Units::cm, 200,
                                 -m_Ymax / Gaudi::Units::cm, m_Ymax / Gaudi::Units::cm );
    m_XvsYHistos.push_back( aHisto2D );

    aHisto1D = book( 300 + iStation, "lossHisto" + std::to_string( 300 + iStation ), 0., m_EMax, 250 );

    m_EnergyLossHistos.push_back( aHisto1D );

  } // loop stations
}

StatusCode MCHitMonitor::execute() {
  /// Executes MCHitMonitor for one event.

  const LHCb::MCHits* hitsCont = m_mcHits.get();

  plot( (double)hitsCont->size(), 1, "num hits", 0., 5000, 100 );
  m_numberHits_counter += hitsCont->size();

  // loop over hits fill some histograms
  for ( const auto& ihit : *hitsCont ) fillHistograms( ihit ).ignore();

  return StatusCode::SUCCESS;
}

StatusCode MCHitMonitor::finalize() {

  std::sort( m_energyVec.begin(), m_energyVec.end() );
  const double shorth    = ModeFunctions::shorth( m_energyVec.begin(), m_energyVec.end() );
  const double halfWidth = ModeFunctions::halfSampleWidth( m_energyVec.begin(), m_energyVec.end() );
  info() << "*** Summary ***" << endmsg;
  info() << "#hits per event: " << m_numberHits_counter.mean() << endmsg;
  info() << "Mean beta * gamma: " << m_betaGamma_counter.mean() << endmsg;
  info() << "Most Probable deposited charge: " << shorth << endmsg;
  info() << "Half Sample width " << halfWidth << endmsg;

  return GaudiHistoAlg::finalize();
}

StatusCode MCHitMonitor::fillHistograms( const LHCb::MCHit* aHit ) const {

  // Initialize

  // check 1 can find pointer to MCparticle
  const LHCb::MCParticle* aParticle = aHit->mcParticle();
  if ( !aParticle ) return Error( "failed to find MCParticle" );

  const LHCb::MCVertex* vertex = aParticle->originVertex();
  if ( vertex ) {
    if ( vertex->type() == LHCb::MCVertex::MCVertexType::DeltaRay ) ++m_DeltaRay_counter;
  }

  // p
  plot( aParticle->p() / Gaudi::Units::GeV, 4, "pMag", 0., 100, 200 );

  // average of entrance and exit...
  Gaudi::XYZPoint mcHitPoint = aHit->midPoint();

  plot( aHit->pathLength(), "path length", 0., m_MaxPath, 200 );

  // bin in regions of z ~ stations
  const int iStation = getStationID( mcHitPoint.z() );

  if ( iStation < 0 ) {
    err() << "failed to get station number for z " << mcHitPoint.z() << endmsg;
    return StatusCode::FAILURE;
  }

  if ( m_selector->accept( aHit->mcParticle() ) == true ) {

    // loss
    m_EnergyLossHistos[iStation]->fill( aHit->energy(), 1.0 );

    // dE/dX
    m_timeOfFlightHistos[iStation]->fill( aHit->time() / Gaudi::Units::ns, 1.0 );
  }

  // scatter plot of x-y of hit
  m_XvsYHistos[iStation]->fill( mcHitPoint.x() / Gaudi::Units::cm, mcHitPoint.y() / Gaudi::Units::cm, 1.0 );

  if ( aHit->pathLength() > m_minPathLength ) m_energyVec.push_back( aHit->energy() );

  m_betaGamma_counter += aParticle->beta() * aParticle->gamma();

  return StatusCode::SUCCESS;
}

int MCHitMonitor::getStationID( const double z ) const {

  // loop and pick up the best z
  int    iStation = -1;
  double zBest    = 99999.;
  double zDiff;
  for ( unsigned int iPos = 0; iPos < m_Zstations.size(); ++iPos ) {
    zDiff = fabs( m_Zstations[iPos] - z );
    if ( zDiff < zBest ) {
      zBest    = zDiff;
      iStation = (int)iPos;
    }
  } // iPos

  // check match reasonable
  if ( zBest > m_ZTolerance ) iStation = -1;

  return iStation;
}
