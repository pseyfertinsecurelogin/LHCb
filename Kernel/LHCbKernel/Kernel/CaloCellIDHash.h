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
// ============================================================================
#ifndef KERNEL_CALOCELLIDHASH_H
#define KERNEL_CALOCELLIDHASH_H 1
// ============================================================================
// Include files
// ============================================================================
// LHCbKernel
// ============================================================================
#include "Kernel/CaloCellID.h"
// ============================================================================
/** @file Kernel/CaloCellIDHash.h
 *
 *  Hash -function for CaloCellID to be used e.g. with
 *   GaudiUtils::HashMap and  GaudiUtils::Hash
 *
 *  @see GaudiUtils::HashMap
 *  @see GaudiUtils::Hash
 *  @author Vanya Belyaev Ivan.Belyaev@nikhef.nl
 *  @date   2009-07-30
 */
// ============================================================================
namespace LHCb
{
  // ==========================================================================
  /** the generic hash-function (in the spirit of boost/hash and TR1)
   *
   *  This function is needed to enable the usage of CaloCellID ad a key for
   *  GaudiUtils::HashMap and GaudiUtils::Hash
   *
   *  @see GaudiUtils::HashMap
   *  @see GaudiUtils::Hash
   *  @author Vanya Belyaev Ivan.Belyaev@nikhef.nl
   *  @date   2009-07-30
   */
  inline std::size_t hash_value  ( const LHCb::CaloCellID& id )
  { return id.hash () ; }
  // ==========================================================================
} //                                                      end of namespace LHCb
// ============================================================================
// The END
// ============================================================================
#endif // KERNEL_CALOCELLIDHASH_H
// ============================================================================
