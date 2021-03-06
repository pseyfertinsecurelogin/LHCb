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
#ifndef TRACKEVENT_STATEPARAMETERS_H
#define TRACKEVENT_STATEPARAMETERS_H 1

// Include files
#include "GaudiKernel/SystemOfUnits.h"

/** @namespace StateParameters
 *
 * This namespace defines some useful variables for the
 * manipulation of States.
 *
 *  @author Eduardo Rodrigues
 *  @date   2006-08-02
 *
 */

namespace StateParameters {
  /// Pre-defined z-positions of states for certain State locations
  /// (see Location enum in State.h)
  constexpr double ZBegRich1 = 990. * Gaudi::Units::mm;
  constexpr double ZEndRich1 = 2165. * Gaudi::Units::mm;
  constexpr double ZEndT     = 9410. * Gaudi::Units::mm;
  constexpr double ZBegRich2 = 9450. * Gaudi::Units::mm;
  constexpr double ZEndRich2 = 11900. * Gaudi::Units::mm;
  constexpr double ZBegT     = 7500. * Gaudi::Units::mm;
  constexpr double ZMidT     = 8520. * Gaudi::Units::mm;
  constexpr double ZMidTT    = 2485. * Gaudi::Units::mm;
  constexpr double ZEndTT    = 2700. * Gaudi::Units::mm;
  constexpr double ZEndVelo  = 770. * Gaudi::Units::mm;
} // namespace StateParameters

// ----------------------------------------------------------------------------
//   end of namespace
// ----------------------------------------------------------------------------

#endif /// TRACKEVENT_STATEPARAMETERS_H
