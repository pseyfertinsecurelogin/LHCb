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
#ifndef LOKI_GENHYBRIDENGINE_H
#define LOKI_GENHYBRIDENGINE_H 1
// ============================================================================
// Include files
// ============================================================================
#include <string>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/IGenHybridTool.h"
// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  namespace Hybrid {
    // ========================================================================
    /** @class GenEngine GenHybridEngine.h LoKi/GenHybridEngine.h
     *
     *  Helper class forn implementation of Hybrid Tools
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
    class GenEngine final {
    public:
      // ======================================================================
      /// constructor
      GenEngine() = default;
      // ======================================================================
    public:
      // ======================================================================
      // predicates
      // ======================================================================
      /// add the cut
      StatusCode process( const std::string& name, const LoKi::Types::GCuts& cut ) const;
      /// add the cut
      StatusCode process( const std::string& name, const LoKi::Types::GVCuts& cut ) const;
      // ======================================================================
    public:
      // ======================================================================
      // functions
      // ======================================================================
      /// add the function
      StatusCode process( const std::string& name, const LoKi::Types::GFunc& func ) const;
      /// add the function
      StatusCode process( const std::string& name, const LoKi::Types::GVFunc& func ) const;
      // ======================================================================
    public:
      // ======================================================================
      // maps
      // ======================================================================
      /// add the map
      StatusCode process( const std::string& name, const LoKi::Types::GMaps& func ) const;
      /// add the map
      StatusCode process( const std::string& name, const LoKi::Types::GVMaps& func ) const;
      // ======================================================================
    public:
      // ======================================================================
      // pipes
      // ======================================================================
      /// add the pipe
      StatusCode process( const std::string& name, const LoKi::Types::GPipes& func ) const;
      /// add the pipe
      StatusCode process( const std::string& name, const LoKi::Types::GVPipes& func ) const;
      // ======================================================================
    public:
      // ======================================================================
      // fun-vals
      // ======================================================================
      /// add the fun-val
      StatusCode process( const std::string& name, const LoKi::Types::GFunVals& func ) const;
      /// add the fun-val
      StatusCode process( const std::string& name, const LoKi::Types::GVFunVals& func ) const;
      // ======================================================================
    public:
      // ======================================================================
      // cut-vals
      // ======================================================================
      /// add the fun-val
      StatusCode process( const std::string& name, const LoKi::Types::GCutVals& func ) const;
      // ======================================================================
    public:
      // ======================================================================
      // sources
      // ======================================================================
      /// add the source
      StatusCode process( const std::string& name, const LoKi::Types::GSources& func ) const;
      /// add the source
      StatusCode process( const std::string& name, const LoKi::Types::GVSources& func ) const;
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
