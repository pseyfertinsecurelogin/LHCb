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

#include "Event/MCFun.h"

double LHCb::MC::zInteraction(const LHCb::MCParticle* aParticle)
{
  double z = 9999.*Gaudi::Units::m;
  for ( const auto & v : aParticle->endVertices() )
  {
    const auto zTest = v->position().z();
    if ( ( zTest < z ) && realInteraction( v->type() ) )
    {
      z = zTest;
    }
  }
  return z;
}

bool LHCb::MC::realInteraction(const LHCb::MCVertex::MCVertexType& type)
{
  return ! ( (type == LHCb::MCVertex::MCVertexType::PhotoElectric)     ||
             (type == LHCb::MCVertex::MCVertexType::RICHPhotoElectric) ||
             (type == LHCb::MCVertex::MCVertexType::Cerenkov)          ||
             (type == LHCb::MCVertex::MCVertexType::DeltaRay)          );
}
