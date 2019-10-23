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
#ifndef LOKI_COREENGINEACTOR_H
#define LOKI_COREENGINEACTOR_H 1
// ============================================================================
// Include files
// ============================================================================
// STD&STL
// ============================================================================
#include <stack>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Context.h"
#include "LoKi/ICoreAntiFactory.h"
#include "LoKi/Interface.h"
// ============================================================================
/** @file LoKi/CoreEngineActor.h
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 */
namespace LoKi {
  namespace Hybrid {
    // ========================================================================
    /** @class CoreEngineActor LoKi/CoreEngine.h
     *
     *  Helper class for implementation of C++/Python "Hybrid" solution
     *
     *  This file is a part of LoKi project -
     *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
     *
     *  The package has been designed with the kind help from
     *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
     *  contributions and advices from G.Raven, J.van Tilburg,
     *  A.Golutvin, P.Koppenburg have been used in the
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date   2008-09-18
     */
    class CoreEngineActor {
    public:
      // ======================================================================
      // get the static instance
      static CoreEngineActor& instance();
      /// connect the hybrid tool for code translation
      StatusCode connect( const LoKi::Hybrid::ICoreAntiFactory* factory, const LoKi::Context& context );
      /// disconnect the tool
      StatusCode disconnect( const LoKi::Hybrid::ICoreAntiFactory* factory );
      /** get the current context
       *  contex is valid only inbetween <code>connect/disconnect</code>
       *  @return the current active context
       */
      const LoKi::Context* context() const;
      // ======================================================================
    public: // "void -> ... "
      // ======================================================================
      /// propagate the cut to the tool
      StatusCode process( const std::string& name, const LoKi::Types::FCuts& cut ) const;
      /// propagate the cut to the tool
      StatusCode process( const std::string& name, const LoKi::Types::FFunc& cut ) const;
      // ======================================================================
    public: // "double -> ... "
      // ======================================================================
      /// propagate the cut to the tool
      StatusCode process( const std::string& name, const LoKi::Types::XCuts& cut ) const;
      /// propagate the cut to the tool
      StatusCode process( const std::string& name, const LoKi::Types::XFunc& cut ) const;
      // ======================================================================
    public: // some functional stuff
      // ======================================================================
      /// propagate the cut to the tool
      StatusCode process( const std::string& name, const LoKi::Types::XMaps& cut ) const;
      /// propagate the cut to the tool
      StatusCode process( const std::string& name, const LoKi::Types::XFunVals& cut ) const;
      // ======================================================================
      /// propagate the cut to the tool
      StatusCode process( const std::string& name, const LoKi::Types::XSources& cut ) const;
      // ======================================================================
    protected:
      // ======================================================================
      /// Standard constructor
      CoreEngineActor();
      /// virtual & protected destrucutor
      virtual ~CoreEngineActor(); // virtual & protected destructor
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
      CoreEngineActor( const CoreEngineActor& ) = delete;
      // the assignement operator is disabled
      CoreEngineActor& operator=( const CoreEngineActor& ) = delete;
      // ======================================================================
    private:
      // ======================================================================
      using Tool = LoKi::Interface<LoKi::Hybrid::ICoreAntiFactory>;
      typedef std::pair<Tool, LoKi::Context> Entry;
      using Stack = std::stack<Entry>;
      ///  the stack of active factories
      Stack m_stack{}; // the stack of active factories
      // ======================================================================
    };
    // ========================================================================
  } // namespace Hybrid
} // namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_COREENGINEACTOR_H
