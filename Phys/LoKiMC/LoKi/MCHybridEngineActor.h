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
// ===========================================================================
#ifndef LOKI_MCCUTSHOLDER_H
#define LOKI_MCCUTSHOLDER_H 1
// ===========================================================================
// Include files
// ===========================================================================
// STD&STL
// ===========================================================================
#include <stack>
// ===========================================================================
// LoKi
// ===========================================================================
#include "LoKi/Context.h"
#include "LoKi/IMCHybridTool.h"
#include "LoKi/Interface.h"
// ===========================================================================
namespace LoKi {
  // ==========================================================================
  namespace Hybrid {
    // ========================================================================
    /** @class MCEngineActor MCHybridEngineActor.h LoKi/MCHybridEngineActor.h
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
    class GAUDI_API MCEngineActor {
    public:
      // ======================================================================
      // get the static instance
      static MCEngineActor& instance();
      /// connect the hybrid tool for code translation
      StatusCode connect( const LoKi::IMCHybridTool* tool, const LoKi::Context& context );
      /// disconnect the tool
      StatusCode disconnect( const LoKi::IMCHybridTool* tool );
      // ======================================================================
      /** get the current context
       *  contex is valid only in between <code>connect/disconnect</code>
       *  @return the current active context
       */
      const LoKi::Context* context() const;
      // ======================================================================
    public:
      // ======================================================================
      // predicates:
      // ======================================================================
      /// propagate the cut to the tool
      StatusCode process( const std::string& name, const LoKi::Types::MCCuts& cut ) const;
      /// propagate the cut to the tool
      StatusCode process( const std::string& name, const LoKi::Types::MCVCuts& cut ) const;
      // ======================================================================
    public:
      // ======================================================================
      // functions:
      // ======================================================================
      /// propagate the function to the tool
      StatusCode process( const std::string& name, const LoKi::Types::MCFunc& func ) const;
      /// propagate the function to the tool
      StatusCode process( const std::string& name, const LoKi::Types::MCVFunc& func ) const;
      // ======================================================================
    public:
      // ======================================================================
      // maps:
      // ======================================================================
      /// propagate the map to the tool
      StatusCode process( const std::string& name, const LoKi::Types::MCMaps& func ) const;
      /// propagate the map to the tool
      StatusCode process( const std::string& name, const LoKi::Types::MCVMaps& func ) const;
      // ======================================================================
    public:
      // ======================================================================
      // pipes:
      // ======================================================================
      /// propagate the pipe to the tool
      StatusCode process( const std::string& name, const LoKi::Types::MCPipes& func ) const;
      /// propagate the pipe to the tool
      StatusCode process( const std::string& name, const LoKi::Types::MCVPipes& func ) const;
      // ======================================================================
    public:
      // ======================================================================
      // fun-vals:
      // ======================================================================
      /// propagate the funval to the tool
      StatusCode process( const std::string& name, const LoKi::Types::MCFunVals& func ) const;
      /// propagate the funval to the tool
      StatusCode process( const std::string& name, const LoKi::Types::MCVFunVals& func ) const;
      // ======================================================================
    public:
      // ======================================================================
      // cut-vals:
      // ======================================================================
      /// propagate the cutval to the tool
      StatusCode process( const std::string& name, const LoKi::Types::MCCutVals& func ) const;
      /// propagate the cutval to the tool
      StatusCode process( const std::string& name, const LoKi::Types::MCVCutVals& func ) const;
      // ======================================================================
    public:
      // ======================================================================
      // sources:
      // ======================================================================
      /// propagate the source to the tool
      StatusCode process( const std::string& name, const LoKi::Types::MCSources& func ) const;
      /// propagate the source to the tool
      StatusCode process( const std::string& name, const LoKi::Types::MCVSources& func ) const;
      // ======================================================================
    protected:
      // ======================================================================
      /// Standard constructor
      MCEngineActor() = default;
      // ======================================================================
    private:
      // ======================================================================
      // just to save some lines
      template <class TYPE>
      inline StatusCode _add( const std::string& name, const TYPE& cut ) const;
      // ======================================================================
    private:
      // ======================================================================
      // the copy contructor is disabled
      MCEngineActor( const MCEngineActor& );
      // ther assignement operator is disabled
      MCEngineActor& operator=( const MCEngineActor& );
      // ======================================================================
    private:
      // ======================================================================
      typedef LoKi::Interface<LoKi::IMCHybridTool> Tool;
      typedef std::pair<Tool, LoKi::Context>       Entry;
      typedef std::stack<Entry>                    Stack;
      ///  the stack of active factories
      Stack m_stack{}; // the stack of active factories
      // ======================================================================
    };
    // ========================================================================
  } // namespace Hybrid
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_MCCUTSHOLDER_H
