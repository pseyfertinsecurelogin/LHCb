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
#ifndef KERNEL_PARTICLE2MCMETHOD_H
#define KERNEL_PARTICLE2MCMETHOD_H 1

#include <array>
#include <string>
// Include files

/** @namespace Particle2MCMethod Particle2MCMethod.h Kernel/Particle2MCMethod.h
 *
 *  @author Philippe CHARPENTIER
 *  @author Juan PALACIOS
 *  @date   2006-10-11
 */
namespace Particle2MCMethod {

  /**  Enumeration for MC association method
   *
   *  @author Philippe CHARPENTIER
   *  @author Juan PALACIOS
   *  @date   2006-10-11
   */
  enum AssociationMethod { No = 0, WithChi2, Chi2, Links, Composite, ChargedPP, NeutralPP, Max };

  inline const std::array<std::string, Max> extension = {"",           "/WithChi2", "/Chi2", "/Links",
                                                         "/Composite", "/PP2MC",    "/PP2MC"};
  inline const std::array<std::string, Max> algType   = {"",
                                                       "Particle2MCWithChi2",
                                                       "Particle2MCChi2",
                                                       "Particle2MCLinks",
                                                       "CompositeParticle2MCLinks",
                                                       "ChargedPP2MC",
                                                       "NeutralPP2MC"};

} // namespace Particle2MCMethod
#endif // KERNEL_PARTICLE2MCMETHOD_H
