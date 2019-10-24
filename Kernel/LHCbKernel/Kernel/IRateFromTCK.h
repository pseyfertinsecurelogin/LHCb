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
#ifndef KERNEL_IRATEFROMTCK_H
#define KERNEL_IRATEFROMTCK_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

/** @class IRateFromTCK IRateFromTCK.h Kernel/IRateFromTCK.h
 *
 *  Interface for tools that get a rate from a TCK
 *
 *  @author Patrick Koppenburg
 *  @date   2011-08-10
 */
struct IRateFromTCK : extend_interfaces<IAlgTool> {

  // Return the interface ID
  DeclareInterfaceID( IRateFromTCK, 3, 0 );

  [[nodiscard]] virtual unsigned int getTCK() const = 0; ///< retrieve TCK

  /// Get rate of rate limiting algorithm given its instance name (See TCKsh)
  [[nodiscard]] virtual double rateFromTCK( const std::string& instanceName ) const = 0;

  /// Get prescale of prescaler algorithm given its instance name (See TCKsh)
  [[nodiscard]] virtual double prescaleFromTCK( const std::string& instanceName ) const = 0;

  /// Update
  virtual StatusCode runUpdate() = 0;
};

#endif // KERNEL_IRATEFROMTCK_H
