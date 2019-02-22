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
#ifndef MCEVENT_MCFUN_H
#define MCEVENT_MCFUN_H 1

#include "Event/MCParticle.h"
#include "Event/MCVertex.h"

namespace LHCb {
  namespace MC {
    /** z of the first interaction vertex of a particle
    * Silly interactions PhotoElectric, RICHPhotoElectric
                         Cerenkov and DeltaRay are ignored
    * @param particle
    * @return z of first interaction
    */
    double zInteraction( const LHCb::MCParticle* aParticle );

    /**
     * Silly interactions  PhotoElectric, RICHPhotoElectric
     *                     Cerenkov and DeltaRay return false
     *  @param type
     *  @return true if not a silly vertex
     */
    bool realInteraction( const LHCb::MCVertex::MCVertexType& type );
  } // namespace MC
} // namespace LHCb

#endif // MCEVENT_MCFUN_H
