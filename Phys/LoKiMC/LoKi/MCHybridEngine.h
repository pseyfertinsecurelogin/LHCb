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
#ifndef LOKI_MCHYBRIDENGINE_H
#define LOKI_MCHYBRIDENGINE_H 1
// ============================================================================
// Include files
// ============================================================================
#include <string>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Context.h"
#include "LoKi/IMCHybridTool.h"
// ============================================================================
namespace LoKi {
  // ==========================================================================
  namespace Hybrid {
    // =======================================================================-
    /** @class MCEngine MCHybridEngine.h LoKi/MCHybridEngine.h
     *
     *  Helper class for the implementation of Hybrid Tools
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
     *  @date   2004-06-30
     */
    class GAUDI_API MCEngine {
    public:
      // ======================================================================
      // predicates:
      // ======================================================================
      /// add the cut
      StatusCode process( const std::string& name, const LoKi::Types::MCCuts& cut ) const;
      /// add the cut
      StatusCode process( const std::string& name, const LoKi::Types::MCVCuts& cut ) const;
      // ======================================================================
    public:
      // ======================================================================
      // functions:
      // ======================================================================
      /// add the function
      StatusCode process( const std::string& name, const LoKi::Types::MCFunc& func ) const;
      /// add the function
      StatusCode process( const std::string& name, const LoKi::Types::MCVFunc& func ) const;
      // ======================================================================
    public:
      // ======================================================================
      // maps:
      // ======================================================================
      /// add the map
      StatusCode process( const std::string& name, const LoKi::Types::MCMaps& func ) const;
      /// add the map
      StatusCode process( const std::string& name, const LoKi::Types::MCVMaps& func ) const;
      // ======================================================================
    public:
      // ======================================================================
      // pipes:
      // ======================================================================
      /// add the pipe
      StatusCode process( const std::string& name, const LoKi::Types::MCPipes& func ) const;
      /// add the pipe
      StatusCode process( const std::string& name, const LoKi::Types::MCVPipes& func ) const;
      // ======================================================================
    public:
      // ======================================================================
      // fun-vals:
      // ======================================================================
      /// add the fun-val
      StatusCode process( const std::string& name, const LoKi::Types::MCFunVals& func ) const;
      /// add the fun-val
      StatusCode process( const std::string& name, const LoKi::Types::MCVFunVals& func ) const;
      // ======================================================================
    public:
      // ======================================================================
      // cut-vals:
      // ======================================================================
      /// add the cut-val
      StatusCode process( const std::string& name, const LoKi::Types::MCCutVals& func ) const;
      // ======================================================================
      /// add the cut-val
      StatusCode process( const std::string& name, const LoKi::Types::MCVCutVals& func ) const;
      // ======================================================================
    public:
      // ======================================================================
      // sources
      // ======================================================================
      /// add the source
      StatusCode process( const std::string& name, const LoKi::Types::MCSources& func ) const;
      /// add the source
      StatusCode process( const std::string& name, const LoKi::Types::MCVSources& func ) const;
      // ======================================================================
    public:
      // ======================================================================
      /// get the current context
      const LoKi::Context* context() const;
      // ======================================================================
    };
    // ========================================================================
  } // namespace Hybrid
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_CUTSHOLDERHELPER_H
