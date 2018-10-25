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
#ifndef MCINTERFACES_IHEPMCPARTICLESELECTOR_H
#define MCINTERFACES_IHEPMCPARTICLESELECTOR_H

#include "GaudiKernel/IAlgTool.h"

namespace HepMC {
  class GenParticle;
}

/** @class IHepMCParticleSelector IHepMCParticleSelector.h MCInterfaces/IHepMCParticleSelector.h
 *
 *  Interface Class for selection of particles given a criteria
 *
 *  @author Victor Coco victor.coco@cern.ch
 *  @date   22.10.2009
 */


struct IHepMCParticleSelector : extend_interfaces<IAlgTool> {


  /// Static access to interface id
  DeclareInterfaceID(IHepMCParticleSelector, 1 , 0);

  /** Method to select or reject a given HepMCParticle
   *  @param aParticle Pointer to the HepMCParticle to consider
   *  @return Boolean indicating if the given HepMCParticle is selected or not
   *  @retval true  HepMCParticle is selected
   *  @retval false HepMCParticle is rejected
   */
  virtual bool accept( const HepMC::GenParticle* aParticle ) const = 0;

  /** Boolean operator to select or reject a given HepMCParticle
   *  @param aParticle Pointer to the HepMCParticle to consider
   *  @return Boolean indicating if the given HepMCParticle is selected or not
   *  @retval true  HepMCParticle is selected
   *  @retval false HepMCParticle is rejected
   */
  inline bool operator() ( const HepMC::GenParticle* aParticle ) const
  {
    return this->accept(aParticle);
  }

};

#endif // MCINTERFACES_IMCPARTICLESELECTOR_H
