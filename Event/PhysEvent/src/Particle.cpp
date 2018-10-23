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

// STD
#include <iostream>

// local
#include "Event/Particle.h"

// Kernel
#include "LHCbMath/MatrixManip.h"

// Gaudi
#include "GaudiKernel/IRegistry.h"

//=============================================================================

Gaudi::SymMatrix7x7 LHCb::Particle::covMatrix() const
{
  Gaudi::Matrix7x7 full;

  full.Place_at(m_posCovMatrix,0,0);
  full.Place_at(m_momCovMatrix,3,3);
  full.Place_at(m_posMomCovMatrix,3,0);
  full.Place_at(ROOT::Math::Transpose(m_posMomCovMatrix),0,3);

  return Gaudi::SymMatrix7x7( full.LowerBlock() );
}

//=============================================================================

std::ostream& LHCb::Particle::fillStream(std::ostream& s) const
{
  s << "{ "
    << "particleID      : " << m_particleID << std::endl
    << "measuredMass    : " << m_measuredMass << std::endl
    << "measuredMassErr : " << m_measuredMassErr << std::endl
    << "momentum        : " << m_momentum << std::endl
    << "referencePoint  : " << m_referencePoint << std::endl
    << "momCovMatrix    : " << std::endl << m_momCovMatrix << std::endl
    << "posCovMatrix    : " << std::endl << m_posCovMatrix << std::endl
    << "posMomCovMatrix : " << std::endl << m_posMomCovMatrix << std::endl
    << "extraInfo       : [";
  for ( const auto & i : extraInfo() )
  {
    if ( i.first < (int)LHCb::Particle::additionalInfo::LastGlobal )
    {
      const auto info =
        static_cast<LHCb::Particle::additionalInfo>(i.first);
      s << " " << info << "=" << i.second;
    }
    else
    {
      s << " " << i.first << "=" << i.second;
    }
  }
  s << " ]";
  const std::string testLocation =
    ( parent() && parent()->registry() ?
      parent()->registry()->identifier() : "" );
  if ( !testLocation.empty() ) { s << std::endl << "TES=" << testLocation; }
  return s << " }";
}

//=============================================================================
