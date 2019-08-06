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

#pragma once

// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IAlgTool.h"
// ============================================================================
// forward declarations
// ============================================================================
namespace LHCb::Event {
  inline namespace v1 {
    class Track;
  }
} // namespace LHCb::Event
// ============================================================================

// ============================================================================
/** @class IInAcceptance IInAcceptance.h Kernel/IInAcceptance.h
 *
 *  An abstract interface for implementation "acceptance" tools.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@lapp.in2p3.fr
 *  @date   2005-07-22
 */
// ============================================================================
struct IInAcceptance : extend_interfaces<IAlgTool> {
  DeclareInterfaceID( IInAcceptance, 2, 0 );

  /** check the track is in acceptance of given "detector"
   *  @param track track to be checked
   *  @return true if the track is in acceptance
   */
  virtual bool inAcceptance( const LHCb::Event::v1::Track* track ) const = 0;
};

// ============================================================================
// The END
// ============================================================================
