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
// $Id: TimingTool.cpp,v 1.3 2006-11-30 15:57:28 cattanem Exp $
// Include files 

// from Gaudi
#include "GaudiKernel/RndmGenerators.h" 

// local
#include "TimingTool.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TimingTool
//
// 2003-11-04 : Marco Cattaneo
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TimingTool )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TimingTool::TimingTool( const std::string& type,
                        const std::string& name,
                        const IInterface* parent )
  : GaudiTool ( type, name , parent )
{
  declareInterface<INormalizeTool>(this);

  declareProperty( "shots", m_shots = 2100000 ); // 1s on 1GHz PIII, gcc 3.2, -o2
}

//=============================================================================
double TimingTool::normalize() {
//=============================================================================

  // Get normalization time for this job:
  IChronoStatSvc::ChronoTag normstamp("TimingTool_normStamp");
  chronoSvc()->chronoStart(normstamp);
  double sum = 0;

  { // Use dummy loop suggested by Vanya Belyaev:
    Rndm::Numbers gauss;
    gauss.initialize( randSvc() , Rndm::Gauss(0.,1.0) );
    unsigned int shots = m_shots;
    while( 0 < --shots ) { sum += gauss() * sum ; }
  }

  chronoSvc()->chronoStop(normstamp);
  double usertimenorm;
  usertimenorm = chronoSvc()->chronoDelta(normstamp, IChronoStatSvc::USER);//us
  usertimenorm = usertimenorm * 1000.0 ;                                   //ns

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
