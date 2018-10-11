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
/** @file LHCbConstants.h
 *
 *  Constants of LHCb general interest
 *
 *  @author Marco Cattaneo
 *  @date   2004-03-26
 */

#ifndef KERNEL_LHCBCONSTANTS_H
#define KERNEL_LHCBCONSTANTS_H 1

// Include files
#include "GaudiKernel/PhysicalConstants.h"

/**  Namespace for constants of LHCb general interest
 *
 *  @author Marco Cattaneo
 *  @date   2004-03-26
 */
namespace LHCbConstants
{

  // Silicon parameters, used by ST silicon simulation (M.Needham)
  static const double SiExcitationEnergy = 174*Gaudi::Units::eV; ///< SiExcitationEnergy
  static const double SiEnergyPerIonPair = 3.6*Gaudi::Units::eV; ///< SiEnergyPerIonPair

  // Description of Beetle chip (M.Needham)
  static const unsigned int nStripsInPort = 32;     ///< nStripsInPort
  static const unsigned int nStripsInBeetle = 128;  ///< nStripsInBeetle

}

#endif // KERNEL_LHCBCONSTANTS_H
