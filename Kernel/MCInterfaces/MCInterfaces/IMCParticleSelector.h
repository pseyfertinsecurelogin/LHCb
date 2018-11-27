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
#ifndef MCINTERFACES_IMCPARTICLESELECTOR_H
#define MCINTERFACES_IMCPARTICLESELECTOR_H

#include "GaudiKernel/IAlgTool.h"

namespace LHCb {
  class MCParticle;
}

/** @class IMCParticleSelector IMCParticleSelector.h MCInterfaces/IMCParticleSelector.h
 *
 *  Interface Class for selection of particles given a criteria
 *
 *  @author M.Needham
 *  @date   13/3/2002
 */


struct IMCParticleSelector : extend_interfaces<IAlgTool> {

  /// Static access to interface id
  DeclareInterfaceID(IMCParticleSelector, 1 , 0);

  /** Method to select or reject a given MCParticle
   *  @param aParticle Pointer to the MCParticle to consider
   *  @return Boolean indicating if the given MCParticle is selected or not
   *  @retval true  MCParticle is selected
   *  @retval false MCParticle is rejected
   */
  virtual bool accept( const LHCb::MCParticle* aParticle ) const = 0;

  /** Boolean operator to select or reject a given MCParticle
   *  @param aParticle Pointer to the MCParticle to consider
   *  @return Boolean indicating if the given MCParticle is selected or not
   *  @retval true  MCParticle is selected
   *  @retval false MCParticle is rejected
   */
  inline bool operator() ( const LHCb::MCParticle* aParticle ) const
  {
    return this->accept(aParticle);
  }

};

#endif // MCINTERFACES_IMCPARTICLESELECTOR_H
