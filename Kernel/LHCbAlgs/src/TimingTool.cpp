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
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/Chrono.h"
#include "GaudiKernel/IRndmGenSvc.h"
#include "GaudiKernel/RndmGenerators.h"
#include "Kernel/INormalizeTool.h"
//-----------------------------------------------------------------------------
// Implementation file for class : TimingTool
//
// 2003-11-04 : Marco Cattaneo
//-----------------------------------------------------------------------------
/** @class TimingTool TimingTool.h
 *  Tool to get a timing normalisation for the Chrono service
 *  Adapted from BrunelAlgs/TimingAlg by M.Ferro-Luzzi
 *
 *  @author Marco Cattaneo
 *  @date   2003-11-04
 */
class TimingTool final : public extends<GaudiTool, INormalizeTool> {

public:
  /// Standard constructor
  using extends::extends;

  StatusCode finalize() override;

  double          normalize() override; ///< Return normalization (in ns)
  IChronoStatSvc* chronoSvc();          ///< Returns pointer to ChronSvc
  IRndmGenSvc*    randSvc();            ///< Returns pointer to random number service

private:
  Gaudi::Property<unsigned int> m_shots{this, "shots", 2100000}; ///< Number of random number shots for normalisation //
                                                                 ///< 1s on 1GHz PIII, gcc 3.2, -O2
  IChronoStatSvc* m_CSS = nullptr;                               ///< Pointer to Chrono service
  IRndmGenSvc*    m_RGS = nullptr;                               ///< Pointer to Random numbers service
};

// Declaration of the Tool Factory
DECLARE_COMPONENT( TimingTool )

//=============================================================================
double TimingTool::normalize() {
  //=============================================================================

  // Get normalization time for this job:
  IChronoStatSvc::ChronoTag normstamp( "TimingTool_normStamp" );
  chronoSvc()->chronoStart( normstamp );
  double sum = 0;

  { // Use dummy loop suggested by Vanya Belyaev:
    Rndm::Numbers gauss;
    gauss.initialize( randSvc(), Rndm::Gauss( 0., 1.0 ) ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    unsigned int shots = m_shots;
    while ( 0 < --shots ) { sum += gauss() * sum; }
  }

  chronoSvc()->chronoStop( normstamp );
  double usertimenorm;
  usertimenorm = chronoSvc()->chronoDelta( normstamp, IChronoStatSvc::USER ); // us
  usertimenorm = usertimenorm * 1000.0;                                       // ns

  return usertimenorm;
}

//=============================================================================
StatusCode TimingTool::finalize()
//=============================================================================
{
  m_CSS = nullptr;
  m_RGS = nullptr;
  return GaudiTool::finalize();
}

//=============================================================================
IRndmGenSvc* TimingTool::randSvc() {
  //=============================================================================
  if ( !m_RGS ) m_RGS = svc<IRndmGenSvc>( "RndmGenSvc", true );
  return m_RGS;
}

//=============================================================================
IChronoStatSvc* TimingTool::chronoSvc() {
  //=============================================================================
  if ( !m_CSS ) m_CSS = svc<IChronoStatSvc>( "ChronoStatSvc", true );
  return m_CSS;
}

//=============================================================================
