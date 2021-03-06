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
#ifndef LOKI_IMCHYBRIDTOOL_H
#define LOKI_IMCHYBRIDTOOL_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <string>
// ============================================================================
// from Gaudi
// ============================================================================
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/StatusCode.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/MCTypes.h"
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @class IMCHybridTool IMCHybridTool.h LoKi/IMCHybridTool.h
   *
   *  Helper interface for implementation of C++/Python "Hybrid" solution
   *
   *  This file is a part of LoKi project -
   *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
   *
   *  The package has been designed with the kind help from
   *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
   *  contributions and advices from G.Raven, J.van Tilburg,
   *  A.Golutvin, P.Koppenburg have been used in the design.
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2004-06-29
   *
   */
  struct GAUDI_API IMCHybridTool : extend_interfaces<IAlgTool> {
    // ========================================================================
    DeclareInterfaceID( IMCHybridTool, 6, 0 );
    // ========================================================================
    // predicates:
    // ========================================================================
    /// set the C++ predicate for LHCb::MCParticle
    virtual void set( const LoKi::Types::MCCuts& cut ) = 0;
    /// set the C++ predicate for LHCb::MCVertex
    virtual void set( const LoKi::Types::MCVCuts& cut ) = 0;
    // ========================================================================
    // functions
    // ========================================================================
    /// set the C++ function for LHCb::MCParticle
    virtual void set( const LoKi::Types::MCFunc& cut ) = 0;
    /// set the C++ function for LHCb::MCVertex
    virtual void set( const LoKi::Types::MCVFunc& cut ) = 0;
    // ========================================================================
    // maps:
    // ========================================================================
    /// set the C++ map for LHCb::MCParticle
    virtual void set( const LoKi::Types::MCMaps& cut ) = 0;
    /// set the C++ map for LHCb::MCVertex
    virtual void set( const LoKi::Types::MCVMaps& cut ) = 0;
    // ========================================================================
    // pipes:
    // ========================================================================
    /// set the C++ pipe for LHCb::MCParticle
    virtual void set( const LoKi::Types::MCPipes& cut ) = 0;
    /// set the C++ pipe for LHCb::MCVertex
    virtual void set( const LoKi::Types::MCVPipes& cut ) = 0;
    // ========================================================================
    // fun-vals:
    // ========================================================================
    /// set the C++ fun-val for LHCb::MCParticle
    virtual void set( const LoKi::Types::MCFunVals& cut ) = 0;
    /// set the C++ fun-val for LHCb::MCVertex
    virtual void set( const LoKi::Types::MCVFunVals& cut ) = 0;
    // ========================================================================
    // cut-vals:
    // ========================================================================
    /// set the C++ cut-val for LHCb::MCParticle
    virtual void set( const LoKi::Types::MCCutVals& cut ) = 0;
    /// set the C++ cut-val for LHCb::MCVertex
    virtual void set( const LoKi::Types::MCVCutVals& cut ) = 0;
    // ========================================================================
    // sources
    // ========================================================================
    /// set the C++ source for LHCb::MCParticle
    virtual void set( const LoKi::Types::MCSources& cut ) = 0;
    /// set the C++ source for LHCb::MCVertex
    virtual void set( const LoKi::Types::MCVSources& cut ) = 0;
    // ========================================================================
  };
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_IMCHYBRIDTOOL_H
